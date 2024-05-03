/*---------------------------------------------------------*\
| AutoStart.cpp                                             |
|                                                           |
|   Autostart common implementation                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "AutoStart.h"

std::string AutoStartInterface::GetAutoStartFile()
{
    return(autostart_file);
}

std::string AutoStartInterface::GetAutoStartName()
{
    return(autostart_name);
}
