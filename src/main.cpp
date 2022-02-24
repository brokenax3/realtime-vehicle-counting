#include <iostream>
#include <boost/format.hpp>
#include <stdlib.h>
#include <random>

#include "utils.h"
#include "detector.h"
#include "tracker.h"
#include "counter.h"

using namespace std;

int main()
{
    string model_path = "./dnn_files/yolov5n6.onnx";
    Config config = {kMinConfidence, nmsThreshold, model_path, "./dnn_files/coco.names", cv::Size(640, 640), false};
    Detector detector(config);

    // Force night mode
    detector.setNight(false);

    // Load a model.
    cv::dnn::Net net = cv::dnn::readNetFromONNX(model_path);

    // Create a window
    static const string kWinName = "Deep learning object detection in OpenCV";
    static const string trackingWin = "Tracking Window";
    // namedWindow(kWinName, cv::WINDOW_NORMAL);
    namedWindow(trackingWin, cv::WINDOW_NORMAL);

    // Open a video file or an image file or a camera stream.
    cv::VideoCapture cap;
    // cv::Mat frame = cv::imread("testimg3.png");

    cap.open("./test_video/test_video.mp4");
    // cap.open("./test_video/low_light.mp4");

    // Process frames.
    vector<cv::Rect> boxes;

    // Create SORT Tracker
    Tracker tracker;
    auto frame_index = 0;
    auto frame_count = 0;
    Colors cl = Colors();

    // Create Counter
    Counter counter;

    // Total Time Required
    cv::TickMeter totalTime;
    cv::Mat frame, tmpFrame;


    // Detection
    Detection detection;
    totalTime.start();
    while(true)
    {
        cap >> frame;
        // frame = tmpFrame;
        if(frame.empty() == true)
        {
            totalTime.stop();
            std::cout << "Actual count : 239" << std::endl;
            std::cout << "Program count : " << to_string(counter.count) << std::endl;
            std::cout << "Accuracy : " << to_string((1 - (float) abs(237 - counter.count) / 237) * 100) << "%" << std::endl;
            float t = totalTime.getTimeSec();
            std::cout << "Total Time Taken : " << to_string(t) + "s" << std::endl;
            std::cout << "Inference Time : " << to_string(detection.inference) + "ms"<< std::endl;
            return 0;
        }
        if(frame_index == 0)
        {
            detection = detector.detect(frame);

            counter.setPadInfo(detection.info);
        }

        if(frame_index == 1)
        {
            detection = detector.detect(frame);

            frame_index = 0;
        }
        tracker.Run(boxes);
        boxes = detector.postProcess(frame, detection, cl);
        const auto tracks = tracker.GetTracks();
        counter.preprocess(frame);
        counter.process(frame, tracks, frame_count);
        counter.postprocess(frame);

        // for(cv::Rect rectangle : detection.boxes)
        // {
        //     cv::rectangle(frame, rectangle, Scalar(0, 0, 255), 1);
        // }
        frame_index++;
        frame_count++;
        imshow(trackingWin, frame);
        cv::waitKey(60);
    }
    // while(cv::waitKey(0) != 27);
}
