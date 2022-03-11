# Results

## test_video

```bash
# Parameters

# The program runs with the following defaults
./build/realtime-counting \
    --model_path="./dnn_files/yolov5s-opt.onnx" \
    --class_path="./dnn_files/coco_c.names" \
    --model_size=640 \
    --roi_A=640,100,0,0 \
    --roi_B=640,100,0,350
```

- Actual Count = 239

1. yolov5n-opt + SORT + default roi
   - Program count : 228
   - Accuracy : 95.397491%
   - FPS : 47.619
   - Inference Time : 1.978215ms
2. yolov5s-opt + SORT + default roi
   - Program count : 235
   - Accuracy : 98.326363%
   - FPS : 32.2581
   - Inference Time : 2.047566ms

## rain_high_angle

```bash
# Parameters

./build/realtime-counting \
    --model_path="./dnn_files/yolov5s-opt.onnx" \
    --class_path="./dnn_files/coco_c.names" \
    --model_size=640 \
    --roi_A=90,50,356,320 \
    --roi_B=200,80,288,404
```

- Actual Count = 368

1. yolov5s-opt + SORT
   - Program count : 368
   - Accuracy : 100%
   - FPS : 29.4118
   - Inference Time : 2.035006ms
2. yolov5n-opt + SORT + default roi
   - Program count : 354
   - Accuracy : 96.195648%
   - FPS : 41.6667
   - Inference Time : 2.189759ms

Smaller images require models with a higher mAP to produce a more accurate result.

## night_high_angle_low_exposure

```bash
# Parameters

./build/realtime-counting \
    --model_path="./dnn_files/yolov5n.onnx" \
    --class_path="./dnn_files/coco_c.names" \
    --model_size=640 \
    --roi_A=250,50,250,400 \
    --roi_B=300,60,200,454
```

- Actual Count = 101

1. Night Vision + Thresholding + Very Low Exposure
   - Params :
     - roi_A=90,30,356,346
     - roi_B=200,60,288,404
   - Program Count : 105
   - Accuracy : 96.039604%
   - FPS : 90.9091
   - Inference Time : 0.683610ms
