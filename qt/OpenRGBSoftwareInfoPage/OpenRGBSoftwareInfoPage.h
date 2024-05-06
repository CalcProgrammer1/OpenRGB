/*---------------------------------------------------------*\
| OpenRGBSoftwareInfoPage.h                                 |
|                                                           |
|   User interface for software information page            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QFrame>
#include "ui_OpenRGBSoftwareInfoPage.h"

namespace Ui
{
    class OpenRGBSoftwareInfoPage;
}

class Ui::OpenRGBSoftwareInfoPage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBSoftwareInfoPage(QWidget *parent = nullptr);
    ~OpenRGBSoftwareInfoPage();

private:
    Ui::OpenRGBSoftwareInfoPageUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
