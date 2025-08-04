/*---------------------------------------------------------*\
| ColorfulTuringGPUController.h                             |
|                                                           |
|   Driver for Colorful Turing GPU                          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

typedef unsigned char colorful_gpu_dev_id;

#define COLORFUL_COLOR_PACKET_LENGTH 8
#define COLORFUL_NON_COLOR_PACKET_LENGTH 5
#define COLORFUL_MODE_PACKET_LENGTH 0x1B

enum
{
    COLORFUL_TURING_GPU_RGB_MODE_STATE_DISPLAY      = 0x013204,
    COLORFUL_TURING_GPU_RGB_MODE_OFF                = 0x00000A,
    COLORFUL_TURING_GPU_RGB_MODE_STATIC             = 0x023202,
    COLORFUL_TURING_GPU_RGB_MODE_RAINBOW            = 0x043202,
    COLORFUL_TURING_GPU_RGB_MODE_BREATHING          = 0x013202,
};

class ColorfulTuringGPUController
{
public:
    ColorfulTuringGPUController(i2c_smbus_interface* bus, colorful_gpu_dev_id dev, std::string dev_name);
    ~ColorfulTuringGPUController();

    std::string             GetDeviceLocation();
    std::string             GetDeviceName();

    int                     GetMode();
    RGBColor                GetColor();
    void                    SetDirect(RGBColor color, bool save);
    void                    SetStateDisplay(RGBColor color);
    void                    SetBreathing(RGBColor color);
    void                    SetOff();
    void                    SetRainbow();


private:
    i2c_smbus_interface *   bus;
    colorful_gpu_dev_id     dev;
    std::string             name;
};
