/*---------------------------------------------------------*\
| OpenRGBPhilipsHueSettingsPage.h                           |
|                                                           |
|   User interface for OpenRGB Philips Hue settings page    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "OpenRGBPhilipsHueSettingsEntry.h"

namespace Ui
{
    class OpenRGBPhilipsHueSettingsPage;
}

class OpenRGBPhilipsHueSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPhilipsHueSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBPhilipsHueSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddPhilipsHueDeviceButton_clicked();

    void on_RemovePhilipsHueDeviceButton_clicked();

    void on_SavePhilipsHueConfigurationButton_clicked();

private:
    Ui::OpenRGBPhilipsHueSettingsPage *ui;
    std::vector<OpenRGBPhilipsHueSettingsEntry*> entries;
};
