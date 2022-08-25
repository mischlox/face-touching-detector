//
// Created by mischlox on 25.08.22.
//

#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/opencv.hpp>

struct Detection {
    int classID;
    float confidence;
    cv::Rect box;
};

class Detector {
   public:
    /**
     * Run detect
     *
     * @param img
     * @param detections
     */
    virtual void detect(const cv::Mat &img,
                        std::vector<Detection> &detections) = 0;

    virtual void loadNet(const std::string &model_path) = 0;

    void loadClassList(const std::string &labelFile);

    void drawBoxes(cv::Mat &img, const std::vector<Detection> &detections);

    void detectImage(const std::string &imgPath, bool show = true);

    void detectVideo(const std::string &videoPath, bool show = true);

   protected:
    std::vector<std::string> classNames_;
};

#endif  // DETECTOR_H
