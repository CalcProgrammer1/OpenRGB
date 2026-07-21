/*---------------------------------------------------------*\
| RGBController_MSIMonitor72.cpp                            |
|                                                           |
|   RGBController for MSI monitors using the 0x72 dual      |
|   control block layout (MPG 322URX QD-OLED)               |
|                                                           |
|   Ken Sanislo                               19 Jul 2026   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include "RGBController_MSIMonitor72.h"

/**------------------------------------------------------------------*\
    @name MSIMonitor72
    @category Accessory
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectMSIMonitorController
    @comment Developed with MSI MPG 322URX QD-OLED
\*-------------------------------------------------------------------*/
RGBController_MSIMonitor72::RGBController_MSIMonitor72(MSIMonitorController* controller_ptr, unsigned int count, unsigned int direction, bool dual)
{
    controller                          = controller_ptr;
    led_count                           = count;
    reverse_order                       = (direction == MSI_MONITOR_72_DIR_RIGHT_TO_LEFT);
    dual_block                          = dual;

    name                                = controller->GetNameString();
    vendor                              = "MSI";
    type                                = DEVICE_TYPE_MONITOR;
    description                         = "MSI Monitor (Gaming Controller)";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    /*-----------------------------------------------------*\
    | This layout has no Advanced mode, so Direct is Static |
    | driven from the inline per LED colour arrays.         |
    \*-----------------------------------------------------*/
    mode Direct;
    Direct.name                         = "Direct";
    Direct.value                        = MSI_MONITOR_72_MODE_STATIC;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.brightness_min               = 0;
    Direct.brightness_max               = 100;
    Direct.brightness                   = 100;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                         = "Static";
    Static.value                        = MSI_MONITOR_72_MODE_STATIC;
    Static.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.colors_min                   = 1;
    Static.colors_max                   = 1;
    Static.colors.resize(1);
    Static.brightness_min               = 0;
    Static.brightness_max               = 100;
    Static.brightness                   = 100;
    Static.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = MSI_MONITOR_72_MODE_BREATHING;
    Breathing.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.colors_min                = 1;
    Breathing.colors_max                = 1;
    Breathing.colors.resize(1);
    Breathing.speed_min                 = MSI_MONITOR_72_SPEED_LOW;
    Breathing.speed_max                 = MSI_MONITOR_72_SPEED_HIGH;
    Breathing.speed                     = MSI_MONITOR_72_SPEED_MEDIUM;
    Breathing.brightness_min            = 0;
    Breathing.brightness_max            = 100;
    Breathing.brightness                = 100;
    Breathing.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name                       = "Flashing";
    Flashing.value                      = MSI_MONITOR_72_MODE_FLASHING;
    Flashing.flags                      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Flashing.colors_min                 = 2;
    Flashing.colors_max                 = 2;
    Flashing.colors.resize(2);
    Flashing.speed_min                  = MSI_MONITOR_72_SPEED_LOW;
    Flashing.speed_max                  = MSI_MONITOR_72_SPEED_HIGH;
    Flashing.speed                      = MSI_MONITOR_72_SPEED_MEDIUM;
    Flashing.brightness_min             = 0;
    Flashing.brightness_max             = 100;
    Flashing.brightness                 = 100;
    Flashing.color_mode                 = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Flashing);

    mode DoubleFlashing;
    DoubleFlashing.name                 = "Double Flashing";
    DoubleFlashing.value                = MSI_MONITOR_72_MODE_DOUBLE_FLASHING;
    DoubleFlashing.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    DoubleFlashing.colors_min           = 2;
    DoubleFlashing.colors_max           = 2;
    DoubleFlashing.colors.resize(2);
    DoubleFlashing.speed_min            = MSI_MONITOR_72_SPEED_LOW;
    DoubleFlashing.speed_max            = MSI_MONITOR_72_SPEED_HIGH;
    DoubleFlashing.speed                = MSI_MONITOR_72_SPEED_MEDIUM;
    DoubleFlashing.brightness_min       = 0;
    DoubleFlashing.brightness_max       = 100;
    DoubleFlashing.brightness           = 100;
    DoubleFlashing.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(DoubleFlashing);

    mode Lightning;
    Lightning.name                      = "Lightning";
    Lightning.value                     = MSI_MONITOR_72_MODE_LIGHTNING;
    Lightning.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Lightning.colors_min                = 1;
    Lightning.colors_max                = 1;
    Lightning.colors.resize(1);
    Lightning.speed_min                 = MSI_MONITOR_72_SPEED_LOW;
    Lightning.speed_max                 = MSI_MONITOR_72_SPEED_HIGH;
    Lightning.speed                     = MSI_MONITOR_72_SPEED_MEDIUM;
    Lightning.brightness_min            = 0;
    Lightning.brightness_max            = 100;
    Lightning.brightness                = 100;
    Lightning.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Lightning);

    mode Scanner;
    Scanner.name                        = "Scanner";
    Scanner.value                       = MSI_MONITOR_72_MODE_SCANNER;
    Scanner.flags                       = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Scanner.colors_min                  = 1;
    Scanner.colors_max                  = 1;
    Scanner.colors.resize(1);
    Scanner.speed_min                   = MSI_MONITOR_72_SPEED_LOW;
    Scanner.speed_max                   = MSI_MONITOR_72_SPEED_HIGH;
    Scanner.speed                       = MSI_MONITOR_72_SPEED_MEDIUM;
    Scanner.brightness_min              = 0;
    Scanner.brightness_max              = 100;
    Scanner.brightness                  = 100;
    Scanner.color_mode                  = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Scanner);

    mode Meteor;
    Meteor.name                         = "Meteor";
    Meteor.value                        = MSI_MONITOR_72_MODE_METEOR;
    Meteor.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Meteor.colors_min                   = 1;
    Meteor.colors_max                   = 1;
    Meteor.colors.resize(1);
    Meteor.speed_min                    = MSI_MONITOR_72_SPEED_LOW;
    Meteor.speed_max                    = MSI_MONITOR_72_SPEED_HIGH;
    Meteor.speed                        = MSI_MONITOR_72_SPEED_MEDIUM;
    Meteor.brightness_min               = 0;
    Meteor.brightness_max               = 100;
    Meteor.brightness                   = 100;
    Meteor.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Meteor);

    mode WaterDrop;
    WaterDrop.name                      = "Water Drop";
    WaterDrop.value                     = MSI_MONITOR_72_MODE_WATER_DROP;
    WaterDrop.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    WaterDrop.colors_min                = 1;
    WaterDrop.colors_max                = 1;
    WaterDrop.colors.resize(1);
    WaterDrop.speed_min                 = MSI_MONITOR_72_SPEED_LOW;
    WaterDrop.speed_max                 = MSI_MONITOR_72_SPEED_HIGH;
    WaterDrop.speed                     = MSI_MONITOR_72_SPEED_MEDIUM;
    WaterDrop.brightness_min            = 0;
    WaterDrop.brightness_max            = 100;
    WaterDrop.brightness                = 100;
    WaterDrop.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(WaterDrop);

    mode RainbowMSI;
    RainbowMSI.name                     = "Rainbow";
    RainbowMSI.value                    = MSI_MONITOR_72_MODE_RAINBOW_MSI;
    RainbowMSI.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    RainbowMSI.speed_min                = MSI_MONITOR_72_SPEED_LOW;
    RainbowMSI.speed_max                = MSI_MONITOR_72_SPEED_HIGH;
    RainbowMSI.speed                    = MSI_MONITOR_72_SPEED_MEDIUM;
    RainbowMSI.brightness_min           = 0;
    RainbowMSI.brightness_max           = 100;
    RainbowMSI.brightness               = 100;
    RainbowMSI.color_mode               = MODE_COLORS_NONE;
    modes.push_back(RainbowMSI);

    mode ColorRing;
    ColorRing.name                      = "Color Ring";
    ColorRing.value                     = MSI_MONITOR_72_MODE_COLOR_RING;
    ColorRing.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    ColorRing.colors_min                = 1;
    ColorRing.colors_max                = 1;
    ColorRing.colors.resize(1);
    ColorRing.speed_min                 = MSI_MONITOR_72_SPEED_LOW;
    ColorRing.speed_max                 = MSI_MONITOR_72_SPEED_HIGH;
    ColorRing.speed                     = MSI_MONITOR_72_SPEED_MEDIUM;
    ColorRing.brightness_min            = 0;
    ColorRing.brightness_max            = 100;
    ColorRing.brightness                = 100;
    ColorRing.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(ColorRing);

    mode Planetary;
    Planetary.name                      = "Planetary";
    Planetary.value                     = MSI_MONITOR_72_MODE_PLANETARY;
    Planetary.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Planetary.colors_min                = 1;
    Planetary.colors_max                = 1;
    Planetary.colors.resize(1);
    Planetary.speed_min                 = MSI_MONITOR_72_SPEED_LOW;
    Planetary.speed_max                 = MSI_MONITOR_72_SPEED_HIGH;
    Planetary.speed                     = MSI_MONITOR_72_SPEED_MEDIUM;
    Planetary.brightness_min            = 0;
    Planetary.brightness_max            = 100;
    Planetary.brightness                = 100;
    Planetary.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Planetary);

    mode DoubleMeteor;
    DoubleMeteor.name                   = "Double Meteor";
    DoubleMeteor.value                  = MSI_MONITOR_72_MODE_DOUBLE_METEOR;
    DoubleMeteor.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    DoubleMeteor.colors_min             = 1;
    DoubleMeteor.colors_max             = 1;
    DoubleMeteor.colors.resize(1);
    DoubleMeteor.speed_min              = MSI_MONITOR_72_SPEED_LOW;
    DoubleMeteor.speed_max              = MSI_MONITOR_72_SPEED_HIGH;
    DoubleMeteor.speed                  = MSI_MONITOR_72_SPEED_MEDIUM;
    DoubleMeteor.brightness_min         = 0;
    DoubleMeteor.brightness_max         = 100;
    DoubleMeteor.brightness             = 100;
    DoubleMeteor.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(DoubleMeteor);

    mode Energy;
    Energy.name                         = "Energy";
    Energy.value                        = MSI_MONITOR_72_MODE_ENERGY;
    Energy.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Energy.colors_min                   = 1;
    Energy.colors_max                   = 1;
    Energy.colors.resize(1);
    Energy.speed_min                    = MSI_MONITOR_72_SPEED_LOW;
    Energy.speed_max                    = MSI_MONITOR_72_SPEED_HIGH;
    Energy.speed                        = MSI_MONITOR_72_SPEED_MEDIUM;
    Energy.brightness_min               = 0;
    Energy.brightness_max               = 100;
    Energy.brightness                   = 100;
    Energy.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Energy);

    mode Blink;
    Blink.name                          = "Blink";
    Blink.value                         = MSI_MONITOR_72_MODE_BLINK;
    Blink.flags                         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Blink.colors_min                    = 1;
    Blink.colors_max                    = 1;
    Blink.colors.resize(1);
    Blink.speed_min                     = MSI_MONITOR_72_SPEED_LOW;
    Blink.speed_max                     = MSI_MONITOR_72_SPEED_HIGH;
    Blink.speed                         = MSI_MONITOR_72_SPEED_MEDIUM;
    Blink.brightness_min                = 0;
    Blink.brightness_max                = 100;
    Blink.brightness                    = 100;
    Blink.color_mode                    = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Blink);

    mode Clock;
    Clock.name                          = "Clock";
    Clock.value                         = MSI_MONITOR_72_MODE_CLOCK;
    Clock.flags                         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Clock.colors_min                    = 1;
    Clock.colors_max                    = 1;
    Clock.colors.resize(1);
    Clock.speed_min                     = MSI_MONITOR_72_SPEED_LOW;
    Clock.speed_max                     = MSI_MONITOR_72_SPEED_HIGH;
    Clock.speed                         = MSI_MONITOR_72_SPEED_MEDIUM;
    Clock.brightness_min                = 0;
    Clock.brightness_max                = 100;
    Clock.brightness                    = 100;
    Clock.color_mode                    = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Clock);

    mode ColorPulse;
    ColorPulse.name                     = "Color Pulse";
    ColorPulse.value                    = MSI_MONITOR_72_MODE_COLOR_PULSE;
    ColorPulse.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    ColorPulse.colors_min               = 1;
    ColorPulse.colors_max               = 1;
    ColorPulse.colors.resize(1);
    ColorPulse.speed_min                = MSI_MONITOR_72_SPEED_LOW;
    ColorPulse.speed_max                = MSI_MONITOR_72_SPEED_HIGH;
    ColorPulse.speed                    = MSI_MONITOR_72_SPEED_MEDIUM;
    ColorPulse.brightness_min           = 0;
    ColorPulse.brightness_max           = 100;
    ColorPulse.brightness               = 100;
    ColorPulse.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(ColorPulse);

    mode ColorShift;
    ColorShift.name                     = "Color Shift";
    ColorShift.value                    = MSI_MONITOR_72_MODE_COLOR_SHIFT;
    ColorShift.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    ColorShift.colors_min               = 2;
    ColorShift.colors_max               = 2;
    ColorShift.colors.resize(2);
    ColorShift.speed_min                = MSI_MONITOR_72_SPEED_LOW;
    ColorShift.speed_max                = MSI_MONITOR_72_SPEED_HIGH;
    ColorShift.speed                    = MSI_MONITOR_72_SPEED_MEDIUM;
    ColorShift.brightness_min           = 0;
    ColorShift.brightness_max           = 100;
    ColorShift.brightness               = 100;
    ColorShift.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(ColorShift);

    mode ColorWave;
    ColorWave.name                      = "Color Wave";
    ColorWave.value                     = MSI_MONITOR_72_MODE_COLOR_WAVE;
    ColorWave.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    ColorWave.colors_min                = 2;
    ColorWave.colors_max                = 2;
    ColorWave.colors.resize(2);
    ColorWave.speed_min                 = MSI_MONITOR_72_SPEED_LOW;
    ColorWave.speed_max                 = MSI_MONITOR_72_SPEED_HIGH;
    ColorWave.speed                     = MSI_MONITOR_72_SPEED_MEDIUM;
    ColorWave.brightness_min            = 0;
    ColorWave.brightness_max            = 100;
    ColorWave.brightness                = 100;
    ColorWave.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(ColorWave);

    /*-----------------------------------------------------*\
    | Mode 24 marquee behaviour is model dependent.  On the |
    | dual block / richer boards it wants the marker bytes  |
    | plus a per LED array of separate colours (a user      |
    | defined marquee).  The vendor app does not even offer |
    | it for version 34, but the firmware still runs it as  |
    | a one colour rolling band, so we expose that.         |
    \*-----------------------------------------------------*/
    mode Marquee;
    Marquee.name                        = "Marquee";
    Marquee.value                       = MSI_MONITOR_72_MODE_MARQUEE;
    Marquee.flags                       = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Marquee.colors_min                  = 1;
    Marquee.colors_max                  = 1;
    Marquee.colors.resize(1);
    Marquee.speed_min                   = MSI_MONITOR_72_SPEED_LOW;
    Marquee.speed_max                   = MSI_MONITOR_72_SPEED_HIGH;
    Marquee.speed                       = MSI_MONITOR_72_SPEED_MEDIUM;
    Marquee.brightness_min              = 0;
    Marquee.brightness_max              = 100;
    Marquee.brightness                  = 100;
    Marquee.color_mode                  = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Marquee);

    mode SpectrumCycle;
    SpectrumCycle.name                  = "Spectrum Cycle";
    SpectrumCycle.value                 = MSI_MONITOR_72_MODE_RAINBOW;
    SpectrumCycle.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    SpectrumCycle.speed_min             = MSI_MONITOR_72_SPEED_LOW;
    SpectrumCycle.speed_max             = MSI_MONITOR_72_SPEED_HIGH;
    SpectrumCycle.speed                 = MSI_MONITOR_72_SPEED_MEDIUM;
    SpectrumCycle.brightness_min        = 0;
    SpectrumCycle.brightness_max        = 100;
    SpectrumCycle.brightness            = 100;
    SpectrumCycle.color_mode            = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode RainbowWave;
    RainbowWave.name                    = "Rainbow Wave";
    RainbowWave.value                   = MSI_MONITOR_72_MODE_RAINBOW_WAVE;
    RainbowWave.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    RainbowWave.speed_min               = MSI_MONITOR_72_SPEED_LOW;
    RainbowWave.speed_max               = MSI_MONITOR_72_SPEED_HIGH;
    RainbowWave.speed                   = MSI_MONITOR_72_SPEED_MEDIUM;
    RainbowWave.brightness_min          = 0;
    RainbowWave.brightness_max          = 100;
    RainbowWave.brightness              = 100;
    RainbowWave.color_mode              = MODE_COLORS_NONE;
    modes.push_back(RainbowWave);

    mode Visor;
    Visor.name                          = "Visor";
    Visor.value                         = MSI_MONITOR_72_MODE_VISOR;
    Visor.flags                         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Visor.colors_min                    = 1;
    Visor.colors_max                    = 1;
    Visor.colors.resize(1);
    Visor.speed_min                     = MSI_MONITOR_72_SPEED_LOW;
    Visor.speed_max                     = MSI_MONITOR_72_SPEED_HIGH;
    Visor.speed                         = MSI_MONITOR_72_SPEED_MEDIUM;
    Visor.brightness_min                = 0;
    Visor.brightness_max                = 100;
    Visor.brightness                    = 100;
    Visor.color_mode                    = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Visor);

    mode JRainbow;
    JRainbow.name                       = "JRainbow";
    JRainbow.value                      = MSI_MONITOR_72_MODE_JRAINBOW;
    JRainbow.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    JRainbow.speed_min                  = MSI_MONITOR_72_SPEED_LOW;
    JRainbow.speed_max                  = MSI_MONITOR_72_SPEED_HIGH;
    JRainbow.speed                      = MSI_MONITOR_72_SPEED_MEDIUM;
    JRainbow.brightness_min             = 0;
    JRainbow.brightness_max             = 100;
    JRainbow.brightness                 = 100;
    JRainbow.color_mode                 = MODE_COLORS_NONE;
    modes.push_back(JRainbow);

    mode RainbowFlash;
    RainbowFlash.name                   = "Rainbow Flashing";
    RainbowFlash.value                  = MSI_MONITOR_72_MODE_RAINBOW_FLASH;
    RainbowFlash.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    RainbowFlash.speed_min              = MSI_MONITOR_72_SPEED_LOW;
    RainbowFlash.speed_max              = MSI_MONITOR_72_SPEED_HIGH;
    RainbowFlash.speed                  = MSI_MONITOR_72_SPEED_MEDIUM;
    RainbowFlash.brightness_min         = 0;
    RainbowFlash.brightness_max         = 100;
    RainbowFlash.brightness             = 100;
    RainbowFlash.color_mode             = MODE_COLORS_NONE;
    modes.push_back(RainbowFlash);

    mode RainbowDblFlash;
    RainbowDblFlash.name                = "Rainbow Double Flashing";
    RainbowDblFlash.value               = MSI_MONITOR_72_MODE_RAINBOW_DBLFLASH;
    RainbowDblFlash.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    RainbowDblFlash.speed_min           = MSI_MONITOR_72_SPEED_LOW;
    RainbowDblFlash.speed_max           = MSI_MONITOR_72_SPEED_HIGH;
    RainbowDblFlash.speed               = MSI_MONITOR_72_SPEED_MEDIUM;
    RainbowDblFlash.brightness_min      = 0;
    RainbowDblFlash.brightness_max      = 100;
    RainbowDblFlash.brightness          = 100;
    RainbowDblFlash.color_mode          = MODE_COLORS_NONE;
    modes.push_back(RainbowDblFlash);

    mode RainbowLine;
    RainbowLine.name                    = "Rainbow Line";
    RainbowLine.value                   = MSI_MONITOR_72_MODE_RAINBOW_LINE;
    RainbowLine.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    RainbowLine.speed_min               = MSI_MONITOR_72_SPEED_LOW;
    RainbowLine.speed_max               = MSI_MONITOR_72_SPEED_HIGH;
    RainbowLine.speed                   = MSI_MONITOR_72_SPEED_MEDIUM;
    RainbowLine.brightness_min          = 0;
    RainbowLine.brightness_max          = 100;
    RainbowLine.brightness              = 100;
    RainbowLine.color_mode              = MODE_COLORS_NONE;
    modes.push_back(RainbowLine);

    mode Off;
    Off.name                            = "Off";
    Off.value                           = MSI_MONITOR_72_MODE_OFF;
    Off.flags                           = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_MSIMonitor72::~RGBController_MSIMonitor72()
{
    Shutdown();

    delete controller;
}

