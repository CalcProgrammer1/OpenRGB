/*---------------------------------------------------------*\
| RGBController_LianLiGAIITrinity.cpp                       |
|                                                           |
|   RGBController for Lian Li GAII Trinity                  |
|                                                           |
|   Michael Losert                              27 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "RGBController_LianLiGAIITrinity.h"

/**------------------------------------------------------------------*\
    @name Lian Li GAII Trinity
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectLianLiGAIITrinity
    @comment
\*-------------------------------------------------------------------*/

RGBController_LianLiGAIITrinity::RGBController_LianLiGAIITrinity(LianLiGAIITrinityController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Lian Li GAII Trinity";
    vendor      = "Lian Li";
    type        = DEVICE_TYPE_COOLER;
    description = "Lian Li Galahad II Trinity AIO";
    location    = controller->GetLocation();

    LianLiGAIITrinityController::GAII_Info controllerInfo = controller->GetControllerInfo();
    version     = controllerInfo.version;
    serial      = controllerInfo.serial;

    mode Rainbow;
    Rainbow.name                 = "Rainbow";
    Rainbow.value                = LianLiGAIITrinityController::GAII_Modes::M_RAINBOW;
    Rainbow.flags                = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Rainbow.color_mode           = MODE_COLORS_RANDOM;
    Rainbow.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    Rainbow.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    Rainbow.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_75;
    Rainbow.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    Rainbow.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    Rainbow.speed                = LianLiGAIITrinityController::GAII_Speed::S_FAST;
    Rainbow.direction            = MODE_DIRECTION_RIGHT;
    modes.push_back(Rainbow);

    mode RainbowMorph;
    RainbowMorph.name                 = "Rainbow Morph";
    RainbowMorph.value                = LianLiGAIITrinityController::GAII_Modes::M_RAINBOW_MORPH;
    RainbowMorph.flags                = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    RainbowMorph.color_mode           = MODE_COLORS_RANDOM;
    RainbowMorph.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    RainbowMorph.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    RainbowMorph.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_75;
    RainbowMorph.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    RainbowMorph.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    RainbowMorph.speed                = LianLiGAIITrinityController::GAII_Speed::S_SLOW;
    modes.push_back(RainbowMorph);

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = LianLiGAIITrinityController::GAII_Modes::M_STATIC_COLOR;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    Direct.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    Direct.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    Direct.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_50;
    modes.push_back(Direct);

    mode BreathingColor;
    BreathingColor.name                 = "Breathing Color";
    BreathingColor.value                = LianLiGAIITrinityController::GAII_Modes::M_BREATHING_COLOR;
    BreathingColor.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    BreathingColor.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    BreathingColor.colors.resize(2);
    BreathingColor.colors[0]            = ToRGBColor(255, 255, 255);
    BreathingColor.colors[1]            = ToRGBColor(255, 0, 0);
    BreathingColor.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    BreathingColor.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    BreathingColor.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_100;
    BreathingColor.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    BreathingColor.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    BreathingColor.speed                = LianLiGAIITrinityController::GAII_Speed::S_MODERATE;
    modes.push_back(BreathingColor);

    mode Runway;
    Runway.name                 = "Runway";
    Runway.value                = LianLiGAIITrinityController::GAII_Modes::M_RUNWAY;
    Runway.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Runway.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Runway.colors.resize(2);
    Runway.colors[0]            = ToRGBColor(0, 0, 0);
    Runway.colors[1]            = ToRGBColor(255, 255, 255);
    Runway.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    Runway.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    Runway.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_50;
    Runway.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    Runway.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    Runway.speed                = LianLiGAIITrinityController::GAII_Speed::S_FAST;
    modes.push_back(Runway);

    mode Meteor;
    Meteor.name                 = "Meteor";
    Meteor.value                = LianLiGAIITrinityController::GAII_Modes::M_METEOR;
    Meteor.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Meteor.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Meteor.colors.resize(4);
    Meteor.colors[0]            = ToRGBColor(50, 50, 50);
    Meteor.colors[1]            = ToRGBColor(100, 100, 100);
    Meteor.colors[2]            = ToRGBColor(180, 180, 180);
    Meteor.colors[3]            = ToRGBColor(255, 0, 0);
    Meteor.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    Meteor.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    Meteor.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_100;
    Meteor.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    Meteor.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    Meteor.speed                = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    Meteor.direction            = MODE_DIRECTION_RIGHT;
    modes.push_back(Meteor);

    mode Vortex;
    Vortex.name                 = "Vortex";
    Vortex.value                = LianLiGAIITrinityController::GAII_Modes::M_VORTEX;
    Vortex.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Vortex.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Vortex.colors.resize(4);
    Vortex.colors[0]            = ToRGBColor(100, 100, 100);
    Vortex.colors[1]            = ToRGBColor(0, 100, 0);
    Vortex.colors[2]            = ToRGBColor(255, 255, 255);
    Vortex.colors[3]            = ToRGBColor(255, 0, 0);
    Vortex.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    Vortex.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    Vortex.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_75;
    Vortex.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    Vortex.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    Vortex.speed                = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    Vortex.direction            = MODE_DIRECTION_RIGHT;
    modes.push_back(Vortex);

    mode CrossingOver;
    CrossingOver.name                 = "Crossing Over";
    CrossingOver.value                = LianLiGAIITrinityController::GAII_Modes::M_CROSSING_OVER;
    CrossingOver.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    CrossingOver.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    CrossingOver.colors.resize(4);
    CrossingOver.colors[0]            = ToRGBColor(255, 0, 0);
    CrossingOver.colors[1]            = ToRGBColor(0, 255, 0);
    CrossingOver.colors[2]            = ToRGBColor(0, 0, 255);
    CrossingOver.colors[3]            = ToRGBColor(255, 255, 0);
    CrossingOver.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    CrossingOver.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    CrossingOver.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_75;
    CrossingOver.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    CrossingOver.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    CrossingOver.speed                = LianLiGAIITrinityController::GAII_Speed::S_FAST;
    CrossingOver.direction            = MODE_DIRECTION_RIGHT;
    modes.push_back(CrossingOver);

    mode TaiChi;
    TaiChi.name                 = "Tai Chi";
    TaiChi.value                = LianLiGAIITrinityController::GAII_Modes::M_TAI_CHI;
    TaiChi.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    TaiChi.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    TaiChi.colors.resize(2);
    TaiChi.colors[0]            = ToRGBColor(255, 0, 0);
    TaiChi.colors[1]            = ToRGBColor(0, 255, 0);
    TaiChi.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    TaiChi.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    TaiChi.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_75;
    TaiChi.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    TaiChi.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    TaiChi.speed                = LianLiGAIITrinityController::GAII_Speed::S_MODERATE;
    TaiChi.direction            = MODE_DIRECTION_RIGHT;
    modes.push_back(TaiChi);

    mode ColorfulStarryNight;
    ColorfulStarryNight.name                 = "Colorful Starry Night";
    ColorfulStarryNight.value                = LianLiGAIITrinityController::GAII_Modes::M_COLORFUL_STARRY_NIGHT;
    ColorfulStarryNight.flags                = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    ColorfulStarryNight.color_mode           = MODE_COLORS_RANDOM;
    ColorfulStarryNight.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    ColorfulStarryNight.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    ColorfulStarryNight.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_50;
    ColorfulStarryNight.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    ColorfulStarryNight.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    ColorfulStarryNight.speed                = LianLiGAIITrinityController::GAII_Speed::S_SLOW;
    modes.push_back(ColorfulStarryNight);

    mode StaticStarryNight;
    StaticStarryNight.name                 = "Static Starry Night";
    StaticStarryNight.value                = LianLiGAIITrinityController::GAII_Modes::M_STATIC_STARRY_NIGHT;
    StaticStarryNight.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    StaticStarryNight.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    StaticStarryNight.colors.resize(1);
    StaticStarryNight.colors[0]            = ToRGBColor(255, 255, 0);
    StaticStarryNight.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    StaticStarryNight.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    StaticStarryNight.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_50;
    StaticStarryNight.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    StaticStarryNight.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    StaticStarryNight.speed                = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    modes.push_back(StaticStarryNight);

    mode Voice;
    Voice.name                 = "Voice";
    Voice.value                = LianLiGAIITrinityController::GAII_Modes::M_VOICE;
    Voice.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Voice.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Voice.colors.resize(2);
    Voice.colors[0]            = ToRGBColor(255, 255, 255);
    Voice.colors[1]            = ToRGBColor(130, 130, 130);
    Voice.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    Voice.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    Voice.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_100;
    Voice.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    Voice.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    Voice.speed                = LianLiGAIITrinityController::GAII_Speed::S_SLOW;
    modes.push_back(Voice);


    mode BigBang;
    BigBang.name                 = "Big Bang";
    BigBang.value                = LianLiGAIITrinityController::GAII_Modes::M_BIG_BANG;
    BigBang.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    BigBang.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    BigBang.colors.resize(4);
    BigBang.colors[0]            = ToRGBColor(255, 255, 255);
    BigBang.colors[1]            = ToRGBColor(255, 0, 0);
    BigBang.colors[2]            = ToRGBColor(255, 255, 255);
    BigBang.colors[3]            = ToRGBColor(0, 255, 0);
    BigBang.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    BigBang.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    BigBang.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_75;
    BigBang.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    BigBang.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    BigBang.speed                = LianLiGAIITrinityController::GAII_Speed::S_FAST;
    modes.push_back(BigBang);

    mode Pump;
    Pump.name                 = "Pump";
    Pump.value                = LianLiGAIITrinityController::GAII_Modes::M_PUMP;
    Pump.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Pump.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Pump.colors.resize(2);
    Pump.colors[0]            = ToRGBColor(0, 255, 0);
    Pump.colors[1]            = ToRGBColor(150, 150, 150);
    Pump.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    Pump.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    Pump.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_75;
    Pump.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    Pump.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    Pump.speed                = LianLiGAIITrinityController::GAII_Speed::S_FAST;
    Pump.direction            = MODE_DIRECTION_RIGHT;
    modes.push_back(Pump);

    mode ColorsMorph;
    ColorsMorph.name                 = "Colors Morph";
    ColorsMorph.value                = LianLiGAIITrinityController::GAII_Modes::M_COLORS_MORPH;
    ColorsMorph.flags                = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    ColorsMorph.color_mode           = MODE_COLORS_RANDOM;
    ColorsMorph.brightness_min       = LianLiGAIITrinityController::GAII_Brightness::B_OFF;
    ColorsMorph.brightness_max       = LianLiGAIITrinityController::GAII_Brightness::B_100;
    ColorsMorph.brightness           = LianLiGAIITrinityController::GAII_Brightness::B_100;
    ColorsMorph.speed_min            = LianLiGAIITrinityController::GAII_Speed::S_VERY_SLOW;
    ColorsMorph.speed_max            = LianLiGAIITrinityController::GAII_Speed::S_VERY_FAST;
    ColorsMorph.speed                = LianLiGAIITrinityController::GAII_Speed::S_MODERATE;
    ColorsMorph.direction            = MODE_DIRECTION_RIGHT;
    modes.push_back(ColorsMorph);

    SetupZones();
}

