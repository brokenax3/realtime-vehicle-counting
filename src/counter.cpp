#include "counter.h"

void Counter::setParams(Roi roiA, Roi roiB, PadInfo padInfo) {
    this->padInfo = padInfo;

    // If default ROI values are set, let the y value of roiA to be the top of
    // the frame
    if ((roiA.x == 0) && (roiA.y == 0)) {
        roiA.y = padInfo.top;
    }

    cv::Rect rectA(roiA.x, roiA.y, roiA.width, roiA.height);
    cv::Rect rectB(roiB.x, roiB.y, roiB.width, roiB.height);

    this->roiA = rectA;
    this->roiB = rectB;
};

void Counter::preprocess(cv::Mat &img) {
    cv::rectangle(img, this->roiA, cv::Scalar(0, 255, 255), 1);
    cv::rectangle(img, this->roiB, cv::Scalar(0, 0, 255), 1);
}

// TODO: Counting Both Ways
//  Current implementation counts when the object passes through ROIA and then
//  ROIB
void Counter::process(cv::Mat &img, const std::map<int, Track> tracks,
                      int frame_index) {
    // for(const auto& [id, track] : tracks)
    for (auto &trk : tracks) {
        // Only count tracks which meet certain criteria
        if (trk.second.coast_cycles_ < kMaxCoastCycles &&
            (trk.second.hit_streak_ >= kMinHits || frame_index < kMinHits)) {
            // const auto &bbox = trk.second.GetStateAsBbox();
            // cv::putText(img_tracking, std::to_string(trk.first),
            // cv::Point(bbox.tl().x, bbox.tl().y - 10),
            // cv::FONT_HERSHEY_DUPLEX, 2, cv::Scalar(255, 255, 255), 2);
            // cv::rectangle(img_tracking, bbox, colors[trk.first % kNumColors],
            // 3);
            int id = trk.first;
            Track track = trk.second;
            std::vector<Item>::iterator idx_a, idx_b;
            idx_a = std::find_if(this->pass_ROI_a.begin(),
                                 this->pass_ROI_a.end(), findItem(id));
            idx_b = std::find_if(this->pass_ROI_b.begin(),
                                 this->pass_ROI_b.end(), findItem(id));
            cv::Rect box = track.GetStateAsBbox();
            if (((roiA & box).area() > 0) && ((roiB & box).area() > 0)) {
                // Should not intersect two roi at the same time
                continue;
            }

            // Track Index not in ROIA
            if (idx_a == this->pass_ROI_a.end()) {
                // Look for intersection
                if ((roiA & box).area() > 0) {
                    // std::cout << "Overlapping detected " << id << " in A" <<
                    // std::endl;
                    Item tmp = {id, 0};
                    this->pass_ROI_a.push_back(tmp);
                }
            } else {
                if (this->pass_ROI_a.at(idx_a - this->pass_ROI_a.begin())
                        .notFoundCount >= notFoundMax) {
                    this->pass_ROI_a.erase(idx_a);
                    // std::cout << "Removing Item from A" << std::endl;
                    continue;
                }
                this->pass_ROI_a.at(idx_a - this->pass_ROI_a.begin())
                    .notFoundCount++;
            }

            // Track Index not in ROIB
            if (idx_b == this->pass_ROI_b.end()) {
                // Look for intersection
                if ((roiB & box).area() > 0) {
                    // std::cout << "Overlapping detected " << id << " in B" <<
                    // std::endl;
                    Item tmp = {id, 0};
                    this->pass_ROI_b.push_back(tmp);
                }
            } else {
                if (idx_a != this->pass_ROI_a.end()) {
                    if (this->pass_ROI_b.at(idx_b - this->pass_ROI_b.begin())
                            .notFoundCount >
                        this->pass_ROI_a.at(idx_a - this->pass_ROI_a.begin())
                            .notFoundCount) {
                        continue;
                    }
                    // std::cout << "Found in both" << std::endl;
                    this->pass_ROI_a.erase(idx_a);
                    this->pass_ROI_b.erase(idx_b);
                    this->count++;
                    // std::cout << "Count : " << to_string(this->count) <<
                    // std::endl;
                } else {
                    if (this->pass_ROI_b.at(idx_b - this->pass_ROI_b.begin())
                            .notFoundCount >= notFoundMax) {
                        this->pass_ROI_b.erase(idx_b);
                        // std::cout << "Removing Item from B" << std::endl;
                        continue;
                    }
                    this->pass_ROI_b.at(idx_b - this->pass_ROI_b.begin())
                        .notFoundCount++;
                }
            }
        }
    }
}

void Counter::processNight(cv::Mat &img, const std::map<int, Track> tracks,
                           int frame_index) {
    for (auto &trk : tracks) {
        // Only count tracks which meet certain criteria
        if (trk.second.coast_cycles_ < kMaxCoastCycles &&
            (trk.second.hit_streak_ >= kMinHits || frame_index < kMinHits)) {
        }
    }
}

int Counter::getCount() { return this->count; }

void Counter::postprocess(cv::Mat &img) {
    cv::putText(img, "Count : " + to_string(this->count),
                cv::Point(this->padInfo.left, this->padInfo.top - 25),
                cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 255, 0), 2);
}
