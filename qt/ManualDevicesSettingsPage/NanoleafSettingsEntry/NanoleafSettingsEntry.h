/*---------------------------------------------------------*\
| NanoleafSettingsEntry.h                                   |
|                                                           |
|   User interface for OpenRGB Nanoleaf settings entry      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"
#include "NanoleafScanningThread.h"

namespace Ui
{
    class NanoleafSettingsEntry;
}

class NanoleafSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit NanoleafSettingsEntry(QWidget *parent = nullptr);
    NanoleafSettingsEntry(QString a_address, int a_port);
    ~NanoleafSettingsEntry();
    void loadFromSettings(const json& data);
    std::string getLocation();

    json saveSettings() override;
    bool isDataValid() override;

private slots:
    void changeEvent(QEvent *event) override;
    void on_UnpairButton_clicked();
    void on_PairButton_clicked();

private:
    Ui::NanoleafSettingsEntry *ui;
    QString address;
    int port;
    std::string auth_token;
    bool paired;
};
