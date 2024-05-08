/*---------------------------------------------------------*\
| OpenRGBDialog.h                                           |
|                                                           |
|   User interface for OpenRGB main window                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include "ui_OpenRGBDialog.h"
#include "i2c_smbus.h"
#include "RGBController.h"

namespace Ui
{
    class OpenRGBDialog;
}

class Ui::OpenRGBDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpenRGBDialog(std::vector<i2c_smbus_interface *>& bus, std::vector<RGBController *>& control, QWidget *parent = 0);
    ~OpenRGBDialog();

    void show();
    void setMode(unsigned char mode_val);

protected:
    std::vector<i2c_smbus_interface *>& busses;
    std::vector<RGBController *>& controllers;

private slots:
    void on_ButtonRed_clicked();
    void on_ButtonYellow_clicked();
    void on_ButtonGreen_clicked();
    void on_ButtonCyan_clicked();
    void on_ButtonBlue_clicked();
    void on_ButtonMagenta_clicked();

    void on_ButtonSetAll_clicked();

    void on_ButtonSetDevice_clicked();

    void on_ButtonSetZone_clicked();

    void on_ButtonSetLED_clicked();

    void on_ComboDevices_currentIndexChanged();

    void on_ComboModes_currentIndexChanged();

private:
    Ui::OpenRGBDialogUi *ui;
};
