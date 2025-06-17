/*---------------------------------------------------------*\
| OpenRGBSystemInfoPage.h                                   |
|                                                           |
|   User interface entry for OpenRGB system information page|
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QFrame>
#include "i2c_smbus.h"

namespace Ui
{
    class OpenRGBSystemInfoPage;
}

class OpenRGBSystemInfoPage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBSystemInfoPage(std::vector<i2c_smbus_interface *>& bus, QWidget *parent = nullptr);
    ~OpenRGBSystemInfoPage();

public slots:
    void UpdateBusList();

private slots:
    void changeEvent(QEvent *event);
    void on_DetectButton_clicked();

    void on_DumpButton_clicked();

    void on_ReadButton_clicked();

private:
    Ui::OpenRGBSystemInfoPage *ui;
    std::vector<i2c_smbus_interface *>& busses;
};
