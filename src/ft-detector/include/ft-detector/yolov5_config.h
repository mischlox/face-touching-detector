#ifndef YOLOV5_CONFIG_H
#define YOLOV5_CONFIG_H

constexpr float INPUT_WIDTH = 640.0;
constexpr float INPUT_HEIGHT = 640.0;
constexpr float SCORE_THRESHOLD = 0.2;
constexpr float NMS_THRESHOLD = 0.4;

const auto MODEL_PATH = "models/detector_gpu.torchscript";
const auto LABELS_PATH = "labels/classes.txt";

#endif  // YOLOV5_CONFIG_H
