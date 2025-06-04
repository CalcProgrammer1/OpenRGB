/*---------------------------------------------------------*\
| OpenRGBKasaSmartSettingsEntry.h                           |
|                                                           |
|   User interface for OpenRGB Kasa Smart settings entry    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include <QWidget>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Ui
{
    class OpenRGBKasaSmartSettingsEntry;
    class OpenRGBKasaSmartSettingsEntryUi;
}

class Ui::OpenRGBKasaSmartSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBKasaSmartSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBKasaSmartSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();
    void setName(QString name);

private:
    Ui::OpenRGBKasaSmartSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
