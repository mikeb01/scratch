//
// Created by Michael Barker on 09/10/15.
//

#include <future>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <iostream>
#include <ifaddrs.h>
#include <cstring>
#include <unistd.h>

const char* mcastaddr = "ff02::3";
const short PORT = 9877;

#define E(expr) CHECK((rc = (expr)) >= 0, #expr)
#define RES(err, expr) ((rc = expr) == (err) || (CHECK(!rc, #expr), 0))
#define CHECK(test, msg) ((test) ? (void)0 : ((void)fprintf(stderr, \
	"%s:%d: %s: %s\n", __FILE__, __LINE__, msg, std::strerror(rc)), abort()))

ssize_t multicastRecv(int sockfd, sockaddr_in6* sendaddr, sockaddr_in6* recvaddr)
{
    socklen_t len = sizeof(sockaddr_in6);
    char b[256];
    const char* retmsg = "def";
    ssize_t rc  = recvfrom(sockfd, b, sizeof(b), 0, (sockaddr*) recvaddr, &len);

    if (rc < 0)
    {
        fprintf(stderr, "Recv failed, %s[%d]\n", std::strerror(errno), errno);
    }
    else
    {
        printf("Received: %s\n", b);
    }

    if (sendto(sockfd, retmsg, strlen(retmsg), 0, (const sockaddr*) sendaddr, sizeof(sockaddr_in6)) < 0)
    {
        fprintf(stderr, "Send failed, %s[%d]\n", std::strerror(errno), errno);
    }

    return rc;
}

void initAddress(sockaddr_in6* sendaddr, short port)
{
    std::memset(sendaddr, 0, sizeof(*sendaddr));
    sendaddr->sin6_family = AF_INET6;
    sendaddr->sin6_addr = in6addr_any;
    sendaddr->sin6_port = htons(port);
}

void printAddress(const char* type, sockaddr_in6* addr)
{
    char name[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &addr->sin6_addr, name, INET6_ADDRSTRLEN);
    printf("%s addr: %s, port: %d, scope: %d\n", type, name, ntohs(addr->sin6_port), addr->sin6_scope_id);
}

int main(int argc, char** argv)
{
    int rc = 0;
    addrinfo hints;
    addrinfo* result;
    sockaddr_in6 bindaddr;
    sockaddr_in6 sendaddr;
    sockaddr_in6 recvaddr;
    const uint32_t yes = 1;
    const uint32_t no = 0;
    const uint32_t mcastTTL = 8;
    ipv6_mreq mreq6;
    char name[INET6_ADDRSTRLEN];
    const char* ifn;

    if (argc > 1)
    {
        ifn = argv[1];
    }
    else
    {
        ifn = "lo0";
    }

    unsigned int scope_id = if_nametoindex(ifn);

    std::memset(&hints, 0, sizeof(addrinfo));

    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = 0;

    E(getaddrinfo(mcastaddr, NULL, &hints, &result));

    if (!result)
    {
        fprintf(stderr, "No results");
        return -1;
    }

    memcpy(&sendaddr, result->ai_addr, sizeof(sockaddr_in6));
    sendaddr.sin6_port = htons(PORT);
    sendaddr.sin6_scope_id = scope_id;

    freeaddrinfo(result);

    printAddress("send", &sendaddr);

    ifaddrs *ifaddrs, *cursor;

    E(getifaddrs(&ifaddrs));

    cursor = ifaddrs;
    bool found = false;
    while (cursor)
    {
        if (strcmp(ifn, cursor->ifa_name) == 0 && cursor->ifa_addr->sa_family == AF_INET6)
        {
            memcpy(&recvaddr, cursor->ifa_addr, sizeof(recvaddr));
            recvaddr.sin6_port = htons(PORT);
            found = true;
            break;
        }

        cursor = cursor->ifa_next;
    }

    freeifaddrs(ifaddrs);

    if (!found)
    {
        fprintf(stderr, "Local interface not found\n");
        abort();
    }

    printAddress("recv", &recvaddr);

    E(socket(PF_INET6, SOCK_DGRAM, IPPROTO_IP));
    int socketFd = rc;

    memcpy(&mreq6.ipv6mr_multiaddr, &(sendaddr.sin6_addr), sizeof(struct in6_addr));
    mreq6.ipv6mr_interface = scope_id;

    E(setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)));

    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    E(setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)));

    initAddress(&bindaddr, PORT);

    printAddress("recv", &recvaddr);
    printAddress("bind", &bindaddr);

    E(bind(socketFd, (const sockaddr*) &bindaddr, sizeof(sockaddr_in6)));

    E(setsockopt(socketFd, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq6, sizeof(mreq6)));

    auto future = std::async(std::launch::async, multicastRecv, socketFd, &sendaddr, &recvaddr);

    const char* msg = "abc";

    if (sendto(socketFd, msg, strlen(msg), 0, (const sockaddr*) &sendaddr, sizeof(sockaddr_in6)) < 0)
    {
        fprintf(stderr, "Send failed, %s[%d]\n", std::strerror(errno), errno);
    }

    E(future.get());

    char b[256];
    socklen_t len = sizeof(sockaddr_in6);
    if (recvfrom(socketFd, b, sizeof(b), 0, (sockaddr*) &recvaddr, &len) < 0)
    {
        fprintf(stderr, "Recv failed, %s[%d]\n", std::strerror(errno), errno);
    }
    else
    {
        printf("Received: %s\n", b);
    }

    close(socketFd);
}
