/*-----------------------------------------*\
|  NvidiaESAController.h                    |
|                                           |
|  Driver for Nvidia ESA lighting           |
|  controller - header file                 |
|                                           |
|  Guimard Morgan (morg) 2/18/2022          |
\*-----------------------------------------*/
#pragma once

#include "RGBController.h"
#include <string>
#include <hidapi/hidapi.h>


class NvidiaESAController
{
public:
    NvidiaESAController(hid_device* dev_handle, const hid_device_info& info);
    ~NvidiaESAController();

    std::string                 GetSerialString();
    std::string                 GetDeviceLocation();
    std::string                 GetFirmwareVersion();

    void                        SetZoneColor(unsigned int zone_idx, RGBColor color);

protected:
    hid_device* dev;

private:
    std::string                 location;
    std::string                 serial_number;
    std::string                 version;
};
