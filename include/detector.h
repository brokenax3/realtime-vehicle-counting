#ifndef __DETECTOR_H__
#define __DETECTOR_H__
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>

#include "utils.h"

using namespace std;
using cv::Mat;
using cv::Scalar;
using cv::Size;
using cv::Rect;

struct PadInfo
{
    float scale;
    int top;
    int left;
    int bottom;
};

struct Detection
{
    PadInfo info;
    std::vector<Mat> detection;
    float inference;
};

class Colors
{
    public:
        vector<string> hex_str;
        vector<Scalar> palette;
        int n = 20;
        Colors():hex_str(20,"")
        {
            this->hex_str = {
                "FF3838", "FF9D97", "FF701F", "FFB21D", "CFD231", "48F90A", "92CC17", "3DDB86", "1A9334", "00D4BB",
                "2C99A8", "00C2FF", "344593", "6473FF", "0018EC", "8438FF", "520085", "CB38FF", "FF95C8", "FF37C7"};
            for (auto &ele : this->hex_str)
            {
                palette.push_back(hex2rgb(ele));
            }
        }
        Scalar hex2rgb(string &hex_color)
        {
            int b, g, r;
            sscanf(hex_color.substr(0, 2).c_str(), "%x", &r);
            sscanf(hex_color.substr(2, 2).c_str(), "%x", &g);
            sscanf(hex_color.substr(4, 2).c_str(), "%x", &b);
            return Scalar(b, g, r);
        }
};

struct Config
{
    float confThreshold;
    float nmsThreshold;
    string weightPath;
    string classNamePath;
    Size size;
    bool _auto;
};

class Detector
{
    public:
        Detector(Config &config);
        ~Detector() = default;
        Detection detect(Mat &img);
        // void postProcess(Mat &img, Detection &detection, Colors&cl);
        std::vector<Rect> postProcess(Mat &img, Detection &detection, Colors &cl);
        vector<Rect> makeBoxes(Mat &img, Detection &detection);
        PadInfo letterbox(Mat &img, Size new_shape, Scalar color, bool _auto, bool scaleFill, bool scaleup, int stride);

    private:
        float nmsThreshold;
        float confThreshold;
        Size inSize;
        bool _auto;
        vector<string> classNames;
        cv::dnn::Net model;
        void drawPrediction(Mat &img, vector<Rect> &boxes, vector<float> &sc, vector<int> &clsIndexs, vector<int> &ind, Colors&cl);
};
#endif
