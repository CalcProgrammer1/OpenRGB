/*---------------------------------------------------------*\
| OpenRGBE131SettingsEntry.h                                |
|                                                           |
|   User interface for OpenRGB E1.31 settings entry         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class OpenRGBE131SettingsEntry;
}

class OpenRGBE131SettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit OpenRGBE131SettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBE131SettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBE131SettingsEntry *ui;

private:
    void HideMatrixSettings();
    void ShowMatrixSettings();

private slots:
    void changeEvent(QEvent *event);
    void on_TypeComboBox_currentIndexChanged(int index);
};
