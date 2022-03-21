/*-----------------------------------------*\
|  SteelSeriesArctis5Controller.h           |
|                                           |
|  Definitions and types for SteelSeries    |
|  Arctis 5 lighting controller             |
|                                           |
|  Morgan Guimard 3/4/2022                  |
\*-----------------------------------------*/
#pragma once

#define ARCTIS_5_REPORT_SIZE 37
#define ARCTIS_5_REPORT_ID 0x06

#include "RGBController.h"
#include <string>
#include <hidapi/hidapi.h>

class SteelSeriesArctis5Controller
{
public:
    SteelSeriesArctis5Controller(hid_device* dev_handle, const hid_device_info& info);

    ~SteelSeriesArctis5Controller();

    std::string         GetDeviceLocation();
    std::string         GetSerialString();
    std::string         GetFirmwareVersion();
    void                SetColor(unsigned char zone_id, RGBColor color);

private:    
    std::string         location;
    std::string         serial_number;
    std::string         version;

    hid_device*         dev;
};
