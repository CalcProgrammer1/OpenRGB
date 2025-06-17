/*---------------------------------------------------------*\
| OpenRGBE131SettingsPage.h                                 |
|                                                           |
|   User interface for OpenRGB E1.31 settings page          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBE131SettingsPage.h"
#include "OpenRGBE131SettingsEntry.h"

namespace Ui
{
    class OpenRGBE131SettingsPage;
}

class OpenRGBE131SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBE131SettingsPage(QWidget *parent = nullptr);
    ~OpenRGBE131SettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddE131DeviceButton_clicked();

    void on_RemoveE131DeviceButton_clicked();

    void on_SaveE131ConfigurationButton_clicked();

private:
    Ui::OpenRGBE131SettingsPage *ui;
    std::vector<OpenRGBE131SettingsEntry*> entries;
};
