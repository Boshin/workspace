#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <netinet/in.h>
#include "unp.h"

// #define SERV_PORT        9877           /* TCP and UDP client-servers */

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
// #define LISTENQ     1024    /* 2nd argument to listen() */

// #define MAXLINE     4096    /* max text line length */

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

void str_echo(int connfd) {
    char buf[MAXLINE] = { 0 };
    int n;

again:
    while ((n = read(connfd, buf, MAXLINE)) > 0) {
        writen(connfd, buf, n);
    }

    if ((n < 0) && (EINTR == errno)) {
        goto again;
    }
    else if (n < 0) {
        printf("str_echo read error\n");
    }
}

int main(int argc, char *argv[])
{
    int listenfd = -1, connfd = -1 ;
    int maxfd = -1, maxi = -1, i = 0, nready =-1, n = -1;
    socklen_t clilen = 0;
    struct sockaddr_in cliaddr, serveraddr;
    fd_set rset, allset;
    char buf[MAXLINE] = { -1 };
    int client[FD_SETSIZE] = { -1 };

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    maxfd = listenfd;

    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port        = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);

    FD_ZERO(&rset);
    FD_ZERO(&allset);
    FD_SET(listenfd, &rset);

    // sleep(5);

    while(1) {
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (nready > 0) {
            if (FD_ISSET(listenfd, &rset)) {    // new client connection
                clilen = sizeof(cliaddr);
                connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);

                if (connfd >= 0) {
                    for (i = 0; i < FD_SETSIZE; ++i) {
                        if (client[i] < 0) {
                            client[i] = connfd; // save descriptor
                            break;
                        }
                    }

                    if (i == FD_SETSIZE) {
                        printf("too many clients\n");
                        return -1;
                    }

                    FD_SET(connfd, &rset); // add new descriptor to set

                    if (connfd > maxfd) {
                        maxfd = connfd; // for select
                    }

                    if (i > maxi) {
                        maxi = i;   // max index in client[] array
                    }

                    if (--nready <= 0) {
                        continue;   // no more readable descriptors
                    }
                }
                else {
                    if (EINTR == errno) {
                        continue;   // back to loop
                    }
                    else {
                        printf("accept error[%s]", strerror(errno));
                        break;
                    }
                }
            }
            else {
                for (i = 0; i <= maxi; ++i) {  // check all clients for data
                    if (client[i] > 0) {
                        if (FD_ISSET(client[i], &rset)) {
                            if ((n = read(client[i], buf, MAXLINE)) > 0) {
                                Writen(client[i], buf, MAXLINE);
                            }
                        }
                        else {  // connection closed by client
                            close(client[i]);
                            FD_CLR(client[i], &rset);
                            client[i] = -1;
                        }

                        if (--nready <= 0) {
                            break;  // no more readable descriptors
                        }
                    }
                }
            }
        }
        else {
            if (EINTR == errno) {
                continue;
            }
            else {
                printf("select error[%s]", strerror(errno));
            }
        }
    }

    close(listenfd);

    return 0;
}
