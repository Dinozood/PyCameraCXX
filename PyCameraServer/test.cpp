//
// Created by dinozood on 21.09.22.
//
#include <iostream>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <cstring>

using namespace cv;

int main() {

    Mat img;
    VideoCapture cap(0);
    cap.read(img);
    imwrite("/home/pi/image2.jpg", img);

    return 0;
}