cmake_minimum_required(VERSION 3.22)

project(out)

FIND_PACKAGE(OpenCV REQUIRED)                              
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

ADD_EXECUTABLE(out processing_experiment.cpp)                         
TARGET_LINK_LIBRARIES(out ${OpenCV_LIBS})
