/*---------------------------------------------------------*\
| PhilipsWizSettingsEntry.h                                 |
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
    class PhilipsWizSettingsEntry;
}

class PhilipsWizSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit PhilipsWizSettingsEntry(QWidget *parent = nullptr);
    ~PhilipsWizSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::PhilipsWizSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event) override;
};
