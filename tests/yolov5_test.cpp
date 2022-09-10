#include <ft-detector/detectors.h>
#include <gtest/gtest.h>

class DetectorTest : public ::testing::Test {
   protected:
    std::unique_ptr<Detector> detector;
    std::vector<Detection> detections;

    void SetUp() override {
        detector = Detectors::yolov5("models/detector_cpu.torchscript", "labels/classes.txt");
    }

    void TearDown() override {}
};

TEST_F(DetectorTest, readNet_invalidFile_throwsRuntimeError) {}

TEST_F(DetectorTest, boxesOverlap_doOverlap_returnsTrue) {
    Detection det1{1, 1.0, cv::Rect(1, 1, 5, 3)};
    Detection det2{2, 1.0, cv::Rect(0, 0, 3, 3)};
    detections.emplace_back(det1);
    detections.emplace_back(det2);

    ASSERT_TRUE(detector->boxesOverlap(detections));
};

TEST_F(DetectorTest, boxesOverlap_doNotOverlap_returnsFalse) {
    Detection det1{1, 1.0, cv::Rect(5, 5, 1, 1)};
    Detection det2{2, 1.0, cv::Rect(0, 0, 1, 1)};
    detections.emplace_back(det1);
    detections.emplace_back(det2);

    ASSERT_FALSE(detector->boxesOverlap(detections));
};

TEST_F(DetectorTest, boxesOverlap_sameClass_returnsFalse) {
    Detection det1{1, 1.0, cv::Rect(1, 1, 5, 3)};
    Detection det2{1, 1.0, cv::Rect(0, 0, 3, 3)};
    detections.emplace_back(det1);
    detections.emplace_back(det2);

    ASSERT_FALSE(detector->boxesOverlap(detections));
};

TEST_F(DetectorTest, readLabels_txtFile_correct) {
    std::vector<std::string> testVector{"Background", "Person", "Face", "Hand"};
    detector->readLabels("labels/classes.txt");
    ASSERT_EQ(testVector, detector->getLabels());
};

TEST_F(DetectorTest, readModel_noError) {
    ASSERT_NO_FATAL_FAILURE(detector->loadModel("models/detector_cpu.torchscript"));
};

TEST_F(DetectorTest, runDetect_noError) {
    ASSERT_NO_FATAL_FAILURE(detector->detectVideo("/dev/video0"));
}