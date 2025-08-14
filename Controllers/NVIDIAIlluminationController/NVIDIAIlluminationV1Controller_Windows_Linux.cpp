/*---------------------------------------------------------*\
| NVIDIAIlluminationV1Controller_Windows_Linux.cpp          |
|                                                           |
|   Driver for NVIDIA Illumination V1 GPU                   |
|                                                           |
|   Carter Miller (GingerRunner)                05 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "NVIDIAIlluminationV1Controller_Windows_Linux.h"

NVIDIAIlluminationV1Controller::NVIDIAIlluminationV1Controller(nvapi_accessor* nvapi_ptr, bool treats_rgbw_as_rgb, std::string dev_name)
{
    nvapi               = nvapi_ptr;
    _treats_rgbw_as_rgb = treats_rgbw_as_rgb;
    name                = dev_name;
}

NVIDIAIlluminationV1Controller::~NVIDIAIlluminationV1Controller()
{

}

std::string NVIDIAIlluminationV1Controller::GetName()
{
    return(name);
}

void NVIDIAIlluminationV1Controller::checkNVAPIreturn()
{
    if (nvapi_return != NVAPI_OK)
    {
        LOG_DEBUG("NVAPI return code not NVAPI_OK: %d", nvapi_return);
    }
}

void NVIDIAIlluminationV1Controller::getControl()
{
    /*------------------------------------------------------------------------------------------------*\
    | This was previously memset(&zone_params, 0, sizeof(zone_params))                                   |
    | But this kind of zero initialization is more up-to-date and safer in the event of non-primitive  |
    | data types                                                                                       |
    \*------------------------------------------------------------------------------------------------*/
    zone_params = {};
    /*---------------------------------------------------------------------------------------------------*\
    | Hardcoded value found via sniffing, this may be different for other cards, once that is             |
    | found, may be best to simply if/else this based on the card detected or map it out in the detector  |
    | and then pass via constructor to here.                                                              |
    \*---------------------------------------------------------------------------------------------------*/
    zone_params.version  = 72012;
    zone_params.bDefault = 0;
    /*---------------------------------------------------------------------------------------------------*\
    | As far as I can tell, this pre-populates the zone type value, as well as the number of zones        |
    | able to be controlled, and their existing settings, very useful for extending this controller.      |
    \*---------------------------------------------------------------------------------------------------*/
    nvapi_return = nvapi->nvapi_zone_control(NVAPI_ZONE_GET_CONTROL, &zone_params);
    checkNVAPIreturn();
}

void NVIDIAIlluminationV1Controller::setControl()
{
    nvapi_return = nvapi->nvapi_zone_control(NVAPI_ZONE_SET_CONTROL, &zone_params);
    checkNVAPIreturn();
}

/*----------------------------------------------------------------------------------------------------*\
| This function exists to check if RGB colors are all set to zero, and if so, to take the brightness   |
| down to zero.  This was done to comply with functionality in OpenRGB such as "Lights Off" which      |
| sends RGB values of all zeroes, but doesn't seem to send a brightness of zero at this time (6/2022). |
\*----------------------------------------------------------------------------------------------------*/
bool NVIDIAIlluminationV1Controller::allZero(std::array<uint8_t, 4> colors)
{
    return colors == all_zeros;
}

void NVIDIAIlluminationV1Controller::setZoneRGBW(uint8_t zone, uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t brightness)
{
    zone_params.zones[zone].data.rgbw.data.manualRGBW.rgbwParams.colorR = red;
    zone_params.zones[zone].data.rgbw.data.manualRGBW.rgbwParams.colorG = green;
    zone_params.zones[zone].data.rgbw.data.manualRGBW.rgbwParams.colorB = blue;
    zone_params.zones[zone].data.rgbw.data.manualRGBW.rgbwParams.colorW = white;
    zone_params.zones[zone].data.rgbw.data.manualRGBW.rgbwParams.brightnessPct = brightness;
}

void NVIDIAIlluminationV1Controller::setZoneRGB(uint8_t zone, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness)
{
    zone_params.zones[zone].data.rgb.data.manualRGB.rgbParams.colorR = red;
    zone_params.zones[zone].data.rgb.data.manualRGB.rgbParams.colorG = green;
    zone_params.zones[zone].data.rgb.data.manualRGB.rgbParams.colorB = blue;
    zone_params.zones[zone].data.rgb.data.manualRGB.rgbParams.brightnessPct = brightness;
}


