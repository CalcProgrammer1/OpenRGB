/*---------------------------------------------------------*\
| OpenRGBQMKORGBSettingsEntry.h                             |
|                                                           |
|   User interface entry for OpenRGB QMK configuration      |
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
    class OpenRGBQMKORGBSettingsEntry;
    class OpenRGBQMKORGBSettingsEntryUi;
}

class Ui::OpenRGBQMKORGBSettingsEntry : public QWidget
{
    Q_OBJECT

private slots:
    void changeEvent(QEvent *event);

public:
    explicit OpenRGBQMKORGBSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBQMKORGBSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBQMKORGBSettingsEntryUi *ui;
};
