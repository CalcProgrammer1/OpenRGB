/*---------------------------------------------------------*\
| OpenRGBLIFXSettingsPage.h                                 |
|                                                           |
|   User interface for OpenRGB LIFX settings page           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBLIFXSettingsPage.h"
#include "OpenRGBLIFXSettingsEntry.h"

namespace Ui
{
    class OpenRGBLIFXSettingsPage;
}

class Ui::OpenRGBLIFXSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBLIFXSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBLIFXSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddLIFXDeviceButton_clicked();

    void on_RemoveLIFXDeviceButton_clicked();

    void on_SaveLIFXConfigurationButton_clicked();

private:
    Ui::OpenRGBLIFXSettingsPageUi *ui;
    std::vector<OpenRGBLIFXSettingsEntry*> entries;

};
