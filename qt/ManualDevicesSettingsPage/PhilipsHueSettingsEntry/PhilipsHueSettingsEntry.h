/*---------------------------------------------------------*\
| OpenRGBPhilipsHueSettingsEntry.h                          |
|                                                           |
|   User interface for OpenRGB Philips Hue settings entry   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class OpenRGBPhilipsHueSettingsEntry;
}

class OpenRGBPhilipsHueSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit OpenRGBPhilipsHueSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBPhilipsHueSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBPhilipsHueSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event);
    void on_UnpairButton_clicked();
};
