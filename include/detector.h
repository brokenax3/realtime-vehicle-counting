#ifndef __DETECTOR_H__
#define __DETECTOR_H__
#include <fstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "helper.h"
#include "utils.h"

using namespace std;

struct PadInfo {
    float scale;
    int top;
    int left;
    int bottom;
};

struct Detection {
    PadInfo info;
    std::vector<cv::Mat> detection;
    float inference;
    std::vector<cv::Rect> boxes;
};

class Colors {
   public:
    vector<string> hex_str;
    vector<cv::Scalar> palette;
    int n = 20;
    Colors() : hex_str(20, "") {
        this->hex_str = {"FF3838", "FF9D97", "FF701F", "FFB21D", "CFD231",
                         "48F90A", "92CC17", "3DDB86", "1A9334", "00D4BB",
                         "2C99A8", "00C2FF", "344593", "6473FF", "0018EC",
                         "8438FF", "520085", "CB38FF", "FF95C8", "FF37C7"};
        for (auto &ele : this->hex_str) {
            palette.push_back(hex2rgb(ele));
        }
    }
    cv::Scalar hex2rgb(string &hex_color) {
        int b, g, r;
        sscanf(hex_color.substr(0, 2).c_str(), "%x", &r);
        sscanf(hex_color.substr(2, 2).c_str(), "%x", &g);
        sscanf(hex_color.substr(4, 2).c_str(), "%x", &b);
        return cv::Scalar(b, g, r);
    }
};

struct Config {
    float confThreshold;
    float nmsThreshold;
    string weightPath;
    string classNamePath;
    cv::Size size;
    bool _auto;

    bool night;
};

class Detector {
   public:
    Detector(Config &config, std::vector<cv::Rect> nightRois);
    ~Detector() = default;
    Detection detect(cv::Mat &img);
    // void postProcess(Mat &img, Detection &detection, Colors&cl);
    std::vector<cv::Rect> postProcess(cv::Mat &img, Detection &detection,
                                      Colors &cl);
    vector<cv::Rect> makeBoxes(cv::Mat &img, Detection &detection);
    PadInfo letterbox(cv::Mat &img, cv::Size new_shape, cv::Scalar color,
                      bool _auto, bool scaleFill, bool scaleup, int stride);
    void setNight(bool night);
    vector<cv::Rect> detectNight(cv::Mat &img);
    void preprocessNight(cv::Mat &img);
    bool autoNightMode(cv::Mat &img, bool preview, PadInfo padInfo);

   private:
    float nmsThreshold;
    float confThreshold;
    cv::Size inSize;
    bool _auto;
    bool night;
    vector<string> classNames;
    cv::dnn::Net model;
    cv::Ptr<cv::BackgroundSubtractor> pBackSub;
    void drawPrediction(cv::Mat &img, vector<cv::Rect> &boxes,
                        vector<float> &sc, vector<int> &clsIndexs,
                        vector<int> &ind, Colors &cl);
    void drawPredictionNight(cv::Mat &img, std::vector<cv::Rect> &boxes);
    std::vector<cv::Rect> nightRois;
};
#endif
