#include <iostream>
#include <boost/format.hpp>
#include <stdlib.h>

#include "dnn_processing.h"

int main()
{
   
    // Create a window
    static const std::string kWinName = "Deep learning object detection in OpenCV";
    /* static const std::string detectionWinName = "Detection Window"; */
    namedWindow(kWinName, cv::WINDOW_NORMAL);
    /* namedWindow(detectionWinName, cv::WINDOW_NORMAL); */

    // Open a video file or an image file or a camera stream.
    cv::VideoCapture cap;

    cap.open("./test_video/low_angle.mp4");

    // Process frames.
    cv::Mat frame, blob;
    int count = 0;

    // Output of Neural Network
    std::vector<cv::Rect> rectangles;
    while (true)
    {
        cap >> frame;
        
        if (frame.empty())
        {
            cv::waitKey();
            break;
        }
        rectangles = dnnProcess(frame);
        for(cv::Rect rectangle : rectangles)
        {
            cv::rectangle(frame, rectangle, cv::Scalar(0, 0, 255), 2);
        }

        imshow(kWinName, frame);
        cv::waitKey(30);
    }
}
