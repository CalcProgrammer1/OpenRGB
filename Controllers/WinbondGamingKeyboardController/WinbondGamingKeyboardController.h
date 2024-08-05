/*---------------------------------------------------------*\
| WinbondGamingKeyboardController.h                         |
|                                                           |
|   Driver for Winbond Gaming Keyboard                      |
|                                                           |
|   Daniel Gibson                               03 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"
#include "KeyboardLayoutManager.h"

class WinbondGamingKeyboardController
{
    void SetNameVendorDescription(const hid_device_info &info, const std::string& name);
    void SetVersionLayout();
    void ParseVersionString(const char* str);

public:
    WinbondGamingKeyboardController(hid_device *dev_handle, const hid_device_info &info, const std::string& name);

    std::string GetSerialString();

    const char* GetDeviceLocation() const
    {
        return location.c_str();
    }

    const char* GetName() const
    {
        return name.c_str();
    }

    const char* GetVendor() const
    {
        return vendor.c_str();
    }

    const char* GetDescription() const
    {
        return description.c_str();
    }

    const char* GetVersion() const
    {
        return version.c_str();
    }

    KEYBOARD_LAYOUT GetLayout() const
    {
        return layout;
    }

    bool HasLogoLight() const
    {
        return hasLogoLight;
    }

    KEYBOARD_SIZE GetSize() const
    {
        return kb_size;
    }

    void SetLEDsData(const std::vector<RGBColor>& colors, const std::vector<led>& leds, int brightness);

    void SetMode(const mode& m);

protected:
    hid_device*   dev = nullptr;

private:
    std::string   location;
    std::string   name;
    std::string   description;
    std::string   vendor;
    std::string   version;
    KEYBOARD_LAYOUT layout = KEYBOARD_LAYOUT_ISO_QWERTY; // default to ISO so most keys can be configured
    KEYBOARD_SIZE kb_size = KEYBOARD_SIZE_FULL;
    bool          hasLogoLight = false;
};
