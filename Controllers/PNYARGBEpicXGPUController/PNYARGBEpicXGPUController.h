/*---------------------------------------------------------*\
| PNYARGBEpicXGPUController.h                               |
|                                                           |
|   Driver for PNY ARGB Epic-X GPU                          |
|                                                           |
|   Peter Berendi                               27 Apr 2025 |
|   Adam Honse <calcprogrammer1@gmail.com>      01 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

enum
{
    PNY_GPU_MODE_ARGB_OFF                    = 0x00,
    PNY_GPU_MODE_ARGB_BREATH                 = 0x0302,
    PNY_GPU_MODE_ARGB_CYCLE                  = 0x03,
    PNY_GPU_MODE_ARGB_NEON                   = 0x04,
    PNY_GPU_MODE_ARGB_EXPLOSION              = 0x05,
    PNY_GPU_MODE_ARGB_SUPERNOVA              = 0x06,
    PNY_GPU_MODE_ARGB_INFINITY               = 0x07,
    PNY_GPU_MODE_ARGB_STREAMER               = 0x08,
    PNY_GPU_MODE_ARGB_DIRECT                 = 0x09,
    PNY_GPU_MODE_ARGB_WAVE                   = 0x0A,
};

/*---------------------------------------------------------*\
| The 5070Ti only uses FRONT and ARROW, the LOGO zone is    |
| used by the 5090                                          |
\*---------------------------------------------------------*/
enum
{
    PNY_GPU_REG_ZONE_ARROW                   = 0x02,
    PNY_GPU_REG_ZONE_FRONT                   = 0x04,
    PNY_GPU_REG_ZONE_LOGO                    = 0x0F,
    PNY_GPU_REG_DETECT                       = 0x81,
};

class PNYARGBEpicXGPUController
{
public:
    PNYARGBEpicXGPUController(i2c_smbus_interface* bus, unsigned char init_i2c_addr, std::string name);
    ~PNYARGBEpicXGPUController();

	std::string             GetDeviceLocation();
    std::string             GetDeviceName();
    
    void                    SetZoneMode(unsigned char zone, unsigned char mode, unsigned char speed, unsigned char brightness, unsigned char subcmd, RGBColor color);
    void                    SetLEDDirect(unsigned char zone, unsigned char led, unsigned char mode, RGBColor color);

private:
	i2c_smbus_interface*    bus;
    unsigned char           i2c_addr;
    std::string             name;
};
