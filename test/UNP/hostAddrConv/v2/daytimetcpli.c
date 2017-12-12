#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "unp.h"

int main(int argc, char* argv[])
{
    int sockfd, n;
    struct sockaddr_in servaddr;
    struct in_addr **pptr;
    struct in_addr *inetaddrp[2];
    struct in_addr inetaddr;
    struct hostent *hptr;
    struct servent *sptr;
    char rcvline[MAXLINE + 1] = { 0 };

    if (3 != argc) {
        printf("usage: daytimecpli <hostname> <service>");
    }

    if ((hptr = gethostbyname(argv[1])) == NULL) {
        if (inet_aton(argv[1], &inetaddr) == 0) {
            printf("hostname error of %s: %s\n", argv[1], hstrerror(h_errno));
            return -1;
        }
        else {
            inetaddrp[0] = &inetaddr;
            inetaddrp[1] = NULL;
            pptr = inetaddrp;
        }
    }
    else {
        pptr = (struct in_addr**)(hptr->h_addr_list);
    }

    if ((sptr = getservbyname(argv[2], "tcp")) == NULL) {
        printf("getservername error for %s\n", argv[2]);
        return -1;
    }

    for (; *pptr != NULL; ++pptr) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        bzero(&servaddr, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = sptr->s_port;
        memcpy(&servaddr.sin_addr, *pptr, sizeof(struct in_addr));
        printf("try %s\n", Sock_ntop((struct sockaddr*)&servaddr, sizeof(servaddr)));

        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == 0) {
            break;
        }

        printf("connect error\n");
        close(sockfd);
    }

    if (*pptr == NULL) {
        printf("Unable to connect\n");
        return -1;
    }

    while ((n = read(sockfd, rcvline, MAXLINE)) > 0) {
        rcvline[n] = 0; // null terminate
        fputs(rcvline, stdout);
    }

    return 0;
}
