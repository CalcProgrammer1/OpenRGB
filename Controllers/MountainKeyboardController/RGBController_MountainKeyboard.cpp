/*-----------------------------------------*\
|  RGBController_MountainKeyboard.cpp       |
|                                           |
|  Generic RGB Interface for Mountain RGB   |
|  keyboard devices                         |
|                                           |
|  Wojciech Lazarski             01/2023    |
\*-----------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_MountainKeyboard.h"

static const unsigned char colorwave_speed_values   [MOUNTAIN_KEYBOARD_SPEED_MAX+1] = { 10,  9,  8,  7,  6};
static const unsigned char tornado_speed_values     [MOUNTAIN_KEYBOARD_SPEED_MAX+1] = { 10,  9,  8,  7,  6};
static const unsigned char breathing_speed_values   [MOUNTAIN_KEYBOARD_SPEED_MAX+1] = {  5,  4,  3,  1,  0};
static const unsigned char matrix_speed_values      [MOUNTAIN_KEYBOARD_SPEED_MAX+1] = { 20, 15, 10,  5,  0};
static const unsigned char reactive_speed_values    [MOUNTAIN_KEYBOARD_SPEED_MAX+1] = {  5,  4,  3,  1,  0};

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

#define KEYBOARD_MATRIX_HEIGHT       6
#define KEYBOARD_MATRIX_WIDTH       24

#define KEYBOARD_MATRIX_EDGE_HEIGHT  6
#define KEYBOARD_MATRIX_EDGE_WIDTH  19

static unsigned int matrix_map[KEYBOARD_MATRIX_HEIGHT][KEYBOARD_MATRIX_WIDTH] =
    { {   0,  NA,   9,  18,  27,  36,  NA,  45,  54,  63,  72,  NA,  81,  90,  99, 108, 117, 114, 123,  NA,  NA,  NA,  NA,  NA },
      {   1,  10,  19,  28,  37,  46,  55,  64,  73,  82,  91,  NA, 100, 109,  87,  NA,  96, 105, 115,  NA,   6,  24,  16,  15 },
      {   2,  NA,  11,  20,  29,  38,  NA,  47,  56,  65,  74,  83,  92, 101, 110, 119,  88,  97, 106,  NA,  61,  69,  70,   7 },
      {   3,  NA,  12,  21,  30,  39,  NA,  48,  57,   6,  75,  84,  93, 102,  NA, 120,  NA,  NA,  NA,  NA,  51,  52,  60,  NA },
      {   4,  NA,  22,  31,  40,  49,  NA,  58,  NA,  67,  76,  85,  94, 103, 121,  NA,  NA, 124,  NA,  NA,  34,  42,  43,  33 },
      {   5,  14,  23,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  68,  77,  86,  95, 104, 113, 122,  NA,  78,  NA,  79,  NA } };


static unsigned int matrix_edge_map[KEYBOARD_MATRIX_EDGE_HEIGHT][KEYBOARD_MATRIX_EDGE_WIDTH] =
    { {  NA,  13,  14,  15,   7,   6,   5,   4,   3,   2,   1,   0,  45,  NA,  NA,  44,  43,  42,  NA },
      {  16,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,   9,  31,  NA,  NA,  NA,  41 },
      {  17,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,   8,  32,  NA,  NA,  NA,  40 },
      {  18,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  10,  33,  NA,  NA,  NA,  39 },
      {  19,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  11,  34,  NA,  NA,  NA,  38 },
      {  NA,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  12,  NA,  NA,  35,  36,  37,  NA } };


/*---------------------------------------------------*\
| TODO: Detect detached keypad                        |
| TODO: Figure out how to read RGB state              |
| TODO: separatee display brughness from backlight    |
\*---------------------------------------------------*/

#if 0
static unsigned int matrix_map_tkl[6][19] =
{ {   0,  NA,   9,  18,  27,  36,  NA,  45,  54,  63,  72,  NA,  81,  90,  99, 108, 117, 114, 123},
  {   1,  10,  19,  28,  37,  46,  55,  64,  73,  82,  91,  NA, 100, 109,  87,  NA,  96, 105, 115},
  {   2,  NA,  11,  20,  29,  38,  NA,  47,  56,  65,  74,  83,  92, 101, 110, 119,  88,  97, 106},
  {   3,  NA,  12,  21,  30,  39,  NA,  48,  57,   6,  75,  84,  93, 102,  NA, 120,  NA,  NA,  NA},
  {   4,  NA,  22,  31,  40,  49,  NA,  58,  NA,  67,  76,  85,  94, 103, 121,  NA,  NA, 124,  NA},
  {   5,  14,  23,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  68,  77,  86,  95, 104, 113, 122} };
