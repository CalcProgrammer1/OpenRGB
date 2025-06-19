/*---------------------------------------------------------*\
| OpenRGBQMKORGBSettingsEntry.h                             |
|                                                           |
|   User interface entry for OpenRGB QMK configuration      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class OpenRGBQMKORGBSettingsEntry;
}

class OpenRGBQMKORGBSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

private slots:
    void changeEvent(QEvent *event);

public:
    explicit OpenRGBQMKORGBSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBQMKORGBSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBQMKORGBSettingsEntry *ui;
};
