#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

    pollrd[1].fd      = sockfd;
    pollrd[1].events  = POLLRDNORM;
    pollrd[1].revents = 0;
    pollrd[0].fd      = infd;
    pollrd[0].events  = POLLWRNORM;
    pollrd[0].revents = 0;

    while (1) {
        nready = poll(pollrd, 2, -1);
        printf("nready: %d\n", nready);

        if (nready >= 0) {
            if (pollrd[1].revents & (POLLRDNORM | POLLERR)) {    // socket is readable
                if ((n = read(sockfd, buf, MAXLINE)) == 0) {
                    if (1 == stdineof) {
                        return; // all done
                    }
                    else {
                        printf("str_cli: server terminated prematurely\n");
                        return;
                    }
                }

                printf("echo server answer:\n");
                write(fileno(stdout), buf, n);

                --nready;
            }


            if (pollrd[0].revents & POLLWRNORM) {
                // printf("send msg to server:\n");
                if ((n = read(infd, buf, MAXLINE)) != 0) {  // input is readable
                    Writen(sockfd, buf, n);
                }
                else {
                    stdineof = 1;
                    printf("close client write edge\n");
                    shutdown(sockfd, SHUT_WR);  // send FIN
                    pollrd[1].fd = -1;
                    continue;
                }
            }

        }
        else {
            if (EINTR == errno) {
                continue;
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
