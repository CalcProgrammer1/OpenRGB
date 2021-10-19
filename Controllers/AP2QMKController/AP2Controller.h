/*-----------------------------------------*\
|  AP2Controller.h                          |
|                                           |
|  Driver for AnnePro2 w/QMK keyboard       |
|                                           |
|  Sergey Gavrilov (DrZlo13) 06/06/2021     |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"

#include <string>
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

class AP2Controller
{
public:
    AP2Controller(hid_device* dev_handle, const char* path);
    ~AP2Controller();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendDirect
        (
        unsigned char       frame_count,
        unsigned char *     frame_data
        );

private:
    hid_device*                 dev;
    std::string                 location;
};
