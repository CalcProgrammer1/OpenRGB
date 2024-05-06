/*---------------------------------------------------------*\
| OpenRGBLIFXSettingsEntry.h                                |
|                                                           |
|   User interface for OpenRGB LIFX settings entry          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QWidget>
#include "ui_OpenRGBLIFXSettingsEntry.h"

namespace Ui
{
    class OpenRGBLIFXSettingsEntry;
}

class Ui::OpenRGBLIFXSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBLIFXSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBLIFXSettingsEntry();
    Ui::OpenRGBLIFXSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
