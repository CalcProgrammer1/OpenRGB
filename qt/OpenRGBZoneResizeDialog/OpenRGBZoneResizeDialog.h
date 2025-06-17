/*---------------------------------------------------------*\
| OpenRGBZoneResizeDialog.h                                 |
|                                                           |
|   User interface for resizing zones                       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>

#include "RGBController.h"

namespace Ui
{
    class OpenRGBZoneResizeDialog;
}

class OpenRGBZoneResizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBZoneResizeDialog(RGBController* edit_dev_ptr, unsigned int edit_zone_idx_val, QWidget *parent = nullptr);
    explicit OpenRGBZoneResizeDialog(unsigned int edit_zone_min_val, unsigned int edit_zone_max_val, unsigned int edit_zone_current_val, QWidget *parent = nullptr);
    ~OpenRGBZoneResizeDialog();

    int show();

private slots:
    void changeEvent(QEvent *event);
    void on_ResizeSlider_valueChanged(int value);

    void on_ResizeBox_valueChanged(int arg1);

    void on_AddSegmentButton_clicked();

    void on_RemoveSegmentButton_clicked();

    void on_segment_slider_valueChanged(int);

    void on_segment_lineedit_textChanged();

private:
    Ui::OpenRGBZoneResizeDialog *ui;
    RGBController* edit_dev;
    unsigned int edit_zone_idx;

    void CheckSegmentsValidity();
};
