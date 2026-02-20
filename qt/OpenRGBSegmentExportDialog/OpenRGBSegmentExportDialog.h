/*---------------------------------------------------------*\
| OpenRGBSegmentExportDialog.h                              |
|                                                           |
|   User interface entry for OpenRGB segment export dialog  |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      22 Feb 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>

namespace Ui
{
    class OpenRGBSegmentExportDialog;
}

class OpenRGBSegmentExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBSegmentExportDialog(QWidget *parent = nullptr);
    ~OpenRGBSegmentExportDialog();

    bool show();

    QString GetDevice();
    QString GetFilename();
    QString GetVendor();

private:
    Ui::OpenRGBSegmentExportDialog* ui;
    QString                         device;
    QString                         filename;
    QString                         vendor;

private slots:
    void changeEvent(QEvent *event);
    void on_ButtonChooseFile_clicked();
};
