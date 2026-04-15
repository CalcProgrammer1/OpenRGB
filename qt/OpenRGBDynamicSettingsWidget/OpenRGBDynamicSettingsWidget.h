/*---------------------------------------------------------*\
| OpenRGBDynamicSettingsWidget.h                            |
|                                                           |
|   Widget for one dynamic settings list entry              |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      16 Apr 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include "nlohmann/json.hpp"

/*---------------------------------------------------------*\
| Callback Type                                             |
\*---------------------------------------------------------*/
typedef void (*OpenRGBDynamicSettingsCallback)(void*, std::string, nlohmann::json);

class OpenRGBDynamicSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    OpenRGBDynamicSettingsWidget(std::string key, nlohmann::json& schema, nlohmann::json& settings, QWidget* parent = nullptr);
    ~OpenRGBDynamicSettingsWidget();

    void ProfileListUpdated();

    void SetCallback(OpenRGBDynamicSettingsCallback callback, void* callback_arg);
    void OnSettingChanged();

private:
    std::string                     key;
    std::string                     type;
    bool                            is_enum;

    QHBoxLayout*                    layout;
    QWidget*                        left_widget;
    QWidget*                        right_widget;

    OpenRGBDynamicSettingsCallback  callback;
    void*                           callback_arg;

public:
    static void NestedCallback(void* this_ptr, std::string key, nlohmann::json settings);
};
