/*---------------------------------------------------------*\
| PhilipsHueSettingsHandler.h                               |
|                                                           |
|   Settings Handler for Philips Hue                        |
|   Due to conflict in jsoh.hpp library, hueplusplus and    |
|   SettingsManager should not be included in the same file |
|   so handle settings in a separate class.                 |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      17 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstddef>
#include <string>

class PhilipsHueSettingsHandler
{
public:
    PhilipsHueSettingsHandler();
    ~PhilipsHueSettingsHandler();

    std::size_t     GetBridgeCount();

    std::string     GetBridgeIP(unsigned int bridge_idx);
    std::string     GetBridgeMAC(unsigned int bridge_idx);
    std::string     GetBridgeUsername(unsigned int bridge_idx);
    std::string     GetBridgeClientKey(unsigned int bridge_idx);
    bool            GetBridgeAutoconnect(unsigned int bridge_idx);
    bool            GetBridgeUseEntertainment(unsigned int bridge_idx);

    bool            BridgeHasUsername(unsigned int bridge_idx);
    bool            BridgeHasClientKey(unsigned int bridge_idx);

    void            SetBridgeUsername(unsigned int bridge_idx, std::string username);
    void            SetBridgeClientKey(unsigned int bridge_idx, std::string clientkey);
    void            SetBridgeAutoconnect(unsigned int bridge_ip, bool auto_connect);
    void            SetBridgeUseEntertainment(unsigned int bridge_idx, bool use_entertainment);

    void            SaveSettings();

private:
    void *          hue_settings;
};
