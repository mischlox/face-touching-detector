#include "main_window.h"

#include "ui_main_window.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    soundBeep_->setSource(QUrl::fromLocalFile((":/sounds/beep.wav")));
    soundBeep_->setVolume(volume_);
    ui->sliderVolume->setValue(volume_ * 100);
    soundBeep_->setLoopCount(QSoundEffect::Infinite);

    // clang-format off
    connect(cap_.get(),
            &detectorQT::newPixMapCaptured,
            this,
            [&]() {
                ui->frame->setPixmap(cap_->pixmap().scaled(cap_->frame().cols, cap_->frame().rows));
            });

    connect(cap_.get(),
            &detectorQT::boxesOverlap,
            this,
            [&]() {
                if(!soundBeep_->isPlaying()) {
                    soundBeep_->play();
                }
                ui->label->setText(QString("STOP TOUCHING!!!"));
                ui->label->setStyleSheet(LABEL_RED);
            });

    connect(cap_.get(),
            &detectorQT::boxesDoNotOverlap,
            this,
            [&]() {
                soundBeep_->stop();
                ui->label->setText(QString("Good!"));
                ui->label->setStyleSheet(LABEL_GREEN);
            });
    // clang-format on
}

MainWindow::~MainWindow() { cap_->terminate(); }

void MainWindow::on_ocvButton_clicked() {
    if (isRunning_) {
        cap_->terminate();
        isRunning_ = false;
        ui->ocvButton->setStyleSheet(BUTTON_PLAY);

    } else {
        cap_->start(QThread::HighestPriority);
        isRunning_ = true;
        ui->ocvButton->setStyleSheet(BUTTON_PAUSE);
    };
}

void MainWindow::on_sliderVolume_valueChanged(int value) {
    soundBeep_->setVolume(float(value) / 100);
    ui->labelVolume->setText(QString("Volume: " + QString::number(value) + "%"));
}

void MainWindow::on_sliderConfidence_valueChanged(int value) {
    float val = float(value) / 100.0f;

    cap_->detector()->setConfThres(val);
    ui->labelConfidence->setText(QString("Confidence Threshold: " + QString::number(val)));
}

void MainWindow::on_sliderNMS_valueChanged(int value) {
    float val = float(value) / 100.0f;

    cap_->detector()->setNMSThres(float(value) / 100);
    ui->labelNMS->setText(QString("NMS Threshold: " + QString::number(val)));
}
