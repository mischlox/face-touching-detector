#ifndef FACE_TOUCHING_DETECTOR_DETECTORS
#define FACE_TOUCHING_DETECTOR_DETECTORS
#include <ft-detector/yolov5.h>

/**
 * Detector Factory
 */
class Detectors {
   public:
    static std::unique_ptr<Detector> yolov5(const std::string &modelPath,
                                            const std::string &classListPath) {
        return std::make_unique<YoloV5>(modelPath, classListPath);
    }
};

#endif  // FACE_TOUCHING_DETECTOR_DETECTORS
