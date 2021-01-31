/*-----------------------------------------*\
|  ZalmanZSyncController.h                  |
|                                           |
|  Definitions and types for Zalman Z Sync  |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/30/2021   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    ZALMAN_Z_SYNC_COLOR_CHANNEL_RED     = 0x00,
    ZALMAN_Z_SYNC_COLOR_CHANNEL_GREEN   = 0x01,
    ZALMAN_Z_SYNC_COLOR_CHANNEL_BLUE    = 0x02,
};

class ZalmanZSyncController
{
public:
    ZalmanZSyncController(hid_device* dev_handle, const char* path);
    ~ZalmanZSyncController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void        SetLEDs(unsigned char channel, RGBColor* colors);

private:
    hid_device*             dev;
    std::string             location;

    void        SendApply();
    void        SendDirectFrame(unsigned char channel, unsigned char color_channel, RGBColor* colors);
};
