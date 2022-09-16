#ifndef DETECTOR_QT_H
#define DETECTOR_QT_H
#include <ft-detector/detectors.h>

#include <QImage>
#include <QPixmap>
#include <QThread>

#define ID_CAM 0

/**
 * QT compatible wrapper for ft-detector
 */
class detectorQT : public QThread {
    Q_OBJECT
   public:
    explicit detectorQT(QObject* parent = nullptr);

    QPixmap pixmap() const { return pixmap_; }
    cv::Mat frame() const { return frame_; }
   signals:
    void newPixMapCaptured();
    void boxesOverlap();
    void boxesDoNotOverlap();

   protected:
    void run() override;

   private:
    QPixmap pixmap_;
    cv::VideoCapture videoCap_;
    cv::Mat frame_;

    QImage cvMatToQImage(const cv::Mat& inMat);
    QPixmap cvMatToQPixmap(const cv::Mat& inMat);
};

#endif  // DETECTOR_QT_H
