/*---------------------------------------------------------*\
| OpenRGBElgatoLightStripSettingsEntry.h                    |
|                                                           |
|   User interface for OpenRGB Elgato Light Strips entry    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class OpenRGBElgatoLightStripSettingsEntry;
}

class OpenRGBElgatoLightStripSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit OpenRGBElgatoLightStripSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBElgatoLightStripSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBElgatoLightStripSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event);
};
