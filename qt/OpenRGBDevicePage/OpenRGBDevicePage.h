/*---------------------------------------------------------*\
| OpenRGBDevicePage.h                                       |
|                                                           |
|   User interface for OpenRGB device page                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QFrame>
#include "RGBController.h"

namespace Ui
{
    class OpenRGBDevicePage;
}

class OpenRGBDevicePage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBDevicePage(RGBController *dev, QWidget *parent = nullptr);
    ~OpenRGBDevicePage();

    RGBController* GetController();

    void SetDevice(unsigned char red, unsigned char green, unsigned char blue); // Could be moved to private
    void SetCustomMode(unsigned char red, unsigned char green, unsigned char blue);
    void UpdateDevice();
    void UpdateMode();
    void UpdateModeUi();
    void ShowDeviceView();
    void HideDeviceView();

private slots:
    void changeEvent(QEvent *event);
    void UpdateInterface();

    void on_ColorWheelBox_colorChanged(const QColor color);
    void on_SwatchBox_swatchChanged(const QColor color);
    void on_DirectionBox_currentIndexChanged(int index);
    void on_ZoneBox_currentIndexChanged(int index);
    void on_LEDBox_currentIndexChanged(int index);
    void on_BrightnessSlider_valueChanged(int value);
    void on_ModeBox_currentIndexChanged(int index);
    void on_SpeedSlider_valueChanged(int value);
    void on_RedSpinBox_valueChanged(int red);
    void on_HueSpinBox_valueChanged(int hue);
    void on_GreenSpinBox_valueChanged(int green);
    void on_SatSpinBox_valueChanged(int sat);
    void on_BlueSpinBox_valueChanged(int blue);
    void on_ValSpinBox_valueChanged(int val);
    void on_HexLineEdit_textChanged(const QString &arg1);
    void on_DeviceViewBox_selectionChanged(QVector<int>);

    void on_SetAllButton_clicked();
    void on_RandomCheck_clicked();
    void on_PerLEDCheck_clicked();
    void on_ModeSpecificCheck_clicked();
    void on_EditZoneButton_clicked();

    void on_ApplyColorsButton_clicked();

    void on_SelectAllLEDsButton_clicked();

    void on_DeviceSaveButton_clicked();

private:
    Ui::OpenRGBDevicePage *ui;
    RGBController *device;

    bool InvertedSpeed      = false;
    bool InvertedBrightness = false;
    bool MultipleSelected   = false;
    bool DeviceViewShowing  = false;
    bool UpdateHex          = true;
    bool HexFormatRGB       = true;

    QColor current_color;
    void updateColorUi();
    void colorChanged();

    bool autoUpdateEnabled();

    QString ModeDescription(const mode& m);

signals:
    void SetAllDevices(unsigned char red, unsigned char green, unsigned char blue);
    void SaveSizeProfile();
};
