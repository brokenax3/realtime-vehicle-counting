# Real-time Vehicle Counting

Counting vehicles in real-time using OpenCV and Machine Vision

# Requirements

Running with CUDA would need an Nvidia GPU. The OpenCV backend and default target runs on a CPU.
This application runs using CUDA by default.

## Getting ONNX Files

```
# Clone YOLOv5 Repo
git clone https://github.com/ultralytics/yolov5
cd yolov5

# Install Requirements
pip install -r requirements.txt

# Export ONNX Files
python export.py --data data/coco.yaml --weights yolov5n.pt --include onnx      # Nano
python export.py --data data/coco.yaml --weights yolov5s.pt --include onnx      # Small
python export.py --data data/coco.yaml --weights yolov5l.pt --include onnx      # Large

mkdir ../dnn_files
mv *.onnx ../dnn_files
```

## Building

```
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

# Running

Make sure that ONNX files are obtained from [YOLOv5](https://github.com/ultralytics/yolov5) and put
into `$SOURCE_DIR/dnn_files`.

```
# Running with TUI Interface
python python/rvc-tui.py

# Running with binary file
./build/realtime-counting --help
```

## Results

### test_video

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
5. traditional + SORT
   - Actual count : 239
   - Program count : 145
   - Accuracy : 60.669456
   - FPS : 90.9091
   - Inference Time : 2.736931ms

### rain_high_angle

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
5. traditional + SORT
   - Actual count : 368
   - Program count : 392
   - Accuracy : 93.478264
   - FPS : 90.9091
   - Inference Time : 2.489109ms

### night_high_angle_low_exposure

```bash
# Parameters

./build/realtime-counting \
    --model_path="./dnn_files/yolov5n.onnx" \
    --class_path="./dnn_files/coco_c.names" \
    --model_size=640 \
    --roi_A=250,50,250,400 \
    --roi_B=300,60,200,454 \
    --night_mode=1
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
2. yolov5l
   - Actual count : 101
   - Program count : 9
   - Accuracy : 8.910889
   - FPS : 10.8696
   - Inference Time : 3.387015ms
3. yolov5n
   - Actual count : 101
   - Program count : 1
   - Accuracy : 0.990099
   - FPS : 41.6667
   - Inference Time : 2.044405ms

### night_high_angle

1. Night Vision
   - night_high_angle
   - Actual Count : 105
   - Program count : 76
   - Accuracy : 72.3809
   - FPS : 71.4286
   - Inference Time : 0.872760ms
2. yolov5n
   - Actual count : 105
   - Program count : 4
   - Accuracy : 3.8095
   - FPS : 41.6667
   - Inference Time : 2.029749ms
3. yolov5l
   - Actual count : 105
   - Program count : 36
   - Accuracy : 34.2857
   - FPS : 11.6279
   - Inference Time : 3.401693ms

## Visualising ONNX Files

[Netron](https://netron.app/)
