#include <spdlog/spdlog.h>

#include "detector/detectors.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        spdlog::warn(
            "Usage: ./face-touching-detector <model-path> <class-names-path> "
            "<input-path>");
        return -1;
    }

    const std::string modelPath = argv[1];
    const std::string classNamesPath = argv[2];
    const std::string videoPath = argv[3];

    auto detector = Detectors::yolov5(modelPath, classNamesPath);

    return 0;
}
