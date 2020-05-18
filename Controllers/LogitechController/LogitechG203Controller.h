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

class LogitechG203Controller
{
public:
    LogitechG203Controller(hid_device* dev_handle);
    ~LogitechG203Controller();

    void        SendMouseMode
                   (
                   unsigned char       mode,
                   unsigned char       speed,
                   unsigned char       red,
                   unsigned char       green,
                   unsigned char       blue
                   );

private:
    hid_device*             dev;
};
