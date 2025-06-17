/*---------------------------------------------------------*\
| OpenRGBNanoleafSettingsPage.h                             |
|                                                           |
|   User interface for OpenRGB Nanoleaf settings page       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "OpenRGBNanoleafSettingsEntry.h"

namespace Ui
{
    class OpenRGBNanoleafSettingsPage;
}

class OpenRGBNanoleafSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBNanoleafSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBNanoleafSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddNanoleafDeviceButton_clicked();
    void on_RemoveNanoleafDeviceButton_clicked();
    void on_ScanForNanoleafDevicesButton_clicked();
    void on_DeviceFound(QString address, int port);

private:
    Ui::OpenRGBNanoleafSettingsPage *ui;
    std::map<std::string, OpenRGBNanoleafSettingsEntry*> entries;
};
