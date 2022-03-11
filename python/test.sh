#!/bin/bash


../build/realtime-counting --model_path="../dnn_files/yolov5n-opt.onnx" --class_path="../dnn_files/coco_c.names" --model_size=640 --roi_A=640,100,0,0 --roi_B=640,100,0,350
