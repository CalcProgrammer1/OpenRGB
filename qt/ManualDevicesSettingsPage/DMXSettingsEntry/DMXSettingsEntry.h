/*---------------------------------------------------------*\
| OpenRGBDMXSettingsEntry.h                                 |
|                                                           |
|   User interface for OpenRGB DMX settings entry           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class OpenRGBDMXSettingsEntry;
}

class OpenRGBDMXSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit OpenRGBDMXSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBDMXSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBDMXSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event);
};
