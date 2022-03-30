/*-----------------------------------------*\
|  AsusAuraStrixEvolveController.h          |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Mola19 11/30/2021                        |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

#define HID_MAX_STR 255

class AuraStrixEvolveController
{
public:
    AuraStrixEvolveController(hid_device* dev_handle, const char* path, uint16_t pid);
    virtual ~AuraStrixEvolveController();

    std::string GetDeviceLocation();
    std::string GetSerialString();
    std::string GetVersion();
    int GetActiveProfile();

    void SendUpdate
        (
        unsigned char   key,
        unsigned char   value
        );

    void UpdateProfile
        (
        unsigned char   key,
        unsigned char   profile,
        unsigned char   value
        );

    void SendSavePacket();

    uint16_t                    device_pid;

private:
    hid_device*                 dev;
    std::string                 location;
};
