/*---------------------------------------------------------*\
| OpenRGBQMKORGBSettingsPage.h                              |
|                                                           |
|   User interface for OpenRGB QMK device configuration page|
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "OpenRGBQMKORGBSettingsEntry.h"

namespace Ui
{
    class OpenRGBQMKORGBSettingsPage;
    class OpenRGBQMKORGBSettingsPageUi;
}

class Ui::OpenRGBQMKORGBSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBQMKORGBSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBQMKORGBSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddQMKORGBDeviceButton_clicked();

    void on_RemoveQMKORGBDeviceButton_clicked();

    void on_SaveQMKORGBConfigurationButton_clicked();

private:
    Ui::OpenRGBQMKORGBSettingsPageUi *ui;
    std::vector<OpenRGBQMKORGBSettingsEntry*> entries;

};
