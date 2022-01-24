#include <iostream>
#include <boost/format.hpp>
#include <stdlib.h>
#include <opencv2/tracking.hpp>
/* #include <opencv2/tracking/tracking_legacy.hpp> */

#include "dnn_processing.h"
#include "tracking.h"

float confThreshold = 0.3;
float nmsThreshold = 0.4;
std::vector<std::string> classes;

int main()
{
    // Initial Variables
    int initialConf = (int)(confThreshold * 100);
    float scale = 0.003925;
    int inpWidth = 416;
    int inpHeight = 416;
    std::string modelPath = "./dnn_files/yolov4-tiny.weights";
    std::string configPath = "./dnn_files/yolov4-tiny.cfg";

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
    int backend = 0;
    int target = 0;
    net.setPreferableBackend(backend);
    net.setPreferableTarget(target);
    std::vector<cv::String> outNames = net.getUnconnectedOutLayersNames();

    // Create a window
    static const std::string kWinName = "Deep learning object detection in OpenCV";
    static const std::string test = "test";
    /* static const std::string detectionWinName = "Detection Window"; */
    namedWindow(kWinName, cv::WINDOW_NORMAL);
    /* namedWindow(detectionWinName, cv::WINDOW_NORMAL); */

    // Open a video file or an image file or a camera stream.
    cv::VideoCapture cap;

    cap.open("./test_video/low_angle.mp4");

    // Process frames.
    cv::Mat frame;
    cv::Mat drawing;
    
    CentroidTracker centroidTracker; 
    // Output of Neural Network
    std::vector<cv::Rect> rectangles;
    int count = 0;
    while (true)
    {
        cap >> frame;
        drawing = cv::Mat(frame.cols, frame.rows, CV_8UC3, cv::Scalar());
        /* cv::GaussianBlur(frame, frame, cv::Size(3, 3), 0); */
        /* roi = frame(cv::Range(150, 300), cv::Range(600, 1016)); */
        
        /* if (frame.empty()) */
        /* { */
        /*     cv::waitKey(); */
        /*     break; */
        /* } */
        /* std::cout << count << std::endl; */
        // Detection
        preprocess(frame, net, cv::Size(inpWidth, inpHeight), scale);

        std::vector<cv::Mat> outs;
        net.forward(outs, outNames);

        rectangles = postprocess(frame, outs, net, backend);

        centroidTracker.updateObjects(rectangles);
        
        for(cv::Rect rectangle : rectangles)
        {
            cv::rectangle(drawing, rectangle, cv::Scalar(255, 0, 0), 2);
        }

        for(Object object : centroidTracker.objects)
        {
            putText(drawing, std::to_string(object.id), cv::Point(object.centroid.x, object.centroid.y), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 255, 0), 2);
        }

        // Put efficiency information.
        std::vector<double> layersTimes;
        double freq = cv::getTickFrequency() / 1000;
        double t = net.getPerfProfile(layersTimes) / freq;
        std::string label = cv::format("Inference time: %.2f ms", t);
        cv::putText(frame, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));

        imshow(kWinName, frame);
        imshow(test, drawing);
        cv::waitKey(30);
    }
}
