#ifndef UNP_H
#define UNP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>

/* *INDENT-OFF* */
#ifndef INET_ADDRSTRLEN
/* $$.Ic INET_ADDRSTRLEN$$ */
#define INET_ADDRSTRLEN     16  /* "ddd.ddd.ddd.ddd\0"
                                    1234567890123456 */
#endif

/* Define following even if IPv6 not supported, so we can always allocate
   an adequately-sized buffer, without #ifdefs in the code. */
#ifndef INET6_ADDRSTRLEN
/* $$.Ic INET6_ADDRSTRLEN$$ */
#define INET6_ADDRSTRLEN    46  /* max size of IPv6 address string:
                   "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx" or
                   "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:ddd.ddd.ddd.ddd\0"
                    1234567890123456789012345678901234567890123456 */
#endif
/* *INDENT-ON* */

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define LISTENQ     1024    /* 2nd argument to listen() */

/* Miscellaneous constants */
#define MAXLINE     4096    /* max text line length */
#define MAXSOCKADDR  128    /* max socket address structure size */
#define BUFFSIZE    8192    /* buffer size for reads and writes */

/* Define some port number that can be used for client-servers */
#define SERV_PORT        9877           /* TCP and UDP client-servers */
#define SERV_PORT_STR   "9877"          /* TCP and UDP client-servers */

#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    char        portstr[8];
    static char str[128];       /* Unix domain is largest */

    switch (sa->sa_family) {
    case AF_INET: {
        struct sockaddr_in  *sin = (struct sockaddr_in *) sa;

        if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
            return(NULL);
        if (ntohs(sin->sin_port) != 0) {
            snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
            strcat(str, portstr);
        }
        return(str);
    }
/* end sock_ntop */

#ifdef  IPV6
    case AF_INET6: {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) sa;

        str[0] = '[';
        if (inet_ntop(AF_INET6, &sin6->sin6_addr, str + 1, sizeof(str) - 1) == NULL)
            return(NULL);
        if (ntohs(sin6->sin6_port) != 0) {
            snprintf(portstr, sizeof(portstr), "]:%d", ntohs(sin6->sin6_port));
            strcat(str, portstr);
            return(str);
        }
        return (str + 1);
    }
#endif

#ifdef  AF_UNIX
    case AF_UNIX: {
        struct sockaddr_un  *unp = (struct sockaddr_un *) sa;

            /* OK to have no pathname bound to the socket: happens on
               every connect() unless client calls bind() first. */
        if (unp->sun_path[0] == 0)
            strcpy(str, "(no pathname bound)");
        else
            snprintf(str, sizeof(str), "%s", unp->sun_path);
        return(str);
    }
#endif

#ifdef  HAVE_SOCKADDR_DL_STRUCT
    case AF_LINK: {
        struct sockaddr_dl  *sdl = (struct sockaddr_dl *) sa;

        if (sdl->sdl_nlen > 0)
            snprintf(str, sizeof(str), "%*s (index %d)",
                     sdl->sdl_nlen, &sdl->sdl_data[0], sdl->sdl_index);
        else
            snprintf(str, sizeof(str), "AF_LINK, index=%d", sdl->sdl_index);
        return(str);
    }
#endif
    default:
        snprintf(str, sizeof(str), "sock_ntop: unknown AF_xxx: %d, len %d",
                 sa->sa_family, salen);
        return(str);
    }
    return (NULL);
}

char *
Sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    char    *ptr;

    if ( (ptr = sock_ntop(sa, salen)) == NULL)
        printf("sock_ntop error\n"); /* inet_ntop() sets errno */
    return(ptr);
}

#endif  // UNP_H