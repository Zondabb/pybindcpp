set(the_description "Video stream processing using ffmpeg")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_CXX_FLAGS "-O2 -g -Wall -Wno-unused-function -std=c++11 -ldl -lm -lpthread -fpermissive")


find_package(FFmpeg)
if(FFmpeg_FOUND)
    message(STATUS "FFmpeg:   YES")
    set(HAVE_FFmpeg 1)
else()
    message(STATUS "FFmpeg:   NO")
endif()

set(AV_SRCS
    config.cpp
    stream_recv.cpp
    )

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${FFMPEG_INCLUDE_DIRS})

ocv_glob_module_sources(${AV_SRCS})
ocv_define_module(av_stream opencv_imgproc ${FFMPEG_LIBRARIES} WRAP python)