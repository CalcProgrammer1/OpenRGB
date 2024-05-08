/*---------------------------------------------------------*\
| OpenRGBHardwareIDsDialog.h                                |
|                                                           |
|   User interface for OpenRGB Hardware IDs dialog          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include "ui_OpenRGBHardwareIDsDialog.h"

namespace Ui
{
    class OpenRGBHardwareIDsDialog;
}

class Ui::OpenRGBHardwareIDsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBHardwareIDsDialog(QWidget *parent = nullptr);
    ~OpenRGBHardwareIDsDialog();

    int show();

private slots:
    void on_CopyToClipboardButton_clicked();

private:
    Ui::OpenRGBHardwareIDsDialogUi *ui;
    QStringList strings;
};
