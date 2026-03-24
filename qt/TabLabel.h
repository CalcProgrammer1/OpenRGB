/*---------------------------------------------------------*\
| TabLabel.h                                                |
|                                                           |
|   Qt widget for OpenRGB tab label                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>

namespace Ui
{
    class TabLabel;
}

class TabLabel : public QWidget
{
    Q_OBJECT

public:
    TabLabel(int icon, QString name, char* original, char* context);
    ~TabLabel();

    void SetTextHidden(bool hidden);

private:
    Ui::TabLabel *ui;
    char* label;
    char* ctxt;

    int icon_initial_width;
    int name_initial_width;


private slots:
    void changeEvent(QEvent *event);
};
