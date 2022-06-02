/*-------------------------------------------*\
|  HyperXPulsefireRaidController.h            |
|                                             |
|  Definitions and types for HyperX           |
|  Pulsefire Raid lighting controller         |
|                                             |
|  Morgan Guimard (morg) 04/06/2022           |
\*-------------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

#define HYPERX_PULSFIRE_RAID_PACKET_DATA_LENGTH         264
#define HYPERX_PULSFIRE_RAID_REPORT_ID                  0x07
#define HYPERX_PULSFIRE_RAID_LEDS_COUNT                 2
#define HYPERX_PULSFIRE_RAID_DIRECT_MODE_START_PACKET   0x0A
#define HYPERX_PULSFIRE_RAID_DIRECT_MODE_END_PACKET     0xA0

enum
{
    HYPERX_PULSFIRE_RAID_BRIGHTNESS_MIN = 0x00,
    HYPERX_PULSFIRE_RAID_BRIGHTNESS_MAX = 0x64
};

class HyperXPulsefireRaidController
{
public:
    HyperXPulsefireRaidController(hid_device* dev_handle, const hid_device_info& info);
    ~HyperXPulsefireRaidController();    

    std::string GetSerialString();
    std::string GetDeviceLocation();
    std::string GetFirmwareVersion();
    void        SendColors(std::vector<RGBColor> colors);
    void        SetBrightness(unsigned char brightness);

protected:
    hid_device* dev;

private:
    std::string location;
    std::string serial_number;
    std::string version;

    void        Send(unsigned char* packet);
};
