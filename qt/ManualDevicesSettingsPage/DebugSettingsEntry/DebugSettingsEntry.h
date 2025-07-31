/*---------------------------------------------------------*\
| DebugSettingsEntry.h                                      |
|                                                           |
|   User interface for OpenRGB Debug settings entry         |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      30 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class DebugSettingsEntry;
}

class DebugSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit DebugSettingsEntry(QWidget *parent = nullptr);
    ~DebugSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::DebugSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event) override;
};
