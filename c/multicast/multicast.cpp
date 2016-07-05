#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "multicast.h"

int get_addr(const char* hostname,
         const char* service,
         int family,
         int socktype,
         int flags,
         struct sockaddr_in6* addr)
{
    struct addrinfo hints, * res, * ressave;
    int n, sockfd, retval;
    char name[INET6_ADDRSTRLEN];

    retval = -1;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    hints.ai_flags = flags;

    n = getaddrinfo(hostname, service, &hints, &res);

    if (n < 0)
    {
        fprintf(stderr,
                "getaddrinfo error:: [%s]\n",
                gai_strerror(n));
        return retval;
    }

    ressave = res;

    sockfd = -1;
    while (res)
    {
        inet_ntop(PF_INET6, &res->ai_addr->sa_data, name, INET6_ADDRSTRLEN);
        printf("Addr: %s, family: %d\n", name, res->ai_family);

        sockfd = socket(res->ai_family,
                        res->ai_socktype,
                        res->ai_protocol);

        if (!(sockfd < 0))
        {
            if (bind(sockfd, res->ai_addr, res->ai_addrlen) == 0)
            {
                close(sockfd);
                memcpy(addr, res->ai_addr, sizeof(*addr));
                retval = 0;
                break;
            }

            close(sockfd);
            sockfd = -1;
        }
        res = res->ai_next;
    }

    freeaddrinfo(ressave);

    return retval;
}

int joinGroup(int sockfd, int loopBack, int mcastTTL, struct sockaddr_in6* addr)
{
    int r1, r2, r3, r4, retval = 0;
    int rc = 0;

    retval = -1;

    struct ipv6_mreq mreq6;

    memcpy(&mreq6.ipv6mr_multiaddr, &(addr->sin6_addr), sizeof(struct in6_addr));

    mreq6.ipv6mr_interface = if_nametoindex("en0"); // cualquier interfaz

    E(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &loopBack, sizeof(loopBack)));
    E(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &mcastTTL, sizeof(mcastTTL)));
    E(setsockopt(sockfd, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq6, sizeof(mreq6)));
    E(setsockopt(sockfd, IPPROTO_IPV6 , IPV6_MULTICAST_IF, &mreq6.ipv6mr_interface, sizeof(mreq6.ipv6mr_interface)));

//    in6_addr iface = in6addr_any;


    return retval;
}


int isMulticast(struct sockaddr_in6* addr)
{
    return IN6_IS_ADDR_MULTICAST(&addr->sin6_addr);
}