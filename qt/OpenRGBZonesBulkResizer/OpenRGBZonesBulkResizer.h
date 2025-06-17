/*---------------------------------------------------------*\
| OpenRGBZonesBulkResizer.h                                 |
|                                                           |
|   User interface for bulk resizing zones                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "RGBController.h"

namespace Ui
{
    class OpenRGBZonesBulkResizer;
}

class OpenRGBZonesBulkResizer : public QWidget
{
    Q_OBJECT

public:
    static void RunChecks(QWidget *parent);

    explicit OpenRGBZonesBulkResizer(QWidget*, const std::vector<std::tuple<RGBController*, unsigned int>>&);
    ~OpenRGBZonesBulkResizer();

private:
    Ui::OpenRGBZonesBulkResizer *ui;

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
