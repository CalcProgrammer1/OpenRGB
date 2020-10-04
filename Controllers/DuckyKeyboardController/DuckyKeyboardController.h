/*-----------------------------------------*\
|  DuckyKeyboardController.h                |
|                                           |
|  Definitions and types for Ducky RGB      |
|  keyboard lighting controller             |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/4/2020    |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class DuckyKeyboardController
{
public:
    DuckyKeyboardController(hid_device* dev_handle, const char* path);
    ~DuckyKeyboardController();

    std::string GetDeviceLocation();

    void SendColors
            (
            unsigned char*  color_data,
            unsigned int    color_data_size
            );

private:
    hid_device*             dev;
    std::string             location;

    void SendInitialize();
    void SendInitializeColorPacket();
    unsigned int SendColorDataPacket
        (
        unsigned char   packet_id,
        unsigned char*  color_data,
        unsigned int    color_size
        );
    void SendTerminateColorPacket();
};
