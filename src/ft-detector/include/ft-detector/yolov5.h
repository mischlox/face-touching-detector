#ifndef YOLOV5_H
#define YOLOV5_H

#include <ft-detector/detector.h>
#include <ft-detector/yolov5_config.h>

// QT also uses slots so undef before including to avoid conflicts
#undef slots
#include <torch/script.h>
#define slots Q_SLOTS

#include <opencv4/opencv2/opencv.hpp>

class YoloV5 : public Detector {
   public:
    YoloV5(const std::string &modelPath, const std::string &labelsPath, bool enableGPU = true);

    void loadModel(const std::string &modelPath) override;
    void detect(const cv::Mat &inputImage, std::vector<Detection> &detections) override;

   private:
    std::unique_ptr<torch::jit::script::Module> model_;
    torch::Device device_ = torch::kCUDA;  // torch::kCPU or torch::kCUDA

    /**
     * Apply Resizing and and change BGR to RGB format
     *
     * @param - img that is modified
     */
    void preprocess(cv::Mat &img);

    /**
     * Convert input image into torch-compatible tensor format. Also apply further preprocessing
     * (e.g. normalization)
     *
     * @param - img Already preprocessed (resized, BGR->RGB) input image
     * @return Input Tensor for model
     */
    torch::Tensor imgToTensor(const cv::Mat &img);

    /**
     * Applies Non-Maximum Supression on model output tensor
     *
     * @param preds - Model Output tensor
     * @return vector of predictions processed with NMS
     */
    std::vector<torch::Tensor> nms(torch::Tensor &preds);

    /**
     * Calls Non-Maximum-Suppression, Retrieve bbox information and convert model output to
     * detections format
     *
     * @param modelOutput - Output tensor of model
     * @param detections - This vector will be filled with detected bbox info
     * @param imgWidth - Width of original input image
     * @param imgHeight - Height of original input image
     */
    void postprocess(torch::Tensor &modelOutput, std::vector<Detection> &detections, int imgWidth,
                     int imgHeight);
};

#endif  // YOLOV5_H
