/*---------------------------------------------------------*\
| OpenRGBSettingsPage.h                                     |
|                                                           |
|   User interface for OpenRGB settings page                |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      15 Apr 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include "nlohmann/json.hpp"

namespace Ui
{
    class OpenRGBSettingsPage;
}

class OpenRGBSettingsPage : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBSettingsPage();

    void OnSettingChanged(std::string key, nlohmann::json settings);
    void UpdateInterface();

private:
    /*-----------------------------------------------------*\
    | UI Pointer                                            |
    \*-----------------------------------------------------*/
    Ui::OpenRGBSettingsPage*    ui;
};
