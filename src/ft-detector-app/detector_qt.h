#ifndef DETECTOR_QT_H
#define DETECTOR_QT_H
#include <ft-detector/detectors.h>

#include <QImage>
#include <QPixmap>
#include <QThread>

#define ID_CAM 0

/**
 * Thread that updates FPS Label in a predefined interval
 */
class FPSEmitter : public QThread {
    Q_OBJECT
   public:
    explicit FPSEmitter(QObject* parent = nullptr);
    void setFPS(float fps) { fps_ = fps; }
    float getFPS() const { return fps_; }

   signals:
    void updateFPS();

   protected:
    void run() override;
    float fps_;
    bool isRunning_ = true;
};

/**
 * QT compatible wrapper for ft-detector
 */
class DetectorQT : public QThread {
    Q_OBJECT
   public:
    explicit DetectorQT(QObject* parent = nullptr);
    ~DetectorQT() override;

    QPixmap pixmap() const { return pixmap_; }
    cv::Mat frame() const { return frame_; }

    std::shared_ptr<Detector> detector() { return detector_; };
    std::shared_ptr<FPSEmitter> fpsEmitter() { return fpsEmitter_; }

   signals:
    void newPixMapCaptured();
    void boxesOverlap();
    void boxesDoNotOverlap();

   protected:
    void run() override;

   private:
    std::shared_ptr<FPSEmitter> fpsEmitter_;
    std::shared_ptr<Detector> detector_;
    QPixmap pixmap_;
    cv::VideoCapture videoCap_;
    cv::Mat frame_;

    QImage cvMatToQImage(const cv::Mat& inMat);
    QPixmap cvMatToQPixmap(const cv::Mat& inMat);
};

#endif  // DETECTOR_QT_H
