/*---------------------------------------------------------*\
| OpenRGBGoveeSettingsEntry.h                               |
|                                                           |
|   User interface for OpenRGB Govee settings entry         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class OpenRGBGoveeSettingsEntry;
    class OpenRGBGoveeSettingsEntryUi;
}

class Ui::OpenRGBGoveeSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit OpenRGBGoveeSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBGoveeSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBGoveeSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