#endif

typedef struct
{
    const char*     name;
    zone_type       type;
    unsigned int*   ptr;
    unsigned int    size;
    unsigned int    height;
    unsigned int    width;
} mountain_zone_t;

static const mountain_zone_t zone_definitions[] =
{
    {
        ZONE_EN_KEYBOARD,
        ZONE_TYPE_MATRIX,
        (unsigned int*)&matrix_map,
        125,
        KEYBOARD_MATRIX_HEIGHT,
        KEYBOARD_MATRIX_WIDTH
    },
    {
        "Keyboard Edge",
        ZONE_TYPE_MATRIX,
        (unsigned int*)&matrix_edge_map,
        46,
        KEYBOARD_MATRIX_EDGE_HEIGHT,
        KEYBOARD_MATRIX_EDGE_WIDTH
    }
};

/*-------------------------------*\
| TODO: Detect detached keypad    |
\*-------------------------------*/

static const char *led_names[] =
{
    KEY_EN_ESCAPE,
    KEY_EN_BACK_TICK,
    KEY_EN_TAB,
    KEY_EN_CAPS_LOCK,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_UNUSED,/*8*/
    KEY_EN_F1,
    KEY_EN_1, /*10*/
    KEY_EN_Q,
    KEY_EN_A,
    KEY_EN_UNUSED,/*13*/
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_UNUSED,/*17*/
    KEY_EN_F2,
    KEY_EN_2,
    KEY_EN_W,
    KEY_EN_S,
    KEY_EN_Z,
    KEY_EN_LEFT_ALT,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_UNUSED,/*25*/
    KEY_EN_UNUSED,/*26*/
    KEY_EN_F3,
    KEY_EN_3,
    KEY_EN_E,
    KEY_EN_D,
    KEY_EN_X,
    KEY_EN_UNUSED,/*32*/
    KEY_EN_NUMPAD_ENTER,
    KEY_EN_NUMPAD_1,
    KEY_EN_UNUSED,/*35*/
    KEY_EN_F4,
    KEY_EN_4,
    KEY_EN_R,
    KEY_EN_F,
    KEY_EN_C,
    KEY_EN_SPACE,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_3,
    KEY_EN_UNUSED, /*44*/
    KEY_EN_F5,
    KEY_EN_5,
    KEY_EN_T,
    KEY_EN_G,
    KEY_EN_V,
    KEY_EN_UNUSED, /*50*/
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_5,
    KEY_EN_UNUSED, /*53*/
    KEY_EN_F6,
    KEY_EN_6,
    KEY_EN_Y,
    KEY_EN_H,
    KEY_EN_B,
    KEY_EN_UNUSED, /*59*/
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_7,
    KEY_EN_UNUSED, /*62*/
    KEY_EN_F7,
    KEY_EN_7,
    KEY_EN_U,
    KEY_EN_J,
    KEY_EN_N,
    KEY_EN_RIGHT_ALT,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_9,
    KEY_EN_UNUSED, /*71*/
    KEY_EN_F8,
    KEY_EN_8,
    KEY_EN_I,
    KEY_EN_K,
    KEY_EN_M,
    KEY_EN_RIGHT_WINDOWS,
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_PERIOD,
    KEY_EN_UNUSED, /*80*/
    KEY_EN_F9,
    KEY_EN_9,
    KEY_EN_O,
    KEY_EN_L,
    KEY_EN_COMMA,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_BACKSPACE,
    KEY_EN_DELETE,
    KEY_EN_UNUSED,/*89*/
    KEY_EN_F10,
    KEY_EN_0,
    KEY_EN_P,
    KEY_EN_SEMICOLON,
    KEY_EN_PERIOD,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_INSERT,
    KEY_EN_END,
    KEY_EN_UNUSED,/*98*/
    KEY_EN_F11,
    KEY_EN_MINUS,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_QUOTE,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_LEFT_ARROW,
    KEY_EN_HOME,
    KEY_EN_PAGE_DOWN,
    KEY_EN_UNUSED,/*107*/
    KEY_EN_F12,
    KEY_EN_EQUALS,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_UNUSED,/*111*/
    KEY_EN_UNUSED,/*112*/
    KEY_EN_DOWN_ARROW,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_PAGE_UP,
    KEY_EN_UNUSED,/*116*/
    KEY_EN_PRINT_SCREEN,
    KEY_EN_UNUSED,/*118*/
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_ANSI_ENTER,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_PAUSE_BREAK,
    KEY_EN_UP_ARROW,
};

