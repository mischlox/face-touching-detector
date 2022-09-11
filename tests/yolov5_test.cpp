#include <ft-detector/detectors.h>
#include <gtest/gtest.h>

class DetectorTestGPU : public ::testing::Test {
   protected:
    std::unique_ptr<Detector> detector;
    std::vector<Detection> detections;

    void SetUp() override {
        detector = Detectors::yolov5("models/detector_gpu.torchscript", "labels/classes.txt");
    }

    void TearDown() override {}
};

class DetectorTestCPU : public ::testing::Test {
   protected:
    std::unique_ptr<Detector> detector;
    std::vector<Detection> detections;

    void SetUp() override {
        detector =
            Detectors::yolov5("models/detector_cpu.torchscript", "labels/classes.txt", false);
    }

    void TearDown() override {}
};

TEST_F(DetectorTestGPU, readNet_invalidFile_throwsRuntimeError) {}

TEST_F(DetectorTestGPU, boxesOverlap_doOverlap_returnsTrue) {
    Detection det1{1, 1.0, cv::Rect(1, 1, 5, 3)};
    Detection det2{2, 1.0, cv::Rect(0, 0, 3, 3)};
    detections.emplace_back(det1);
    detections.emplace_back(det2);

    ASSERT_TRUE(detector->boxesOverlap(detections));
};

TEST_F(DetectorTestGPU, boxesOverlap_doNotOverlap_returnsFalse) {
    Detection det1{1, 1.0, cv::Rect(5, 5, 1, 1)};
    Detection det2{2, 1.0, cv::Rect(0, 0, 1, 1)};
    detections.emplace_back(det1);
    detections.emplace_back(det2);

    ASSERT_FALSE(detector->boxesOverlap(detections));
};

TEST_F(DetectorTestGPU, boxesOverlap_sameClass_returnsFalse) {
    Detection det1{1, 1.0, cv::Rect(1, 1, 5, 3)};
    Detection det2{1, 1.0, cv::Rect(0, 0, 3, 3)};
    detections.emplace_back(det1);
    detections.emplace_back(det2);

    ASSERT_FALSE(detector->boxesOverlap(detections));
};

TEST_F(DetectorTestGPU, readLabels_txtFile_correct) {
    std::vector<std::string> testVector{"Background", "Person", "Face", "Hand"};
    ASSERT_EQ(testVector, detector->getLabels());
};

TEST_F(DetectorTestGPU, readModel_noError) {
    ASSERT_NO_FATAL_FAILURE(detector->loadModel("models/detector_cpu.torchscript"));
};

TEST_F(DetectorTestGPU, runDetectWithGPU_noError) {
    ASSERT_NO_FATAL_FAILURE(detector->detectImage("videos/sample2.jpg", false));
}

TEST_F(DetectorTestCPU, runDetectWithCPU_noError) {
    ASSERT_NO_FATAL_FAILURE(detector->detectImage("videos/sample2.jpg", false));
}

TEST_F(DetectorTestGPU, runDetectWithGPU_notify_noError) {
    ASSERT_NO_FATAL_FAILURE(detector->detectVideo("/dev/video0"));
}