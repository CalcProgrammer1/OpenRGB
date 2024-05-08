/*---------------------------------------------------------*\
| OpenRGBDeviceInfoPage.h                                   |
|                                                           |
|   User interface for OpenRGB device information page      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QFrame>
#include "RGBController.h"
#include "ui_OpenRGBDeviceInfoPage.h"

namespace Ui
{
    class OpenRGBDeviceInfoPage;
}

class Ui::OpenRGBDeviceInfoPage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBDeviceInfoPage(RGBController *dev, QWidget *parent = nullptr);
    ~OpenRGBDeviceInfoPage();

    RGBController* GetController();

private:
    RGBController*                  controller;
    Ui::OpenRGBDeviceInfoPageUi*    ui;

private slots:
    void changeEvent(QEvent *event);
};
