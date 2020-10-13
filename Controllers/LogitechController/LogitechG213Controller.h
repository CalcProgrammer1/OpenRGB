/*-----------------------------------------*\
|  LogitechG213Controller.h                 |
|                                           |
|  Generic RGB Interface for Logitech G213  |
|                                           |
|  Eric Samuelson (edbgon) 10/06/2020       |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class LogitechG213Controller
{
public:
    LogitechG213Controller(hid_device* dev_handle);
    ~LogitechG213Controller();

    void        SetDirect
                    (
                    unsigned char       zone,
                    unsigned char       r,
                    unsigned char       g,
                    unsigned char       b
                    );

private:
    hid_device* dev;

};
