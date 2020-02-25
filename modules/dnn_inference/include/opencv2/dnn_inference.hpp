#ifndef __OPENCV_DNN_INFERENCE_HPP__
#define __OPENCV_DNN_INFERENCE_HPP__

#include "opencv2/core.hpp"

typedef struct DLContext DLContext;

namespace cv {
namespace dnn_inference {

class CV_EXPORTS_W Model {
public:
    CV_WRAP Model() {}
    virtual ~Model() {}

    CV_WRAP virtual bool open(const String& model_file);
    CV_WRAP void face_detect(InputArray src, CV_OUT std::vector<cv::Rect>& objects);
    CV_WRAP virtual bool landmark(
        InputArray src, std::vector<Rect>& objects, OutputArray points);
    CV_WRAP virtual bool feature(InputArray src, OutputArray feature);
};

} // dnn_inference
} // cv

#endif