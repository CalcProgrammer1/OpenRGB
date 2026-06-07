/*---------------------------------------------------------*\
| RGBController_ZotacBlackwellGPU.h                         |
|                                                           |
|   RGBController for ZOTAC Blackwell (RTX 50 series) GPU   |
|                                                           |
|   Eder Sánchez                                27 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include "RGBController.h"
#include "ZotacBlackwellGPUController.h"

class RGBController_ZotacBlackwellGPU : public RGBController
{
public:
    RGBController_ZotacBlackwellGPU(ZotacBlackwellGPUController* controller_ptr,
                                    uint16_t device, uint16_t subdevice);
    ~RGBController_ZotacBlackwellGPU();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();
    void DeviceUpdateZone(int zone);

private:
    ZotacBlackwellGPUController* controller;
    std::vector<std::string>     zone_names;

    struct DeviceZoneConfig
    {
        uint16_t device;
        uint16_t subdevice;
        const char* zones[4];
        uint8_t     zone_count;
    };

    static const DeviceZoneConfig device_zone_configs[];

    static const DeviceZoneConfig* FindZoneConfig(uint16_t device, uint16_t subdevice);
};
