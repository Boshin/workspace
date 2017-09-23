	.file	"140617.cpp"
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata
.LC0:
	.string	"CC"
	.text
	.align 2
	.globl	_ZN3funC2Ev
	.type	_ZN3funC2Ev, @function
_ZN3funC2Ev:
.LFB969:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	$_ZTV3fun+16, (%rax)
	movl	$.LC0, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE969:
	.size	_ZN3funC2Ev, .-_ZN3funC2Ev
	.section	.rodata
.LC1:
	.string	"dd"
	.text
	.align 2
	.globl	_ZN3funD2Ev
	.type	_ZN3funD2Ev, @function
_ZN3funD2Ev:
.LFB972:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	$_ZTV3fun+16, (%rax)
	movl	$.LC1, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	movl	$0, %eax
	andl	$1, %eax
	testb	%al, %al
	je	.L2
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	_ZdlPv
.L2:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE972:
	.size	_ZN3funD2Ev, .-_ZN3funD2Ev
	.align 2
	.globl	_ZN3funD0Ev
	.type	_ZN3funD0Ev, @function
_ZN3funD0Ev:
.LFB974:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN3funD1Ev
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	_ZdlPv
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE974:
	.size	_ZN3funD0Ev, .-_ZN3funD0Ev
	.section	.rodata
.LC2:
	.string	"print"
	.text
	.align 2
	.globl	_ZN3fun5printEv
	.type	_ZN3fun5printEv, @function
_ZN3fun5printEv:
.LFB975:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	$.LC2, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE975:
	.size	_ZN3fun5printEv, .-_ZN3fun5printEv
	.globl	main
	.type	main, @function
main:
.LFB976:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA976
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$32, %rsp
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	movl	$8, %edi
.LEHB0:
	.cfi_offset 3, -32
	.cfi_offset 12, -24
	call	_Znwm
.LEHE0:
	movq	%rax, %rbx
	movq	%rbx, %rdi
.LEHB1:
	call	_ZN3funC1Ev
.LEHE1:
	movq	%rbx, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, -24(%rbp)
	cmpq	$0, -32(%rbp)
	je	.L9
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	addq	$8, %rax
	movq	(%rax), %rdx
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
.LEHB2:
	call	*%rdx
.L9:
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN3fun5printEv
	movl	$0, %eax
	addq	$32, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L11:
	.cfi_restore_state
	movq	%rax, %r12
	movq	%rbx, %rdi
	call	_ZdlPv
	movq	%r12, %rax
	movq	%rax, %rdi
	call	_Unwind_Resume
.LEHE2:
	.cfi_endproc
.LFE976:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA976:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE976-.LLSDACSB976
.LLSDACSB976:
	.uleb128 .LEHB0-.LFB976
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB976
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L11-.LFB976
	.uleb128 0
	.uleb128 .LEHB2-.LFB976
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSE976:
	.text
	.size	main, .-main
	.weak	_ZTV3fun
	.section	.rodata._ZTV3fun,"aG",@progbits,_ZTV3fun,comdat
	.align 32
	.type	_ZTV3fun, @object
	.size	_ZTV3fun, 32
_ZTV3fun:
	.quad	0
	.quad	_ZTI3fun
	.quad	_ZN3funD1Ev
	.quad	_ZN3funD0Ev
	.weak	_ZTI3fun
	.section	.rodata._ZTI3fun,"aG",@progbits,_ZTI3fun,comdat
	.align 16
	.type	_ZTI3fun, @object
	.size	_ZTI3fun, 16
_ZTI3fun:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTS3fun
	.weak	_ZTS3fun
	.section	.rodata._ZTS3fun,"aG",@progbits,_ZTS3fun,comdat
	.type	_ZTS3fun, @object
	.size	_ZTS3fun, 5
_ZTS3fun:
	.string	"3fun"
	.text
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB985:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$1, -4(%rbp)
	jne	.L12
	cmpl	$65535, -8(%rbp)
	jne	.L12
	movl	$_ZStL8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$_ZNSt8ios_base4InitD1Ev, %eax
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movq	%rax, %rdi
	call	__cxa_atexit
