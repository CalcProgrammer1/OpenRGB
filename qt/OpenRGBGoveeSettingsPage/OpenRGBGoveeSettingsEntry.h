/*---------------------------------------------------------*\
| OpenRGBGoveeSettingsEntry.h                               |
|                                                           |
|   User interface for OpenRGB Govee settings entry         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBGoveeSettingsEntry.h"

namespace Ui
{
    class OpenRGBGoveeSettingsEntry;
}

class Ui::OpenRGBGoveeSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBGoveeSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBGoveeSettingsEntry();
    Ui::OpenRGBGoveeSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
