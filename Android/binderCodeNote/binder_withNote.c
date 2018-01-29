/* binder.c
 *
 * Android IPC Subsystem
 *
 * Copyright (C) 2007-2008 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <asm/cacheflush.h>
#include <linux/fdtable.h>
#include <linux/file.h>
#include <linux/freezer.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/nsproxy.h>
#include <linux/poll.h>
#include <linux/debugfs.h>
#include <linux/rbtree.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/pid_namespace.h>
#include <linux/security.h>

#ifdef CONFIG_ANDROID_BINDER_IPC_32BIT
#define BINDER_IPC_32BIT 1
#endif

#include <uapi/linux/android/binder.h>
#include "binder_trace.h"

static DEFINE_MUTEX(binder_main_lock);
static DEFINE_MUTEX(binder_deferred_lock);
static DEFINE_MUTEX(binder_mmap_lock);

static HLIST_HEAD(binder_procs);
static HLIST_HEAD(binder_deferred_list);
static HLIST_HEAD(binder_dead_nodes);

static struct dentry *binder_debugfs_dir_entry_root;
static struct dentry *binder_debugfs_dir_entry_proc;
static struct binder_node *binder_context_mgr_node;
static kuid_t binder_context_mgr_uid = INVALID_UID;
static int binder_last_id;
static struct workqueue_struct *binder_deferred_workqueue;

#define BINDER_DEBUG_ENTRY(name) \
static int binder_##name##_open(struct inode *inode, struct file *file) \
{ \
    return single_open(file, binder_##name##_show, inode->i_private); \
} \
\
static const struct file_operations binder_##name##_fops = { \
    .owner = THIS_MODULE, \
    .open = binder_##name##_open, \
    .read = seq_read, \
    .llseek = seq_lseek, \
    .release = single_release, \
}

static int binder_proc_show(struct seq_file *m, void *unused);
BINDER_DEBUG_ENTRY(proc);

/* This is only defined in include/asm-arm/sizes.h */
#ifndef SZ_1K
#define SZ_1K                               0x400
#endif

#ifndef SZ_4M
#define SZ_4M                               0x400000
#endif

#define FORBIDDEN_MMAP_FLAGS                (VM_WRITE)

#define BINDER_SMALL_BUF_SIZE (PAGE_SIZE * 64)

enum {
    BINDER_DEBUG_USER_ERROR             = 1U << 0,
    BINDER_DEBUG_FAILED_TRANSACTION     = 1U << 1,
    BINDER_DEBUG_DEAD_TRANSACTION       = 1U << 2,
    BINDER_DEBUG_OPEN_CLOSE             = 1U << 3,
    BINDER_DEBUG_DEAD_BINDER            = 1U << 4,
    BINDER_DEBUG_DEATH_NOTIFICATION     = 1U << 5,
    BINDER_DEBUG_READ_WRITE             = 1U << 6,
    BINDER_DEBUG_USER_REFS              = 1U << 7,
    BINDER_DEBUG_THREADS                = 1U << 8,
    BINDER_DEBUG_TRANSACTION            = 1U << 9,
    BINDER_DEBUG_TRANSACTION_COMPLETE   = 1U << 10,
    BINDER_DEBUG_FREE_BUFFER            = 1U << 11,
    BINDER_DEBUG_INTERNAL_REFS          = 1U << 12,
    BINDER_DEBUG_BUFFER_ALLOC           = 1U << 13,
    BINDER_DEBUG_PRIORITY_CAP           = 1U << 14,
    BINDER_DEBUG_BUFFER_ALLOC_ASYNC     = 1U << 15,
};
static uint32_t binder_debug_mask = BINDER_DEBUG_USER_ERROR |
    BINDER_DEBUG_FAILED_TRANSACTION | BINDER_DEBUG_DEAD_TRANSACTION;
module_param_named(debug_mask, binder_debug_mask, uint, S_IWUSR | S_IRUGO);

static bool binder_debug_no_lock;
module_param_named(proc_no_lock, binder_debug_no_lock, bool, S_IWUSR | S_IRUGO);

static DECLARE_WAIT_QUEUE_HEAD(binder_user_error_wait);
static int binder_stop_on_user_error;

static int binder_set_stop_on_user_error(const char *val,
                     struct kernel_param *kp)
{
    int ret;

    ret = param_set_int(val, kp);
    if (binder_stop_on_user_error < 2)
        wake_up(&binder_user_error_wait);
    return ret;
}
module_param_call(stop_on_user_error, binder_set_stop_on_user_error,
    param_get_int, &binder_stop_on_user_error, S_IWUSR | S_IRUGO);

#define binder_debug(mask, x...) \
    do { \
        if (binder_debug_mask & mask) \
            pr_info(x); \
    } while (0)

#define binder_user_error(x...) \
    do { \
        if (binder_debug_mask & BINDER_DEBUG_USER_ERROR) \
            pr_info(x); \
        if (binder_stop_on_user_error) \
            binder_stop_on_user_error = 2; \
    } while (0)

enum binder_stat_types {
    BINDER_STAT_PROC,
    BINDER_STAT_THREAD,
    BINDER_STAT_NODE,
    BINDER_STAT_REF,
    BINDER_STAT_DEATH,
    BINDER_STAT_TRANSACTION,
    BINDER_STAT_TRANSACTION_COMPLETE,
    BINDER_STAT_COUNT
};

struct binder_stats {
    int br[_IOC_NR(BR_FAILED_REPLY) + 1];
    int bc[_IOC_NR(BC_DEAD_BINDER_DONE) + 1];
    int obj_created[BINDER_STAT_COUNT];
    int obj_deleted[BINDER_STAT_COUNT];
};

static struct binder_stats binder_stats;

static inline void binder_stats_deleted(enum binder_stat_types type)
{
    binder_stats.obj_deleted[type]++;
}

static inline void binder_stats_created(enum binder_stat_types type)
{
    binder_stats.obj_created[type]++;
}

struct binder_transaction_log_entry {
    int debug_id;
    int call_type;
    int from_proc;
    int from_thread;
    int target_handle;
    int to_proc;
    int to_thread;
    int to_node;
    int data_size;
    int offsets_size;
};
struct binder_transaction_log {
    int next;
    int full;
    struct binder_transaction_log_entry entry[32];
};
static struct binder_transaction_log binder_transaction_log;
static struct binder_transaction_log binder_transaction_log_failed;

static struct binder_transaction_log_entry *binder_transaction_log_add(
    struct binder_transaction_log *log)
{
    struct binder_transaction_log_entry *e;

    e = &log->entry[log->next];
    memset(e, 0, sizeof(*e));
    log->next++;
    if (log->next == ARRAY_SIZE(log->entry)) {
        log->next = 0;
        log->full = 1;
    }
    return e;
}

struct binder_work {
    struct list_head entry;
    enum {
        BINDER_WORK_TRANSACTION = 1,
        BINDER_WORK_TRANSACTION_COMPLETE,
        BINDER_WORK_NODE,
        BINDER_WORK_DEAD_BINDER,
        BINDER_WORK_DEAD_BINDER_AND_CLEAR,
        BINDER_WORK_CLEAR_DEATH_NOTIFICATION,
    } type;
};

struct binder_node {
    int debug_id;
    struct binder_work work;
    union {
        struct rb_node rb_node;
        struct hlist_node dead_node;
    };
    struct binder_proc *proc;
    struct hlist_head refs;
    int internal_strong_refs;
    int local_weak_refs;
    int local_strong_refs;
    binder_uintptr_t ptr;
    binder_uintptr_t cookie;
    unsigned has_strong_ref:1;
    unsigned pending_strong_ref:1;
    unsigned has_weak_ref:1;
    unsigned pending_weak_ref:1;
    unsigned has_async_transaction:1;
    unsigned accept_fds:1;
    unsigned min_priority:8;
    struct list_head async_todo;
};

/*
  只要某进程对某binder引用订阅了其实体的死亡通知，那么binder驱动将会为该binder引用建立一个通知结构体：binder_ref_death，
  将其保存在当前进程的对应binder引用结构体的death域中。
*/
struct binder_ref_death {
    struct binder_work work;
    binder_uintptr_t cookie;
};

struct binder_ref {
    /* Lookups needed: */
    /*   node + proc => ref (transaction) */
    /*   desc + proc => ref (transaction, inc/dec ref) */
    /*   node => refs + procs (proc exit) */
    int debug_id;
    struct rb_node rb_node_desc;
    struct rb_node rb_node_node;
    struct hlist_node node_entry;
    struct binder_proc *proc;
    struct binder_node *node;
    uint32_t desc;
    int strong;
    int weak;
    struct binder_ref_death *death;
};

struct binder_buffer {
    struct list_head entry; /* free and allocated entries by address */
    struct rb_node rb_node; /* free entry by size or allocated entry */
                /* by address */
    unsigned free:1;
    unsigned allow_user_free:1;
    unsigned async_transaction:1;
    unsigned debug_id:29;

    struct binder_transaction *transaction;

    struct binder_node *target_node;
    size_t data_size;
    size_t offsets_size;
    uint8_t data[0];
};

enum binder_deferred_state {
    BINDER_DEFERRED_PUT_FILES    = 0x01,
    BINDER_DEFERRED_FLUSH        = 0x02,
    BINDER_DEFERRED_RELEASE      = 0x04,
};

struct binder_proc {
    struct hlist_node proc_node;
    struct rb_root threads;
    struct rb_root nodes;
    struct rb_root refs_by_desc;
    struct rb_root refs_by_node;
    int pid;
    struct vm_area_struct *vma;
    struct mm_struct *vma_vm_mm;
    struct task_struct *tsk;
    struct files_struct *files;
    struct hlist_node deferred_work_node;
    int deferred_work;
    void *buffer;
    ptrdiff_t user_buffer_offset;

    struct list_head buffers;
    struct rb_root free_buffers;
    struct rb_root allocated_buffers;
    size_t free_async_space;

    struct page **pages;
    size_t buffer_size;
    uint32_t buffer_free;
    struct list_head todo;
    wait_queue_head_t wait;
    struct binder_stats stats;
    struct list_head delivered_death;
    int max_threads;
    int requested_threads;
    int requested_threads_started;
    int ready_threads;
    long default_priority;
    struct dentry *debugfs_entry;
};

enum {
    BINDER_LOOPER_STATE_REGISTERED  = 0x01,
    BINDER_LOOPER_STATE_ENTERED     = 0x02,
    BINDER_LOOPER_STATE_EXITED      = 0x04,
    BINDER_LOOPER_STATE_INVALID     = 0x08,
    BINDER_LOOPER_STATE_WAITING     = 0x10,
    BINDER_LOOPER_STATE_NEED_RETURN = 0x20
};

struct binder_thread {
    struct binder_proc *proc;
    struct rb_node rb_node;
    int pid;
    int looper;
    struct binder_transaction *transaction_stack;
    struct list_head todo;
    uint32_t return_error; /* Write failed, return error code in read buf */
    uint32_t return_error2; /* Write failed, return error code in read */
        /* buffer. Used when sending a reply to a dead process that */
        /* we are also waiting on */
    wait_queue_head_t wait;
    struct binder_stats stats;
};

struct binder_transaction {
    int debug_id;
    struct binder_work work;
    struct binder_thread *from;
    struct binder_transaction *from_parent;
    struct binder_proc *to_proc;
    struct binder_thread *to_thread;
    struct binder_transaction *to_parent;
    unsigned need_reply:1;
    /* unsigned is_dead:1; */   /* not used at the moment */

    struct binder_buffer *buffer;
    unsigned int    code;
    unsigned int    flags;
    long    priority;
    long    saved_priority;
    kuid_t  sender_euid;
};

static void
binder_defer_work(struct binder_proc *proc, enum binder_deferred_state defer);

static int task_get_unused_fd_flags(struct binder_proc *proc, int flags)
{
    struct files_struct *files = proc->files;
    unsigned long rlim_cur;
    unsigned long irqs;

    if (files == NULL)
        return -ESRCH;

    if (!lock_task_sighand(proc->tsk, &irqs))
        return -EMFILE;

    rlim_cur = task_rlimit(proc->tsk, RLIMIT_NOFILE);
    unlock_task_sighand(proc->tsk, &irqs);

    return __alloc_fd(files, 0, rlim_cur, flags);
}

/*
 * copied from fd_install
 */
static void task_fd_install(
    struct binder_proc *proc, unsigned int fd, struct file *file)
{
    if (proc->files)
        __fd_install(proc->files, fd, file);
}

/*
 * copied from sys_close
 */
static long task_close_fd(struct binder_proc *proc, unsigned int fd)
{
    int retval;

    if (proc->files == NULL)
        return -ESRCH;

    retval = __close_fd(proc->files, fd);
    /* can't restart close syscall because file table entry was cleared */
    if (unlikely(retval == -ERESTARTSYS ||
             retval == -ERESTARTNOINTR ||
             retval == -ERESTARTNOHAND ||
             retval == -ERESTART_RESTARTBLOCK))
        retval = -EINTR;

    return retval;
}

static inline void binder_lock(const char *tag)
{
    trace_binder_lock(tag);
    mutex_lock(&binder_main_lock);
    trace_binder_locked(tag);
}

static inline void binder_unlock(const char *tag)
{
    trace_binder_unlock(tag);
    mutex_unlock(&binder_main_lock);
}

static void binder_set_nice(long nice)
{
    long min_nice;

    if (can_nice(current, nice)) {
        set_user_nice(current, nice);
        return;
    }
    min_nice = rlimit_to_nice(current->signal->rlim[RLIMIT_NICE].rlim_cur);
    binder_debug(BINDER_DEBUG_PRIORITY_CAP,
             "%d: nice value %ld not allowed use %ld instead\n",
              current->pid, nice, min_nice);
    set_user_nice(current, min_nice);
    if (min_nice <= MAX_NICE)
        return;
    binder_user_error("%d RLIMIT_NICE not set\n", current->pid);
}

static size_t binder_buffer_size(struct binder_proc *proc,
                 struct binder_buffer *buffer)
{
    /*
        如果这个binder_buffer是在binder_proc.buffers链表的最后,
        也就是最新加入的,那么这个buffer的size是最大的，因为也把未分配的虚拟空间大小算上了。
    */
    if (list_is_last(&buffer->entry, &proc->buffers))
        return proc->buffer + proc->buffer_size - (void *)buffer->data;

    // 否则就用下一个binder_bufffer的首地址减去这个binder_buffer的data首地址
    return (size_t)list_entry(buffer->entry.next,
              struct binder_buffer, entry) - (size_t)buffer->data;
}

// 以binder_buffer的size为索引将自身挂入当前进程红黑树free_buffers中
static void binder_insert_free_buffer(struct binder_proc *proc,
                      struct binder_buffer *new_buffer)
{
    struct rb_node **p = &proc->free_buffers.rb_node;
    struct rb_node *parent = NULL;
    struct binder_buffer *buffer;
    size_t buffer_size;
    size_t new_buffer_size;

    BUG_ON(!new_buffer->free);

    new_buffer_size = binder_buffer_size(proc, new_buffer);

    binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
             "%d: add free buffer, size %zd, at %p\n",
              proc->pid, new_buffer_size, new_buffer);

     // 查找插入红黑树free_buffers中的最佳位置
    while (*p) {
        parent = *p;
        buffer = rb_entry(parent, struct binder_buffer, rb_node);
        BUG_ON(!buffer->free);   // 异常检测，当前binder_buffer应该是空闲的才对。

        buffer_size = binder_buffer_size(proc, buffer);

        if (new_buffer_size < buffer_size)
            p = &parent->rb_left;
        else
            p = &parent->rb_right;
    }
    rb_link_node(&new_buffer->rb_node, parent, p);
    // 将最新的binder_buffer插入红黑树free_buffers
    rb_insert_color(&new_buffer->rb_node, &proc->free_buffers);
}

static void binder_insert_allocated_buffer(struct binder_proc *proc,
                       struct binder_buffer *new_buffer)
{
    struct rb_node **p = &proc->allocated_buffers.rb_node;
    struct rb_node *parent = NULL;
    struct binder_buffer *buffer;

    BUG_ON(new_buffer->free);

    while (*p) {
        parent = *p;
        buffer = rb_entry(parent, struct binder_buffer, rb_node);
        BUG_ON(buffer->free);

        if (new_buffer < buffer)
            p = &parent->rb_left;
        else if (new_buffer > buffer)
            p = &parent->rb_right;
        else
            BUG();
    }
    rb_link_node(&new_buffer->rb_node, parent, p);
    rb_insert_color(&new_buffer->rb_node, &proc->allocated_buffers);
}

static struct binder_buffer *binder_buffer_lookup(struct binder_proc *proc,
                          uintptr_t user_ptr)
{
    // 对于已经分配的buffer空间，以内存地址为索引加入红黑树allocated_buffers中
    struct rb_node *n = proc->allocated_buffers.rb_node;
    struct binder_buffer *buffer;
    struct binder_buffer *kern_ptr;

    /*
        进程ioctl传下来的指针并不是binder_buffer的地址，而直接是binder_buffer.data的地址。
        user_buffer_offset用户空间和内核空间，被映射区域起始地址之间的偏移
    */
    kern_ptr = (struct binder_buffer *)(user_ptr - proc->user_buffer_offset
        - offsetof(struct binder_buffer, data));

    while (n) {
        buffer = rb_entry(n, struct binder_buffer, rb_node);
        BUG_ON(buffer->free);

        if (kern_ptr < buffer)
            n = n->rb_left;
        else if (kern_ptr > buffer)
            n = n->rb_right;
        else
            return buffer;
    }
    return NULL;
}

static int binder_update_page_range(struct binder_proc *proc, int allocate,
                    void *start, void *end,
                    struct vm_area_struct *vma)
{
    void *page_addr;
    unsigned long user_page_addr;
    struct page **page;
    struct mm_struct *mm;

    binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
             "%d: %s pages %p-%p\n", proc->pid,
             allocate ? "allocate" : "free", start, end);

    if (end <= start)
        return 0;

    trace_binder_update_page_range(proc, allocate, start, end);

    if (vma)
        mm = NULL;
    else
        mm = get_task_mm(proc->tsk);

    if (mm) {
        down_write(&mm->mmap_sem);
        vma = proc->vma;
        if (vma && mm != proc->vma_vm_mm) {
            pr_err("%d: vma mm and task mm mismatch\n",
                proc->pid);
            vma = NULL;
        }
    }

    // 释放特定范围的内存
    if (allocate == 0)
        goto free_range;

    if (vma == NULL) {
        pr_err("%d: binder_alloc_buf failed to map pages in userspace, no vma\n",
            proc->pid);
        goto err_no_vma;
    }

    for (page_addr = start; page_addr < end; page_addr += PAGE_SIZE) {
        int ret;

        // 得到当前page_addr在binder_proc.pages中的数组项地址
        page = &proc->pages[(page_addr - proc->buffer) / PAGE_SIZE];

        BUG_ON(*page);  // 异常检测
        *page = alloc_page(GFP_KERNEL | __GFP_HIGHMEM | __GFP_ZERO);  // 分配一个物理清0页，返回struct page结构体指针
        if (*page == NULL) {
            pr_err("%d: binder_alloc_buf failed for page at %p\n",
                proc->pid, page_addr);
            goto err_alloc_page_failed;
        }
        ret = map_kernel_range_noflush((unsigned long)page_addr,
                    PAGE_SIZE, PAGE_KERNEL, page);  //  将刚刚分配的物理页映射到内核虚拟地址page_addr上。
        flush_cache_vmap((unsigned long)page_addr,
                (unsigned long)page_addr + PAGE_SIZE);
        if (ret != 1) {
            pr_err("%d: binder_alloc_buf failed to map page at %p in kernel\n",
                   proc->pid, page_addr);
            goto err_map_kernel_failed;
        }
        user_page_addr =
            (uintptr_t)1 + proc->user_buffer_offset;  // 内核虚拟地址段中地址page_addr在用户空间虚拟地址段中对应的虚拟地址
        ret = vm_insert_page(vma, user_page_addr, page[0]);  // 将刚刚分配的物理页插入到上面计算出的用户空间对应的虚拟地址上。
        if (ret) {
            pr_err("%d: binder_alloc_buf failed to map page at %lx in userspace\n",
                   proc->pid, user_page_addr);
            goto err_vm_insert_page_failed;
        }
        /* vm_insert_page does not seem to increment the refcount */
    }
    if (mm) {
        up_write(&mm->mmap_sem);
        mmput(mm);
    }
    return 0;

