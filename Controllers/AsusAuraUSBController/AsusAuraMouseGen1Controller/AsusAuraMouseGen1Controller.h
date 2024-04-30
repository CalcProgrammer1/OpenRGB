/*-----------------------------------------*\
|  AsusAuraMouseGen1Controller.h            |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Mola19 11/30/2021                        |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

#define HID_MAX_STR 255

class AsusAuraMouseGen1Controller
{
public:
    AsusAuraMouseGen1Controller(hid_device* dev_handle, const char* path, uint16_t pid);
    virtual ~AsusAuraMouseGen1Controller();

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
    void SendDirectSpatha(std::vector<RGBColor> colors);

    void ResetToSavedLighting();

    uint16_t                    device_pid;

private:
    hid_device*                 dev;
    std::string                 location;
};
