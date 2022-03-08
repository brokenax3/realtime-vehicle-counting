#include <stdlib.h>

#include <boost/program_options.hpp>
#include <chrono>
#include <iostream>

#include "counter.h"
#include "detector.h"
#include "helper.h"
#include "tracker.h"
#include "utils.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    // Parse arguments from command line
    po::variables_map vm;
    vm = parse_cmd(argc, argv);

    //
    // Setup program arguments
    //
    string model_path = vm["model_path"].as<string>();
    string class_path = vm["class_path"].as<string>();
    int model_size = vm["model_size"].as<int>();
    // int model_size = 640;
    Config config = {kMinConfidence,
                     nmsThreshold,
                     model_path,
                     class_path,
                     Size(model_size, model_size),
                     false};

    // Setup Object Detector
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
        cerr << "Error! Unable to open camera" << endl;
        return -1;
    }

    // Process frames.
    vector<cv::Rect> boxes;

    // Create SORT Tracker
    Tracker tracker;
    // auto frame_index = 0;
    auto frame_count = 0;
    Colors cl = Colors();

    //
    // Create Counter
    //
    Counter counter;
    // Counter ROIs
    Roi roiA = vm["roi_A"].as<Roi>();
    Roi roiB = vm["roi_B"].as<Roi>();

    // Total Time Required
    cv::Mat frame, tmpFrame;

    float rtFPS;

    // Detection
    Detection detection;

    int actual_count = vm["actual_count"].as<int>();

    while (true) {
        auto fpsTimeStart = chrono::high_resolution_clock::now();
        cap >> frame;

        if (frame.empty() == true) {
            cap.release();
            cout << "Actual count : " << actual_count << endl;
            cout << "Program count : " << to_string(counter.getCount()) << endl;
            cout << "Accuracy : "
                 << to_string(
                        (1 - (float)abs(actual_count - counter.getCount()) /
                                 actual_count) *
                        100)
                 << "%" << endl;
            cout << "FPS : " << rtFPS << endl;
            cout << "Inference Time : " << to_string(detection.inference) + "ms"
                 << endl;
            return 0;
        }

        if (frame_count == 0) {
            detection = detector.detect(frame);
            counter.setParams(roiA, roiB, detection.info);
        }

        detection = detector.detect(frame);

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

        cv::putText(frame, "FPS : " + to_string(rtFPS), cv::Point(0, 30),
                    cv::FONT_HERSHEY_PLAIN, 0.7, cv::Scalar(255, 0, 0), 1);

        // for(cv::Rect rectangle : detection.boxes)
        // {
        //     cv::rectangle(frame, rectangle, Scalar(0, 0, 255), 1);
        // }
        // frame_index++;
        //

        // Draw Tracks
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
