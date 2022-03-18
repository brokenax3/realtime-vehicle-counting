# Report
```
December 16, 2021
Prepared by: Mark Cai Yee Lee
```


This report is to update on the progress on Vehicle Counting on Highways using Computer Vision - Accuracy Enhancement.


### Summary of work completed
1. Analysing previous work
2. OpenCV on C++ and Python Comparison
3. OpenCV and C++ Work Environment Setup
    - Learning how to use CMake
    - Language Server Setup
4. Revising C++ and OpenCV Fundamentals
    - Reading images and video
    - Basic Image Manipulation (Erosion, Dilation, Gaussian Blur, Colour Conversion, Resize, Crop)
    - Basic OpenCV Shape and Text Input
    - Revision on Colour Detection
    - Experimenting with Shape Detection
5. Obtaining Test Footage for Experimentation

The goal of the project as clarified in the meeting on the 8th of December (Wednesday) was to produce an implementation of the previous project with several enhancements. First, improving performance in low lighting conditions. Next, implementation as an Android application.

First, OpenCV on C++ and Python Comparison is required as the project needed low power consumption and processing overhead. In this case, C++ is selected as it is faster and is transferable to use for the Android application down the line. Thus, it was decided that C++ would be used for this project. The drawback of using C++ is that it requires more time to produce a prototype. In addition, I am currently not completely familiar with the programming language.

Next, the work environment for C++ and OpenCV was configured. This task took the longest time as previous C/C++ programming projects were only scripts. This configuration was done on a full project scale which uses the CMake build tool. This task is essential as most projects which use C/C++ will use CMake or Ninja. Utilising these tools would benefit to simulate a real-word project.

Third, revision of OpenCV and C++ fundamentals. This task consists of learning OpenCV on YouTube. There were several roadblocks in the process due to the difference in versions of OpenCV. As an example, the createTrackbar function will not support passing the trackbar values into the pointer. This currently requires creating a callback function which handles the output. 

Finally, some test footage was obtained on the pedestrian bridge next to the university. This location was selected as it would cause lower amounts of disruption and less dangerous as compared to the site on the original article.


### Upcoming Tasks Planned
1. Replicating the experiment process and results from the original article
2. Obtaining test footage under low light conditions

# Ideas of Implementation
The goal of this project is to be low cost, low power and low processing overhead. However, I believe that such a device can be designed for more functionality. This means that the idea would not be able to satisfy the goals above.

The idea is to have the deployed device to have enough processing power to capture and stream video. This device will stream data to a server/ computer over the Internet. Next, the footage will be processed on the server with a program which uses https://pjreddie.com/darknet/yolo/ trained for vehicle classification. This result can be updated in real-time on a web app that can be accessed anywhere. 

In regards to implementation with the original goals in mind, I will need to replicate the previous results from the paper before proceeding.

# Conclusion
The project will be completed by the scheduled date. The main issue that would result in a delay would be the mismatch of information due to different OpenCV versions. However, this would be rectified after being more familiar with OpenCV and C++.
