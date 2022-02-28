#include <math.h>

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    // Create a window
    // static const string kWinName = "Nightime Vehicle Detection";
    // static const string trackingWin = "Tracking Window";
    // namedWindow(kWinName, cv::WINDOW_NORMAL);
    // namedWindow(trackingWin, cv::WINDOW_NORMAL);
    // Open a video file or an image file or a camera stream.
    cv::VideoCapture cap;

    cap.open("../test_video/test_video.mp4");

    auto frame_count = cap.get(CAP_PROP_FRAME_COUNT);

    auto fps = cap.get(CAP_PROP_FPS);

    auto duration = frame_count / fps;

    cout << "Duration : " << duration << endl;
    cout << "FPS : " << fps << endl;

    // Mat img, imgGrey, imgTemp;
    // Mat mask;
    //
    // while (true)
    // {
    //     // cap.set(CAP_PROP_POS_MSEC, 6*1000);
    //     cap >> img;
    //
    //     GaussianBlur(img, img, Size(3, 3), 0);
    //     cvtColor(img, img, COLOR_BGR2GRAY);
    //     Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    //     threshold(img, imgTemp, 200, 255, THRESH_BINARY);
    //     erode(imgTemp, imgTemp, kernel);
    //     dilate(imgTemp, mask, kernel);
    //
    //     getContours(mask, mask);
    //
    //
    //
    //     // imshow("Original", img);
    //     // imshow("Mask", mask);
    //     // imshow("Grey", imgGrey);
    //     // imshow("Corrected", imgTemp);
    //     imshow(kWinName, mask);
    //
    //     waitKey(30);
    //     // imwrite("test.png", img);
    //     // return 0;
    //
    // }
}
