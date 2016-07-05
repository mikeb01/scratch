#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "multicast.h"

const char* DAYTIME_PORT = "1300";

int
main(int argc, char* argv[])
{
    int sockfd, n;
    char timeStr[256];
    char b[256];
    struct sockaddr_in6 clientaddr;
    struct sockaddr_in6 addr;
//    struct sockaddr_in6 localAddr;
    socklen_t addrlen;
    time_t now;
    char clienthost[NI_MAXHOST];
    char clientservice[NI_MAXSERV];

    memset(&addr, 0, sizeof(addr));

    if (get_addr(mcastaddr, NULL, PF_INET6, SOCK_DGRAM, AI_PASSIVE, &addr) < 0)
    {
        fprintf(stderr, "get_addr error:: could not find multicast "
            "address=[%s] port=[%s]\n", mcastaddr, DAYTIME_PORT);
        return -1;
    }
    addr.sin6_port = htons(1300);

//    if (get_addr("fe80::60c:ceff:fee3:c8c0", DAYTIME_PORT, PF_INET6, SOCK_DGRAM, AI_PASSIVE, &localAddr) < 0)
//    {
//        fprintf(stderr, "get_addr error:: could not find multicast "
//            "address=[%s] port=[%s]\n", mcastaddr, DAYTIME_PORT);
//        return -1;
//    }

    if (isMulticast(&addr) < 0)
    {
        fprintf(stderr,
                "This address does not seem a multicast address [%s]\n",
                mcastaddr);
        return -1;
    }

    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);

    uint yes = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    if (bind(sockfd, (struct sockaddr*) &addr, sizeof(struct sockaddr_in6)) < 0)
    {
        perror("bind error:: ");
        close(sockfd);
        return -1;
    }

    if (joinGroup(sockfd, 0, 8, &addr) < 0)
    {
        close(sockfd);
        return -1;
    }

    addrlen = sizeof(clientaddr);
    for (;;)
    {
        n = recvfrom(sockfd,
                     b,
                     sizeof(b),
                     0,
                     (struct sockaddr*) &clientaddr,
                     &addrlen);

        if (n < 0)
            continue;

        memset(clienthost, 0, sizeof(clienthost));
        memset(clientservice, 0, sizeof(clientservice));

        getnameinfo((struct sockaddr*) &clientaddr, addrlen,
                    clienthost, sizeof(clienthost),
                    clientservice, sizeof(clientservice),
                    NI_NUMERICHOST);

        printf("Received request from host=[%s] port=[%s]\n",
               clienthost, clientservice);

        memset(timeStr, 0, sizeof(timeStr));
        time(&now);
        sprintf(timeStr, "%s", ctime(&now));

        n = sendto(sockfd, timeStr, sizeof(timeStr), 0,
                   (struct sockaddr*) &addr,
                   sizeof(addr));
        if (n < 1)
            perror("sendto error:: \n");

    }
    return 0;
}