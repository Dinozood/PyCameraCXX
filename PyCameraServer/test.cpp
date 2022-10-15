//
// Created by dinozood on 21.09.22.
//
#include <iostream>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <cstring>
#include <unistd.h>
using namespace cv;

int main() {

    chroot("~/PyCameraCXX");
    Mat img;
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cerr << "cannot open camera";
    }

    cap >> img;

    imwrite("/home/pi/PyCameraCXX/PyCXXImage.jpg", img);

    return 0;
}