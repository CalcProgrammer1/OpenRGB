/*---------------------------------------------------------*\
| AutoStart-FreeBSD.h                                       |
|                                                           |
|   Autostart implementation for FreeBSD                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "AutoStart.h"

class AutoStart: public AutoStartInterface
{
public:
    AutoStart(std::string name);

    bool            DisableAutoStart();
    bool            EnableAutoStart(AutoStartInfo autostart_info);
    bool            IsAutoStartEnabled();
    std::string     GetExePath();

private:
    void            InitAutoStart(std::string name);
    std::string     GenerateLaunchAgentFile(AutoStartInfo autostart_info);
};
