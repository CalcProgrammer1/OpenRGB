/*---------------------------------------------------------*\
| OpenRGBSupportedDevicePage.h                              |
|                                                           |
|   User interface for enabling and disabling devices       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include <QSortFilterProxyModel>
#include "DetectorTableModel.h"

namespace Ui
{
    class OpenRGBSupportedDevicesPage;
}

class OpenRGBSupportedDevicesPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBSupportedDevicesPage(QWidget *parent = nullptr);
    ~OpenRGBSupportedDevicesPage();
    DetectorTableModel* detectorTableModel;
    QSortFilterProxyModel* detectorSortModel;

private slots:
    void changeEvent(QEvent *event);
    void on_SaveButton_clicked();
    void on_GetHardwareIDsButton_clicked();

    void on_Filter_textChanged(const QString &arg1);

    void on_ToggleAllCheckbox_toggled(const bool checked);

private:
    Ui::OpenRGBSupportedDevicesPage *ui;
};
