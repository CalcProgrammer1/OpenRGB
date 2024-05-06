/*---------------------------------------------------------*\
| OpenRGBPhilipsHueSettingsEntry.h                          |
|                                                           |
|   User interface for OpenRGB Philips Hue settings entry   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "ui_OpenRGBPhilipsHueSettingsEntry.h"
#include <QWidget>

namespace Ui
{
    class OpenRGBPhilipsHueSettingsEntry;
}

class Ui::OpenRGBPhilipsHueSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPhilipsHueSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBPhilipsHueSettingsEntry();
    Ui::OpenRGBPhilipsHueSettingsEntryUi *ui;
private slots:
    void changeEvent(QEvent *event);
    void on_UnpairButton_clicked();
};
