/*---------------------------------------------------------*\
| PhilipsHueSettingsEntry.h                                 |
|                                                           |
|   User interface for OpenRGB Philips Hue settings entry   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class PhilipsHueSettingsEntry;
}

class PhilipsHueSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit PhilipsHueSettingsEntry(QWidget *parent = nullptr);
    ~PhilipsHueSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::PhilipsHueSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event) override;
    void on_UnpairButton_clicked();
};
