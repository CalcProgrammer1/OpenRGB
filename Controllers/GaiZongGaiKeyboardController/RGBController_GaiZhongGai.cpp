/*---------------------------------------------------------*\
| RGBController_GaiZhongGai.cpp                             |
|                                                           |
|   RGBController for GaiZhongGai keyboard                  |
|                                                           |
|   An Yang                                     24 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <math.h>
#include "RGBController_GaiZhongGai.h"
#include "RGBControllerKeyNames.h"

using namespace std;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map_68[5][17] =
{
    {  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  NA,  66,  67 },
    {  36,  NA,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51 },
    {  23,  NA,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  NA,  35,  NA,  NA },
    {  10,  NA,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  NA,  21,  NA,  22,  NA },
    {   0,   1,   2,  NA,  NA,  NA,   3,  NA,  NA,  NA,   4,   5,   6,  NA,   7,   8,   9 }
};

static unsigned int matrix_map_42[4][12] =
{
    {  41,  40,  39,  38,  37,  36,  35,  34,  33,  32,  31,  30 },
    {  29,  28,  27,  26,  25,  24,  23,  22,  21,  20,  NA,  19 },
    {  18,  NA,  17,  16,  15,  14,  13,  12,  11,  10,   9,   8 },
    {   7,   6,   5,  NA,   4,  NA,  NA,   3,  NA,   2,   1,   0 }
};

static unsigned int matrix_map_dial[2][2] =
{
    {  88, 85 },
    {  87, 86 }
};

static unsigned int matrix_map_17PAD[5][4] =
{
    {  84, 83, 82, 81 },
    {  80, 79, 78, 77 },
    {  76, 75, 74, NA },
    {  73, 72, 71, 68 },
    {  70, NA, 69, NA }
};

static unsigned int matrix_map_20PAD[6][4] =
{
    {  86, 87, 85, NA },
    {  84, 83, 82, 81 },
    {  80, 79, 78, 77 },
    {  76, 75, 74, NA },
    {  73, 72, 71, 68 },
    {  70, NA, 69, NA }
};

static unsigned int matrix_map_PAD_Touch[5][5] =
{
    {  84, 83, 82, 81 , NA},
    {  80, 79, 78, 77 , 85},
    {  76, 75, 74, NA , 86},
    {  73, 72, 71, 68 , 87},
    {  70, NA, 69, NA , NA}
};

static const char* zone_names[] =
{
    "Keyboard",
    "RGB HUB CH1",
    "RGB HUB CH2",
    "RGB HUB CH3",
    "RGB HUB CH4",
    "RGB HUB CH5",
    "RGB HUB CH6",
    "RGB HUB CH7",
    "RGB HUB CH8"
};

static const char *led_names_general[] =
{
    KEY_EN_LEFT_CONTROL,//0
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_SPACE,
    KEY_EN_RIGHT_ALT,
    KEY_EN_MENU,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW,

    KEY_EN_LEFT_SHIFT,//10
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
    KEY_EN_UP_ARROW,

    KEY_EN_CAPS_LOCK,//23
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

    KEY_EN_TAB,//36
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
    KEY_EN_DELETE,
    KEY_EN_PAGE_DOWN,

    KEY_EN_ESCAPE,//52
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
    KEY_EN_PRINT_SCREEN,
    KEY_EN_PAGE_UP,

    KEY_EN_NUMPAD_ENTER,//68
    KEY_EN_NUMPAD_PERIOD,
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_LOCK,

    "RGB Strip 1",
    "RGB Strip 2",
    "RGB Strip 3",
    "RGB Strip 4",
    "RGB Strip 5",
    "RGB Strip 6"
};

static const char *led_names_42key[] =
{
    KEY_EN_RIGHT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_LEFT_ARROW,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_SPACE,
    KEY_EN_LEFT_ALT,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_CONTROL,

    KEY_EN_RIGHT_SHIFT,
    KEY_EN_UP_ARROW,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_M,
    KEY_EN_N,
    KEY_EN_B,
    KEY_EN_V,
    KEY_EN_C,
    KEY_EN_X,
    KEY_EN_Z,
    KEY_EN_LEFT_SHIFT,

    KEY_EN_ANSI_ENTER,
    KEY_EN_L,
    KEY_EN_K,
    KEY_EN_J,
    KEY_EN_H,
    KEY_EN_G,
    KEY_EN_F,
    KEY_EN_D,
    KEY_EN_S,
    KEY_EN_A,
    KEY_EN_CAPS_LOCK,

    KEY_EN_BACKSPACE,
    KEY_EN_P,
    KEY_EN_O,
    KEY_EN_I,
    KEY_EN_U,
    KEY_EN_Y,
    KEY_EN_T,
    KEY_EN_R,
    KEY_EN_E,
    KEY_EN_W,
    KEY_EN_Q,
    KEY_EN_ESCAPE
};

/*---------------------------------------------------------*\
| Enumerated coordinates                                    |
\*---------------------------------------------------------*/
static unsigned int matrix_map_light_board[1024];
void board_led_xy_self_call
    (
    uint8_t* p_in,  uint8_t*  offset,
    unsigned int* map,
    uint16_t* led_len,
    float x,        float y,
    float distance, float angle
    )
{
    uint8_t i;
    uint8_t temp;
    uint8_t range_num = 0;      //cycle index
    float led_distance = 0;     //Light to center(cm)
    float board_distance = 0;   //Edge to center(cm)
    float angle_step = 0;       //Step radians
    float pi = acosf(-1.0f);    //PI
    float new_x, new_y;         //New center point coordinates
    float new_angle;

    if (*offset == 120)
        return;

    temp = p_in[*offset];
    switch(temp >> 5)
    {
    case 0x07://END
        return;

    case 0x01://regular hexagon
        led_distance = 3.0f;
        board_distance = 4.33f;
        range_num = 12;
        angle_step = pi / 6.0f;
    break;

    case 0x00://transferred meaning
        switch(temp >> 3)
        {
        case 0x01://square
            led_distance = 3.54f;
            board_distance = 5.0f;
            range_num = 8;
            angle_step = pi / 4.0f;
            break;
        case 0x00://transferred meaning
            if(temp >> 2 == 1)//regular triangle
            {
                led_distance = 2.8f;
                board_distance = 2.9f;
                range_num = 6;
                angle_step = pi / 3.0f;
            }
            break;
        }
        break;
    }
    //New center point coordinates
    new_x = x + cosf(angle) * (distance + board_distance);
    new_y = y + sinf(angle) * (distance + board_distance);

    //Rotate 180 degrees
    if(angle > pi)
    {
        new_angle = angle - pi;
    }
    else
    {
        new_angle = angle + pi;
    }

    for(i = 1; i < range_num; i++)
    {
        new_angle -= angle_step;//clockwise
        if(i & 1)//Is led
        {
            uint8_t x_u8 = (int16_t)(((new_x + cos(new_angle) * led_distance) *
                            p_in[120] * 0.01f + 0.5f) / 1) -
                            *(int8_t*)&p_in[122];

            uint8_t y_u8 = (int16_t)(((new_y + sin(new_angle) * led_distance) *
                            p_in[120] * 0.01f + 0.5f) / -1) -
                            *(int8_t*)&p_in[123];

            map[y_u8*p_in[124]+x_u8] = *led_len;

            (*led_len)++;
        }
        else//Is COM
        {
            if(temp & (1 << (i / 2 - 1)))//child node
            {
                (*offset)++;
                board_led_xy_self_call(
                    p_in, offset,
                    map, led_len,
                    new_x, new_y,
                    board_distance, new_angle
                );
            }
        }
    }
}

