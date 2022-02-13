# realtime-vehicle-counting
Counting vehicles in realtime using OpenCV and Machine Vision

## Requirements
Running with CUDA would need an Nvidia GPU. The OpenCV backend and default target runs on a CPU.

## Building
```
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Running
```
./build/realtime-counting
```
