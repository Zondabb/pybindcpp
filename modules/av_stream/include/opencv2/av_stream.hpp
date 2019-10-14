#ifndef __OPENCV_AV_STREAM_HPP__
#define __OPENCV_AV_STREAM_HPP__

#include "opencv2/core.hpp"

typedef struct AVCapture AVCapture;
// typedef struct MediaReader MediaReader;

namespace cv {
namespace av_stream {

class CV_EXPORTS_W VideoCapture {
public:
    CV_WRAP VideoCapture() {}
    CV_WRAP VideoCapture(const String& filename);
    virtual ~VideoCapture() {}

    CV_WRAP virtual bool open(const String& filename);
    CV_WRAP virtual bool read(OutputArray image);
    CV_WRAP virtual bool grab();
    CV_WRAP virtual bool retrieve(OutputArray image, int flag = 0);

private:
    Ptr<AVCapture> cap;
    // Ptr<MediaReader> reader;
};

} //av_stream
} //cv

#endif