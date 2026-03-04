/*---------------------------------------------------------*\
| RGBController_CMARGBController.cpp                        |
|                                                           |
|   RGBController for Cooler Master ARGB controller         |
|                                                           |
|   Chris M (Dr_No)                             14 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_CMARGBController.h"

/**------------------------------------------------------------------*\
    @name Coolermaster ARGB
    @category LEDStrip
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterARGB
    @comment The Coolermaster ARGB device supports `Direct` mode from
        firmware 0028 onwards. Check the serial number for the date
        "A202105291658" or newer.
\*-------------------------------------------------------------------*/

RGBController_CMARGBController::RGBController_CMARGBController(CMARGBController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "Cooler Master";
    type                            = DEVICE_TYPE_LEDSTRIP;
    description                     = "Cooler Master ARGB Controller Device";
    version                         = controller->GetVersion();
    serial                          = controller->GetSerial();
    location                        = controller->GetLocation();

    /*-----------------------------------------------------*\
    | The ARGB ports support more modes than the RGB port.  |
    | Define all of the modes the ARGB ports support and    |
    | map RGB modes to them as best as we can.  Per-zone    |
    | support will be added in the future.                  |
    \*-----------------------------------------------------*/
    mode Off;
    Off.name                        = "Off";
    Off.value                       = CM_ARGB_MODE_OFF;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Reload;
    Reload.name                     = "Reload";
    Reload.value                    = CM_ARGB_MODE_RELOAD;
    Reload.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Reload.speed_min                = CM_ARGB_SPEED_SLOWEST;
    Reload.speed_max                = CM_ARGB_SPEED_FASTEST;
    Reload.speed                    = CM_ARGB_SPEED_NORMAL;
    Reload.brightness_min           = 0;
    Reload.brightness_max           = CM_ARGB_BRIGHTNESS_MAX;
    Reload.brightness               = CM_ARGB_BRIGHTNESS_MAX;
    Reload.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Reload.colors_min               = 1;
    Reload.colors_max               = 1;
    Reload.colors.resize(Reload.colors_max);
    modes.push_back(Reload);

    mode Recoil;
    Recoil.name                     = "Recoil";
    Recoil.value                    = CM_ARGB_MODE_RECOIL;
    Recoil.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Recoil.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Recoil.speed_min                = CM_ARGB_SPEED_SLOWEST;
    Recoil.speed_max                = CM_ARGB_SPEED_FASTEST;
    Recoil.speed                    = CM_ARGB_SPEED_NORMAL;
    Recoil.brightness_min           = 0;
    Recoil.brightness_max           = CM_ARGB_BRIGHTNESS_MAX;
    Recoil.brightness               = CM_ARGB_BRIGHTNESS_MAX;
    Recoil.colors_min               = 1;
    Recoil.colors_max               = 1;
    Recoil.colors.resize(Recoil.colors_max);
    modes.push_back(Recoil);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = CM_ARGB_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed_min             = CM_ARGB_SPEED_SLOWEST;
    Breathing.speed_max             = CM_ARGB_SPEED_FASTEST;
    Breathing.speed                 = CM_ARGB_SPEED_NORMAL;
    Breathing.brightness_min        = 0;
    Breathing.brightness_max        = CM_ARGB_BRIGHTNESS_MAX;
    Breathing.brightness            = CM_ARGB_BRIGHTNESS_MAX;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.colors.resize(Breathing.colors_max);
    modes.push_back(Breathing);

    mode Refill;
    Refill.name                     = "Refill";
    Refill.value                    = CM_ARGB_MODE_REFILL;
    Refill.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Refill.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Refill.speed_min                = CM_ARGB_SPEED_SLOWEST;
    Refill.speed_max                = CM_ARGB_SPEED_FASTEST;
    Refill.speed                    = CM_ARGB_SPEED_NORMAL;
    Refill.brightness_min           = 0;
    Refill.brightness_max           = CM_ARGB_BRIGHTNESS_MAX;
    Refill.brightness               = CM_ARGB_BRIGHTNESS_MAX;
    Refill.colors_min               = 1;
    Refill.colors_max               = 1;
    Refill.colors.resize(Refill.colors_max);
    modes.push_back(Refill);

    mode Demo;
    Demo.name                       = "Demo";
    Demo.value                      = CM_ARGB_MODE_DEMO;
    Demo.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Demo.color_mode                 = MODE_COLORS_NONE;
    Demo.speed_min                  = CM_ARGB_SPEED_SLOWEST;
    Demo.speed_max                  = CM_ARGB_SPEED_FASTEST;
    Demo.speed                      = CM_ARGB_SPEED_NORMAL;
    Demo.brightness_min             = 0;
    Demo.brightness_max             = CM_ARGB_BRIGHTNESS_MAX;
    Demo.brightness                 = CM_ARGB_BRIGHTNESS_MAX;
    modes.push_back(Demo);

    mode Spectrum;
    Spectrum.name                   = "Rainbow Wave";
    Spectrum.value                  = CM_ARGB_MODE_SPECTRUM;
    Spectrum.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Spectrum.color_mode             = MODE_COLORS_NONE;
    Spectrum.speed_min              = CM_ARGB_SPEED_SLOWEST;
    Spectrum.speed_max              = CM_ARGB_SPEED_FASTEST;
    Spectrum.speed                  = CM_ARGB_SPEED_NORMAL;
    Spectrum.brightness_min         = 0;
    Spectrum.brightness_max         = CM_ARGB_BRIGHTNESS_MAX;
    Spectrum.brightness             = CM_ARGB_BRIGHTNESS_MAX;
    modes.push_back(Spectrum);

    mode FillFlow;
    FillFlow.name                   = "Fill Flow";
    FillFlow.value                  = CM_ARGB_MODE_FILLFLOW;
    FillFlow.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    FillFlow.color_mode             = MODE_COLORS_NONE;
    FillFlow.speed_min              = CM_ARGB_SPEED_SLOWEST;
    FillFlow.speed_max              = CM_ARGB_SPEED_FASTEST;
    FillFlow.speed                  = CM_ARGB_SPEED_NORMAL;
    FillFlow.brightness_min         = 0;
    FillFlow.brightness_max         = CM_ARGB_BRIGHTNESS_MAX;
    FillFlow.brightness             = CM_ARGB_BRIGHTNESS_MAX;
    modes.push_back(FillFlow);

    mode Rainbow;
    Rainbow.name                    = "Rainbow";
    Rainbow.value                   = CM_ARGB_MODE_RAINBOW;
    Rainbow.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow.color_mode              = MODE_COLORS_NONE;
    Rainbow.speed_min               = CM_ARGB_SPEED_SLOWEST;
    Rainbow.speed_max               = CM_ARGB_SPEED_FASTEST;
    Rainbow.speed                   = CM_ARGB_SPEED_NORMAL;
    Rainbow.brightness_min          = 0;
    Rainbow.brightness_max          = CM_ARGB_BRIGHTNESS_MAX;
    Rainbow.brightness              = CM_ARGB_BRIGHTNESS_MAX;
    modes.push_back(Rainbow);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = CM_ARGB_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.speed_min                = CM_ARGB_SPEED_SLOWEST;
    Static.speed_max                = CM_ARGB_SPEED_FASTEST;
    Static.speed                    = CM_ARGB_SPEED_NORMAL;
    Static.brightness_min           = 0;
    Static.brightness_max           = CM_ARGB_BRIGHTNESS_MAX;
    Static.brightness               = CM_ARGB_BRIGHTNESS_MAX;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.colors.resize(Static.colors_max);
    modes.push_back(Static);

    mode Direct;
    Direct.name                     = (serial >= CM_ARGB_FW0028) ? "Direct" : "Custom";
    Direct.value                    = CM_ARGB_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode PassThru;
    PassThru.name                   = "Pass Thru";
    PassThru.value                  = CM_ARGB_MODE_PASSTHRU;
    PassThru.flags                  = 0;
    PassThru.color_mode             = MODE_COLORS_NONE;
    modes.push_back(PassThru);

    SetupZones();

    /*-----------------------------------------------------*\
    | Initialize the active mode to port 0                  |
    \*-----------------------------------------------------*/
    unsigned char   port_mode;
    unsigned char   port_speed;
    unsigned char   port_brightness;
    bool            port_random;
    unsigned char   port_red;
    unsigned char   port_green;
    unsigned char   port_blue;

    controller->GetPortStatus(0, &port_mode, &port_speed, &port_brightness, &port_random, &port_red, &port_green, &port_blue);

    for(std::size_t mode_idx = 0; mode_idx < modes.size(); mode_idx++)
    {
        if(modes[mode_idx].value == port_mode)
        {
            active_mode = mode_idx;

            if((modes[mode_idx].flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR) && (modes[mode_idx].colors.size() > 0))
            {
                modes[mode_idx].colors[0] = ToRGBColor(port_red, port_green, port_blue);
            }

            if(modes[mode_idx].flags & MODE_FLAG_HAS_SPEED)
            {
                modes[mode_idx].speed = port_speed;
            }

            if(modes[mode_idx].flags & MODE_FLAG_HAS_BRIGHTNESS)
            {
                modes[mode_idx].brightness = port_brightness;
            }

            if(modes[mode_idx].flags & MODE_FLAG_HAS_RANDOM_COLOR)
            {
                if(port_random)
                {
                    modes[mode_idx].color_mode = MODE_COLORS_RANDOM;
                }
            }

            break;
        }
    }
}

