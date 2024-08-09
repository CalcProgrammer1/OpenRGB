/*---------------------------------------------------------*\
| RGBController_NVIDIAIllumination_Windows_Linux.h          |
|                                                           |
|   RGBController for NVIDIA Illumination GPU               |
|                                                           |
|   Carter Miller (GingerRunner)                04 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <map>
#include "RGBController.h"
#include "NVIDIAIlluminationV1Controller_Windows_Linux.h"

#define NVIDIA_FOUNDERS_V1_CONTROLLER_NAME   "NVIDIA_FOUNDERS_V1"

class RGBController_NVIDIAIlluminationV1 : public RGBController
{
    public:
        RGBController_NVIDIAIlluminationV1(NVIDIAIlluminationV1Controller* nvidia_founders_ptr);
        ~RGBController_NVIDIAIlluminationV1();

        void SetupZones();
        void ResizeZone(int zone, int new_size);

        void DeviceUpdateLEDs();
        void UpdateZoneLEDs(int zone);
        void UpdateSingleLED(int led);

        void DeviceUpdateMode();

    private:
        uint8_t getModeIndex(uint8_t mode_value);
        NVIDIAIlluminationV1Controller* controller;
        std::vector<uint8_t> zoneIndexMap;
        std::vector<NV_GPU_CLIENT_ILLUM_ZONE_TYPE> zoneTypes;
        std::map<NV_GPU_CLIENT_ILLUM_ZONE_TYPE, const char *> nvidia_illum_zone_names;
};
