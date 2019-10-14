#include "precomp.hpp"
#include "stream_recv.hpp"
#include "config.hpp"

#include "opencv2/core/utility.hpp"
#include "opencv2/core/private.hpp"

namespace cv {
namespace av_stream {

VideoCapture::VideoCapture(const String& filename) {
    CV_TRACE_FUNCTION();
    open(filename);
}

bool VideoCapture::open(const String& filename) {
    CV_TRACE_FUNCTION();

    cap.reset(new StreamRecv());
    return cap->open(filename);
}

bool VideoCapture::read(OutputArray image)
{
    CV_INSTRUMENT_REGION()

    if(cap->grabFrame()) {
        IplImage* _img = cap->retrieveFrame(0);
        if(!_img) {
            image.release();
            return false;
        }
        if(_img->origin == IPL_ORIGIN_TL) {
            cv::cvarrToMat(_img).copyTo(image);
        } else {
            Mat temp = cv::cvarrToMat(_img);
            flip(temp, image, 0);
        }
    } else {
        image.release();
    }
    return !image.empty();
}

bool VideoCapture::grab() {
    CV_INSTRUMENT_REGION()

    return cap->grabFrame();
}

bool VideoCapture::retrieve(OutputArray image, int flag) {
    CV_INSTRUMENT_REGION()

    IplImage* _img = cap->retrieveFrame(0);
    if(!_img) {
        image.release();
        return false;
    }
    if(_img->origin == IPL_ORIGIN_TL) {
        cv::cvarrToMat(_img).copyTo(image);
    } else {
        Mat temp = cv::cvarrToMat(_img);
        flip(temp, image, 0);
    }
    return true;
}

}   // av_stream
}   // cv
