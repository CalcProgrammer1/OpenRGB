/*-----------------------------------------*\
|  SteelSeriesSiberiaController.h           |
|                                           |
|  Definitions and types for SteelSeries    |
|  Siberia lighting controller              |
|                                           |
|  E Karlsson (pilophae) 18/6/2020          |
\*-----------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class SteelSeriesSiberiaController
{
public:
    SteelSeriesSiberiaController
        (
        hid_device*         dev_handle,
        const char*         path
        );

    ~SteelSeriesSiberiaController();

    std::string GetDeviceLocation();
    char*       GetDeviceName();
    std::string GetSerialString();

    void SetColor
            (
            unsigned char   red,
            unsigned char   green,
            unsigned char   blue
            );

private:
    char                    device_name[32];
    hid_device*             dev;
    std::string             location;
};
