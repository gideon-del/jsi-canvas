cmake_minimum_required(VERSION 3.18.1)

project(appmodules)
# ---- Include shared headers ----
file(GLOB_RECURSE SHARED_CPP
        ${CMAKE_SOURCE_DIR}/../../../../../cpp/**/*.cpp
)

list(FILTER SHARED_CPP EXCLUDE REGEX ".*/tests/.*")
include_directories(${CMAKE_SOURCE_DIR}/../../../../../cpp)
include(${REACT_ANDROID_DIR}/cmake-utils/ReactNative-application.cmake)
# ---- Build native library ----
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        ${SHARED_CPP}
        OnLoad.cpp
        CameraJNI.cpp
        SceneGraphJNI.cpp


)
# ---- Android system libraries ----
find_library(log-lib log)


target_link_libraries(${CMAKE_PROJECT_NAME}
        ${log-lib}

)
