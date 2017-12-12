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
    int maxfdp1 = max(infd, sockfd) + 1;
    int stdineof = 0;
    int n = 0;
    fd_set rset;
    FD_ZERO(&rset);

    while (1) {
        if (stdineof == 0) {
            FD_SET(infd, &rset);
        }

        FD_SET(sockfd, &rset);

        int ret = select(maxfdp1, &rset, NULL, NULL, NULL);

        if (ret >= 0) {
            if (FD_ISSET(infd, &rset)) {
                if ((n = read(infd, buf, MAXLINE)) != 0) {  // input is readable
                    printf("send msg to server\n");
                    Writen(sockfd, buf, n);
                }
                else {
                    stdineof = 1;
                    shutdown(sockfd, SHUT_WR);  // send FIN
                    FD_CLR(infd, &rset);
                    continue;
                }
                --ret;
            }

            // printf("client ret: %d\n", ret);
            if (FD_ISSET(sockfd, &rset)) {    // socket is readable
                if ((n = read(sockfd, buf, MAXLINE)) == 0) {
                    if (1 == stdineof) {
                        return; // all done
                    }
                    else {
                        printf("str_cli: server terminated prematurely\n");
                        return;
                    }
                }

                printf("echo server answer: \n");
                write(fileno(stdout), buf, n);
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
