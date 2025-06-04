/*---------------------------------------------------------*\
| OpenRGBYeelightSettingsEntry.h                            |
|                                                           |
|   User interface for Yeelight settings entry              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Ui
{
    class OpenRGBYeelightSettingsEntry;
    class OpenRGBYeelightSettingsEntryUi;
}

class Ui::OpenRGBYeelightSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBYeelightSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBYeelightSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBYeelightSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
    void on_HostIPChooserButton_clicked();
};
