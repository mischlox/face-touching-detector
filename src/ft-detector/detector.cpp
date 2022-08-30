#include <ft-detector/detector.h>
#include <spdlog/spdlog.h>

void Detector::readLabels(const std::string &labelFile) {
    std::ifstream in(labelFile);
    if (!in) {
        spdlog::error("Labels could not be loaded from file {}", labelFile);
    }

    std::string line;
    while (getline(in, line)) {
        labels_.emplace_back(line);
    }
}

void Detector::detectVideo(const std::string &videoPath, bool show) {
    cv::VideoCapture cap;
    cap.open(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Error opening video file\n";
        return;
    }

    while (true) {
        cv::Mat frame;
        std::vector<Detection> detections;

        cap >> frame;
        if (!frame.empty()) {
            detect(frame, detections);

            if (show) {
                drawBoxes(frame, detections);
                cv::imshow("output", frame);
            }
        }
        if (cv::waitKey(1) != -1) {
            cap.release();
            std::cout << "finished by user\n";
            break;
        }
    }
}

void Detector::detectImage(const std::string &imgPath, bool show) {}

void Detector::drawBoxes(cv::Mat &img, const std::vector<Detection> &detections) {
    const std::vector<cv::Scalar> colors = {cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 0),
                                            cv::Scalar(0, 255, 255), cv::Scalar(255, 0, 0)};

    for (const auto &detection : detections) {
        auto box = detection.box;
        auto classId = detection.classID;
        const auto color = colors[classId % colors.size()];
        cv::rectangle(img, box, color, 3);

        cv::rectangle(img, cv::Point(box.x, box.y - 20), cv::Point(box.x + box.width, box.y), color,
                      cv::FILLED);
        cv::putText(img, labels_[classId], cv::Point(box.x, box.y - 5), cv::FONT_HERSHEY_SIMPLEX,
                    0.5, cv::Scalar(0, 0, 0));
    }
}

bool Detector::boxesOverlap(const std::vector<Detection> &detections) {
    std::vector<Detection> handDets, faceDets;
    for (int i = 0; i < detections.size(); ++i) {
        for (int j = i + 1; j < detections.size(); ++j) {
            auto detA = detections[i];
            auto detB = detections[j];
            if (detA.box != detB.box) {
                if ((detA.classID == 1 && detB.classID == 2) ||
                    (detA.classID == 2 && detB.classID == 1)) {
                    int overlap = (detA.box & detB.box).area();
                    spdlog::info("Area of overlap: {}", overlap);
                    return overlap > 0;
                }
            }
        }
    }
    return false;
}
