/*---------------------------------------------------------*\
| OpenRGBPhilipsWizSettingsPage.h                           |
|                                                           |
|   User interface for OpenRGB Philips Wiz settings page    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBPhilipsWizSettingsPage.h"
#include "OpenRGBPhilipsWizSettingsEntry.h"

namespace Ui
{
    class OpenRGBPhilipsWizSettingsPage;
}

class Ui::OpenRGBPhilipsWizSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPhilipsWizSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBPhilipsWizSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddPhilipsWizDeviceButton_clicked();

    void on_RemovePhilipsWizDeviceButton_clicked();

    void on_SavePhilipsWizConfigurationButton_clicked();

private:
    Ui::OpenRGBPhilipsWizSettingsPageUi *ui;
    std::vector<OpenRGBPhilipsWizSettingsEntry*> entries;

};
