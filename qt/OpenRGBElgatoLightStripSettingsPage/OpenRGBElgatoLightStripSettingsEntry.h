/*---------------------------------------------------------*\
| OpenRGBElgatoLightStripSettingsEntry.h                    |
|                                                           |
|   User interface for OpenRGB Elgato Light Strips entry    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include "ui_OpenRGBElgatoLightStripSettingsEntry.h"

namespace Ui
{
    class OpenRGBElgatoLightStripSettingsEntry;
}

class Ui::OpenRGBElgatoLightStripSettingsEntry : public QDialog
{
    Q_OBJECT

    public:
        explicit OpenRGBElgatoLightStripSettingsEntry(QWidget *parent = nullptr);
        ~OpenRGBElgatoLightStripSettingsEntry();
        Ui::OpenRGBElgatoLightStripSettingsEntryUi *ui;

    private slots:
        void changeEvent(QEvent *event);
};
