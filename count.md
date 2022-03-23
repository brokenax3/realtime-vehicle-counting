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
   - Program count : 234
   - Accuracy : 97.907951
   - FPS : 47.619
   - Inference Time : 1.894408ms
2. yolov5s-opt + SORT + default roi
   - Actual count : 239
   - Program count : 243
   - Accuracy : 98.326363
   - FPS : 31.25
   - Inference Time : 2.246295ms
3. yolov5m-opt + SORT + default roi
   - Actual count : 239
   - Program count : 242
   - Accuracy : 98.744766
   - FPS : 19.2308
   - Inference Time : 2.797125ms
4. yolov5l-opt + SORT + default roi
   - Actual count : 239
   - Program count : 241
   - Accuracy : 99.163177
   - FPS : 11.7647
   - Inference Time : 3.621556ms

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

1. yolov5n-opt + SORT
   - Actual Count = 368
   - Program count : 366
   - Accuracy : 99.456217
   - FPS : 47.619
   - Inference Time : 2.103620ms
2. yolov5s-opt + SORT
   - Actual count : 368
   - Program count : 367
   - Accuracy : 99.728264
   - FPS : 28.5714
   - Inference Time : 2.039811ms
3. yolov5m-opt + SORT
   - Actual count : 368
   - Program count : 368
   - Accuracy : 100.000000
   - FPS : 19.6078
   - Inference Time : 2.748721ms
4. yolov5l-opt + SORT
   - Actual count : 368
   - Program count : 372
   - Accuracy : 98.913040
   - FPS : 11.3636
   - Inference Time : 3.355781ms

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

# Kind of neural networks

11
