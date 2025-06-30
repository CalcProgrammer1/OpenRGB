/*---------------------------------------------------------*\
| DMXSettingsEntry.h                                        |
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
    class DMXSettingsEntry;
}

class DMXSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit DMXSettingsEntry(QWidget *parent = nullptr);
    ~DMXSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::DMXSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event) override;
};
