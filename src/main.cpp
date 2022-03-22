#include <stdlib.h>

#include <boost/format.hpp>
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

void previewRoi(cv::Mat frame, Detection detection, Detector detector,
                Counter counter, Roi roiA, Roi roiB, Colors cl) {
    bool isNight;

    cv::Mat previewImage = frame.clone();

    detection = detector.detect(previewImage);
    counter.setParams(roiA, roiB, detection.info);
    detector.postProcess(previewImage, detection, cl);
    isNight = detector.autoNightMode(previewImage, true, detection.info);
    counter.preprocess(previewImage);
    detector.setNight(isNight);

    imshow("Detection Preview", previewImage);
}

int main(int argc, char* argv[]) {
    // OpenCV Configuration
    int k;
    cv::redirectError(handleError);
    // Parse arguments from command line
    po::variables_map vm;

    vm = parse_cmd(argc, argv);

    //
    // Setup program arguments
    //
    string model_path = vm["model_path"].as<string>();
    string class_path = vm["class_path"].as<string>();
    int model_size = vm["model_size"].as<int>();
    Config config = {kMinConfidence,
                     nmsThreshold,
                     model_path,
                     class_path,
                     cv::Size(model_size, model_size),
                     false};

    // Night Mode Configuration
    bool isNight;
    // Night Mode Detection Rois
    std::vector<cv::Rect> nightRois;

    for (int i = 1; i < 3; i++) {
        string param_string = "night_roi_" + to_string(i);
        Roi tempRoi = vm[param_string].as<Roi>();
        nightRois.push_back(
            cv::Rect(tempRoi.x, tempRoi.y, tempRoi.width, tempRoi.height));
    }

    // Setup Object Detector
    Detector detector(config, nightRois);

    // Force night mode
    int force_night = vm["night_mode"].as<int>() == 1;
    if (force_night) {
        detector.setNight(true);
    }

    // Open a video file or an image file or a camera stream.
    cv::VideoCapture cap;
    // cv::Mat frame = cv::imread("testimg3.png");
    cap.open(2);

    // Check if Camera is available
    if (!cap.isOpened()) {
        cout << "Error! Unable to open camera" << endl;
        exit(-1);
    }

    // Process frames.
    vector<cv::Rect> boxes;

    // Create SORT Tracker
    Tracker tracker;
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

    //
    // Preview Mode Setting
    //
    int preview_bool = vm["preview"].as<int>();
    while (preview_bool == 1) {
        cap >> frame;
        previewRoi(frame, detection, detector, counter, roiA, roiB, cl);
        k = cv::waitKey(1);

        // Press 's' to start and 'esc' to end
        if (k == 's') {
            cv::destroyWindow("Detection Preview");
            break;
        } else if (k == ESC) {
            exit(1);
        }
    }

    // Create a window
    static const string trackingWin = "Tracking Window";
    namedWindow(trackingWin, cv::WINDOW_NORMAL);

    while (true) {
        k = cv::waitKey(1);
        auto fpsTimeStart = chrono::high_resolution_clock::now();

        cap >> frame;

        if (frame.empty() == true || k == 27) {
            cap.release();
            cout << boost::format("Actual count : %1% \n") % actual_count
                 << boost::format("Program count : %1% \n") % counter.getCount()
                 << boost::format("Accuracy : %1% \n") %
                        to_string(
                            (1 - (float)abs(actual_count - counter.getCount()) /
                                     actual_count) *
                            100)
                 << boost::format("FPS : %1% \n") % rtFPS
                 << boost::format("Inference Time : %1%ms \n") %
                        to_string(detection.inference)
                 << endl;
            return 0;
        }

        if (frame_count == 0) {
            detection = detector.detect(frame);
            counter.setParams(roiA, roiB, detection.info);
        }
        //
        // if ((frame_count % 120 == 0) && (force_night == 0)) {
        //     isNight = detector.autoNightMode(frame, false, detection.info);
        //     detector.setNight(isNight);
        // }

        detection = detector.detect(frame);

        boxes = detector.postProcess(frame, detection, cl);
        tracker.Run(boxes);
        const auto tracks = tracker.GetTracks();
        counter.preprocess(frame);
        counter.processExp(frame, tracks, frame_count);
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
                // cv::putText(frame, std::to_string(trk.first),
                //             cv::Point(box.tl().x, box.tl().y - 10),
                //             cv::FONT_HERSHEY_DUPLEX, 2,
                //             cv::Scalar(255, 255, 255), 2);
                cv::rectangle(frame, box, cv::Scalar(0, 0, 255), 1);
            }
        }

        frame_count++;
        imshow(trackingWin, frame);
    }
}
