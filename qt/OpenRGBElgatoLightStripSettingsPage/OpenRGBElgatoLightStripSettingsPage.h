/*---------------------------------------------------------*\
| OpenRGBElgatoLightStripSettingsPage.h                     |
|                                                           |
|   User interface for OpenRGB Elgato Light Strips page     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "OpenRGBElgatoLightStripSettingsEntry.h"

namespace Ui
{
    class OpenRGBElgatoLightStripSettingsPage;
}

class OpenRGBElgatoLightStripSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBElgatoLightStripSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBElgatoLightStripSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddElgatoLightStripDeviceButton_clicked();

    void on_RemoveElgatoLightStripDeviceButton_clicked();

    void on_SaveElgatoLightStripConfigurationButton_clicked();

private:
    Ui::OpenRGBElgatoLightStripSettingsPage *ui;
    std::vector<OpenRGBElgatoLightStripSettingsEntry*> entries;
};
