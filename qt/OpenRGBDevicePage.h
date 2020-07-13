#ifndef OPENRGBDEVICEPAGE_H
#define OPENRGBDEVICEPAGE_H

#include "ui_OpenRGBDevicePage.h"
#include "RGBController.h"

#include <QFrame>

namespace Ui {
class OpenRGBDevicePage;
}

class Ui::OpenRGBDevicePage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBDevicePage(RGBController *dev, QWidget *parent = nullptr);
    ~OpenRGBDevicePage();

    void SetDevice(unsigned char red, unsigned char green, unsigned char blue);
    void SetCustomMode(unsigned char red, unsigned char green, unsigned char blue);
    void UpdateDevice();
    void UpdateMode();
    void UpdateModeUi();

private slots:
    void on_ButtonRed_clicked();
    void on_ButtonYellow_clicked();
    void on_ButtonGreen_clicked();
    void on_ButtonCyan_clicked();
    void on_ButtonBlue_clicked();
    void on_ButtonMagenta_clicked();
    void on_ZoneBox_currentIndexChanged(int index);
    void on_LEDBox_currentIndexChanged(int index);
    void on_ModeBox_currentIndexChanged(int index);
    void on_SetDeviceButton_clicked();
    void on_SetZoneButton_clicked();
    void on_SetLEDButton_clicked();
    void on_RedSpinBox_valueChanged(int arg1);
    void on_HueSpinBox_valueChanged(int arg1);
    void on_GreenSpinBox_valueChanged(int arg1);
    void on_SatSpinBox_valueChanged(int arg1);
    void on_BlueSpinBox_valueChanged(int arg1);
    void on_ValSpinBox_valueChanged(int arg1);

    void on_SetAllButton_clicked();

    void on_RandomCheck_clicked();

    void on_SpeedSlider_valueChanged(int value);

    void on_DirectionBox_currentIndexChanged(int index);

    void on_PerLEDCheck_clicked();

    void on_ModeSpecificCheck_clicked();

    void on_ResizeButton_clicked();

    void on_ColorWheelBox_colorChanged(const QColor color);

    void UpdateInterface();

private:
    Ui::OpenRGBDevicePageUi *ui;
    RGBController *device;

    bool UpdatingColor = false;
    bool InvertedSpeed = false;

    void updateRGB();

    void updateHSV();

    void updateWheel();

signals:
    void SetAllDevices(unsigned char red, unsigned char green, unsigned char blue);
    void SaveSizeProfile();
};

#endif // OPENRGBDEVICEPAGE_H