void NVIDIAIlluminationV1Controller::setZone(uint8_t zone, uint8_t mode, NVIDIAIllumination_Config zone_config)
{
    getControl();
    uint8_t red = RGBGetRValue(zone_config.colors[0]);
    uint8_t green = RGBGetGValue(zone_config.colors[0]);
    uint8_t blue = RGBGetBValue(zone_config.colors[0]);
    uint8_t white = 0;
    switch(mode)
    {
        case NVIDIA_ILLUMINATION_OFF:
            zone_params.zones[zone].ctrlMode = NV_GPU_CLIENT_ILLUM_CTRL_MODE_MANUAL_RGB;
            if(zone_params.zones[zone].type == NV_GPU_CLIENT_ILLUM_ZONE_TYPE_RGB)
            {
                setZoneRGB(zone, 0, 0, 0, 0);
            }
            else if(zone_params.zones[zone].type == NV_GPU_CLIENT_ILLUM_ZONE_TYPE_RGBW)
            {
                setZoneRGBW(zone, 0, 0, 0, 0, 0);
            }
            else if(zone_params.zones[zone].type == NV_GPU_CLIENT_ILLUM_ZONE_TYPE_SINGLE_COLOR)
            {
                zone_params.zones[zone].data.singleColor.data.manualSingleColor.singleColorParams.brightnessPct = 0;
            }
            else if(zone_params.zones[zone].type == NV_GPU_CLIENT_ILLUM_ZONE_TYPE_COLOR_FIXED)
            {
                zone_params.zones[zone].data.colorFixed.data.manualColorFixed.colorFixedParams.brightnessPct = 0;
            }
            break;
        case NVIDIA_ILLUMINATION_DIRECT:
            zone_params.zones[zone].ctrlMode = NV_GPU_CLIENT_ILLUM_CTRL_MODE_MANUAL_RGB;
            if(zone_params.zones[zone].type == NV_GPU_CLIENT_ILLUM_ZONE_TYPE_RGB)
            {
                setZoneRGB(zone, red, green, blue, zone_config.brightness);
            }
            else if(zone_params.zones[zone].type == NV_GPU_CLIENT_ILLUM_ZONE_TYPE_RGBW)
            {
                /*----------------------------------------------------------------------------------------------------*\
                | Certain devices seem to ignore the white value entirely, despite the zone being reported back by the |
                | API as RGBW, as such, this if statement was added to conduct a different course of action based      |
                | on definitions placed in the controller page                                                         |
                \*----------------------------------------------------------------------------------------------------*/
                if(!_treats_rgbw_as_rgb)
                {
                    uint8_t min_rgb_value = 0xFF;
                    uint8_t max_rgb_value = 0;
                    min_rgb_value = ((red < 0xFF) ? red : min_rgb_value);
                    min_rgb_value = ((green < min_rgb_value) ? green : min_rgb_value);
                    min_rgb_value = ((blue < min_rgb_value) ? blue : min_rgb_value);
                    max_rgb_value = ((red > 0) ? red : max_rgb_value);
                    max_rgb_value = ((green > max_rgb_value) ? green : max_rgb_value);
                    max_rgb_value = ((blue > max_rgb_value) ? blue : max_rgb_value);
                    /*---------------------------------------------------------------------------------------------------*\
                    | If difference between the highest and lowest RGB values is 10 or lower, set the white value only,   |
                    | zero out the rest, this logic was found via tedious examination                                     |
                    \*---------------------------------------------------------------------------------------------------*/
                    if (max_rgb_value - min_rgb_value <= 10)
                    {
                        red = 0;
                        green = 0;
                        blue = 0;
                        white = (max_rgb_value + min_rgb_value)/2;
                    }
                }
                setZoneRGBW(zone, red, green, blue, white, zone_config.brightness);
            }
            else if(zone_params.zones[zone].type == NV_GPU_CLIENT_ILLUM_ZONE_TYPE_SINGLE_COLOR)
            {
                zone_params.zones[zone].data.singleColor.data.manualSingleColor.singleColorParams.brightnessPct = allZero({red, green, blue, white}) ? 0 : zone_config.brightness;
            }
            else if(zone_params.zones[zone].type == NV_GPU_CLIENT_ILLUM_ZONE_TYPE_COLOR_FIXED)
            {
                zone_params.zones[zone].data.colorFixed.data.manualColorFixed.colorFixedParams.brightnessPct = allZero({red, green, blue, white}) ? 0 : zone_config.brightness;
            }
            break;
    }
    setControl();
}

int NVIDIAIlluminationV1Controller::getZoneColor(uint8_t zone_index)
{
    return ToRGBColor(zone_params.zones[zone_index].data.rgb.data.manualRGB.rgbParams.colorR,
                      zone_params.zones[zone_index].data.rgb.data.manualRGB.rgbParams.colorG,
                      zone_params.zones[zone_index].data.rgb.data.manualRGB.rgbParams.colorB);
}

std::vector<NV_GPU_CLIENT_ILLUM_ZONE_TYPE> NVIDIAIlluminationV1Controller::getInfo()
{
    std::vector<NV_GPU_CLIENT_ILLUM_ZONE_TYPE> zone_types;
    getControl();
    for(unsigned int i = 0; i < zone_params.numIllumZonesControl; i++)
    {
        zone_types.push_back(zone_params.zones[i].type);
    }
    return zone_types;
}
