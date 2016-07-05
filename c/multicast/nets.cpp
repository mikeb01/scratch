//
// Created by Michael Barker on 31/08/15.
//

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <cinttypes>
#include <memory>
#include <iostream>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <libkern/OSByteOrder.h>
#include <tuple>


const char* family_name(int ai_family)
{
    switch (ai_family)
    {
        case AF_INET:
            return "AF_INET";
        case AF_INET6:
            return "AF_INET6";
        default:
            return "N/A";
    }
}

void info(const char* node, const char* service)
{
    struct addrinfo hint, * res = NULL;
    memset(&hint, '\0', sizeof hint);

    hint.ai_family = PF_UNSPEC;
    hint.ai_flags = AI_NUMERICHOST;
    hint.ai_socktype = SOCK_DGRAM;

    int ret = 0;

    if ((ret = getaddrinfo(node, service, NULL, &res)) != 0)
    {
        printf("grr: %s\n", gai_strerror(ret));
    }
    else
    {
        printf("%s - Family: %s\n", node, family_name(res->ai_family));
    }

    freeaddrinfo(res);
}

uint32_t maskToPrefixLength(in_addr addr)
{
    return __builtin_popcount(addr.s_addr);
}

uint32_t maskToPrefixLength(in6_addr addr)
{
    union _u
    {
        in6_addr addr;
        struct
        {
            uint64_t hi;
            uint64_t lo;
        } parts;
    };

    union _u cvt;

    cvt.addr = addr;

    return __builtin_popcountll(cvt.parts.hi) + __builtin_popcountll(cvt.parts.lo);
}

int main()
{
//    const char* ipv4 = "127.0.0.1";
//    const char* ipv6 = "::1";
//    struct in_addr ip1;
//    char data[INET6_ADDRSTRLEN];
//    char out[INET6_ADDRSTRLEN];
//    char* p;
//
//    p = (char*) &ip1;
//
//    int i = inet_pton(AF_INET, ipv4, p);
//    printf("inet_pton(AF_INET, ipv4, data) = 0x%x, 0x%x, 0x%x, 0x%x\n", p[0], p[1], p[2], p[3]);
//    printf("sizeof(INET6_ADDRSTRLEN) = %d\n", INET6_ADDRSTRLEN);
//    printf("sizeof(struct sockaddr_in6) = %d\n", sizeof(struct in6_addr));
//
//    inet_ntop(AF_INET, data, out, INET6_ADDRSTRLEN);
//    printf("foo = %s\n", out);
//
//    info(ipv4, NULL);
//    info(ipv6, NULL);
//    info("localhost", NULL);
//    info("www.google.com", NULL);
//    info("127.0.0.0", NULL);
//
//    in6_addr addr;
//    inet_pton(AF_INET6, "::FF", &addr);
//
//    std::cout << (int) addr.__u6_addr.__u6_addr8[15]  << '\n';
//    addr.__u6_addr.__u6_addr8[15]++;
//    std::cout << (int) addr.__u6_addr.__u6_addr8[15]  << '\n';
//    std::cout << (int) addr.__u6_addr.__u6_addr8[14]  << '\n';

    struct ifaddrs* interfaces;

    getifaddrs(&interfaces);

    struct ifaddrs* cursor = interfaces;
    char name[INET6_ADDRSTRLEN];

    while (cursor)
    {
        name[0] = '\0';

        if (cursor->ifa_netmask)
        {
            sa_family_t family = cursor->ifa_netmask->sa_family;
            const char* c;
            uint32_t subnetPrefix = 0;
            if (family == PF_INET)
            {
                sockaddr_in* sockaddrIn = (sockaddr_in*) cursor->ifa_netmask;
                inet_ntop(PF_INET, &sockaddrIn->sin_addr, name, INET6_ADDRSTRLEN);
                subnetPrefix = maskToPrefixLength(sockaddrIn->sin_addr);
                c = "IPv4";
            }
            else if (family == PF_INET6)
            {
                sockaddr_in6* sockaddrIn = (sockaddr_in6*) cursor->ifa_netmask;
                subnetPrefix = maskToPrefixLength(sockaddrIn->sin6_addr);
                inet_ntop(PF_INET6, &sockaddrIn->sin6_addr, name, INET6_ADDRSTRLEN);
                c = "IPv6";
            }
            else
            {
                c = "Unknown";
            }

            bool multicast = (cursor->ifa_flags & IFF_MULTICAST) != 0;

            std::cout <<
                "family: " << c << " (" << (int) family << "), " <<
                "name: " << cursor->ifa_name << ", " <<
                "mask: " << name << "(" << subnetPrefix << "), "
                "mulicast: " << multicast << '\n';
        }


        cursor = cursor->ifa_next;
    }

    int ti = 5;
    std::tuple<int&> t = std::make_tuple(ti);

    return 0;
}