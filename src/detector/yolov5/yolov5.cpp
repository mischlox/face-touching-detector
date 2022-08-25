#include "yolov5.h"

#include <spdlog/spdlog.h>

YoloV5::YoloV5(const std::string &modelPath, const std::string &classListPath) {
}

void YoloV5::loadNet(const std::string &model_path) {}

void YoloV5::preprocess(cv::Mat &img) {}

void YoloV5::detect(const cv::Mat &image, std::vector<Detection> &detections) {}