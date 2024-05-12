/*---------------------------------------------------------*\
| RGBController_CMSmallARGBController.cpp                   |
|                                                           |
|   RGBController for Cooler Master Small ARGB controller   |
|                                                           |
|   Chris M (Dr_No)                             31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_CMSmallARGBController.h"

/**------------------------------------------------------------------*\
    @name Coolermaster Small ARGB
    @category LEDStrip
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterSmallARGB
    @comment The Coolermaster Small ARGB device supports `Direct` mode
        from firmware 0012 onwards. Check the serial number for the date
        "A202104052336" or newer.
\*-------------------------------------------------------------------*/

RGBController_CMSmallARGBController::RGBController_CMSmallARGBController(CMSmallARGBController* controller_ptr)
{
    controller                  = controller_ptr;
    unsigned char speed         = controller->GetLedSpeed();

    name                        = cm_small_argb_header_data[controller->GetZoneIndex()].name;
    vendor                      = "Cooler Master";
    type                        = DEVICE_TYPE_LEDSTRIP;
    description                 = controller->GetDeviceName();
    version                     = "2.0 for FW0012";
    serial                      = controller->GetSerial();
    location                    = controller->GetLocation();

    if(serial >= CM_SMALL_ARGB_FW0012)
    {
        mode Direct;
        Direct.name             = "Direct";
        Direct.value            = CM_SMALL_ARGB_MODE_DIRECT;
        Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Direct.brightness_min   = 0;
        Direct.brightness_max   = CM_SMALL_ARGB_BRIGHTNESS_MAX;
        Direct.brightness       = CM_SMALL_ARGB_BRIGHTNESS_MAX;
        Direct.color_mode       = MODE_COLORS_PER_LED;
        modes.push_back(Direct);
    }

    mode Off;
    Off.name                    = "Turn Off";
    Off.value                   = CM_SMALL_ARGB_MODE_OFF;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Reload;
    Reload.name                 = "Reload";
    Reload.value                = CM_SMALL_ARGB_MODE_RELOAD;
    Reload.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Reload.colors_min           = 1;
    Reload.colors_max           = 1;
    Reload.colors.resize(Reload.colors_max);
    Reload.brightness_min       = 0;
    Reload.brightness_max       = CM_SMALL_ARGB_BRIGHTNESS_MAX;
    Reload.brightness           = CM_SMALL_ARGB_BRIGHTNESS_MAX;
    Reload.speed_min            = CM_SMALL_ARGB_SPEED_SLOWEST;
    Reload.speed_max            = CM_SMALL_ARGB_SPEED_FASTEST;
    Reload.color_mode           = MODE_COLORS_RANDOM;
    Reload.speed                = speed;
    modes.push_back(Reload);

    mode Recoil;
    Recoil.name                 = "Recoil";
    Recoil.value                = CM_SMALL_ARGB_MODE_RECOIL;
    Recoil.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Recoil.colors_min           = 1;
    Recoil.colors_max           = 1;
    Recoil.colors.resize(Recoil.colors_max);
    Recoil.brightness_min       = 0;
    Recoil.brightness_max       = CM_SMALL_ARGB_BRIGHTNESS_MAX;
    Recoil.brightness           = CM_SMALL_ARGB_BRIGHTNESS_MAX;
    Recoil.speed_min            = CM_SMALL_ARGB_SPEED_SLOWEST;
    Recoil.speed_max            = CM_SMALL_ARGB_SPEED_FASTEST;
    Recoil.color_mode           = MODE_COLORS_RANDOM;
    Recoil.speed                = speed;
    modes.push_back(Recoil);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = CM_SMALL_ARGB_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(Breathing.colors_max);
    Breathing.brightness_min    = 0;
    Breathing.brightness_max    = CM_SMALL_ARGB_BRIGHTNESS_MAX;
    Breathing.brightness        = CM_SMALL_ARGB_BRIGHTNESS_MAX;
    Breathing.speed_min         = CM_SMALL_ARGB_SPEED_SLOWEST;
    Breathing.speed_max         = CM_SMALL_ARGB_SPEED_FASTEST;
    Breathing.color_mode        = MODE_COLORS_RANDOM;
    Breathing.speed             = speed;
    modes.push_back(Breathing);

    mode Refill;
    Refill.name                 = "Refill";
    Refill.value                = CM_SMALL_ARGB_MODE_REFILL;
    Refill.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Refill.colors_min           = 1;
    Refill.colors_max           = 1;
    Refill.colors.resize(Refill.colors_max);
    Refill.brightness_min       = 0;
    Refill.brightness_max       = CM_SMALL_ARGB_BRIGHTNESS_MAX;
    Refill.brightness           = CM_SMALL_ARGB_BRIGHTNESS_MAX;
    Refill.speed_min            = CM_SMALL_ARGB_SPEED_SLOWEST;
    Refill.speed_max            = CM_SMALL_ARGB_SPEED_FASTEST;
    Refill.color_mode           = MODE_COLORS_RANDOM;
    Refill.speed                = speed;
    modes.push_back(Refill);

    mode Demo;
    Demo.name                   = "Demo";
    Demo.value                  = CM_SMALL_ARGB_MODE_DEMO;
    Demo.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Demo.brightness_min         = 0;
    Demo.brightness_max         = CM_SMALL_ARGB_BRIGHTNESS_MAX;
    Demo.brightness             = CM_SMALL_ARGB_BRIGHTNESS_MAX;
    Demo.speed_min              = CM_SMALL_ARGB_SPEED_SLOWEST;
    Demo.speed_max              = CM_SMALL_ARGB_SPEED_FASTEST;
    Demo.color_mode             = MODE_COLORS_NONE;
    Demo.speed                  = speed;
    modes.push_back(Demo);

    mode Spectrum;
    Spectrum.name               = "Spectrum";
    Spectrum.value              = CM_SMALL_ARGB_MODE_SPECTRUM;
    Spectrum.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Spectrum.brightness_min     = 0;
    Spectrum.brightness_max     = CM_SMALL_ARGB_BRIGHTNESS_MAX;
    Spectrum.brightness         = CM_SMALL_ARGB_BRIGHTNESS_MAX;
    Spectrum.speed_min          = CM_SMALL_ARGB_SPEED_SLOWEST;
    Spectrum.speed_max          = CM_SMALL_ARGB_SPEED_FASTEST;
    Spectrum.color_mode         = MODE_COLORS_NONE;
    Spectrum.speed              = speed;
    modes.push_back(Spectrum);

    mode PassThru;
    PassThru.name               = "Pass Thru";
    PassThru.value              = CM_SMALL_ARGB_MODE_PASSTHRU;
    PassThru.color_mode         = MODE_COLORS_NONE;
    modes.push_back(PassThru);

    Init_Controller();         //Only processed on first run
    SetupZones();

    int temp_mode               = controller->GetMode();

    for(int mode_idx = 0; mode_idx < (int)modes.size() ; mode_idx++)
    {
        if(temp_mode == modes[mode_idx].value)
        {
            active_mode         = mode_idx;
            break;
        }
    }

    if (modes[active_mode].flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        modes[active_mode].colors[0] = ToRGBColor(controller->GetLedRed(), controller->GetLedGreen(), controller->GetLedBlue());
    }

    modes[active_mode].color_mode = (controller->GetRandomColours()) ? MODE_COLORS_RANDOM : MODE_COLORS_MODE_SPECIFIC;

    if (modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
    {
        modes[active_mode].speed = controller->GetLedSpeed();
    }
}

RGBController_CMSmallARGBController::~RGBController_CMSmallARGBController()
{
    delete controller;
}

void RGBController_CMSmallARGBController::Init_Controller()
{
    int zone_idx            = controller->GetZoneIndex();
    int zone_led_count      = cm_small_argb_header_data[zone_idx].count;
    bool boolSingleLED      = ( zone_led_count == 1 );          //If argb_header_data[zone_idx].count == 1 then the zone is ZONE_TYPE_SINGLE

    zone ARGB_zone;
    ARGB_zone.name          = std::to_string(zone_idx);
    ARGB_zone.type          = (boolSingleLED) ? ZONE_TYPE_SINGLE : ZONE_TYPE_LINEAR;
    ARGB_zone.leds_min      = CM_SMALL_ARGB_MIN_LEDS;
    ARGB_zone.leds_max      = CM_SMALL_ARGB_MAX_LEDS;
    ARGB_zone.leds_count    = zone_led_count;
    ARGB_zone.matrix_map    = NULL;
    zones.push_back(ARGB_zone);
}

void RGBController_CMSmallARGBController::SetupZones()
{
    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        bool boolSingleLED = (zones[zone_idx].type == ZONE_TYPE_SINGLE);    //Calculated for later use

        if (!boolSingleLED)
        {
            controller->SetLedCount(cm_small_argb_header_data[zone_idx].header, zones[zone_idx].leds_count);
        }

        for(unsigned int lp_idx = 0; lp_idx < zones[zone_idx].leds_count; lp_idx++)
        {
            led     new_led;
            unsigned int i = std::stoi(zones[zone_idx].name);

            if(boolSingleLED)
            {
                new_led.name  = i;
                new_led.value = cm_small_argb_header_data[i].header;
            }
            else
            {
                new_led.name = i;
                new_led.name.append(" LED " + std::to_string(lp_idx));
                new_led.value = cm_small_argb_header_data[i].header;
            }

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_CMSmallARGBController::ResizeZone(int zone, int new_size)
{
    if((size_t) zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        SetupZones();
    }
}

void RGBController_CMSmallARGBController::DeviceUpdateLEDs()
{
    for(int zone_idx = 0; zone_idx < (int)zones.size(); zone_idx++)
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_CMSmallARGBController::UpdateZoneLEDs(int zone)
{
    if(serial >= CM_SMALL_ARGB_FW0012)
    {
        controller->SetLedsDirect( zones[zone].colors, zones[zone].leds_count );
    }
}

void RGBController_CMSmallARGBController::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_CMSmallARGBController::SetCustomMode()
{
    /*-------------------------------------------------*\
    | The small ARGB may not support "Direct" mode      |
    |   in which case this will select "Pass Thru"      |
    \*-------------------------------------------------*/
    if(serial >= CM_SMALL_ARGB_FW0012)
    {
        active_mode = 0;
    }
    else
    {
        active_mode = 7;
    }
}

void RGBController_CMSmallARGBController::DeviceUpdateMode()
{
    bool     random_colours = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    RGBColor colour         = (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC) ? modes[active_mode].colors[0] : 0;

    controller->SetMode( modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, colour, random_colours);
}
