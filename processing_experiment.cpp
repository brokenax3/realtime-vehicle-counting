#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat makeSkewImg(Mat imgInput)
{
    float w = 200, h = 400;
    Mat imgWarp;

    Point2f src[4] = {
        {144,0},
        {220,0},
        {9,200},
        {362,200}
    };

    Point2f dst[4] = {
        {0.0f, 0.0f},
        {w, 0.0f},
        {0.0f, h},
        {w, h}
    };
    
    Mat matrix = getPerspectiveTransform(src, dst);
    warpPerspective(imgInput, imgWarp, matrix, Point(w, h));
    /* imshow("Warp", imgWarp); */
    return imgWarp;
}

void getContours(Mat imgInput, Mat img)
{

    vector<vector<Point>> contours;

    findContours(imgInput, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    /* vector<Rect> boundRect(contours.size()); */
    vector<vector<Point>> hull(contours.size());
    Mat drawing = Mat::zeros(imgInput.size(), CV_8UC3);
    for( size_t i = 0; i < contours.size(); i++ )
    {
        convexHull( contours[i], hull[i] );
    }

    for(int i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(255, 0, 0);
        /* fillPoly(drawing, pts=[contours], color); */
        /* drawContours(drawing, contours, i, color); */
        drawContours(drawing, hull, i, color);
    }

    imshow("Drawing", drawing);


    /* for(vector<Point> contour : contours) */
    /* { */
    /*     int area = contourArea(contour); */
    /*     /1* cout << area << endl; *1/ */
    /*     if((1500 < area) && (area < 5000)) */
    /*     { */
    /*         cout << area << endl; */
    /*         /1* drawContours(img, conPoly, count, Scalar(0, 255, 0), 2); *1/ */

    /*         boundRect[count] = boundingRect(contour); */
    /*         if(boundRect[count].area() >= 2000) */

    /*         rectangle(img, boundRect[count].tl(), boundRect[count].br(), Scalar(255, 0, 0), 5); */
    /*         cout << boundRect[count].tl() << endl; */
    /*         cout << boundRect[count].br() << endl; */
    /*     } */
    /*     count++; */
    /* } */
}

int main()
{
    /* VideoCapture cap("test_video/hd_autobitrate_close_view.mp4"); */
    VideoCapture cap("test_video/low_angle.mp4");
    Mat img, imgNext, imgDiff;

    Mat imgGrey, imgBlur, imgCanny, imgDilate, imgBlurBlur, mask, imgErode;

    Mat imgWarp;
    Mat imgTemp;

    int firstFrame = 1;
    while(true)
    {
        img = imgNext;
        cap.read(imgNext);
        cvtColor(imgNext, imgNext, COLOR_BGR2GRAY);
        resize(imgNext, imgNext, Size(), 0.4, 0.4, INTER_LINEAR);
        if(firstFrame == 0)
        {
            absdiff(imgNext, img, imgDiff);
            GaussianBlur(imgDiff, imgDiff, Size(5, 5), 0);
            threshold(imgDiff, mask, 25, 255, THRESH_BINARY);
            Mat kernel = getStructuringElement(MORPH_RECT, Size(10, 10));
            Mat ekernel = getStructuringElement(MORPH_RECT, Size(3, 3));
            erode(mask, imgErode, ekernel);
            dilate(mask, imgDilate, kernel);
            getContours(imgDilate, mask);
            imshow("Difference", imgDiff);
            imshow("Threshold", mask);
            imshow("Dilated", imgDilate);
            imshow("Eroded", imgErode);
            waitKey(30);
        }
        else
        {
            firstFrame = 0;
        }
    }
        

    /* while(true) */
    /* { */
    /*     cap.read(img); */

    /*     if(img.empty()) */
    /*     { */
    /*         cap.set(CAP_PROP_POS_AVI_RATIO, 0); */
    /*         cap.read(img); */
    /*     } */
    /*     resize(img, img, Size(), 0.2, 0.2, INTER_LINEAR); */
    /*     cvtColor(img, imgGrey, COLOR_BGR2GRAY); */
    /*     GaussianBlur(imgGrey, imgBlur, Size(5, 5), 0); */
    /*     pBacksub -> apply(imgBlur, mask); */


    /*     Mat ekernel = getStructuringElement(MORPH_RECT, Size(5, 5)); */
    /*     /1* erode(mask, imgErode, ekernel); *1/ */
    /*     /1* dilate(imgErode, imgDilate, kernel); *1/ */
    /*     /1* dilate(imgDilate, imgDilate, kernel); *1/ */
    /*     /1* dilate(imgDilate, imgDilate, kernel); *1/ */
    /*     morphologyEx(mask, imgTemp, MORPH_OPEN, ekernel); */
    /*     morphologyEx(imgTemp, imgErode, MORPH_CLOSE, kernel); */
    /*     morphologyEx(imgErode, imgDilate, MORPH_OPEN, kernel); */

    /*     getContours(imgDilate, img); */

    /*     /1* imshow("Test Video", img); *1/ */
    /*     /1* imshow("Test Video Blur", imgBlur); *1/ */
    /*     /1* /2* imshow("Test Video Cammy", imgCanny); *2/ *1/ */
    /*     /1* imshow("Test video", imgDilate); *1/ */
    /*     imshow("Image", img); */
    /*     /1* imshow("Mask", mask); *1/ */
    /*     imshow("Mask Erode", imgErode); */
    /*     imshow("Mask Dilate", imgDilate); */
    /*     waitKey(30); */
    /* } */
}
