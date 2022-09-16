#ifndef FACE_TOUCHING_DETECTOR_MAIN_WINDOW_H
#define FACE_TOUCHING_DETECTOR_MAIN_WINDOW_H

#include <QMainWindow>

#include "detector_qt.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

   private slots:
    void on_ocvButton_clicked();

   private:
    bool isRunning_ = false;

    std::unique_ptr<Ui::MainWindow> ui;

    std::unique_ptr<detectorQT> cap_;
};

#endif  // FACE_TOUCHING_DETECTOR_MAIN_WINDOW_H
