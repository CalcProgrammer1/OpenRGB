/*---------------------------------------------------------*\
| OpenRGBQMKORGBSettingsEntry.h                             |
|                                                           |
|   User interface entry for OpenRGB QMK configuration      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBQMKORGBSettingsEntry.h"

namespace Ui
{
    class OpenRGBQMKORGBSettingsEntry;
}

class Ui::OpenRGBQMKORGBSettingsEntry : public QWidget
{
    Q_OBJECT

private slots:
    void changeEvent(QEvent *event);

public:
    explicit OpenRGBQMKORGBSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBQMKORGBSettingsEntry();
    Ui::OpenRGBQMKORGBSettingsEntryUi *ui;
};
