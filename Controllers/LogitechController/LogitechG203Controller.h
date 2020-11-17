/*-----------------------------------------*\
|  LogitechG203Controller.h                 |
|                                           |
|  Definitions and types for Logitech G203  |
|  Prodigy mouse lighting controller        |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/17/2020   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    LOGITECH_G203_MODE_OFF               = 0x00,
    LOGITECH_G203_MODE_STATIC            = 0x01,
    LOGITECH_G203_MODE_CYCLE             = 0x02,
    LOGITECH_G203_MODE_BREATHING         = 0x03,
};

/*---------------------------------------------------------------------------------------------*\
| Speed is 1000 for fast and 20000 for slow.                                                    |
| Values are multiplied by 100 later to give lots of GUI steps.                                 |
\*---------------------------------------------------------------------------------------------*/
enum
{
    LOGITECH_G203_SPEED_SLOWEST          = 0xC8,    /* Slowest speed                           */
    LOGITECH_G203_SPEED_NORMAL           = 0x32,    /* Normal speed                            */
    LOGITECH_G203_SPEED_FASTEST          = 0x0A,    /* Fastest speed                           */
};

class LogitechG203Controller
{
public:
    LogitechG203Controller(hid_device* dev_handle, const char* path);
    ~LogitechG203Controller();

    std::string GetDeviceLocation();

    void        SendMouseMode
                   (
                   unsigned char       mode,
                   unsigned short      speed,
                   unsigned char       red,
                   unsigned char       green,
                   unsigned char       blue
                   );

private:
    hid_device*             dev;
    std::string             location;
};
