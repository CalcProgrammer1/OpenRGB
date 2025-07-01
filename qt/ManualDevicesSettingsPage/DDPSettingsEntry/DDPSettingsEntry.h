/*---------------------------------------------------------*\
| DDPSettingsEntry.h                                        |
|                                                           |
|   User interface for OpenRGB DDP settings entry          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class DDPSettingsEntry;
}

class DDPSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit DDPSettingsEntry(QWidget *parent = nullptr);
    ~DDPSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::DDPSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event) override;
};
