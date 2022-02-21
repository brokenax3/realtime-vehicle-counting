```
February 17, 2022
Prepared by: Mark Cai Yee Lee
```

# Summary of Work Done
1. Refactor object detection sections to incorporate more universal models
2. SORT Algorithm in C++

# Report

## Refactor Object Detection

The previous method for using deep learning object detection models were based on the example from OpenCV. This method utilises darknet models. Darknet models use `.cfg` and `.weights` files which are not universally used in newer models. In this case, it was decided that `.onnx` models are to be used instead as most deep learning models can be converted to this format.

The main issue of this task is that current research papers do not always have the same standard and list of requirements to replicate research results. Moreover, not all deep learning models can be converted and used as normal with OpenCV. This was apparent as the YOLOR and YOLOv4 models are not compatible with this conversion. 

Next, the results shown in `www.paperswithcode.com` are mainly focused on mean average prediction. This website is the main source which compares the performance of many deep learning algorithms. However, this source does not compare the processing requirements of each deep learning algorithm.



