/*-----------------------------------------*\
|  AsusAuraMonitorController.h              |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Mola19 08/03/2023                        |
\*-----------------------------------------*/

#include "RGBController.h"

#include <hidapi/hidapi.h>

#pragma once

enum
{
    AURA_ROG_STRIX_XG27AQ_PID    = 0x198C,
    AURA_ROG_STRIX_XG27W_PID     = 0x1933,
    AURA_ROG_PG32UQ_PID          = 0x19B9,
};


class AuraMonitorController
{
public:
    AuraMonitorController(hid_device* dev_handle, const char* path, uint16_t pid);
    virtual ~AuraMonitorController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void BeginUpdate();
    void UpdateLed(int led, unsigned char red, unsigned char green, unsigned char blue);
    void ApplyChanges();

    uint16_t                    device_pid;

private:
    hid_device*                 dev;
    std::string                 location;
};
