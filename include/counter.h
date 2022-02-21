#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "utils.h"
#include "detector.h"
#include "tracker.h"


struct Item
{
    int id;
    int notFoundCount;
};

struct findItem
{
    int id;
    findItem(int id) : id(id) {}
    bool operator () (const Item& item) const
    {
        return item.id == id;
    }
};

class Counter
{
    private:

    public:
        void setPadInfo(PadInfo padInfo);
        // ~Counter() = default;
        void preprocess(cv::Mat &img);
        void process(cv::Mat &img, const std::map<int, Track> tracks, int frame_index);
        void postprocess(cv::Mat &img);
        int getCount();

        int count = 0;
        std::vector<Item> pass_ROI_a;
        std::vector<Item> pass_ROI_b;
        cv::Rect roiA; 
        cv::Rect roiB;
        PadInfo padInfo;
        

};
