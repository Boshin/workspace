#include <poll.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stropts.h>
#include <netinet/in.h>
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

int main(int argc, char *argv[])
{
    // printf("echo V4\n");
    int listenfd = -1, connfd = -1 ;
    int maxi = -1, i = 0, nready =-1, n = -1;
    socklen_t clilen = 0;
    struct sockaddr_in cliaddr, serveraddr;
    char buf[MAXLINE] = { -1 };

    // struct pollfd client[OPEN_MAX];  // macro OPEN_MAX doesn't support anymore since linux 2.6.24
    long openmax = sysconf(_SC_OPEN_MAX);
    // printf("limit: %ld\n", openmax);
    struct pollfd *client = (struct pollfd*)malloc(sizeof(struct pollfd) * openmax);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port        = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    maxi = 0;

    for (i = 1; i < openmax; i++) {
        client[i].fd = -1;
    }

    // sleep(5);

    while(1) {
        nready = poll(client, maxi + 1, -1);    // macro INFTIM doesn't support anymore in linux os
        if (nready > 0) {
            if (client[0].revents & POLLRDNORM) {    // new client connection
                printf("server main new client connection\n");
                clilen = sizeof(cliaddr);
                connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);

                if (connfd >= 0) {
                    for (i = 1; i < openmax; ++i) {
                        if (client[i].fd < 0) {
                            client[i].fd = connfd; // save descriptor
                            break;
                        }
                    }

                    if (i == openmax) {
                        printf("too many clients\n");
                        free(client);
                        return -1;
                    }

                    client[i].events = POLLRDNORM;

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
                for (i = 1; i <= maxi; ++i) {  // check all clients for data
                    if (client[i].fd > 0) {
                        if (client[i].revents & (POLLRDNORM | POLLERR)) {
                            if ((n = read(client[i].fd, buf, MAXLINE)) > 0) {
                                Writen(client[i].fd, buf, MAXLINE);
                                // write(fileno(stdout), buf, MAXLINE);

                                // sleep(3);
                                // Writen(client[i].fd, "aaa\n", 4);
                                // Writen(client[i].fd, "bbb\n", 4);
                            }
                            else if(n == 0) {
                                // connection reset by client
                                close(client[i].fd);
                                client[i].fd = -1;
                            }
                            else {  // n < 0
                                if (ECONNRESET == errno) {
                                    close(client[i].fd);
                                    client[i].fd = -1;
                                }
                                else {
                                    printf("read error\n");
                                }
                            }

                            if (--nready <= 0) {
                                break;  // no more readable descriptors
                            }
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
                printf("poll error[%s]", strerror(errno));
            }
        }
    }

    close(listenfd);
    free(client);
    return 0;
}
