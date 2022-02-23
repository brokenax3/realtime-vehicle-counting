cmake_minimum_required(VERSION 3.22)

project(test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread")
# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# Check for dependencies
FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    )
# set(INFERENCE_HELPER_DIR ${CMAKE_CURRENT_LIST_DIR}/extern/InferenceHelper/)
# add_subdirectory(${INFERENCE_HELPER_DIR}/inference_helper inference_helper)


file(GLOB SOURCE_FILES test.cpp)
# list(REMOVE_ITEM SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/processing_experiment.cpp")
# list(REMOVE_ITEM SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/object_detection.cpp")

message(STATUS "${SOURCE_FILES}")
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(
    ${CMAKE_PROJECT_NAME}
    ${OpenCV_LIBS})
    # InferenceHelper)
# target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${INFERENCE_HELPER_DIR}/inference_helper)
