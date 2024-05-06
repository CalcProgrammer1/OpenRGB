/*---------------------------------------------------------*\
| OpenRGBProfileSaveDialog.h                                |
|                                                           |
|   User interface entry for OpenRGB profile save dialog    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include "ui_OpenRGBProfileSaveDialog.h"
#include "OpenRGBDialog2.h"

namespace Ui
{
    class OpenRGBProfileSaveDialog;
}

class Ui::OpenRGBProfileSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBProfileSaveDialog(QWidget *parent = nullptr);
    ~OpenRGBProfileSaveDialog();

    std::string show();

private:
    Ui::OpenRGBProfileSaveDialogUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
