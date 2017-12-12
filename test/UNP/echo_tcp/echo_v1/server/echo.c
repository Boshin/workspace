#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
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

typedef void(*sigfunc)(int signo);

void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated\n", pid);
    }

    return;
}

sigfunc signalHandle(int signo, sigfunc func)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;   // SunOS 4.x
#endif
    }
    else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;     // SVR4, 4,4BSD
#endif
    }

    if (sigaction(signo, &act, &oact) < 0) {
        return (SIG_ERR);
    }

    return (oact.sa_handler);

}

int main(int argc, char *argv[])
{
    int listenfd = -1, connfd = -1;
    pid_t childPid = -1;
    socklen_t clilen = 0;
    struct sockaddr_in cliaddr, serveraddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port        = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);
    signalHandle(SIGCHLD, sig_chld);
    sleep(5);

    while(1) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);

        if (connfd >= 0) {
            if ((childPid = fork()) == 0) { // child process
                close(listenfd);    // close listening socket
                str_echo(connfd);   // process the request
                exit(0);
            }
            else if (childPid > 0) {
                printf("return parent process childPid: %d\n", getpid());
            }
            else {
                printf("fork error\n");
            }

            close(connfd); // parent closes connected socket
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

    close(listenfd);

    return 0;
}
