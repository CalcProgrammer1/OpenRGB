/*---------------------------------------------------------*\
| RGBController_AnnePro2.cpp                                |
|                                                           |
|   RGBController for Obins Lab AnnePro2 keyboard           |
|                                                           |
|   Sergey Gavrilov (DrZlo13)                   06 Jun 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_AnnePro2.h"

#define NA              0xFFFFFFFF
#define LED_REAL_COUNT  (5*14)
#define LED_COUNT       (LED_REAL_COUNT - 9)

static unsigned int matrix_map[5][14] =
    { { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13 },
      { 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 },
      { 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, NA },
      { 41, NA, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, NA },
      { 53, NA, 54, 55, NA, NA, 56, NA, NA, 57, 58, 59, 60, NA } };

static const char* zone_names[] =
{
    ZONE_EN_KEYBOARD,
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX
};

static const unsigned int zone_sizes[] =
{
    LED_COUNT,
};

static const char* led_names[] =
{
    KEY_EN_ESCAPE,
    KEY_EN_1,
    KEY_EN_2,
    KEY_EN_3,
    KEY_EN_4,
    KEY_EN_5,
    KEY_EN_6,
    KEY_EN_7,
    KEY_EN_8,
    KEY_EN_9,
    KEY_EN_0,
    KEY_EN_MINUS,
    KEY_EN_EQUALS,
    KEY_EN_BACKSPACE,
    KEY_EN_TAB,
    KEY_EN_Q,
    KEY_EN_W,
    KEY_EN_E,
    KEY_EN_R,
    KEY_EN_T,
    KEY_EN_Y,
    KEY_EN_U,
    KEY_EN_I,
    KEY_EN_O,
    KEY_EN_P,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_CAPS_LOCK,
    KEY_EN_A,
    KEY_EN_S,
    KEY_EN_D,
    KEY_EN_F,
    KEY_EN_G,
    KEY_EN_H,
    KEY_EN_J,
    KEY_EN_K,
    KEY_EN_L,
    KEY_EN_SEMICOLON,
    KEY_EN_QUOTE,
    KEY_EN_ANSI_ENTER,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_Z,
    KEY_EN_X,
    KEY_EN_C,
    KEY_EN_V,
    KEY_EN_B,
    KEY_EN_N,
    KEY_EN_M,
    KEY_EN_COMMA,
    KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_SPACE,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_MENU,
    KEY_EN_RIGHT_CONTROL,
};

/**------------------------------------------------------------------*\
    @name Anne Pro 2
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectAnnePro2Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AnnePro2::RGBController_AnnePro2(AnnePro2Controller* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Anne Pro 2";
    vendor      = "Obinslab";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Obinslab Anne Pro 2 Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_AnnePro2::~RGBController_AnnePro2()
{
    Shutdown();

    delete controller;
}

void RGBController_AnnePro2::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        zone new_zone;
        new_zone.name                   = zone_names[zone_idx];
        new_zone.type                   = zone_types[zone_idx];
        new_zone.leds_min               = zone_sizes[zone_idx];
        new_zone.leds_max               = zone_sizes[zone_idx];
        new_zone.leds_count             = zone_sizes[zone_idx];

        if(zone_types[zone_idx] == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map.Set(5, 14, (unsigned int *)&matrix_map);
        }

        zones.push_back(new_zone);

        total_led_count += zone_sizes[zone_idx];
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name  = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_AnnePro2::DeviceUpdateLEDs()
{
    const unsigned char frame_buf_length = LED_REAL_COUNT * 3;
    unsigned char frame_buf[frame_buf_length];

    /*---------------------------------------------------------*\
    | TODO: Send packets with multiple LED frames               |
    \*---------------------------------------------------------*/
    std::size_t led_real_idx = 0;
    for(std::size_t led_idx = 0; led_idx < colors.size(); led_idx++)
    {
        frame_buf[(led_real_idx * 3) + 0] = RGBGetRValue(colors[led_idx]);
        frame_buf[(led_real_idx * 3) + 1] = RGBGetGValue(colors[led_idx]);
        frame_buf[(led_real_idx * 3) + 2] = RGBGetBValue(colors[led_idx]);

        if(led_idx == 40 || led_idx == 41 || led_idx == 52 || led_idx == 53 || led_idx == 60)
        {
            led_real_idx++;
        }
        else if(led_idx == 55 || led_idx == 56)
        {
            led_real_idx++;
            led_real_idx++;
        }

        led_real_idx++;
    }

    controller->SendDirect(frame_buf_length, frame_buf);
}

void RGBController_AnnePro2::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AnnePro2::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AnnePro2::DeviceUpdateMode()
{

}
