/*---------------------------------------------------------*\
| E131SettingsEntry.h                                       |
|                                                           |
|   User interface for OpenRGB E1.31 settings entry         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class E131SettingsEntry;
}

class E131SettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit E131SettingsEntry(QWidget *parent = nullptr);
    ~E131SettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::E131SettingsEntry *ui;

private:
    void HideMatrixSettings();
    void ShowMatrixSettings();

private slots:
    void changeEvent(QEvent *event) override;
    void on_TypeComboBox_currentIndexChanged(int index);
};
