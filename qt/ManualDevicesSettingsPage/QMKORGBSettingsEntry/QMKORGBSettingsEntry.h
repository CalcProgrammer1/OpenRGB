/*---------------------------------------------------------*\
| QMKORGBSettingsEntry.h                             |
|                                                           |
|   User interface entry for OpenRGB QMK configuration      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class QMKORGBSettingsEntry;
}

class QMKORGBSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

private slots:
    void changeEvent(QEvent *event) override;

public:
    explicit QMKORGBSettingsEntry(QWidget *parent = nullptr);
    ~QMKORGBSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::QMKORGBSettingsEntry *ui;
};
