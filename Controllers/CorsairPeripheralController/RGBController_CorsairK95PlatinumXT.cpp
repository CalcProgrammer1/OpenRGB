/*-----------------------------------------*\
|  RGBController_CorsairK95PlatinumXT.h     |
|                                           |
|                                           |
|  Guimard Morgan (morg) 6/07/2022          |
\*-----------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_CorsairK95PlatinumXT.h"
#include "RGBControllerKeyNames.h"

#define NA  0xFFFFFFFF

#define K95_PLATINUM_BAR_LENGTH 19
#define K95_PLATINUM_XT_HEIGHT  7
#define K95_PLATINUM_XT_WIDTH 23

static unsigned int k95_platinum_xt_matrix_map[K95_PLATINUM_XT_HEIGHT][K95_PLATINUM_XT_WIDTH] =
{
    { NA, NA,  NA,  NA,  22,  28,  34,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,   96,   NA,   NA,    NA},
    {  0,  6,  NA,  16,  23,  29,  35,  NA,  45,  50,  55,  60,  66,  72,  76,  78,  83,  87,  92,   97,  103,  108,   114},
    {  1,  7,  12,  17,  24,  30,  36,  40,  46,  51,  56,  61,  67,  73,  NA,  79,  84,  88,  93,   98,  104,  109,   115},
    {  2,  8,  13,  18,  25,  31,  37,  41,  47,  52,  57,  62,  68,  74,  NA,  80,  85,  89,  94,   99,  105,  110,   116},
    {  3,  9,  14,  19,  26,  32,  38,  42,  48,  53,  58,  63,  69,  NA,  NA,  81,  NA,  NA,  NA,  100,  106,  111,    NA},
    {  4, 10,  NA,  20,  27,  33,  39,  43,  49,  54,  59,  64,  70,  NA,  77,  NA,  NA,  90,  NA,  101,  107,  112,   117},
    {  5, 11,  15,  21,  NA,  NA,  NA,  44,  NA,  NA,  NA,  65,  71,  75,  NA,  82,  86,  91,  95,  102,   NA,  113,    NA}
};


static std::vector<std::tuple<std::string, unsigned int>> k95_platinum_xt_leds_names_and_positions =
{
    // Top Bar
    {"LED: 1",                  133},
    {"LED: 2",                  134},
    {"LED: 3",                  135},
    {"LED: 4",                  136},
    {"LED: 5",                  137},
    {"LED: 6",                  138},
    {"LED: 7",                  139},
    {"LED: 8",                  140},
    {"LED: 9",                  141},
    {"LED: 10",                 142},
    {"LED: 11",                 143},
    {"LED: 12",                 144},
    {"LED: 13",                 145},
    {"LED: 14",                 146},
    {"LED: 15",                 147},
    {"LED: 16",                 148},
    {"LED: 17",                 149},
    {"LED: 18",                 150},
    {"LED: 19",                 151},

    // COL 1
    {"Key: G1",                 127},
    {"Key: G2",                 128},
    {"Key: G3",                 129},
    {"Key: G4",                 130},
    {"Key: G5",                 131},
    {"Key: G6",                 132},

    // COL 2
    {KEY_EN_ESCAPE,             37},
    {KEY_EN_BACK_TICK,          49},
    {KEY_EN_TAB,                39},
    {KEY_EN_CAPS_LOCK,          53},
    {KEY_EN_LEFT_SHIFT,         102},
    {KEY_EN_LEFT_CONTROL,       101},

    // COL 3
    {KEY_EN_1,                  26},
    {KEY_EN_Q,                  16},
    {KEY_EN_A,                  0},
    {KEY_EN_LEFT_WINDOWS,       104},

    // COL 4
    {KEY_EN_F1,                 54},
    {KEY_EN_2,                  27},
    {KEY_EN_W,                  22},
    {KEY_EN_S,                  18},
    {KEY_EN_Z,                  25},
    {KEY_EN_LEFT_ALT,           103},

    // COL 5
    {"Key: User",               124},
    {KEY_EN_F2,                 55},
    {KEY_EN_3,                  28},
    {KEY_EN_E,                  4},
    {KEY_EN_D,                  3},
    {KEY_EN_X,                  23},

    // COL 6
    {"Key: Brightness",         109},
    {KEY_EN_F3,                 56},
    {KEY_EN_4,                  29},
    {KEY_EN_R,                  17},
    {KEY_EN_F,                  5},
    {KEY_EN_C,                  2},

    // COL 7
    {"Key: Lock",               110},
    {KEY_EN_F4,                 57},
    {KEY_EN_5,                  30},
    {KEY_EN_T,                  19},
    {KEY_EN_G,                  6},
    {KEY_EN_V,                  21},

    // COL 8
    {KEY_EN_6,                  31},
    {KEY_EN_Y,                  24},
    {KEY_EN_H,                  7},
    {KEY_EN_B,                  1},
    {KEY_EN_SPACE,              40},

    // COL 9
    {KEY_EN_F5,                 58},
    {KEY_EN_7,                  32},
    {KEY_EN_U,                  20},
    {KEY_EN_J,                  9},
    {KEY_EN_N,                  13},

    // COL 10
    {KEY_EN_F6,                 59},
    {KEY_EN_8,                  33},
    {KEY_EN_I,                  8},
    {KEY_EN_K,                  10},
    {KEY_EN_M,                  12},

    // COL 11
    {KEY_EN_F7,                 60},
    {KEY_EN_9,                  34},
    {KEY_EN_O,                  14},
    {KEY_EN_L,                  11},
    {KEY_EN_COMMA,              50},

    // COL 12
    {KEY_EN_F8,                 61},
    {KEY_EN_0,                  35},
    {KEY_EN_P,                  15},
    {KEY_EN_SEMICOLON,          47},
    {KEY_EN_PERIOD,             51},
    {KEY_EN_RIGHT_ALT,          107},

    // COL 13
    {KEY_EN_F9,                 62},
    {KEY_EN_MINUS,              41},
    {KEY_EN_LEFT_BRACKET,       43},
    {KEY_EN_QUOTE,              48},
    {KEY_EN_FORWARD_SLASH,      52},
    {KEY_EN_RIGHT_WINDOWS,      108},

    // COL 14
    {KEY_EN_F10,                63},
    {KEY_EN_EQUALS,             42},
    {KEY_EN_RIGHT_BRACKET,      44},
    {KEY_EN_MENU,               97},

    // COL 15
    {KEY_EN_F11,                64},
    {KEY_EN_RIGHT_SHIFT,        106},

    // COL 16
    {KEY_EN_F12,                65},
    {KEY_EN_BACKSPACE,          38},
    {KEY_EN_ANSI_BACK_SLASH,    45},
    {KEY_EN_ANSI_ENTER,         36},
    {KEY_EN_RIGHT_CONTROL,      105},


    // COL 17
    {KEY_EN_PRINT_SCREEN,       66},
    {KEY_EN_INSERT,             69},
    {KEY_EN_DELETE,             72},
    {KEY_EN_LEFT_ARROW,         76},

    // COL 18
    {KEY_EN_SCROLL_LOCK,        67},
    {KEY_EN_HOME,               70},
    {KEY_EN_END,                73},
    {KEY_EN_UP_ARROW,           78},
    {KEY_EN_DOWN_ARROW,         77},

    // COL 19
    {KEY_EN_PAUSE_BREAK,        68},
    {KEY_EN_PAGE_UP,            71},
    {KEY_EN_PAGE_DOWN,          74},
    {KEY_EN_RIGHT_ARROW,        75},


    // COL 20
    {KEY_EN_MEDIA_MUTE,         98},
    {KEY_EN_MEDIA_STOP,         119},
    {KEY_EN_NUMPAD_LOCK,        79},
    {KEY_EN_NUMPAD_7,           91},
    {KEY_EN_NUMPAD_4,           88},
    {KEY_EN_NUMPAD_1,           85},
    {KEY_EN_NUMPAD_0,           94},

    // COL 21
    {KEY_EN_MEDIA_PREVIOUS,    122},
    {KEY_EN_NUMPAD_DIVIDE,      80},
    {KEY_EN_NUMPAD_8,           92},
    {KEY_EN_NUMPAD_5,           89},
    {KEY_EN_NUMPAD_2,           86},

    // COL 22
    {KEY_EN_MEDIA_PLAY_PAUSE,   120},
    {KEY_EN_NUMPAD_TIMES,       81},
    {KEY_EN_NUMPAD_9,           93},
    {KEY_EN_NUMPAD_6,           90},
    {KEY_EN_NUMPAD_3,           87},
    {KEY_EN_NUMPAD_PERIOD,      95},

    // COL 23
    {KEY_EN_MEDIA_NEXT,         121},
    {KEY_EN_NUMPAD_MINUS,       82},
    {KEY_EN_NUMPAD_PLUS,        83},
    {KEY_EN_NUMPAD_ENTER,       84}
};

/**------------------------------------------------------------------*\
    @name Corsair K95 Platinum XT Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairK95PlatinumXTControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairK95PlatinumXT::RGBController_CorsairK95PlatinumXT(CorsairK95PlatinumXTController* controller_ptr)
{
    controller      = controller_ptr;

    name            = controller->GetName();
    vendor          = "Corsair";
    description     = "Corsair K95 Platinum XT Keyboard Device";
    type            = DEVICE_TYPE_KEYBOARD;
    version         = controller->GetFirmwareString();
    location        = controller->GetDeviceLocation();
    serial          = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_CorsairK95PlatinumXT::~RGBController_CorsairK95PlatinumXT()
{    
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].type == ZONE_TYPE_MATRIX)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_CorsairK95PlatinumXT::SetupZones()
{
    /*---------------------------------------------------------*\
    | Top bar zone setup                                        |
    \*---------------------------------------------------------*/
    zone top_bar_zone;

    top_bar_zone.name                   = "Top Bar";
    top_bar_zone.type                   = ZONE_TYPE_LINEAR;
    top_bar_zone.leds_min               = K95_PLATINUM_BAR_LENGTH;
    top_bar_zone.leds_max               = K95_PLATINUM_BAR_LENGTH;
    top_bar_zone.leds_count             = K95_PLATINUM_BAR_LENGTH;
    top_bar_zone.matrix_map             = nullptr;

    for(unsigned int i = 0; i < top_bar_zone.leds_count; i++)
    {
        led new_led;
        new_led.name = std::get<0>(k95_platinum_xt_leds_names_and_positions[i]);
        leds.push_back(new_led);
    }

    zones.push_back(top_bar_zone);

    /*---------------------------------------------------------*\
    | Keyboard zone setup                                       |
    \*---------------------------------------------------------*/
    zone keyboard_zone;

    keyboard_zone.name                  = ZONE_EN_KEYBOARD;
    keyboard_zone.type                  = ZONE_TYPE_MATRIX;
    keyboard_zone.matrix_map            = new matrix_map_type;
    keyboard_zone.matrix_map->height    = K95_PLATINUM_XT_HEIGHT;
    keyboard_zone.matrix_map->width     = K95_PLATINUM_XT_WIDTH;
    keyboard_zone.matrix_map->map       = new unsigned int[K95_PLATINUM_XT_HEIGHT * K95_PLATINUM_XT_WIDTH];

    unsigned int keyboard_leds_count = 0;

    for(unsigned int w = 0; w < K95_PLATINUM_XT_WIDTH; w++)
    {
        for(unsigned int h = 0; h < K95_PLATINUM_XT_HEIGHT; h++)
        {
            unsigned int key = k95_platinum_xt_matrix_map[h][w];
            keyboard_zone.matrix_map->map[h * K95_PLATINUM_XT_WIDTH + w] = key;

            if(key != NA)
            {
                led new_led;
                new_led.name = std::get<0>(k95_platinum_xt_leds_names_and_positions[top_bar_zone.leds_count + keyboard_leds_count]);
                leds.push_back(new_led);
                keyboard_leds_count++;
            }
        }
    }

    keyboard_zone.leds_min   = keyboard_leds_count;
    keyboard_zone.leds_max   = keyboard_leds_count;
    keyboard_zone.leds_count = keyboard_leds_count;

    zones.push_back(keyboard_zone);

    SetupColors();
}

void RGBController_CorsairK95PlatinumXT::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairK95PlatinumXT::DeviceUpdateLEDs()
{
    controller->SendDirect(colors, k95_platinum_xt_leds_names_and_positions);
}

void RGBController_CorsairK95PlatinumXT::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairK95PlatinumXT::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairK95PlatinumXT::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