/**------------------------------------------------------------------*\
    @name Mountain Keyboard
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectMountainKeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_MountainKeyboard::RGBController_MountainKeyboard(MountainKeyboardController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Mountain Everest Keyboard";
    vendor      = "Mountain";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Mountain Everest Keyboard";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.value                        = MOUNTAIN_KEYBOARD_MODE_DIRECT;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.brightness_min               = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    Direct.brightness                   = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Direct.brightness_max               = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name                            = "Off";
    Off.value                           = MOUNTAIN_KEYBOARD_MODE_OFF;
    Off.flags                           = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                         = "Static";
    Static.value                        = MOUNTAIN_KEYBOARD_MODE_STATIC;
    Static.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.brightness_min               = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    Static.brightness                   = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Static.brightness_max               = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Static.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min                   = 1;
    Static.colors_max                   = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode ColorWaveRainbow;
    ColorWaveRainbow.name               = "Rainbow Wave";
    ColorWaveRainbow.value              = MOUNTAIN_KEYBOARD_MODE_COLOR_WAVE;
    ColorWaveRainbow.flags              = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_MANUAL_SAVE;
    ColorWaveRainbow.brightness_min     = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    ColorWaveRainbow.brightness         = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    ColorWaveRainbow.brightness_max     = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    ColorWaveRainbow.speed_min          = MOUNTAIN_KEYBOARD_SPEED_MIN;
    ColorWaveRainbow.speed              = MOUNTAIN_KEYBOARD_SPEED_DEFAULT;
    ColorWaveRainbow.speed_max          = MOUNTAIN_KEYBOARD_SPEED_MAX;
    ColorWaveRainbow.color_mode         = MODE_COLORS_RANDOM;
    modes.push_back(ColorWaveRainbow);

    mode ColorWave;
    ColorWave.name                      = "ColorWave";
    ColorWave.value                     = MOUNTAIN_KEYBOARD_MODE_COLOR_WAVE;
    ColorWave.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_MANUAL_SAVE;
    ColorWave.brightness_min            = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    ColorWave.brightness                = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    ColorWave.brightness_max            = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    ColorWave.speed_min                 = MOUNTAIN_KEYBOARD_SPEED_MIN;
    ColorWave.speed                     = MOUNTAIN_KEYBOARD_SPEED_DEFAULT;
    ColorWave.speed_max                 = MOUNTAIN_KEYBOARD_SPEED_MAX;
    ColorWave.colors_min                = 1;
    ColorWave.colors_max                = 2;
    ColorWave.colors.resize(2);
    ColorWave.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(ColorWave);

    mode Tornado;
    Tornado.name                        = "Tornado";
    Tornado.value                       = MOUNTAIN_KEYBOARD_MODE_TORNADO;
    Tornado.flags                       = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_MANUAL_SAVE;
    Tornado.brightness_min              = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    Tornado.brightness                  = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Tornado.brightness_max              = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Tornado.speed_min                   = MOUNTAIN_KEYBOARD_SPEED_MIN;
    Tornado.speed                       = MOUNTAIN_KEYBOARD_SPEED_DEFAULT;
    Tornado.speed_max                   = MOUNTAIN_KEYBOARD_SPEED_MAX;
    Tornado.colors_min                  = 1;
    Tornado.colors_max                  = 1;
    Tornado.colors.resize(1);
    Tornado.color_mode                  = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Tornado);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = MOUNTAIN_KEYBOARD_MODE_BREATHING;
    Breathing.flags                     = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Breathing.brightness_min            = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    Breathing.brightness                = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Breathing.brightness_max            = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Breathing.speed_min                 = MOUNTAIN_KEYBOARD_SPEED_MIN;
    Breathing.speed                     = MOUNTAIN_KEYBOARD_SPEED_DEFAULT;
    Breathing.speed_max                 = MOUNTAIN_KEYBOARD_SPEED_MAX;
    Breathing.colors_min                = 1;
    Breathing.colors_max                = 2;
    Breathing.colors.resize(2);
    Breathing.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Breathing);

    mode Reactive;
    Reactive.name                       = "Reactive";
    Reactive.value                      = MOUNTAIN_KEYBOARD_MODE_REACTIVE;
    Reactive.flags                      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Reactive.brightness_min             = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    Reactive.brightness                 = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Reactive.brightness_max             = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Reactive.speed_min                  = MOUNTAIN_KEYBOARD_SPEED_MIN;
    Reactive.speed                      = MOUNTAIN_KEYBOARD_SPEED_DEFAULT;
    Reactive.speed_max                  = MOUNTAIN_KEYBOARD_SPEED_MAX;
    Reactive.color_mode                 = MODE_COLORS_MODE_SPECIFIC;
    Reactive.colors_min                 = 1;
    Reactive.colors_max                 = 2;
    Reactive.colors.resize(2);
    modes.push_back(Reactive);

    mode Matrix;
    Matrix.name                         = "Matrix";
    Matrix.value                        = MOUNTAIN_KEYBOARD_MODE_MATRIX;
    Matrix.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Matrix.brightness_min               = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    Matrix.brightness                   = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Matrix.brightness_max               = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Matrix.speed_min                    = MOUNTAIN_KEYBOARD_SPEED_MIN;
    Matrix.speed                        = MOUNTAIN_KEYBOARD_SPEED_DEFAULT;
    Matrix.speed_max                    = MOUNTAIN_KEYBOARD_SPEED_MAX;
    Matrix.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Matrix.colors_min                   = 1;
    Matrix.colors_max                   = 2;
    Matrix.colors.resize(2);
    modes.push_back(Matrix);

    SetupZones();

}

RGBController_MountainKeyboard::~RGBController_MountainKeyboard()
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

void RGBController_MountainKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < sizeof(zone_definitions)/sizeof(zone_definitions[0]); zone_idx++)
    {
        zone new_zone;
        new_zone.name                   = zone_definitions[zone_idx].name;
        new_zone.type                   = zone_definitions[zone_idx].type;
        new_zone.leds_min               = zone_definitions[zone_idx].size;
        new_zone.leds_max               = zone_definitions[zone_idx].size;
        new_zone.leds_count             = zone_definitions[zone_idx].size;
        new_zone.matrix_map             = new matrix_map_type;
        new_zone.matrix_map->height     = zone_definitions[zone_idx].height;
        new_zone.matrix_map->width      = zone_definitions[zone_idx].width;
        new_zone.matrix_map->map        = zone_definitions[zone_idx].ptr;
        zones.push_back(new_zone);
    }

    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {

        if(zone_idx == 0)
        {
            for(unsigned int led_idx=0;led_idx<zones[zone_idx].leds_count;led_idx++)
            {
                led* new_led = new led();
                new_led->name = led_names[led_idx];
                leds.push_back(*new_led);
            }
        }
        else
        {
            for(unsigned int led_idx=0;led_idx<zones[zone_idx].leds_count;led_idx++)
            {
                led* new_led = new led();
                new_led->name = zones[zone_idx].name + " LED:";
                new_led->name.append(std::to_string(led_idx + 1));
                leds.push_back(*new_led);
            }
        }
    }

    SetupColors();

    DeviceUpdateMode();
}

void RGBController_MountainKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_MountainKeyboard::DeviceUpdate(const mode& current_mode)
{
    switch(current_mode.value)
    {
        case MOUNTAIN_KEYBOARD_MODE_DIRECT:
        {
            unsigned char *color_data;

            color_data = (unsigned char*)malloc(colors.size() * 3);
            if(color_data)
            {
                for(int led_idx = 0; led_idx < colors.size(); led_idx++)
                {
                    color_data[(3 * led_idx)]   = RGBGetRValue(colors[led_idx]);
                    color_data[(3 * led_idx)+1] = RGBGetGValue(colors[led_idx]);
                    color_data[(3 * led_idx)+2] = RGBGetBValue(colors[led_idx]);
                }

                unsigned char *color_ptr = color_data;
                for(unsigned int zone_idx=0;zone_idx<zones.size();zone_idx++)
                {
                    switch(zone_idx)
                    {
                        case 0:
                        {
                            controller->SendDirectColorCmd(current_mode.brightness, color_ptr,zones[zone_idx].leds_count * 3);
                            color_ptr += zones[zone_idx].leds_count * 3;
                        }
                        break;
                        case 1:
                        {
                            controller->SendDirectColorEdgeCmd(current_mode.brightness, color_ptr,zones[zone_idx].leds_count * 3);
                            color_ptr += zones[zone_idx].leds_count * 3;
                        }
                        break;

                        default:
                        break;
                    }
                }

                free(color_data);
            }
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_OFF:
        {
            controller->SendOffCmd();
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_STATIC:
        {
            color_setup setup;

            setup.brightness        = current_mode.brightness;
            setup.mode.one_color.r  = RGBGetRValue(current_mode.colors[0]);
            setup.mode.one_color.g  = RGBGetGValue(current_mode.colors[0]);
            setup.mode.one_color.b  = RGBGetBValue(current_mode.colors[0]);

            controller->SendColorStaticCmd(setup);
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_COLOR_WAVE:
        {
            color_setup setup;

            setup.speed         = colorwave_speed_values[current_mode.speed];
            setup.direction     = ConvertDirection(current_mode.direction,false);
            setup.brightness    = current_mode.brightness;

            if(current_mode.color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                if(current_mode.colors.size() == 2)
                {
                    setup.color_mode            = MOUNTAIN_KEYBOARD_COLOR_MODE_DUAL;
                    setup.mode.two_colors.r1    = RGBGetRValue(current_mode.colors[0]);
                    setup.mode.two_colors.g1    = RGBGetGValue(current_mode.colors[0]);
                    setup.mode.two_colors.b1    = RGBGetBValue(current_mode.colors[0]);
                    setup.mode.two_colors.r2    = RGBGetRValue(current_mode.colors[1]);
                    setup.mode.two_colors.g2    = RGBGetGValue(current_mode.colors[1]);
                    setup.mode.two_colors.b2    = RGBGetBValue(current_mode.colors[1]);
                }
                else
                {
                    if(current_mode.colors.size() == 1)
                    {
                        setup.color_mode        = MOUNTAIN_KEYBOARD_COLOR_MODE_SINGLE;
                        setup.mode.one_color.r  = RGBGetRValue(current_mode.colors[0]);
                        setup.mode.one_color.g  = RGBGetGValue(current_mode.colors[0]);
                        setup.mode.one_color.b  = RGBGetBValue(current_mode.colors[0]);
                    }
                }
            }
            else
            {
                setup.color_mode    = MOUNTAIN_KEYBOARD_COLOR_MODE_RAINBOW;
            }

            controller->SendColorWaveCmd(setup);
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_TORNADO:
        {
            color_setup setup;

            setup.speed      = tornado_speed_values[current_mode.speed];
            setup.direction  = ConvertDirection(current_mode.direction,true);
            setup.brightness = current_mode.brightness;

            if(current_mode.color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                setup.color_mode        = MOUNTAIN_KEYBOARD_COLOR_MODE_SINGLE;
                setup.mode.one_color.r  = RGBGetRValue(current_mode.colors[0]);
                setup.mode.one_color.g  = RGBGetGValue(current_mode.colors[0]);
                setup.mode.one_color.b  = RGBGetBValue(current_mode.colors[0]);
            }
            else
            {
                setup.color_mode    = MOUNTAIN_KEYBOARD_COLOR_MODE_RAINBOW;
            }

            controller->SendColorTornadoCmd(setup);
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_BREATHING:
        {
            color_setup setup;

            setup.speed         = breathing_speed_values[current_mode.speed];
            setup.brightness    = current_mode.brightness;

            if(current_mode.color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                if(current_mode.colors.size() == 2)
                {
                    setup.color_mode            = MOUNTAIN_KEYBOARD_COLOR_MODE_DUAL;
                    setup.mode.two_colors.r1    = RGBGetRValue(current_mode.colors[0]);
                    setup.mode.two_colors.g1    = RGBGetGValue(current_mode.colors[0]);
                    setup.mode.two_colors.b1    = RGBGetBValue(current_mode.colors[0]);
                    setup.mode.two_colors.r2    = RGBGetRValue(current_mode.colors[1]);
                    setup.mode.two_colors.g2    = RGBGetGValue(current_mode.colors[1]);
                    setup.mode.two_colors.b2    = RGBGetBValue(current_mode.colors[1]);
                }
                else
                {
                    if(current_mode.colors.size() == 1)
                    {
                        setup.color_mode        = MOUNTAIN_KEYBOARD_COLOR_MODE_SINGLE;
                        setup.mode.one_color.r  = RGBGetRValue(current_mode.colors[0]);
                        setup.mode.one_color.g  = RGBGetGValue(current_mode.colors[0]);
                        setup.mode.one_color.b  = RGBGetBValue(current_mode.colors[0]);
                    }
                }
            }
            else
            {
                setup.color_mode        = MOUNTAIN_KEYBOARD_COLOR_MODE_RAINBOW;
            }

            controller->SendColorBreathingCmd(setup);
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_MATRIX:
        {
            color_setup setup;

            setup.speed       = matrix_speed_values[current_mode.speed];
            setup.brightness  = current_mode.brightness;
            setup.mode.two_colors.r1    = RGBGetRValue(current_mode.colors[0]);
            setup.mode.two_colors.g1    = RGBGetGValue(current_mode.colors[0]);
            setup.mode.two_colors.b1    = RGBGetBValue(current_mode.colors[0]);
            setup.mode.two_colors.r2    = RGBGetRValue(current_mode.colors[1]);
            setup.mode.two_colors.g2    = RGBGetGValue(current_mode.colors[1]);
            setup.mode.two_colors.b2    = RGBGetBValue(current_mode.colors[1]);

            controller->SendColorMatrixCmd(setup);
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_REACTIVE:
        {
            color_setup setup;

            setup.speed         = reactive_speed_values[current_mode.speed];
            setup.brightness    = current_mode.brightness;
            setup.mode.two_colors.r1    = RGBGetRValue(current_mode.colors[0]);
            setup.mode.two_colors.g1    = RGBGetGValue(current_mode.colors[0]);
            setup.mode.two_colors.b1    = RGBGetBValue(current_mode.colors[0]);
            setup.mode.two_colors.r2    = RGBGetRValue(current_mode.colors[1]);
            setup.mode.two_colors.g2    = RGBGetGValue(current_mode.colors[1]);
            setup.mode.two_colors.b2    = RGBGetBValue(current_mode.colors[1]);

            controller->SendColorReactiveCmd(setup);
        }
        break;

        default:
        break;
    }
}


void RGBController_MountainKeyboard::DeviceUpdateLEDs()
{
    mode current_mode = modes[active_mode];
    DeviceUpdate(current_mode);
}



void RGBController_MountainKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MountainKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MountainKeyboard::DeviceUpdateMode()
{
    static int prv_mode =-1;

    mode current_mode = modes[active_mode];
    if(prv_mode != current_mode.value)
    {
        controller->SelectMode(current_mode.value);
        prv_mode = current_mode.value;
    }

    DeviceUpdate(current_mode);
}

unsigned char RGBController_MountainKeyboard::ConvertDirection(unsigned int direction, bool rotation)
{
    unsigned char ret;
    switch(direction)
    {
        case MODE_DIRECTION_LEFT:
        {
            ret = rotation?MOUNTAIN_KEYBOARD_DIRECTION_ANTICLK:MOUNTAIN_KEYBOARD_DIRECTION_LEFT;
        }
        break;

        case MODE_DIRECTION_RIGHT:
        {
            ret = rotation?MOUNTAIN_KEYBOARD_DIRECTION_CLK:MOUNTAIN_KEYBOARD_DIRECTION_RIGHT;
        }
        break;

        case MODE_DIRECTION_UP:
        {
            ret = MOUNTAIN_KEYBOARD_DIRECTION_UP;
        }
        break;

        case MODE_DIRECTION_DOWN:
        {
            ret = MOUNTAIN_KEYBOARD_DIRECTION_DOWN;
        }
        break;

        default:
        {
            ret = MOUNTAIN_KEYBOARD_DIRECTION_LEFT;
        }
        break;
    }
    return ret;
}

void RGBController_MountainKeyboard::DeviceSaveMode()
{
    controller->SaveData(modes[active_mode].value);
}
