#include <arpa/inet.h>
#include "unp.h"

int main(int argc, char *argv[])
{
    // static char* addStr = "192.168.1.22";
    int domain = -1;
    char buf[sizeof(struct in6_addr)] = { 0 };
    char str[INET6_ADDRSTRLEN] = { 0 };

    if (3 != argc) {
        printf("arg error\n");

        return -1;
    }

    domain = strcmp(argv[1],  "i4") == 0 ? AF_INET : strcmp(argv[1], "i6") == 0 \
           ? AF_INET6 : atoi(argv[1]);
    int len = domain == AF_INET ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN;

    int ret = inet_pton(domain, argv[2], buf);

    if (1 == ret) {
        printf("pton success\n");
        if (NULL != inet_ntop(domain, buf, str, len))
            printf("%s", domain == AF_INET ? "ipv4" : "ipv6");
            printf(": %s\n", str);
    }
    else {
        printf("pton failed\n");
    }

    return 0;
}

