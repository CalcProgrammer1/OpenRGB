/*---------------------------------------------------------*\
| OpenRGBZoneInitializationDialog.h                         |
|                                                           |
|   User interface for initializing zones                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "RGBController.h"

namespace Ui
{
    class OpenRGBZoneInitializationDialog;
}

class OpenRGBZoneInitializationDialog : public QWidget
{
    Q_OBJECT

public:
    static void RunChecks(QWidget *parent);

    explicit OpenRGBZoneInitializationDialog(QWidget*, const std::vector<std::tuple<RGBController*, unsigned int>>&);
    ~OpenRGBZoneInitializationDialog();

private:
    Ui::OpenRGBZoneInitializationDialog *ui;

    const std::vector<std::tuple<RGBController*, unsigned int>>& unconfigured_zones;

    void CreateZoneWidget(RGBController*, unsigned int);

    void SaveDoNotRunState();

signals:
    void CloseRequest();

private slots:
    void changeEvent(QEvent *event);
    void on_save_button_clicked();
    void on_ignore_button_clicked();

};
