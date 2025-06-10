/*---------------------------------------------------------*\
| OpenRGBPhilipsWizSettingsEntry.h                          |
|                                                           |
|   User interface for OpenRGB Philips Wiz settings entry   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class OpenRGBPhilipsWizSettingsEntry;
    class OpenRGBPhilipsWizSettingsEntryUi;
}

class Ui::OpenRGBPhilipsWizSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit OpenRGBPhilipsWizSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBPhilipsWizSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBPhilipsWizSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
