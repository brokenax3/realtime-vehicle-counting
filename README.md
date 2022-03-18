# realtime-vehicle-counting

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
   - Program count : 228
   - Accuracy : 95.397491%
   - FPS : 47.619
   - Inference Time : 1.978215ms
2. yolov5s-opt + SORT + default roi
   - Program count : 235
   - Accuracy : 98.326363%
   - FPS : 32.2581
   - Inference Time : 2.047566ms

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

1. yolov5s-opt + SORT
   - Program count : 368
   - Accuracy : 100%
   - FPS : 29.4118
   - Inference Time : 2.035006ms
2. yolov5n-opt + SORT + default roi
   - Program count : 358
   - Accuracy : 97.282608%
   - FPS : 41.6667
   - Inference Time : 2.189759ms

Smaller images require models with a higher mAP to produce a more accurate result.

### night_high_angle_low_exposure

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

## Visualising ONNX Files

[Netron](https://netron.app/)
