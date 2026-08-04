/*---------------------------------------------------------*\
| EdifierHaloController.h                                   |
|                                                           |
|   Driver for Edifier Halo series USB/HID devices          |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  20 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <mutex>
#include <string>
#include <hidapi.h>

enum
{
    EDIFIER_HALO_AMBIENT_MODE_BREATHING = 1, // Breathing
    EDIFIER_HALO_AMBIENT_MODE_TIDE      = 2, // Rainbow
    EDIFIER_HALO_AMBIENT_MODE_STATIC    = 3, // Static
    EDIFIER_HALO_AMBIENT_MODE_RIPPLE    = 4, // Glittering and colorful
    EDIFIER_HALO_AMBIENT_MODE_FLOW      = 5, // Light-flow
    EDIFIER_HALO_AMBIENT_MODE_DYNAMIC   = 6  // Dynamic Shadows
};

enum
{
    EDIFIER_HALO_AMBIENT_SPEED_SLOWEST  = 1,
    EDIFIER_HALO_AMBIENT_SPEED_FASTEST  = 10
};

enum
{
    EDIFIER_HALO_AMBIENT_BRIGHTNESS_LOWEST  = 1,
    EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST = 3
};

class EdifierHaloController
{
public:
    EdifierHaloController(hid_device* dev_handle, const char* path);
    ~EdifierHaloController();

    std::string             GetDeviceName() const;
    std::string             GetSerial() const;
    std::string             GetLocation() const;

    void                    SetAmbientLight(unsigned char effect, unsigned char r, unsigned char g, unsigned char b, unsigned char brightness, unsigned char speed);
    void                    SetAmbientLightPower(bool enabled);
    void                    SetPixelScreenColor(unsigned char r, unsigned char g, unsigned char b);
    void                    SetScreenUIModel(unsigned char model_id, unsigned char r, unsigned char g, unsigned char b);
    void                    SetScreenScene(unsigned char category, unsigned char index, unsigned char r, unsigned char g, unsigned char b);

private:
    hid_device*             dev;
    std::string             device_name;
    std::string             serial;
    std::string             location;
    std::mutex              device_mutex;

    void                    SendEdifierPacket(unsigned char commandIndex, const unsigned char* payload, unsigned int payload_size);
};
