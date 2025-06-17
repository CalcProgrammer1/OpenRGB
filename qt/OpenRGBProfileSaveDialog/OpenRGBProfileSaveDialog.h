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
#include "OpenRGBDialog.h"

namespace Ui
{
    class OpenRGBProfileSaveDialog;
}

class OpenRGBProfileSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBProfileSaveDialog(QWidget *parent = nullptr);
    ~OpenRGBProfileSaveDialog();

    std::string show();

private:
    Ui::OpenRGBProfileSaveDialog *ui;

private slots:
    void changeEvent(QEvent *event);
};
