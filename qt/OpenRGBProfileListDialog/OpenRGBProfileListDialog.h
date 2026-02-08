/*---------------------------------------------------------*\
| OpenRGBProfileListDialog.h                                |
|                                                           |
|   User interface entry for OpenRGB profile save dialog    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include "OpenRGBDialog.h"

namespace Ui
{
    class OpenRGBProfileListDialog;
}

class OpenRGBProfileListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBProfileListDialog(bool create = true, QWidget *parent = nullptr);
    ~OpenRGBProfileListDialog();

    std::string show();

private:
    Ui::OpenRGBProfileListDialog *ui;

private slots:
    void changeEvent(QEvent *event);
};
