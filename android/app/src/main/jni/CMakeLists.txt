cmake_minimum_required(VERSION 3.18.1)

project(appmodules)
set(REACT_ANDROID_DIR ${CMAKE_SOURCE_DIR}/../../../../../node_modules/react-native/ReactAndroid)
# ---- Include shared headers ----
file(GLOB SHARED_CPP ${CMAKE_SOURCE_DIR}/../../../../../cpp/*.cpp)
include_directories(${CMAKE_SOURCE_DIR}/../../../../../cpp)
include(${REACT_ANDROID_DIR}/cmake-utils/ReactNative-application.cmake)
# ---- Build native library ----
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        OnLoad.cpp
        CameraJNI.cpp
        ${SHARED_CPP}

)
# ---- Android system libraries ----
find_library(log-lib log)
find_library(android-lib android)