free_range:
    for (page_addr = end - PAGE_SIZE; page_addr >= start;
         page_addr -= PAGE_SIZE) {
        page = &proc->pages[(page_addr - proc->buffer) / PAGE_SIZE];
        if (vma)
            zap_page_range(vma, (uintptr_t)page_addr +
                proc->user_buffer_offset, PAGE_SIZE, NULL);
err_vm_insert_page_failed:
        unmap_kernel_range((unsigned long)page_addr, PAGE_SIZE);
err_map_kernel_failed:
        __free_page(*page);
        *page = NULL;
err_alloc_page_failed:
        ;
    }
err_no_vma:
    if (mm) {
        up_write(&mm->mmap_sem);
        mmput(mm);
    }
    return -ENOMEM;
}

static struct binder_buffer *binder_alloc_buf(struct binder_proc *proc,
                          size_t data_size,
                          size_t offsets_size, int is_async)
{
    struct rb_node *n = proc->free_buffers.rb_node;
    struct binder_buffer *buffer;
    size_t buffer_size;
    struct rb_node *best_fit = NULL;
    void *has_page_addr;
    void *end_page_addr;
    size_t size;

    if (proc->vma == NULL) {
        pr_err("%d: binder_alloc_buf, no vma\n",
               proc->pid);
        return NULL;
    }

    size = ALIGN(data_size, sizeof(void *)) +
        ALIGN(offsets_size, sizeof(void *));

    if (size < data_size || size < offsets_size) {
        binder_user_error("%d: got transaction with invalid size %zd-%zd\n",
                proc->pid, data_size, offsets_size);
        return NULL;
    }

    /*
        如果是异步传输，同时异步传输的空间不够用将会报错。
        为什么后面还要求多出sizeof(struct binder_buffer)的长度，因为需要为下一个binder_buffer留出物理内存空间*
    */
    if (is_async &&
        proc->free_async_space < size + sizeof(struct binder_buffer)) {
        binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
                 "%d: binder_alloc_buf size %zd failed, no async space left\n",
                  proc->pid, size);
        return NULL;
    }

    while (n) {
        buffer = rb_entry(n, struct binder_buffer, rb_node);
        BUG_ON(!buffer->free);  // 非空闲的binder_buffer挂错了树: free_buffers
        buffer_size = binder_buffer_size(proc, buffer);

        if (size < buffer_size) {
            best_fit = n;
            n = n->rb_left;
        } else if (size > buffer_size)
            n = n->rb_right;
        else {
            best_fit = n;
            break;
        }
    }

    /*
        在红黑树free_buffers上寻找大小最合适的binder_buffer，这里会出现几种情况:

        1.最极端的情况，free_buffers树上已经存在的binder_buffer中，除了最右侧这个包含大量虚拟地址空间的binder_buffer合适之外，
        其余binder_buffer的data大小都比需求的小。此时best_fit等于最右侧的binder_buffer，n = NULL。
        (和这种情况类似，binder_mmap刚刚执行完之后到来的第一个数据传输时，调用该函数后的情形，
        就是这个红黑树上除了这个包含大量虚拟空间的binder_buffer之外，没有其他的binder_buffer。)

        2.常见的情况，所有空闲binder_buffer之中，存在一个binder_buferr,它的data的大小最接近需求的大小(肯定是大于)。
          根据这个相差的字节数，又存在两种情况：
            2.1 差值 >= sizeof(struct binder_buffer) + 4；
            2.2差值 < sizeof(struct binder_buffer) +4，后面的代码会根据这两种情况做不同的处理。
            为什么要多4个字节？是这样的，binder_buffer结构体最后的域data[0]，在用sizeof计算这个结构体占多少内存时本身就没有计算进去，
            而多4个字节呢，是考虑到BC_x或者BR_x协议有不带参数的命令，就一个命令正好占4字节，刚刚好也够一次单边传输之用了。
          这种情形下best_fit为最data大小最接近需求大小的binder_buffer，而n还是为NULL。

        3.常见的情况，恰好找到一个data大小和需求空间大小相等的binder_buffer。
          这种请best_fit就等于这个binder­_buffer，而n不为NULL，等于这个binder­_buffer的rb­_node指针。

    */

    if (best_fit == NULL) {
        pr_err("%d: binder_alloc_buf size %zd failed, no address space\n",
            proc->pid, size);
        return NULL;    //  由上面三种情况，可以看出，如果空间足够，这个best_fit就不可能为NULL
    }

     /*
        前2种情况下，n都为NULL, 这里取出best_fit对应的binder_buffer，并且计算
        出这个binder_buffer的data空间。对于第一种情况，这个buffer_size是比较大的，其中包含了没有做映射的空间；
        而对于第二种情况，这个buffer_size是data所占的实际物理内存大小。
    */
    if (n == NULL) {
        buffer = rb_entry(best_fit, struct binder_buffer, rb_node);
        buffer_size = binder_buffer_size(proc, buffer);
    }

    binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
             "%d: binder_alloc_buf size %zd got buffer %p size %zd\n",
              proc->pid, size, buffer, buffer_size);

    /*
        地址向减小方向页对齐。这里有两种情况：
        1. (uintptr_t)buffer->data + buffer_size已经翻页；
        2.  (uintptr_t)buffer->data + buffer_size 还在buffer->data同一页。
    */
    has_page_addr =
        (void *)(((uintptr_t)buffer->data + buffer_size) & PAGE_MASK);
    if (n == NULL) {
        if (size + sizeof(struct binder_buffer) + 4 >= buffer_size)  // 情况2.2
            buffer_size = size; /* no room for other buffers */
        else                                                        // 情况2.1和第一种情况
            buffer_size = size + sizeof(struct binder_buffer);
    }
    end_page_addr =
        (void *)PAGE_ALIGN((uintptr_t)buffer->data + buffer_size);
    if (end_page_addr > has_page_addr)
        end_page_addr = has_page_addr;

     /*
     binder_update_page_range()在上面三种情况下，所做动作也不同：

     第一种情况：
      end_page_addr远小于has_page_addr，这时end_page_addr没变，
      还是PAGE_ALIGN(buffer->data+ size+ sizeof(struct binder_buffer))，
      只是这个buffer->data+ size+ sizeof(struct binder_buffer)值还会不会和buffer->data处于同一个页面，
      如果是，那么函数binder_update_page_range()中的第3、4个参数就是相等的，这个函数就不会分配物理内存页；
      如果不是处于同一物理页内，也就是翻过页了，具体翻几页都行，这个时候第3个参数表示的是即将分配的第一个物理页的页地址，
      而end_page_addr表示最后一个需求的物理页的页地址。

    第2.1种情况：
     ((uintptr_t)buffer->data + buffer_size) & PAGE_MASK，如果加上buffer_size没有翻页，
     那么这个式子的结果是buffer->data所在页的页地址；如果是翻了n页了，那么这个式子的结果是buffer->data所在页的后n页的页地址。
     如果没有翻页的情况下，PAGE_ALIGN((uintptr_t)buffer->data + buffer_size)会是buffer->data所在页的下一页，
     这样经过上面的if条件的限制，这个函数的第3、4个参数就是一样的了，那么这个函数就不会为其多分配物理页。
     如果在翻页了的情况下，最极端的情况就是buffer->data+ size+ sizeof(struct binder_buffer)和
     前面计算has_page_addr的buffer->data + buffer_size处于同一页内，这个时候算出来的end_page_addr就比has_page_addr大1，
     不过经过那个if的限定，最终调用函数binder_update_page_range()分配的也是翻过的那n个物理页。其余不极端的情况都是很好理解的。

    情况2.2和2.1的情形类似，可以自行分析。

    第三种情况就更加好理解了，因为是找了大小和需求恰好相等的binder_buffer，这个时候算出来的end_page_addr正好比has_page_addr大1，
    由if条件限定，所以binder_update_page_range()函数不会分配物理空间，因为这是之前用过的，物理页已经分配好了。

*/
    if (binder_update_page_range(proc, 1,
        (void *)PAGE_ALIGN((uintptr_t)buffer->data), end_page_addr, NULL))
        return NULL;

    rb_erase(best_fit, &proc->free_buffers);    // 从空闲树上摘除这个binder_buffer
    buffer->free = 0;
    binder_insert_allocated_buffer(proc, buffer);  // 挂入已分配的红黑树上: allocated_buffers

    //  情况2.1和第一种情况，生成一新的binder_buffer挂入空闲树
    if (buffer_size != size) {
        struct binder_buffer *new_buffer = (void *)buffer->data + size;

        list_add(&new_buffer->entry, &buffer->entry);  // 挂入proc->buffers链表
        new_buffer->free = 1;
        binder_insert_free_buffer(proc, new_buffer);  // 将这个新的binder_buffer挂入空闲树
    }
    binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
             "%d: binder_alloc_buf size %zd got %p\n",
              proc->pid, size, buffer);
    buffer->data_size = data_size;
    buffer->offsets_size = offsets_size;
    buffer->async_transaction = is_async;
    if (is_async) {
        proc->free_async_space -= size + sizeof(struct binder_buffer);  // 如果是异步传输，异步空间相应减小
        binder_debug(BINDER_DEBUG_BUFFER_ALLOC_ASYNC,
                 "%d: binder_alloc_buf size %zd async free %zd\n",
                  proc->pid, size, proc->free_async_space);
    }

    return buffer;
}

static void *buffer_start_page(struct binder_buffer *buffer)
{
    return (void *)((uintptr_t)buffer & PAGE_MASK);
}

static void *buffer_end_page(struct binder_buffer *buffer)
{
    return (void *)(((uintptr_t)(buffer + 1) - 1) & PAGE_MASK);
}

static void binder_delete_free_buffer(struct binder_proc *proc,
                      struct binder_buffer *buffer)
{
    struct binder_buffer *prev, *next = NULL;
    int free_page_end = 1;
    int free_page_start = 1;

    BUG_ON(proc->buffers.next == &buffer->entry);
    prev = list_entry(buffer->entry.prev, struct binder_buffer, entry);
    BUG_ON(!prev->free);
    if (buffer_end_page(prev) == buffer_start_page(buffer)) {
        free_page_start = 0;
        if (buffer_end_page(prev) == buffer_end_page(buffer))
            free_page_end = 0;
        binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
                 "%d: merge free, buffer %p share page with %p\n",
                  proc->pid, buffer, prev);
    }

    if (!list_is_last(&buffer->entry, &proc->buffers)) {
        next = list_entry(buffer->entry.next,
                  struct binder_buffer, entry);
        if (buffer_start_page(next) == buffer_end_page(buffer)) {
            free_page_end = 0;
            if (buffer_start_page(next) ==
                buffer_start_page(buffer))
                free_page_start = 0;
            binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
                     "%d: merge free, buffer %p share page with %p\n",
                      proc->pid, buffer, prev);
        }
    }
    list_del(&buffer->entry);
    if (free_page_start || free_page_end) {
        binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
                 "%d: merge free, buffer %p do not share page%s%s with %p or %p\n",
                 proc->pid, buffer, free_page_start ? "" : " end",
                 free_page_end ? "" : " start", prev, next);
        binder_update_page_range(proc, 0, free_page_start ?
            buffer_start_page(buffer) : buffer_end_page(buffer),
            (free_page_end ? buffer_end_page(buffer) :
            buffer_start_page(buffer)) + PAGE_SIZE, NULL);
    }
}

static void binder_free_buf(struct binder_proc *proc,
                struct binder_buffer *buffer)
{
    size_t size, buffer_size;

    buffer_size = binder_buffer_size(proc, buffer);

    size = ALIGN(buffer->data_size, sizeof(void *)) +
        ALIGN(buffer->offsets_size, sizeof(void *));

    binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
             "%d: binder_free_buf %p size %zd buffer_size %zd\n",
              proc->pid, buffer, size, buffer_size);

    BUG_ON(buffer->free);
    BUG_ON(size > buffer_size);
    BUG_ON(buffer->transaction != NULL);
    BUG_ON((void *)buffer < proc->buffer);
    BUG_ON((void *)buffer > proc->buffer + proc->buffer_size);

    if (buffer->async_transaction) {
        proc->free_async_space += size + sizeof(struct binder_buffer);

        binder_debug(BINDER_DEBUG_BUFFER_ALLOC_ASYNC,
                 "%d: binder_free_buf size %zd async free %zd\n",
                  proc->pid, size, proc->free_async_space);
    }

    binder_update_page_range(proc, 0,
        (void *)PAGE_ALIGN((uintptr_t)buffer->data),
        (void *)(((uintptr_t)buffer->data + buffer_size) & PAGE_MASK),
        NULL);
    rb_erase(&buffer->rb_node, &proc->allocated_buffers);
    buffer->free = 1;
    if (!list_is_last(&buffer->entry, &proc->buffers)) {
        struct binder_buffer *next = list_entry(buffer->entry.next,
                        struct binder_buffer, entry);

        if (next->free) {
            rb_erase(&next->rb_node, &proc->free_buffers);
            binder_delete_free_buffer(proc, next);
        }
    }
    if (proc->buffers.next != &buffer->entry) {
        struct binder_buffer *prev = list_entry(buffer->entry.prev,
                        struct binder_buffer, entry);

        if (prev->free) {
            binder_delete_free_buffer(proc, buffer);
            rb_erase(&prev->rb_node, &proc->free_buffers);
            buffer = prev;
        }
    }
    binder_insert_free_buffer(proc, buffer);
}

static struct binder_node *binder_get_node(struct binder_proc *proc,
                       binder_uintptr_t ptr)
{
    struct rb_node *n = proc->nodes.rb_node;
    struct binder_node *node;

    while (n) {
        node = rb_entry(n, struct binder_node, rb_node);

        if (ptr < node->ptr)
            n = n->rb_left;
        else if (ptr > node->ptr)
            n = n->rb_right;
        else
            return node;
    }
    return NULL;
}

static struct binder_node *binder_new_node(struct binder_proc *proc,
                       binder_uintptr_t ptr,
                       binder_uintptr_t cookie)
{
    struct rb_node **p = &proc->nodes.rb_node;
    struct rb_node *parent = NULL;
    struct binder_node *node;

    // 先搜索红黑树binder_proc.nodes,如果上面已经挂上了该binder_node，那么直接返回NULL
    // 以binder实体在用户进程空间中的地址为索引
    while (*p) {
        parent = *p;
        node = rb_entry(parent, struct binder_node, rb_node);

        if (ptr < node->ptr)
            p = &(*p)->rb_left;
        else if (ptr > node->ptr)
            p = &(*p)->rb_right;
        else
            return NULL;
    }

    node = kzalloc(sizeof(*node), GFP_KERNEL);
    if (node == NULL)
        return NULL;
    binder_stats_created(BINDER_STAT_NODE);  // 全局统计node个数计数
    rb_link_node(&node->rb_node, parent, p);
    rb_insert_color(&node->rb_node, &proc->nodes);   // 插入binder_proc.nodes红黑树
    node->debug_id = ++binder_last_id;
    node->proc = proc;
    node->ptr = ptr;
    node->cookie = cookie;
    node->work.type = BINDER_WORK_NODE;
    INIT_LIST_HEAD(&node->work.entry);
    INIT_LIST_HEAD(&node->async_todo);
    binder_debug(BINDER_DEBUG_INTERNAL_REFS,
             "%d:%d node %d u%016llx c%016llx created\n",
             proc->pid, current->pid, node->debug_id,
             (u64)node->ptr, (u64)node->cookie);
    return node;
}

static int binder_inc_node(struct binder_node *node, int strong, int internal,
               struct list_head *target_list)
{
    if (strong) {
        if (internal) {
            if (target_list == NULL &&
                node->internal_strong_refs == 0 &&
                !(node == binder_context_mgr_node &&
                node->has_strong_ref)) {
                pr_err("invalid inc strong node for %d\n",
                    node->debug_id);
                return -EINVAL;
            }
            node->internal_strong_refs++;
        } else
            node->local_strong_refs++;
        if (!node->has_strong_ref && target_list) {
            list_del_init(&node->work.entry);
            list_add_tail(&node->work.entry, target_list);
        }
    } else {
        if (!internal)
            node->local_weak_refs++;
        if (!node->has_weak_ref && list_empty(&node->work.entry)) {
            if (target_list == NULL) {
                pr_err("invalid inc weak node for %d\n",
                    node->debug_id);
                return -EINVAL;
            }
            list_add_tail(&node->work.entry, target_list);
        }
    }
    return 0;
}

static int binder_dec_node(struct binder_node *node, int strong, int internal)
{
    if (strong) {
        if (internal)
            node->internal_strong_refs--;
        else
            node->local_strong_refs--;
        if (node->local_strong_refs || node->internal_strong_refs)
            return 0;
    } else {
        if (!internal)
            node->local_weak_refs--;
        if (node->local_weak_refs || !hlist_empty(&node->refs))
            return 0;
    }
    if (node->proc && (node->has_strong_ref || node->has_weak_ref)) {
        if (list_empty(&node->work.entry)) {
            list_add_tail(&node->work.entry, &node->proc->todo);
            wake_up_interruptible(&node->proc->wait);
        }
    } else {
        if (hlist_empty(&node->refs) && !node->local_strong_refs &&
            !node->local_weak_refs) {
            list_del_init(&node->work.entry);
            if (node->proc) {
                rb_erase(&node->rb_node, &node->proc->nodes);
                binder_debug(BINDER_DEBUG_INTERNAL_REFS,
                         "refless node %d deleted\n",
                         node->debug_id);
            } else {
                hlist_del(&node->dead_node);
                binder_debug(BINDER_DEBUG_INTERNAL_REFS,
                         "dead node %d deleted\n",
                         node->debug_id);
            }
            kfree(node);
            binder_stats_deleted(BINDER_STAT_NODE);
        }
    }

    return 0;
}


static struct binder_ref *binder_get_ref(struct binder_proc *proc,
                     uint32_t desc)
{
    struct rb_node *n = proc->refs_by_desc.rb_node;
    struct binder_ref *ref;

    while (n) {
        ref = rb_entry(n, struct binder_ref, rb_node_desc);

        if (desc < ref->desc)
            n = n->rb_left;
        else if (desc > ref->desc)
            n = n->rb_right;
        else
            return ref;
    }
    return NULL;
}

/*
  根据node的地址在target_proc所代表进程的refs_by_node这颗红黑树上查找对应的binder_ref是否存在。
  如果存在，表明驱动已经为当前进程建立了与node相对应的binder_ref；
  如果不存在，就新建立一个binder_ref数据结构。
*/
static struct binder_ref *binder_get_ref_for_node(struct binder_proc *proc,
                          struct binder_node *node)
{
    struct rb_node *n;
    struct rb_node **p = &proc->refs_by_node.rb_node;
    struct rb_node *parent = NULL;
    struct binder_ref *ref, *new_ref;

    // 以binder_node内存地址为索引查找红黑树binder_proc.refs_by_node。
    while (*p) {
        parent = *p;
        ref = rb_entry(parent, struct binder_ref, rb_node_node);

        if (node < ref->node)
            p = &(*p)->rb_left;
        else if (node > ref->node)
            p = &(*p)->rb_right;
        else
            return ref;  // 找到的话直接返回binder_ref的地址
    }
    new_ref = kzalloc(sizeof(*ref), GFP_KERNEL);  // 新建binder_ref数据结构
    if (new_ref == NULL)
        return NULL;
    binder_stats_created(BINDER_STAT_REF);        // 全局统计数据结构相应域加1
    new_ref->debug_id = ++binder_last_id;
    new_ref->proc = proc;                         // 和目标的binder_proc对应
    new_ref->node = node;                         // 和binder_node对应
    rb_link_node(&new_ref->rb_node_node, parent, p);
    rb_insert_color(&new_ref->rb_node_node, &proc->refs_by_node);   // 插入proc->refs_by_node红黑树

    // 如果是binder_node处于ServiceManager进程中，那么对于每个其他进程来说，引用号都是0。否则为1，然后再重新分配。
    new_ref->desc = (node == binder_context_mgr_node) ? 0 : 1;

    // 为新binder_ref重新分配引用号, 对于新创建索引的索引值desc，都是refs_by_desc红黑树中最大的索引值加1
    for (n = rb_first(&proc->refs_by_desc); n != NULL; n = rb_next(n)) {
        ref = rb_entry(n, struct binder_ref, rb_node_desc);
        if (ref->desc > new_ref->desc)
            break;
        new_ref->desc = ref->desc + 1;
    }

    p = &proc->refs_by_desc.rb_node;

