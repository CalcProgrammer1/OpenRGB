/*-----------------------------------------*\
|  AsusAuraMouseController.h                |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/23/2020  |
\*-----------------------------------------*/

#include "RGBController.h"
#include "AsusAuraMouseDevices.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

#define ASUS_AURA_MOUSE_PACKET_SIZE 65
#define HID_MAX_STR 255

class AuraMouseController
{
public:
    AuraMouseController(hid_device* dev_handle, const char* path, uint16_t pid);
    virtual ~AuraMouseController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SaveMode();
    void SendUpdate
        (
        unsigned char   zone,
        unsigned char   mode,
        unsigned char   red,
        unsigned char   grn,
        unsigned char   blu,
        unsigned char   dir,
        bool            random,
        unsigned char   speed,
        unsigned char   brightness
	);

    uint16_t                    device_pid;

private:
    hid_device*                 dev;
    std::string                 location;
};
