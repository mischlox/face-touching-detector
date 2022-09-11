#ifndef FACE_TOUCHING_DETECTOR_MAIN_WINDOW_H
#define FACE_TOUCHING_DETECTOR_MAIN_WINDOW_H

#include <QMainWindow>

#include "ft_detector_capture.h"

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
    Ui::MainWindow *ui;
    FTdetectorCapture *cap_;
};

#endif  // FACE_TOUCHING_DETECTOR_MAIN_WINDOW_H
