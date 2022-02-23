#ifndef __UTILS_H__
#define __UTILS_H__

// Constants
constexpr int kNumColors = 32;
constexpr int kMaxCoastCycles = 1;
constexpr int kMinHits = 3;
// Set threshold to 0 to accept all detections
constexpr float kMinConfidence = 0.3;
constexpr float nmsThreshold = 0.45; 

constexpr int roiHeight = 120; 
constexpr int notFoundMax = 200;
constexpr int boxMaxArea = 200000;



#endif
