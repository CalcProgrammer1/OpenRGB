/*---------------------------------------------------------*\
| RGBController_JGINYUEInternalUSBV2.cpp                    |
|                                                           |
|   RGBController for JGINYUE USB motherboard V2            |
|                                                           |
|   Tong R (tcr020)                           03 July 2024  |
|   Liu ShiMeng(Moon dream stars)               09 Aug 2023 |
|   Dongguan Yonghang Electronic Technology Co., Ltd        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "RGBController_JGINYUEInternalUSBV2.h"

#define JGINYUE_MAX_ZONES               2
#define JGINYUE_ADDRESSABLE_MAX_LEDS    100

/**------------------------------------------------------------------*\
    @name JGINYUEInternalUSBV2
    @category MotherBoard
    @type  USB
    @save  :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectJGINYUEInternalUSBV2
    @comment Insert multiline JGINYUEInternalUSBV2 comment here
\*--------------------------------------------------------------------*/

RGBController_JGINYUEInternalUSBV2::RGBController_JGINYUEInternalUSBV2(JGINYUEInternalUSBV2Controller* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetDeviceName();
    description                         = "JGINYUE USB ARGB Device";
    vendor                              = "JGINYUE";
    type                                = DEVICE_TYPE_MOTHERBOARD;
    location                            = controller->GetDeviceLocation();
    version                             = controller->GetDeviceFWVersion();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.value                        = JGINYUE_USB_V2_MODE_DIRECT;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name                            = "Off";
    Off.value                           = JGINYUE_USB_V2_MODE_OFF;
    Off.flags                           = 0;
    Off.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                         = "Static";
    Static.value                        = JGINYUE_USB_V2_MODE_STATIC;
    Static.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR|MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_max                   = 1;
    Static.colors_min                   = 1;
    Static.brightness                   = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Static.brightness_max               = JGINYUE_USB_BRIGHTNESS_MAX;
    Static.brightness_min               = JGINYUE_USB_BRIGHTNESS_MIN;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = JGINYUE_USB_V2_MODE_BREATHING;
    Breathing.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_max                = 1;
    Breathing.colors_min                = 1;
    Breathing.brightness                = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Breathing.brightness_max            = JGINYUE_USB_BRIGHTNESS_MAX;
    Breathing.brightness_min            = JGINYUE_USB_BRIGHTNESS_MIN;
    Breathing.speed                     = JGINYUE_USB_SPEED_DEFAULT;
    Breathing.speed_max                 = JGINYUE_USB_SPEED_MAX;
    Breathing.speed_min                 = JGINYUE_USB_SPEED_MIN;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Cycling;
    Cycling.name                        = "Cycling";
    Cycling.value                       = JGINYUE_USB_V2_MODE_CYCLING;
    Cycling.flags                       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Cycling.color_mode                  = MODE_COLORS_NONE;
    Cycling.brightness                  = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Cycling.brightness_max              = JGINYUE_USB_BRIGHTNESS_MAX;
    Cycling.brightness_min              = JGINYUE_USB_BRIGHTNESS_MIN;
    Cycling.speed                       = JGINYUE_USB_SPEED_DEFAULT;
    Cycling.speed_max                   = JGINYUE_USB_SPEED_MAX;
    Cycling.speed_min                   = JGINYUE_USB_SPEED_MIN;
    Cycling.direction                   = JGINYUE_DIRECTION_RIGHT;
    modes.push_back(Cycling);

    mode Random;
    Random.name                         = "Random";
    Random.value                        = JGINYUE_USB_V2_MODE_RANDOM;
    Random.flags                        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Random.color_mode                   = MODE_COLORS_NONE;
    Random.brightness                   = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Random.brightness_max               = JGINYUE_USB_BRIGHTNESS_MAX;
    Random.brightness_min               = JGINYUE_USB_BRIGHTNESS_MIN;
    Random.speed                        = JGINYUE_USB_SPEED_DEFAULT;
    Random.speed_max                    = JGINYUE_USB_SPEED_MAX;
    Random.speed_min                    = JGINYUE_USB_SPEED_MIN;
    modes.push_back(Random);

    mode Wave;
    Wave.name                           = "Wave";
    Wave.value                          = JGINYUE_USB_V2_MODE_WAVE;
    Wave.flags                          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS| MODE_FLAG_HAS_DIRECTION_LR;
    Wave.color_mode                     = MODE_COLORS_MODE_SPECIFIC;
    Wave.colors_max                     = 1;
    Wave.colors_min                     = 1;
    Wave.brightness                     = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Wave.brightness_max                 = JGINYUE_USB_BRIGHTNESS_MAX;
    Wave.brightness_min                 = JGINYUE_USB_BRIGHTNESS_MIN;
    Wave.speed                          = JGINYUE_USB_SPEED_DEFAULT;
    Wave.speed_max                      = JGINYUE_USB_SPEED_MAX;
    Wave.speed_min                      = JGINYUE_USB_SPEED_MIN;
    Wave.direction                      = JGINYUE_DIRECTION_LEFT;
    Wave.colors.resize(1);
    modes.push_back(Wave);

    mode Spring;
    Spring.name                         = "Spring";
    Spring.value                        = JGINYUE_USB_V2_MODE_SPRING;
    Spring.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS| MODE_FLAG_HAS_DIRECTION_LR;
    Spring.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Spring.colors_max                   = 1;
    Spring.colors_min                   = 1;
    Spring.brightness                   = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Spring.brightness_max               = JGINYUE_USB_BRIGHTNESS_MAX;
    Spring.brightness_min               = JGINYUE_USB_BRIGHTNESS_MIN;
    Spring.speed                        = JGINYUE_USB_SPEED_DEFAULT;
    Spring.speed_max                    = JGINYUE_USB_SPEED_MAX;
    Spring.speed_min                    = JGINYUE_USB_SPEED_MIN;
    Spring.direction                    = JGINYUE_DIRECTION_RIGHT;
    Spring.colors.resize(1);
    //modes.push_back(Spring);

    mode Water;
    Water.name                          = "Water";
    Water.value                         = JGINYUE_USB_V2_MODE_WATER;
    Water.flags                         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Water.color_mode                    = MODE_COLORS_MODE_SPECIFIC;
    Water.colors_max                    = 1;
    Water.colors_min                    = 1;
    Water.brightness                    = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Water.brightness_max                = JGINYUE_USB_BRIGHTNESS_MAX;
    Water.brightness_min                = JGINYUE_USB_BRIGHTNESS_MIN;
    Water.speed                         = JGINYUE_USB_SPEED_DEFAULT;
    Water.speed_max                     = JGINYUE_USB_SPEED_MAX;
    Water.speed_min                     = JGINYUE_USB_SPEED_MIN;
    Water.direction                     = MODE_DIRECTION_RIGHT;
    Water.colors.resize(1);
    modes.push_back(Water);

    mode Rainbow;
    Rainbow.name                        = "Rainbow";
    Rainbow.value                       = JGINYUE_USB_V2_MODE_RAINBOW;
    Rainbow.flags                       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Rainbow.color_mode                  = MODE_COLORS_NONE;
    Rainbow.brightness                  = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Rainbow.brightness_max              = JGINYUE_USB_BRIGHTNESS_MAX;
    Rainbow.brightness_min              = JGINYUE_USB_BRIGHTNESS_MIN;
    Rainbow.speed                       = JGINYUE_USB_SPEED_DEFAULT;
    Rainbow.speed_max                   = JGINYUE_USB_SPEED_MAX;
    Rainbow.speed_min                   = JGINYUE_USB_SPEED_MIN;
    Rainbow.direction                   = MODE_DIRECTION_RIGHT;
    modes.push_back(Rainbow);

    mode MulticolorCycling;
    MulticolorCycling.name              = "Multicolor CYCLING";
    MulticolorCycling.value             = JGINYUE_USB_V2_MODE_MULTICOLOR_CYCLING;
    MulticolorCycling.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    MulticolorCycling.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    MulticolorCycling.colors_max        = 8;
    MulticolorCycling.colors_min        = 1;
    MulticolorCycling.brightness        = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    MulticolorCycling.brightness_max    = JGINYUE_USB_BRIGHTNESS_MAX;
    MulticolorCycling.brightness_min    = JGINYUE_USB_BRIGHTNESS_MIN;
    MulticolorCycling.speed             = JGINYUE_USB_SPEED_DEFAULT;
    MulticolorCycling.speed_max         = JGINYUE_USB_SPEED_MAX;
    MulticolorCycling.speed_min         = JGINYUE_USB_SPEED_MIN;
    MulticolorCycling.direction         = MODE_DIRECTION_RIGHT;
    MulticolorCycling.colors.resize(8);
    modes.push_back(MulticolorCycling);

    mode Sunrise;
    Sunrise.name                        = "Sunrise";
    Sunrise.value                       = JGINYUE_USB_V2_MODE_SUNRISE;
    Sunrise.flags                       = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Sunrise.color_mode                  = MODE_COLORS_MODE_SPECIFIC;
    Sunrise.colors_max                  = 8;
    Sunrise.colors_min                  = 1;
    Sunrise.brightness                  = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Sunrise.brightness_max              = JGINYUE_USB_BRIGHTNESS_MAX;
    Sunrise.brightness_min              = JGINYUE_USB_BRIGHTNESS_MIN;
    Sunrise.speed                       = JGINYUE_USB_SPEED_DEFAULT;
    Sunrise.speed_max                   = JGINYUE_USB_SPEED_MAX;
    Sunrise.speed_min                   = JGINYUE_USB_SPEED_MIN;
    Sunrise.direction                   = MODE_DIRECTION_RIGHT;
    Sunrise.colors.resize(8);
    modes.push_back(Sunrise);

    mode Rotate_star;
    Rotate_star.name                    = "Rotate Star";
    Rotate_star.value                   = JGINYUE_USB_V2_MODE_ROTATE_STAR;
    Rotate_star.flags                   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Rotate_star.color_mode              = MODE_COLORS_MODE_SPECIFIC;
    Rotate_star.colors_max              = 8;
    Rotate_star.colors_min              = 1;
    Rotate_star.brightness              = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Rotate_star.brightness_max          = JGINYUE_USB_BRIGHTNESS_MAX;
    Rotate_star.brightness_min          = JGINYUE_USB_BRIGHTNESS_MIN;
    Rotate_star.speed                   = JGINYUE_USB_SPEED_DEFAULT;
    Rotate_star.speed_max               = JGINYUE_USB_SPEED_MAX;
    Rotate_star.speed_min               = JGINYUE_USB_SPEED_MIN;
    Rotate_star.direction               = MODE_DIRECTION_RIGHT;
    Rotate_star.colors.resize(8);
    modes.push_back(Rotate_star);

    mode Meteor;
    Meteor.name                         = "Meteor";
    Meteor.value                        = JGINYUE_USB_V2_MODE_METEOR;
    Meteor.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Meteor.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Meteor.colors_max                   = 1;
    Meteor.colors_min                   = 1;
    Meteor.brightness                   = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Meteor.brightness_max               = JGINYUE_USB_BRIGHTNESS_MAX;
    Meteor.brightness_min               = JGINYUE_USB_BRIGHTNESS_MIN;
    Meteor.speed                        = JGINYUE_USB_SPEED_DEFAULT;
    Meteor.speed_max                    = JGINYUE_USB_SPEED_MAX;
    Meteor.speed_min                    = JGINYUE_USB_SPEED_MIN;
    Meteor.direction                    = MODE_DIRECTION_RIGHT;
    Meteor.colors.resize(8);
    modes.push_back(Meteor);

    mode Cycling_Breathing;
    Cycling_Breathing.name              = "Cycling Breathing";
    Cycling_Breathing.value             = JGINYUE_USB_V2_MODE_CYCLING_BREATHING;
    Cycling_Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Cycling_Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Cycling_Breathing.colors_max        = 8;
    Cycling_Breathing.colors_min        = 1;
    Cycling_Breathing.brightness        = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Cycling_Breathing.brightness_max    = JGINYUE_USB_BRIGHTNESS_MAX;
    Cycling_Breathing.brightness_min    = JGINYUE_USB_BRIGHTNESS_MIN;
    Cycling_Breathing.speed             = JGINYUE_USB_SPEED_DEFAULT;
    Cycling_Breathing.speed_max         = JGINYUE_USB_SPEED_MAX;
    Cycling_Breathing.speed_min         = JGINYUE_USB_SPEED_MIN;
    Cycling_Breathing.direction         = MODE_DIRECTION_RIGHT;
    Cycling_Breathing.colors.resize(8);
    modes.push_back(Cycling_Breathing);

    mode Raining;
    Raining.name                        = "Raining";
    Raining.value                       = JGINYUE_USB_V2_MODE_CYCLING_RAINING;
    Raining.flags                       = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Raining.color_mode                  = MODE_COLORS_MODE_SPECIFIC;
    Raining.colors_max                  = 1;
    Raining.colors_min                  = 1;
    Raining.brightness                  = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Raining.brightness_max              = JGINYUE_USB_BRIGHTNESS_MAX;
    Raining.brightness_min              = JGINYUE_USB_BRIGHTNESS_MIN;
    Raining.speed                       = JGINYUE_USB_SPEED_DEFAULT;
    Raining.speed_max                   = JGINYUE_USB_SPEED_MAX;
    Raining.speed_min                   = JGINYUE_USB_SPEED_MIN;
    Raining.direction                   = MODE_DIRECTION_RIGHT;
    Raining.colors.resize(8);
    modes.push_back(Raining);

    mode MulticolorWater1;
    MulticolorWater1.name               = "Multicolor Water 1";
    MulticolorWater1.value              = JGINYUE_USB_V2_MODE_MULTICOLOR_WATER_1;
    MulticolorWater1.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    MulticolorWater1.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    MulticolorWater1.colors_max         = 8;
    MulticolorWater1.colors_min         = 1;
    MulticolorWater1.brightness         = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    MulticolorWater1.brightness_max     = JGINYUE_USB_BRIGHTNESS_MAX;
    MulticolorWater1.brightness_min     = JGINYUE_USB_BRIGHTNESS_MIN;
    MulticolorWater1.speed              = JGINYUE_USB_SPEED_DEFAULT;
    MulticolorWater1.speed_max          = JGINYUE_USB_SPEED_MAX;
    MulticolorWater1.speed_min          = JGINYUE_USB_SPEED_MIN;
    MulticolorWater1.direction          = JGINYUE_DIRECTION_LEFT;
    MulticolorWater1.colors.resize(8);
    modes.push_back(MulticolorWater1);


    mode MulticolorWater2;
    MulticolorWater2.name               = "Multicolor Water 2";
    MulticolorWater2.value              = JGINYUE_USB_V2_MODE_MULTICOLOR_WATER_2;
    MulticolorWater2.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    MulticolorWater2.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    MulticolorWater2.colors_max         = 8;
    MulticolorWater2.colors_min         = 1;
    MulticolorWater2.brightness         = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    MulticolorWater2.brightness_max     = JGINYUE_USB_BRIGHTNESS_MAX;
    MulticolorWater2.brightness_min     = JGINYUE_USB_BRIGHTNESS_MIN;
    MulticolorWater2.speed              = JGINYUE_USB_SPEED_DEFAULT;
    MulticolorWater2.speed_max          = JGINYUE_USB_SPEED_MAX;
    MulticolorWater2.speed_min          = JGINYUE_USB_SPEED_MIN;
    MulticolorWater2.direction          = JGINYUE_DIRECTION_LEFT;
    MulticolorWater2.colors.resize(8);
    modes.push_back(MulticolorWater2);

    mode Hourglass;
    Hourglass.name                      = "Hourglass";
    Hourglass.value                     = JGINYUE_USB_V2_MODE_HOURGLASS;
    Hourglass.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Hourglass.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Hourglass.colors_max                = 3;
    Hourglass.colors_min                = 3;
    Hourglass.brightness                = JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Hourglass.brightness_max            = JGINYUE_USB_BRIGHTNESS_MAX;
    Hourglass.brightness_min            = JGINYUE_USB_BRIGHTNESS_MIN;
    Hourglass.speed                     = JGINYUE_USB_SPEED_DEFAULT;
    Hourglass.speed_max                 = JGINYUE_USB_SPEED_MAX;
    Hourglass.speed_min                 = JGINYUE_USB_SPEED_MIN;
    Hourglass.direction                 = MODE_DIRECTION_RIGHT;
    Hourglass.colors.resize(8);
    //modes.push_back(Hourglass);

    InitZones();
}

