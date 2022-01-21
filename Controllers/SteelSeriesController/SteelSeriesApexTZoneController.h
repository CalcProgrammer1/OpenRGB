/*-----------------------------------------*\
|  SteelSeriesApexTZoneController.h         |
|                                           |
|  Edbgon 06.10.21                          |
\*-----------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>

#include "SteelSeriesGeneric.h"
#include "SteelSeriesApexBaseController.h"

#pragma once

#define STEELSERIES_TZ_WRITE_PACKET_SIZE    33
#define STEELSERIES_TZ_BRIGHTNESS_MIN       0x00
#define STEELSERIES_TZ_BRIGHTNESS_MAX       0x64

class SteelSeriesApexTZoneController
{
public:
    SteelSeriesApexTZoneController
        (
        hid_device*         dev_handle,
        steelseries_type    proto_type,
        const char*         path
        );

    ~SteelSeriesApexTZoneController();

    std::string GetDeviceLocation();
    char*       GetDeviceName();
    std::string GetSerialString();
    steelseries_type GetKeyboardType();

    void SetColor(std::vector<RGBColor> colors, unsigned char brightness);
    void Save();

private:
    char                    device_name[32];
    hid_device*             dev;
    std::string             location;
    steelseries_type        proto;
};
