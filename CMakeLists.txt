cmake_minimum_required(VERSION 3.22)

project(realtime-counting)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread")
# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED True)
# add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
# Check for dependencies
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    )


file(GLOB SOURCE_FILES src/*.cpp)
# list(REMOVE_ITEM SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/processing_experiment.cpp")
# list(REMOVE_ITEM SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/object_detection.cpp")

message(STATUS "${SOURCE_FILES}")
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(
    ${CMAKE_PROJECT_NAME}
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES})
