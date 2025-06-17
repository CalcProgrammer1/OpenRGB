/*---------------------------------------------------------*\
| OpenRGBSerialSettingsPage.h                               |
|                                                           |
|   User interface for serial device configuration page     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>

#include "OpenRGBSerialSettingsEntry.h"

namespace Ui
{
    class OpenRGBSerialSettingsPage;
}

class OpenRGBSerialSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBSerialSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBSerialSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddSerialDeviceButton_clicked();

    void on_RemoveSerialDeviceButton_clicked();

    void on_SaveSerialConfigurationButton_clicked();

private:
    Ui::OpenRGBSerialSettingsPage *ui;
    std::vector<OpenRGBSerialSettingsEntry*> entries;

};
