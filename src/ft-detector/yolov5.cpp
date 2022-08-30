#include <ft-detector/yolov5.h>
#include <spdlog/spdlog.h>

YoloV5::YoloV5(const std::string &modelPath, const std::string &labelsPath) {}

void YoloV5::loadModel(const std::string &modelPath) {
    model_ = std::make_unique<torch::jit::script::Module>(torch::jit::load(modelPath));
}

void YoloV5::preprocess(cv::Mat &img) {}

void YoloV5::detect(const cv::Mat &image, std::vector<Detection> &detections) {}