#include "main_window.h"

#include "ui_main_window.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    cap_ = std::make_unique<FTdetectorCapture>(this);

    // clang-format off
    connect(cap_.get(),
            &FTdetectorCapture::newPixMapCaptured,
            this,
            [&]() {
                ui->frame->setPixmap(cap_->pixmap().scaled(cap_->frame().cols, cap_->frame().rows));
            });

    connect(cap_.get(),
            &FTdetectorCapture::boxesOverlap,
            this,
            [&]() {
                ui->label->setText(QString("STOP TOUCHING!!!"));
            });

    connect(cap_.get(),
            &FTdetectorCapture::boxesDoNotOverlap,
            this,
            [&]() {
                ui->label->setText(QString("Good!"));
            });
    // clang-format on
}

MainWindow::~MainWindow() { cap_->terminate(); }

void MainWindow::on_ocvButton_clicked() { cap_->start(QThread::HighestPriority); }
