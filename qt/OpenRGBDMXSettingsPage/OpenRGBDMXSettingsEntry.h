/*---------------------------------------------------------*\
| OpenRGBDMXSettingsEntry.h                                 |
|                                                           |
|   User interface for OpenRGB DMX settings entry           |
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
    class OpenRGBDMXSettingsEntry;
    class OpenRGBDMXSettingsEntryUi;
}

class Ui::OpenRGBDMXSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBDMXSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBDMXSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

private:
    Ui::OpenRGBDMXSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
