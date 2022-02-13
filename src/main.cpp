#include <iostream>
#include <boost/format.hpp>
#include <stdlib.h>

// #ifndef __CONSTANT_H__
// #include "constants.h"
// #endif

#include "detector.h"

using namespace std;
using namespace cv;

int main()
{
    string model_path = "./dnn_files/yolov5s.onnx";
    Config config = {0.25f, 0.45f, model_path, "./dnn_files/coco.names", cv::Size(640, 640), false};
    Detector detector(config);

    // Load a model.
    cv::dnn::Net net = cv::dnn::readNetFromONNX(model_path);

    // Create a window
    static const string kWinName = "Deep learning object detection in OpenCV";
    namedWindow(kWinName, WINDOW_NORMAL);

    // Open a video file or an image file or a camera stream.
    cv::VideoCapture cap;

    cap.open("./test_video/low_angle.mp4");

    // Process frames.
    cv::Mat frame;
    
    while (true)
    {
        cap >> frame;
        Detection detection = detector.detect(frame);
        Colors cl = Colors();

        detector.postProcess(frame, detection, cl);

        imshow(kWinName, frame);
        cv::waitKey(30);
    }
}
