/*-----------------------------------------*\
|  Logitech502PSController.h                |
|                                           |
|  Definitions and types for Logitech       |
|  G502 Proteus Spectrum mouse lighting     |
|  controller                               |
|                                           |
|  kernzerfall 07/28/2020                   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    LOGITECH_G502_PS_MODE_OFF               = 0x00,
    LOGITECH_G502_PS_MODE_STATIC            = 0x01,
    LOGITECH_G502_PS_MODE_CYCLE             = 0x03,
    LOGITECH_G502_PS_MODE_BREATHING         = 0x02
};

enum
{
    LOGITECH_G502_PS_SPEED_SLOWEST          = 0x00, /* Slowest speed                */
    LOGITECH_G502_PS_SPEED_SLOW             = 0x01, /* Slow speed                   */
    LOGITECH_G502_PS_SPEED_NORMAL           = 0x02, /* Normal speed                 */
    LOGITECH_G502_PS_SPEED_FAST             = 0x03, /* Fast speed                   */
    LOGITECH_G502_PS_SPEED_FASTEST          = 0x04, /* Fastest speed                */
};

class LogitechG502PSController
{
public:
    LogitechG502PSController(hid_device* dev_handle);
    ~LogitechG502PSController();

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
};
