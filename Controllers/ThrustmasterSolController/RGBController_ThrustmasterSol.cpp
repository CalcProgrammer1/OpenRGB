/*---------------------------------------------------------*\
| RGBController_ThrustmasterSol.cpp                         |
|                                                           |
|   RGBController for Thrustmaster Sol series joysticks     |
|                                                           |
|   Ken Sanislo                                 02 Apr 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_ThrustmasterSol.h"

/**------------------------------------------------------------------*\
    @name Thrustmaster Sol
    @category Gamepad
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectThrustmasterSolControllers
    @comment Thrustmaster Sol series joystick RGB LED control. Supports
    Sol-R, Sol F16, and Sol F18 variants. Only Sol-R has been tested.
    Uses vendor-specific USB interface 1 (not HID).
\*-------------------------------------------------------------------*/

#define NA 0xFFFFFFFF

static unsigned int logo_matrix_map[2][2] =
{
    {  0,   3 },
    {  1,   2 },
};

static unsigned int ring_matrix_map[3][5] =
{
    { NA,   7,   0,   1,  NA },
    {  6,  NA,  NA,  NA,   2 },
    { NA,   5,   4,   3,  NA },
};

static unsigned int right_buttons_matrix_map[2][2] =
{
    {  0,   1 },
    {  2,   3 },
};

static unsigned int left_buttons_matrix_map[2][2] =
{
    {  0,   1 },
    {  2,   3 },
};

RGBController_ThrustmasterSol::RGBController_ThrustmasterSol(ThrustmasterSolController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller_ptr->GetDeviceName();
    vendor      = "Thrustmaster";
    type        = DEVICE_TYPE_GAMEPAD;
    description = "Thrustmaster Sol Series Joystick";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    /*---------------------------------------------------------*\
    | Read current EEPROM colors from device so the UI starts   |
    | with the actual hardware state rather than all-black      |
    \*---------------------------------------------------------*/
    std::vector<unsigned int>   hw_zones;
    std::vector<RGBColor>       hw_colors;
    controller->ReadColors(hw_zones, hw_colors);

    for(unsigned int i = 0; i < hw_zones.size(); i++)
    {
        for(unsigned int j = 0; j < leds.size(); j++)
        {
            if(leds[j].value == hw_zones[i])
            {
                colors[j] = hw_colors[i];
                break;
            }
        }
    }
}

RGBController_ThrustmasterSol::~RGBController_ThrustmasterSol()
{
    delete controller;
}

