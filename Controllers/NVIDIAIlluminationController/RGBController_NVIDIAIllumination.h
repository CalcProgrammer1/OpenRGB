/*-----------------------------------------*\
|  RGBController_NVIDIAIllumination.h       |
|                                           |
|  NVIDIA Illumination Generic RGB Interface|
|  for OpenRGB                              |
|                                           |
|  Carter Miller (GingerRunner) 1/4/2022    |
\*-----------------------------------------*/


#pragma once

#include "RGBController.h"
#include "NVIDIAIlluminationV1Controller.h"
#include <map>

#define NVIDIA_FOUNDERS_V1_CONTROLLER_NAME   "NVIDIA_FOUNDERS_V1"

class RGBController_NVIDIAIlluminationV1 : public RGBController
{
    public:
        RGBController_NVIDIAIlluminationV1(NVIDIAIlluminationV1Controller* nvidia_founders_ptr);
        ~RGBController_NVIDIAIlluminationV1();

        void UpdateSingleLED(int led);

        void SetupZones();

        void DeviceUpdateLEDs();

        void UpdateZoneLEDs(int);

        void SetCustomMode();

        void DeviceUpdateMode();

        void ResizeZone(int, int);

    private:
        uint8_t getModeIndex(uint8_t mode_value);
        NVIDIAIlluminationV1Controller* controller;
        std::vector<uint8_t> zoneIndexMap;
        std::vector<NV_GPU_CLIENT_ILLUM_ZONE_TYPE> zoneTypes;
        std::map<NV_GPU_CLIENT_ILLUM_ZONE_TYPE, const char *> nvidia_illum_zone_names;
};