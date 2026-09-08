/*---------------------------------------------------------*\
| PalitGPUv2Controller.h                                    |
|                                                           |
|   Driver for Palit v2 GPU (merged from Gainward v2 and    |
|   PNY controllers)                                        |
|                                                           |
|   KundaPanda                                  04 Jan 2021 |
|   KendallMorgan                               17 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char palit_gpu_dev_id;

enum
{
    /* Control Registers */
    PALIT_V2_CONTROL_REGISTER                    = 0x60, /* Control Register (direct mode / software control) */

    /* Breathing speed registers */
    PALIT_V2_BREATHE_SPEED_REGISTER_A            = 0x62, /* Lower part of speed control for breathe effect */
    PALIT_V2_BREATHE_SPEED_REGISTER_B            = 0x63, /* Upper part of speed control for breathe effect */
    PALIT_V2_BREATHE_SPEED_SECONDARY_REGISTER_A  = 0x64, /* Lower part of speed control for breathe effect - prob. secondary color */
    PALIT_V2_BREATHE_SPEED_SECONDARY_REGISTER_B  = 0x65, /* Upper part of speed control for breathe effect - prob. secondary color */

    /* RGB Registers - Primary color */
    PALIT_V2_RED_REGISTER                        = 0x6C, /* Red Register */
    PALIT_V2_GREEN_REGISTER                      = 0x6D, /* Green Register */
    PALIT_V2_BLUE_REGISTER                       = 0x6E, /* Blue Register */
    PALIT_V2_BRIGHTNESS_REGISTER                 = 0x6F, /* Brightness Register */

    /* RGB Registers - Secondary color */
    PALIT_V2_RED_SECONDARY_REGISTER              = 0x70, /* Red Register 2 */
    PALIT_V2_GREEN_SECONDARY_REGISTER            = 0x71, /* Green Register 2 */
    PALIT_V2_BLUE_SECONDARY_REGISTER             = 0x72, /* Blue Register 2 */

    /* Mode Registers */
    PALIT_V2_MODE_REGISTER                       = 0xE0, /* Mode Register */
    PALIT_V2_MODE_DIRECTION_REGISTER             = 0xE1, /* Mode direction Register */
    PALIT_V2_SPEED_REGISTER                      = 0xE2, /* Mode speed Register */

    /* RGB Registers - Tertiary color */
    PALIT_V2_RED_TERTIARY_REGISTER               = 0xE4, /* Red Register 3 */
    PALIT_V2_GREEN_TERTIARY_REGISTER             = 0xE5, /* Green Register 3 */
    PALIT_V2_BLUE_TERTIARY_REGISTER              = 0xE6, /* Blue Register 3 */
};

/* Mode values for the hardware mode register */
enum
{
    PALIT_V2_MODE_OFF                            = 0x00,
    PALIT_V2_MODE_STATIC                         = 0x00,
    PALIT_V2_MODE_CYCLE                          = 0x01,
    PALIT_V2_MODE_STROBE                         = 0x02,
};

/* Mode identifiers for RGBController (unique values for switch statements) */
enum
{
    PALIT_V2_MODE_ID_OFF                         = 0,
    PALIT_V2_MODE_ID_DIRECT                      = 1,
    PALIT_V2_MODE_ID_BREATHING                   = 2,
    PALIT_V2_MODE_ID_CYCLE                       = 3,
    PALIT_V2_MODE_ID_STROBE                      = 4,
};

enum
{
    PALIT_V2_COLOR_REGISTER_PRIMARY,
    PALIT_V2_COLOR_REGISTER_SECONDARY,
    PALIT_V2_COLOR_REGISTER_TERTIARY,
};

enum
{
    /* Software controlled direct mode */
    PALIT_V2_STATIC_SOFTWARE                     = 0x01,
    /* GPU controlled direct mode with breathing effect */
    PALIT_V2_STATIC_BREATHING                    = 0x22,
};

class PalitGPUv2Controller
{
public:
    PalitGPUv2Controller(i2c_smbus_interface* bus, palit_gpu_dev_id, std::string dev_name);
    ~PalitGPUv2Controller();

    std::string   GetDeviceLocation();
    std::string   GetDeviceName();

    unsigned char GetLEDRed();
    unsigned char GetLEDGreen();
    unsigned char GetLEDBlue();
    void          SetLEDColors(unsigned char red, unsigned char green, unsigned char blue, unsigned char color_register = PALIT_V2_COLOR_REGISTER_PRIMARY);
    void          SetMode(unsigned char mode, unsigned char speed, unsigned char direct_mode = PALIT_V2_STATIC_SOFTWARE);
    void          SetBreathingSpeed(unsigned int speed);
    void          SetDirection(unsigned char direction);
    void          SetOff();
    void          SetCycle(unsigned char speed);
    void          SetStrobe(unsigned char r, unsigned char g, unsigned char b, unsigned char speed, unsigned char brightness);
    void          SetDirect(unsigned char red, unsigned char green, unsigned char blue, unsigned char brightness);

private:
    i2c_smbus_interface *   bus;
    palit_gpu_dev_id        dev;
    std::string             name;

    void            WriteI2CData(u8 command, u8 length, u8* data);
};
