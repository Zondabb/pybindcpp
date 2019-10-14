#ifndef __OPENCV_AV_STREAM_PRECOMP_HPP__
#define __OPENCV_AV_STREAM_PRECOMP_HPP__

#include "../include/opencv2/av_stream.hpp"
#include "opencv2/imgproc/imgproc_c.h"

#include <string>

struct AVCapture
{
    virtual ~AVCapture() {}
    virtual bool open(std::string) { return true; }
    virtual bool grabFrame() { return true; }
    virtual IplImage* retrieveFrame(int) { return 0; }
};

#endif