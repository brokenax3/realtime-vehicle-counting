#include <stdlib.h>

#include <boost/format.hpp>
#include <chrono>
#include <iostream>

#include "counter.h"
#include "detector.h"
#include "tracker.h"
#include "utils.h"

using namespace std;

int main() {
    string modelPath = "./dnn_files/yolov5n-opt.onnx";
    Config config = {kMinConfidence, nmsThreshold,
                     modelPath,      "./dnn_files/coco_c.names",
                     Size(640, 640), false};
    // Config config = {kMinConfidence, nmsThreshold, model_path,
    // "./dnn_files/coco.names", cv::Size(640, 640), false};
    Detector detector(config);

    // Force night mode
    detector.setNight(false);

    // Create a window
    static const string kWinName = "Deep learning object detection in OpenCV";
    static const string trackingWin = "Tracking Window";
    namedWindow(trackingWin, cv::WINDOW_NORMAL);

    // Open a video file or an image file or a camera stream.
    cv::VideoCapture cap;
    // cv::Mat frame = cv::imread("testimg3.png");

    cap.open(2);

    // Check if Camera is available
    if (!cap.isOpened()) {
        std::cerr << "Error! Unable to open camera" << std::endl;
        return -1;
    }

    // Process frames.
    vector<cv::Rect> boxes;

    // Create SORT Tracker
    Tracker tracker;
    // auto frame_index = 0;
    auto frame_count = 0;
    Colors cl = Colors();

    // Create Counter
    Counter counter;

    // Total Time Required
    cv::Mat frame, tmpFrame;

    float rtFPS;

    // Detection
    Detection detection;

    // totalTime.start();
    while (true) {
        auto fpsTimeStart = chrono::high_resolution_clock::now();
        cap >> frame;
        // frame = tmpFrame;
        if (frame.empty() == true) {
            cap.release();
            std::cout << "Actual count : 239" << std::endl;
            std::cout << "Program count : " << to_string(counter.count)
                      << std::endl;
            std::cout << "Accuracy : "
                      << to_string((1 - (float)abs(237 - counter.count) / 237) *
                                   100)
                      << "%" << std::endl;
            std::cout << "FPS : " << rtFPS << endl;
            std::cout << "Inference Time : "
                      << to_string(detection.inference) + "ms" << std::endl;
            return 0;
        }
        if (frame_count == 0) {
            detection = detector.detect(frame);
            counter.setPadInfo(detection.info);
        }

        // if(frame_count % 10 == 0)
        // {
        detection = detector.detect(frame);
        // }
        boxes = detector.postProcess(frame, detection, cl);
        tracker.Run(boxes);
        const auto tracks = tracker.GetTracks();
        counter.preprocess(frame);
        counter.process(frame, tracks, frame_count);
        counter.postprocess(frame);
        auto fpsTimeStop = chrono::high_resolution_clock::now();
        auto fpsDuration = chrono::duration_cast<chrono::milliseconds>(
            fpsTimeStop - fpsTimeStart);

        rtFPS = 1 / ((float)fpsDuration.count() / 1000);

        cv::putText(frame, "FPS : " + std::to_string(rtFPS), cv::Point(0, 30),
                    cv::FONT_HERSHEY_PLAIN, 0.7, cv::Scalar(255, 0, 0), 1);

        // for(cv::Rect rectangle : detection.boxes)
        // {
        //     cv::rectangle(frame, rectangle, Scalar(0, 0, 255), 1);
        // }
        // frame_index++;
        for (auto& trk : tracks) {
            if (trk.second.coast_cycles_ < kMaxCoastCycles &&
                (trk.second.hit_streak_ >= kMinHits ||
                 frame_count < kMinHits)) {
                auto box = trk.second.GetStateAsBbox();
                cv::rectangle(frame, box, Scalar(0, 0, 255), 1);
            }
        }

        frame_count++;
        imshow(trackingWin, frame);
        cv::waitKey(1);
    }
}