RGBController_CMARGBController::~RGBController_CMARGBController()
{
    delete controller;
}

void RGBController_CMARGBController::SetupZones()
{
    /*-----------------------------------------------------*\
    | Only set LED count on the first run                   |
    \*-----------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-----------------------------------------------------*\
    | Clear any existing color/LED configuration            |
    \*-----------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(5);

    /*-----------------------------------------------------*\
    | Set up addressable zones                              |
    \*-----------------------------------------------------*/
    for(unsigned int channel_idx = 0; channel_idx < 4; channel_idx++)
    {
        char ch_idx_string[2];
        snprintf(ch_idx_string, 2, "%d", channel_idx + 1);

        zones[channel_idx].name     = "Addressable RGB Header ";
        zones[channel_idx].name.append(ch_idx_string);
        zones[channel_idx].type     = ZONE_TYPE_LINEAR;
        zones[channel_idx].leds_min = 0;
        zones[channel_idx].leds_max = 48;

        if(first_run)
        {
            zones[channel_idx].leds_count = 0;
        }

        zones[channel_idx].matrix_map = NULL;

        for(unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            char led_idx_string[4];
            snprintf(led_idx_string, 4, "%d", led_ch_idx + 1);

            led new_led;
            new_led.name            = zones[channel_idx].name;
            new_led.name.append(", LED ");
            new_led.name.append(led_idx_string);
            new_led.value           = channel_idx;

            leds.push_back(new_led);
        }
    }

    /*-----------------------------------------------------*\
    | Set up RGB zone                                       |
    \*-----------------------------------------------------*/
    zones[4].name                   = "RGB Header";
    zones[4].type                   = ZONE_TYPE_SINGLE;
    zones[4].leds_min               = 1;
    zones[4].leds_max               = 1;
    zones[4].leds_count             = 1;
    zones[4].matrix_map             = NULL;

    led new_led;
    new_led.name                    = "RGB Header";
    new_led.value                   = 4;
    leds.push_back(new_led);

    SetupColors();
}

