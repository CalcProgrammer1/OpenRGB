/*---------------------------------------------------------*\
| OpenRGBNanoleafSettingsEntry.h                            |
|                                                           |
|   User interface for OpenRGB Nanoleaf settings entry      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBNanoleafSettingsEntry.h"
#include "OpenRGBNanoleafScanningThread.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Ui
{
    class OpenRGBNanoleafSettingsEntry;
    class OpenRGBNanoleafSettingsEntryUi;
}

class Ui::OpenRGBNanoleafSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBNanoleafSettingsEntry(QWidget *parent = nullptr);
    OpenRGBNanoleafSettingsEntry(QString a_address, int a_port);
    ~OpenRGBNanoleafSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();

    QString address;
    int port;

private:
    Ui::OpenRGBNanoleafSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
    void on_UnpairButton_clicked();
    void on_PairButton_clicked();

private:
    std::string auth_token;
    bool paired;
};