    // 以新的引用号来搜索红黑树refs_by_desc
    while (*p) {
        parent = *p;
        ref = rb_entry(parent, struct binder_ref, rb_node_desc);

        if (new_ref->desc < ref->desc)
            p = &(*p)->rb_left;
        else if (new_ref->desc > ref->desc)
            p = &(*p)->rb_right;
        else
            BUG(); // 这里之所以用BUG(),是因为如果在refs_by_node没找到，这里在refs_by_desc也应该没有才合理。
    }
    rb_link_node(&new_ref->rb_node_desc, parent, p);
    rb_insert_color(&new_ref->rb_node_desc, &proc->refs_by_desc);  //  将该binder_ref挂入红黑树refs_by_desc中
    if (node) {
        // 如果node存在，这里会将新建好的binder_ref添加进binder_node的refs链表中统一管理。
        hlist_add_head(&new_ref->node_entry, &node->refs);

        binder_debug(BINDER_DEBUG_INTERNAL_REFS,
                 "%d new ref %d desc %d for node %d\n",
                  proc->pid, new_ref->debug_id, new_ref->desc,
                  node->debug_id);
    } else {
        binder_debug(BINDER_DEBUG_INTERNAL_REFS,
                 "%d new ref %d desc %d for dead node\n",
                  proc->pid, new_ref->debug_id, new_ref->desc);
    }
    return new_ref;   // 返回新建的binder_ref结构体指针
}

static void binder_delete_ref(struct binder_ref *ref)
{
    binder_debug(BINDER_DEBUG_INTERNAL_REFS,
             "%d delete ref %d desc %d for node %d\n",
              ref->proc->pid, ref->debug_id, ref->desc,
              ref->node->debug_id);

    rb_erase(&ref->rb_node_desc, &ref->proc->refs_by_desc);
    rb_erase(&ref->rb_node_node, &ref->proc->refs_by_node);
    if (ref->strong)
        binder_dec_node(ref->node, 1, 1);
    hlist_del(&ref->node_entry);
    binder_dec_node(ref->node, 0, 1);
    if (ref->death) {
        binder_debug(BINDER_DEBUG_DEAD_BINDER,
                 "%d delete ref %d desc %d has death notification\n",
                  ref->proc->pid, ref->debug_id, ref->desc);
        list_del(&ref->death->work.entry);
        kfree(ref->death);
        binder_stats_deleted(BINDER_STAT_DEATH);
    }
    kfree(ref);
    binder_stats_deleted(BINDER_STAT_REF);
}

static int binder_inc_ref(struct binder_ref *ref, int strong,
              struct list_head *target_list)
{
    int ret;

    if (strong) {
        if (ref->strong == 0) {
            ret = binder_inc_node(ref->node, 1, 1, target_list);
            if (ret)
                return ret;
        }
        ref->strong++;
    } else {
        if (ref->weak == 0) {
            ret = binder_inc_node(ref->node, 0, 1, target_list);
            if (ret)
                return ret;
        }
        ref->weak++;
    }
    return 0;
}


static int binder_dec_ref(struct binder_ref *ref, int strong)
{
    if (strong) {
        if (ref->strong == 0) {
            binder_user_error("%d invalid dec strong, ref %d desc %d s %d w %d\n",
                      ref->proc->pid, ref->debug_id,
                      ref->desc, ref->strong, ref->weak);
            return -EINVAL;
        }
        ref->strong--;
        if (ref->strong == 0) {
            int ret;

            ret = binder_dec_node(ref->node, strong, 1);
            if (ret)
                return ret;
        }
    } else {
        if (ref->weak == 0) {
            binder_user_error("%d invalid dec weak, ref %d desc %d s %d w %d\n",
                      ref->proc->pid, ref->debug_id,
                      ref->desc, ref->strong, ref->weak);
            return -EINVAL;
        }
        ref->weak--;
    }
    if (ref->strong == 0 && ref->weak == 0)
        binder_delete_ref(ref);
    return 0;
}

static void binder_pop_transaction(struct binder_thread *target_thread,
                   struct binder_transaction *t)
{
    if (target_thread) {
        BUG_ON(target_thread->transaction_stack != t);
        BUG_ON(target_thread->transaction_stack->from != target_thread);
        target_thread->transaction_stack =
            target_thread->transaction_stack->from_parent;
        t->from = NULL;
    }
    t->need_reply = 0;
    if (t->buffer)
        t->buffer->transaction = NULL;
    kfree(t);
    binder_stats_deleted(BINDER_STAT_TRANSACTION);
}

static void binder_send_failed_reply(struct binder_transaction *t,
                     uint32_t error_code)
{
    struct binder_thread *target_thread;
    struct binder_transaction *next;

    BUG_ON(t->flags & TF_ONE_WAY);
    while (1) {
        target_thread = t->from;
        if (target_thread) {
            if (target_thread->return_error != BR_OK &&
               target_thread->return_error2 == BR_OK) {
                target_thread->return_error2 =
                    target_thread->return_error;
                target_thread->return_error = BR_OK;
            }
            if (target_thread->return_error == BR_OK) {
                binder_debug(BINDER_DEBUG_FAILED_TRANSACTION,
                         "send failed reply for transaction %d to %d:%d\n",
                          t->debug_id,
                          target_thread->proc->pid,
                          target_thread->pid);

                binder_pop_transaction(target_thread, t);
                target_thread->return_error = error_code;
                wake_up_interruptible(&target_thread->wait);
            } else {
                pr_err("reply failed, target thread, %d:%d, has error code %d already\n",
                    target_thread->proc->pid,
                    target_thread->pid,
                    target_thread->return_error);
            }
            return;
        }
        next = t->from_parent;

        binder_debug(BINDER_DEBUG_FAILED_TRANSACTION,
                 "send failed reply for transaction %d, target dead\n",
                 t->debug_id);

        binder_pop_transaction(target_thread, t);
        if (next == NULL) {
            binder_debug(BINDER_DEBUG_DEAD_BINDER,
                     "reply failed, no target thread at root\n");
            return;
        }
        t = next;
        binder_debug(BINDER_DEBUG_DEAD_BINDER,
                 "reply failed, no target thread -- retry %d\n",
                  t->debug_id);
    }
}

static void binder_transaction_buffer_release(struct binder_proc *proc,
                          struct binder_buffer *buffer,
                          binder_size_t *failed_at)
{
    binder_size_t *offp, *off_end;
    int debug_id = buffer->debug_id;

    binder_debug(BINDER_DEBUG_TRANSACTION,
             "%d buffer release %d, size %zd-%zd, failed at %p\n",
             proc->pid, buffer->debug_id,
             buffer->data_size, buffer->offsets_size, failed_at);

    if (buffer->target_node)
        binder_dec_node(buffer->target_node, 1, 0);

    offp = (binder_size_t *)(buffer->data +
                 ALIGN(buffer->data_size, sizeof(void *)));
    if (failed_at)
        off_end = failed_at;
    else
        off_end = (void *)offp + buffer->offsets_size;
    for (; offp < off_end; offp++) {
        struct flat_binder_object *fp;

        if (*offp > buffer->data_size - sizeof(*fp) ||
            buffer->data_size < sizeof(*fp) ||
            !IS_ALIGNED(*offp, sizeof(u32))) {
            pr_err("transaction release %d bad offset %lld, size %zd\n",
                   debug_id, (u64)*offp, buffer->data_size);
            continue;
        }
        fp = (struct flat_binder_object *)(buffer->data + *offp);
        switch (fp->type) {
        case BINDER_TYPE_BINDER:
        case BINDER_TYPE_WEAK_BINDER: {
            struct binder_node *node = binder_get_node(proc, fp->binder);

            if (node == NULL) {
                pr_err("transaction release %d bad node %016llx\n",
                       debug_id, (u64)fp->binder);
                break;
            }
            binder_debug(BINDER_DEBUG_TRANSACTION,
                     "        node %d u%016llx\n",
                     node->debug_id, (u64)node->ptr);
            binder_dec_node(node, fp->type == BINDER_TYPE_BINDER, 0);
        } break;
        case BINDER_TYPE_HANDLE:
        case BINDER_TYPE_WEAK_HANDLE: {
            struct binder_ref *ref = binder_get_ref(proc, fp->handle);

            if (ref == NULL) {
                pr_err("transaction release %d bad handle %d\n",
                 debug_id, fp->handle);
                break;
            }
            binder_debug(BINDER_DEBUG_TRANSACTION,
                     "        ref %d desc %d (node %d)\n",
                     ref->debug_id, ref->desc, ref->node->debug_id);
            binder_dec_ref(ref, fp->type == BINDER_TYPE_HANDLE);
        } break;

        case BINDER_TYPE_FD:
            binder_debug(BINDER_DEBUG_TRANSACTION,
                     "        fd %d\n", fp->handle);
            if (failed_at)
                task_close_fd(proc, fp->handle);
            break;

        default:
            pr_err("transaction release %d bad object type %x\n",
                debug_id, fp->type);
            break;
        }
    }
}

/*
  调用该函数传递下去的参数有：
  当前task所在进程在binder驱动中对应的binder_proc结构体、和当前task在binder驱动中对应的binder_thread结构体；
  struct binder_transaction_data结构体指针，
  最后一个参数表示当前发送的是请求还是回复。

  这个函数将发送请求和发送回复放在了一个函数，而且包含了数据包中有binder在传输的处理情况，所以显得很复杂
*/
static void binder_transaction(struct binder_proc *proc,
                   struct binder_thread *thread,
                   struct binder_transaction_data *tr, int reply)
{
    /*
       理解binder驱动的关键之一在于认清下面两个结构体的区别和联系：
       struct  binder_transaction_data 和struct binder_transaction ，
       前者是用于在用户空间中表示传输的数据，而后者是binder驱动在内核空间中来表示传输的数据，
       接下来所做的工作很大部分就是完成前者向后者转换，
       而对于binder读取函数binder_thread_read()则主要是完成从后者向前者转换。
    */
    struct binder_transaction *t;   // 发送请求时使用的内核传输数据结构
    struct binder_work *tcomplete;  // binder的工作任务项
    binder_size_t *offp, *off_end;  // 处理数据包中的binder结构体用
    binder_size_t off_min;
    struct binder_proc *target_proc;  // 目标进程对应的binder_proc
    struct binder_thread *target_thread = NULL;  // 目标线程对应的binder_thread
    struct binder_node *target_node = NULL;  // 目标binder实体在内核中的节点结构体
    struct list_head *target_list;
    wait_queue_head_t *target_wait;
    struct binder_transaction *in_reply_to = NULL;
    struct binder_transaction_log_entry *e; // log record
    uint32_t return_error;

    e = binder_transaction_log_add(&binder_transaction_log);
    e->call_type = reply ? 2 : !!(tr->flags & TF_ONE_WAY);
    e->from_proc = proc->pid;
    e->from_thread = thread->pid;
    e->target_handle = tr->target.handle;
    e->data_size = tr->data_size;
    e->offsets_size = tr->offsets_size;

    if (reply) {  // ioctl写入的是回复数据

        /*
            一次单边传输过程，不管是同步还是异步，都只存在一个binder_transaction结构体，

            同步传输返回信息也是使用的发送请求时创建的binder_transaction结构体。
        */
        in_reply_to = thread->transaction_stack;
        if (in_reply_to == NULL) {
            /*
                只要是同步传输，都会有发送回复数据的过程。
            */
            binder_user_error("%d:%d got reply transaction with no transaction stack\n",
                      proc->pid, thread->pid);
            return_error = BR_FAILED_REPLY;
            goto err_empty_call_stack;
        }
        binder_set_nice(in_reply_to->saved_priority);  // 将当前task的nice优先级还原成处理接收数据之前的本来拥有的优先级

        /*
            当前线程在之前接收数据的时候保存了自己的线程地址到binder_transaction.to_thread中，这里用来做校验。
        */
        if (in_reply_to->to_thread != thread) {
            // 验证不成功的错误处理
            binder_user_error("%d:%d got reply transaction with bad transaction stack, transaction %d has target %d:%d\n",
                proc->pid, thread->pid, in_reply_to->debug_id,
                in_reply_to->to_proc ?
                in_reply_to->to_proc->pid : 0,
                in_reply_to->to_thread ?
                in_reply_to->to_thread->pid : 0);
            return_error = BR_FAILED_REPLY;
            in_reply_to = NULL;
            goto err_bad_call_stack;
        }
        thread->transaction_stack = in_reply_to->to_parent;
        target_thread = in_reply_to->from;      // 将之前的请求发送方变成回复数据接收方。
        if (target_thread == NULL) {   // 唯有BC_REPLY和异步传输时这个from才为NULL
            return_error = BR_DEAD_REPLY;
            goto err_dead_binder;
        }

        /*
            如前所述，一次单边传输只有一个binder_transaction，
            所以发送方和接收方线程的transaction_stack指向同一个binder_transaction结构体。
            如果这里不相等，那就说明前面发送请求就出了问题，不过，这种错误几乎不会发生，但是必须得留这么一手。
        */
        if (target_thread->transaction_stack != in_reply_to) {
            binder_user_error("%d:%d got reply transaction with bad target transaction stack %d, expected %d\n",
                proc->pid, thread->pid,
                target_thread->transaction_stack ?
                target_thread->transaction_stack->debug_id : 0,
                in_reply_to->debug_id);
            return_error = BR_FAILED_REPLY;
            in_reply_to = NULL;
            target_thread = NULL;
            goto err_dead_binder;
        }
        /*
            不用设置target_node这个指针，在发送回复数据的时候确实不用这个binder_node了。
            为什么？一般的接收方都是server，是具有binder实体的，而发送方一般是client，是没有binder实体，所以这个不用设置。
            除非发送方和接收方都具有binder实体，才有可能。
        */
        target_proc = target_thread->proc;
    } else {    // ioctl写入的是请求数据
        if (tr->target.handle) {  // 目标进程不是ServiceManager进程
            struct binder_ref *ref;

            /*
              根据引用号得到当前进程的binder_proc.refs_by_desc红黑树中保存的
              对应binder_ref指针，以引用号为索引。
            */
            ref = binder_get_ref(proc, tr->target.handle);
            if (ref == NULL) {  // 错误处理， 无效的handle号
                binder_user_error("%d:%d got transaction to invalid handle\n",
                    proc->pid, thread->pid);
                return_error = BR_FAILED_REPLY;
                goto err_invalid_target_handle;
            }
            target_node = ref->node;  // 通过binder_ref.node域得到目标binder实体的binder_node节点指针
        } else { // 目标进程是ServiceManager进程

            // 这个是已经存在的binder节点，系统启动时首先会让一个进程变成ServiceManager进程，才可能有后续的binder通讯。
            target_node = binder_context_mgr_node;
            if (target_node == NULL) {  // 错误处理，如果ServiceManager进程的binder内核节点还没建立好
                return_error = BR_DEAD_REPLY;
                goto err_no_context_mgr_node;
            }
        }
        e->to_node = target_node->debug_id;
        target_proc = target_node->proc;  // 得到binder_node所对应的binder_proc结构体，也就得到了binder实体所在进程的相关信息
        if (target_proc == NULL) {
            return_error = BR_DEAD_REPLY;
            goto err_dead_binder;
        }
        if (security_binder_transaction(proc->tsk,
                        target_proc->tsk) < 0) {
            return_error = BR_FAILED_REPLY;
            goto err_invalid_target_handle;
        }

        /*
          下面if部分就是universus在他文中提到的关于传输中对目标task的一点小小的优化：
          当进程P1的线程T1向进程P2发送请求时，驱动会先查看一下线程T1是否也正在处理来自P2某个线程请求但尚未完成（没有发送回复）。
          这种情况通常发生在两个进程都有Binder实体并互相对发时请求时。
          假如驱动在进程P2中发现了这样的线程，比如说T2，就会要求T2来处理T1的这次请求。因为T2既然向T1发送了请求尚未得到返回包，
          说明T2肯定（或将会）阻塞在读取返回包的状态。
          这时候可以让T2顺便做点事情，总比等在那里闲着好。
          而且如果T2不是线程池中的线程还可以为线程池分担部分工作，减少线程池使用率。

          这个优化是针对同步通讯的，所以对于异步通讯就不存在这个优化了。
          如上面的情景：本来是T1向P2发送的同步请求(交互1)，ioctl运行到这里，而如果在这之前，
          T1也正在处理来自P2进程T2的请求还没结束(交互2)，那么交互2中接收方T1也就是交互1的发送方了。
          其实这里完全可以不用优化，直接将交互1的请求投进P2进程的全局todo队列进行处理，但是为了提高效率和节省资源，
          反正交互2的发送方T2也在等待接收方的回复，也没有做事，这个时候让其做点事情岂不是更好，所以出于这样的考虑，才有了如下的优化过程。
          交互2中发送方T2和接收方T1的binder_thread.transaction_stack都应该是指向同一个binder_transaction结构体，
          而交互1中的发送方T1也就是交互2中的接收方。
        */
        if (!(tr->flags & TF_ONE_WAY) && thread->transaction_stack) {  // 同步传输的优化
            struct binder_transaction *tmp;

            tmp = thread->transaction_stack;
            if (tmp->to_thread != thread) {  // 这种优化的时候，判断发送者就是前面某次通讯的接收者->当前thread是前面某次同步传输中的接收方
                binder_user_error("%d:%d got new transaction with bad transaction stack, transaction %d has target %d:%d\n",
                    proc->pid, thread->pid, tmp->debug_id,
                    tmp->to_proc ? tmp->to_proc->pid : 0,
                    tmp->to_thread ?
                    tmp->to_thread->pid : 0);
                return_error = BR_FAILED_REPLY;
                goto err_bad_call_stack;
            }

            /*
                如果此时T1在这之前，不仅仅只是接收到T2的请求，
                而且还接收到了其他很多线程的请求均没有完成的话，那么这个transaction_stack链表中就有多个binder_transaction结构体存在，
                所以需要查找。
                查找的时候第一个条件都满足，只是第二个条件就不好满足了，就要一个一个比较，需要交互1的目标binder_proc匹配。
            */
            while (tmp) {
                if (tmp->from && tmp->from->proc == target_proc)
                    target_thread = tmp->from;  // 找到之后，直接将交互2的接收者设置成交互1的发送者。
                tmp = tmp->from_parent;     //  否则，继续查找链表
            }
        }
    }
    // 如果这个优化有结果，有找到对应的线程作为目标线程-->发送回复数据时，这个由binder驱动记录，非NULL。
    if (target_thread) {
        // 目标任务队列和线程等待队列使用特定线程的
        e->to_thread = target_thread->pid;
        target_list = &target_thread->todo;
        target_wait = &target_thread->wait;
    } else { // 优化没有结果
        // 使用目标进程的任务队列和等待队列
        target_list = &target_proc->todo;
        target_wait = &target_proc->wait;
    }
    e->to_proc = target_proc->pid;

    /*
      TODO: reuse incoming transaction for reply

      分配本次单边传输需要使用的binder_transaction结构体内存，
      这个结构体在一次单边传输中存在一个, 供发送和接收侧使用，
      一般在发送侧创建，然后接收侧通过work域反向得到binder_transaction的指针，
      最后在发送侧接收到接收侧的回复后将其内存空间释放。
    */
    t = kzalloc(sizeof(*t), GFP_KERNEL);
    if (t == NULL) {
        return_error = BR_FAILED_REPLY;
        goto err_alloc_t_failed;
    }
    binder_stats_created(BINDER_STAT_TRANSACTION);   // 全局统计计数

    tcomplete = kzalloc(sizeof(*tcomplete), GFP_KERNEL);
    if (tcomplete == NULL) {
        return_error = BR_FAILED_REPLY;
        goto err_alloc_tcomplete_failed;
    }
    binder_stats_created(BINDER_STAT_TRANSACTION_COMPLETE);

    t->debug_id = ++binder_last_id;
    e->debug_id = t->debug_id;

    if (reply)
        binder_debug(BINDER_DEBUG_TRANSACTION,
                 "%d:%d BC_REPLY %d -> %d:%d, data %016llx-%016llx size %lld-%lld\n",
                 proc->pid, thread->pid, t->debug_id,
                 target_proc->pid, target_thread->pid,
                 (u64)tr->data.ptr.buffer,
                 (u64)tr->data.ptr.offsets,
                 (u64)tr->data_size, (u64)tr->offsets_size);
    else
        binder_debug(BINDER_DEBUG_TRANSACTION,
                 "%d:%d BC_TRANSACTION %d -> %d - node %d, data %016llx-%016llx size %lld-%lld\n",
                 proc->pid, thread->pid, t->debug_id,
                 target_proc->pid, target_node->debug_id,
                 (u64)tr->data.ptr.buffer,
                 (u64)tr->data.ptr.offsets,
                 (u64)tr->data_size, (u64)tr->offsets_size);

