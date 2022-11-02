/*-----------------------------------------*\
|  IntelArcA770LEController.h               |
|                                           |
|  Definitions and types for Intel Arc A770 |
|  LE lighting controller                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/1/2022   |
\*-----------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>

#include "RGBController.h"

#pragma once

class IntelArcA770LEController
{
public:
    IntelArcA770LEController(hid_device* dev_handle, const char* path);
    ~IntelArcA770LEController();

    std::string GetEffectChannelString(unsigned char channel);
    std::string GetFirmwareVersionString();
    std::string GetLocationString();
    std::string GetSerialString();

    void SendDirectPacket
        (
        unsigned char   size,
        unsigned char * led_ids,
        RGBColor *      colors
        );

    void SendEnableCommand();

    void SendApplyCommand();

private:
    hid_device*             dev;
    std::string             location;
};
