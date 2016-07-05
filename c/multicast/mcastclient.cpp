#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>

#include "multicast.h"

const char* DAYTIME_PORT = "1300";

int
main(int argc, char* argv[])
{
    int rc = 0;
    int sockfd;
    ssize_t n;
    char timeStr[256];
    char letter;
    struct sockaddr_in6 addr, clientaddr;
    socklen_t addrlen;
    socklen_t clientaddrlen;

    addrlen = sizeof(addr);
    memset(&addr, 0, addrlen);

    get_addr(mcastaddr, NULL, PF_INET6, SOCK_DGRAM, 0, &addr);
    addr.sin6_port = htons(1300);
//    get_addr(NULL, DAYTIME_PORT, PF_INET6, SOCK_DGRAM, 0, &localAddr);

    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);

    const uint no = 0;
    const uint yes = 1;
    const uint mcastTTL = 8;

    E(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)));

//    if (bind(sockfd, (struct sockaddr*) &addr, addrlen) < 0)
//    {
//        perror("bind error:: \n");
//        close(sockfd);
//        return -1;
//    }

    struct ipv6_mreq mreq6;
    memcpy(&mreq6.ipv6mr_multiaddr, &(addr.sin6_addr), sizeof(struct in6_addr));
    mreq6.ipv6mr_interface = if_nametoindex("en0"); // cualquier interfaz

//    E(setsockopt(sockfd, IPPROTO_IPV6 , IPV6_MULTICAST_IF, &mreq6.ipv6mr_interface, sizeof(mreq6.ipv6mr_interface)));
//    if (joinGroup(sockfd, 0, 8, &addr) < 0)
//    {
//        close(sockfd);
//        return -1;
//    }

    letter = '1';
    n = sendto(sockfd, &letter, sizeof(letter), 0, (struct sockaddr*) &addr, addrlen);
    if (n < 0)
    {
        printf("%d\n", errno);
        perror("sendto error:: ");
        close(sockfd);
        return -1;
    }

    memset(timeStr, 0, sizeof(timeStr));
    clientaddrlen = sizeof(clientaddr);

    n = recvfrom(sockfd,
                 timeStr,
                 sizeof(timeStr),
                 0,
                 (struct sockaddr*) &clientaddr,
                 &clientaddrlen);


    if (n < 0)
    {
        perror("recvfrom error:: ");
        close(sockfd);
        return -1;
    }

    printf("%s\n", timeStr);

    close(sockfd);
    return 0;
}