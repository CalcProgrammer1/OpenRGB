/*-----------------------------------------*\
|  LogitechG403Controller.h                 |
|                                           |
|  Definitions and types for Logitech G403  |
|  Prodigy mouse lighting controller        |
|                                           |
|  Martin Hartl (inlart) 5/19/2020          |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    LOGITECH_G403_MODE_OFF               = 0x00,
    LOGITECH_G403_MODE_STATIC            = 0x01,
    LOGITECH_G403_MODE_CYCLE             = 0x02,
    LOGITECH_G403_MODE_BREATHING         = 0x03,
};

/*---------------------------------------------------------------------------------------------*\
| Speed is 1000 for fast and 20000 for slow.                                                    |
| Values are multiplied by 100 later to give lots of GUI steps.                                 |
\*---------------------------------------------------------------------------------------------*/
enum
{
    LOGITECH_G403_SPEED_SLOWEST          = 0xC8,    /* Slowest speed                           */
    LOGITECH_G403_SPEED_NORMAL           = 0x32,    /* Normal speed                            */
    LOGITECH_G403_SPEED_FASTEST          = 0x0A,    /* Fastest speed                           */
};

class LogitechG403Controller
{
public:
    LogitechG403Controller(hid_device* dev_handle, const char* path);
    ~LogitechG403Controller();

    std::string GetDeviceLocation();

    void        SendMouseMode
                   (
                    unsigned char       mode,
                    unsigned short      speed,
                    unsigned char       channel,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                   );

private:
    hid_device*             dev;
    std::string             location;
};