void RGBController_CMARGBController::ResizeZone(int zone, int new_size)
{
    if((size_t) zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        controller->SetPortLEDCount(zone, zones[zone].leds_count);

        SetupZones();
    }
}

void RGBController_CMARGBController::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_CMARGBController::UpdateZoneLEDs(int zone)
{
    /*-----------------------------------------------------*\
    | The RGB zone doesn't have a separate Direct mode, so  |
    | use static mode with the per-LED color for it         |
    \*-----------------------------------------------------*/
    if(zone < 4)
    {
        controller->SetPortLEDsDirect(zone, zones[zone].colors, zones[zone].leds_count);
    }
    else
    {
        controller->SetPortMode(zone, CM_RGB_MODE_STATIC, 0, 255, false, RGBGetRValue(zones[zone].colors[0]), RGBGetGValue(zones[zone].colors[0]), RGBGetBValue(zones[zone].colors[0]));
    }
}

void RGBController_CMARGBController::UpdateSingleLED(int led)
{
    unsigned int zone_idx = leds[led].value;

    UpdateZoneLEDs(zone_idx);
}

void RGBController_CMARGBController::DeviceUpdateMode()
{
    /*-----------------------------------------------------*\
    | Determine mode parameters                             |
    \*-----------------------------------------------------*/
    bool        random      = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    RGBColor    color       = (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC) ? modes[active_mode].colors[0] : 0;
    int         rgb_mode;
    bool        rgb_random  = random;

    /*-----------------------------------------------------*\
    | Map ARGB modes with the equivalent RGB modes          |
    \*-----------------------------------------------------*/
    switch(modes[active_mode].value)
    {
        case CM_ARGB_MODE_SPECTRUM:
        case CM_ARGB_MODE_FILLFLOW:
        case CM_ARGB_MODE_RAINBOW:
            rgb_mode    = CM_RGB_MODE_MIRAGE;
            rgb_random  = true;
            break;

        case CM_ARGB_MODE_RELOAD:
        case CM_ARGB_MODE_RECOIL:
            rgb_mode    = CM_RGB_MODE_FLASH;
            break;

        case CM_ARGB_MODE_BREATHING:
            rgb_mode    = CM_RGB_MODE_BREATHING;
            break;

        case CM_ARGB_MODE_REFILL:
        case CM_ARGB_MODE_STATIC:
            rgb_mode    = CM_RGB_MODE_STATIC;
            break;

        case CM_ARGB_MODE_DEMO:
            rgb_mode    = CM_RGB_MODE_FLASH;
            rgb_random  = true;
            break;

        case CM_ARGB_MODE_OFF:
        default:
            rgb_mode    = CM_RGB_MODE_OFF;
            break;

        case CM_ARGB_MODE_PASSTHRU:
            rgb_mode    = CM_RGB_MODE_PASSTHRU;
            break;
    }

    /*-----------------------------------------------------*\
    | Apply mode to all zones                               |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        controller->SetPortMode
                        (
                            zone_idx,
                            (zone_idx == 4) ? rgb_mode : modes[active_mode].value,
                            modes[active_mode].speed,
                            modes[active_mode].brightness,
                            (zone_idx == 4) ? rgb_random : random,
                            RGBGetRValue(color),
                            RGBGetGValue(color),
                            RGBGetBValue(color)
                        );
    }
}
