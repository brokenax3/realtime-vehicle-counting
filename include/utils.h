#ifndef __UTILS_H__
#define __UTILS_H__

// Escape constant
const int ESC = 27;

// Constants
constexpr int kNumColors = 32;
constexpr int kMaxCoastCycles = 6;
constexpr int kMinHits = 1;
// Set threshold to 0 to accept all detections
constexpr float kMinConfidence = 0.3;
constexpr float nmsThreshold = 0.45;

constexpr int roiHeight = 10;
constexpr int notFoundMax = 200;
constexpr int boxMaxArea = 200000;

#endif
