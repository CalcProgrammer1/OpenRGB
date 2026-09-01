/*---------------------------------------------------------*\
| RGBController_CMSmallARGBController.cpp                   |
|                                                           |
|   RGBController for Cooler Master Small ARGB controller   |
|                                                           |
|   Chris M (Dr_No)                             31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    name                        = cm_small_argb_header_data[0].name;
    vendor                      = "Cooler Master";
    type                        = DEVICE_TYPE_LEDSTRIP;
    description                 = controller->GetDeviceName();
    version                     = controller->GetVersion();
    serial                      = controller->GetSerial();
    location                    = controller->GetLocation();

    SetupModes();
    SetupZones();

    /*-----------------------------------------------------*\
    | Initialize the active mode to the device's current    |
    | mode                                                  |
    \*-----------------------------------------------------*/
    int             device_mode         = controller->GetMode();

    for(std::size_t mode_idx = 0; mode_idx < modes.size(); mode_idx++)
    {
        if(modes[mode_idx].value == device_mode)
        {
            active_mode = (int)mode_idx;

            if((modes[mode_idx].flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR) && (modes[mode_idx].colors.size() > 0))
            {
                modes[mode_idx].colors[0] = ToRGBColor(controller->GetLedRed(), controller->GetLedGreen(), controller->GetLedBlue());
            }

            if(modes[mode_idx].flags & MODE_FLAG_HAS_SPEED)
            {
                modes[mode_idx].speed = controller->GetLedSpeed();
            }

            if(modes[mode_idx].flags & MODE_FLAG_HAS_BRIGHTNESS)
            {
                modes[mode_idx].brightness = controller->GetBrightness();
            }

            if(modes[mode_idx].flags & MODE_FLAG_HAS_RANDOM_COLOR)
            {
                modes[mode_idx].color_mode = controller->GetRandomColours() ? MODE_COLORS_RANDOM : MODE_COLORS_MODE_SPECIFIC;
            }

            break;
        }
    }
}

void RGBController_CMSmallARGBController::SetupModes()
{
    /*-----------------------------------------------------*\
    | The Small ARGB only supports Direct mode from FW0012  |
    | onwards, older firmware does not accept direct mode   |
    | packets                                               |
    \*-----------------------------------------------------*/
    //if(serial >= CM_SMALL_ARGB_FW0012)
    {
        mode Direct;
        Direct.name             = "Direct";
        Direct.value            = CM_SMALL_ARGB_MODE_DIRECT;
        Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
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
    Reload.speed                = CM_SMALL_ARGB_SPEED_NORMAL;
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
    Recoil.speed                = CM_SMALL_ARGB_SPEED_NORMAL;
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
    Breathing.speed             = CM_SMALL_ARGB_SPEED_NORMAL;
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
    Refill.speed                = CM_SMALL_ARGB_SPEED_NORMAL;
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
    Demo.speed                  = CM_SMALL_ARGB_SPEED_NORMAL;
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
    Spectrum.speed              = CM_SMALL_ARGB_SPEED_NORMAL;
    modes.push_back(Spectrum);

    /*-----------------------------------------------------*\
    | Define Passthrough mode as requiring the entire       |
    | device, it hands control over to the motherboard      |
    \*-----------------------------------------------------*/
    mode PassThru;
    PassThru.name               = "Pass Thru";
    PassThru.value              = CM_SMALL_ARGB_MODE_PASSTHRU;
    PassThru.flags              = MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    PassThru.color_mode         = MODE_COLORS_NONE;
    modes.push_back(PassThru);
}

RGBController_CMSmallARGBController::~RGBController_CMSmallARGBController()
{
    Shutdown();

    delete controller;
}

void RGBController_CMSmallARGBController::SetupZones()
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
    zones.resize(1);

    /*-----------------------------------------------------*\
    | Set up zones and LEDs                                 |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].leds_min                    = 0;
        zones[zone_idx].leds_max                    = CM_SMALL_ARGB_MAX_LEDS;

        if(first_run)
        {
            zones[zone_idx].flags                   = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
        {
            zones[zone_idx].name                    = "Addressable RGB Header";
            zones[zone_idx].name.append(std::to_string(zone_idx + 1));
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
        {
            zones[zone_idx].leds_count              = 0;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
        {
            zones[zone_idx].type                    = ZONE_TYPE_LINEAR;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
        {
            zones[zone_idx].matrix_map.width        = 0;
            zones[zone_idx].matrix_map.height       = 0;
            zones[zone_idx].matrix_map.map.resize(0);
        }

        for(unsigned int lp_idx = 0; lp_idx < zones[zone_idx].leds_count; lp_idx++)
        {
            led new_led;
            new_led.name                            = zones[zone_idx].name;
            new_led.name.append(", LED ");
            new_led.name.append(std::to_string(lp_idx + 1));
            new_led.value                           = (unsigned int)zone_idx;

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_CMSmallARGBController::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        controller->SetLedCount(cm_small_argb_header_data[zone_idx].header, zones[zone_idx].leds_count);

        SetupZones();
    }
}

void RGBController_CMSmallARGBController::DeviceUpdateLEDs()
{
    for(int zone_idx = 0; zone_idx < (int)zones.size(); zone_idx++)
    {
        DeviceUpdateZoneLEDs(zone_idx);
    }
}

void RGBController_CMSmallARGBController::DeviceUpdateZoneLEDs(int zone)
{
    /*-----------------------------------------------------*\
    | Only send direct packets while Direct mode is active. |
    | Direct mode is only offered on FW0012 and newer, so   |
    | older firmware will never send direct packets here    |
    \*-----------------------------------------------------*/
    if(modes[active_mode].value != CM_SMALL_ARGB_MODE_DIRECT)
    {
        return;
    }

    controller->SetLedsDirect(zones[zone].colors, zones[zone].leds_count);
}

