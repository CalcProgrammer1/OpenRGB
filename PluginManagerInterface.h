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
#include "nlohmann/json.hpp"

class PluginManagerInterface
{
public:
    /*-----------------------------------------------------*\
    | Plugin Information                                    |
    \*-----------------------------------------------------*/
    virtual unsigned int                    GetPluginCount()                                    = 0;
    virtual std::string                     GetPluginDescription(unsigned int plugin_idx)       = 0;
    virtual std::string                     GetPluginName(unsigned int plugin_idx)              = 0;
    virtual unsigned int                    GetPluginProtocolVersion(unsigned int plugin_idx)   = 0;
    virtual std::string                     GetPluginVersion(unsigned int plugin_idx)           = 0;

    /*-----------------------------------------------------*\
    | Plugin-Created RGBControllers                         |
    \*-----------------------------------------------------*/
    virtual std::vector<RGBController *>    GetRGBControllers()                                 = 0;

    /*-----------------------------------------------------*\
    | Plugin Management                                     |
    \*-----------------------------------------------------*/
    virtual void                            LoadPlugins()                                       = 0;
    virtual void                            UnloadPlugins()                                     = 0;

    /*-----------------------------------------------------*\
    | Plugin Profile Integration                            |
    \*-----------------------------------------------------*/
    virtual void                            OnProfileAboutToLoad()                              = 0;
    virtual void                            OnProfileLoad(nlohmann::json profile_data)          = 0;
    virtual nlohmann::json                  OnProfileSave()                                     = 0;

    /*-----------------------------------------------------*\
    | Plugin SDK Integration                                |
    \*-----------------------------------------------------*/
    virtual unsigned char *                 OnSDKCommand(unsigned int plugin_idx, unsigned int pkt_id, unsigned char * pkt_data, unsigned int * pkt_size) = 0;
};