RGBController_LianLiGAIITrinity::~RGBController_LianLiGAIITrinity()
{
    delete controller;
}

void RGBController_LianLiGAIITrinity::SetupZones()
{
    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/

    zone gaii_trinity;
    gaii_trinity.name          = "GAII Trinity";
    gaii_trinity.type          = ZONE_TYPE_SINGLE;
    gaii_trinity.leds_min      = 2;
    gaii_trinity.leds_max      = 2;
    gaii_trinity.leds_count    = 2;
    gaii_trinity.matrix_map    = NULL;
    zones.push_back(gaii_trinity);

    led inner_led;
    inner_led.name = "Inner Ring LEDs";
    leds.push_back(inner_led);

    led outer_led;
    outer_led.name = "Outer Ring LEDs";
    leds.push_back(outer_led);

    SetupColors();

    // set default color values
    zones[0].colors[0] = ToRGBColor(255, 255, 255);
    zones[0].colors[1] = ToRGBColor(0, 0, 255);
}

void RGBController_LianLiGAIITrinity::ResizeZone(int /* zone */, int /* new_size */)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LianLiGAIITrinity::DeviceUpdateLEDs()
{
    switch(modes[active_mode].value)
    {
        case LianLiGAIITrinityController::GAII_Modes::M_RAINBOW:
        controller->SetMode_Rainbow(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                    static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed),
                                    OpenRGBDirection2GAIIDirection(modes[active_mode].direction));
        break;
        case LianLiGAIITrinityController::GAII_Modes::M_RAINBOW_MORPH:
        controller->SetMode_RainbowMorph(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                         static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed));
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_STATIC_COLOR:
        controller->SetMode_StaticColor(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                        zones[0].colors[0], zones[0].colors[1]);
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_BREATHING_COLOR:
        controller->SetMode_BreathingColor(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                           static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed),
                                           modes[active_mode].colors[0], modes[active_mode].colors[1]);
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_RUNWAY:
        controller->SetMode_Runway(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                   static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed),
                                   modes[active_mode].colors[0], modes[active_mode].colors[1]);
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_METEOR:
        controller->SetMode_Meteor(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                   static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed),
                                   OpenRGBDirection2GAIIDirection(modes[active_mode].direction),
                                   modes[active_mode].colors[0], modes[active_mode].colors[1], modes[active_mode].colors[2], modes[active_mode].colors[3]);
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_VORTEX:
        controller->SetMode_Vortex(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                   static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed),
                                   OpenRGBDirection2GAIIDirection(modes[active_mode].direction),
                                   modes[active_mode].colors[0], modes[active_mode].colors[1], modes[active_mode].colors[2], modes[active_mode].colors[3]);
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_CROSSING_OVER:
        controller->SetMode_CrossingOver(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                         static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed),
                                         OpenRGBDirection2GAIIDirection(modes[active_mode].direction),
                                         modes[active_mode].colors[0], modes[active_mode].colors[1], modes[active_mode].colors[2], modes[active_mode].colors[3]);
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_TAI_CHI:
        controller->SetMode_TaiChi(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                   static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed),
                                   OpenRGBDirection2GAIIDirection(modes[active_mode].direction),
                                   modes[active_mode].colors[0], modes[active_mode].colors[1]);
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_COLORFUL_STARRY_NIGHT:
        controller->SetMode_ColorfulStarryNight(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                                static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed));
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_STATIC_STARRY_NIGHT:
        controller->SetMode_StaticStarryNight(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                              static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed),
                                              modes[active_mode].colors[0]);
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_VOICE:
        controller->SetMode_Voice(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                  static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed),
                                  modes[active_mode].colors[0], modes[active_mode].colors[1]);
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_BIG_BANG:
        controller->SetMode_BigBang(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                    static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed),
                                    modes[active_mode].colors[0], modes[active_mode].colors[1], modes[active_mode].colors[2], modes[active_mode].colors[3]);
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_PUMP:
        controller->SetMode_Pump(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                 static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed),
                                 OpenRGBDirection2GAIIDirection(modes[active_mode].direction),
                                 modes[active_mode].colors[0], modes[active_mode].colors[1]);
        break;
    case LianLiGAIITrinityController::GAII_Modes::M_COLORS_MORPH:
        controller->SetMode_ColorsMorph(static_cast<LianLiGAIITrinityController::GAII_Brightness>(modes[active_mode].brightness),
                                        static_cast<LianLiGAIITrinityController::GAII_Speed>(modes[active_mode].speed),
                                        OpenRGBDirection2GAIIDirection(modes[active_mode].direction));
        break;
    }
}

void RGBController_LianLiGAIITrinity::UpdateZoneLEDs(int /* zone */)
{
    DeviceUpdateLEDs();
}

void RGBController_LianLiGAIITrinity::UpdateSingleLED(int /* led */)
{
    DeviceUpdateLEDs();
}

void RGBController_LianLiGAIITrinity::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
