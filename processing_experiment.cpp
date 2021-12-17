#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {

    VideoCapture cap("test_video/hd_autobitrate_close_view.mp4");

    Mat img, imgGrey, imgBlur, imgCanny;

    while (true) {
        cap.read(img);

        cvtColor(img, imgGrey, COLOR_BGR2GRAY);
        GaussianBlur(imgGrey, imgBlur, Size(5, 5), 20);
        Canny(imgBlur, imgCanny, 25, 75);

        /* imshow("Test Video Blur", imgBlur); */
        imshow("Test video", imgCanny);
        waitKey(20);
    }

}
