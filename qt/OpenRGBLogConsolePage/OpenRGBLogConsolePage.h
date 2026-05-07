/*---------------------------------------------------------*\
| OpenRGBLogConsolePage.h                                   |
|                                                           |
|   User interface for OpenRGB console page                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QFrame>
#include "LogManager.h"

namespace Ui
{
    class OpenRGBLogConsolePage;
}

class OpenRGBLogConsolePage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBLogConsolePage(QWidget *parent = nullptr);
    ~OpenRGBLogConsolePage();

private slots:
    void AddLogEntry(PLogMessage message);
    void changeEvent(QEvent *event);
    void on_ComboBoxLogLevel_currentIndexChanged(int);
    void on_ButtonClear_clicked();

private:
    Ui::OpenRGBLogConsolePage *ui;

    void Refresh();
};
