/*-----------------------------------------*\
|  LexipMouseController.h                   |
|                                           |
|  Driver for Lexip mouse lighting          |
|  controller - header file                 |
|                                           |
|  Guimard Morgan (morg) 2/21/2022          |
\*-----------------------------------------*/
#pragma once

#include "RGBController.h"
#include <string>
#include <hidapi/hidapi.h>

#define PACKET_DATA_LENGTH 64

class LexipMouseController
{
public:
    LexipMouseController(hid_device* dev_handle, const hid_device_info& info);
    ~LexipMouseController();

    std::string                 GetSerialString();
    std::string                 GetDeviceLocation();
    std::string                 GetFirmwareVersion();

    void                        SetDirect(RGBColor color);
protected:
    hid_device* dev;

private:
    std::string                 location;
    std::string                 serial_number;
    std::string                 version;
};
