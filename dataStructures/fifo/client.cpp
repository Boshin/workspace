#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define  FIFO_NAME   "/tmp/my_fifo" 

//!> 注意消费者需要访问的路径就是“生产者”的写入路径文件(一个写，一个读)

#define  BUFFER_SIZE 40                          

//!>PIPE_BUF                  //!> PIPE_BUF，linux 2.6.11 以前，是4096，以后是65536。

int main()
{
    int pipe_fd;
    int res;
    int open_mode =O_RDONLY;                     //!> 消费者只读
    int bytes =0;
    char buffer[BUFFER_SIZE + 1];

    memset(buffer, '\0', sizeof( buffer ));            //!> 分配给buffer结束字符值而已

    printf("Process %d opening FIFO O_RDONLY \n", getpid() );

    pipe_fd =open( FIFO_NAME, open_mode);      //!> 打开已有的文件只读

    printf("Process %d result %d \n", getpid(), pipe_fd );

    if( pipe_fd!= -1 )
    {
        res = read(pipe_fd, buffer, BUFFER_SIZE);      //!> 读取第一次( 以防没有数据 )
        while( res> 0 )
        {
            bytes +=res;

            printf("消费者读出：--->  ");
            puts( buffer);

            res = read(pipe_fd, buffer, BUFFER_SIZE);   //!>从文件中读取data到buffer中   
        }

        close(pipe_fd );
    }
    else
    {
        exit(EXIT_FAILURE );
    }

    printf("Process %d finished, %d bytes read/n", getpid(),bytes); 
    exit(EXIT_SUCCESS);   
}
