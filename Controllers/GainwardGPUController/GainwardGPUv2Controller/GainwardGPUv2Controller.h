/*---------------------------------------------------------*\
| GainwardGPUv2Controller.h                                 |
|                                                           |
|   Driver for Gainward v2 GPU                              |
|                                                           |
|   KundaPanda                                  04 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char gainward_gpu_dev_id;

/*---------------------------------------------------------------------------------*\
 | Newer Gainward models seem to use addresses very similar to those used by EVGA   |
 \*-------------------------------------------------------------------------------**/

enum
{
    /* RGB Registers */
    GAINWARD_V2_RED_REGISTER                        = 0x6C, /* Red Register */
    GAINWARD_V2_GREEN_REGISTER                      = 0x6D, /* Green Register */
    GAINWARD_V2_BLUE_REGISTER                       = 0x6E, /* Blue Register */
    GAINWARD_V2_RED_SECONDARY_REGISTER              = 0x70, /* Red Register 2 */
    GAINWARD_V2_GREEN_SECONDARY_REGISTER            = 0x71, /* Green Register 2 */
    GAINWARD_V2_BLUE_SECONDARY_REGISTER             = 0x72, /* Blue Register 2 */
    GAINWARD_V2_RED_TERTIARY_REGISTER               = 0xE4, /* Red Register 3 */
    GAINWARD_V2_GREEN_TERTIARY_REGISTER             = 0xE5, /* Green Register 3 */
    GAINWARD_V2_BLUE_TERTIARY_REGISTER              = 0xE6, /* Blue Register 3 */
    GAINWARD_V2_MODE_REGISTER                       = 0xE0, /* Mode Register */
    /* Direct mode switch register
     * 0x1 Software control
     * 0x22 Breathing */
    GAINWARD_V2_STATIC_CONTROL_REGISTER             = 0x60,
    GAINWARD_V2_BREATHE_SPEED_REGISTER_A            = 0x62, /* Lower part of speed control for breathe effect */
    GAINWARD_V2_BREATHE_SPEED_REGISTER_B            = 0x63, /* Upper part of speed control for breathe effect */
    GAINWARD_V2_BREATHE_SPEED_SECONDARY_REGISTER_A  = 0x64, /* Lower part of speed control for breathe effect - prob. secondary color */
    GAINWARD_V2_BREATHE_SPEED_SECONDARY_REGISTER_B  = 0x65, /* Upper part of speed control for breathe effect - prob. secondary color */
    /* Mode direction Register
     * 0x0
     * 0x1 */
    GAINWARD_V2_MODE_DIRECTION_REGISTER             = 0xE1,
    /* Mode speed Register
     * 0x0 MAX
     * 0xF MIN */
    GAINWARD_V2_SPEED_REGISTER                      = 0xE2,
};

enum
{
    /* Manual color selection using primary registers */
    GAINWARD_V2_MODE_STATIC                         = 0x00,
    /* Rainbow cycling with direction and speed controls */
    GAINWARD_V2_MODE_CYCLE                          = 0x01,
    /* One strobing color running around the fan
     * Color using primary registers, direction and speed control */
    GAINWARD_V2_MODE_STROBE                         = 0x02,
};

enum
{
    GAINWARD_V2_COLOR_REGISTER_PRIMARY,
    GAINWARD_V2_COLOR_REGISTER_SECONDARY,
    GAINWARD_V2_COLOR_REGISTER_TERTIARY,
};

enum
{
    /* Software controlled direct mode */
    GAINWARD_V2_STATIC_SOFTWARE                     = 0x01,
    /* GPU controlled direct mode with breathing effect */
    GAINWARD_V2_STATIC_BREATHING                    = 0x22,
};

class GainwardGPUv2Controller
{
public:
    GainwardGPUv2Controller(i2c_smbus_interface* bus, gainward_gpu_dev_id, std::string dev_name);
    ~GainwardGPUv2Controller();

    std::string   GetDeviceLocation();
    std::string   GetDeviceName();

    unsigned char GetLEDRed();
    unsigned char GetLEDGreen();
    unsigned char GetLEDBlue();
    void          SetLEDColors(unsigned char red, unsigned char green, unsigned char blue, unsigned char color_register = GAINWARD_V2_COLOR_REGISTER_PRIMARY);
    void          SetMode(unsigned char mode, unsigned char speed, unsigned char direct_mode = GAINWARD_V2_STATIC_SOFTWARE);
    void          SetBreathingSpeed(unsigned int speed);
    void          SetDirection(unsigned char direction);

private:
    i2c_smbus_interface *   bus;
    gainward_gpu_dev_id     dev;
    std::string             name;
};
