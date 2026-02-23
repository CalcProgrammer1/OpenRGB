/*---------------------------------------------------------*\
| OpenRGBZoneEditorDialog.h                                 |
|                                                           |
|   User interface for editing zones                        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include <QTreeWidgetItem>
#include "RGBController.h"

namespace Ui
{
    class OpenRGBZoneEditorDialog;
}


class SegmentTreeWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT

public:
    using QTreeWidgetItem::QTreeWidgetItem;

    matrix_map_type matrix_map;

private slots:
    void on_button_matrix_map_clicked();
};

class OpenRGBZoneEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBZoneEditorDialog(RGBController* edit_dev_ptr, unsigned int edit_zone_idx_val, QWidget *parent = nullptr);
    ~OpenRGBZoneEditorDialog();

    int show();

private slots:
    void changeEvent(QEvent *event);
    void on_SliderZoneSize_valueChanged(int value);
    void on_SpinBoxZoneSize_valueChanged(int arg1);
    void on_AddSegmentButton_clicked();
    void on_RemoveSegmentButton_clicked();
    void on_segment_slider_valueChanged(int);
    void on_segment_lineedit_textChanged();
    void on_ImportConfigurationButton_clicked();
    void on_ExportConfigurationButton_clicked();
    void on_ButtonZoneMatrixMap_clicked();
    void on_ButtonResetZoneConfiguration_clicked();
    void on_LineEditZoneName_textChanged(const QString& arg1);
    void on_ComboBoxZoneType_currentIndexChanged(int index);
    void on_ComboBoxZoneColorOrder_currentIndexChanged(int index);

private:
    Ui::OpenRGBZoneEditorDialog*    ui;
    RGBController*                  edit_dev;
    zone                            edit_zone;
    unsigned int                    edit_zone_idx;

    void AddSegmentRow(QString name, unsigned int length, zone_type type, matrix_map_type matrix_map);
    void CheckSegmentsValidity();
};
