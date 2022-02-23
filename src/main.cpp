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
    detector.setNight(true);

    // Load a model.
    cv::dnn::Net net = cv::dnn::readNetFromONNX(model_path);

    // Create a window
    static const string kWinName = "Deep learning object detection in OpenCV";
    static const string trackingWin = "Tracking Window";
    // namedWindow(kWinName, cv::WINDOW_NORMAL);
    namedWindow(trackingWin, cv::WINDOW_NORMAL);

    // Open a video file or an image file or a camera stream.
    cv::VideoCapture cap;

    // cap.open("./test_video/test_video.mp4");
    cap.open("./test_video/low_light.mp4");

    // Process frames.
    cv::Mat frame, tmpFrame;
    vector<cv::Rect> boxes;

    // Create SORT Tracker
    Tracker tracker;
    auto frame_index = 0;

    Colors cl = Colors();

    // Create Counter
    Counter counter;

    // Total Time Required
    cv::TickMeter totalTime;

    // Detection
    Detection detection;

    totalTime.start();
    while (true)
    {
        cap >> tmpFrame;

        if(tmpFrame.empty() == true)
        {
            totalTime.stop();
            std::cout << "Actual count : 239" << std::endl;
            // std::cout << "Program count : " << to_string(counter.count) << std::endl;
            // std::cout << "Accuracy : " << to_string((1 - (float) abs(237 - counter.count) / 237) * 100) << "%" << std::endl;
            float t = totalTime.getTimeSec();
            std::cout << "Total Time Taken : " << to_string(t) + "s" << std::endl;
            std::cout << "Inference Time : " << to_string(detection.inference) + "ms"<< std::endl;
            return 0;
        }
        if(frame_index == 0)
        {
            frame = tmpFrame;
            detection = detector.detect(frame);
            // boxes = detector.makeBoxes(frame, detection);

            // boxes = detector.postProcess(frame, detection, cl);
            // tracker.Run(boxes);
            // const auto tracks = tracker.GetTracks();
            //
            //
            // counter.setPadInfo(detection.info);
        }
        // Process every 2 frames for faster processing
        if(frame_index == 2)
        {
            frame = tmpFrame;
            detection = detector.detect(frame);
            // boxes = detector.postProcess(frame, detection, cl);
            //
            // frame_index = 0;
            //
            // tracker.Run(boxes);
            // const auto tracks = tracker.GetTracks();
            // counter.preprocess(frame);
            // counter.process(frame, tracks, frame_index);
        }
        // counter.postprocess(frame);
        // frame_index++;

        imshow(trackingWin, frame);
        cv::waitKey(60);
    }
}
