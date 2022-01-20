#include <fstream>
#include <sstream>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>

/* std::tuple<std::vector<cv::Rect>, cv::Mat> dnnProcess(cv::Mat frame); */
std::vector<cv::Rect> dnnProcess(cv::Mat &frame);
void preprocess(const cv::Mat& frame, cv::dnn::Net& net, cv::Size inpSize, float scale);
std::vector<cv::Rect> postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs, cv::dnn::Net& net, int backend);
