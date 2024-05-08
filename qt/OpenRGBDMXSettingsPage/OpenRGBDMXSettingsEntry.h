/*---------------------------------------------------------*\
| OpenRGBDMXSettingsEntry.h                                 |
|                                                           |
|   User interface for OpenRGB DMX settings entry           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBDMXSettingsEntry.h"

namespace Ui
{
    class OpenRGBDMXSettingsEntry;
}

class Ui::OpenRGBDMXSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBDMXSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBDMXSettingsEntry();
    Ui::OpenRGBDMXSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