.L12:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE985:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I__ZN3funC2Ev, @function
_GLOBAL__sub_I__ZN3funC2Ev:
.LFB986:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE986:
	.size	_GLOBAL__sub_I__ZN3funC2Ev, .-_GLOBAL__sub_I__ZN3funC2Ev
	.section	.ctors,"aw",@progbits
	.align 8
	.quad	_GLOBAL__sub_I__ZN3funC2Ev
	.weakref	_ZL20__gthrw_pthread_oncePiPFvvE,pthread_once
	.weakref	_ZL27__gthrw_pthread_getspecificj,pthread_getspecific
	.weakref	_ZL27__gthrw_pthread_setspecificjPKv,pthread_setspecific
	.weakref	_ZL22__gthrw_pthread_createPmPK14pthread_attr_tPFPvS3_ES3_,pthread_create
	.weakref	_ZL20__gthrw_pthread_joinmPPv,pthread_join
	.weakref	_ZL21__gthrw_pthread_equalmm,pthread_equal
	.weakref	_ZL20__gthrw_pthread_selfv,pthread_self
	.weakref	_ZL22__gthrw_pthread_detachm,pthread_detach
	.weakref	_ZL22__gthrw_pthread_cancelm,pthread_cancel
	.weakref	_ZL19__gthrw_sched_yieldv,sched_yield
	.weakref	_ZL26__gthrw_pthread_mutex_lockP15pthread_mutex_t,pthread_mutex_lock
	.weakref	_ZL29__gthrw_pthread_mutex_trylockP15pthread_mutex_t,pthread_mutex_trylock
	.weakref	_ZL31__gthrw_pthread_mutex_timedlockP15pthread_mutex_tPK8timespec,pthread_mutex_timedlock
	.weakref	_ZL28__gthrw_pthread_mutex_unlockP15pthread_mutex_t,pthread_mutex_unlock
	.weakref	_ZL26__gthrw_pthread_mutex_initP15pthread_mutex_tPK19pthread_mutexattr_t,pthread_mutex_init
	.weakref	_ZL29__gthrw_pthread_mutex_destroyP15pthread_mutex_t,pthread_mutex_destroy
	.weakref	_ZL30__gthrw_pthread_cond_broadcastP14pthread_cond_t,pthread_cond_broadcast
	.weakref	_ZL27__gthrw_pthread_cond_signalP14pthread_cond_t,pthread_cond_signal
	.weakref	_ZL25__gthrw_pthread_cond_waitP14pthread_cond_tP15pthread_mutex_t,pthread_cond_wait
	.weakref	_ZL30__gthrw_pthread_cond_timedwaitP14pthread_cond_tP15pthread_mutex_tPK8timespec,pthread_cond_timedwait
	.weakref	_ZL28__gthrw_pthread_cond_destroyP14pthread_cond_t,pthread_cond_destroy
	.weakref	_ZL26__gthrw_pthread_key_createPjPFvPvE,pthread_key_create
	.weakref	_ZL26__gthrw_pthread_key_deletej,pthread_key_delete
	.weakref	_ZL30__gthrw_pthread_mutexattr_initP19pthread_mutexattr_t,pthread_mutexattr_init
	.weakref	_ZL33__gthrw_pthread_mutexattr_settypeP19pthread_mutexattr_ti,pthread_mutexattr_settype
	.weakref	_ZL33__gthrw_pthread_mutexattr_destroyP19pthread_mutexattr_t,pthread_mutexattr_destroy
	.globl	_ZN3funC1Ev
	.set	_ZN3funC1Ev,_ZN3funC2Ev
	.globl	_ZN3funD1Ev
	.set	_ZN3funD1Ev,_ZN3funD2Ev
	.ident	"GCC: (Ubuntu/Linaro 4.6.4-1ubuntu1) 4.6.4"
	.section	.note.GNU-stack,"",@progbits
