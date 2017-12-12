#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>

#define SERV_PORT        9877           /* TCP and UDP client-servers */

#define MAXMSG     4096    /* max text line length */

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

void dg_echo(int sockfd)
{
    char buff[MAXMSG] = { 0 };
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);

    while(1) {
        int ret = recvfrom(sockfd, buff, MAXMSG, 0, (struct sockaddr*)&cliaddr, &clilen);
        if (ret >= 0) {
                printf("rcv cline msg\n");
                sendto(sockfd, buff, ret, 0, (struct sockaddr*)&cliaddr, clilen);
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
}

int main(int argc, char *argv[])
{
    struct sockaddr_in serveraddr;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port        = htons(SERV_PORT);

    bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    dg_echo(sockfd);
    close(sockfd);

    return 0;
}
