/*---------------------------------------------------------*\
| OpenRGBElgatoKeyLightSettingsEntry.h                      |
|                                                           |
|   User interface for OpenRGB Elgato Key Light entry       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include "ui_OpenRGBElgatoKeyLightSettingsEntry.h"

namespace Ui
{
    class OpenRGBElgatoKeyLightSettingsEntry;
}

class Ui::OpenRGBElgatoKeyLightSettingsEntry : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBElgatoKeyLightSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBElgatoKeyLightSettingsEntry();
    Ui::OpenRGBElgatoKeyLightSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
