/*---------------------------------------------------------*\
| OpenRGBDeviceEditorDialog.h                               |
|                                                           |
|   User interface for OpenRGB device editor dialog         |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      15 Apr 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include "RGBController.h"

namespace Ui
{
    class OpenRGBDeviceEditorDialog;
}

class OpenRGBDeviceEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBDeviceEditorDialog(RGBController *dev, QWidget *parent = nullptr);
    ~OpenRGBDeviceEditorDialog();

    int show();
    void OnSettingChanged(std::string key, nlohmann::json settings);

private:
    /*-----------------------------------------------------*\
    | UI Pointer                                            |
    \*-----------------------------------------------------*/
    Ui::OpenRGBDeviceEditorDialog*  ui;

    /*-----------------------------------------------------*\
    | Device pointer                                        |
    \*-----------------------------------------------------*/
    RGBController*                  edit_dev;

    /*-----------------------------------------------------*\
    | Device configuration                                  |
    \*-----------------------------------------------------*/
    nlohmann::json                  device_configuration;
};
