/*---------------------------------------------------------*\
| GalaxGPUv2Controller.h                                    |
|                                                           |
|   RGBController for Galax GPUs (Xtreme Tuner)             |
|                                                           |
|   Daniel Stuart  (daniel.stuart14)            26 may 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char galax_gpu_dev_id;

enum
{
    GALAX_V2_RED_REGISTER                      = 0x02, /* Red Register */
    GALAX_V2_GREEN_REGISTER                    = 0x03, /* Green Register */
    GALAX_V2_BLUE_REGISTER                     = 0x04, /* Blue Register */
    GALAX_V2_SPEED_REGISTER_A                  = 0x21, /* Speed Register A */
    GALAX_V2_SPEED_REGISTER_B                  = 0x22, /* Speed Register B */
    GALAX_V2_SYNC_REGISTER                     = 0x27, /* Sync Register */
    GALAX_V2_BRIGHTNESS_REGISTER               = 0x2D, /* Brightness Register */
    GALAX_V2_MODE_REGISTER                     = 0x30, /* Mode Register */
    GALAX_V2_SAVE_REGISTER                     = 0x40, /* Save Register */
};

enum
{
    GALAX_V2_MODE_STATIC_VALUE                 = 0x01, /* Static Mode */
    GALAX_V2_MODE_BREATHING_VALUE              = 0x02, /* Breathing Mode */
    GALAX_V2_MODE_RAINBOW_VALUE                = 0x16, /* Rainbow Mode */
    GALAX_V2_MODE_OFF_VALUE                    = 0x19, /* Off Mode */
};

enum
{
    GALAX_V2_SYNC_OFF                          = 0x00, /* Sync Off */
    GALAX_V2_SYNC_ON                           = 0x01, /* Sync On */
};

enum
{
    GALAX_V2_SAVE_VALUE                        = 0x5A, /* Save Value */
};

class GalaxGPUv2Controller
{
public:
    GalaxGPUv2Controller(i2c_smbus_interface* bus, galax_gpu_dev_id, std::string dev_name);
    ~GalaxGPUv2Controller();

    std::string   GetDeviceLocation();
    std::string   GetDeviceName();
    unsigned char GetLEDRed();
    unsigned char GetLEDGreen();
    unsigned char GetLEDBlue();
    unsigned char GetMode();
    unsigned char GetSync();
    unsigned char GetSpeed();
    unsigned char GetBrightness();
    void          SetLEDColors(unsigned char red, unsigned char green, unsigned char blue);
    void          SetMode(unsigned char value);
    void          SetSync(unsigned char value);
    void          SetSpeed(unsigned char value);
    void          SetBrightness(unsigned char value);
    void          SaveMode();

    unsigned char GalaxGPURegisterRead(unsigned char reg);
    void          GalaxGPURegisterWrite(unsigned char reg, unsigned char val);

    bool          direct = false;                                                // Temporary solution to check if we are in "Direct" mode

private:
    i2c_smbus_interface *   bus;
    galax_gpu_dev_id        dev;
    std::string             name;
};
