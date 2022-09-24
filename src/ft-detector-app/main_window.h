#ifndef FACE_TOUCHING_DETECTOR_MAIN_WINDOW_H
#define FACE_TOUCHING_DETECTOR_MAIN_WINDOW_H
#include <QMainWindow>
#include <QtMultimedia/QSoundEffect>

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
    void on_sliderVolume_valueChanged(int value);
    void on_sliderNMS_valueChanged(int value);
    void on_sliderConfidence_valueChanged(int value);

   private:
    const QString STYLE_LABEL =
        "QLabel { "
        "border: 2px solid rgb(113, 113, 113);"
        "border-width: 2px;"
        "border-radius: 10px;";
    const QString STYLE_PLAY_BUTTON =
        "#ocvButton {"
        "background-color: transparent;"
        "background: none;"
        "border: none;"
        "background-repeat: none;";

    const QString BUTTON_PLAY = STYLE_PLAY_BUTTON + "border-image: url(:/images/play.png)};";
    const QString BUTTON_PAUSE = STYLE_PLAY_BUTTON + "border-image: url(:/images/pause.png)};";
    const QString LABEL_RED = STYLE_LABEL + "background-color: rgb(179, 0, 0);}";
    const QString LABEL_GREEN = STYLE_LABEL + "background-color: rgb(5, 123, 16);}";

    std::unique_ptr<QSoundEffect> soundBeep_ = std::make_unique<QSoundEffect>(this);
    float volume_ = 0.5;

    bool isRunning_ = false;

    std::unique_ptr<Ui::MainWindow> ui;

    std::unique_ptr<detectorQT> cap_ = std::make_unique<detectorQT>(this);
    ;
};

#endif  // FACE_TOUCHING_DETECTOR_MAIN_WINDOW_H