void RGBController_JGINYUEInternalUSBV2::SetupZones()
{
    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    unsigned char normal_zone_count = controller->GetZoneCount();
    if((controller->support_Global_zone == true) && (normal_zone_count > 1))
    {
        normal_zone_count--;
        //TODO support_Global_zone
    }

    for(unsigned int zone_idx = 0; zone_idx < normal_zone_count; zone_idx++)
    {
        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name    = zones[zone_idx].name + " LED#" + std::to_string(led_idx + 1);
            new_led.value   = 0;
            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_JGINYUEInternalUSBV2::ResizeZone(int zone, int new_size)
{
    unsigned char area;

    area = controller->device_config[zone].Area_ID;

    zones[zone].leds_count = new_size;

    SetupZones();

    if(modes[active_mode].value == JGINYUE_USB_V2_MODE_DIRECT)
    {
        controller->DirectLEDControl(zones[zone].colors, new_size, area);
    }
    else
    {
        controller->WriteZoneMode(area,modes[active_mode].value, new_size,modes[active_mode].colors, modes[active_mode].speed, modes[active_mode].brightness, modes[active_mode].direction);
    }
}

void RGBController_JGINYUEInternalUSBV2::DeviceUpdateLEDs()
{
    unsigned char normal_zone_count = controller->GetZoneCount();

    if((controller->support_Global_zone == true) && (normal_zone_count > 1))
    {
        normal_zone_count--;
        //TODO support_Global_zone
    }

    for(int i = 0; i < normal_zone_count; i++)
    {
        UpdateZoneLEDs(i);
    }
}

void RGBController_JGINYUEInternalUSBV2::UpdateZoneLEDs(int zone)
{
    unsigned char area;
    area = controller->device_config[zone].Area_ID;

    controller->DirectLEDControl(zones[zone].colors, zones[zone].leds_count, area);
}

void RGBController_JGINYUEInternalUSBV2::UpdateSingleLED(int led)
{
    int zone;
    zone = leds[led].value;

    UpdateZoneLEDs(zone);
}

void RGBController_JGINYUEInternalUSBV2::DeviceUpdateMode()
{
    if(modes[active_mode].value == JGINYUE_USB_V2_MODE_DIRECT)
    {
        DeviceUpdateLEDs();
        return;
    }

    unsigned int Area_num = 0;
    if(controller->support_Global_zone == true)
    {
        Area_num = controller->GetZoneCount() - 1;
    }
    else
    {
        Area_num = controller->GetZoneCount();
    }
    for(unsigned int i = 0; i < Area_num; i++)
    {
        DeviceUpdateZoneMode((int)i);
    }
}

void RGBController_JGINYUEInternalUSBV2::DeviceUpdateZoneMode(int zone)
{
    unsigned char Area_ID = controller->device_config[zone].Area_ID;
    controller->WriteZoneMode(
        Area_ID,
        modes[active_mode].value,
        zones[zone].leds_count,
        modes[active_mode].colors,
        modes[active_mode].speed,
        modes[active_mode].brightness,
        modes[active_mode].direction);
}

void RGBController_JGINYUEInternalUSBV2::InitZones()
{
    unsigned char normal_zone_count = controller->GetZoneCount();
    zones.clear();
    zones.resize(normal_zone_count);

    if((controller->support_Global_zone == true) && (normal_zone_count > 1))
    {
        normal_zone_count--;
        //TODO support_Global_zone
    }

    for(size_t i = 0; i < normal_zone_count; i++)
    {
        zone * zone_to_init         = &(zones[i]);
        AreaConfigurationV2 * cfg   = &(controller->device_config[i]);

        zone_to_init->leds_min      = 0;
        zone_to_init->leds_max      = cfg->Max_LED_numbers;
        zone_to_init->leds_count    = 0;
        zone_to_init->type          = ZONE_TYPE_LINEAR;
        zone_to_init->matrix_map    = NULL;

        switch(cfg->Area_ID)
        {
            case JGINYUE_USB_V2_ARGB_STRIP_1:
                zone_to_init->name = "ARGB Strip Header 1";
                break;
            case JGINYUE_USB_V2_ARGB_STRIP_2:
                zone_to_init->name = "ARGB Strip Header 2";
                break;
            case JGINYUE_USB_V2_ARGB_FAN_1:
                zone_to_init->name = "ARGB Fan Header 1";
                break;
            case JGINYUE_USB_V2_ARGB_FAN_2:
                zone_to_init->name = "ARGB Fan Header 2";
                break;
            case JGINYUE_USB_V2_ARGB_FAN_3:
                zone_to_init->name = "ARGB Fan Header 3";
                break;
            case JGINYUE_USB_V2_ARGB_FAN_4:
                zone_to_init->name = "ARGB Fan Header 4";
                break;
            case JGINYUE_USB_V2_ARGB_FAN_5:
                zone_to_init->name = "ARGB Fan Header 5";
                break;
            default:
                zone_to_init->name = "Unknow Device";
                break;
        }
    }
    SetupZones();
}
