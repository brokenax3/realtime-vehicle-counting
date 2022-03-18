```
January 21, 2022
Prepared by: Mark Cai Yee Lee
```

# Report

## Summary of Work Completed

1. Research on Object Tracking
2. Testing multiple Deep Learning Object Detection Algorithms
3. Writing a simple object tracking algorithm
4. Research on Object Tracking using Functions in OpenCV


## Writing a Simple Object Tracking Algorithm

The premise behind object tracking is that the centroid of an object can be remembered at first and compared to the next available centroids in the next frames. When tracking multiple objects, the centroid with the smallest distance between the previous centroid is taken. Next, any additional centroids are treated as new objects. When an object has not been detected for a set amount of frames, it is removed from the list of objects.

An issue that I have encountered is that when a vehicle is not always detected will make the algorithm spawn new objects. This issue affects the minimum distance which differentiate objects from one another. In addition, the angle which the camera is placed would cause the centroid of an object to move faster as the object gets closer to the camera.

## Testing Multiple Deep Learning Object Detection Algorithms

The program aims to be modular to be able to compare traditional methods and deep learning methods. Next, pre-trained models and weights are currently used for simplicity. However, deep learning algorithms do require a lot of processing power. In my case, I have used YOLOv4-Mish-416 and YOLOv4-Tiny. YOLOv4-Mish-416 runs very well on a discrete graphics card and provides accurate object detection. YOLOv4-Tiny runs well without a graphics card and provides and adequate accuracy when detecting objects. This algorithm combined with a good camera angle would be able to provide sufficient accuracy with low enough processing power for a mobile device.

More deep learning algorithms would be tested in the future once object tracking is handled properly.

The following uses the example code provided from [OpenCV](https://docs.opencv.org/4.5.5/d4/db9/samples_2dnn_2object_detection_8cpp-example.html)

```bash
./build/out -c=./yolov4-mish-416.cfg -m=./yolov4-mish-416.weights \
    -i=./test_video/downscaled.mp4 --width=416 --height=416 \
    --classes=./coco_classes.txt --scale=0.0039215 --target=0 --backend=0
```

- `--width` and `--height` correspond to the input resolution used for training.
- Target and Backend correnspond to computational target and backend.

## Research on Object Tracking using OpenCV

As noted in the section above, simple centroid tracking does not provide enough accuracy when objects are not consistently being detected. This is because the drastic movement in centroid and vehicles moving quite fast on a highway. Given a vehicle detection line, the simple centroid tracking algorithm that I have written might treat an object as another new object as it moves towards the camera. 

This week's goal would be to improve on object tracking with the following methods:

1. Using OpenCV Built-in Object Tracking Functions
2. Using two or three vehicle detection lines where a bounding rectangle must pass through all of them to be counted as a vehicle

## Conclusion

The task of using deep learning algorithms and combining with Object Tracking took much more time than expected. This was due to lack of example code and explanations using this with OpenCV online. 

In addition, I have still yet to be able to figure out how to integrate OpenCV Object Tracking Functions within the program.
