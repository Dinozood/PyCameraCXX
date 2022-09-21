//
// Created by dinozood on 21.09.22.
//

// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
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

// Driver code
void InitSocket(int& sockfd, sockaddr_in& servaddr, sockaddr_in& cliaddr) {
    uint64 PORT = 9417;

    if (SOCKET_ERROR == (sockfd = socket(AF_INET, SOCK_DGRAM, 0))) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    const int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

int main() {
    uint64 MAXLINE = 700*700;
    int sockfd, newsockfd;
    char buffer[MAXLINE];
    char hello[] = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;

    InitSocket(sockfd, servaddr, cliaddr);

    int len, n;

    len = sizeof(cliaddr); //len is value/result
    listen(sockfd,5);
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cliaddr,
                       reinterpret_cast<socklen_t *>(&len));
    if (newsockfd < 0) {
        std::cerr << "ERROR on accept" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    bzero(buffer,MAXLINE);
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                 reinterpret_cast<socklen_t *>(&len));
    if (n < 0) {
        std::cerr << "ERROR reading from socket" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    sendto(sockfd, (const char *)hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
           len);
    printf("Hello message sent.\n");

    close(newsockfd);
    close(sockfd);
    return 0;

    return 0;
}
