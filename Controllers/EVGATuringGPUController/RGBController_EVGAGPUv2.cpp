/*---------------------------------------------------------*\
| RGBController_EVGAGPUv2.cpp                               |
|                                                           |
|   RGBController for EVGA V2 (Turing) GPU                  |
|                                                           |
|   TheRogueZeta                                15 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_EVGAGPUv2.h"

/**------------------------------------------------------------------*\
    @name EVGA RGB v2 GPU
    @category GPU
    @type I2C
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectEVGATuringGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_EVGAGPUv2::RGBController_EVGAGPUv2(EVGAGPUv2Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "EVGA";
    description                 = "EVGA Turing RGB GPU Device";
    location                    = controller->GetDeviceLocation();
    type                        = DEVICE_TYPE_GPU;

    mode Off;
    Off.name                    = "Off";
    Off.value                   = EVGA_GPU_V2_RGB_MODE_OFF;
    Off.flags                   = 0;  //pretty sure not needed
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Direct";
    Static.value                = EVGA_GPU_V2_RGB_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Static.color_mode           = MODE_COLORS_PER_LED;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.brightness_min       = EVGA_GPU_V2_BRIGHTNESS_MIN;
    Static.brightness           = EVGA_GPU_V2_BRIGHTNESS_DEFAULT;
    Static.brightness_max       = EVGA_GPU_V2_BRIGHTNESS_MAX;

    modes.push_back(Static);

    mode Rainbow;
    Rainbow.name                = "Spectrum Cycle";
    Rainbow.value               = EVGA_GPU_V2_RGB_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.brightness_min      = EVGA_GPU_V2_BRIGHTNESS_MIN;
    Rainbow.brightness          = EVGA_GPU_V2_BRIGHTNESS_DEFAULT;
    Rainbow.brightness_max      = EVGA_GPU_V2_BRIGHTNESS_MAX;
    modes.push_back(Rainbow);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = EVGA_GPU_V2_RGB_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE;
    Breathing.speed_min         = EVGA_GPU_V2_SPEED_BREATHING_SLOWEST;
    Breathing.speed             = EVGA_GPU_V2_SPEED_BREATHING_NORMAL;
    Breathing.speed_max         = EVGA_GPU_V2_SPEED_BREATHING_FASTEST;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.brightness_min    = EVGA_GPU_V2_BRIGHTNESS_MIN;
    Breathing.brightness        = EVGA_GPU_V2_BRIGHTNESS_DEFAULT;
    Breathing.brightness_max    = EVGA_GPU_V2_BRIGHTNESS_MAX;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 2;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Pulse;
    Pulse.name                  = "Flashing";
    Pulse.value                 = EVGA_GPU_V2_RGB_MODE_PULSE;
    Pulse.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE;
    Pulse.speed_min             = EVGA_GPU_V2_SPEED_PULSE_SLOWEST;
    Pulse.speed                 = EVGA_GPU_V2_SPEED_PULSE_NORMAL;
    Pulse.speed_max             = EVGA_GPU_V2_SPEED_PULSE_FASTEST;
    Pulse.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Pulse.colors_min            = 1;
    Pulse.colors_max            = 2;
    Pulse.brightness_min        = EVGA_GPU_V2_BRIGHTNESS_MIN;
    Pulse.brightness            = EVGA_GPU_V2_BRIGHTNESS_DEFAULT;
    Pulse.brightness_max        = EVGA_GPU_V2_BRIGHTNESS_MAX;
    Pulse.colors.resize(1);
    modes.push_back(Pulse);


    SetupZones();

    // Initialize active mode
    active_mode = getModeIndex(controller->GetMode());

    /*---------------------------------------------------------*\
    | The LED color (color[0]) will always be set. Mode colors  |
    | are only set for the MODE_COLORS_MODE_SPECIFIC modes and  |
    | by extension colorB is only necessary if its not black    |
    \*---------------------------------------------------------*/

    colors[0]           = controller->GetColorA();
    RGBColor colorB     = controller->GetColorB();

    int breathing_mode_index = getModeIndex(EVGA_GPU_V2_RGB_MODE_BREATHING);
    int pulse_mode_index     = getModeIndex(EVGA_GPU_V2_RGB_MODE_PULSE);

    // Pre fill in colors for mode specific colors
    modes[breathing_mode_index].colors[0] = colors[0];
    modes[pulse_mode_index].colors[0] = colors[0];
    // Add colors if colorB is not equal to 0.
    if(colorB != 0)
    {
        modes[breathing_mode_index].colors.push_back(colorB);
        modes[pulse_mode_index].colors.push_back(colorB);
    }

    // Load speed settings from the card:
    modes[active_mode].speed = controller->GetSpeed();
    modes[active_mode].brightness = controller->GetBrightnessA();
}

RGBController_EVGAGPUv2::~RGBController_EVGAGPUv2()
{
    delete controller;
}

int RGBController_EVGAGPUv2::getModeIndex(unsigned char mode_value)
{
    for(int mode_index = 0; mode_index < (int)modes.size(); mode_index++)
    {
        if(modes[mode_index].value == mode_value)
        {
            return(mode_index);
        }
    }

    return(0);
}

void RGBController_EVGAGPUv2::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device only has one LED, so create a single zone and |
    | LED for it                                                |
    \*---------------------------------------------------------*/
    zone* new_zone = new zone();
    led*  new_led  = new led();

    new_zone->name          = "GPU Zone";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;
    new_zone->matrix_map    = NULL;

    new_led->name           = "GPU LED";

    /*---------------------------------------------------------*\
    | Push the zone and LED on to device vectors                |
    \*---------------------------------------------------------*/
    leds.push_back(*new_led);
    zones.push_back(*new_zone);

    SetupColors();
}

void RGBController_EVGAGPUv2::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_EVGAGPUv2::DeviceUpdateLEDs()
{
    /*---------------------------------------------------------*\
    | DeviceUpdateLEDs() is only used in MODE_COLORS_PER_LED    |
    | modes and as such colorB will always be black (0x000000)  |
    \*---------------------------------------------------------*/

    controller->SetColor(colors[0], /* colorB*/ 0, modes[active_mode].brightness);
}

void RGBController_EVGAGPUv2::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGAGPUv2::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGAGPUv2::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | Modes with MODE_COLORS_MODE_SPECIFIC may have either      |
    | 1 or 2 colors associated with it. The device controller   |
    | expects colorB as black (0x000000) in 1 color scenarios   |
    \*---------------------------------------------------------*/

    RGBColor colorA = colors[0];
    RGBColor colorB = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        colorA = modes[active_mode].colors[0];
        colorB = (modes[active_mode].colors.size() == 2) ? modes[active_mode].colors[1] : 0 ;
    }

    controller->SetMode( modes[active_mode].value, colorA, colorB, modes[active_mode].speed, modes[active_mode].brightness);
}

void RGBController_EVGAGPUv2::DeviceSaveMode()
{
    controller->SaveSettings();
}
