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
#include <chrono>

#define SOCKET_ERROR (-1)

void InitSocket(int& sockfd, sockaddr_in& serveraddr) {
    uint64 PORT = 8080;
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

//    inetAddr = "10.166.0.12";
    inetAddr = "192.168.0.61";
    // Filling server information
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr(inetAddr);
}

int main() {
    uint64 MAXLINE = 1024 * 50;
    char buffer[MAXLINE];
    std::vector<char> imgVec(640*480*3, 0);
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
    cv::Mat img;
    cv::namedWindow("PyCameraCXX", cv::WINDOW_AUTOSIZE);
    cv::resizeWindow("PyCameraCXX", cv::Size(1900,1010));

    while (true) {

        n = recvfrom(sockfd, buffer, 1024,
                     0, (struct sockaddr *) &servaddr,
                     reinterpret_cast<socklen_t *>(&len));
        if (buffer[0] == 'B' and buffer[1] == 'E') {
            auto start = std::chrono::system_clock::now();
//            std::cout << "Started Img\n";
            for (int i = 0; i < 18; ++i) {
                n = recvfrom(sockfd, buffer, MAXLINE,
                             0, (struct sockaddr *) &servaddr,
                             reinterpret_cast<socklen_t *>(&len));
                memcpy(imgVec.data() + (i * MAXLINE), buffer, MAXLINE);
            }
//            std::cout << "Got Image\n";
            img = cv::Mat(480, 640 ,CV_8UC3, imgVec.data());
            cv::resize(img, img, cv::Size(1900,1010), 0, 0,cv::INTER_LINEAR);
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = end - start;
            float fps = 1.0 / diff.count();
            cv::putText(img, std::to_string(fps), cv::Point(10,10), cv::FONT_HERSHEY_PLAIN,
                        1, cv::Scalar(0,0,255), 1, false);
            cv::imshow("PyCameraCXX", img);
//            std::cout << diff.count() << "\n";


            cv::waitKey(1);
        }
    }
    img = cv::Mat(480,640 ,CV_8UC3, imgVec.data());

    cv::imshow("PyCameraCXX", img);
    cv::imwrite("/home/dinozood/Projects/Pets/PyCameraC/remoteImage.jpg", img);
    cv::waitKey(0);
    cv::imwrite("/home/dinozood/Projects/PyCameraCXX/remoteImage.jpg", img);

    close(sockfd);
    return 0;

}

