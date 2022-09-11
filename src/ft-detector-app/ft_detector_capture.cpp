#include "ft_detector_capture.h"

#include <QDebug>

FTdetectorCapture::FTdetectorCapture(QObject *parent) : QThread(parent), videoCap_(ID_CAM) {}

void FTdetectorCapture::run() {
    auto detector = Detectors::yolov5("models/detector_gpu.torchscript", "labels/classes.txt");
    if (videoCap_.isOpened()) {
        while (true) {
            std::vector<Detection> detections;

            videoCap_ >> frame_;
            if (!frame_.empty()) {
                detector->detect(frame_, detections);
                detector->drawBoxes(frame_, detections);
                pixmap_ = cvMatToQPixmap(frame_);
                emit newPixMapCaptured();
            }
            if (detector->boxesOverlap(detections)) {
                emit boxesOverlap();
            } else {
                emit boxesDoNotOverlap();
            }
        }
    }
}

QImage FTdetectorCapture::cvMatToQImage(const cv::Mat &inMat) {
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

QPixmap FTdetectorCapture::cvMatToQPixmap(const cv::Mat &inMat) {
    return QPixmap::fromImage(cvMatToQImage(inMat));
}