uint16_t LightBoard_init(uint8_t* p_in)
{
    for(uint16_t i = 0 ;i < 1024; i++)
    {
        matrix_map_light_board[i] = NA;
    }

    float angle;
    uint16_t led_len = 0;
    uint8_t offset = 0;

    angle = ((21 - p_in[121]) % 12) * acosf(-1.0f) / 6;

    board_led_xy_self_call(p_in, &offset,matrix_map_light_board , &led_len, 0, 0, 0, angle);

    return led_len;
}

/**------------------------------------------------------------------*\
    @name GaiZhongGai Keyboard/Controller
    @category  Keyboard/Controller
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectGaiZhongGaiKeyboardControllers
    @comment
        Open source web : https://oshwhub.com/yangzen/zui-gai68-

        | function                      | command code      | format    | format    | format    |  format                                   |
        | ----------------------------- | ----------------- | --------  | --------  | --------  | ----------------------------------------- |
        |                               |        [0]        |    [1]    |   [2]     |   [3]     |   [4]                                     |
        | ----------------------------- | ----------------- | --------  | --------  | --------  | ----------------------------------------- |
        | Restore offline effects       |   0xFF            | undefined | undefined | undefined | undefined                                 |
        | Set color                     |   0x10            | LED0_G    | LED0_R    | LED0_B    | LED1_G(And so on to LED20)                |
        | Set color                     |   0x11            | LED21_G   | LED21_R   | LED21_B   | LED22_G(The sequence is WS2812 network)   |
        | Get color                     |   0x20            | LED0_G    | LED0_R    | LED0_B    | LED1_G(And so on to LED20)                |
        | Get color                     |   0x21            | LED21_G   | LED21_R   | LED21_B   | LED22_G(The sequence is WS2812 network)   |

        Note: Get color only sends command code, and the keyboard returns LED color data
\*-------------------------------------------------------------------*/

