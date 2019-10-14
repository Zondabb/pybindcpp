#include "precomp.hpp"
#include "config.hpp"

// #include <opencv2/opencv.hpp>

namespace cv {
namespace dnn_inference {

bool Model::open(const String& model_file) {
    INFO_LOG("model file: %s", model_file.c_str());
    return true;
}

bool Model::landmark(InputArray src, std::vector<cv::Rect>& objects,
        OutputArray points) {

    std::vector<int> points_dim = {static_cast<int>(objects.size()), 68, 3};
    cv::Mat landmark_pts(points_dim, CV_32F);
    cv::Mat _src = src.getMat();
    int batch_size = objects.size();
    for (int i = 0; i < batch_size; i++) {
        int length = 68;
        for (int c = 0; c < length; ++c) {
            landmark_pts.at<float>(i, c, 0) = 1.0f;
            landmark_pts.at<float>(i, c, 1) = 2.0f;
            landmark_pts.at<float>(i, c, 2) = 3.0f;
        }
    }
    landmark_pts.copyTo(points);
    return true;
}

void Model::face_detect(InputArray src, CV_OUT std::vector<Rect>& objects) {
    int ret;
    cv::Mat _src = src.getMat();
    int image_width = _src.cols;
    int image_height = _src.rows;
    
    objects.clear();
    for (int i = 0; i < 2; i++) {
        objects.push_back({
            static_cast<int>(1), 
            static_cast<int>(2),
            static_cast<int>(3), 
            static_cast<int>(4),
        });
    }
}

} // dnn_inference
} // cv