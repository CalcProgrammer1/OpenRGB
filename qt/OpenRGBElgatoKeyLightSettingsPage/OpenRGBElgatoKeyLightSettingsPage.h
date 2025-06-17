/*---------------------------------------------------------*\
| OpenRGBElgatoKeyLightSettingsPage.h                       |
|                                                           |
|   User interface for OpenRGB Elgato Key Light page        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "OpenRGBElgatoKeyLightSettingsEntry.h"

namespace Ui
{
    class OpenRGBElgatoKeyLightSettingsPage;
}

class OpenRGBElgatoKeyLightSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBElgatoKeyLightSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBElgatoKeyLightSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddElgatoKeyLightDeviceButton_clicked();

    void on_RemoveElgatoKeyLightDeviceButton_clicked();

    void on_SaveElgatoKeyLightConfigurationButton_clicked();

private:
    Ui::OpenRGBElgatoKeyLightSettingsPage *ui;
    std::vector<OpenRGBElgatoKeyLightSettingsEntry*> entries;
};
