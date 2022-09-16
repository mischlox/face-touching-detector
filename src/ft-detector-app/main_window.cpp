#include "main_window.h"

#include <QtMultimedia/QSound>

#include "ui_main_window.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    cap_ = std::make_unique<detectorQT>(this);

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
                QSound::play("media/sounds/beep.wav");
                ui->label->setText(QString("STOP TOUCHING!!!"));
                ui->label->setStyleSheet("QLabel {background-color: red;}");
            });

    connect(cap_.get(),
            &detectorQT::boxesDoNotOverlap,
            this,
            [&]() {
                ui->label->setText(QString("Good!"));
                ui->label->setStyleSheet("QLabel {background-color: green;}");
            });
    // clang-format on
}

MainWindow::~MainWindow() { cap_->terminate(); }

void MainWindow::on_ocvButton_clicked() {
    if (isRunning_) {
        cap_->terminate();
        isRunning_ = false;
        ui->ocvButton->setText("Start");
    } else {
        cap_->start(QThread::HighestPriority);
        isRunning_ = true;
        ui->ocvButton->setText("Stop");
    };
}
