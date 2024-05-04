/*---------------------------------------------------------*\
| TabLabel.h                                                |
|                                                           |
|   Qt widget for OpenRGB tab label                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_TabLabel.h"

namespace Ui
{
    class TabLabel;
}

class Ui::TabLabel : public QWidget
{
    Q_OBJECT

public:
    TabLabel(int icon, QString name, char* original, char* context);
    ~TabLabel();

private:
    Ui::TabLabelUi *ui;
    char* label;
    char* ctxt;

private slots:
    void changeEvent(QEvent *event);
};
