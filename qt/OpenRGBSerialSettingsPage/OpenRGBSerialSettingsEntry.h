/*---------------------------------------------------------*\
| OpenRGBSerialSettingsEntry.h                              |
|                                                           |
|   User interface entry for serial device configuration    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class OpenRGBSerialSettingsEntry;
    class OpenRGBSerialSettingsEntryUi;
}

class Ui::OpenRGBSerialSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

private slots:
    void changeEvent(QEvent *event);

    void on_ProtocolComboBox_currentIndexChanged(int index);

public:
    explicit OpenRGBSerialSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBSerialSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBSerialSettingsEntryUi *ui;
};
