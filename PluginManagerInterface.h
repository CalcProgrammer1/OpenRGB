/*---------------------------------------------------------*\
| PluginManagerInterface.h                                  |
|                                                           |
|   OpenRGB plugin manager interface                        |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      Nov 18 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>

class PluginManagerInterface
{
public:
    virtual unsigned int    GetPluginCount()                                    = 0;
    virtual std::string     GetPluginDescription(unsigned int plugin_idx)       = 0;
    virtual std::string     GetPluginName(unsigned int plugin_idx)              = 0;
    virtual unsigned int    GetPluginProtocolVersion(unsigned int plugin_idx)   = 0;
    virtual std::string     GetPluginVersion(unsigned int plugin_idx)           = 0;

    virtual void            LoadPlugins()                                       = 0;
    virtual void            UnloadPlugins()                                     = 0;

    virtual unsigned char * OnSDKCommand(unsigned int plugin_idx, unsigned int pkt_id, unsigned char * pkt_data, unsigned int * pkt_size) = 0;
};
