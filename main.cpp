#include <iostream>
#include <boost/format.hpp>
#include <stdlib.h>

#include "dnn_processing.h"

float confThreshold = 0.5;
float nmsThreshold = 0.4;
std::vector<std::string> classes;

int main()
{
    int initialConf = (int)(confThreshold * 100);
    float scale = 0.003925;
    int inpWidth = 416;
    int inpHeight = 416;
    std::string modelPath = "./dnn_files/yolov4-mish-416.weights";
    std::string configPath = "./dnn_files/yolov4-mish-416.cfg";

    // Unpack Class Names
    std::string classPath = "./dnn_files/coco_classes.txt";
    std::ifstream ifs(classPath.c_str());
    std::string line;

    while(std::getline(ifs, line))
    {
        classes.push_back(line);
    }
     
    // Load a model.
    cv::dnn::Net net = cv::dnn::readNet(modelPath, configPath);

    // CUDA for Processing
    int backend = 5;
    int target = 6;
    net.setPreferableBackend(backend);
    net.setPreferableTarget(target);
    std::vector<cv::String> outNames = net.getUnconnectedOutLayersNames();


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
        preprocess(frame, net, cv::Size(inpWidth, inpHeight), scale);

        std::vector<cv::Mat> outs;
        net.forward(outs, outNames);

        std::vector<cv::Rect> rectangles = postprocess(frame, outs, net, backend);

        for(cv::Rect rectangle : rectangles)
        {
            cv::rectangle(frame, rectangle, cv::Scalar(0, 0, 255), 2);
        }

        // Put efficiency information.
        std::vector<double> layersTimes;
        double freq = cv::getTickFrequency() / 1000;
        double t = net.getPerfProfile(layersTimes) / freq;
        std::string label = cv::format("Inference time: %.2f ms", t);
        cv::putText(frame, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
        imshow(kWinName, frame);
        cv::waitKey(30);
    }
}
