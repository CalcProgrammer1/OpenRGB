/*---------------------------------------------------------*\
| OpenRGBElgatoKeyLightSettingsEntry.h                      |
|                                                           |
|   User interface for OpenRGB Elgato Key Light entry       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class OpenRGBElgatoKeyLightSettingsEntry;
}

class OpenRGBElgatoKeyLightSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit OpenRGBElgatoKeyLightSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBElgatoKeyLightSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBElgatoKeyLightSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event);
};
