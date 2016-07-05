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

const char* mcastaddr = "224.0.1.1";
const char* inf = "en0";
const short PORT = 9877;

#define E(expr) CHECK((rc = (expr)) >= 0, #expr)
#define RES(err, expr) ((rc = expr) == (err) || (CHECK(!rc, #expr), 0))
#define CHECK(test, msg) ((test) ? (void)0 : ((void)fprintf(stderr, \
	"%s:%d: %s: %s\n", __FILE__, __LINE__, msg, strerror(rc)), abort()))

ssize_t multicastRecv(int sockfd, sockaddr_in* sendaddr, sockaddr_in* recvaddr)
{
    socklen_t len = sizeof(sockaddr_in);
    char b[256];
    const char* retmsg = "def";
    ssize_t rc  = recvfrom(sockfd, b, sizeof(b), 0, (sockaddr*) recvaddr, &len);

    if (rc < 0)
    {
        fprintf(stderr, "Recv failed, %s[%d]\n", strerror(errno), errno);
    }
    else
    {
        printf("Received: %s\n", b);
    }

    if (sendto(sockfd, retmsg, strlen(retmsg), 0, (const sockaddr*) sendaddr, len) < 0)
    {
        fprintf(stderr, "Send failed, %s[%d]\n", strerror(errno), errno);
    }

    return rc;
}

void initAddress(sockaddr_in* sendaddr, short port)
{
    memset(sendaddr, 0, sizeof(*sendaddr));
    sendaddr->sin_family = AF_INET;
    sendaddr->sin_addr.s_addr = INADDR_ANY;
    sendaddr->sin_port = htons(port);
}

int main(int argc, char** argv)
{
    int rc = 0;
    addrinfo hints;
    addrinfo* result;
    sockaddr_in bindaddr;
    sockaddr_in sendaddr;
    sockaddr_in recvaddr;
    const uint32_t yes = 1;
    const uint32_t no = 0;
    const uint32_t mcastTTL = 8;
        ip_mreq mreq;
    char name[INET_ADDRSTRLEN];
    const char* ifn;

    if (argc > 1)
    {
        ifn = argv[1];
    }
    else
    {
        ifn = "en0";
    }

    ifaddrs *ifaddrs, *cursor;

    E(getifaddrs(&ifaddrs));

    cursor = ifaddrs;
    bool found = false;
    while (cursor)
    {
        if (strcmp(ifn, cursor->ifa_name) == 0 && cursor->ifa_addr->sa_family == AF_INET)
        {
            sockaddr_in* ifa_addr = (sockaddr_in*) cursor->ifa_addr;
            recvaddr.sin_family = AF_INET;
            recvaddr.sin_addr = ifa_addr->sin_addr;
            recvaddr.sin_port = htons(PORT);
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

    inet_ntop(AF_INET, &recvaddr.sin_addr, name, INET6_ADDRSTRLEN);
    printf("revc addr: %s, port: %d\n", name, ntohs(recvaddr.sin_port));

    E(socket(PF_INET, SOCK_DGRAM, IPPROTO_IP));
    int recvSockFd = rc;

    E(setsockopt(recvSockFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)));

    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    E(setsockopt(recvSockFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)));

    initAddress(&bindaddr, PORT);

    E(bind(recvSockFd, (const sockaddr*) &bindaddr, sizeof(sockaddr_in)));

    mreq.imr_multiaddr.s_addr = inet_addr(mcastaddr);
    mreq.imr_interface = recvaddr.sin_addr;

    E(setsockopt(recvSockFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)));

    sendaddr.sin_addr.s_addr = inet_addr(mcastaddr);
    sendaddr.sin_port = htons(PORT);

    auto future = std::async(std::launch::async, multicastRecv, recvSockFd, &sendaddr, &recvaddr);

    const char* msg = "abc";

    if (sendto(recvSockFd, msg, strlen(msg), 0, (const sockaddr*) &sendaddr, sizeof(sockaddr_in)) < 0)
    {
        fprintf(stderr, "Send failed, %s[%d]\n", strerror(errno), errno);
    }

    int c = future.get();

    std::cout << c << std::endl;

    char b[256];
    socklen_t len = sizeof(sockaddr_in);
    if (recvfrom(recvSockFd, b, sizeof(b), 0, (sockaddr*) &recvaddr, &len) < 0)
    {
        fprintf(stderr, "Recv failed, %s[%d]\n", strerror(errno), errno);
    }
    else
    {
        printf("Received: %s\n", b);
    }

    close(recvSockFd);
}
