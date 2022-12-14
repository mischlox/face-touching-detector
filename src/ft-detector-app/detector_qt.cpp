#include "detector_qt.h"

#include <ft-detector/utils.h>

#include <QDebug>

DetectorQT::DetectorQT(QObject *parent) : QThread(parent), videoCap_(ID_CAM) {
    detector_ = Detectors::yolov5();
    fpsEmitter_ = std::make_shared<FPSEmitter>(parent);
}

void DetectorQT::run() {
    Timer clock;
    fpsEmitter_->start();
    while (videoCap_.isOpened()) {
        clock.tick();
        std::vector<Detection> detections;

        videoCap_ >> frame_;
        if (!frame_.empty()) {
            detector_->detect(frame_, detections);
            detector_->drawBoxes(frame_, detections);
            pixmap_ = cvMatToQPixmap(frame_);
            emit newPixMapCaptured();
        }
        if (detector_->boxesOverlap(detections)) {
            emit boxesOverlap();
        } else {
            emit boxesDoNotOverlap();
        }
        clock.tock();
        fpsEmitter_->setFPS(1000.0f / clock.elapsedTime());
    }
}

QImage DetectorQT::cvMatToQImage(const cv::Mat &inMat) {
    switch (inMat.type()) {
        // 8-bit, 4 channel
        case CV_8UC4: {
            QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step),
                         QImage::Format_ARGB32);

            return image;
        }

            // 8-bit, 3 channel
        case CV_8UC3: {
            QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step),
                         QImage::Format_RGB888);

            return image.rgbSwapped();
        }

            // 8-bit, 1 channel
        case CV_8UC1: {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
            QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step),
                         QImage::Format_Grayscale8);
#else
            static QVector<QRgb> sColorTable;

            // only create our color table the first time
            if (sColorTable.isEmpty()) {
                sColorTable.resize(256);

                for (int i = 0; i < 256; ++i) {
                    sColorTable[i] = qRgb(i, i, i);
                }
            }

            QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step),
                         QImage::Format_Indexed8);

            image.setColorTable(sColorTable);
#endif

            return image;
        }

        default:
            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:"
                       << inMat.type();
            break;
    }

    return QImage();
}

QPixmap DetectorQT::cvMatToQPixmap(const cv::Mat &inMat) {
    return QPixmap::fromImage(cvMatToQImage(inMat));
}

DetectorQT::~DetectorQT() { fpsEmitter_->terminate(); }

FPSEmitter::FPSEmitter(QObject *parent) : QThread(parent) {}

void FPSEmitter::run() {
    while (isRunning_) {
        emit updateFPS();
        QThread::sleep(1);
    }
}
