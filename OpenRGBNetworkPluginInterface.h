/*---------------------------------------------------------*\
| OpenRGBNetworkPluginInterface.h                           |
|                                                           |
|   OpenRGB Plugin Network API                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                24 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>

typedef unsigned char* (*OpenRGBNetworkPluginCallback)(void *, unsigned int, unsigned char*, unsigned int*);

struct OpenRGBNetworkPlugin
{
    /*-------------------------------------------------------------------------------------------------*\
    | Network Plugin Information                                                                        |
    \*-------------------------------------------------------------------------------------------------*/
    unsigned int                    protocol_version;
    std::string                     name;
    std::string                     description;
    std::string                     version;

    /*-------------------------------------------------------------------------------------------------*\
    | Network Plugin Functionality                                                                      |
    \*-------------------------------------------------------------------------------------------------*/
    OpenRGBNetworkPluginCallback    callback;
    void*                           callback_arg;
};
