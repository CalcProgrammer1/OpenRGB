/*---------------------------------------------------------*\
| OpenRGBGoveeSettingsPage.h                                |
|                                                           |
|   User interface for OpenRGB Govee settings page          |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "OpenRGBGoveeSettingsEntry.h"

namespace Ui
{
    class OpenRGBGoveeSettingsPage;
}

class OpenRGBGoveeSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBGoveeSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBGoveeSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddGoveeDeviceButton_clicked();

    void on_RemoveGoveeDeviceButton_clicked();

    void on_SaveGoveeConfigurationButton_clicked();

private:
    Ui::OpenRGBGoveeSettingsPage *ui;
    std::vector<OpenRGBGoveeSettingsEntry*> entries;

};
