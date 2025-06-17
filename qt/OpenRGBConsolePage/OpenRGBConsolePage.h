/*---------------------------------------------------------*\
| OpenRGBConsolePage.h                                      |
|                                                           |
|   User interface for OpenRGB console page                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QFrame>

namespace Ui
{
    class OpenRGBConsolePage;
}

class OpenRGBConsolePage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBConsolePage(QWidget *parent = nullptr);
    ~OpenRGBConsolePage();

private slots:
    void changeEvent(QEvent *event);
    void on_log_level_currentIndexChanged(int);
    void on_clear_clicked();
    void on_refresh_clicked();

private:
    Ui::OpenRGBConsolePage *ui;

    void Refresh();
};
