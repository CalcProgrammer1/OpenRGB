/*---------------------------------------------------------*\
| OpenRGBProfileEditorDialog.h                              |
|                                                           |
|   User interface entry for OpenRGB profile editor dialog  |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      16 Feb 2026 |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include "RGBController.h"
#include "ui_OpenRGBProfileEditorDialog.h"

namespace Ui
{
    class OpenRGBProfileEditorDialog;
}

class OpenRGBProfileEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBProfileEditorDialog(std::string name, QWidget* parent = nullptr);
    ~OpenRGBProfileEditorDialog();

    bool show();

private:
    Ui::OpenRGBProfileEditorDialog* ui;
    bool                            HexFormatRGB;
    std::string                     profile_name;

    std::vector<RGBController*>     old_controllers;
    std::vector<RGBController*>     new_controllers;
    std::vector<RGBController*>     updated_controllers;

private slots:
    void changeEvent(QEvent* event);
    void on_ColorWheelBaseColor_colorChanged(const QColor& color);
    void on_LineEditBaseColorHex_textChanged(const QString& text);
    void on_ButtonSelectAllControllerStates_clicked();
    void on_ButtonSelectNoneControllerStates_clicked();
    void on_ButtonSelectAllPlugins_clicked();
    void on_ButtonSelectNonePlugins_clicked();
};