    if (!reply && !(tr->flags & TF_ONE_WAY))  // 发送同步非回复数据
        /*
          如果是同步传输的发送边，这里将当前的binder_thread记录在binder_transaction.from中，
          以供在同步传输的回复边时，驱动可以根据这个找到回复的目的thread。
        */
        t->from = thread;
    else
        t->from = NULL;     // 如果是BC_REPLY或者是异步传输，这里不需要记录和返回信息相关的东西。
    t->sender_euid = task_euid(proc->tsk);  // 记录用户有效ID
    t->to_proc = target_proc;
    t->to_thread = target_thread;           // 可以为NULL
    t->code = tr->code;                     // 保持不变，驱动不会关心它
    t->flags = tr->flags;                   // 保持不变，驱动不会关心它
    t->priority = task_nice(current);       // 保存当前处于发送状态的task的nice值

    trace_binder_transaction(reply, t, target_node);

    /*
     从目标进程的接收缓冲区中分配data大小为tr->data_size+tr->offsets_size的binder_buffer
     最后一个参数只有在非回复并且是异步的情况下才会为1，
     这个值会记录在所分配的binder_buffer.async_transaction中来表示当前的buffer用在异步传输中还是同步传输中。

     如果是发送回复数据的时候，binder_transaction_data.flags的TF_ONE_WAY需要为1才行，
     因为既然是发送的回复数据，那肯定就不需要再让对方回信息了，除非没完没了。
    */
    t->buffer = binder_alloc_buf(target_proc, tr->data_size,
        tr->offsets_size, !reply && (t->flags & TF_ONE_WAY));
    if (t->buffer == NULL) {
        return_error = BR_FAILED_REPLY;
        goto err_binder_alloc_buf_failed;
    }
    t->buffer->allow_user_free = 0;     // 先设置成禁止用户空间释放，后面将会在接收完成之后设置成1
    t->buffer->debug_id = t->debug_id;
    t->buffer->transaction = t;         // 指向所属的binder_ transaction结构体
    t->buffer->target_node = target_node;  // 可以等于NULL, BC_REPLY时应该为NULL
    trace_binder_transaction_alloc_buf(t->buffer);
    if (target_node)
        binder_inc_node(target_node, 1, 0, NULL);   // 增加binder_node的计数

    // 计算出存放flat_binder_object结构体偏移数组的起始地址，4字节对齐。
    offp = (binder_size_t *)(t->buffer->data +
                 ALIGN(tr->data_size, sizeof(void *)));

    // 这里完成binder通讯单边时候在用户进程同内核buffer之间的一次拷贝动作
    if (copy_from_user(t->buffer->data, (const void __user *)(uintptr_t)
               tr->data.ptr.buffer, tr->data_size)) {
        binder_user_error("%d:%d got transaction with invalid data ptr\n",
                proc->pid, thread->pid);
        return_error = BR_FAILED_REPLY;
        goto err_copy_data_failed;
    }
    // 拷贝内嵌在传输数据中的flat_binder_object结构体偏移数组
    if (copy_from_user(offp, (const void __user *)(uintptr_t)
               tr->data.ptr.offsets, tr->offsets_size)) {
        binder_user_error("%d:%d got transaction with invalid offsets ptr\n",
                proc->pid, thread->pid);
        return_error = BR_FAILED_REPLY;
        goto err_copy_data_failed;
    }
    if (!IS_ALIGNED(tr->offsets_size, sizeof(binder_size_t))) {
        binder_user_error("%d:%d got transaction with invalid offsets size, %lld\n",
                proc->pid, thread->pid, (u64)tr->offsets_size);
        return_error = BR_FAILED_REPLY;
        goto err_bad_offset;
    }
    off_end = (void *)offp + tr->offsets_size;  // flat_binder_object结构体偏移数组的结束地址
    off_min = 0;
    for (; offp < off_end; offp++) {
        // struct flat_binder_object是binder在进程之间传输的表示方式。
        struct flat_binder_object *fp;

        /*
         offp是t->buffer中第一个flat_binder_object结构体的位置偏移,相当于t->buffer->data的偏移，
         这里的if语句用来判断binder偏移数组的第一个元素所指向的flat_binder_object结构体地址是否是在t->buffer->data的有效范围内，
         或者数据的总大小就小于一个flat_binder_object的大小，
         或者说这个数组的元素根本就没有4字节对齐(一个指针在32位平台上用4个字节表示)。
        */
        if (*offp > t->buffer->data_size - sizeof(*fp) ||
            *offp < off_min ||
            t->buffer->data_size < sizeof(*fp) ||
            !IS_ALIGNED(*offp, sizeof(u32))) {
            binder_user_error("%d:%d got transaction with invalid offset, %lld (min %lld, max %lld)\n",
                      proc->pid, thread->pid, (u64)*offp,
                      (u64)off_min,
                      (u64)(t->buffer->data_size -
                      sizeof(*fp)));
            return_error = BR_FAILED_REPLY;
            goto err_bad_offset;
        }
        fp = (struct flat_binder_object *)(t->buffer->data + *offp);  // 取得一个flat_binder_object结构体指针
        off_min = *offp + sizeof(struct flat_binder_object);
        switch (fp->type) {
        case BINDER_TYPE_BINDER:
        case BINDER_TYPE_WEAK_BINDER: {  // 只有具有binder实体的进程才有权利发送这类binder
            struct binder_ref *ref;
            /*
              根据flat_binder_object.binder这个binder实体在进程间的地址搜索当前进程的binder_proc->nodes红黑树，
              看看是否已经创建了binder_node内核节点
            */
            struct binder_node *node = binder_get_node(proc, fp->binder);

            if (node == NULL) {
                node = binder_new_node(proc, fp->binder, fp->cookie);
                if (node == NULL) {
                    return_error = BR_FAILED_REPLY;
                    goto err_binder_new_node_failed;
                }
                node->min_priority = fp->flags & FLAT_BINDER_FLAG_PRIORITY_MASK;  // 设置处理Binder请求的线程的最低优先级
                node->accept_fds = !!(fp->flags & FLAT_BINDER_FLAG_ACCEPTS_FDS);  // 表明节点是否同意接受文件方式的Binder
            }
            if (fp->cookie != node->cookie) {  // 额外数据校验
                binder_user_error("%d:%d sending u%016llx node %d, cookie mismatch %016llx != %016llx\n",
                    proc->pid, thread->pid,
                    (u64)fp->binder, node->debug_id,
                    (u64)fp->cookie, (u64)node->cookie);
                return_error = BR_FAILED_REPLY;
                goto err_binder_get_ref_for_node_failed;
            }
            if (security_binder_transfer_binder(proc->tsk,
                                target_proc->tsk)) {
                return_error = BR_FAILED_REPLY;
                goto err_binder_get_ref_for_node_failed;
            }
            // 在目标进程中通过binder_node搜索或者新建一个对应的binder_ref结构体
            ref = binder_get_ref_for_node(target_proc, node);
            if (ref == NULL) {
                return_error = BR_FAILED_REPLY;
                goto err_binder_get_ref_for_node_failed;
            }

            // // 修改flat_binder_object数据结构的type和handle域，接下来要传给接收方
            if (fp->type == BINDER_TYPE_BINDER)
                fp->type = BINDER_TYPE_HANDLE;
            else
                fp->type = BINDER_TYPE_WEAK_HANDLE;
            fp->handle = ref->desc;
            // 增加binder_ref的引用计数
            binder_inc_ref(ref, fp->type == BINDER_TYPE_HANDLE,
                       &thread->todo);

            trace_binder_transaction_node_to_ref(t, node, ref);
            binder_debug(BINDER_DEBUG_TRANSACTION,
                     "        node %d u%016llx -> ref %d desc %d\n",
                     node->debug_id, (u64)node->ptr,
                     ref->debug_id, ref->desc);
        } break;
        case BINDER_TYPE_HANDLE:
        case BINDER_TYPE_WEAK_HANDLE: {
            // 通过引用号取得当前进程中对应的binder_ref结构体
            struct binder_ref *ref = binder_get_ref(proc, fp->handle);

            if (ref == NULL) {
                binder_user_error("%d:%d got transaction with invalid handle, %d\n",
                        proc->pid,
                        thread->pid, fp->handle);
                return_error = BR_FAILED_REPLY;
                goto err_binder_get_ref_failed;
            }
            if (security_binder_transfer_binder(proc->tsk,
                                target_proc->tsk)) {
                return_error = BR_FAILED_REPLY;
                goto err_binder_get_ref_failed;
            }
            /*
                如果目标进程正好是提供该引用号对应的binder实体的进程
                将ptr域替换为保存在节点中的binder值；
                cookie替换为保存在节点中的cookie 值；
                type替换为BINDER_TYPE_(WEAK_)BINDER。
            */
            if (ref->node->proc == target_proc) {
                if (fp->type == BINDER_TYPE_HANDLE)
                    fp->type = BINDER_TYPE_BINDER;
                else
                    fp->type = BINDER_TYPE_WEAK_BINDER;
                fp->binder = ref->node->ptr;
                fp->cookie = ref->node->cookie;
                // 增加binder_node的引用计数
                binder_inc_node(ref->node, fp->type == BINDER_TYPE_BINDER, 0, NULL);
                trace_binder_transaction_ref_to_node(t, ref);
                binder_debug(BINDER_DEBUG_TRANSACTION,
                         "        ref %d desc %d -> node %d u%016llx\n",
                         ref->debug_id, ref->desc, ref->node->debug_id,
                         (u64)ref->node->ptr);
            } else {
                /*
                  否则会在目标进程的refs_by_node红黑树中先搜索看是否之前有创建过对应的binder_ref，
                  如果没有找到，那么就需要为ref->node节点在目标进程中新建一个目标进程的binder_ref,
                  并挂入红黑树refs_by_node中
                */
                struct binder_ref *new_ref;

                new_ref = binder_get_ref_for_node(target_proc, ref->node);
                if (new_ref == NULL) {
                    return_error = BR_FAILED_REPLY;
                    goto err_binder_get_ref_for_node_failed;
                }
                fp->handle = new_ref->desc;  // 此时只需要将此域修改为新建binder_ref的引用号
                binder_inc_ref(new_ref, fp->type == BINDER_TYPE_HANDLE, NULL);  // 增加binder_ref的引用计数
                trace_binder_transaction_ref_to_ref(t, ref,
                                    new_ref);
                binder_debug(BINDER_DEBUG_TRANSACTION,
                         "        ref %d desc %d -> ref %d desc %d (node %d)\n",
                         ref->debug_id, ref->desc, new_ref->debug_id,
                         new_ref->desc, ref->node->debug_id);
            }
        } break;

        case BINDER_TYPE_FD: {
            int target_fd;
            struct file *file;

            /*
              使用文件Binder打开的文件共享linux VFS中的struct file, struct dentry，struct inode结构，
              这意味着一个进程使用read()/write()/seek()改变了文件指针另一个进程的文件指针也会改变
            */

            if (reply) {  // 发送的是回复数据
                if (!(in_reply_to->flags & TF_ACCEPT_FDS)) {
                    binder_user_error("%d:%d got reply with fd, %d, but target does not allow fds\n",
                        proc->pid, thread->pid, fp->handle);
                    return_error = BR_FAILED_REPLY;
                    goto err_fd_not_allowed;
                }
            } else if (!target_node->accept_fds) {  // 目标进程不接受文件形式的binder
                binder_user_error("%d:%d got transaction with fd, %d, but target does not allow fds\n",
                    proc->pid, thread->pid, fp->handle);
                return_error = BR_FAILED_REPLY;
                goto err_fd_not_allowed;
            }

            file = fget(fp->handle);  // 取得当前进程的当前文件描述符对应的struct file指针
            if (file == NULL) {  // 无效的文件描述符
                binder_user_error("%d:%d got transaction with invalid fd, %d\n",
                    proc->pid, thread->pid, fp->handle);
                return_error = BR_FAILED_REPLY;
                goto err_fget_failed;
            }
            if (security_binder_transfer_file(proc->tsk,
                              target_proc->tsk,
                              file) < 0) {
                fput(file);
                return_error = BR_FAILED_REPLY;
                goto err_get_unused_fd_failed;
            }
            // 在目标进程中得到一个未使用的文件描述符返回
            target_fd = task_get_unused_fd_flags(target_proc, O_CLOEXEC);
            if (target_fd < 0) {  // 获取不成功
                fput(file);
                return_error = BR_FAILED_REPLY;
                goto err_get_unused_fd_failed;
            }
            // 通过该函数将获取的file和文件描述符加入目标进程的files_struct结构体中
            task_fd_install(target_proc, target_fd, file);
            trace_binder_transaction_fd(t, fp->handle, target_fd);
            binder_debug(BINDER_DEBUG_TRANSACTION,
                     "        fd %d -> %d\n", fp->handle, target_fd);
            /* TODO: fput? */
            fp->handle = target_fd;  // 将handle域修改成目标进程中对应的fd
        } break;

        default:
            binder_user_error("%d:%d got transaction with invalid object type, %x\n",
                proc->pid, thread->pid, fp->type);
            return_error = BR_FAILED_REPLY;
            goto err_bad_object_type;
        }
    }
    if (reply) {   // 发送的是回复数据

        BUG_ON(t->buffer->async_transaction != 0);
        /*
            这里表示一次同步通讯过程中，接收方已经将回复数据发送给发送方，
            这里就可以pop出之前发送方给接收方发送时创建的binder_transaction数据结构，释放其占用的内存空间。

            这次同步通信中，发送请求时候产生的binder_transaction数据结构在该函数前面已经从请求接收方的stack链表摘下，
            这里就将其从发送请求方的stack链表上摘下。
        */
        binder_pop_transaction(target_thread, in_reply_to);
    } else if (!(t->flags & TF_ONE_WAY)) {  // 同步发送请求
        BUG_ON(t->buffer->async_transaction != 0);  //0 表示同步, 1表示异步
        t->need_reply = 1;  // 需要回复

        /*
          如果本次发起传输之前，当前thread没有处于通讯过程中的话，这里必然为NULL。
          而且第一次发起传输时，这里也是为NULl。
          如果之前有异步传输没处理完，那么这里不为null，如果之前本thread正在处理接收请求，这里也不为NULL。
        */
        t->from_parent = thread->transaction_stack;
        /*
          这里将传输中间数据结构保存在binder_transaction链表顶部。
          这个transaction_stack实际上是管理这一个链表，
          只不过这个指针始终指向最新加入该链表的成员，最先加入的成员在最底部，有点类似于stack，所以这里取名叫transaction_stack。
        */
        thread->transaction_stack = t;
    } else {  // 异步发送请求
        BUG_ON(target_node == NULL);    // 异步传输时目标一定要明确
        /*
          最后一个参数只有在非回复并且是异步的情况下才会为1，这个值会记录在所分配

          的binder_buffer.async_transaction中来表示当前的buffer用在异步传输中还是同步传输中。
        */
        BUG_ON(t->buffer->async_transaction != 1);

        // 表明该节点在to-do队列中有异步交互尚未完成, 目标node仍然还有一个异步需要处理的话, 该标志为1
        if (target_node->has_async_transaction) {
            target_list = &target_node->async_todo; // 将当前的这个的异步传输任务转入目标进程binder_node的异步等待队列async_todo中
            target_wait = NULL;
        } else
            target_node->has_async_transaction = 1; // 否则，将该标志置1，表明当前有一个异步交互正在处理
    }

    /*
     对于异步发送请求来说，到这里结束之后，驱动中已经准备好了相应的binder_transaction结构体，
     只不过极有可能将这个结构体通过其中的binder_work域挂到了目标进程的binder_node.async_todo队列中，
     直到target_node->has_async_transaction标志为0。接着如果运气好的话，
     才有可能将binder_transaction移动到目标进程的全局任务队列binder_node.todo中，
     这时binder驱动就会给他找一个空闲线程来处理这个请求数据包
    */
    t->work.type = BINDER_WORK_TRANSACTION;
    list_add_tail(&t->work.entry, target_list);  // 将binder_transaction.binder_work加入目标to_do队列中去等待分配处理
    tcomplete->type = BINDER_WORK_TRANSACTION_COMPLETE;
    // 发送端binder_work加入当前线程的todo队列中，binder驱动通知发送端，数据已经成功发送出去了。
    list_add_tail(&tcomplete->entry, &thread->todo);
    if (target_wait)
        wake_up_interruptible(target_wait);  // 唤醒目标task的等待队列
    return;

err_get_unused_fd_failed:
err_fget_failed:
err_fd_not_allowed:
err_binder_get_ref_for_node_failed:
err_binder_get_ref_failed:
err_binder_new_node_failed:
err_bad_object_type:
err_bad_offset:
err_copy_data_failed:
    trace_binder_transaction_failed_buffer_release(t->buffer);
    binder_transaction_buffer_release(target_proc, t->buffer, offp);
    t->buffer->transaction = NULL;
    binder_free_buf(target_proc, t->buffer);
err_binder_alloc_buf_failed:
    kfree(tcomplete);
    binder_stats_deleted(BINDER_STAT_TRANSACTION_COMPLETE);
err_alloc_tcomplete_failed:
    kfree(t);
    binder_stats_deleted(BINDER_STAT_TRANSACTION);
err_alloc_t_failed:
err_bad_call_stack:
err_empty_call_stack:
err_dead_binder:
err_invalid_target_handle:
err_no_context_mgr_node:
    binder_debug(BINDER_DEBUG_FAILED_TRANSACTION,
             "%d:%d transaction failed %d, size %lld-%lld\n",
             proc->pid, thread->pid, return_error,
             (u64)tr->data_size, (u64)tr->offsets_size);

    {
        struct binder_transaction_log_entry *fe;

        fe = binder_transaction_log_add(&binder_transaction_log_failed);
        *fe = *e;
    }

    BUG_ON(thread->return_error != BR_OK);
    if (in_reply_to) {
        thread->return_error = BR_TRANSACTION_COMPLETE;
        binder_send_failed_reply(in_reply_to, return_error);
    } else
        thread->return_error = return_error;
}

