#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void getContours(Mat imgInput, Mat img)
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
        if((1500 < area) && (area < 5000))
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
}

int main()
{
    /* VideoCapture cap("test_video/hd_autobitrate_close_view.mp4"); */
    VideoCapture cap("test_video/low_angle.mp4");
    Mat img, imgNext, imgDiff, imgOriginal;

    Mat imgGrey, imgBlur, imgCanny, imgDilate, imgBlurBlur, mask, imgErode;

    Mat imgWarp;
    Mat imgTemp;

    int firstFrame = 1;
    while(true)
    {
        img = imgNext;
        cap.read(imgOriginal);
        resize(imgOriginal, imgOriginal, Size(), 0.5, 0.5, INTER_LINEAR);
        imgNext = imgOriginal;
        cvtColor(imgNext, imgNext, COLOR_BGR2GRAY);
        if(firstFrame == 0)
        {
            absdiff(imgNext, img, imgDiff);
            /* GaussianBlur(imgDiff, imgDiff, Size(5, 5), 0); */
            threshold(imgDiff, mask, 30, 255, THRESH_BINARY);
            Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
            Mat ekernel = getStructuringElement(MORPH_RECT, Size(2, 2));
            erode(mask, imgErode, ekernel);
            dilate(imgErode, imgDilate, kernel);
            /* morphologyEx(imgErode, imgErode, MORPH_CLOSE, kernel); */
            /* morphologyEx(imgErode, imgErode, MORPH_CLOSE, kernel); */
            /* morphologyEx(imgErode, imgErode, MORPH_OPEN, ekernel); */
            /* erode(mask, imgErode, ekernel); */
            getContours(imgDilate, imgOriginal);
            imshow("Difference", imgDiff);
            imshow("Threshold", mask);
            imshow("Dilated", imgDilate);
            imshow("Eroded", imgErode);
            imshow("Detection Image", imgOriginal);
            waitKey(30);
        }
        else
        {
            firstFrame = 0;
        }
<<<<<<< HEAD
        resize(img, img, Size(), 0.4, 0.4, INTER_LINEAR);
        cvtColor(img, imgGrey, COLOR_BGR2GRAY);
        GaussianBlur(imgGrey, imgBlur, Size(3, 3), 0);
        pBacksub -> apply(imgBlur, mask);

        /* threshold(mask, mask, 30, 255, THRESH_BINARY); */


        Mat fkernel = getStructuringElement(MORPH_RECT, Size(2, 2));
        Mat ekernel = getStructuringElement(MORPH_RECT, Size(5, 5));
        Mat kernel = getStructuringElement(MORPH_RECT, Size(8, 8));
        erode(mask, mask, fkernel);
        threshold(mask, mask, 30, 255, THRESH_BINARY);
        /* dilate(imgErode, imgDilate, fkernel); */
        /* dilate(imgDilate, imgDilate, kernel); */
        /* dilate(imgDilate, imgDilate, kernel); */
        /* morphologyEx(mask, imgTemp, MORPH_OPEN, ekernel); */
        morphologyEx(mask, imgErode, MORPH_CLOSE, kernel);
        morphologyEx(imgErode, imgDilate, MORPH_OPEN, kernel);

        getContours(imgDilate, img);

        /* imshow("Test Video", img); */
        /* imshow("Test Video Blur", imgBlur); */
        /* /1* imshow("Test Video Cammy", imgCanny); *1/ */
        /* imshow("Test video", imgDilate); */
        imshow("Image", img);
        imshow("Mask", mask);
        /* imshow("Mask Erode", imgErode); */
        imshow("Mask Dilate", imgDilate);
        waitKey(30);
=======
>>>>>>> test
    }
}
