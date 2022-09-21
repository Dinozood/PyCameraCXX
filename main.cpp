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

int main() {
    uint64 MAXLINE = 700*700;
    int sockfd;
    uint64 PORT = 9417;
    char buffer[MAXLINE];
    char hello[] = "Hello from client";
    struct sockaddr_in servaddr{};
    struct addrinfo addrInfo{}, *res;
    const char * inetAddr;

    // Creating socket file descriptor
    if (SOCKET_ERROR == (sockfd = socket(AF_INET, SOCK_DGRAM, 0))) {
        std::cerr << "socket creation failed" << std::endl;
        return EXIT_FAILURE;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&addrInfo, 0, sizeof(addrInfo));

    if (getaddrinfo("dinozood.ddns.net", NULL, &addrInfo, &res) != 0)
        return EXIT_FAILURE;

    inetAddr = inet_ntoa(((sockaddr_in *) res -> ai_addr) -> sin_addr);

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(inetAddr);

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

