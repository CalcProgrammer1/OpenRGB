/*---------------------------------------------------------*\
| OpenRGBMatrixMapEditorDialog.h                            |
|                                                           |
|   User interface for editing matrix maps                  |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      21 Feb 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include "RGBController.h"

namespace Ui
{
    class OpenRGBMatrixMapEditorDialog;
}

class OpenRGBMatrixMapEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBMatrixMapEditorDialog(QString name, matrix_map_type* edit_map_ptr, unsigned int led_count, QWidget *parent = nullptr);
    ~OpenRGBMatrixMapEditorDialog();

    int show();

private slots:
    void changeEvent(QEvent *event);

    void on_LineEditHeight_textChanged(const QString &arg1);

    void on_LineEditWidth_textChanged(const QString &arg1);

private:
    Ui::OpenRGBMatrixMapEditorDialog*   ui;
    matrix_map_type*                    edit_map;
    unsigned int                        edit_led_count;
};
