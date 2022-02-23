#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

vector<Rect> getContours(Mat imgInput, Mat img)
{

    vector<vector<Point>> contours;

    findContours(imgInput, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<Rect> boundRect(contours.size());
    vector<vector<Point>> hull(contours.size());
    Mat drawing = Mat::zeros(imgInput.size(), CV_8UC3);
    for( size_t i = 0; i < contours.size(); i++ )
    {
        convexHull( contours[i], hull[i] );
    }

    for(int i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(255, 0, 0);
        drawContours(drawing, hull, i, color);
    }

    imshow("Drawing", drawing);

    int count = 0;
    for(vector<Point> contour : contours)
    {
        int area = contourArea(contour);
        /* cout << area << endl; */
        if(area < 1000)
        {
            /* cout << area << endl; */
            /* drawContours(img, conPoly, count, Scalar(0, 255, 0), 2); */

            boundRect[count] = boundingRect(contour);

            rectangle(img, boundRect[count].tl(), boundRect[count].br(), Scalar(255, 0, 0), 5);
            /* cout << boundRect[count].tl() << endl; */
            /* cout << boundRect[count].br() << endl; */
        }
        count++;
    }

    return boundRect;
}

void checkMatchRect(vector<Rect> inputRect)
{
    for


}

int main()
{
    // Create a window
    static const string kWinName = "Nightime Vehicle Detection";
    // static const string trackingWin = "Tracking Window";
    namedWindow(kWinName, cv::WINDOW_NORMAL);
    // namedWindow(trackingWin, cv::WINDOW_NORMAL);
    // Open a video file or an image file or a camera stream.
    cv::VideoCapture cap;

    cap.open("../test_video/low_light.mp4");
    //
    // auto frame_count = cap.get(CAP_PROP_FRAME_COUNT);
    //
    // auto fps = cap.get(CAP_PROP_FPS);
    //
    // auto duration = frame_count / fps;
    //
    // cout << "Duration : " << duration << endl;
    // cout << "FPS : " << fps << endl;

    Mat img, imgGrey, imgTemp;
    Mat mask;
    
    while (true)
    {
        // cap.set(CAP_PROP_POS_MSEC, 6*1000);
        cap >> img;

        GaussianBlur(img, img, Size(3, 3), 0);
        cvtColor(img, img, COLOR_BGR2GRAY);
        Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
        threshold(img, imgTemp, 200, 255, THRESH_BINARY);
        erode(imgTemp, imgTemp, kernel);
        dilate(imgTemp, mask, kernel);

        getContours(mask, mask);



        // imshow("Original", img);
        // imshow("Mask", mask);
        // imshow("Grey", imgGrey);
        // imshow("Corrected", imgTemp);
        imshow(kWinName, mask);

        waitKey(30);
        // imwrite("test.png", img);
        // return 0;

    } 
}

