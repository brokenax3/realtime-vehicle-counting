#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "detector.h"
#include "helper.h"
#include "tracker.h"
#include "utils.h"

// struct Roi {
//    public:
//     int width;
//     int height;
//     int x;
//     int y;
//     Roi(int width, int height, int x, int y){};
// };

struct Item {
    int id;
    int notFoundCount;
};

struct findItem {
    int id;
    findItem(int id) : id(id) {}
    bool operator()(const Item &item) const { return item.id == id; }
};

class Counter {
   private:
    int count = 0;
    PadInfo padInfo;
    std::vector<Item> pass_ROI_a;
    std::vector<Item> pass_ROI_b;
    cv::Rect roiA;
    cv::Rect roiB;

   public:
    // Counter();
    // Counter(Roi roiA, Roi roiB);
    void setParams(Roi roiA, Roi roiB, PadInfo padInfo);
    // void setPadInfo(PadInfo padInfo);
    // ~Counter() = default;
    void preprocess(cv::Mat &img);
    void process(cv::Mat &img, const std::map<int, Track> tracks,
                 int frame_index);
    void processNight(cv::Mat &img, const std::map<int, Track> tracks,
                      int frame_index);
    void postprocess(cv::Mat &img);
    int getCount();
};