/*
 调用该函数传递下去的参数有：
 当前task所在进程在binder驱动中对应的binder_proc结构体、和当前thread在binder驱动中对应的binder_thread结构体；
 本次ioctl的参数binder_write_read结构体的write_buffer、write_size、write_consumed域。

*/
static int binder_thread_write(struct binder_proc *proc,
            struct binder_thread *thread,
            binder_uintptr_t binder_buffer, size_t size,
            binder_size_t *consumed)
{
    uint32_t cmd;
    void __user *buffer = (void __user *)(uintptr_t)binder_buffer;
    void __user *ptr = buffer + *consumed;
    void __user *end = buffer + size;

    /*
      一次ioctl的发送过程，可以在bwr.write_buffer中按照格式：命令+参数，组织多个命令包发送给接收方，
      所以这里使用了循环的方式来一个一个地处理这些命令包。
    */
    while (ptr < end && thread->return_error == BR_OK) {
        /*
          ioctl传进来的只是结构体binder_write_read，而对于其中write和read的

          buffer却还是存在于用户空间，下面用get_user来获取用户空间buffer中的值。

        */
        // 取得用户空间buffer中每个命令包的命令字
        if (get_user(cmd, (uint32_t __user *)ptr))
            return -EFAULT;
        ptr += sizeof(uint32_t);
        trace_binder_command(cmd);
        if (_IOC_NR(cmd) < ARRAY_SIZE(binder_stats.bc)) {
            // 提取出cmd中的命令序号，对全局统计数据结构中bc对应命令使用域加1
            binder_stats.bc[_IOC_NR(cmd)]++;    // binder驱动全局统计数据
            proc->stats.bc[_IOC_NR(cmd)]++;     // binder_proc统计数据
            thread->stats.bc[_IOC_NR(cmd)]++;   //  binder_thread统计数据
        }
        switch (cmd) {
        case BC_INCREFS:
        case BC_ACQUIRE:
        case BC_RELEASE:
        case BC_DECREFS: {
            uint32_t target;
            struct binder_ref *ref;
            const char *debug_string;

            if (get_user(target, (uint32_t __user *)ptr))
                return -EFAULT;
            ptr += sizeof(uint32_t);
            if (target == 0 && binder_context_mgr_node &&
                (cmd == BC_INCREFS || cmd == BC_ACQUIRE)) {
                ref = binder_get_ref_for_node(proc,
                           binder_context_mgr_node);
                if (ref->desc != target) {
                    binder_user_error("%d:%d tried to acquire reference to desc 0, got %d instead\n",
                        proc->pid, thread->pid,
                        ref->desc);
                }
            } else
                ref = binder_get_ref(proc, target);
            if (ref == NULL) {
                binder_user_error("%d:%d refcount change on invalid ref %d\n",
                    proc->pid, thread->pid, target);
                break;
            }
            switch (cmd) {
            case BC_INCREFS:
                debug_string = "IncRefs";
                binder_inc_ref(ref, 0, NULL);
                break;
            case BC_ACQUIRE:
                debug_string = "Acquire";
                binder_inc_ref(ref, 1, NULL);
                break;
            case BC_RELEASE:
                debug_string = "Release";
                binder_dec_ref(ref, 1);
                break;
            case BC_DECREFS:
            default:
                debug_string = "DecRefs";
                binder_dec_ref(ref, 0);
                break;
            }
            binder_debug(BINDER_DEBUG_USER_REFS,
                     "%d:%d %s ref %d desc %d s %d w %d for node %d\n",
                     proc->pid, thread->pid, debug_string, ref->debug_id,
                     ref->desc, ref->strong, ref->weak, ref->node->debug_id);
            break;
        }
        case BC_INCREFS_DONE:
        case BC_ACQUIRE_DONE: {
            binder_uintptr_t node_ptr;
            binder_uintptr_t cookie;
            struct binder_node *node;

            if (get_user(node_ptr, (binder_uintptr_t __user *)ptr))
                return -EFAULT;
            ptr += sizeof(binder_uintptr_t);
            if (get_user(cookie, (binder_uintptr_t __user *)ptr))
                return -EFAULT;
            ptr += sizeof(binder_uintptr_t);
            node = binder_get_node(proc, node_ptr);
            if (node == NULL) {
                binder_user_error("%d:%d %s u%016llx no match\n",
                    proc->pid, thread->pid,
                    cmd == BC_INCREFS_DONE ?
                    "BC_INCREFS_DONE" :
                    "BC_ACQUIRE_DONE",
                    (u64)node_ptr);
                break;
            }
            if (cookie != node->cookie) {
                binder_user_error("%d:%d %s u%016llx node %d cookie mismatch %016llx != %016llx\n",
                    proc->pid, thread->pid,
                    cmd == BC_INCREFS_DONE ?
                    "BC_INCREFS_DONE" : "BC_ACQUIRE_DONE",
                    (u64)node_ptr, node->debug_id,
                    (u64)cookie, (u64)node->cookie);
                break;
            }
            if (cmd == BC_ACQUIRE_DONE) {
                if (node->pending_strong_ref == 0) {
                    binder_user_error("%d:%d BC_ACQUIRE_DONE node %d has no pending acquire request\n",
                        proc->pid, thread->pid,
                        node->debug_id);
                    break;
                }
                node->pending_strong_ref = 0;
            } else {
                if (node->pending_weak_ref == 0) {
                    binder_user_error("%d:%d BC_INCREFS_DONE node %d has no pending increfs request\n",
                        proc->pid, thread->pid,
                        node->debug_id);
                    break;
                }
                node->pending_weak_ref = 0;
            }
            binder_dec_node(node, cmd == BC_ACQUIRE_DONE, 0);
            binder_debug(BINDER_DEBUG_USER_REFS,
                     "%d:%d %s node %d ls %d lw %d\n",
                     proc->pid, thread->pid,
                     cmd == BC_INCREFS_DONE ? "BC_INCREFS_DONE" : "BC_ACQUIRE_DONE",
                     node->debug_id, node->local_strong_refs, node->local_weak_refs);
            break;
        }
        case BC_ATTEMPT_ACQUIRE:
            pr_err("BC_ATTEMPT_ACQUIRE not supported\n");
            return -EINVAL;
        case BC_ACQUIRE_RESULT:
            pr_err("BC_ACQUIRE_RESULT not supported\n");
            return -EINVAL;

        case BC_FREE_BUFFER: {
            // data_ptr指向在read操作中接收到的binder_buffer的data 区域
            binder_uintptr_t data_ptr;
            struct binder_buffer *buffer;

            if (get_user(data_ptr, (binder_uintptr_t __user *)ptr))
                return -EFAULT;
            ptr += sizeof(binder_uintptr_t);

            // 取出binder_buffer结构体指针
            buffer = binder_buffer_lookup(proc, data_ptr);
            if (buffer == NULL) {
                binder_user_error("%d:%d BC_FREE_BUFFER u%016llx no match\n",
                    proc->pid, thread->pid, (u64)data_ptr);
                break;
            }
            if (!buffer->allow_user_free) {  // 判断buffer对象是否允许用户释放, binder_thread_read在使用完之后会将这里设置成1
                binder_user_error("%d:%d BC_FREE_BUFFER u%016llx matched unreturned buffer\n",
                    proc->pid, thread->pid, (u64)data_ptr);
                break;
            }
            binder_debug(BINDER_DEBUG_FREE_BUFFER,
                     "%d:%d BC_FREE_BUFFER u%016llx found buffer %d for %s transaction\n",
                     proc->pid, thread->pid, (u64)data_ptr,
                     buffer->debug_id,
                     buffer->transaction ? "active" : "finished");

            if (buffer->transaction) {  // 如果为active，那么强制将buffer->transaction置成NULL
                buffer->transaction->buffer = NULL;
                buffer->transaction = NULL;
            }

            // 当前释放的binder_buffer如果是异步传输所用，并且目标binder_node存在 (非回复的情况下)。
            if (buffer->async_transaction && buffer->target_node) {
                // 异常检查, 该成员表明该节点在to-do队列中有异步交互尚未完成
                BUG_ON(!buffer->target_node->has_async_transaction);

                // 为NULL，表明没有异步任务在等待执行了
                if (list_empty(&buffer->target_node->async_todo))
                    buffer->target_node->has_async_transaction = 0;
                else  // 不为NULL,将最早等待的异步任务加入当前task的todo队列中
                    list_move_tail(buffer->target_node->async_todo.next, &thread->todo);
            }
            trace_binder_transaction_buffer_release(buffer);
            /*
                该函数将对使用buffer这个binder_buffer的通讯中出现的flat_binder_object结构体
                对应的binder_node或者binder_ref减少一个引用计数。
            */
            binder_transaction_buffer_release(proc, buffer, NULL);
            binder_free_buf(proc, buffer);  // 真正的释放buffer这个binder_buffer内存空间的函数
            break;
        }

        case BC_TRANSACTION:
        case BC_REPLY: {
            /*
               理解binder驱动的关键之一在于认清下面两个结构体的区别和联系：
               struct  binder_transaction_data 和struct binder_transaction ，
               前者是用于在用户空间中表示传输的数据，而后者是binder驱动在内核空间中来表示传输的数据，
               接下来所做的工作很大部分就是完成前者向后者转换，
               而对于binder读取函数binder_thread_read()则主要是完成从后者向前者转换。
            */
            struct binder_transaction_data tr;

            // 先将传输数据结构从用户空间拷贝到内核空间
            if (copy_from_user(&tr, ptr, sizeof(tr)))
                return -EFAULT;
            ptr += sizeof(tr);   // 指针向前移动
            /*
              完成传输数据的转换、如果有binder在传输数据包中，
              需要为检查这些binder是否已经符合将来通讯的要求，最后就是往目标任务队列中添加任务，唤醒目标task
            */
            binder_transaction(proc, thread, &tr, cmd == BC_REPLY);
            break;
        }

        case BC_REGISTER_LOOPER:
            binder_debug(BINDER_DEBUG_THREADS,
                     "%d:%d BC_REGISTER_LOOPER\n",
                     proc->pid, thread->pid);
            if (thread->looper & BINDER_LOOPER_STATE_ENTERED) {
                thread->looper |= BINDER_LOOPER_STATE_INVALID;
                binder_user_error("%d:%d ERROR: BC_REGISTER_LOOPER called after BC_ENTER_LOOPER\n",
                    proc->pid, thread->pid);
            } else if (proc->requested_threads == 0) {
                thread->looper |= BINDER_LOOPER_STATE_INVALID;
                binder_user_error("%d:%d ERROR: BC_REGISTER_LOOPER called without request\n",
                    proc->pid, thread->pid);
            } else {
                proc->requested_threads--;
                proc->requested_threads_started++;
            }
            thread->looper |= BINDER_LOOPER_STATE_REGISTERED;
            break;
        case BC_ENTER_LOOPER:
            binder_debug(BINDER_DEBUG_THREADS,
                     "%d:%d BC_ENTER_LOOPER\n",
                     proc->pid, thread->pid);
            if (thread->looper & BINDER_LOOPER_STATE_REGISTERED) {
                thread->looper |= BINDER_LOOPER_STATE_INVALID;
                binder_user_error("%d:%d ERROR: BC_ENTER_LOOPER called after BC_REGISTER_LOOPER\n",
                    proc->pid, thread->pid);
            }
            thread->looper |= BINDER_LOOPER_STATE_ENTERED;
            break;
        case BC_EXIT_LOOPER:
            binder_debug(BINDER_DEBUG_THREADS,
                     "%d:%d BC_EXIT_LOOPER\n",
                     proc->pid, thread->pid);
            thread->looper |= BINDER_LOOPER_STATE_EXITED;
            break;

        case BC_REQUEST_DEATH_NOTIFICATION:
        case BC_CLEAR_DEATH_NOTIFICATION: {
            uint32_t target;
            binder_uintptr_t cookie;
            struct binder_ref *ref;
            struct binder_ref_death *death;

            if (get_user(target, (uint32_t __user *)ptr))
                return -EFAULT;
            ptr += sizeof(uint32_t);
            if (get_user(cookie, (binder_uintptr_t __user *)ptr))
                return -EFAULT;
            ptr += sizeof(binder_uintptr_t);
            ref = binder_get_ref(proc, target);
            if (ref == NULL) {
                binder_user_error("%d:%d %s invalid ref %d\n",
                    proc->pid, thread->pid,
                    cmd == BC_REQUEST_DEATH_NOTIFICATION ?
                    "BC_REQUEST_DEATH_NOTIFICATION" :
                    "BC_CLEAR_DEATH_NOTIFICATION",
                    target);
                break;
            }

            binder_debug(BINDER_DEBUG_DEATH_NOTIFICATION,
                     "%d:%d %s %016llx ref %d desc %d s %d w %d for node %d\n",
                     proc->pid, thread->pid,
                     cmd == BC_REQUEST_DEATH_NOTIFICATION ?
                     "BC_REQUEST_DEATH_NOTIFICATION" :
                     "BC_CLEAR_DEATH_NOTIFICATION",
                     (u64)cookie, ref->debug_id, ref->desc,
                     ref->strong, ref->weak, ref->node->debug_id);

            if (cmd == BC_REQUEST_DEATH_NOTIFICATION) {
                if (ref->death) {
                    binder_user_error("%d:%d BC_REQUEST_DEATH_NOTIFICATION death notification already set\n",
                        proc->pid, thread->pid);
                    break;
                }
                death = kzalloc(sizeof(*death), GFP_KERNEL);
                if (death == NULL) {
                    thread->return_error = BR_ERROR;
                    binder_debug(BINDER_DEBUG_FAILED_TRANSACTION,
                             "%d:%d BC_REQUEST_DEATH_NOTIFICATION failed\n",
                             proc->pid, thread->pid);
                    break;
                }
                binder_stats_created(BINDER_STAT_DEATH);
                INIT_LIST_HEAD(&death->work.entry);
                death->cookie = cookie;
                ref->death = death;
                if (ref->node->proc == NULL) { // 如果实体不存在了就将binder_work的类设置成该值，表示binder实体死亡
                    ref->death->work.type = BINDER_WORK_DEAD_BINDER;

                    /*
                        如果binder_thread表示的task已就绪，那么将该work加入当前task的私有todo队列中,否则加入进程的全局todo中，
                        然后唤醒进程的空闲线程等待队列。
                    */
                    if (thread->looper & (BINDER_LOOPER_STATE_REGISTERED | BINDER_LOOPER_STATE_ENTERED)) {
                        list_add_tail(&ref->death->work.entry, &thread->todo);
                    } else {
                        list_add_tail(&ref->death->work.entry, &proc->todo);
                        wake_up_interruptible(&proc->wait);
                    }
                }
            } else {  // 取消订阅
                if (ref->death == NULL) {
                    binder_user_error("%d:%d BC_CLEAR_DEATH_NOTIFICATION death notification not active\n",
                        proc->pid, thread->pid);
                    break;
                }
                death = ref->death;
                if (death->cookie != cookie) {  // cookie数据验证，之前订阅是保存在驱动中的和当前ioctl传下来的cookie比较。
                    binder_user_error("%d:%d BC_CLEAR_DEATH_NOTIFICATION death notification cookie mismatch %016llx != %016llx\n",
                        proc->pid, thread->pid,
                        (u64)death->cookie,
                        (u64)cookie);
                    break;
                }
                ref->death = NULL;
                if (list_empty(&death->work.entry)) { //  无当前binder实体的死亡通知需要处理。
                    // 那么就直接将binder_work的类型定义成该值，表示只取消订阅通知。
                    death->work.type = BINDER_WORK_CLEAR_DEATH_NOTIFICATION;

                    /*
                      这里为什么不用wake_up一下thread->wait呢？这个wait中经常是没有task在等待的，
                      即使有也是在同步通信的时候将自己挂进这个wait中等待，没有其他情况了，
                      所以这里首先判断这个task是不是已经在进入循环开始工作了，如果是的话，
                      可以将当前work加入到task的私有todo中，这样在后面的通讯过程中，迟早会处理这个work任务的。
                    */
                    if (thread->looper & (BINDER_LOOPER_STATE_REGISTERED | BINDER_LOOPER_STATE_ENTERED)) {
                        list_add_tail(&death->work.entry, &thread->todo);
                    } else {
                        list_add_tail(&death->work.entry, &proc->todo);
                        wake_up_interruptible(&proc->wait);
                    }
                } else {
                    /*
                      如果在取消订阅之前已经有收到当前对应binder实体的死亡通知了，也就是&death->work.entry已经存在于todo队列中。
                      这个时候将binder_work类型修改为BINDER_WORK_DEAD_BINDER_AND_CLEAR，表示需要先处理死亡通知，
                      再处理取消取消订阅的请求。
                    */
                    BUG_ON(death->work.type != BINDER_WORK_DEAD_BINDER);
                    death->work.type = BINDER_WORK_DEAD_BINDER_AND_CLEAR;
                }
            }
        } break;
        case BC_DEAD_BINDER_DONE: {
            struct binder_work *w;
            binder_uintptr_t cookie;
            struct binder_ref_death *death = NULL;

            if (get_user(cookie, (binder_uintptr_t __user *)ptr))
                return -EFAULT;

            ptr += sizeof(void *);
            list_for_each_entry(w, &proc->delivered_death, entry) {
                struct binder_ref_death *tmp_death = container_of(w, struct binder_ref_death, work);

                if (tmp_death->cookie == cookie) {
                    death = tmp_death;
                    break;
                }
            }
            binder_debug(BINDER_DEBUG_DEAD_BINDER,
                     "%d:%d BC_DEAD_BINDER_DONE %016llx found %p\n",
                     proc->pid, thread->pid, (u64)cookie,
                     death);
            if (death == NULL) {
                binder_user_error("%d:%d BC_DEAD_BINDER_DONE %016llx not found\n",
                    proc->pid, thread->pid, (u64)cookie);
                break;
            }

            list_del_init(&death->work.entry);
            if (death->work.type == BINDER_WORK_DEAD_BINDER_AND_CLEAR) {
                death->work.type = BINDER_WORK_CLEAR_DEATH_NOTIFICATION;
                if (thread->looper & (BINDER_LOOPER_STATE_REGISTERED | BINDER_LOOPER_STATE_ENTERED)) {
                    list_add_tail(&death->work.entry, &thread->todo);
                } else {
                    list_add_tail(&death->work.entry, &proc->todo);
                    wake_up_interruptible(&proc->wait);
                }
            }
        } break;

        default:
            pr_err("%d:%d unknown command %d\n",
                   proc->pid, thread->pid, cmd);
            return -EINVAL;
        }
        *consumed = ptr - buffer;
    }
    return 0;
}

// 统计计数
static void binder_stat_br(struct binder_proc *proc,
               struct binder_thread *thread, uint32_t cmd)
{
    trace_binder_return(cmd);
    if (_IOC_NR(cmd) < ARRAY_SIZE(binder_stats.br)) {
        binder_stats.br[_IOC_NR(cmd)]++;
        proc->stats.br[_IOC_NR(cmd)]++;
        thread->stats.br[_IOC_NR(cmd)]++;
    }
}

static int binder_has_proc_work(struct binder_proc *proc,
                struct binder_thread *thread)
{
    /*
      binder_thread中looper的这个标识: BINDER_LOOPER_STATE_NEED_RETURN, 表示等待时强制让线程return。
      binder_thread在创建的时候都自带有BINDER_LOOPER_STATE_NEED_RETURN，所以默认情况下是允许强制return的。
    */
    return !list_empty(&proc->todo) ||
        (thread->looper & BINDER_LOOPER_STATE_NEED_RETURN);
}

static int binder_has_thread_work(struct binder_thread *thread)
{
    /*
      binder_thread中looper的这个标识: BINDER_LOOPER_STATE_NEED_RETURN, 表示等待时强制让线程return。
      binder_thread在创建的时候都自带有BINDER_LOOPER_STATE_NEED_RETURN，所以默认情况下是允许强制return的。
    */
    return !list_empty(&thread->todo) || thread->return_error != BR_OK ||
        (thread->looper & BINDER_LOOPER_STATE_NEED_RETURN);
}


