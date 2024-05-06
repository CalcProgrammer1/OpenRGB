/*---------------------------------------------------------*\
| OpenRGBSerialSettingsEntry.h                              |
|                                                           |
|   User interface entry for serial device configuration    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBSerialSettingsEntry.h"

namespace Ui
{
    class OpenRGBSerialSettingsEntry;
}

class Ui::OpenRGBSerialSettingsEntry : public QWidget
{
    Q_OBJECT

private slots:
    void changeEvent(QEvent *event);

    void on_ProtocolComboBox_currentIndexChanged(int index);

public:
    explicit OpenRGBSerialSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBSerialSettingsEntry();
    Ui::OpenRGBSerialSettingsEntryUi *ui;
};
