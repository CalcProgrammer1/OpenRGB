/*-----------------------------------------*\
|  LogitechX56Controller.h                  |
|                                           |
|  Generic RGB Interface Logitech X56       |
|  Class                                    |
|                                           |
|  Edbgon 11/06/2021                        |
\*-----------------------------------------*/

#include "RGBController.h"
#include <string>
#include <hidapi/hidapi.h>

#pragma once

class LogitechX56Controller
{
public:
    LogitechX56Controller(hid_device* dev_handle, const char* path);

    ~LogitechX56Controller();

    std::string GetDeviceLocation();
    char*       GetDeviceName();
    std::string GetSerialString();

    void SetColor(RGBColor colors);

private:
    char                    device_name[32];
    hid_device*             dev;
    std::string             location;
};
