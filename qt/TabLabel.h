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
    TabLabel(int icon, char* label, char* context, bool translatable);
    ~TabLabel();

private:
    Ui::TabLabel*   ui;
    char*           label;
    char*           context;
    bool            translatable;

    void UpdateLabel(bool in_constructor);

private slots:
    void changeEvent(QEvent *event);
};