void RGBController_ThrustmasterSol::SetupZones()
{
    /*---------------------------------------------------------*\
    | Thumbstick zone (grip, 1 LED)                            |
    \*---------------------------------------------------------*/
    zone thumbstick_zone;
    thumbstick_zone.name       = "Thumbstick";
    thumbstick_zone.type       = ZONE_TYPE_SINGLE;
    thumbstick_zone.leds_min   = 1;
    thumbstick_zone.leds_max   = 1;
    thumbstick_zone.leds_count = 1;
    zones.push_back(thumbstick_zone);

    led thumbstick_led;
    thumbstick_led.name  = "Thumbstick";
    thumbstick_led.value = THRUSTMASTER_SOL_GRIP_FLAG | 0x00;
    leds.push_back(thumbstick_led);

    /*---------------------------------------------------------*\
    | TM Logo zone (3 LEDs)                                    |
    \*---------------------------------------------------------*/
    zone logo_zone;
    logo_zone.name               = "TM Logo";
    logo_zone.type               = ZONE_TYPE_MATRIX;
    logo_zone.leds_min           = 4;
    logo_zone.leds_max           = 4;
    logo_zone.leds_count         = 4;
    logo_zone.matrix_map.Set(2, 2, (unsigned int *)&logo_matrix_map);
    zones.push_back(logo_zone);

    led logo_top_left;
    logo_top_left.name  = "TM Logo Top Left";
    logo_top_left.value = 0x01;
    leds.push_back(logo_top_left);

    led logo_bottom_left;
    logo_bottom_left.name  = "TM Logo Bottom Left";
    logo_bottom_left.value = 0x02;
    leds.push_back(logo_bottom_left);

    led logo_bottom_right;
    logo_bottom_right.name  = "TM Logo Bottom Right";
    logo_bottom_right.value = 0x03;
    leds.push_back(logo_bottom_right);

    led logo_top_right;
    logo_top_right.name  = "TM Logo Top Right";
    logo_top_right.value = 0x00;
    leds.push_back(logo_top_right);

    /*---------------------------------------------------------*\
    | Left Buttons zone (4 LEDs: buttons 5-8)                   |
    \*---------------------------------------------------------*/
    zone left_buttons_zone;
    left_buttons_zone.name               = "Left Buttons";
    left_buttons_zone.type               = ZONE_TYPE_MATRIX;
    left_buttons_zone.leds_min           = 4;
    left_buttons_zone.leds_max           = 4;
    left_buttons_zone.leds_count         = 4;
    left_buttons_zone.matrix_map.Set(2, 2, (unsigned int *)&left_buttons_matrix_map);
    zones.push_back(left_buttons_zone);

    led btn5;
    btn5.name  = "Button 5";
    btn5.value = 0x11;
    leds.push_back(btn5);

    led btn6;
    btn6.name  = "Button 6";
    btn6.value = 0x10;
    leds.push_back(btn6);

    led btn7;
    btn7.name  = "Button 7";
    btn7.value = 0x12;
    leds.push_back(btn7);

    led btn8;
    btn8.name  = "Button 8";
    btn8.value = 0x13;
    leds.push_back(btn8);

    /*---------------------------------------------------------*\
    | Base Ring zone (8 LEDs, clockwise from top)               |
    \*---------------------------------------------------------*/
    zone ring_zone;
    ring_zone.name               = "Base Ring";
    ring_zone.type               = ZONE_TYPE_MATRIX;
    ring_zone.leds_min           = 8;
    ring_zone.leds_max           = 8;
    ring_zone.leds_count         = 8;
    ring_zone.matrix_map.Set(3, 5, (unsigned int *)&ring_matrix_map);
    zones.push_back(ring_zone);

    led ring_upper;
    ring_upper.name  = "Upper";
    ring_upper.value = 0x04;
    leds.push_back(ring_upper);

    led ring_upper_right;
    ring_upper_right.name  = "Upper Right";
    ring_upper_right.value = 0x05;
    leds.push_back(ring_upper_right);

    led ring_right;
    ring_right.name  = "Right";
    ring_right.value = 0x06;
    leds.push_back(ring_right);

    led ring_bottom_right;
    ring_bottom_right.name  = "Bottom Right";
    ring_bottom_right.value = 0x0B;
    leds.push_back(ring_bottom_right);

    led ring_bottom;
    ring_bottom.name  = "Bottom";
    ring_bottom.value = 0x0C;
    leds.push_back(ring_bottom);

    led ring_bottom_left;
    ring_bottom_left.name  = "Bottom Left";
    ring_bottom_left.value = 0x0D;
    leds.push_back(ring_bottom_left);

    led ring_left;
    ring_left.name  = "Left";
    ring_left.value = 0x0E;
    leds.push_back(ring_left);

    led ring_upper_left;
    ring_upper_left.name  = "Upper Left";
    ring_upper_left.value = 0x0F;
    leds.push_back(ring_upper_left);

    /*---------------------------------------------------------*\
    | Right Buttons zone (4 LEDs: buttons 16-19)                |
    \*---------------------------------------------------------*/
    zone right_buttons_zone;
    right_buttons_zone.name               = "Right Buttons";
    right_buttons_zone.type               = ZONE_TYPE_MATRIX;
    right_buttons_zone.leds_min           = 4;
    right_buttons_zone.leds_max           = 4;
    right_buttons_zone.leds_count         = 4;
    right_buttons_zone.matrix_map.Set(2, 2, (unsigned int *)&right_buttons_matrix_map);
    zones.push_back(right_buttons_zone);

    led btn17;
    btn17.name  = "Button 17";
    btn17.value = 0x07;
    leds.push_back(btn17);

    led btn16;
    btn16.name  = "Button 16";
    btn16.value = 0x08;
    leds.push_back(btn16);

    led btn19;
    btn19.name  = "Button 19";
    btn19.value = 0x0A;
    leds.push_back(btn19);

    led btn18;
    btn18.name  = "Button 18";
    btn18.value = 0x09;
    leds.push_back(btn18);

    SetupColors();
}

void RGBController_ThrustmasterSol::DeviceUpdateLEDs()
{
    unsigned int    led_zones[THRUSTMASTER_SOL_R_ZONE_COUNT];
    RGBColor        led_colors[THRUSTMASTER_SOL_R_ZONE_COUNT];

    for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        led_zones[led_idx]  = leds[led_idx].value;
        led_colors[led_idx] = colors[led_idx];
    }

    controller->SetLEDColors(led_zones, led_colors, static_cast<unsigned int>(leds.size()));
}

void RGBController_ThrustmasterSol::DeviceUpdateZoneLEDs(int zone)
{
    unsigned int start_idx = 0;
    unsigned int zone_size = 0;

    for(unsigned int z_idx = 0; z_idx < zones.size(); z_idx++)
    {
        if(z_idx == (unsigned int)zone)
        {
            zone_size = zones[z_idx].leds_count;
            break;
        }

        start_idx += zones[z_idx].leds_count;
    }

    unsigned int    led_zones[THRUSTMASTER_SOL_R_ZONE_COUNT];
    RGBColor        led_colors[THRUSTMASTER_SOL_R_ZONE_COUNT];

    for(unsigned int led_idx = 0; led_idx < zone_size; led_idx++)
    {
        unsigned int current_idx    = start_idx + led_idx;
        led_zones[led_idx]          = leds[current_idx].value;
        led_colors[led_idx]         = colors[current_idx];
    }

    controller->SetLEDColors(led_zones, led_colors, zone_size);
}

void RGBController_ThrustmasterSol::DeviceUpdateSingleLED(int led)
{
    controller->SetLEDColor(leds[led].value, colors[led]);
}

void RGBController_ThrustmasterSol::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_ThrustmasterSol::DeviceSaveMode()
{
    unsigned int    led_zones[THRUSTMASTER_SOL_R_ZONE_COUNT];
    RGBColor        led_colors[THRUSTMASTER_SOL_R_ZONE_COUNT];

    for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        led_zones[led_idx]  = leds[led_idx].value;
        led_colors[led_idx] = colors[led_idx];
    }

    controller->SaveColors(led_zones, led_colors, static_cast<unsigned int>(leds.size()));
}
