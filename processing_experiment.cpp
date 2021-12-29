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
    int ratio = 3;
    int lowThreshold = 40;

    /* VideoCapture cap("test_video/hd_autobitrate_close_view.mp4"); */
    VideoCapture cap("test_video/360p_close_view_gphoto.mp4");
    Ptr<BackgroundSubtractor> pBacksub;
    Mat img;

    pBacksub = createBackgroundSubtractorKNN();

    Mat imgGrey, imgBlur, imgCanny, imgDilate, imgBlurBlur, mask, imgErode;

    Mat imgWarp;
    Mat imgTemp;

    while(true)
    {
        cap.read(img);

        if(img.empty())
        {
            cap.set(CAP_PROP_POS_AVI_RATIO, 0);
            cap.read(img);
        }
        cvtColor(img, imgGrey, COLOR_BGR2GRAY);
        GaussianBlur(imgGrey, imgBlur, Size(5, 5), 0);
        pBacksub -> apply(imgBlur, mask);


        Mat ekernel = getStructuringElement(MORPH_RECT, Size(5, 5));
        Mat kernel = getStructuringElement(MORPH_RECT, Size(10, 10));
        /* erode(mask, imgErode, ekernel); */
        /* dilate(imgErode, imgDilate, kernel); */
        /* dilate(imgDilate, imgDilate, kernel); */
        /* dilate(imgDilate, imgDilate, kernel); */
        morphologyEx(mask, imgTemp, MORPH_OPEN, kernel);
        morphologyEx(imgTemp, imgErode, MORPH_CLOSE, kernel);
        morphologyEx(imgErode, imgDilate, MORPH_OPEN, ekernel);

        getContours(imgDilate, img);

        /* imshow("Test Video", img); */
        /* imshow("Test Video Blur", imgBlur); */
        /* /1* imshow("Test Video Cammy", imgCanny); *1/ */
        /* imshow("Test video", imgDilate); */
        imshow("Image", img);
        /* imshow("Mask", mask); */
        imshow("Mask Erode", imgErode);
        imshow("Mask Dilate", imgDilate);
        waitKey(30);
    }
}