/*
 该函数调用传递的参数有：当前task所在的进程对应的binder_proc和binder_thread结构体指针，
 用户空间的read_buffer地址，想读取数据的大小，
 已经打开binder节点的时候是否是非阻塞打开，默认情况下是阻塞打开文件的。
*/
static int binder_thread_read(struct binder_proc *proc,
                  struct binder_thread *thread,
                  binder_uintptr_t binder_buffer, size_t size,
                  binder_size_t *consumed, int non_block)
{
    void __user *buffer = (void __user *)(uintptr_t)binder_buffer;
    void __user *ptr = buffer + *consumed;
    void __user *end = buffer + size;

    int ret = 0;
    int wait_for_proc_work;

    // 如果实际读取到的大小等于0，那么将会在返回的数据包中插入BR_NOOP的命令字。
    if (*consumed == 0) {
        if (put_user(BR_NOOP, (uint32_t __user *)ptr))
            return -EFAULT;
        ptr += sizeof(uint32_t);
    }

retry:
    /*
     该标志表示当前task是要去等待处理proc中全局的todo还是自己本thread的todo队列中的任务，两个条件决定这个标志是否为1。
     第一个条件当前task的binder_transaction这个链表为NULL, 它记录着本task上是否有传输正在进行；
     第二个条件是当前task的私有任务队列为NULL。
    */
    wait_for_proc_work = thread->transaction_stack == NULL &&
                list_empty(&thread->todo);

    if (thread->return_error != BR_OK && ptr < end) {  // 错误处理
        if (thread->return_error2 != BR_OK) {
            if (put_user(thread->return_error2, (uint32_t __user *)ptr))
                return -EFAULT;
            ptr += sizeof(uint32_t);
            binder_stat_br(proc, thread, thread->return_error2);
            if (ptr == end)
                goto done;
            thread->return_error2 = BR_OK;
        }
        if (put_user(thread->return_error, (uint32_t __user *)ptr))
            return -EFAULT;
        ptr += sizeof(uint32_t);
        binder_stat_br(proc, thread, thread->return_error);
        thread->return_error = BR_OK;
        goto done;
    }


    thread->looper |= BINDER_LOOPER_STATE_WAITING;  // 表明线程即将进入等待状态
    if (wait_for_proc_work)
        proc->ready_threads++;                      // 就绪等待任务的空闲线程数加1

    binder_unlock(__func__);

    trace_binder_wait_for_work(wait_for_proc_work,
                   !!thread->transaction_stack,
                   !list_empty(&thread->todo));
    if (wait_for_proc_work) {
        if (!(thread->looper & (BINDER_LOOPER_STATE_REGISTERED |
                    BINDER_LOOPER_STATE_ENTERED))) {
            binder_user_error("%d:%d ERROR: Thread waiting for process work before calling BC_REGISTER_LOOPER or BC_ENTER_LOOPER (state %x)\n",
                proc->pid, thread->pid, thread->looper);
            wait_event_interruptible(binder_user_error_wait,
                         binder_stop_on_user_error < 2);
        }
        binder_set_nice(proc->default_priority);
        if (non_block) {  // 非阻塞打开的binder设备节点
            if (!binder_has_proc_work(proc, thread))
                ret = -EAGAIN;
        } else
            // 当前thread互斥等待在进程全局的等待队列中
            ret = wait_event_freezable_exclusive(proc->wait, binder_has_proc_work(proc, thread));
    } else {
        if (non_block) {
            if (!binder_has_thread_work(thread))
                ret = -EAGAIN;
        } else
            // 当前task等待在thread自己的等待队列中(binder_thread.todo)，永远只有其自己。
            ret = wait_event_freezable(thread->wait, binder_has_thread_work(thread));
    }

    binder_lock(__func__);

    if (wait_for_proc_work)
        proc->ready_threads--;                       // 就绪空闲线程少了一个,
    thread->looper &= ~BINDER_LOOPER_STATE_WAITING;

    if (ret)
        return ret;                                 // 如果是以非阻塞的方式打开的binder设备节点，那么这里将会返回-EAGAIN

    while (1) {
        uint32_t cmd;
        struct binder_transaction_data tr;         // 为用户空间准备的数据结构
        struct binder_work *w;
        struct binder_transaction *t = NULL;

        if (!list_empty(&thread->todo)) {   // 当前thread私有todo任务队列里有任务
            // 取出todo队列中第一个binder_work结构体
            w = list_first_entry(&thread->todo, struct binder_work,
                         entry);
        } else if (!list_empty(&proc->todo) && wait_for_proc_work) { //当前proc的公共todo任务队列里有任务
            // proc->todo是当前thread所属进程的公共todo任务队列
            w = list_first_entry(&proc->todo, struct binder_work,
                         entry);
        } else {
            /*
              no data added
              buffer中只有一个BR_NOOP,  同时当前task的BINDER_LOOPER_STATE_NEED_RETURN标志已被清除，
              那么就跳转回去重新睡眠等待。
            */
            if (ptr - buffer == 4 &&
                !(thread->looper & BINDER_LOOPER_STATE_NEED_RETURN))
                goto retry;
            break;
        }

        if (end - ptr < sizeof(tr) + 4)   // 检查剩余的read_buffer的大小是否合理
            break;

        switch (w->type) {
        case BINDER_WORK_TRANSACTION: {
            // 通过binder_work：w反向找到所属的binder_transaction数据结构指针
            t = container_of(w, struct binder_transaction, work);
        } break;
        case BINDER_WORK_TRANSACTION_COMPLETE: {
            cmd = BR_TRANSACTION_COMPLETE;
            if (put_user(cmd, (uint32_t __user *)ptr))
                return -EFAULT;
            ptr += sizeof(uint32_t);

            binder_stat_br(proc, thread, cmd);
            binder_debug(BINDER_DEBUG_TRANSACTION_COMPLETE,
                     "%d:%d BR_TRANSACTION_COMPLETE\n",
                     proc->pid, thread->pid);

            list_del(&w->entry);
            kfree(w);
            binder_stats_deleted(BINDER_STAT_TRANSACTION_COMPLETE);
        } break;
        case BINDER_WORK_NODE: {
            struct binder_node *node = container_of(w, struct binder_node, work);
            uint32_t cmd = BR_NOOP;
            const char *cmd_name;
            int strong = node->internal_strong_refs || node->local_strong_refs;
            int weak = !hlist_empty(&node->refs) || node->local_weak_refs || strong;

            if (weak && !node->has_weak_ref) {
                cmd = BR_INCREFS;
                cmd_name = "BR_INCREFS";
                node->has_weak_ref = 1;
                node->pending_weak_ref = 1;
                node->local_weak_refs++;
            } else if (strong && !node->has_strong_ref) {
                cmd = BR_ACQUIRE;
                cmd_name = "BR_ACQUIRE";
                node->has_strong_ref = 1;
                node->pending_strong_ref = 1;
                node->local_strong_refs++;
            } else if (!strong && node->has_strong_ref) {
                cmd = BR_RELEASE;
                cmd_name = "BR_RELEASE";
                node->has_strong_ref = 0;
            } else if (!weak && node->has_weak_ref) {
                cmd = BR_DECREFS;
                cmd_name = "BR_DECREFS";
                node->has_weak_ref = 0;
            }
            if (cmd != BR_NOOP) {
                if (put_user(cmd, (uint32_t __user *)ptr))
                    return -EFAULT;
                ptr += sizeof(uint32_t);
                if (put_user(node->ptr,
                         (binder_uintptr_t __user *)ptr))
                    return -EFAULT;
                ptr += sizeof(binder_uintptr_t);
                if (put_user(node->cookie,
                         (binder_uintptr_t __user *)ptr))
                    return -EFAULT;
                ptr += sizeof(binder_uintptr_t);

                binder_stat_br(proc, thread, cmd);
                binder_debug(BINDER_DEBUG_USER_REFS,
                         "%d:%d %s %d u%016llx c%016llx\n",
                         proc->pid, thread->pid, cmd_name,
                         node->debug_id,
                         (u64)node->ptr, (u64)node->cookie);
            } else {
                list_del_init(&w->entry);
                if (!weak && !strong) {
                    binder_debug(BINDER_DEBUG_INTERNAL_REFS,
                             "%d:%d node %d u%016llx c%016llx deleted\n",
                             proc->pid, thread->pid,
                             node->debug_id,
                             (u64)node->ptr,
                             (u64)node->cookie);
                    rb_erase(&node->rb_node, &proc->nodes);
                    kfree(node);
                    binder_stats_deleted(BINDER_STAT_NODE);
                } else {
                    binder_debug(BINDER_DEBUG_INTERNAL_REFS,
                             "%d:%d node %d u%016llx c%016llx state unchanged\n",
                             proc->pid, thread->pid,
                             node->debug_id,
                             (u64)node->ptr,
                             (u64)node->cookie);
                }
            }
        } break;
        case BINDER_WORK_DEAD_BINDER:
        case BINDER_WORK_DEAD_BINDER_AND_CLEAR:
        case BINDER_WORK_CLEAR_DEATH_NOTIFICATION: {
            struct binder_ref_death *death;
            uint32_t cmd;

            death = container_of(w, struct binder_ref_death, work);
            if (w->type == BINDER_WORK_CLEAR_DEATH_NOTIFICATION)
                cmd = BR_CLEAR_DEATH_NOTIFICATION_DONE;
            else
                cmd = BR_DEAD_BINDER;
            if (put_user(cmd, (uint32_t __user *)ptr))
                return -EFAULT;
            ptr += sizeof(uint32_t);
            if (put_user(death->cookie,
                     (binder_uintptr_t __user *)ptr))
                return -EFAULT;
            ptr += sizeof(binder_uintptr_t);
            binder_stat_br(proc, thread, cmd);
            binder_debug(BINDER_DEBUG_DEATH_NOTIFICATION,
                     "%d:%d %s %016llx\n",
                      proc->pid, thread->pid,
                      cmd == BR_DEAD_BINDER ?
                      "BR_DEAD_BINDER" :
                      "BR_CLEAR_DEATH_NOTIFICATION_DONE",
                      (u64)death->cookie);

            if (w->type == BINDER_WORK_CLEAR_DEATH_NOTIFICATION) {
                list_del(&w->entry);
                kfree(death);
                binder_stats_deleted(BINDER_STAT_DEATH);
            } else
                list_move(&w->entry, &proc->delivered_death);
            if (cmd == BR_DEAD_BINDER)
                goto done; /* DEAD_BINDER notifications can cause transactions */
        } break;
        }

        if (!t)  // 非BINDER_WORK_TRANSACTION 的情况，放弃执行后面的重新循环
            continue;

        /*
          这里的t所指向的binder_transaction结构体就是前面发送者thread建立的binder_transaction数据结构，
          所以这里如果为NULL，说明有异常。
        */
        BUG_ON(t->buffer == NULL);

        // 可以为NULL，如: BC_REPLY的时候。这个值是发送者task填入，对发送者来说才有意义。
        if (t->buffer->target_node) {

            //下面开始将binder_transaction转换成binder_transaction_data结构了
            struct binder_node *target_node = t->buffer->target_node;

            tr.target.ptr = target_node->ptr;        // binder实体的用户空间指针
            tr.cookie =  target_node->cookie;        // binder实体的额外数据
            t->saved_priority = task_nice(current);  // 保留当前task的nice值

            /*
             t->priority保存的是发送者thread的nice值，
             target_node->min_priority保存的是binder所在进程中处理请求线程的最低优先级，
             如果前者优先级高于后者，那么将当前task的优先级设置成发送者task的nice值。
            */
            if (t->priority < target_node->min_priority &&
                !(t->flags & TF_ONE_WAY))
                binder_set_nice(t->priority);      // 同步传输
            else if (!(t->flags & TF_ONE_WAY) ||
                 t->saved_priority > target_node->min_priority)
                /*
                 如果当前接收task的nice过低(saved_priority),那么使用binder_node
                 创建时候设置的任务处理线程的最低优先级(min_priority)。
                */
                binder_set_nice(target_node->min_priority);
            // 接收thread上层会收到BR_TRANSACTION|binder_transaction_data的数据包
            cmd = BR_TRANSACTION;
        } else {   // 如果发送者是发送的回复数据：BC_REPLY
            tr.target.ptr = 0;
            tr.cookie = 0;
            cmd = BR_REPLY;  // 接收thread上层会收到BR_REPLY
        }
        tr.code = t->code;  // 函数调用编号
        tr.flags = t->flags;  // transaction_flags: TF_ONE_WAY等
        tr.sender_euid = from_kuid(current_user_ns(), t->sender_euid);  // 发送进程的有效用户ID

        if (t->from) {
            // 记录发送线程的binder_thread
            struct task_struct *sender = t->from->proc->tsk;

            tr.sender_pid = task_tgid_nr_ns(sender,
                            task_active_pid_ns(current));
        } else {
            tr.sender_pid = 0;
        }

        /*
          接收方在这里完成数据从内核空间到用户空间的转移，其实没有实际的数据移动，
          而是buffer地址在内核空间和用户空间中的转换: user_buffer_offset。
        */
        tr.data_size = t->buffer->data_size;
        tr.offsets_size = t->buffer->offsets_size;
        // 需要加上这个偏移量才是用户空间的地址，这个偏移量是在mmap函数中计算出来的。
        tr.data.ptr.buffer = (binder_uintptr_t)(
                    (uintptr_t)t->buffer->data +
                    proc->user_buffer_offset);
        tr.data.ptr.offsets = tr.data.ptr.buffer +
                    ALIGN(t->buffer->data_size,
                        sizeof(void *));

        /*
          拷贝cmd和binder_transaction_data数据回用户空间,上层只需要准备这二者的内存空间即可。
          其余的数据均在binder_buffer之中呆着。
        */
        if (put_user(cmd, (uint32_t __user *)ptr))
            return -EFAULT;
        ptr += sizeof(uint32_t);
        if (copy_to_user(ptr, &tr, sizeof(tr)))
            return -EFAULT;
        ptr += sizeof(tr);

        trace_binder_transaction_received(t);
        binder_stat_br(proc, thread, cmd);       // 统计计数
        binder_debug(BINDER_DEBUG_TRANSACTION,
                 "%d:%d %s %d %d:%d, cmd %d size %zd-%zd ptr %016llx-%016llx\n",
                 proc->pid, thread->pid,
                 (cmd == BR_TRANSACTION) ? "BR_TRANSACTION" :
                 "BR_REPLY",
                 t->debug_id, t->from ? t->from->proc->pid : 0,
                 t->from ? t->from->pid : 0, cmd,
                 t->buffer->data_size, t->buffer->offsets_size,
                 (u64)tr.data.ptr.buffer, (u64)tr.data.ptr.offsets);

        // 从todo队列中删除对应的binder_work
        list_del(&t->work.entry);
        // 允许释放binder_buffer空间，驱动处理当前task的BC_FREE_BUFFER的时候会检查这个。
        t->buffer->allow_user_free = 1;
        if (cmd == BR_TRANSACTION && !(t->flags & TF_ONE_WAY)) {  // 同步, 请求数据
            /*
             这个部份表示了同一个binder_transaction在发送thread和接收thread中都有修改的部分。
             发送thread和接收thread的binder_thread.transaction_stack指向的是同一个binder_transcation结构体。
            */
            t->to_parent = thread->transaction_stack;
            t->to_thread = thread;
            thread->transaction_stack = t;  // 压入thread的tranaaction_stack栈顶
        } else {  // 同步，回复数据；异步
            // 无论收到的是回复数据或者是异步请求，这里所做的是一样，释放掉这个单边传输是产生的binder_transaction数据结构所占内存
            t->buffer->transaction = NULL;
            kfree(t);
            binder_stats_deleted(BINDER_STAT_TRANSACTION);
        }
        break;
    }

done:

    *consumed = ptr - buffer;  // 实际接收到的字节数
    /*
     检查当前进程中的空闲线程是否够用：如果正在请求的线程数和空闲的线程数都为0，
     请求的线程已经开始运行的数目小于运行开辟的最大线程数。

     当驱动发现接收方所有线程都处于忙碌状态, 且线程池里的线程总数没有超过BINDER_SET_MAX_THREADS设置的最大线程数时，
     向接收方发送该命令要求创建更多线程以备接收数据。
    */
    if (proc->requested_threads + proc->ready_threads == 0 &&
        proc->requested_threads_started < proc->max_threads &&
        (thread->looper & (BINDER_LOOPER_STATE_REGISTERED |
         BINDER_LOOPER_STATE_ENTERED)) /* the user-space code fails to */
         /*spawn a new thread if we leave this out */) {
        proc->requested_threads++;
        binder_debug(BINDER_DEBUG_THREADS,
                 "%d:%d BR_SPAWN_LOOPER\n",
                 proc->pid, thread->pid);
        if (put_user(BR_SPAWN_LOOPER, (uint32_t __user *)buffer))
            return -EFAULT;
        binder_stat_br(proc, thread, BR_SPAWN_LOOPER);
    }
    return 0;
}

static void binder_release_work(struct list_head *list)
{
    struct binder_work *w;

    while (!list_empty(list)) {
        w = list_first_entry(list, struct binder_work, entry);
        list_del_init(&w->entry);
        switch (w->type) {
        case BINDER_WORK_TRANSACTION: {
            struct binder_transaction *t;

            t = container_of(w, struct binder_transaction, work);
            if (t->buffer->target_node &&
                !(t->flags & TF_ONE_WAY)) {
                binder_send_failed_reply(t, BR_DEAD_REPLY);
            } else {
                binder_debug(BINDER_DEBUG_DEAD_TRANSACTION,
                    "undelivered transaction %d\n",
                    t->debug_id);
                t->buffer->transaction = NULL;
                kfree(t);
                binder_stats_deleted(BINDER_STAT_TRANSACTION);
            }
        } break;
        case BINDER_WORK_TRANSACTION_COMPLETE: {
            binder_debug(BINDER_DEBUG_DEAD_TRANSACTION,
                "undelivered TRANSACTION_COMPLETE\n");
            kfree(w);
            binder_stats_deleted(BINDER_STAT_TRANSACTION_COMPLETE);
        } break;
        case BINDER_WORK_DEAD_BINDER_AND_CLEAR:
        case BINDER_WORK_CLEAR_DEATH_NOTIFICATION: {
            struct binder_ref_death *death;

            death = container_of(w, struct binder_ref_death, work);
            binder_debug(BINDER_DEBUG_DEAD_TRANSACTION,
                "undelivered death notification, %016llx\n",
                (u64)death->cookie);
            kfree(death);
            binder_stats_deleted(BINDER_STAT_DEATH);
        } break;
        default:
            pr_err("unexpected work type, %d, not freed\n",
                   w->type);
            break;
        }
    }

}

static struct binder_thread *binder_get_thread(struct binder_proc *proc)
{
    struct binder_thread *thread = NULL;
    struct rb_node *parent = NULL;
    struct rb_node **p = &proc->threads.rb_node;

    // 搜索红黑树binder_proc.threads,这颗红黑树是以task的pid为索引值
    while (*p) {
        parent = *p;
        // 通过结构体成员的指针地址，得到这个结构体的地址 container_of(ptr, type, member)
        thread = rb_entry(parent, struct binder_thread, rb_node);

        if (current->pid < thread->pid)
            p = &(*p)->rb_left;
        else if (current->pid > thread->pid)
            p = &(*p)->rb_right;
        // 如果找到这个binder_thread结构体，立即退出查找，否则*p最终为NULL
        else
            break;
    }
    // 如果没找到，也就是当前thread第一次调用ioctl的时候，会新建binder_thread数据结构
    if (*p == NULL) {
        thread = kzalloc(sizeof(*thread), GFP_KERNEL);
        if (thread == NULL)
            return NULL;
        // 在binder_stats全局统计数据中，为新建的binder_thread对应的统计项加1。
        binder_stats_created(BINDER_STAT_THREAD);
        thread->proc = proc;
        thread->pid = current->pid;
        /*
          初始化thread的私有等待队列; 虽然名叫队列，其实线程私有等待队列中最多只有一个线程，即它自己。
        */
        init_waitqueue_head(&thread->wait);
        INIT_LIST_HEAD(&thread->todo);       // 初始化thread的私有任务队列
        rb_link_node(&thread->rb_node, parent, p);
        // 将新建的binder_thread结构体加入当前进程的红黑树binder_proc.threads中
        rb_insert_color(&thread->rb_node, &proc->threads);
        thread->looper |= BINDER_LOOPER_STATE_NEED_RETURN;
        // 初始化错误返回值
        thread->return_error = BR_OK;
        thread->return_error2 = BR_OK;
    }
    return thread;
}

static int binder_free_thread(struct binder_proc *proc,
                  struct binder_thread *thread)
{
    struct binder_transaction *t;
    struct binder_transaction *send_reply = NULL;
    int active_transactions = 0;

    rb_erase(&thread->rb_node, &proc->threads);
    t = thread->transaction_stack;
    if (t && t->to_thread == thread)
        send_reply = t;
    while (t) {
        active_transactions++;
        binder_debug(BINDER_DEBUG_DEAD_TRANSACTION,
                 "release %d:%d transaction %d %s, still active\n",
                  proc->pid, thread->pid,
                 t->debug_id,
                 (t->to_thread == thread) ? "in" : "out");

        if (t->to_thread == thread) {
            t->to_proc = NULL;
            t->to_thread = NULL;
            if (t->buffer) {
                t->buffer->transaction = NULL;
                t->buffer = NULL;
            }
            t = t->to_parent;
        } else if (t->from == thread) {
            t->from = NULL;
            t = t->from_parent;
        } else
            BUG();
    }
    if (send_reply)
        binder_send_failed_reply(send_reply, BR_DEAD_REPLY);
    binder_release_work(&thread->todo);
    kfree(thread);
    binder_stats_deleted(BINDER_STAT_THREAD);
    return active_transactions;
}

static unsigned int binder_poll(struct file *filp,
                struct poll_table_struct *wait)
{
    struct binder_proc *proc = filp->private_data;
    struct binder_thread *thread = NULL;
    int wait_for_proc_work;

    binder_lock(__func__);

    thread = binder_get_thread(proc);

    wait_for_proc_work = thread->transaction_stack == NULL &&
        list_empty(&thread->todo) && thread->return_error == BR_OK;

    binder_unlock(__func__);

    if (wait_for_proc_work) {
        if (binder_has_proc_work(proc, thread))
            return POLLIN;
        poll_wait(filp, &proc->wait, wait);
        if (binder_has_proc_work(proc, thread))
            return POLLIN;
    } else {
        if (binder_has_thread_work(thread))
            return POLLIN;
        poll_wait(filp, &thread->wait, wait);
        if (binder_has_thread_work(thread))
            return POLLIN;
    }
    return 0;
}

static int binder_ioctl_write_read(struct file *filp,
                unsigned int cmd, unsigned long arg,
                struct binder_thread *thread)
{
    int ret = 0;
    struct binder_proc *proc = filp->private_data;
    unsigned int size = _IOC_SIZE(cmd);
    void __user *ubuf = (void __user *)arg;
    struct binder_write_read bwr;

    if (size != sizeof(struct binder_write_read)) {
        ret = -EINVAL;
        goto out;
    }
    if (copy_from_user(&bwr, ubuf, sizeof(bwr))) {
        ret = -EFAULT;
        goto out;
    }
    binder_debug(BINDER_DEBUG_READ_WRITE,
             "%d:%d write %lld at %016llx, read %lld at %016llx\n",
             proc->pid, thread->pid,
             (u64)bwr.write_size, (u64)bwr.write_buffer,
             (u64)bwr.read_size, (u64)bwr.read_buffer);

    if (bwr.write_size > 0) {  // 表示本次ioctl有待发送的数据
        ret = binder_thread_write(proc, thread,
                      bwr.write_buffer,
                      bwr.write_size,
                      &bwr.write_consumed);
        trace_binder_write_done(ret);
        if (ret < 0) {  // 成功返回0，出错小于0
            bwr.read_consumed = 0;
            if (copy_to_user(ubuf, &bwr, sizeof(bwr)))
                ret = -EFAULT;
            goto out;
        }
    }

