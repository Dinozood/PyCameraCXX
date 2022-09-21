//
// Created by dinozood on 21.09.22.
//

#include <iostream>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>

#define SOCKET_ERROR (-1)

void InitSocket(int& sockfd, sockaddr_in& serveraddr) {
    uint64 PORT = 9417;
    uint64 MAXLINE = 700*700;
    struct addrinfo addrInfo{}, *res;
    const char * inetAddr;

    // Creating socket file descriptor
    if (SOCKET_ERROR == (sockfd = socket(AF_INET, SOCK_DGRAM, 0))) {
        std::cerr << "socket creation failed" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    memset(&serveraddr, 0, sizeof(serveraddr));
    memset(&addrInfo, 0, sizeof(addrInfo));

    if (getaddrinfo("dinozood.ddns.net", NULL, &addrInfo, &res) != 0) {
        std::cerr << "Get ip address error" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    inetAddr = inet_ntoa(((sockaddr_in *) res -> ai_addr) -> sin_addr);

    // Filling server information
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr(inetAddr);
}

int main() {
    uint64 MAXLINE = 700*700;
    char buffer[MAXLINE];
    char hello[] = "Hello from client";

    int sockfd;
    struct sockaddr_in servaddr{};
    InitSocket(sockfd, servaddr);
    int len;
    size_t n;

    sendto(sockfd, (const char *)hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,
           sizeof(servaddr));
    printf("Hello message sent.\n");

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *) &servaddr,
                 reinterpret_cast<socklen_t *>(&len));
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}

