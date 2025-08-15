/*---------------------------------------------------------*\
| RGBController_CMARGBGen2A1Controller.cpp                  |
|                                                           |
|   Driver for Cooler Master ARGB Gen 2 A1 controller       |
|                                                           |
|   Morgan Guimard (morg)                       26 Jun 2022 |
|   Fabian R (kderazorback)                     11 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_CMARGBGen2A1Controller.h"

/**------------------------------------------------------------------*\
    @name Coolermaster ARGB A1
    @category LEDStrip
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterARGBGen2A1
    @comment OpenRGB partially supports Gen 2 protocol for this device.

    Gen2 has auto-resize feature and parallel to serial magical stuff,
    Strip size is auto detected by the controller but not reported
    back to OpenRGB. Configure zones and segments for each channel
    to allow individual addressing.
    Take note that this controller is extremely slow, using fast
    update rates may introduce color artifacts.<
\*-------------------------------------------------------------------*/

RGBController_CMARGBGen2A1Controller::RGBController_CMARGBGen2A1Controller(CMARGBGen2A1controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "CoolerMaster";
    type                        = DEVICE_TYPE_LEDSTRIP;
    description                 = "CoolerMaster LED Controller A1 Device";
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = CM_ARGB_GEN2_A1_DIRECT_MODE;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Spectrum;
    Spectrum.name               = "Spectrum";
    Spectrum.value              = CM_ARGB_GEN2_A1_SPECTRUM_MODE;
    Spectrum.flags              = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Spectrum.color_mode         = MODE_COLORS_NONE;
    Spectrum.brightness         = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Spectrum.brightness_min     = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Spectrum.brightness_max     = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Spectrum.speed              = CM_ARGB_GEN2_A1_SPEED_MAX/2;
    Spectrum.speed_min          = CM_ARGB_GEN2_A1_SPEED_MIN;
    Spectrum.speed_max          = CM_ARGB_GEN2_A1_SPEED_MAX;
    modes.push_back(Spectrum);

    mode Static;
    Static.name                 = "Static";
    Static.value                = CM_ARGB_GEN2_A1_STATIC_MODE;
    Static.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness           = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Static.brightness_min       = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Static.brightness_max       = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Reload;
    Reload.name                 = "Reload";
    Reload.value                = CM_ARGB_GEN2_A1_RELOAD_MODE;
    Reload.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_MANUAL_SAVE;
    Reload.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Reload.brightness           = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Reload.brightness_min       = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Reload.brightness_max       = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Reload.speed                = CM_ARGB_GEN2_A1_SPEED_MAX/2;
    Reload.speed_min            = CM_ARGB_GEN2_A1_SPEED_MIN;
    Reload.speed_max            = CM_ARGB_GEN2_A1_SPEED_MAX;
    Reload.colors.resize(1);
    modes.push_back(Reload);

    mode Recoil;
    Recoil.name                 = "Recoil";
    Recoil.value                = CM_ARGB_GEN2_A1_RECOIL_MODE;
    Recoil.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_MANUAL_SAVE;
    Recoil.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Recoil.brightness           = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Recoil.brightness_min       = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Recoil.brightness_max       = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Recoil.speed                = CM_ARGB_GEN2_A1_SPEED_MAX/2;
    Recoil.speed_min            = CM_ARGB_GEN2_A1_SPEED_MIN;
    Recoil.speed_max            = CM_ARGB_GEN2_A1_SPEED_MAX;
    Recoil.colors.resize(1);
    modes.push_back(Recoil);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = CM_ARGB_GEN2_A1_BREATHING_MODE;
    Breathing.flags             = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.brightness        = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Breathing.brightness_min    = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Breathing.brightness_max    = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Breathing.speed             = CM_ARGB_GEN2_A1_SPEED_MAX/2;
    Breathing.speed_min         = CM_ARGB_GEN2_A1_SPEED_MIN;
    Breathing.speed_max         = CM_ARGB_GEN2_A1_SPEED_MAX;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);


    mode Refill;
    Refill.name                 = "Refill";
    Refill.value                = CM_ARGB_GEN2_A1_REFILL_MODE;
    Refill.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_MANUAL_SAVE;
    Refill.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Refill.brightness           = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Refill.brightness_min       = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Refill.brightness_max       = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Refill.speed                = CM_ARGB_GEN2_A1_SPEED_MAX/2;
    Refill.speed_min            = CM_ARGB_GEN2_A1_SPEED_MIN;
    Refill.speed_max            = CM_ARGB_GEN2_A1_SPEED_MAX;
    Refill.colors.resize(1);
    modes.push_back(Refill);

    mode Demo;
    Demo.name                   = "Demo";
    Demo.value                  = CM_ARGB_GEN2_A1_DEMO_MODE;
    Demo.flags                  = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Demo.color_mode             = MODE_COLORS_NONE;
    Demo.brightness             = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Demo.brightness_min         = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Demo.brightness_max         = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    modes.push_back(Demo);

    mode FillFlow;
    FillFlow.name               = "Fill Flow";
    FillFlow.value              = CM_ARGB_GEN2_A1_FILL_FLOW_MODE;
    FillFlow.flags              = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;;
    FillFlow.color_mode         = MODE_COLORS_NONE;
    FillFlow.brightness         = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    FillFlow.brightness_min     = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    FillFlow.brightness_max     = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    FillFlow.speed              = CM_ARGB_GEN2_A1_SPEED_MAX/2;
    FillFlow.speed_min          = CM_ARGB_GEN2_A1_SPEED_MIN;
    FillFlow.speed_max          = CM_ARGB_GEN2_A1_SPEED_MAX;
    modes.push_back(FillFlow);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = CM_ARGB_GEN2_A1_RAINBOW_MODE;
    Rainbow.flags               = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.brightness          = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Rainbow.brightness_min      = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Rainbow.speed               = CM_ARGB_GEN2_A1_SPEED_MAX/2;
    Rainbow.speed_min           = CM_ARGB_GEN2_A1_SPEED_MIN;
    Rainbow.speed_max           = CM_ARGB_GEN2_A1_SPEED_MAX;
    modes.push_back(Rainbow);

    mode Custom;
    Custom.name                 = "Custom";
    Custom.value                = CM_ARGB_GEN2_A1_CUSTOM_MODE;
    Custom.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Custom.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = CM_ARGB_GEN2_A1_OFF_MODE;
    Off.flags                   = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_CMARGBGen2A1Controller::~RGBController_CMARGBGen2A1Controller()
{
    delete controller;
}

