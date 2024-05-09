/*---------------------------------------------------------*\
| RGBController_AnnePro2.cpp                                |
|                                                           |
|   RGBController for Obins Lab AnnePro2 keyboard           |
|                                                           |
|   Sergey Gavrilov (DrZlo13)                   06 Jun 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

typedef struct
{
    const char *        name;
    const unsigned char idx;
} annepro2_led_type;

static const annepro2_led_type led_names[] =
{
    /* Key Label                Index  */
    { KEY_EN_ESCAPE,            0       },
    { KEY_EN_1,                 1       },
    { KEY_EN_2,                 2       },
    { KEY_EN_3,                 3       },
    { KEY_EN_4,                 4       },
    { KEY_EN_5,                 5       },
    { KEY_EN_6,                 6       },
    { KEY_EN_7,                 7       },
    { KEY_EN_8,                 8       },
    { KEY_EN_9,                 9       },
    { KEY_EN_0,                 10      },
    { KEY_EN_MINUS,             11      },
    { KEY_EN_EQUALS,            12      },
    { KEY_EN_BACKSPACE,         13      },
    { KEY_EN_TAB,               14      },
    { KEY_EN_Q,                 15      },
    { KEY_EN_W,                 16      },
    { KEY_EN_E,                 17      },
    { KEY_EN_R,                 18      },
    { KEY_EN_T,                 19      },
    { KEY_EN_Y,                 20      },
    { KEY_EN_U,                 21      },
    { KEY_EN_I,                 22      },
    { KEY_EN_O,                 23      },
    { KEY_EN_P,                 24      },
    { KEY_EN_LEFT_BRACKET,      25      },
    { KEY_EN_RIGHT_BRACKET,     26      },
    { KEY_EN_ANSI_BACK_SLASH,   27      },
    { KEY_EN_CAPS_LOCK,         28      },
    { KEY_EN_A,                 29      },
    { KEY_EN_S,                 30      },
    { KEY_EN_D,                 31      },
    { KEY_EN_F,                 32      },
    { KEY_EN_G,                 33      },
    { KEY_EN_H,                 34      },
    { KEY_EN_J,                 35      },
    { KEY_EN_K,                 36      },
    { KEY_EN_L,                 37      },
    { KEY_EN_SEMICOLON,         38      },
    { KEY_EN_QUOTE,             39      },
    { KEY_EN_ANSI_ENTER,        40      },
    { KEY_EN_LEFT_SHIFT,        41      },
    { KEY_EN_Z,                 42      },
    { KEY_EN_X,                 43      },
    { KEY_EN_C,                 44      },
    { KEY_EN_V,                 45      },
    { KEY_EN_B,                 46      },
    { KEY_EN_N,                 47      },
    { KEY_EN_M,                 48      },
    { KEY_EN_COMMA,             49      },
    { KEY_EN_PERIOD,            50      },
    { KEY_EN_FORWARD_SLASH,     51      },
    { KEY_EN_RIGHT_SHIFT,       52      },
    { KEY_EN_LEFT_CONTROL,      53      },
    { KEY_EN_LEFT_WINDOWS,      54      },
    { KEY_EN_LEFT_ALT,          55      },
    { KEY_EN_SPACE,             56      },
    { KEY_EN_RIGHT_ALT,         57      },
    { KEY_EN_RIGHT_FUNCTION,    58      },
    { KEY_EN_MENU,              59      },
    { KEY_EN_RIGHT_CONTROL,     60      },
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
            new_zone.matrix_map         = new matrix_map_type;
            new_zone.matrix_map->height = 5;
            new_zone.matrix_map->width  = 14;
            new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
        }
        else
        {
            new_zone.matrix_map         = NULL;
        }

        zones.push_back(new_zone);

        total_led_count += zone_sizes[zone_idx];
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name  = led_names[led_idx].name;
        new_led.value = led_names[led_idx].idx;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_AnnePro2::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AnnePro2::DeviceUpdateLEDs()
{
    const unsigned char frame_buf_length = LED_REAL_COUNT * 3;
    unsigned char frame_buf[frame_buf_length];

    /*---------------------------------------------------------*\
    | TODO: Send packets with multiple LED frames               |
    \*---------------------------------------------------------*/
    std::size_t led_real_idx = 0;
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
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

void RGBController_AnnePro2::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AnnePro2::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AnnePro2::DeviceUpdateMode()
{

}
