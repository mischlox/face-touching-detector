#include "main_window.h"

#include "ui_main_window.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    cap_ = new FTdetectorCapture(this);

    connect(cap_, &FTdetectorCapture::newPixMapCaptured, this, [&]() {
        ui->frame->setPixmap(cap_->pixmap().scaled(cap_->frame().cols, cap_->frame().rows));
    });
}

MainWindow::~MainWindow() {
    delete ui;
    cap_->terminate();
}

void MainWindow::on_ocvButton_clicked() { cap_->start(QThread::HighestPriority); }
