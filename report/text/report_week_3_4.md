```
January 07, 2022
Prepared by: Mark Cai Yee Lee
```

# Report

## Summary of Work Completed

1. Experiment with methods for vehicle detection which require low processing.
2. Android Development Set-up and Learning.
    - Learning how to program Android applications using Kotlin.
    - Learning how to integrate OpenCV with Android applications.
3. Retake of Experiment Video with a stable mount.


## Android Development Set-up and Learning

An Android application (apk file) would consist of two layers. The first is the managed layer which is written in Java or Kotlin. This is compiled as `.dex`. Next, the native layer is written in C++. This layer contains the required shared libraries which the application uses during operation. This is compiled as `.so`. The two layers are linked using the Java Native Interface (JNI).

A major issue is that there are not many up-to-date OpenCV - Android Documentation online. The current latest official documentation is for OpenCV 2.4. The current latest release of OpenCV for Android is version 4.5.5. This had resulted in a delay as a lot of time was spent looking at how to link the OpenCV library for application building.

The linking issue for OpenCV and Android is solved with the following parameters in the CMakeLists file.

```
# OpenCV
# This sets the building architecture of the application. Without this,
# OpenCV will not load.
set(ANDROID_NDK_ABI_NAME x86)

# Use Static Linking
set(OpenCV_STATIC on)

# Set OpenCV Directory
set(OpenCV_DIR /home/mark/OpenCVAndroid/OpenCV-android-sdk/sdk/native/jni)

# Link OpenCV
find_package(OpenCV 4.5 REQUIRED java)

# JNIGraphics
find_library(jnigraphics-lib jnigraphics)

# Linked Libraries
target_link_libraries(
    ...
    ${OpenCV_LIBRARIES}
    ${jnigraphics-lib}
    ...
)
```

Next, the `build.gradle` of the application needs to have the parameters specified within the OpenCV - Android zip file. 
