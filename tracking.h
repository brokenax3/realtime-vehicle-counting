/* Functions which enable object tracking */
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

#include "objects.cpp"

class CentroidTracker
{
    private:
    public:
        std::vector<Object> objects;
        int max_disappear = 20;
        void registerObject(Centroid centroid);
        void unregisterObject(int id);
        void updateObjects(std::vector<cv::Rect> rectangles);
};


