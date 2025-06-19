/*---------------------------------------------------------*\
| OpenRGBYeelightSettingsEntry.h                            |
|                                                           |
|   User interface for Yeelight settings entry              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class OpenRGBYeelightSettingsEntry;
}

class OpenRGBYeelightSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit OpenRGBYeelightSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBYeelightSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBYeelightSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event);
    void on_HostIPChooserButton_clicked();
};
