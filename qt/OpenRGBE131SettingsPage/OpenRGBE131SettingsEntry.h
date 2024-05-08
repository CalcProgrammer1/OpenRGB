/*---------------------------------------------------------*\
| OpenRGBE131SettingsEntry.h                                |
|                                                           |
|   User interface for OpenRGB E1.31 settings entry         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBE131SettingsEntry.h"

namespace Ui
{
    class OpenRGBE131SettingsEntry;
}

class Ui::OpenRGBE131SettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBE131SettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBE131SettingsEntry();
    Ui::OpenRGBE131SettingsEntryUi *ui;

private:
    void HideMatrixSettings();
    void ShowMatrixSettings();

private slots:
    void changeEvent(QEvent *event);
    void on_TypeComboBox_currentIndexChanged(int index);
};
