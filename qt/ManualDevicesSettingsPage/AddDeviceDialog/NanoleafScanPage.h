/*---------------------------------------------------------*\
| OpenRGBNanoleafScanPage.h                                 |
|                                                           |
|   User interface for OpenRGB Nanoleaf scan & pairing page |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "OpenRGBNanoleafSettingsEntry.h"

namespace Ui
{
    class NanoleafScanPage;
}

class NanoleafScanPage : public QWidget
{
    Q_OBJECT

public:
    explicit NanoleafScanPage(QWidget *parent = nullptr);
    ~NanoleafScanPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddNanoleafDeviceButton_clicked();
    void on_RemoveNanoleafDeviceButton_clicked();
    void on_ScanForNanoleafDevicesButton_clicked();
    void on_DeviceFound(QString address, int port);

private:
    Ui::NanoleafScanPage *ui;
    std::map<std::string, OpenRGBNanoleafSettingsEntry*> entries;
};
