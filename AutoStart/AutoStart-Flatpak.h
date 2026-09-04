/*---------------------------------------------------------*\
| AutoStart-Flatpak.h                                       |
|                                                           |
|   Autostart implementation for Linux via Flatpak Portal   |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      04 Sep 2026 |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "AutoStart.h"

class AutoStart : public AutoStartInterface
{
public:
    AutoStart(std::string name);

    bool        DisableAutoStart();
    bool        EnableAutoStart(AutoStartInfo autostart_info);
    bool        IsAutoStartEnabled();
    std::string GetExePath();

private:
    void        InitAutoStart(std::string name);
    bool        CallBackgroundPortal(bool enable, const AutoStartInfo& autostart_info = {});
    
    std::string app_name;
};
