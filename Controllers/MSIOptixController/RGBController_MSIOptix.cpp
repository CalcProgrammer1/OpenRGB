/*---------------------------------------------------------*\
| RGBController_MSIOptix.cpp                                |
|                                                           |
|   RGBController for MSI Optix                             |
|                                                           |
|   Morgan Guimard (morg)                       10 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_MSIOptix.h"

/**------------------------------------------------------------------*\
    @name MSI Optix
    @category LEDStrip
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectMSIOptixControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_MSIOptix::RGBController_MSIOptix(MSIOptixController* controller_ptr)
{
    controller                          = controller_ptr;
    name                                = controller->GetNameString();
    vendor                              = "MSI";
    type                                = DEVICE_TYPE_LEDSTRIP;
    description                         = "MSI Optix USB Device";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.value                        = DIRECT_MODE_VALUE;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    Direct.brightness_min               = MSI_OPTIX_BRIGHTNESS_MIN;
    Direct.brightness_max               = MSI_OPTIX_BRIGHTNESS_MAX;
    Direct.brightness                   = MSI_OPTIX_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    mode Static;
    Static.name                         = "Static";
    Static.value                        = STATIC_MODE_VALUE;
    Static.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min                   = 1;
    Static.colors_max                   = 1;
    Static.brightness_min               = MSI_OPTIX_BRIGHTNESS_MIN;
    Static.brightness_max               = MSI_OPTIX_BRIGHTNESS_MAX;
    Static.brightness                   = MSI_OPTIX_BRIGHTNESS_MAX;
    Static.colors.resize(1);
    Static.colors[0]                    = MSI_OPTIX_DEFAULT_MODE_COLOR;
    modes.push_back(Static);

    mode OFF;
    OFF.name                            = "Off";
    OFF.value                           = OFF_MODE_VALUE;
    OFF.flags                           = 0;
    OFF.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(OFF);

    mode Rainbow;
    Rainbow.name                        = "Rainbow";
    Rainbow.value                       = RAINBOW_MODE_VALUE;
    Rainbow.flags                       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow.color_mode                  = MODE_COLORS_NONE;
    Rainbow.brightness_min              = MSI_OPTIX_BRIGHTNESS_MIN;
    Rainbow.brightness_max              = MSI_OPTIX_BRIGHTNESS_MAX;
    Rainbow.brightness                  = MSI_OPTIX_BRIGHTNESS_MAX;
    Rainbow.speed_min                   = MSI_OPTIX_SPEED_MIN;
    Rainbow.speed_max                   = MSI_OPTIX_SPEED_MAX;
    Rainbow.speed                       = MSI_OPTIX_SPEED_MIN;
    modes.push_back(Rainbow);

    mode Meteor;
    Meteor.name                         = "Meteor";
    Meteor.value                        = METEOR_MODE_VALUE;
    Meteor.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Meteor.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Meteor.colors_min                   = 1;
    Meteor.colors_max                   = 1;
    Meteor.brightness_min               = MSI_OPTIX_BRIGHTNESS_MIN;
    Meteor.brightness_max               = MSI_OPTIX_BRIGHTNESS_MAX;
    Meteor.brightness                   = MSI_OPTIX_BRIGHTNESS_MAX;
    Meteor.speed_min                    = MSI_OPTIX_SPEED_MIN;
    Meteor.speed_max                    = MSI_OPTIX_SPEED_MAX;
    Meteor.speed                        = MSI_OPTIX_SPEED_MIN;
    Meteor.colors.resize(1);
    Meteor.colors[0]                    = MSI_OPTIX_DEFAULT_MODE_COLOR;
    modes.push_back(Meteor);

    mode Stack;
    Stack.name                          = "Stack";
    Stack.value                         = STACK_MODE_VALUE;
    Stack.flags                         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Stack.color_mode                    = MODE_COLORS_MODE_SPECIFIC;
    Stack.colors_min                    = 1;
    Stack.colors_max                    = 1;
    Stack.brightness_min                = MSI_OPTIX_BRIGHTNESS_MIN;
    Stack.brightness_max                = MSI_OPTIX_BRIGHTNESS_MAX;
    Stack.brightness                    = MSI_OPTIX_BRIGHTNESS_MAX;
    Stack.speed_min                     = MSI_OPTIX_SPEED_MIN;
    Stack.speed_max                     = MSI_OPTIX_SPEED_MAX;
    Stack.speed                         = MSI_OPTIX_SPEED_MIN;
    Stack.colors.resize(1);
    Stack.colors[0]                     = MSI_OPTIX_DEFAULT_MODE_COLOR;
    modes.push_back(Stack);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = BREATHING_MODE_VALUE;
    Breathing.flags                     = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode                = MODE_COLORS_PER_LED;
    Breathing.brightness_min            = MSI_OPTIX_BRIGHTNESS_MIN;
    Breathing.brightness_max            = MSI_OPTIX_BRIGHTNESS_MAX;
    Breathing.brightness                = MSI_OPTIX_BRIGHTNESS_MAX;
    Breathing.speed_min                 = MSI_OPTIX_SPEED_MIN;
    Breathing.speed_max                 = MSI_OPTIX_SPEED_MAX;
    Breathing.speed                     = MSI_OPTIX_SPEED_MIN;
    Breathing.colors.resize(1);
    Breathing.colors[0]                 = MSI_OPTIX_DEFAULT_MODE_COLOR;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name                       = "Flashing";
    Flashing.value                      = FLASHING_MODE_VALUE;
    Flashing.flags                      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Flashing.color_mode                 = MODE_COLORS_MODE_SPECIFIC;
    Flashing.colors_min                 = 1;
    Flashing.colors_max                 = 1;
    Flashing.brightness_min             = MSI_OPTIX_BRIGHTNESS_MIN;
    Flashing.brightness_max             = MSI_OPTIX_BRIGHTNESS_MAX;
    Flashing.brightness                 = MSI_OPTIX_BRIGHTNESS_MAX;
    Flashing.speed_min                  = MSI_OPTIX_SPEED_MIN;
    Flashing.speed_max                  = MSI_OPTIX_SPEED_MAX;
    Flashing.speed                      = MSI_OPTIX_SPEED_MIN;
    Flashing.colors.resize(1);
    Flashing.colors[0]                  = MSI_OPTIX_DEFAULT_MODE_COLOR;
    modes.push_back(Flashing);

    mode Double_Flashing;
    Double_Flashing.name                = "Double Flashing";
    Double_Flashing.value               = DOUBLE_FLASHING_MODE_VALUE;
    Double_Flashing.flags               = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Double_Flashing.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Double_Flashing.colors_min          = 1;
    Double_Flashing.colors_max          = 1;
    Double_Flashing.brightness_min      = MSI_OPTIX_BRIGHTNESS_MIN;
    Double_Flashing.brightness_max      = MSI_OPTIX_BRIGHTNESS_MAX;
    Double_Flashing.brightness          = MSI_OPTIX_BRIGHTNESS_MAX;
    Double_Flashing.speed_min           = MSI_OPTIX_SPEED_MIN;
    Double_Flashing.speed_max           = MSI_OPTIX_SPEED_MAX;
    Double_Flashing.speed               = MSI_OPTIX_SPEED_MIN;
    Double_Flashing.colors.resize(1);
    Double_Flashing.colors[0]           = MSI_OPTIX_DEFAULT_MODE_COLOR;
    modes.push_back(Double_Flashing);

    mode Lightning;
    Lightning.name                      = "Lightning";
    Lightning.value                     = LIGHNING_MODE_VALUE;
    Lightning.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Lightning.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Lightning.colors_min                = 1;
    Lightning.colors_max                = 1;
    Lightning.brightness_min            = MSI_OPTIX_BRIGHTNESS_MIN;
    Lightning.brightness_max            = MSI_OPTIX_BRIGHTNESS_MAX;
    Lightning.brightness                = MSI_OPTIX_BRIGHTNESS_MAX;
    Lightning.speed_min                 = MSI_OPTIX_SPEED_MIN;
    Lightning.speed_max                 = MSI_OPTIX_SPEED_MAX;
    Lightning.speed                     = MSI_OPTIX_SPEED_MIN;
    Lightning.colors.resize(1);
    Lightning.colors[0]                 = MSI_OPTIX_DEFAULT_MODE_COLOR;
    modes.push_back(Lightning);

    mode Planetary;
    Planetary.name                      = "Planetary";
    Planetary.value                     = PLANETARY_MODE_VALUE;
    Planetary.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Planetary.color_mode                = MODE_COLORS_NONE;
    Planetary.brightness_min            = MSI_OPTIX_BRIGHTNESS_MIN;
    Planetary.brightness_max            = MSI_OPTIX_BRIGHTNESS_MAX;
    Planetary.brightness                = MSI_OPTIX_BRIGHTNESS_MAX;
    Planetary.speed_min                 = MSI_OPTIX_SPEED_MIN;
    Planetary.speed_max                 = MSI_OPTIX_SPEED_MAX;
    Planetary.speed                     = MSI_OPTIX_SPEED_MIN;
    modes.push_back(Planetary);

    mode Double_Meteor;
    Double_Meteor.name                  = "Double_Meteor";
    Double_Meteor.value                 = DOUBLE_METEOR_MODE_VALUE;
    Double_Meteor.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Double_Meteor.color_mode            = MODE_COLORS_NONE;
    Double_Meteor.brightness_min        = MSI_OPTIX_BRIGHTNESS_MIN;
    Double_Meteor.brightness_max        = MSI_OPTIX_BRIGHTNESS_MAX;
    Double_Meteor.brightness            = MSI_OPTIX_BRIGHTNESS_MAX;
    Double_Meteor.speed_min             = MSI_OPTIX_SPEED_MIN;
    Double_Meteor.speed_max             = MSI_OPTIX_SPEED_MAX;
    Double_Meteor.speed                 = MSI_OPTIX_SPEED_MIN;
    modes.push_back(Double_Meteor);

    mode Energy;
    Energy.name                         = "Energy";
    Energy.value                        = ENERGY_MODE_VALUE;
    Energy.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Energy.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Energy.colors_min                   = 1;
    Energy.colors_max                   = 1;
    Energy.brightness_min               = MSI_OPTIX_BRIGHTNESS_MIN;
    Energy.brightness_max               = MSI_OPTIX_BRIGHTNESS_MAX;
    Energy.brightness                   = MSI_OPTIX_BRIGHTNESS_MAX;
    Energy.speed_min                    = MSI_OPTIX_SPEED_MIN;
    Energy.speed_max                    = MSI_OPTIX_SPEED_MAX;
    Energy.speed                        = MSI_OPTIX_SPEED_MIN;
    Energy.colors.resize(1);
    Energy.colors[0]                    = MSI_OPTIX_DEFAULT_MODE_COLOR;
    modes.push_back(Energy);

    mode Blink;
    Blink.name                          = "Blink";
    Blink.value                         = BLINK_MODE_VALUE;
    Blink.flags                         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Blink.color_mode                    = MODE_COLORS_NONE;
    Blink.brightness_min                = MSI_OPTIX_BRIGHTNESS_MIN;
    Blink.brightness_max                = MSI_OPTIX_BRIGHTNESS_MAX;
    Blink.brightness                    = MSI_OPTIX_BRIGHTNESS_MAX;
    Blink.speed_min                     = MSI_OPTIX_SPEED_MIN;
    Blink.speed_max                     = MSI_OPTIX_SPEED_MAX;
    Blink.speed                         = MSI_OPTIX_SPEED_MIN;
    modes.push_back(Blink);

    mode Clock;
    Clock.name                          = "Clock";
    Clock.value                         = CLOCK_MODE_VALUE;
    Clock.flags                         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Clock.color_mode                    = MODE_COLORS_NONE;
    Clock.brightness_min                = MSI_OPTIX_BRIGHTNESS_MIN;
    Clock.brightness_max                = MSI_OPTIX_BRIGHTNESS_MAX;
    Clock.brightness                    = MSI_OPTIX_BRIGHTNESS_MAX;
    Clock.speed_min                     = MSI_OPTIX_SPEED_MIN;
    Clock.speed_max                     = MSI_OPTIX_SPEED_MAX;
    Clock.speed                         = MSI_OPTIX_SPEED_MIN;
    modes.push_back(Clock);

    mode Color_Pulse;
    Color_Pulse.name                    = "Color Pulse";
    Color_Pulse.value                   = COLOR_PULSE_MODE_VALUE;
    Color_Pulse.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Color_Pulse.color_mode              = MODE_COLORS_NONE;
    Color_Pulse.brightness_min          = MSI_OPTIX_BRIGHTNESS_MIN;
    Color_Pulse.brightness_max          = MSI_OPTIX_BRIGHTNESS_MAX;
    Color_Pulse.brightness              = MSI_OPTIX_BRIGHTNESS_MAX;
    Color_Pulse.speed_min               = MSI_OPTIX_SPEED_MIN;
    Color_Pulse.speed_max               = MSI_OPTIX_SPEED_MAX;
    Color_Pulse.speed                   = MSI_OPTIX_SPEED_MIN;
    modes.push_back(Color_Pulse);

    mode Color_Shift;
    Color_Shift.name                    = "Color Shift";
    Color_Shift.value                   = COLOR_SHIFT_MODE_VALUE;
    Color_Shift.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Color_Shift.color_mode              = MODE_COLORS_NONE;
    Color_Shift.brightness_min          = MSI_OPTIX_BRIGHTNESS_MIN;
    Color_Shift.brightness_max          = MSI_OPTIX_BRIGHTNESS_MAX;
    Color_Shift.brightness              = MSI_OPTIX_BRIGHTNESS_MAX;
    Color_Shift.speed_min               = MSI_OPTIX_SPEED_MIN;
    Color_Shift.speed_max               = MSI_OPTIX_SPEED_MAX;
    Color_Shift.speed                   = MSI_OPTIX_SPEED_MIN;
    modes.push_back(Color_Shift);

    mode Color_wave;
    Color_wave.name                     = "Color Wave";
    Color_wave.value                    = COLOR_WAVE_MODE_VALUE;
    Color_wave.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Color_wave.color_mode               = MODE_COLORS_NONE;
    Color_wave.brightness_min           = MSI_OPTIX_BRIGHTNESS_MIN;
    Color_wave.brightness_max           = MSI_OPTIX_BRIGHTNESS_MAX;
    Color_wave.brightness               = MSI_OPTIX_BRIGHTNESS_MAX;
    Color_wave.speed_min                = MSI_OPTIX_SPEED_MIN;
    Color_wave.speed_max                = MSI_OPTIX_SPEED_MAX;
    Color_wave.speed                    = MSI_OPTIX_SPEED_MIN;
    modes.push_back(Color_wave);

    mode Marquee;
    Marquee.name                        = "Marquee";
    Marquee.value                       = MARQUEE_MODE_VALUE;
    Marquee.flags                       = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Marquee.color_mode                  = MODE_COLORS_MODE_SPECIFIC;
    Marquee.colors_min                  = 1;
    Marquee.colors_max                  = 1;
    Marquee.brightness_min              = MSI_OPTIX_BRIGHTNESS_MIN;
    Marquee.brightness_max              = MSI_OPTIX_BRIGHTNESS_MAX;
    Marquee.brightness                  = MSI_OPTIX_BRIGHTNESS_MAX;
    Marquee.speed_min                   = MSI_OPTIX_SPEED_MIN;
    Marquee.speed_max                   = MSI_OPTIX_SPEED_MAX;
    Marquee.speed                       = MSI_OPTIX_SPEED_MIN;
    Marquee.colors.resize(1);
    Marquee.colors[0]                   = MSI_OPTIX_DEFAULT_MODE_COLOR;
    modes.push_back(Marquee);

    mode Rainbow_Wave;
    Rainbow_Wave.name                   = "Rainbow Wave";
    Rainbow_Wave.value                  = RAINBOW_WAVE_MODE_VALUE;
    Rainbow_Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow_Wave.color_mode             = MODE_COLORS_NONE;
    Rainbow_Wave.brightness_min         = MSI_OPTIX_BRIGHTNESS_MIN;
    Rainbow_Wave.brightness_max         = MSI_OPTIX_BRIGHTNESS_MAX;
    Rainbow_Wave.brightness             = MSI_OPTIX_BRIGHTNESS_MAX;
    Rainbow_Wave.speed_min              = MSI_OPTIX_SPEED_MIN;
    Rainbow_Wave.speed_max              = MSI_OPTIX_SPEED_MAX;
    Rainbow_Wave.speed                  = MSI_OPTIX_SPEED_MIN;
    modes.push_back(Rainbow_Wave);

    mode Visor;
    Visor.name                          = "Visor";
    Visor.value                         = VISOR_MODE_VALUE;
    Visor.flags                         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Visor.color_mode                    = MODE_COLORS_NONE;
    Visor.brightness_min                = MSI_OPTIX_BRIGHTNESS_MIN;
    Visor.brightness_max                = MSI_OPTIX_BRIGHTNESS_MAX;
    Visor.brightness                    = MSI_OPTIX_BRIGHTNESS_MAX;
    Visor.speed_min                     = MSI_OPTIX_SPEED_MIN;
    Visor.speed_max                     = MSI_OPTIX_SPEED_MAX;
    Visor.speed                         = MSI_OPTIX_SPEED_MIN;
    modes.push_back(Visor);

    SetupZones();
}

RGBController_MSIOptix::~RGBController_MSIOptix()
{
    delete controller;
}

void RGBController_MSIOptix::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Backside";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = MSI_OPTIX_NUMBER_OF_LEDS;
    new_zone.leds_max   = MSI_OPTIX_NUMBER_OF_LEDS;
    new_zone.leds_count = MSI_OPTIX_NUMBER_OF_LEDS;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    leds.resize(new_zone.leds_count);

    for(unsigned int i = 0; i < MSI_OPTIX_NUMBER_OF_LEDS; i++)
    {
        leds[i].name = "LED " + std::to_string(i);
    }

    SetupColors();
}

void RGBController_MSIOptix::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_MSIOptix::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
}

void RGBController_MSIOptix::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetDirect(colors, modes[active_mode].brightness);
}

void RGBController_MSIOptix::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_MSIOptix::DeviceUpdateMode()
{
    if(modes[active_mode].flags & MODE_FLAG_HAS_PER_LED_COLOR)
    {
        controller->SetMode(colors, modes[active_mode].brightness, modes[active_mode].speed, modes[active_mode].value, modes[active_mode].flags);
    }
    else
    {
        controller->SetMode(modes[active_mode].colors, modes[active_mode].brightness, modes[active_mode].speed, modes[active_mode].value, modes[active_mode].flags);
    }
}
