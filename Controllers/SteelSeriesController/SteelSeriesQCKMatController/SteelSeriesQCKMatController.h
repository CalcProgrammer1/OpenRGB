/*-----------------------------------------*\
|  SteelSeriesQCKControllerMat.h            |
|                                           |
|  Generic RGB Interface SteelSeriesQCK     |
|  Class                                    |
|                                           |
|  Edbgon 22/05/2021                        |
\*-----------------------------------------*/

#include "RGBController.h"
#include <string>
#include <hidapi/hidapi.h>

#pragma once

class SteelSeriesQCKMatController
{
public:
    SteelSeriesQCKMatController(hid_device* dev_handle, const char* path);

    ~SteelSeriesQCKMatController();

    std::string GetDeviceLocation();
    char*       GetDeviceName();
    std::string GetSerialString();

    void SetColors(std::vector<RGBColor> colors);

private:
    char                    device_name[32];
    hid_device*             dev;
    std::string             location;
};
