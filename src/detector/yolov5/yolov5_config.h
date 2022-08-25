//
// Created by mischlox on 22.08.22.
//

#ifndef YOLOV5_CONFIG_H
#define YOLOV5_CONFIG_H

// TFLite specific
constexpr int NUM_THREADS = 4;

// YoloV5 specific
constexpr float INPUT_WIDTH = 640.0;
constexpr float INPUT_HEIGHT = 640.0;
constexpr float SCORE_THRESHOLD = 0.2;
constexpr float NMS_THRESHOLD = 0.4;
constexpr float CONFIDENCE_THRESHOLD = 0.4;

#endif  // YOLOV5_CONFIG_H
