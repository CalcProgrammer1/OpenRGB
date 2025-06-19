/*---------------------------------------------------------*\
| OpenRGBKasaSmartSettingsEntry.h                           |
|                                                           |
|   User interface for OpenRGB Kasa Smart settings entry    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class OpenRGBKasaSmartSettingsEntry;
}

class OpenRGBKasaSmartSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit OpenRGBKasaSmartSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBKasaSmartSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();
    void setName(QString name);

private:
    Ui::OpenRGBKasaSmartSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event);
};
