/*---------------------------------------------------------------*\
|  RGBController_GaiZhongGaiKeyboard.cpp                          |
|                                                                 |
| https://oshwlab.com/yangdsada/GaiZhongGai-Keyboard-68-4PRO      |
|                                                                 |
| An Yang     2022/6/12                                           |
\*---------------------------------------------------------------*/

#include "RGBController_GaiZhongGaiKeyboard.h"
#include "RGBControllerKeyNames.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

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

static unsigned int matrix_map_68[5][17] =
{
    {  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  NA,  66,  67 },
    {  36,  NA,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51 },
    {  23,  NA,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  NA,  35,  NA,  NA },
    {  10,  NA,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  NA,  21,  NA,  22,  NA },
    {   0,   1,   2,  NA,  NA,  NA,   3,  NA,  NA,  NA,   4,   5,   6,  NA,   7,   8,   9 }
};

static const char* zone_names[] =
{
    "Keyboard"
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes_68[] =
{
    68
};

static const unsigned int zone_sizes_PAD[] =
{
    85
};
static const unsigned int zone_sizes_PAD_Touch[] =
{
    88
};

static const char *led_names[] =
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

/**------------------------------------------------------------------*\
    @name GaiZhongGai Keyboard
    @category  Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectGaiZhongGaiKeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_GaiZhongGaiKeyboard::RGBController_GaiZhongGaiKeyboard(GaiZhongGaiKeyboardController* controller_ptr)
{
    controller = controller_ptr;

    name        = "GaiZhongGai Keyboard Device";
    vendor      = "Yang";
    version     = controller->GetVersion();
    type        = DEVICE_TYPE_KEYBOARD;
    description = "https://oshwlab.com/";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
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
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        unsigned int    zone_size       = 0;
        unsigned int    matrix_width    = 0;
        unsigned int    matrix_height   = 0;
        unsigned int*   matrix_map_ptr  = NULL;

        switch(controller->GetUSBPID())
        {
            case GAIZHONGGAI_68_PRO_PID:
                zone_size               = zone_sizes_68[zone_idx];
                matrix_width            = 17;
                matrix_height           = 5;
                matrix_map_ptr          = (unsigned int *)&matrix_map_68;
                break;

            case GAIZHONGGAI_17_TOUCH_PRO_PID:
                zone_size               = zone_sizes_PAD_Touch[zone_idx];
                matrix_width            = 5;
                matrix_height           = 5;
                matrix_map_ptr          = (unsigned int *)&matrix_map_PAD_Touch;
                break;

            case GAIZHONGGAI_17_PRO_PID:
                zone_size               = zone_sizes_PAD[zone_idx];
                matrix_width            = 4;
                matrix_height           = 5;
                matrix_map_ptr          = (unsigned int *)&matrix_map_17PAD;
                break;

            case GAIZHONGGAI_20_PRO_PID:
                zone_size               = zone_sizes_PAD_Touch[zone_idx];
                matrix_width            = 4;
                matrix_height           = 6;
                matrix_map_ptr          = (unsigned int *)&matrix_map_20PAD;
                break;

        }

        zone new_zone;
        new_zone.name                   = zone_names[zone_idx];
        new_zone.type                   = zone_types[zone_idx];
        new_zone.leds_min               = zone_size;
        new_zone.leds_max               = zone_size;
        new_zone.leds_count             = zone_size;
        new_zone.matrix_map             = new matrix_map_type;
        new_zone.matrix_map->height     = matrix_height;
        new_zone.matrix_map->width      = matrix_width;
        new_zone.matrix_map->map        = matrix_map_ptr;
        zones.push_back(new_zone);

        total_led_count += zone_size;
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_GaiZhongGaiKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}
    
void RGBController_GaiZhongGaiKeyboard::DeviceUpdateLEDs()
{
    unsigned char colordata[100 * 3];
    
    for(std::size_t color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        uint16_t offset = color_idx * 3;

        colordata[offset + 0] = RGBGetGValue(colors[color_idx]);
        colordata[offset + 1] = RGBGetRValue(colors[color_idx]);
        colordata[offset + 2] = RGBGetBValue(colors[color_idx]);
    }

    controller->SendColors(colordata, sizeof(colordata));
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
