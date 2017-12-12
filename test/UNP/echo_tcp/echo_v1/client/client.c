#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "unp.h"

#define SERV_PORT   9877    /* TCP and UDP client-servers */
#define MAXLINE     4096    /* max text line length */

static int  read_cnt;
static char *read_ptr;
static char read_buf[MAXLINE];

static ssize_t my_read(int fd, char *ptr)
{

    if (read_cnt <= 0) {
again:
        if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
            if (errno == EINTR)
                goto again;
            return(-1);
        } else if (read_cnt == 0)
            return(0);
        read_ptr = read_buf;
    }

    read_cnt--;
    *ptr = *read_ptr++;
    return(1);
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char    c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
        if ( (rc = my_read(fd, &c)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;  /* newline is stored, like fgets() */
        } else if (rc == 0) {
            *ptr = 0;
            return(n - 1);  /* EOF, n - 1 bytes were read */
        } else
            return(-1);     /* error, errno set by read() */
    }

    *ptr = 0;   /* null terminate like fgets() */
    return(n);
}

ssize_t readlinebuf(void **vptrptr)
{
    if (read_cnt)
        *vptrptr = read_ptr;
    return(read_cnt);
}
/* end readline */

ssize_t Readline(int fd, void *ptr, size_t maxlen)
{
    ssize_t     n;

    if ( (n = readline(fd, ptr, maxlen)) < 0)
        printf("readline error\n");
    return(n);
}

/* Write "n" bytes to a descriptor. */
ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t      nleft;
    ssize_t     nwritten;
    const char  *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;       /* and call write() again */
            else
                return(-1);         /* error */
        }

        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}
/* end writen */

void Writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
        printf("writen error\n");
}

void str_cli(FILE *fp, int sockfd)
{
    char sndline[MAXLINE] = { 0 };
    char rcvline[MAXLINE] = { 0 };

    while (fgets(sndline, MAXLINE, fp) != NULL) {
        printf("call server\n");
        Writen(sockfd, sndline, strlen(sndline));

        if (Readline(sockfd, rcvline, MAXLINE) == 0) {
            printf("str_cli: server terminated prematurely\n");
        }

        printf("server answer me: ");
        fputs(rcvline, stdout);
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    int sockfd[20] = { -1 };
    int i;
    struct sockaddr_in serveraddr;

    if (3 != argc) {
        printf("argument error\n");
        return -1;
    }

    int cliCnt = atoi(argv[1]);

    for (i = 0; i < cliCnt; ++i) {
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);

        bzero(&serveraddr, sizeof(serveraddr));

        serveraddr.sin_family = AF_INET;
        inet_pton(AF_INET, argv[1], &serveraddr.sin_addr);
        serveraddr.sin_port   = htons(SERV_PORT);

        connect(sockfd[i], (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    }

    str_cli(stdin, sockfd[0]);

    return 0;
}
