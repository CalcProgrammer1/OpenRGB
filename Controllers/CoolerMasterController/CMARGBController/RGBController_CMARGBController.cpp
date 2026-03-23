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

    SetupZones();
    SetupModes();

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
            active_mode = (int)mode_idx;

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
    Shutdown();

    delete controller;
}

void RGBController_CMARGBController::SetupModes()
{
    /*-----------------------------------------------------*\
    | Define Direct and Passthrough modes as require entire |
    | device, these are not available as per-zone modes     |
    \*-----------------------------------------------------*/
    mode Direct;
    Direct.name                     = (serial >= CM_ARGB_FW0028) ? "Direct" : "Custom";
    Direct.value                    = CM_ARGB_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode PassThru;
    PassThru.name                   = "Passthrough";
    PassThru.value                  = CM_ARGB_MODE_PASSTHRU | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    PassThru.flags                  = 0;
    PassThru.color_mode             = MODE_COLORS_NONE;
    modes.push_back(PassThru);

    /*-----------------------------------------------------*\
    | The ARGB ports support more modes than the RGB port.  |
    | Define all of the modes the ARGB ports support and    |
    | map RGB modes to them as best as we can.              |
    \*-----------------------------------------------------*/
    mode Off;
    Off.name                        = "Off";
    Off.value                       = CM_ARGB_MODE_OFF;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    for(std::size_t zone_idx = 0; zone_idx < 4; zone_idx++)
    {
        zones[zone_idx].modes.push_back(Off);
    }

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

    for(std::size_t zone_idx = 0; zone_idx < 4; zone_idx++)
    {
        zones[zone_idx].modes.push_back(Reload);
    }

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

    for(std::size_t zone_idx = 0; zone_idx < 4; zone_idx++)
    {
        zones[zone_idx].modes.push_back(Recoil);
    }

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

    for(std::size_t zone_idx = 0; zone_idx < 4; zone_idx++)
    {
        zones[zone_idx].modes.push_back(Breathing);
    }

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

    for(std::size_t zone_idx = 0; zone_idx < 4; zone_idx++)
    {
        zones[zone_idx].modes.push_back(Refill);
    }

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

    for(std::size_t zone_idx = 0; zone_idx < 4; zone_idx++)
    {
        zones[zone_idx].modes.push_back(Demo);
    }

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

    for(std::size_t zone_idx = 0; zone_idx < 4; zone_idx++)
    {
        zones[zone_idx].modes.push_back(Spectrum);
    }

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

    for(std::size_t zone_idx = 0; zone_idx < 4; zone_idx++)
    {
        zones[zone_idx].modes.push_back(FillFlow);
    }

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

    for(std::size_t zone_idx = 0; zone_idx < 4; zone_idx++)
    {
        zones[zone_idx].modes.push_back(Rainbow);
    }

    mode Static;
    Static.name                     = "Static";
    Static.value                    = CM_ARGB_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness_min           = 0;
    Static.brightness_max           = CM_ARGB_BRIGHTNESS_MAX;
    Static.brightness               = CM_ARGB_BRIGHTNESS_MAX;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.colors.resize(Static.colors_max);
    modes.push_back(Static);

    for(std::size_t zone_idx = 0; zone_idx < 4; zone_idx++)
    {
        zones[zone_idx].modes.push_back(Static);
    }

    /*-----------------------------------------------------*\
    | Define zone-specific modes for the RGB port           |
    \*-----------------------------------------------------*/
    mode RGBOff;
    RGBOff.name                     = "Off";
    RGBOff.value                    = CM_RGB_MODE_OFF;
    RGBOff.color_mode               = MODE_COLORS_NONE;
    zones[4].modes.push_back(RGBOff);

    mode RGBMirage;
    RGBMirage.name                  = "Mirage";
    RGBMirage.value                 = CM_RGB_MODE_MIRAGE;
    RGBMirage.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RGBMirage.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    RGBMirage.speed_min             = CM_ARGB_SPEED_SLOWEST;
    RGBMirage.speed_max             = CM_ARGB_SPEED_FASTEST;
    RGBMirage.speed                 = CM_ARGB_SPEED_NORMAL;
    RGBMirage.brightness_min        = 0;
    RGBMirage.brightness_max        = CM_ARGB_BRIGHTNESS_MAX;
    RGBMirage.brightness            = CM_ARGB_BRIGHTNESS_MAX;
    RGBMirage.colors_min            = 1;
    RGBMirage.colors_max            = 1;
    RGBMirage.colors.resize(RGBMirage.colors_max);
    zones[4].modes.push_back(RGBMirage);

    mode RGBFlash;
    RGBFlash.name                   = "Flash";
    RGBFlash.value                  = CM_RGB_MODE_FLASH;
    RGBFlash.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RGBFlash.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    RGBFlash.speed_min              = CM_ARGB_SPEED_SLOWEST;
    RGBFlash.speed_max              = CM_ARGB_SPEED_FASTEST;
    RGBFlash.speed                  = CM_ARGB_SPEED_NORMAL;
    RGBFlash.brightness_min         = 0;
    RGBFlash.brightness_max         = CM_ARGB_BRIGHTNESS_MAX;
    RGBFlash.brightness             = CM_ARGB_BRIGHTNESS_MAX;
    RGBFlash.colors_min             = 1;
    RGBFlash.colors_max             = 1;
    RGBFlash.colors.resize(RGBFlash.colors_max);
    zones[4].modes.push_back(RGBFlash);

    mode RGBBreathing;
    RGBBreathing.name               = "Breathing";
    RGBBreathing.value              = CM_RGB_MODE_BREATHING;
    RGBBreathing.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RGBBreathing.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    RGBBreathing.speed_min          = CM_ARGB_SPEED_SLOWEST;
    RGBBreathing.speed_max          = CM_ARGB_SPEED_FASTEST;
    RGBBreathing.speed              = CM_ARGB_SPEED_NORMAL;
    RGBBreathing.brightness_min     = 0;
    RGBBreathing.brightness_max     = CM_ARGB_BRIGHTNESS_MAX;
    RGBBreathing.brightness         = CM_ARGB_BRIGHTNESS_MAX;
    RGBBreathing.colors_min         = 1;
    RGBBreathing.colors_max         = 1;
    RGBBreathing.colors.resize(RGBBreathing.colors_max);
    zones[4].modes.push_back(RGBBreathing);

    mode RGBStatic;
    RGBStatic.name                  = "Static";
    RGBStatic.value                 = CM_RGB_MODE_STATIC;
    RGBStatic.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RGBStatic.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    RGBStatic.brightness_min        = 0;
    RGBStatic.brightness_max        = CM_ARGB_BRIGHTNESS_MAX;
    RGBStatic.brightness            = CM_ARGB_BRIGHTNESS_MAX;
    RGBStatic.colors_min            = 1;
    RGBStatic.colors_max            = 1;
    RGBStatic.colors.resize(RGBStatic.colors_max);
    zones[4].modes.push_back(RGBStatic);
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
        zones[channel_idx].leds_min                 = 0;
        zones[channel_idx].leds_max                 = 48;

        if(first_run)
        {
            zones[channel_idx].flags                = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS;
        }

        if(!(zones[channel_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
        {
            zones[channel_idx].name                 = "Addressable RGB Header ";
            zones[channel_idx].name.append(std::to_string(channel_idx + 1));
        }

        if(!(zones[channel_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
        {
            zones[channel_idx].leds_count           = 0;
        }

        if(!(zones[channel_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
        {
            zones[channel_idx].type                 = ZONE_TYPE_LINEAR;
        }

        if(!(zones[channel_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
        {
            zones[channel_idx].matrix_map.width     = 0;
            zones[channel_idx].matrix_map.height    = 0;
            zones[channel_idx].matrix_map.map.resize(0);
        }

        for(unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            led new_led;
            new_led.name            = zones[channel_idx].name;
            new_led.name.append(", LED ");
            new_led.name.append(std::to_string(led_ch_idx + 1));
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

    led new_led;
    new_led.name                    = "RGB Header";
    new_led.value                   = 4;
    leds.push_back(new_led);

    SetupColors();
}

void RGBController_CMARGBController::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        controller->SetPortLEDCount(zone_idx, zones[zone_idx].leds_count);

        SetupZones();
    }
}

void RGBController_CMARGBController::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        DeviceUpdateZoneLEDs((int)zone_idx);
    }
}

void RGBController_CMARGBController::DeviceUpdateZoneLEDs(int zone)
{
    if(modes[active_mode].value != CM_ARGB_MODE_DIRECT)
    {
        return;
    }

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

void RGBController_CMARGBController::DeviceUpdateSingleLED(int led)
{
    unsigned int zone_idx = leds[led].value;

    UpdateZoneLEDs(zone_idx);
}

void RGBController_CMARGBController::DeviceUpdateMode()
{
    /*-----------------------------------------------------*\
    | Apply mode to all zones                               |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        DeviceUpdateZoneMode((int)zone_idx);
    }
}

void RGBController_CMARGBController::DeviceUpdateZoneMode(int zone)
{
    /*-----------------------------------------------------*\
    | Determine the active mode for this zone               |
    \*-----------------------------------------------------*/
    bool            use_zone_mode       = false;
    int             zone_active_mode    = active_mode;
    int             zone_mode_value     = modes[active_mode].value;

    /*-----------------------------------------------------*\
    | Set up mode parameters                                |
    \*-----------------------------------------------------*/
    bool            random              = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    RGBColor        color               = 0;
    unsigned int    brightness          = modes[active_mode].brightness;
    unsigned int    speed               = modes[active_mode].speed;

    if(modes[active_mode].colors.size() > 0)
    {
        color                           = modes[active_mode].colors[0];
    }

    if(!(modes[active_mode].flags & MODE_FLAG_REQUIRES_ENTIRE_DEVICE))
    {
        if(zones[zone].active_mode >= 0)
        {
            use_zone_mode               = true;
            zone_active_mode            = zones[zone].active_mode;
            zone_mode_value             = zones[zone].modes[zone_active_mode].value;

            random                      = (zones[zone].modes[zone_active_mode].color_mode == MODE_COLORS_RANDOM);

            if(zones[zone].modes[zone_active_mode].colors.size() > 0)
            {
                color                   = zones[zone].modes[zone_active_mode].colors[0];
            }

            brightness                  = zones[zone].modes[zone_active_mode].brightness;
            speed                       = zones[zone].modes[zone_active_mode].speed;
        }
    }

    if((zone == 4) && !use_zone_mode)
    {
        /*-------------------------------------------------*\
        | Map entire-device ARGB modes with the equivalent  |
        | RGB modes for the RGB zone                        |
        \*-------------------------------------------------*/
        switch(modes[active_mode].value)
        {
            case CM_ARGB_MODE_SPECTRUM:
            case CM_ARGB_MODE_FILLFLOW:
            case CM_ARGB_MODE_RAINBOW:
                zone_mode_value         = CM_RGB_MODE_MIRAGE;
                random                  = true;
                break;

            case CM_ARGB_MODE_RELOAD:
            case CM_ARGB_MODE_RECOIL:
                zone_mode_value         = CM_RGB_MODE_FLASH;
                break;

            case CM_ARGB_MODE_BREATHING:
                zone_mode_value         = CM_RGB_MODE_BREATHING;
                break;

            case CM_ARGB_MODE_REFILL:
            case CM_ARGB_MODE_STATIC:
                zone_mode_value         = CM_RGB_MODE_STATIC;
                break;

            case CM_ARGB_MODE_DEMO:
                zone_mode_value         = CM_RGB_MODE_FLASH;
                random                  = true;
                break;

            case CM_ARGB_MODE_OFF:
            default:
                zone_mode_value         = CM_RGB_MODE_OFF;
                break;

            case CM_ARGB_MODE_PASSTHRU:
                zone_mode_value         = CM_RGB_MODE_PASSTHRU;
                break;
        }
    }

    controller->SetPortMode
                    (
                        zone,
                        zone_mode_value,
                        speed,
                        brightness,
                        random,
                        RGBGetRValue(color),
                        RGBGetGValue(color),
                        RGBGetBValue(color)
                    );
}
