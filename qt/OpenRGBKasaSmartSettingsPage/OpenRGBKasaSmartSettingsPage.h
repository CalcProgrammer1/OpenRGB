/*---------------------------------------------------------*\
| OpenRGBKasaSmartSettingsPage.h                            |
|                                                           |
|   User interface for OpenRGB Kasa Smart settings page     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QWidget>
#include "OpenRGBKasaSmartSettingsEntry.h"

namespace Ui
{
    class OpenRGBKasaSmartSettingsPage;
}

class OpenRGBKasaSmartSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBKasaSmartSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBKasaSmartSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddKasaSmartDeviceButton_clicked();

    void on_RemoveKasaSmartDeviceButton_clicked();

    void on_SaveKasaSmartConfigurationButton_clicked();

private:
    Ui::OpenRGBKasaSmartSettingsPage *ui;
    std::vector<OpenRGBKasaSmartSettingsEntry*> entries;

};
