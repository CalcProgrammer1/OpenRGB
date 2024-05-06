/*---------------------------------------------------------*\
| OpenRGBYeelightSettingsEntry.h                            |
|                                                           |
|   User interface for Yeelight settings entry              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBYeelightSettingsEntry.h"

namespace Ui
{
    class OpenRGBYeelightSettingsEntry;
}

class Ui::OpenRGBYeelightSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBYeelightSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBYeelightSettingsEntry();
    Ui::OpenRGBYeelightSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
    void on_HostIPChooserButton_clicked();
};
