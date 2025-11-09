/*---------------------------------------------------------*\
| OpenRGBDevicePage.h                                       |
|                                                           |
|   User interface for OpenRGB device page                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    /*-----------------------------------------------------*\
    | Device Functions                                      |
    \*-----------------------------------------------------*/
    RGBController* GetController();
    void SetCustomMode(unsigned char red, unsigned char green, unsigned char blue);

    /*-----------------------------------------------------*\
    | Device View Functions                                 |
    \*-----------------------------------------------------*/
    void HideDeviceView();
    void ShowDeviceView();

private:
    /*-----------------------------------------------------*\
    | UI Pointer                                            |
    \*-----------------------------------------------------*/
    Ui::OpenRGBDevicePage *ui;

    /*-----------------------------------------------------*\
    | Device pointer                                        |
    \*-----------------------------------------------------*/
    RGBController *device;

    /*-----------------------------------------------------*\
    | Current color                                         |
    \*-----------------------------------------------------*/
    QColor current_color;

    /*-----------------------------------------------------*\
    | Flags                                                 |
    \*-----------------------------------------------------*/
    bool DeviceViewShowing  = false;
    bool HexFormatRGB       = true;
    bool InvertedBrightness = false;
    bool InvertedSpeed      = false;
    bool MultipleSelected   = false;
    bool UpdateHex          = true;

    /*-----------------------------------------------------*\
    | UI Update Functions                                   |
    \*-----------------------------------------------------*/
    void UpdateColor();
    void UpdateColorUi();
    void UpdateDevice();
    void UpdateLEDList();
    void UpdateLEDUi();
    void UpdateMode();
    void UpdateModeList();
    void UpdateModeUi();
    void UpdateZoneList();

    /*-----------------------------------------------------*\
    | Helper functions                                      |
    \*-----------------------------------------------------*/
    bool AutoUpdateEnabled();
    void ColorChanged();
    void GetSelectedMode(bool * selected_zone_mode, int * selected_mode);
    void GetSelectedZone(bool * selected_all_zones, int * selected_zone, int * selected_segment);
    QString ModeDescription(const std::string mode_name);

private slots:
    /*-----------------------------------------------------*\
    | Callback handler                                      |
    \*-----------------------------------------------------*/
    void UpdateInterface(unsigned int update_reason);

    /*-----------------------------------------------------*\
    | Retranslate Event Slot                                |
    \*-----------------------------------------------------*/
    void changeEvent(QEvent *event);

    /*-----------------------------------------------------*\
    | UI Control Slots                                      |
    \*-----------------------------------------------------*/
    void on_ApplyColorsButton_clicked();
    void on_BlueSpinBox_valueChanged(int blue);
    void on_BrightnessSlider_valueChanged(int value);
    void on_ColorWheelBox_colorChanged(const QColor color);
    void on_DeviceSaveButton_clicked();
    void on_DeviceViewBox_selectionChanged(QVector<int>);
    void on_DirectionBox_currentIndexChanged(int index);
    void on_EditZoneButton_clicked();
    void on_GreenSpinBox_valueChanged(int green);
    void on_HexLineEdit_textChanged(const QString &arg1);
    void on_HueSpinBox_valueChanged(int hue);
    void on_LEDBox_currentIndexChanged(int index);
    void on_ModeBox_currentIndexChanged(int index);
    void on_ModeSpecificCheck_clicked();
    void on_PerLEDCheck_clicked();
    void on_RandomCheck_clicked();
    void on_RedSpinBox_valueChanged(int red);
    void on_SatSpinBox_valueChanged(int sat);
    void on_SelectAllLEDsButton_clicked();
    void on_SetAllButton_clicked();
    void on_SpeedSlider_valueChanged(int value);
    void on_SpinBoxModeColors_valueChanged(int arg1);
    void on_SwatchBox_swatchChanged(const QColor color);
    void on_ValSpinBox_valueChanged(int val);
    void on_ZoneBox_currentIndexChanged(int index);

signals:
    void RefreshList();
    void SetAllDevices(unsigned char red, unsigned char green, unsigned char blue);
    void SaveSizeProfile();
};
