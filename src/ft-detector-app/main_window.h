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
    const QString LABEL_STYLE =
        "QLabel { "
        "border: 2px solid rgb(113, 113, 113);"
        "border-width: 2px;"
        "border-radius: 10px;";

    const QString LABEL_RED = LABEL_STYLE + "background-color: rgb(179, 0, 0);}";
    const QString LABEL_GREEN = LABEL_STYLE + "background-color: rgb(5, 123, 16);}";

    bool isRunning_ = false;

    std::unique_ptr<Ui::MainWindow> ui;

    std::unique_ptr<detectorQT> cap_;
};

#endif  // FACE_TOUCHING_DETECTOR_MAIN_WINDOW_H
