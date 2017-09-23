#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME   "/tmp/my_fifo"            //!> FIFO全名
#define  BUFFER_SIZE 40                              //!> PIPE_BUF，linux 2.6.11 以前，是4096，以后是65536。
#define TEN_MEG     36 * 10                        //!> ( 1024 * 1024 * 10)            

int main()
{
    int pipe_fd;
    int res;
    int open_mode =O_WRONLY;                     //!> 生产者：只是写data就可以了～

    int bytes =0;
    char buffer[BUFFER_SIZE + 1] ="abcdefghijklmnopqrstuvwxyz";      //!> 36 个测试字符

    if( access(FIFO_NAME, F_OK) == -1)            //!> if 文件不存在( if 文件存在，那么access返回值是0 )
    {
        res =mkfifo( FIFO_NAME, 0777);            //!> 文件不存在就创建文件

        if( res != 0)                                 //!> 文件创建成功就返回0，否则返回-1，此处就是没有成功的情况
        {
            fprintf(stderr, "Can't create fifo %s\n", FIFO_NAME );
            exit(EXIT_FAILURE );
        }
    }

    printf("Process %d opening  FIFO O_WRONLY \n", getpid());

    pipe_fd =open( FIFO_NAME, open_mode);                  //!> 打开创建的文件是只写模式 （ 注意open返回的是文件描述符 ）
    //!>从此处开始：生产者开始写文件，所以需要启动Device，那么就由“消费者”来执行CPU
    printf("Process %d ---> FILE  ID： %d\n",getpid(), pipe_fd );

    if( pipe_fd!= -1 )                        //!>对于open来说，成功返回FILE_ID，失败返回-1 ( 此处是成功～ )
    {   
        char test_char ='0';   

        while( bytes< TEN_MEG)                             //!> 指定大小为TEN_MEG
        {
            //!>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>      
            test_char++;                  //!> 作为测试字符(标志每次读出来的是不是写入的还是仅仅是开头的数据 )
            int len =strlen( buffer);                                //!> 测试字符串处理( 仅仅是测试而已 )
            buffer[len]= test_char;                                 
            buffer[len+1] = '\0';
            //!>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

            res = write(pipe_fd, buffer, BUFFER_SIZE ); //!> 将数据写到文件中   (注意返回的是一次写入的字节数 )

            if( res ==-1 )
            {
                fprintf(stderr, "Write error on pipe\n" );
                exit(EXIT_FAILURE );
            }

            printf("生产者写入：--->  ");
            puts( buffer);
            bytes +=res;
        }
        close(pipe_fd );                                          //!> 关闭文件描述符( 也就是关闭文件s )
    }
    else
    {
        exit(EXIT_FAILURE );
    }

    printf("Process %d   finish... \n",getpid() );

    exit( 0);
}
