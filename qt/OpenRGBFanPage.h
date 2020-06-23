#ifndef OPENRGBFANPAGE_H
#define OPENRGBFANPAGE_H

#include "ui_OpenRGBFanPage.h"
#include "FanController.h"

#include <QFrame>
#include <thread>

namespace Ui {
class OpenRGBFanPage;
}

class Ui::OpenRGBFanPage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBFanPage(FanController *dev, QWidget *parent = nullptr);
    ~OpenRGBFanPage();
    
    void FanUpdateThreadFunction();
    void SetDevice(unsigned char red, unsigned char green, unsigned char blue);
    void SetCustomMode();
    void UpdateDevice();
    void UpdateMode();
    void UpdateModeUi();

private slots:
    void UpdateRPM();
    void on_FanBox_currentIndexChanged(int index);
    void on_ModeBox_currentIndexChanged(int index);
    void on_SpeedSlider_valueChanged(int value);

private:
    Ui::OpenRGBFanPageUi *ui;
    FanController *device;

    std::thread* FanUpdateThread;

signals:
};

#endif // OPENRGBDEVICEPAGE_H
