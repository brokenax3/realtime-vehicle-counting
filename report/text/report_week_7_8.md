```
February 03, 2022
Prepared by: Mark Cai Yee Lee
```

# Summary of Work Done
1. Re-evaluation of the components required for accurate object tracking
2. Implementation of SORT algorithm in c++

# Report

## Re-evaluation of Object Tracking

Issues with object tracking had caused a delay in the project. Thus, it was decided to re-evaluate the knowledge on object tracking. Object tracking was significantly different from object detection. Although a simple object detector would be able to achieve the goal of the project, it would also decrease the accuracy of detections.
 This is because using the centroid method would cause objects close to each other to identify as the same object. Vehicles usually move side-by-side at a close distance apart and move in a relatively straight line towards the camera. This would cause misidentification of existing stored objects when using the simple object tracker that was presented.

The following are the problems associated with accurate target tracking.

### Assignment Problem

A multiple target tracking system requires sensors to generate multiple detections from multiple targets in a scan. These detections must be assigned to their appropriate targets. The trackers can also try to maintain tracks to match detections.

- 2-D Assignment Problem
    - This means that only one sensor is being used to track targets
- S-D Assignment Problem
    - This means that object tracking uses a combination of sensors and each sensor would output different number of detections

#### Problems in Multiple Target Tracking

- Target or detection distribution
    - Dense targets are harder to differentiate
- Probability of detection of sensor
    - A target may not get detected by a sensor and may steal detections from other tracks
- Sensor resolution
    - Sensors may not detect all true targets in the scene. This results in tracking errors as multiple tracks are provided
- Clutter or false alarm rate of the sensor
    - This increases the number of possible extra assignments which increases clutter and complexity of data assignment

#### Gating

**Gating** is a screening mechanism to determine which observations are valid candidates to update existing tracks and eliminate unlikely detection-to-track pairs using the distribution information of the predicted tracks

The estimated track for the current step is predicted from the previous step

- Global Nearest Neighbour (GNN) Method
    - When there are multiple tracking candidates present at the output of the tracker, a cost matrix can be evaluated to find the least track to observation cost
    - The cost matrix can be evaluated by penalising less desirable characteristics of observations

## Implementation of SORT algorithm in C++

There is already implementations of the SORT algorithm in Python. It is currently not possible to run Python code with OpenCV on Android as it can only use C++ bindings. Thus, this SORT algorithm would be needed to be translated to C++ to run on android applications.
