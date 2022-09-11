//
// Created by mischlox on 25.08.22.
//

#ifndef DETECTOR_H
#define DETECTOR_H

#include <ft-detector/bbox_intersect_notifier.h>

#include <opencv2/opencv.hpp>

struct Detection {
    int classID;
    float confidence;
    cv::Rect box;
};

class Detector {
   public:
    Detector();

    virtual void detect(const cv::Mat &img, std::vector<Detection> &detections) = 0;

    virtual void loadModel(const std::string &model_path) = 0;

    void readLabels(const std::string &labelFile);

    void drawBoxes(cv::Mat &img, const std::vector<Detection> &detections);

    bool boxesOverlap(const std::vector<Detection> &detections);

    void detectImage(const std::string &imgPath, bool show = true);

    void detectVideo(const std::string &videoPath, bool show = true);

    std::vector<std::string> getLabels() { return labels_; }

   protected:
    std::vector<std::string> labels_;

   private:
    std::unique_ptr<IntersectNotifier> notifier_ = std::make_unique<IntersectNotifier>();
};

#endif  // DETECTOR_H
