/*---------------------------------------------------------*\
| CorsairVengeanceController.h                              |
|                                                           |
|   Driver for original single-zone Corsair Vengeance DDR4  |
|   RGB RAM                                                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 Mar 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char	corsair_dev_id;
typedef unsigned char   corsair_cmd;

enum
{
    CORSAIR_VENGEANCE_RGB_CMD_FADE_TIME = 0xA4,     /* Fade Time, 0 for Static              */
    CORSAIR_VENGEANCE_RGB_CMD_HOLD_TIME = 0xA5,     /* Hold Time                            */
    CORSAIR_VENGEANCE_RGB_CMD_MODE      = 0xA6,     /* Mode Control Value                   */
    CORSAIR_VENGEANCE_RGB_CMD_RED_VAL   = 0xB0,     /* Red Color Value                      */
    CORSAIR_VENGEANCE_RGB_CMD_GREEN_VAL = 0xB1,     /* Green Color Value                    */
    CORSAIR_VENGEANCE_RGB_CMD_BLUE_VAL  = 0xB2,     /* Blue Color Value                     */
};

enum
{
    CORSAIR_VENGEANCE_RGB_MODE_SINGLE   = 0x00,     /* Single Color Effect Mode             */
    CORSAIR_VENGEANCE_RGB_MODE_FADE     = 0x01,     /* Fade Through Colors                  */
    CORSAIR_VENGEANCE_RGB_MODE_PULSE    = 0x02,     /* Pulse Through Colors                 */
    CORSAIR_NUMBER_MODES                            /* Number of Corsair modes              */
};

class CorsairVengeanceController
{
public:
    CorsairVengeanceController(i2c_smbus_interface* bus, corsair_dev_id dev);
    ~CorsairVengeanceController();

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    unsigned int    GetLEDCount();
    void            SetMode(unsigned char mode);

    void            SetLEDColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    char                    device_name[32];
    unsigned int            led_count;
    i2c_smbus_interface *   bus;
    corsair_dev_id          dev;
};
