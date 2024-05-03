/*---------------------------------------------------------*\
| AutoStart.h                                               |
|                                                           |
|   Autostart common implementation                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>

struct AutoStartInfo
{
    std::string path;
    std::string args;
    std::string desc;
    std::string icon;
    std::string category;
};

class AutoStartInterface
{
public:
    virtual bool        DisableAutoStart()                              = 0;
    virtual bool        EnableAutoStart(AutoStartInfo autostart_info)   = 0;
    virtual bool        IsAutoStartEnabled()                            = 0;
    virtual std::string GetExePath()                                    = 0;

    std::string         GetAutoStartFile();
    std::string         GetAutoStartName();

protected:
    std::string         autostart_file;
    std::string         autostart_name;
};

#ifdef _WIN32
#include "AutoStart-Windows.h"
#endif

#ifdef __linux__
#include "AutoStart-Linux.h"
#endif

#ifdef __APPLE__
#include "AutoStart-MacOS.h"
#endif

#ifdef __FreeBSD__
#include "AutoStart-FreeBSD.h"
#endif
