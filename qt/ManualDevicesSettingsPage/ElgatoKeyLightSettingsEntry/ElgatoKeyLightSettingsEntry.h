/*---------------------------------------------------------*\
| ElgatoKeyLightSettingsEntry.h                             |
|                                                           |
|   User interface for OpenRGB Elgato Key Light entry       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class ElgatoKeyLightSettingsEntry;
}

class ElgatoKeyLightSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit ElgatoKeyLightSettingsEntry(QWidget *parent = nullptr);
    ~ElgatoKeyLightSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::ElgatoKeyLightSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event) override;
};
