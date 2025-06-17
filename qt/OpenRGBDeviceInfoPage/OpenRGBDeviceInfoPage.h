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

namespace Ui
{
    class OpenRGBDeviceInfoPage;
}

class OpenRGBDeviceInfoPage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBDeviceInfoPage(RGBController *dev, QWidget *parent = nullptr);
    ~OpenRGBDeviceInfoPage();

    RGBController* GetController();

private:
    RGBController*             controller;
    Ui::OpenRGBDeviceInfoPage* ui;

private slots:
    void changeEvent(QEvent *event);
};
