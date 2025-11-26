/*---------------------------------------------------------*\
| QMKVialRGBSettingsEntry.h                                 |
|                                                           |
|   User interface entry for VialRGB QMK configuration      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class QMKVialRGBSettingsEntry;
}

class QMKVialRGBSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

private slots:
    void changeEvent(QEvent *event) override;

public:
    explicit QMKVialRGBSettingsEntry(QWidget *parent = nullptr);
    ~QMKVialRGBSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::QMKVialRGBSettingsEntry *ui;
};