RGBController_GaiZhongGaiKeyboard::RGBController_GaiZhongGaiKeyboard(GaiZhongGaiKeyboardController* controller_ptr)
{
    controller          = controller_ptr;
    name                = controller->GetNameString();

    switch(controller->GetUSBPID())
    {
        case GAIZHONGGAI_68_PRO_PID:
            type        = DEVICE_TYPE_KEYBOARD;
            description = "https://oshwhub.com/yangzen/zui-gai68-/";
            break;

        case GAIZHONGGAI_42_PRO_PID:
            type        = DEVICE_TYPE_KEYBOARD;
            description = "https://oshwhub.com/myng/42-jian-pan/";
            break;

        case GAIZHONGGAI_17_TOUCH_PRO_PID:
            type        = DEVICE_TYPE_KEYPAD;
            description = "https://oshwhub.com/yangzen/xing-huo-ji-hua-zui-gai-17-4-chu-mo-ji-xie-jian-pan-pro/";
            break;

        case GAIZHONGGAI_17_PRO_PID:
            type        = DEVICE_TYPE_KEYPAD;
            description = "https://oshwhub.com/hivisme/17jian-shuo-zi-xiao-jian-pan/";
            break;

        case GAIZHONGGAI_20_PRO_PID:
            type        = DEVICE_TYPE_KEYPAD;
            description = "https://oshwhub.com/runkuny/19keys_pad_normal/";
            break;

        case GAIZHONGGAI_LIGHT_BOARD_PID:
            type        = DEVICE_TYPE_ACCESSORY;
            description = "https://oshwhub.com/yangzen/xing-huo-2-qi-guang-ban-qu-dong-/";
            break;

        case GAIZHONGGAI_RGB_HUB_GREEN_PID:
            type        = DEVICE_TYPE_LEDSTRIP;
            description = "https://oshwhub.com/yangzen/album/gai-zhong-gai-jian-pan-ge-ji/";
            break;

        case GAIZHONGGAI_RGB_HUB_BLUE_PID:
            type        = DEVICE_TYPE_LEDSTRIP;
            description = "https://oshwhub.com/yangzen/album/gai-zhong-gai-jian-pan-ge-ji/";
            break;

        case GAIZHONGGAI_DIAL_PID:
            type        = DEVICE_TYPE_UNKNOWN;
            description = "https://oshwhub.com/morempty/CH552gyin-liang-xuan-niu/";
            break;
    }

    vendor              = "Yang";
    version             = controller->GetVersion();
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0xFFFF;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_GaiZhongGaiKeyboard::~RGBController_GaiZhongGaiKeyboard()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_GaiZhongGaiKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    char str[10];
    unsigned int total_led_count = 0;
    unsigned int zone_idx_len = 1;
    unsigned int temp;

    leds.clear();
    colors.clear();
    zones.clear();

    for(unsigned int zone_idx = 0; zone_idx < zone_idx_len; zone_idx++)
    {
        zone new_zone;

        switch(controller->GetUSBPID())
        {
            case GAIZHONGGAI_68_PRO_PID:
            {
                new_zone.type                   = ZONE_TYPE_MATRIX;
                new_zone.leds_min               = 68;
                new_zone.leds_max               = 68;
                new_zone.leds_count             = 68;
                new_zone.matrix_map             = new matrix_map_type;
                new_zone.matrix_map->width      = 17;
                new_zone.matrix_map->height     = 5;
                new_zone.matrix_map->map        = (unsigned int *)&matrix_map_68;
                new_zone.name                   = zone_names[zone_idx];
            }
            break;

            case GAIZHONGGAI_42_PRO_PID:
            {
                new_zone.type                   = ZONE_TYPE_MATRIX;
                new_zone.leds_min               = 42;
                new_zone.leds_max               = 42;
                new_zone.leds_count             = 42;
                new_zone.matrix_map             = new matrix_map_type;
                new_zone.matrix_map->width      = 12;
                new_zone.matrix_map->height     = 4;
                new_zone.matrix_map->map        = (unsigned int *)&matrix_map_42;
                new_zone.name                   = zone_names[zone_idx];
            }
            break;

            case GAIZHONGGAI_17_TOUCH_PRO_PID:
            {
                new_zone.type                   = ZONE_TYPE_MATRIX;
                new_zone.leds_min               = 88;
                new_zone.leds_max               = 88;
                new_zone.leds_count             = 88;
                new_zone.matrix_map             = new matrix_map_type;
                new_zone.matrix_map->width      = 5;
                new_zone.matrix_map->height     = 5;
                new_zone.matrix_map->map        = (unsigned int *)&matrix_map_PAD_Touch;
                new_zone.name                   = zone_names[zone_idx];
            }
                break;

            case GAIZHONGGAI_17_PRO_PID:
            {
                new_zone.type                   = ZONE_TYPE_MATRIX;
                new_zone.leds_min               = 85;
                new_zone.leds_max               = 85;
                new_zone.leds_count             = 85;
                new_zone.matrix_map             = new matrix_map_type;
                new_zone.matrix_map->width      = 4;
                new_zone.matrix_map->height     = 5;
                new_zone.matrix_map->map        = (unsigned int *)&matrix_map_17PAD;
                new_zone.name                   = zone_names[zone_idx];
            }
                break;

            case GAIZHONGGAI_20_PRO_PID:
            {
                new_zone.type                   = ZONE_TYPE_MATRIX;
                new_zone.leds_min               = 88;
                new_zone.leds_max               = 88;
                new_zone.leds_count             = 88;
                new_zone.matrix_map             = new matrix_map_type;
                new_zone.matrix_map->width      = 4;
                new_zone.matrix_map->height     = 6;
                new_zone.matrix_map->map        = (unsigned int *)&matrix_map_20PAD;
                new_zone.name                   = zone_names[zone_idx];
            }
                break;

            case GAIZHONGGAI_DIAL_PID:
            {
                new_zone.type                   = ZONE_TYPE_MATRIX;
                new_zone.leds_min               = 89;
                new_zone.leds_max               = 89;
                new_zone.leds_count             = 89;
                new_zone.matrix_map             = new matrix_map_type;
                new_zone.matrix_map->width      = 2;
                new_zone.matrix_map->height     = 2;
                new_zone.matrix_map->map        = (unsigned int *)&matrix_map_dial;
                new_zone.name                   = zone_names[zone_idx];
            }
                break;

            case GAIZHONGGAI_LIGHT_BOARD_PID:
            {
                temp                            = LightBoard_init(controller->GetDataFlash());//get led_len
                new_zone.type                   = ZONE_TYPE_MATRIX;
                new_zone.leds_min               = temp;
                new_zone.leds_max               = temp;
                new_zone.leds_count             = temp;
                new_zone.matrix_map             = new matrix_map_type;
                new_zone.matrix_map->width      = controller->GetDataFlash()[124];
                new_zone.matrix_map->height     = controller->GetDataFlash()[125];
                new_zone.matrix_map->map        = (unsigned int *)&matrix_map_light_board;
                new_zone.name                   = zone_names[zone_idx + 1];
            }
                break;

            case GAIZHONGGAI_RGB_HUB_GREEN_PID:
            {
                zone_idx_len = 4;
                temp                            = controller->GetChannelLen(zone_idx);//get led_len
                new_zone.type                   = ZONE_TYPE_LINEAR;
                new_zone.leds_min               = 1;
                new_zone.leds_max               = 637;
                new_zone.leds_count             = temp;
                new_zone.name                   = zone_names[zone_idx + 1];
            }
                break;

            case GAIZHONGGAI_RGB_HUB_BLUE_PID:
            {
                zone_idx_len = 8;
                temp                            = controller->GetChannelLen(zone_idx);//get led_len
                new_zone.type                   = ZONE_TYPE_LINEAR;
                new_zone.leds_min               = 1;
                new_zone.leds_max               = 633;
                new_zone.leds_count             = temp;
                new_zone.name                   = zone_names[zone_idx + 1];
            }
                break;
        }

        zones.push_back(new_zone);

        total_led_count += new_zone.leds_count;
    }

    switch(controller->GetUSBPID())
    {
        case GAIZHONGGAI_LIGHT_BOARD_PID:
        case GAIZHONGGAI_RGB_HUB_GREEN_PID:
        case GAIZHONGGAI_RGB_HUB_BLUE_PID:
            for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
            {
                    led new_led;
                    snprintf(str, 10, "RGB_%03d", led_idx + 1);
                    new_led.name = str;
                    leds.push_back(new_led);
            }
            break;
        case GAIZHONGGAI_42_PRO_PID:
            for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
            {
                    led new_led;
                    new_led.name = led_names_42key[led_idx];
                    leds.push_back(new_led);
            }
            break;
        default:
            for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
            {
                    led new_led;
                    new_led.name = led_names_general[led_idx];
                    leds.push_back(new_led);
            }
            break;

    }

    SetupColors();
}

void RGBController_GaiZhongGaiKeyboard::ResizeZone(int zone, int new_size)
{
    if((size_t) zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        switch(controller->GetUSBPID())
        {
            case GAIZHONGGAI_RGB_HUB_GREEN_PID:
            case GAIZHONGGAI_RGB_HUB_BLUE_PID:
                controller->SetChannelLen(zone, new_size);
                break;
            default:
                return;
        }

        SetupZones();
    }
}

void RGBController_GaiZhongGaiKeyboard::DeviceUpdateLEDs()
{
    unsigned char colordata[1024 * 3];
    unsigned int data_size = (unsigned int)colors.size();

    for(unsigned int color_idx = 0; color_idx < data_size; color_idx++)
    {
        unsigned int offset = color_idx * 3;

        colordata[offset + 0] = RGBGetGValue(colors[color_idx]);
        colordata[offset + 1] = RGBGetRValue(colors[color_idx]);
        colordata[offset + 2] = RGBGetBValue(colors[color_idx]);
    }

    controller->SendColors(colordata, data_size * 3);
}

void RGBController_GaiZhongGaiKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GaiZhongGaiKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GaiZhongGaiKeyboard::DeviceUpdateMode()
{

}
