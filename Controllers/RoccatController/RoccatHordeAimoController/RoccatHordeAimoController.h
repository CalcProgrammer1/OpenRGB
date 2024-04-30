/*-------------------------------------------------------------------*\
|  RoccatHordeAimoController.h                                        |
|                                                                     |
|  Driver for Roccat Horde Aimo Keyboard                              |
|                                                                     |
|  Morgan Guimard (morg)   2/24/2022                                  |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <hidapi/hidapi.h>

#define WRITE_PACKET_LENGTH 24
#define READ_PACKET_LENGTH  3
#define REPORT_ID           0x18
#define NUMBER_OF_LEDS      6

class RoccatHordeAimoController
{
public:
    RoccatHordeAimoController(hid_device* dev_handle, const hid_device_info& info);
    ~RoccatHordeAimoController();

    std::string     GetSerialString();
    std::string     GetDeviceLocation();
    std::string     GetFirmwareVersion();

    void            SetColors(std::vector<RGBColor> colors);

private:
    hid_device*     dev;
    std::string     location;
    std::string     serial_number;
    std::string     version;

    void            InitialPacket();
};
