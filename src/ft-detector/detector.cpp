#include <ft-detector/detector.h>
#include <spdlog/spdlog.h>

Detector::Detector() { notifier_->registerListener(std::make_shared<IntersectListenerStdout>()); }

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
                if (cv::waitKey(5) >= 0) {
                    cap.release();
                    std::cout << "finished by user\n";
                    break;
                }
            }
        }
        if (boxesOverlap(detections)) {
            notifier_->notifyAll();
        }
    }
}

void Detector::detectImage(const std::string &imgPath, bool show) {
    cv::Mat img = cv::imread(imgPath);
    std::vector<Detection> detections;
    detect(img, detections);
    if (show) {
        drawBoxes(img, detections);
        cv::imshow("output", img);
        cv::waitKey(0);
    }
}

void Detector::drawBoxes(cv::Mat &img, const std::vector<Detection> &detections) {
    const std::vector<cv::Scalar> colors = {cv::Scalar(0, 0, 255),      // Hand
                                            cv::Scalar(255, 255, 255),  // Person
                                            cv::Scalar(0, 255, 0)};     // Face

    for (const auto &detection : detections) {
        auto box = detection.box;
        auto classId = detection.classID;

        // convert confidence to string with 2 decimals
        std::stringstream confStream;
        confStream << std::fixed << std::setprecision(2) << detection.confidence;
        auto confidence = confStream.str();

        const auto color = colors[classId % colors.size()];
        cv::rectangle(img, box, color, 3);

        cv::rectangle(img, cv::Point(box.x, box.y - 20), cv::Point(box.x + box.width, box.y), color,
                      cv::FILLED);
        // Left aligned class name
        cv::putText(img, labels_[classId], cv::Point(box.x, box.y - 5), cv::FONT_HERSHEY_SIMPLEX,
                    0.5, cv::Scalar(0, 0, 0));
        // Right aligned confidence
        cv::putText(img, confidence, cv::Point(box.x + box.width - 40, box.y - 5),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
    }
}

bool Detector::boxesOverlap(const std::vector<Detection> &detections) {
    for (auto &detectionA : detections) {
        // If class ID of detection A is face, then iterate again over all detections
        if (detectionA.classID == 2) {
            for (auto &detectionB : detections) {
                // If class ID of detection B is hand and detection A overlaps with detection B then
                // return true
                if (detectionB.classID == 3 && ((detectionA.box & detectionB.box).area() > 0)) {
                    return true;
                }
            }
        }
    }
    return false;
}
