#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_PORT   9877    /* TCP and UDP client-servers */
#define MAXLINE     4096    /* max text line length */

void dg_cli(FILE *fp, int sockfd, struct sockaddr* addr, socklen_t len)
{
    char sndline[MAXLINE] = { 0 }; 
    char rcvline[MAXLINE + 1] = { 0 };
    connect(sockfd, addr, len);
    int bufsize = 220 * 1024;

    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));

    while (fgets(sndline, MAXLINE, fp) != NULL) {
        //sendto(sockfd, sndline, MAXLINE, 0, addr, len );
        write(sockfd, sndline, strlen(sndline));
        // int n = recvfrom(sockfd, rcvline, MAXLINE, 0, NULL, NULL);
        int n = read(sockfd, rcvline, MAXLINE);
        rcvline[n] = 0; // null terminate
        printf("server answer:\n");
        fputs(rcvline, stdout);
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    int sockfd[20] = { -1 };
    struct sockaddr_in serveraddr;
    int i;

    if (3 != argc) {
        printf("argument error\n");
        return -1;
    }

    int cliCnt = atoi(argv[1]);

    for (i = 0; i < cliCnt; ++i) {
        sockfd[i] = socket(AF_INET, SOCK_DGRAM, 0);

        bzero(&serveraddr, sizeof(serveraddr));

        serveraddr.sin_family = AF_INET;
        inet_pton(AF_INET, argv[1], &serveraddr.sin_addr);
        serveraddr.sin_port   = htons(SERV_PORT);

        dg_cli(stdin, sockfd[i], (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    }

    return 0;
}
