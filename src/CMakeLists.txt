cmake_minimum_required(VERSION 3.22)

SET(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
SET(CMAKE_HAVE_THREADS_LIBRARY 1)
SET(CMAKE_USE_WIN32_THREADS_INIT 0)
SET(CMAKE_USE_PTHREADS_INIT 1)
SET(THREADS_PREFER_PTHREAD_FLAG ON)

project(out)

FIND_PACKAGE(OpenCV REQUIRED)                              
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
# ADD_EXECUTABLE(out processing_experiment.cpp)                         
ADD_EXECUTABLE(out main.cpp dnn_processing.cpp tracking.cpp objects.cpp helper.cpp)                         
TARGET_LINK_LIBRARIES(out ${OpenCV_LIBS})