void RGBController_CMSmallARGBController::DeviceUpdateSingleLED(int led)
{
    /*-----------------------------------------------------*\
    | Map the LED back to its zone and update that zone     |
    \*-----------------------------------------------------*/
    unsigned int zone_idx = leds[led].value;

    UpdateZoneLEDs(zone_idx);
}

void RGBController_CMSmallARGBController::SetCustomMode()
{
    /*-----------------------------------------------------*\
    | Direct mode is only supported on FW0012 and newer     |
    \*-----------------------------------------------------*/
    if(serial >= CM_SMALL_ARGB_FW0012)
    {
        /*-------------------------------------------------*\
        | Search the mode list for Direct mode              |
        \*-------------------------------------------------*/
        for(std::size_t mode_idx = 0; mode_idx < modes.size(); mode_idx++)
        {
            if(modes[mode_idx].name == "Direct")
            {
                SetActiveMode((int)mode_idx);
                break;
            }
        }
    }
    else
    {
        /*-------------------------------------------------*\
        | Direct mode is unsupported by the firmware,       |
        | fall back to Pass Thru                            |
        \*-------------------------------------------------*/
        for(std::size_t mode_idx = 0; mode_idx < modes.size(); mode_idx++)
        {
            if(modes[mode_idx].value == CM_SMALL_ARGB_MODE_PASSTHRU)
            {
                SetActiveMode((int)mode_idx);
                break;
            }
        }
    }
}

void RGBController_CMSmallARGBController::DeviceUpdateMode()
{
    /*-----------------------------------------------------*\
    | Set up mode parameters                                |
    \*-----------------------------------------------------*/
    bool            random_colours  = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    RGBColor        colour          = 0;
    unsigned int    speed           = modes[active_mode].speed;
    unsigned int    brightness      = modes[active_mode].brightness;

    if(modes[active_mode].colors.size() > 0)
    {
        colour                      = modes[active_mode].colors[0];
    }

    controller->SetMode(modes[active_mode].value, speed, brightness, colour, random_colours);
}
