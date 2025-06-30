/*---------------------------------------------------------*\
| GoveeSettingsEntry.h                                      |
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
    class GoveeSettingsEntry;
}

class GoveeSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit GoveeSettingsEntry(QWidget *parent = nullptr);
    ~GoveeSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::GoveeSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event) override;
};
