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
#include "OpenRGBPhilipsWizSettingsEntry.h"

namespace Ui
{
    class OpenRGBPhilipsWizSettingsPage;
}

class OpenRGBPhilipsWizSettingsPage : public QWidget
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
    Ui::OpenRGBPhilipsWizSettingsPage *ui;
    std::vector<OpenRGBPhilipsWizSettingsEntry*> entries;

};
