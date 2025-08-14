/*---------------------------------------------------------*\
| NVIDIAIlluminationV1Controller_Windows_Linux.h            |
|                                                           |
|   Driver for NVIDIA Illumination V1 GPU                   |
|                                                           |
|   Carter Miller (GingerRunner)                04 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstring>
#include <string>
#include <vector>
#include "nvapi_accessor_Windows_Linux.h"
#include "RGBController.h"
#include "LogManager.h"

#define NVIDIA_ILLUMINATION_V1_CONTROLLER_NAME   "NVIDIA_ILLUMINATION_V1"
#define NVAPI_OK                                 0

struct NVIDIAIllumination_Config
{
    uint8_t brightness;
    RGBColor colors[7];
};

enum
{
    NVIDIA_ILLUMINATION_OFF = 0,
    NVIDIA_ILLUMINATION_DIRECT = 1
};

class NVIDIAIlluminationV1Controller
{
public:
    NVIDIAIlluminationV1Controller(nvapi_accessor* nvapi_ptr, bool treats_rgbw_as_rgb, std::string dev_name);
    ~NVIDIAIlluminationV1Controller();

    std::string GetName();

    void        getControl();
    void        setControl();
    bool        allZero(std::array<uint8_t, 4> colors);
    void        setZoneRGBW(uint8_t zone, uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t brightness);
    void        setZoneRGB(uint8_t zone, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);
    void        setZone(uint8_t zone, uint8_t mode, NVIDIAIllumination_Config zone_config);
    int         getZoneColor(uint8_t zone_index);
    std::vector<NV_GPU_CLIENT_ILLUM_ZONE_TYPE> getInfo();

private:
    nvapi_accessor*                         nvapi;
    bool                                    _treats_rgbw_as_rgb;
    NV_GPU_CLIENT_ILLUM_ZONE_CONTROL_PARAMS zone_params;
    NV_STATUS                               nvapi_return            = 0;
    const std::array<uint8_t, 4>            all_zeros               = {0, 0, 0, 0};
    std::string                             name;

    void checkNVAPIreturn();
};
