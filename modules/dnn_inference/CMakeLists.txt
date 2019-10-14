set(the_description "dnn inference module")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_CXX_FLAGS "-O3 -g -Wall -Wno-unused-function -std=c++11")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(MODEL_SRCS config.cpp)
ocv_glob_module_sources(${MODEL_SRCS})
ocv_define_module(dnn_innference opencv_imgproc WRAP python)