void RGBController_CMARGBGen2A1Controller::SetupZones()
{
    unsigned int total_leds = 0;

    for(unsigned int channel = 0; channel < CM_ARGB_GEN2_A1_CHANNEL_COUNT; channel++)
    {
        zone new_zone;

        new_zone.name       = "Channel " + std::to_string(channel + 1);
        new_zone.type       = ZONE_TYPE_LINEAR;
        new_zone.leds_min   = 0;
        new_zone.leds_max   = CM_ARGB_GEN2_A1_CHANNEL_MAX_SIZE;
        new_zone.leds_count = 0;
        new_zone.matrix_map = nullptr;

        zones.push_back(new_zone);

        total_leds += new_zone.leds_count;
    }

    leds.resize(total_leds);

    for(unsigned int i = 0; i < total_leds; i++)
    {
        leds[i].name = "LED " + std::to_string(i + 1);
    }

    SetupColors();
}

void RGBController_CMARGBGen2A1Controller::ResizeZone(int zone, int new_size)
{
    zones[zone].leds_count = new_size;

    unsigned int total_leds = 0;

    for(unsigned int channel = 0; channel < CM_ARGB_GEN2_A1_CHANNEL_COUNT; channel++)
    {
        total_leds += zones[channel].leds_count;
    }

    leds.resize(total_leds);

    for(unsigned int i = 0; i < total_leds; i++)
    {
        leds[i].name = "LED " + std::to_string(i + 1);
    }

    controller->SetupZoneSize(zone, new_size);

    SetupColors();
}

void RGBController_CMARGBGen2A1Controller::DeviceUpdateLEDs()
{
    for(unsigned int channel = 0; channel < CM_ARGB_GEN2_A1_CHANNEL_COUNT; channel ++)
    {
        if (zones[channel].segments.size() > 0)
        {
            unsigned int i = 0;
            for(std::vector<segment>::iterator it = zones[channel].segments.begin(); it != zones[channel].segments.end(); it++)
            {
                UpdateSegmentLEDs(channel, i++);
            }
        }
        else
        {
            UpdateSegmentLEDs(channel, CM_ARGB_GEN2_A1_SUBCHANNEL_ALL);
        }
    }
}

void RGBController_CMARGBGen2A1Controller::UpdateZoneLEDs(int zone)
{
    if(zones[zone].leds_count > 0)
    {
        unsigned int start = zones[zone].start_idx;
        unsigned int end = start + zones[zone].leds_count;

        std::vector<RGBColor> zone_colors(colors.begin() + start , colors.begin() + end);

        controller->SendChannelColors(zone, CM_ARGB_GEN2_A1_SUBCHANNEL_ALL, zone_colors);
    }
}

void RGBController_CMARGBGen2A1Controller::UpdateSegmentLEDs(int zone, int subchannel)
{
    if(zones[zone].leds_count <= 0)
    {
        return;
    }

    unsigned int start = zones[zone].start_idx;
    unsigned int end = start + zones[zone].leds_count;
    bool use_direct_mode = modes[active_mode].value == CM_ARGB_GEN2_A1_DIRECT_MODE || modes[active_mode].value == CM_ARGB_GEN2_A1_CUSTOM_MODE;

    std::vector<RGBColor> color_vector(colors.begin() + start, colors.begin() + start + end);

    if(use_direct_mode)
    {
        if(zones[zone].segments.size() > 0)
        {
            start += zones[zone].segments[subchannel].start_idx;
            end += zones[zone].segments[subchannel].start_idx + zones[zone].segments[subchannel].leds_count;

            color_vector = std::vector<RGBColor>(colors.begin() + start , colors.begin() + end);
        }

        controller->SendChannelColors(zone, subchannel, color_vector);
        return;
    }

    controller->SendChannelColors(zone, CM_ARGB_GEN2_A1_SUBCHANNEL_ALL, color_vector);
}

void RGBController_CMARGBGen2A1Controller::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMARGBGen2A1Controller::DeviceUpdateMode()
{
    const mode& active = modes[active_mode];

    if(active.value == CM_ARGB_GEN2_A1_DIRECT_MODE)
    {
        controller->SetupDirectMode();
    }
    else
    {
        RGBColor color = active.color_mode == MODE_COLORS_MODE_SPECIFIC ?
                    active.colors[0] : 0;

        controller->SetMode
                (
                    active.value,
                    active.speed,
                    active.brightness,
                    color,
                    active.color_mode == MODE_COLORS_RANDOM
                );
    }
}

void RGBController_CMARGBGen2A1Controller::DeviceSaveMode()
{
    controller->SaveToFlash();
}

