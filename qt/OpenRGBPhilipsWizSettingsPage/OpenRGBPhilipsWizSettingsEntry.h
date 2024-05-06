/*---------------------------------------------------------*\
| OpenRGBPhilipsWizSettingsEntry.h                          |
|                                                           |
|   User interface for OpenRGB Philips Wiz settings entry   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBPhilipsWizSettingsEntry.h"

namespace Ui
{
    class OpenRGBPhilipsWizSettingsEntry;
}

class Ui::OpenRGBPhilipsWizSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPhilipsWizSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBPhilipsWizSettingsEntry();
    Ui::OpenRGBPhilipsWizSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
