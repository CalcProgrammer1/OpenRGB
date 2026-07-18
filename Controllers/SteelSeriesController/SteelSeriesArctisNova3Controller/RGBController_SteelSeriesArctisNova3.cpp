/*---------------------------------------------------------*\
| RGBController_SteelSeriesArctisNova3.cpp                  |
|                                                           |
|   RGBController for SteelSeries Arctis Nova 3             |
|                                                           |
|   Tomasz Gorczyca (eldiablo123)               18 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/
#include "RGBController_SteelSeriesArctisNova3.h"

/**------------------------------------------------------------------*\
    @name Steelseries Arctis Nova 3
    @category Headset
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectSteelSeriesArctisNova3
    @comment
\*-------------------------------------------------------------------*/

RGBController_SteelSeriesArctisNova3::RGBController_SteelSeriesArctisNova3(SteelSeriesArctisNova3Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "SteelSeries";
    type                    = DEVICE_TYPE_HEADSET;
    description             = "SteelSeries Arctis Nova 3 Headset Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = ARCTIS_NOVA3_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode       = MODE_COLORS_PER_LED;

    mode Off;
    Off.name                = "Off";
    Off.value                = ARCTIS_NOVA3_MODE_OFF;
    Off.flags                = 0;
    Off.color_mode            = MODE_COLORS_NONE;

    mode Breathe;
    Breathe.name             = "Breathe";
    Breathe.value            = ARCTIS_NOVA3_MODE_BREATHE;
    Breathe.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Breathe.color_mode       = MODE_COLORS_PER_LED;
    Breathe.speed_min        = 1;
    Breathe.speed_max        = 10;
    Breathe.speed            = 9;

    mode ColorShiftRainbow;
    ColorShiftRainbow.name             = "Color Shift - Rainbow";
    ColorShiftRainbow.value            = ARCTIS_NOVA3_MODE_COLORSHIFT_RAINBOW;
    ColorShiftRainbow.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    ColorShiftRainbow.color_mode       = MODE_COLORS_NONE;
    ColorShiftRainbow.speed_min        = 1;
    ColorShiftRainbow.speed_max        = 10;
    ColorShiftRainbow.speed            = 6;

    mode ColorShiftOrange;
    ColorShiftOrange.name             = "Color Shift - Heat Orange";
    ColorShiftOrange.value            = ARCTIS_NOVA3_MODE_COLORSHIFT_ORANGE;
    ColorShiftOrange.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    ColorShiftOrange.color_mode       = MODE_COLORS_NONE;
    ColorShiftOrange.speed_min        = 1;
    ColorShiftOrange.speed_max        = 10;
    ColorShiftOrange.speed            = 6;

    mode ColorShiftBlue;
    ColorShiftBlue.name             = "Color Shift - Frost Blue";
    ColorShiftBlue.value            = ARCTIS_NOVA3_MODE_COLORSHIFT_BLUE;
    ColorShiftBlue.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    ColorShiftBlue.color_mode       = MODE_COLORS_NONE;
    ColorShiftBlue.speed_min        = 1;
    ColorShiftBlue.speed_max        = 10;
    ColorShiftBlue.speed            = 6;

    modes.push_back(Direct);
    modes.push_back(Off);
    modes.push_back(Breathe);
    modes.push_back(ColorShiftRainbow);
    modes.push_back(ColorShiftOrange);
    modes.push_back(ColorShiftBlue);

    SetupZones();
}

RGBController_SteelSeriesArctisNova3::~RGBController_SteelSeriesArctisNova3()
{
    delete controller;
}

void RGBController_SteelSeriesArctisNova3::SetupZones()
{
    zone zone1;
    zone1.name       = "Nova 3";
    zone1.type       = ZONE_TYPE_LINEAR;
    zone1.leds_min   = 2;
    zone1.leds_max   = 2;
    zone1.leds_count = 2;
    zones.push_back(zone1);

    led right_led;
    right_led.name = "Right";
    leds.push_back(right_led);

    led left_led;
    left_led.name = "Left";
    leds.push_back(left_led);

    SetupColors();
}

void RGBController_SteelSeriesArctisNova3::DeviceUpdateLEDs()
{
    for(unsigned int i = 0; i < leds.size(); i++)
    {
        DeviceUpdateSingleLED(i);
    }
}

void RGBController_SteelSeriesArctisNova3::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesArctisNova3::DeviceUpdateSingleLED(int led)
{
    unsigned char zone_id = (unsigned char)led;
    unsigned char speed   = (unsigned char)modes[active_mode].speed;

    switch(modes[active_mode].value)
    {
        case ARCTIS_NOVA3_MODE_OFF:
            controller->SetOff(zone_id);
            break;

        case ARCTIS_NOVA3_MODE_DIRECT:
            controller->SetDirect(zone_id, colors[led]);
            break;

        case ARCTIS_NOVA3_MODE_BREATHE:
            controller->SetBreathe(zone_id, colors[led], speed);
            break;

        case ARCTIS_NOVA3_MODE_COLORSHIFT_RAINBOW:
            controller->SetColorShiftRainbow(zone_id, speed);
            break;

        case ARCTIS_NOVA3_MODE_COLORSHIFT_ORANGE:
            controller->SetColorShiftOrange(zone_id, speed);
            break;

        case ARCTIS_NOVA3_MODE_COLORSHIFT_BLUE:
            controller->SetColorShiftBlue(zone_id, speed);
            break;
    }
}

void RGBController_SteelSeriesArctisNova3::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesArctisNova3::DeviceSaveMode()
{
    controller->Save(0);
    controller->Save(1);
}
