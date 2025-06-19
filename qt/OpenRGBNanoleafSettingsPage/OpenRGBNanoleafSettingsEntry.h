/*---------------------------------------------------------*\
| OpenRGBNanoleafSettingsEntry.h                            |
|                                                           |
|   User interface for OpenRGB Nanoleaf settings entry      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"
#include "OpenRGBNanoleafScanningThread.h"

namespace Ui
{
    class OpenRGBNanoleafSettingsEntry;
}

class OpenRGBNanoleafSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit OpenRGBNanoleafSettingsEntry(QWidget *parent = nullptr);
    OpenRGBNanoleafSettingsEntry(QString a_address, int a_port);
    ~OpenRGBNanoleafSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();
    std::string getLocation();

private slots:
    void changeEvent(QEvent *event);
    void on_UnpairButton_clicked();
    void on_PairButton_clicked();

private:
    Ui::OpenRGBNanoleafSettingsEntry *ui;
    QString address;
    int port;
    std::string auth_token;
    bool paired;
};
