#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/devpoll.h>
#include "unp.h"

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
    char buf[MAXLINE] = { 0 };
    int infd = fileno(fp);
    int stdineof = 0;
    int n = 0;
    int nready = 0;
    struct pollfd pollrd[2];
    struct dvpoll;

    pollrd[1].fd      = sockfd;
    pollrd[1].events  = POLLIN;
    pollrd[1].revents = 0;

    pollrd[0].fd      = infd;
    pollrd[0].events  = POLLIN;
    pollrd[0].revents = 0;

    int wfd = open("/dev/poll", O_RDWR, 0);

    write(wfd, pollrd, sizeof(struct pollfd) * 2);

    while (1) {
        // block until /dev/poll says something is ready
        dopoll.dp_timeout = -1;
        dopoll.dp_nfds    = 2;
        dopoll.dp_fds     = pollfd;
        nready = ioctl(wfd, DP_POLL, &dopoll);

        int i = 0;
        for (i = 0; i < nready; ++i) {
            if (dopoll.dp_fds[i].fd = sockfd) {
                // sock is readable
                if ((n = read(sockfd, buf, MAXLINE)) == 0) {
                    if (1 == stdineof) {
                        return; // normal termination 
                    }
                    else {
                        printf("str_cli: server terminated prematurely\n");
                        return;
                    }
                }
                else {
                    // input is readable
                    if ((n = read(infd, buf, maxline)) != 0) {  // input is readable
                        writen(sockfd, buf, n);
                    }
                    else {
                        stdineof = 1;
                        printf("close client write edge\n");
                        shutdown(sockfd, shut_wr);  // send fin
                        continue;
                    }
                }
            }
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