    /*
      接收端的task也是利用ioctl()进行读操作的，前面列出的struct binder_write_read结构体中如果有包含读取数据的buffer和预备读取大小，
      那么在write完该传输的数据之后就会进行read的操作，如下：(整理下思路，以下所讨论的内容均属于另一个进程的某个task，
      和前面传输部分属于不同的task，所以调用ioctl的时候，所描述的当前task也是接收端的，而非发送端。
    */
    if (bwr.read_size > 0) {  // > 0, 表示本次ioctl想接收数据
        ret = binder_thread_read(proc, thread, bwr.read_buffer,
                     bwr.read_size,
                     &bwr.read_consumed,
                     filp->f_flags & O_NONBLOCK);
        trace_binder_read_done(ret);
        if (!list_empty(&proc->todo))
            wake_up_interruptible(&proc->wait);  // 读返回的时候如果发现todo任务队列中有待处理的任务，那么将会唤醒binder_proc.wait中下一个等待着的空闲线程
        if (ret < 0) {  // 成功返回0，出错小于0
            if (copy_to_user(ubuf, &bwr, sizeof(bwr)))
                ret = -EFAULT;
            goto out;
        }
    }
    binder_debug(BINDER_DEBUG_READ_WRITE,
             "%d:%d wrote %lld of %lld, read return %lld of %lld\n",
             proc->pid, thread->pid,
             (u64)bwr.write_consumed, (u64)bwr.write_size,
             (u64)bwr.read_consumed, (u64)bwr.read_size);
    if (copy_to_user(ubuf, &bwr, sizeof(bwr))) {
        ret = -EFAULT;
        goto out;
    }
out:
    return ret;
}

static int binder_ioctl_set_ctx_mgr(struct file *filp)
{
    int ret = 0;
    struct binder_proc *proc = filp->private_data;
    kuid_t curr_euid = current_euid();

    if (binder_context_mgr_node != NULL) {
        pr_err("BINDER_SET_CONTEXT_MGR already set\n");
        ret = -EBUSY;
        goto out;
    }
    ret = security_binder_set_context_mgr(proc->tsk);
    if (ret < 0)
        goto out;
    if (uid_valid(binder_context_mgr_uid)) {
        if (!uid_eq(binder_context_mgr_uid, curr_euid)) {
            pr_err("BINDER_SET_CONTEXT_MGR bad uid %d != %d\n",
                   from_kuid(&init_user_ns, curr_euid),
                   from_kuid(&init_user_ns,
                    binder_context_mgr_uid));
            ret = -EPERM;
            goto out;
        }
    } else {
        binder_context_mgr_uid = curr_euid;
    }
    binder_context_mgr_node = binder_new_node(proc, 0, 0);
    if (binder_context_mgr_node == NULL) {
        ret = -ENOMEM;
        goto out;
    }
    binder_context_mgr_node->local_weak_refs++;
    binder_context_mgr_node->local_strong_refs++;
    binder_context_mgr_node->has_strong_ref = 1;
    binder_context_mgr_node->has_weak_ref = 1;
out:
    return ret;
}

static long binder_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret;
    struct binder_proc *proc = filp->private_data;
    struct binder_thread *thread;
    unsigned int size = _IOC_SIZE(cmd);
    void __user *ubuf = (void __user *)arg;

    /*pr_info("binder_ioctl: %d:%d %x %lx\n",
            proc->pid, current->pid, cmd, arg);*/

    trace_binder_ioctl(cmd, arg);

    /*
      进入时检查binder_stop_on_user_error（module参数stop_on_user_error），确定是否wait。
       stop_on_user_error，设置为0时，不起作用，设置为1时，如果某个进程调用binder时发生错误，则该进程sleep，
       以后别的进程再调用binder的ioctl也会sleep，此时你若往这个参数写入一个小于2的数，进程会被唤醒。
       设置为2以上的值，那悲剧了，ioctl直接睡觉。该参数默认为0。

       #define wait_event_interruptible(wq, condition)          \
       ({                                                       \
           int __ret = 0;                                       \
           if (!(condition))                                    \
           __wait_event_interruptible(wq, condition, __ret);\
           __ret;                                               \
       })
       #define __wait_event_interruptible(wq, condition, ret)                        \
       do {                                                                        \
            DEFINE_WAIT(__wait);                                                \
                                                                                \
            for (;;) {                                                        \
                prepare_to_wait(&wq, &__wait, TASK_INTERRUPTIBLE);        \
                if (condition)                                                \
                        break;                                                \
                if (!signal_pending(current)) {                                \
                        schedule();                                        \
                        continue;                                        \
                }                                                        \
                ret = -ERESTARTSYS;                                        \
                break;                                                        \
            }                                                                \
            finish_wait(&wq, &__wait);                                        \
        } while (0)
        __wait_event_interruptible()首先定义并初始化一个wait_queue_t变量__wait，其中数据为当前进程current，并把__wait入队。
        在无限循环中，__wait_event_interruptible()将本进程置为可中断的挂起状态，反复检查condition是否成立，
        如果成立则退出，如果不成立则继续休眠；条件满足后，即把本进程运行状态置为运行态(
        此时如果不执行下面的函数 wake_up_interruptible，上面wait_event_interruptible还会继续休眠)，并将__wait从等待队列中
        清除掉，从而进程能够调度运行。如果进程当前有异步信号(POSIX的)，则返回-ERESTARTSYS。
        1, 唤醒 q 指定的注册在等待队列上的进程。该函数不能直接的立即唤醒进程，而是由调度程序转换上下文，调整为可运行状态。
        2, wake_up_interruptible (wait_queue_head_t *q);

       ref: http://blog.csdn.net/sucjhwaxp/article/details/7915386
            http://blog.sina.com.cn/s/blog_4770ef020101h48l.html
    */
    ret = wait_event_interruptible(binder_user_error_wait, binder_stop_on_user_error < 2);
    if (ret)
        goto err_unlocked;

    binder_lock(__func__);
    thread = binder_get_thread(proc);   // 查找当前thread对应的binder_thread结构体，如果没找到就新建一个binder_thread，同时将其加入binder_proc的threads的红黑树中。
    if (thread == NULL) {
        ret = -ENOMEM;
        goto err;
    }

    switch (cmd) {
    case BINDER_WRITE_READ:
        ret = binder_ioctl_write_read(filp, cmd, arg, thread);
        if (ret)
            goto err;
        break;
    case BINDER_SET_MAX_THREADS:
        if (copy_from_user(&proc->max_threads, ubuf, sizeof(proc->max_threads))) {
            ret = -EINVAL;
            goto err;
        }
        break;
    case BINDER_SET_CONTEXT_MGR:
        ret = binder_ioctl_set_ctx_mgr(filp);
        if (ret)
            goto err;
        break;
    case BINDER_THREAD_EXIT:
        binder_debug(BINDER_DEBUG_THREADS, "%d:%d exit\n",
                 proc->pid, thread->pid);
        binder_free_thread(proc, thread);
        thread = NULL;
        break;
    case BINDER_VERSION: {
        struct binder_version __user *ver = ubuf;

        if (size != sizeof(struct binder_version)) {
            ret = -EINVAL;
            goto err;
        }
        if (put_user(BINDER_CURRENT_PROTOCOL_VERSION,
                 &ver->protocol_version)) {
            ret = -EINVAL;
            goto err;
        }
        break;
    }
    default:
        ret = -EINVAL;
        goto err;
    }
    ret = 0;
err:
    if (thread)
        thread->looper &= ~BINDER_LOOPER_STATE_NEED_RETURN;
    binder_unlock(__func__);
    wait_event_interruptible(binder_user_error_wait, binder_stop_on_user_error < 2);
    if (ret && ret != -ERESTARTSYS)
        pr_info("%d:%d ioctl %x %lx returned %d\n", proc->pid, current->pid, cmd, arg, ret);
err_unlocked:
    trace_binder_ioctl_done(ret);
    return ret;
}

static void binder_vma_open(struct vm_area_struct *vma)
{
    struct binder_proc *proc = vma->vm_private_data;

    binder_debug(BINDER_DEBUG_OPEN_CLOSE,
             "%d open vm area %lx-%lx (%ld K) vma %lx pagep %lx\n",
             proc->pid, vma->vm_start, vma->vm_end,
             (vma->vm_end - vma->vm_start) / SZ_1K, vma->vm_flags,
             (unsigned long)pgprot_val(vma->vm_page_prot));
}

static void binder_vma_close(struct vm_area_struct *vma)
{
    struct binder_proc *proc = vma->vm_private_data;

    binder_debug(BINDER_DEBUG_OPEN_CLOSE,
             "%d close vm area %lx-%lx (%ld K) vma %lx pagep %lx\n",
             proc->pid, vma->vm_start, vma->vm_end,
             (vma->vm_end - vma->vm_start) / SZ_1K, vma->vm_flags,
             (unsigned long)pgprot_val(vma->vm_page_prot));
    proc->vma = NULL;
    proc->vma_vm_mm = NULL;
    binder_defer_work(proc, BINDER_DEFERRED_PUT_FILES);
}

static int binder_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
    return VM_FAULT_SIGBUS;
}

static const struct vm_operations_struct binder_vm_ops = {
    .open = binder_vma_open,
    .close = binder_vma_close,
    .fault = binder_vm_fault,
};

/*
binder驱动实现了自己的mmap函数，正如universus阐述的那样：它不是为了在物理
介质和用户空间做映射，而是用来创建数据接收的缓存空间。
Binder数据只在用户空间和内核空间拷贝一次的秘密也就在于binder驱动对接收缓冲区的管理。

如果某应用程序只进行异步请求，那么我们可以不用给对应进程分配和管理接收缓存区了。
另外binder_mmap()只允许最大分配4MB的虚拟地址空间，而且对于应用程序，只拥有对该内存的读权限。

mmap函数实际上在为用户空间分配了一段虚拟地址空间vma之后，以这个为参数传递给binder_mmap()函数，
这个函数中首先开辟一段内核的虚拟地址空间，保存在binder_proc.buffer域，初始化一些其他域之后，
就分配一页物理内存，将其先映射到内核虚拟地址段，随后将这页物理页插入到用户空间的虚拟地址段中去。
最后将整个内核虚拟空间看做一个binder_buffer插入到以buffer大小为索引的红黑树free_buffers上。
*/
static int binder_mmap(struct file *filp, struct vm_area_struct *vma)
{
    int ret;
    struct vm_struct *area;
    struct binder_proc *proc = filp->private_data;
    const char *failure_string;
    struct binder_buffer *buffer;

    if (proc->tsk != current)
        return -EINVAL;

    if ((vma->vm_end - vma->vm_start) > SZ_4M)
        vma->vm_end = vma->vm_start + SZ_4M;

    binder_debug(BINDER_DEBUG_OPEN_CLOSE,
             "binder_mmap: %d %lx-%lx (%ld K) vma %lx pagep %lx\n",
             proc->pid, vma->vm_start, vma->vm_end,
             (vma->vm_end - vma->vm_start) / SZ_1K, vma->vm_flags,
             (unsigned long)pgprot_val(vma->vm_page_prot));

    if (vma->vm_flags & FORBIDDEN_MMAP_FLAGS) { // 只能被映射成只读
        ret = -EPERM;
        failure_string = "bad vm_flags";
        goto err_bad_arg;
    }
    vma->vm_flags = (vma->vm_flags | VM_DONTCOPY) & ~VM_MAYWRITE;  // fork的时候不拷贝这段vma; VM_MAYWRITE: 尝试写操作

    mutex_lock(&binder_mmap_lock);
    if (proc->buffer) {
        ret = -EBUSY;
        failure_string = "already mapped";
        goto err_already_mapped;
    }

    area = get_vm_area(vma->vm_end - vma->vm_start, VM_IOREMAP);
    if (area == NULL) {
        ret = -ENOMEM;
        failure_string = "get_vm_area";
        goto err_get_vm_area_failed;
    }
    proc->buffer = area->addr;  // 内核虚拟内存空间的起始地址
    // 被映射的区域的起始地址在用户空间和内核空间之间的偏移，可以为负数。
    proc->user_buffer_offset = vma->vm_start - (uintptr_t)proc->buffer;
    mutex_unlock(&binder_mmap_lock);

#ifdef CONFIG_CPU_CACHE_VIPT
    if (cache_is_vipt_aliasing()) {
        while (CACHE_COLOUR((vma->vm_start ^ (uint32_t)proc->buffer))) {
            pr_info("binder_mmap: %d %lx-%lx maps %p bad alignment\n", proc->pid, vma->vm_start, vma->vm_end, proc->buffer);
            vma->vm_start += PAGE_SIZE;
        }
    }
#endif
    // 分配存放struct page结构体指针的指针数组内存空间，主要用来保存指向申请的物理页struct page的指针。
    // 最大申请4MB的虚拟地址空间，所以这里的struct page结构体指针数组,数组最大长度是1024
    proc->pages = kzalloc(sizeof(proc->pages[0]) * ((vma->vm_end - vma->vm_start) / PAGE_SIZE), GFP_KERNEL);
    if (proc->pages == NULL) {
        ret = -ENOMEM;
        failure_string = "alloc page array";
        goto err_alloc_pages_failed;
    }
    proc->buffer_size = vma->vm_end - vma->vm_start;  // 内核分配的那段虚拟地址空间大小

    vma->vm_ops = &binder_vm_ops;
    vma->vm_private_data = proc;

    /*
        这里它是在分配物理页并映射到刚才保留的虚拟内存空间上。当然根据参数，它也可以释放物理页面。
        在这里，Binder使用 alloc_page分配页面，使用map_vm_area为分配的物理内存做映射，
        使用vm_insert_page把分配的物理页插入到用户进程的当前vma区域。
    */
    if (binder_update_page_range(proc, 1, proc->buffer, proc->buffer + PAGE_SIZE, vma)) {
        ret = -ENOMEM;
        failure_string = "alloc small buf";
        goto err_alloc_small_buf_failed;
    }
    buffer = proc->buffer;  // 准备用binder_buffer来格式化这片内核虚拟地址空间

    /*
       此刻刚刚分配了内核虚拟地址空间，也刚刚分配了1页的物理内存，
       这个时候将这一页的物理内存看做是一个binder_buffer结构体，插入到binder_proc.buffers链表中。
    */
    INIT_LIST_HEAD(&proc->buffers);
    list_add(&buffer->entry, &proc->buffers);
    buffer->free = 1;
    binder_insert_free_buffer(proc, buffer);    // 以binder_buffer的size为索引将自身挂入当前进程红黑树free_buffers中
    proc->free_async_space = proc->buffer_size / 2;  // 空闲的异步传输空间
    barrier();
    proc->files = get_files_struct(current);
    proc->vma = vma;
    proc->vma_vm_mm = vma->vm_mm;

    /*pr_info("binder_mmap: %d %lx-%lx maps %p\n",
         proc->pid, vma->vm_start, vma->vm_end, proc->buffer);*/
    return 0;

err_alloc_small_buf_failed:
    kfree(proc->pages);
    proc->pages = NULL;
err_alloc_pages_failed:
    mutex_lock(&binder_mmap_lock);
    vfree(proc->buffer);
    proc->buffer = NULL;
err_get_vm_area_failed:
err_already_mapped:
    mutex_unlock(&binder_mmap_lock);
err_bad_arg:
    pr_err("binder_mmap: %d %lx-%lx %s failed %d\n",
           proc->pid, vma->vm_start, vma->vm_end, failure_string, ret);
    return ret;
}

static int binder_open(struct inode *nodp, struct file *filp)
{
    struct binder_proc *proc;

    binder_debug(BINDER_DEBUG_OPEN_CLOSE, "binder_open: %d:%d\n",
             current->group_leader->pid, current->pid);

    proc = kzalloc(sizeof(*proc), GFP_KERNEL);
    if (proc == NULL)
        return -ENOMEM;
    get_task_struct(current);  // 增加当前进程的引用计数
    proc->tsk = current;
    INIT_LIST_HEAD(&proc->todo);
    init_waitqueue_head(&proc->wait);
    proc->default_priority = task_nice(current);

    binder_lock(__func__);

    binder_stats_created(BINDER_STAT_PROC);             // 在全局统计计数数据结构binder_stats中记录下创建了一个binder_proc
    hlist_add_head(&proc->proc_node, &binder_procs);    // 将代表当前进程的binder_proc结构体加入全局链表binder_procs中

    /*
      如果当前task是进程，那么取得当前进程的pid；
      如果当前task是线程，那么此处取得创建该线程的进程的pid: current->tgid tgid = thread group id
    */
    proc->pid = current->group_leader->pid;
    INIT_LIST_HEAD(&proc->delivered_death);
    filp->private_data = proc;

    binder_unlock(__func__);

    if (binder_debugfs_dir_entry_proc) {
        char strbuf[11];

        snprintf(strbuf, sizeof(strbuf), "%u", proc->pid);
        // 以proc->pid作为名字在proc/binder/proc中创建一个入口文件，以方便上层查看该进程的所有binder通讯
        proc->debugfs_entry = debugfs_create_file(strbuf, S_IRUGO,
            binder_debugfs_dir_entry_proc, proc, &binder_proc_fops);
    }

    return 0;
}

static int binder_flush(struct file *filp, fl_owner_t id)
{
    struct binder_proc *proc = filp->private_data;

    binder_defer_work(proc, BINDER_DEFERRED_FLUSH);

    return 0;
}

static void binder_deferred_flush(struct binder_proc *proc)
{
    struct rb_node *n;
    int wake_count = 0;

    for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n)) {
        struct binder_thread *thread = rb_entry(n, struct binder_thread, rb_node);

        thread->looper |= BINDER_LOOPER_STATE_NEED_RETURN;
        if (thread->looper & BINDER_LOOPER_STATE_WAITING) {
            wake_up_interruptible(&thread->wait);
            wake_count++;
        }
    }
    wake_up_interruptible_all(&proc->wait);

    binder_debug(BINDER_DEBUG_OPEN_CLOSE,
             "binder_flush: %d woke %d threads\n", proc->pid,
             wake_count);
}

static int binder_release(struct inode *nodp, struct file *filp)
{
    struct binder_proc *proc = filp->private_data;

    debugfs_remove(proc->debugfs_entry);
    binder_defer_work(proc, BINDER_DEFERRED_RELEASE);

    return 0;
}

static int binder_node_release(struct binder_node *node, int refs)
{
    struct binder_ref *ref;
    int death = 0;

    list_del_init(&node->work.entry);
    binder_release_work(&node->async_todo);

    if (hlist_empty(&node->refs)) {
        kfree(node);
        binder_stats_deleted(BINDER_STAT_NODE);

        return refs;
    }

    node->proc = NULL;
    node->local_strong_refs = 0;
    node->local_weak_refs = 0;
    hlist_add_head(&node->dead_node, &binder_dead_nodes);

    hlist_for_each_entry(ref, &node->refs, node_entry) {
        refs++;

        if (!ref->death)
            continue;

        death++;

        if (list_empty(&ref->death->work.entry)) {
            ref->death->work.type = BINDER_WORK_DEAD_BINDER;
            list_add_tail(&ref->death->work.entry,
                      &ref->proc->todo);
            wake_up_interruptible(&ref->proc->wait);
        } else
            BUG();
    }

    binder_debug(BINDER_DEBUG_DEAD_BINDER,
             "node %d now dead, refs %d, death %d\n",
             node->debug_id, refs, death);

    return refs;
}

static void binder_deferred_release(struct binder_proc *proc)
{
    struct binder_transaction *t;
    struct rb_node *n;
    int threads, nodes, incoming_refs, outgoing_refs, buffers,
        active_transactions, page_count;

    BUG_ON(proc->vma);
    BUG_ON(proc->files);

    hlist_del(&proc->proc_node);

    if (binder_context_mgr_node && binder_context_mgr_node->proc == proc) {
        binder_debug(BINDER_DEBUG_DEAD_BINDER,
                 "%s: %d context_mgr_node gone\n",
                 __func__, proc->pid);
        binder_context_mgr_node = NULL;
    }

    threads = 0;
    active_transactions = 0;
    while ((n = rb_first(&proc->threads))) {
        struct binder_thread *thread;

        thread = rb_entry(n, struct binder_thread, rb_node);
        threads++;
        active_transactions += binder_free_thread(proc, thread);
    }

    nodes = 0;
    incoming_refs = 0;
    while ((n = rb_first(&proc->nodes))) {
        struct binder_node *node;

        node = rb_entry(n, struct binder_node, rb_node);
        nodes++;
        rb_erase(&node->rb_node, &proc->nodes);
        incoming_refs = binder_node_release(node, incoming_refs);
    }

    outgoing_refs = 0;
    while ((n = rb_first(&proc->refs_by_desc))) {
        struct binder_ref *ref;

        ref = rb_entry(n, struct binder_ref, rb_node_desc);
        outgoing_refs++;
        binder_delete_ref(ref);
    }

    binder_release_work(&proc->todo);
    binder_release_work(&proc->delivered_death);

    buffers = 0;
    while ((n = rb_first(&proc->allocated_buffers))) {
        struct binder_buffer *buffer;

        buffer = rb_entry(n, struct binder_buffer, rb_node);

        t = buffer->transaction;
        if (t) {
            t->buffer = NULL;
            buffer->transaction = NULL;
            pr_err("release proc %d, transaction %d, not freed\n",
                   proc->pid, t->debug_id);
            /*BUG();*/
        }

        binder_free_buf(proc, buffer);
        buffers++;
    }

    binder_stats_deleted(BINDER_STAT_PROC);

    page_count = 0;
    if (proc->pages) {
        int i;

        for (i = 0; i < proc->buffer_size / PAGE_SIZE; i++) {
            void *page_addr;

            if (!proc->pages[i])
                continue;

            page_addr = proc->buffer + i * PAGE_SIZE;
            binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
                     "%s: %d: page %d at %p not freed\n",
                     __func__, proc->pid, i, page_addr);
            unmap_kernel_range((unsigned long)page_addr, PAGE_SIZE);
            __free_page(proc->pages[i]);
            page_count++;
        }
        kfree(proc->pages);
        vfree(proc->buffer);
    }

    put_task_struct(proc->tsk);

    binder_debug(BINDER_DEBUG_OPEN_CLOSE,
             "%s: %d threads %d, nodes %d (ref %d), refs %d, active transactions %d, buffers %d, pages %d\n",
             __func__, proc->pid, threads, nodes, incoming_refs,
             outgoing_refs, active_transactions, buffers, page_count);

    kfree(proc);
}

