/*---------------------------------------------------------*\
| ElgatoLightStripSettingsEntry.h                           |
|                                                           |
|   User interface for OpenRGB Elgato Light Strips entry    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class ElgatoLightStripSettingsEntry;
}

class ElgatoLightStripSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit ElgatoLightStripSettingsEntry(QWidget *parent = nullptr);
    ~ElgatoLightStripSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::ElgatoLightStripSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event) override;
};
