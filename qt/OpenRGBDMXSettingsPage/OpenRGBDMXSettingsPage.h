/*---------------------------------------------------------*\
| OpenRGBDMXSettingsPage.h                                  |
|                                                           |
|   User interface for OpenRGB DMX settings page            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "OpenRGBDMXSettingsEntry.h"

namespace Ui
{
    class OpenRGBDMXSettingsPage;
}

class OpenRGBDMXSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBDMXSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBDMXSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddDMXDeviceButton_clicked();

    void on_RemoveDMXDeviceButton_clicked();

    void on_SaveDMXConfigurationButton_clicked();

private:
    Ui::OpenRGBDMXSettingsPage*           ui;
    std::vector<OpenRGBDMXSettingsEntry*> entries;

};