static void binder_deferred_func(struct work_struct *work)
{
    struct binder_proc *proc;
    struct files_struct *files;

    int defer;

    do {
        binder_lock(__func__);
        mutex_lock(&binder_deferred_lock);
        if (!hlist_empty(&binder_deferred_list)) {
            proc = hlist_entry(binder_deferred_list.first,
                    struct binder_proc, deferred_work_node);
            hlist_del_init(&proc->deferred_work_node);
            defer = proc->deferred_work;
            proc->deferred_work = 0;
        } else {
            proc = NULL;
            defer = 0;
        }
        mutex_unlock(&binder_deferred_lock);

        files = NULL;
        if (defer & BINDER_DEFERRED_PUT_FILES) {
            files = proc->files;
            if (files)
                proc->files = NULL;
        }

        if (defer & BINDER_DEFERRED_FLUSH)
            binder_deferred_flush(proc);

        if (defer & BINDER_DEFERRED_RELEASE)
            binder_deferred_release(proc); /* frees proc */

        binder_unlock(__func__);
        if (files)
            put_files_struct(files);
    } while (proc);
}
static DECLARE_WORK(binder_deferred_work, binder_deferred_func);

static void
binder_defer_work(struct binder_proc *proc, enum binder_deferred_state defer)
{
    mutex_lock(&binder_deferred_lock);
    proc->deferred_work |= defer;
    if (hlist_unhashed(&proc->deferred_work_node)) {
        hlist_add_head(&proc->deferred_work_node,
                &binder_deferred_list);
        queue_work(binder_deferred_workqueue, &binder_deferred_work);
    }
    mutex_unlock(&binder_deferred_lock);
}

static void print_binder_transaction(struct seq_file *m, const char *prefix,
                     struct binder_transaction *t)
{
    seq_printf(m,
           "%s %d: %p from %d:%d to %d:%d code %x flags %x pri %ld r%d",
           prefix, t->debug_id, t,
           t->from ? t->from->proc->pid : 0,
           t->from ? t->from->pid : 0,
           t->to_proc ? t->to_proc->pid : 0,
           t->to_thread ? t->to_thread->pid : 0,
           t->code, t->flags, t->priority, t->need_reply);
    if (t->buffer == NULL) {
        seq_puts(m, " buffer free\n");
        return;
    }
    if (t->buffer->target_node)
        seq_printf(m, " node %d",
               t->buffer->target_node->debug_id);
    seq_printf(m, " size %zd:%zd data %p\n",
           t->buffer->data_size, t->buffer->offsets_size,
           t->buffer->data);
}

static void print_binder_buffer(struct seq_file *m, const char *prefix,
                struct binder_buffer *buffer)
{
    seq_printf(m, "%s %d: %p size %zd:%zd %s\n",
           prefix, buffer->debug_id, buffer->data,
           buffer->data_size, buffer->offsets_size,
           buffer->transaction ? "active" : "delivered");
}

static void print_binder_work(struct seq_file *m, const char *prefix,
                  const char *transaction_prefix,
                  struct binder_work *w)
{
    struct binder_node *node;
    struct binder_transaction *t;

    switch (w->type) {
    case BINDER_WORK_TRANSACTION:
        t = container_of(w, struct binder_transaction, work);
        print_binder_transaction(m, transaction_prefix, t);
        break;
    case BINDER_WORK_TRANSACTION_COMPLETE:
        seq_printf(m, "%stransaction complete\n", prefix);
        break;
    case BINDER_WORK_NODE:
        node = container_of(w, struct binder_node, work);
        seq_printf(m, "%snode work %d: u%016llx c%016llx\n",
               prefix, node->debug_id,
               (u64)node->ptr, (u64)node->cookie);
        break;
    case BINDER_WORK_DEAD_BINDER:
        seq_printf(m, "%shas dead binder\n", prefix);
        break;
    case BINDER_WORK_DEAD_BINDER_AND_CLEAR:
        seq_printf(m, "%shas cleared dead binder\n", prefix);
        break;
    case BINDER_WORK_CLEAR_DEATH_NOTIFICATION:
        seq_printf(m, "%shas cleared death notification\n", prefix);
        break;
    default:
        seq_printf(m, "%sunknown work: type %d\n", prefix, w->type);
        break;
    }
}

static void print_binder_thread(struct seq_file *m,
                struct binder_thread *thread,
                int print_always)
{
    struct binder_transaction *t;
    struct binder_work *w;
    size_t start_pos = m->count;
    size_t header_pos;

    seq_printf(m, "  thread %d: l %02x\n", thread->pid, thread->looper);
    header_pos = m->count;
    t = thread->transaction_stack;
    while (t) {
        if (t->from == thread) {
            print_binder_transaction(m,
                         "    outgoing transaction", t);
            t = t->from_parent;
        } else if (t->to_thread == thread) {
            print_binder_transaction(m,
                         "    incoming transaction", t);
            t = t->to_parent;
        } else {
            print_binder_transaction(m, "    bad transaction", t);
            t = NULL;
        }
    }
    list_for_each_entry(w, &thread->todo, entry) {
        print_binder_work(m, "    ", "    pending transaction", w);
    }
    if (!print_always && m->count == header_pos)
        m->count = start_pos;
}

static void print_binder_node(struct seq_file *m, struct binder_node *node)
{
    struct binder_ref *ref;
    struct binder_work *w;
    int count;

    count = 0;
    hlist_for_each_entry(ref, &node->refs, node_entry)
        count++;

    seq_printf(m, "  node %d: u%016llx c%016llx hs %d hw %d ls %d lw %d is %d iw %d",
           node->debug_id, (u64)node->ptr, (u64)node->cookie,
           node->has_strong_ref, node->has_weak_ref,
           node->local_strong_refs, node->local_weak_refs,
           node->internal_strong_refs, count);
    if (count) {
        seq_puts(m, " proc");
        hlist_for_each_entry(ref, &node->refs, node_entry)
            seq_printf(m, " %d", ref->proc->pid);
    }
    seq_puts(m, "\n");
    list_for_each_entry(w, &node->async_todo, entry)
        print_binder_work(m, "    ",
                  "    pending async transaction", w);
}

static void print_binder_ref(struct seq_file *m, struct binder_ref *ref)
{
    seq_printf(m, "  ref %d: desc %d %snode %d s %d w %d d %p\n",
           ref->debug_id, ref->desc, ref->node->proc ? "" : "dead ",
           ref->node->debug_id, ref->strong, ref->weak, ref->death);
}

static void print_binder_proc(struct seq_file *m,
                  struct binder_proc *proc, int print_all)
{
    struct binder_work *w;
    struct rb_node *n;
    size_t start_pos = m->count;
    size_t header_pos;

    seq_printf(m, "proc %d\n", proc->pid);
    header_pos = m->count;

    for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n))
        print_binder_thread(m, rb_entry(n, struct binder_thread,
                        rb_node), print_all);
    for (n = rb_first(&proc->nodes); n != NULL; n = rb_next(n)) {
        struct binder_node *node = rb_entry(n, struct binder_node,
                            rb_node);
        if (print_all || node->has_async_transaction)
            print_binder_node(m, node);
    }
    if (print_all) {
        for (n = rb_first(&proc->refs_by_desc);
             n != NULL;
             n = rb_next(n))
            print_binder_ref(m, rb_entry(n, struct binder_ref,
                             rb_node_desc));
    }
    for (n = rb_first(&proc->allocated_buffers); n != NULL; n = rb_next(n))
        print_binder_buffer(m, "  buffer",
                    rb_entry(n, struct binder_buffer, rb_node));
    list_for_each_entry(w, &proc->todo, entry)
        print_binder_work(m, "  ", "  pending transaction", w);
    list_for_each_entry(w, &proc->delivered_death, entry) {
        seq_puts(m, "  has delivered dead binder\n");
        break;
    }
    if (!print_all && m->count == header_pos)
        m->count = start_pos;
}

static const char * const binder_return_strings[] = {
    "BR_ERROR",
    "BR_OK",
    "BR_TRANSACTION",
    "BR_REPLY",
    "BR_ACQUIRE_RESULT",
    "BR_DEAD_REPLY",
    "BR_TRANSACTION_COMPLETE",
    "BR_INCREFS",
    "BR_ACQUIRE",
    "BR_RELEASE",
    "BR_DECREFS",
    "BR_ATTEMPT_ACQUIRE",
    "BR_NOOP",
    "BR_SPAWN_LOOPER",
    "BR_FINISHED",
    "BR_DEAD_BINDER",
    "BR_CLEAR_DEATH_NOTIFICATION_DONE",
    "BR_FAILED_REPLY"
};

static const char * const binder_command_strings[] = {
    "BC_TRANSACTION",
    "BC_REPLY",
    "BC_ACQUIRE_RESULT",
    "BC_FREE_BUFFER",
    "BC_INCREFS",
    "BC_ACQUIRE",
    "BC_RELEASE",
    "BC_DECREFS",
    "BC_INCREFS_DONE",
    "BC_ACQUIRE_DONE",
    "BC_ATTEMPT_ACQUIRE",
    "BC_REGISTER_LOOPER",
    "BC_ENTER_LOOPER",
    "BC_EXIT_LOOPER",
    "BC_REQUEST_DEATH_NOTIFICATION",
    "BC_CLEAR_DEATH_NOTIFICATION",
    "BC_DEAD_BINDER_DONE"
};

static const char * const binder_objstat_strings[] = {
    "proc",
    "thread",
    "node",
    "ref",
    "death",
    "transaction",
    "transaction_complete"
};

static void print_binder_stats(struct seq_file *m, const char *prefix,
                   struct binder_stats *stats)
{
    int i;

    BUILD_BUG_ON(ARRAY_SIZE(stats->bc) !=
             ARRAY_SIZE(binder_command_strings));
    for (i = 0; i < ARRAY_SIZE(stats->bc); i++) {
        if (stats->bc[i])
            seq_printf(m, "%s%s: %d\n", prefix,
                   binder_command_strings[i], stats->bc[i]);
    }

    BUILD_BUG_ON(ARRAY_SIZE(stats->br) !=
             ARRAY_SIZE(binder_return_strings));
    for (i = 0; i < ARRAY_SIZE(stats->br); i++) {
        if (stats->br[i])
            seq_printf(m, "%s%s: %d\n", prefix,
                   binder_return_strings[i], stats->br[i]);
    }

    BUILD_BUG_ON(ARRAY_SIZE(stats->obj_created) !=
             ARRAY_SIZE(binder_objstat_strings));
    BUILD_BUG_ON(ARRAY_SIZE(stats->obj_created) !=
             ARRAY_SIZE(stats->obj_deleted));
    for (i = 0; i < ARRAY_SIZE(stats->obj_created); i++) {
        if (stats->obj_created[i] || stats->obj_deleted[i])
            seq_printf(m, "%s%s: active %d total %d\n", prefix,
                binder_objstat_strings[i],
                stats->obj_created[i] - stats->obj_deleted[i],
                stats->obj_created[i]);
    }
}

static void print_binder_proc_stats(struct seq_file *m,
                    struct binder_proc *proc)
{
    struct binder_work *w;
    struct rb_node *n;
    int count, strong, weak;

    seq_printf(m, "proc %d\n", proc->pid);
    count = 0;
    for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n))
        count++;
    seq_printf(m, "  threads: %d\n", count);
    seq_printf(m, "  requested threads: %d+%d/%d\n"
            "  ready threads %d\n"
            "  free async space %zd\n", proc->requested_threads,
            proc->requested_threads_started, proc->max_threads,
            proc->ready_threads, proc->free_async_space);
    count = 0;
    for (n = rb_first(&proc->nodes); n != NULL; n = rb_next(n))
        count++;
    seq_printf(m, "  nodes: %d\n", count);
    count = 0;
    strong = 0;
    weak = 0;
    for (n = rb_first(&proc->refs_by_desc); n != NULL; n = rb_next(n)) {
        struct binder_ref *ref = rb_entry(n, struct binder_ref,
                          rb_node_desc);
        count++;
        strong += ref->strong;
        weak += ref->weak;
    }
    seq_printf(m, "  refs: %d s %d w %d\n", count, strong, weak);

    count = 0;
    for (n = rb_first(&proc->allocated_buffers); n != NULL; n = rb_next(n))
        count++;
    seq_printf(m, "  buffers: %d\n", count);

    count = 0;
    list_for_each_entry(w, &proc->todo, entry) {
        switch (w->type) {
        case BINDER_WORK_TRANSACTION:
            count++;
            break;
        default:
            break;
        }
    }
    seq_printf(m, "  pending transactions: %d\n", count);

    print_binder_stats(m, "  ", &proc->stats);
}


static int binder_state_show(struct seq_file *m, void *unused)
{
    struct binder_proc *proc;
    struct binder_node *node;
    int do_lock = !binder_debug_no_lock;

    if (do_lock)
        binder_lock(__func__);

    seq_puts(m, "binder state:\n");

    if (!hlist_empty(&binder_dead_nodes))
        seq_puts(m, "dead nodes:\n");
    hlist_for_each_entry(node, &binder_dead_nodes, dead_node)
        print_binder_node(m, node);

    hlist_for_each_entry(proc, &binder_procs, proc_node)
        print_binder_proc(m, proc, 1);
    if (do_lock)
        binder_unlock(__func__);
    return 0;
}

static int binder_stats_show(struct seq_file *m, void *unused)
{
    struct binder_proc *proc;
    int do_lock = !binder_debug_no_lock;

    if (do_lock)
        binder_lock(__func__);

    seq_puts(m, "binder stats:\n");

    print_binder_stats(m, "", &binder_stats);

    hlist_for_each_entry(proc, &binder_procs, proc_node)
        print_binder_proc_stats(m, proc);
    if (do_lock)
        binder_unlock(__func__);
    return 0;
}

static int binder_transactions_show(struct seq_file *m, void *unused)
{
    struct binder_proc *proc;
    int do_lock = !binder_debug_no_lock;

    if (do_lock)
        binder_lock(__func__);

    seq_puts(m, "binder transactions:\n");
    hlist_for_each_entry(proc, &binder_procs, proc_node)
        print_binder_proc(m, proc, 0);
    if (do_lock)
        binder_unlock(__func__);
    return 0;
}

static int binder_proc_show(struct seq_file *m, void *unused)
{
    struct binder_proc *itr;
    struct binder_proc *proc = m->private;
    int do_lock = !binder_debug_no_lock;
    bool valid_proc = false;

    if (do_lock)
        binder_lock(__func__);

    hlist_for_each_entry(itr, &binder_procs, proc_node) {
        if (itr == proc) {
            valid_proc = true;
            break;
        }
    }
    if (valid_proc) {
        seq_puts(m, "binder proc state:\n");
        print_binder_proc(m, proc, 1);
    }
    if (do_lock)
        binder_unlock(__func__);
    return 0;
}

static void print_binder_transaction_log_entry(struct seq_file *m,
                    struct binder_transaction_log_entry *e)
{
    seq_printf(m,
           "%d: %s from %d:%d to %d:%d node %d handle %d size %d:%d\n",
           e->debug_id, (e->call_type == 2) ? "reply" :
           ((e->call_type == 1) ? "async" : "call "), e->from_proc,
           e->from_thread, e->to_proc, e->to_thread, e->to_node,
           e->target_handle, e->data_size, e->offsets_size);
}

static int binder_transaction_log_show(struct seq_file *m, void *unused)
{
    struct binder_transaction_log *log = m->private;
    int i;

    if (log->full) {
        for (i = log->next; i < ARRAY_SIZE(log->entry); i++)
            print_binder_transaction_log_entry(m, &log->entry[i]);
    }
    for (i = 0; i < log->next; i++)
        print_binder_transaction_log_entry(m, &log->entry[i]);
    return 0;
}

static const struct file_operations binder_fops = {
    .owner = THIS_MODULE,
    .poll = binder_poll,
    .unlocked_ioctl = binder_ioctl,
    .compat_ioctl = binder_ioctl,
    .mmap = binder_mmap,
    .open = binder_open,
    .flush = binder_flush,
    .release = binder_release,
};

static struct miscdevice binder_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "binder",
    .fops = &binder_fops
};

BINDER_DEBUG_ENTRY(state);
BINDER_DEBUG_ENTRY(stats);
BINDER_DEBUG_ENTRY(transactions);
BINDER_DEBUG_ENTRY(transaction_log);

/*
 binder_init()函数。这个函数其实也没做太多的事情：

 1. 新建工作队列：binder_deferred_workqueue和工作者内核线程:binder来处理一些可以稍后执行的工作；

 2. 为binder驱动注册一个misc设备，设备节点/dev/binder；

 3. 创建proc下的相关目录和文件:

    proc/binder、proc/binder/proc，

    proc/state、proc/stats、proc/transactions、proc/transaction_log、proc/failed_transaction_log。

    这些接口文件对应的proc读取函数分别是：binder_read_proc_state()、binder_read_proc_stats()、
    binder_read_proc_transactions()、binder_read_proc_transaction_log()、binder_read_proc_transaction_log()。

    所以如果需要对这几个文件进行详细研究的，这些函数就不容错过了。

    另外在binder.c文件中看到的以print_binder_开头的函数都是和这些属性接口文件有关，如下。

        print_binder_transaction()

        print_binder_buffer()

        print_binder_work()

        print_binder_thread()

        print_binder_node()

        print_binder_ref()

        print_binder_proc ()

        print_binder_stats()

        print_binder_proc_stats()

        binder_read_proc_state()

        binder_read_proc_stats()

        binder_read_proc_transactions()

        binder_read_proc_proc()

        print_binder_transaction_log_entry()

        binder_read_proc_transaction_log()
*/
static int __init binder_init(void)
{
    int ret;

    binder_deferred_workqueue = create_singlethread_workqueue("binder");
    if (!binder_deferred_workqueue)
        return -ENOMEM;

    binder_debugfs_dir_entry_root = debugfs_create_dir("binder", NULL);
    if (binder_debugfs_dir_entry_root)
        binder_debugfs_dir_entry_proc = debugfs_create_dir("proc",
                         binder_debugfs_dir_entry_root);
    ret = misc_register(&binder_miscdev);
    if (binder_debugfs_dir_entry_root) {
        debugfs_create_file("state",
                    S_IRUGO,
                    binder_debugfs_dir_entry_root,
                    NULL,
                    &binder_state_fops);
        debugfs_create_file("stats",
                    S_IRUGO,
                    binder_debugfs_dir_entry_root,
                    NULL,
                    &binder_stats_fops);
        debugfs_create_file("transactions",
                    S_IRUGO,
                    binder_debugfs_dir_entry_root,
                    NULL,
                    &binder_transactions_fops);
        debugfs_create_file("transaction_log",
                    S_IRUGO,
                    binder_debugfs_dir_entry_root,
                    &binder_transaction_log,
                    &binder_transaction_log_fops);
        debugfs_create_file("failed_transaction_log",
                    S_IRUGO,
                    binder_debugfs_dir_entry_root,
                    &binder_transaction_log_failed,
                    &binder_transaction_log_fops);
    }
    return ret;
}

device_initcall(binder_init);

#define CREATE_TRACE_POINTS
#include "binder_trace.h"

MODULE_LICENSE("GPL v2");
