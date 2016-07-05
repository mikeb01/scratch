//
// Created by Michael Barker on 09/10/15.
//

#include <future>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <iostream>

const char* mcastaddr = "ff02::3";
const char* inf = "lo0";
const short PORT = 9877;

#define E(expr) CHECK((rc = (expr)) >= 0, #expr)
#define RES(err, expr) ((rc = expr) == (err) || (CHECK(!rc, #expr), 0))
#define CHECK(test, msg) ((test) ? (void)0 : ((void)fprintf(stderr, \
	"%s:%d: %s: %s\n", __FILE__, __LINE__, msg, strerror(rc)), abort()))

ssize_t multicastRecv(int sockfd, sockaddr_in6* addr)
{
    socklen_t len = sizeof(sockaddr_in6);
    char b[256];
    const char* retmsg = "def";
    ssize_t rc  = recvfrom(sockfd, b, sizeof(b), 0, (sockaddr*) addr, &len);

    if (rc < 0)
    {
        fprintf(stderr, "Recv failed, %s[%d]\n", strerror(errno), errno);
    }
    else
    {
        printf("Received: %s\n", b);
    }

    if (sendto(sockfd, retmsg, strlen(retmsg), 0, (const sockaddr*) addr, sizeof(sockaddr_in6)) < 0)
    {
        fprintf(stderr, "Send failed, %s[%d]\n", strerror(errno), errno);
    }

    return rc;
}

void initAddress(sockaddr_in6* sendaddr, short port)
{
    memset(sendaddr, 0, sizeof(*sendaddr));
    sendaddr->sin6_family = AF_INET6;
    sendaddr->sin6_addr = in6addr_any;
    sendaddr->sin6_port = htons(port);
}

int main()
{
    int rc = 0;
    addrinfo hints;
    addrinfo* result;
    sockaddr_in6 recvaddr;
    sockaddr_in6 bindaddr;
    sockaddr_in6 sendaddr;
    sockaddr_in6 clientaddr;
    const uint32_t yes = 1;
    const uint32_t no = 0;
    const uint32_t mcastTTL = 8;
    ipv6_mreq mreq6;
    char name[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(addrinfo));
    memset(&recvaddr, 0, sizeof(sockaddr_in6));

    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = 0;

    E(getaddrinfo(mcastaddr, NULL, &hints, &result));

    if (!result)
    {
        fprintf(stderr, "No results");
        return -1;
    }

    sockaddr_in6* _a = (sockaddr_in6*) result->ai_addr;
    inet_ntop(AF_INET6, &_a->sin6_addr, name, INET6_ADDRSTRLEN);
    printf("Addr: %s\n", name);

    memcpy(&recvaddr, result->ai_addr, sizeof(sockaddr_in6));
    freeaddrinfo(result);

    inet_ntop(AF_INET6, &recvaddr.sin6_addr, name, INET6_ADDRSTRLEN);
    printf("Addr: %s, scope: %d\n", name, recvaddr.sin6_scope_id);

    recvaddr.sin6_port = htons(PORT);

    E(socket(PF_INET6, SOCK_DGRAM, IPPROTO_IP));
    int recvSockFd = rc;

    std::cout << "Socket: " << recvSockFd << std::endl;

    memcpy(&mreq6.ipv6mr_multiaddr, &(recvaddr.sin6_addr), sizeof(struct in6_addr));
    mreq6.ipv6mr_interface = if_nametoindex(inf);

    E(setsockopt(recvSockFd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &no, sizeof(no)));
    E(setsockopt(recvSockFd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &mcastTTL, sizeof(mcastTTL)));
    E(setsockopt(recvSockFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)));

    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    E(setsockopt(recvSockFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)));

    initAddress(&bindaddr, PORT);
    E(bind(recvSockFd, (const sockaddr*) &bindaddr, sizeof(sockaddr_in6)));

    E(setsockopt(recvSockFd, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq6, sizeof(mreq6)));


    E(socket(PF_INET6, SOCK_DGRAM, IPPROTO_IP));
    int sendSockFd = rc;

    std::cout << "Socket: " << sendSockFd << std::endl;

    E(setsockopt(sendSockFd, IPPROTO_IPV6 , IPV6_MULTICAST_IF, &mreq6.ipv6mr_interface, sizeof(mreq6.ipv6mr_interface)));
    E(setsockopt(sendSockFd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &mcastTTL, sizeof(mcastTTL)));
    E(setsockopt(sendSockFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)));

    initAddress(&sendaddr, 0);
    E(bind(sendSockFd, (const sockaddr*) &sendaddr, sizeof(sockaddr_in6)));

    memcpy(&clientaddr, &recvaddr, sizeof(recvaddr));
    auto future = std::async(std::launch::async, multicastRecv, recvSockFd, &clientaddr);

    const char* msg = "abc";

    if (sendto(sendSockFd, msg, strlen(msg), 0, (const sockaddr*) &recvaddr, sizeof(sockaddr_in6)) < 0)
    {
        fprintf(stderr, "Send failed, %s[%d]\n", strerror(errno), errno);
    }

    int c = future.get();

    std::cout << c << std::endl;

    char b[256];
    socklen_t len = sizeof(sockaddr_in6);
    if (recvfrom(sendSockFd, b, sizeof(b), 0, (sockaddr*) &recvaddr, &len) < 0)
    {
        fprintf(stderr, "Recv failed, %s[%d]\n", strerror(errno), errno);
    }
    else
    {
        printf("Received: %s\n", b);
    }


    close(recvSockFd);
    close(sendSockFd);
}
