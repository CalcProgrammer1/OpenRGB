/*---------------------------------------------------------*\
| OpenRGBKasaSmartSettingsEntry.h                           |
|                                                           |
|   User interface for OpenRGB Kasa Smart settings entry    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBKasaSmartSettingsEntry.h"

namespace Ui
{
    class OpenRGBKasaSmartSettingsEntry;
}

class Ui::OpenRGBKasaSmartSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBKasaSmartSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBKasaSmartSettingsEntry();
    Ui::OpenRGBKasaSmartSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
