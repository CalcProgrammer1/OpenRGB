/*---------------------------------------------------------*\
| OpenRGBPluginAPI.h                                        |
|                                                           |
|   Interface for OpenRGB plugins to call OpenRGB functions |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 Feb 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "LogManager.h"
#include "OpenRGBPluginInterface.h"
#include "PluginManagerInterface.h"
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

class OpenRGBPluginAPI : public OpenRGBPluginAPIInterface
{
public:
    OpenRGBPluginAPI(std::vector<RGBController> & rgb_controllers_ref);

    /*-----------------------------------------------------*\
    | LogManager APIs                                       |
    \*-----------------------------------------------------*/
    void                            append(const char* filename, int line, unsigned int level, const char* fmt, ...);

    /*-----------------------------------------------------*\
    | PluginManager APIs                                    |
    \*-----------------------------------------------------*/
    void                            RegisterRGBController(RGBController * rgb_controller);
    void                            UnregisterRGBController(RGBController * rgb_controller);

    /*-----------------------------------------------------*\
    | ProfileManager APIs                                   |
    \*-----------------------------------------------------*/
    void                            ClearActiveProfile();

    /*-----------------------------------------------------*\
    | ResourceManager APIs                                  |
    \*-----------------------------------------------------*/
    filesystem::path                GetConfigurationDirectory();
    bool                            GetDetectionEnabled();
    unsigned int                    GetDetectionPercent();
    std::string                     GetDetectionString();
    void                            WaitForDetection();
    std::vector<RGBController*> &   GetRGBControllers();

    /*-----------------------------------------------------*\
    | SettingsManager APIs                                  |
    \*-----------------------------------------------------*/
    nlohmann::json                  GetSettings(std::string settings_key);
    void                            SaveSettings();
    void                            SetSettings(std::string settings_key, nlohmann::json new_settings);

private:
    std::vector<RGBController*> &   rgb_controllers;
    LogManager *                    log_manager;
    PluginManagerInterface *        plugin_manager;
    ProfileManager *                profile_manager;
    ResourceManager *               resource_manager;
    SettingsManager *               settings_manager;
};
