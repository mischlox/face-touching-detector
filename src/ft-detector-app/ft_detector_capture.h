#ifndef FACE_TOUCHING_DETECTOR_FT_DETECTOR_CAPTURE_H
#define FACE_TOUCHING_DETECTOR_FT_DETECTOR_CAPTURE_H
#include <ft-detector/detectors.h>

#include <QImage>
#include <QPixmap>
#include <QThread>

#define ID_CAM 0

/**
 * QT compatible wrapper for ft-detector
 */
class FTdetectorCapture : public QThread {
    Q_OBJECT
   public:
    explicit FTdetectorCapture(QObject* parent = nullptr);
    QPixmap pixmap() const { return pixmap_; }
    cv::Mat frame() const { return frame_; }
   signals:
    void newPixMapCaptured();

   protected:
    void run() override;

   private:
    QPixmap pixmap_;
    cv::VideoCapture videoCap_;
    cv::Mat frame_;

    QImage cvMatToQImage(const cv::Mat& inMat);
    QPixmap cvMatToQPixmap(const cv::Mat& inMat);
};

#endif  // FACE_TOUCHING_DETECTOR_FT_DETECTOR_CAPTURE_H