void RGBController_MSIMonitor72::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Rear";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = led_count;
    new_zone.leds_max   = led_count;
    new_zone.leds_count = led_count;

    zones.emplace_back(new_zone);

    for(unsigned int led_idx = 0; led_idx < led_count; led_idx++)
    {
        led new_led;
        new_led.name = "LED " + std::to_string(led_idx + 1);
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_MSIMonitor72::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_MSIMonitor72::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSIMonitor72::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSIMonitor72::ApplyMode(bool save)
{
    const mode& active = modes[active_mode];

    RGBColor              color1        = 0;
    RGBColor              color2        = 0;
    bool                  user_palette  = false;
    std::vector<RGBColor> led_colors;

    if(active.color_mode == MODE_COLORS_PER_LED)
    {
        user_palette = true;
        led_colors   = colors;

        if(led_colors.size() > 0)
        {
            color1 = led_colors[0];
        }
    }
    else if(active.color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        user_palette = true;

        if(active.colors.size() > 0)
        {
            color1 = active.colors[0];

            if(active.colors.size() > 1)
            {
                color2 = active.colors[1];
            }
        }

        led_colors.assign(led_count, color1);
    }

    /*-----------------------------------------------------*\
    | RightToLeft layouts start at the right end, so mirror |
    | the per LED order to keep OpenRGB LED 0 on the left.  |
    | Uniform colour modes are unaffected by the order.     |
    \*-----------------------------------------------------*/
    if(reverse_order)
    {
        std::reverse(led_colors.begin(), led_colors.end());
    }

    controller->SetMode72((uint8_t)active.value, (uint8_t)active.speed, (uint8_t)active.brightness, color1, color2, user_palette, save, led_colors, dual_block);
}

void RGBController_MSIMonitor72::DeviceUpdateMode()
{
    /*-----------------------------------------------------*\
    | Updates are volatile.  Persisting to flash is the     |
    | Save To Device button (DeviceSaveMode), so live edits |
    | do not burn flash write cycles.                       |
    \*-----------------------------------------------------*/
    ApplyMode(false);
}

void RGBController_MSIMonitor72::DeviceSaveMode()
{
    ApplyMode(true);
}
