#ifndef YOLOV5_H
#define YOLOV5_H

#include <opencv2/opencv.hpp>

#include "../detector.h"
#include "yolov5_config.h"

class YoloV5 : public Detector {
   public:
    YoloV5(const std::string &modelPath, const std::string &classListPath);

    void loadNet(const std::string &model_path) override;
    void detect(const cv::Mat &img, std::vector<Detection> &detections) override;

   private:
    std::vector<std::string> classNames_;

    // Model

    void preprocess(cv::Mat &img);
};

#endif  // YOLOV5_H
