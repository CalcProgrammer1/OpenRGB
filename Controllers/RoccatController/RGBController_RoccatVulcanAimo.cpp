/*-----------------------------------------*\
|  RGBController_RoccatVulcanAimo.cpp       |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|                                           |
|                                           |
|  Mola19 17/12/2021                        |
\*-----------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_RoccatVulcanAimo.h"
#include <vector>

#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][22] =
    { {   0,  NA,  11,  17,  23,  28,  NA,  48,  53,  59,  65,  78,  84,  85,  86,  99, 103, 108,  NA,  NA,  NA,  NA },
      {   1,   6,  12,  18,  24,  29,  33,  49,  54,  60,  66,  72,  79,  87,  NA, 100, 104, 109, 113, 119, 124, 129 },
      {   2,  NA,   7,  13,  19,  25,  30,  34,  50,  55,  61,  67,  73,  80,  NA, 101, 105, 110, 114, 120, 125, 130 },
      {   3,  NA,   8,  14,  20,  26,  31,  35,  51,  56,  62,  68,  74,  96,  88,  NA,  NA,  NA, 115, 121, 126,  NA },
      {   4,  NA,   9,  15,  21,  27,  32,  36,  52,  57,  63,  69,  75,  82,  NA,  NA, 106,  NA, 116, 122, 127, 131 },
      {   5,  10,  16,  NA,  NA,  NA,  37,  NA,  NA,  NA,  NA,  70,  76,  83,  89, 102, 107, 111, 117,  NA, 128,  NA } };

static const unsigned int zone_size = 132;

static const char *led_names[] =
{
    KEY_EN_ESCAPE,
    "Key: ^",
    KEY_EN_TAB,
    KEY_EN_CAPS_LOCK,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_1,
    KEY_EN_Q,
    KEY_EN_A,
    "Key: <",
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_F1,
    KEY_EN_2,
    KEY_EN_W,
    KEY_EN_S,
    KEY_EN_Y,
    KEY_EN_LEFT_ALT,
    KEY_EN_F2,
    KEY_EN_3,
    KEY_EN_E,
    KEY_EN_D,
    KEY_EN_X,
    KEY_EN_UNUSED,
    KEY_EN_F3,
    KEY_EN_4,
    KEY_EN_R,
    KEY_EN_F,
    KEY_EN_C,
    KEY_EN_F4,
    KEY_EN_5,
    KEY_EN_T,
    KEY_EN_G,
    KEY_EN_V,
    KEY_EN_6,
    KEY_EN_Z,
    KEY_EN_H,
    KEY_EN_B,
    KEY_EN_SPACE,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_F5,
    KEY_EN_7,
    KEY_EN_U,
    KEY_EN_J,
    KEY_EN_N,
    KEY_EN_F6,
    KEY_EN_8,
    KEY_EN_I,
    KEY_EN_K,
    KEY_EN_M,
    KEY_EN_UNUSED,
    KEY_EN_F7,
    KEY_EN_9,
    KEY_EN_O,
    KEY_EN_L,
    KEY_EN_COMMA,
    KEY_EN_UNUSED,
    KEY_EN_F8,
    KEY_EN_0,
    KEY_EN_P,
    "Key: Ö",
    KEY_EN_PERIOD,
    KEY_EN_RIGHT_ALT,
    KEY_EN_UNUSED,
    "Key: ß",
    "Key: Ü",
    "Key: Ä",
    KEY_EN_MINUS,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_UNUSED,
    KEY_EN_F9,
    "Key: ´",
    "Key: +",
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_MENU,
    KEY_EN_F10,
    KEY_EN_F11,
    KEY_EN_F12,
    KEY_EN_BACKSPACE,
    KEY_EN_ANSI_ENTER,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_POUND,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_PRINT_SCREEN,
    KEY_EN_INSERT,
    KEY_EN_DELETE,
    KEY_EN_LEFT_ARROW,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_HOME,
    KEY_EN_END,
    KEY_EN_UP_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_PAUSE_BREAK,
    KEY_EN_PAGE_UP,
    KEY_EN_PAGE_DOWN,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_UNUSED,
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_0,
    KEY_EN_UNUSED,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_2,
    KEY_EN_UNUSED,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_3,
    "Key: Number Pad ,",
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_NUMPAD_ENTER,
};

/**------------------------------------------------------------------*\
    @name Roccat Vulcan Aimo
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectRoccatKeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RoccatVulcanAimo::RGBController_RoccatVulcanAimo(RoccatVulcanAimoController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Roccat Vulcan 120 Aimo";
    vendor      = "Roccat";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Roccat Vulcan Aimo Keyboard";
    version     = controller->GetVersion();
    location    = controller->GetLocation();
    serial      = controller->GetSerial();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = ROCCAT_VULCAN_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name             = "Static";
    Static.value            = ROCCAT_VULCAN_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.brightness_min   = ROCCAT_VULCAN_BRIGHTNESS_MIN;
    Static.brightness_max   = ROCCAT_VULCAN_BRIGHTNESS_MAX;
    Static.brightness       = ROCCAT_VULCAN_BRIGHTNESS_DEFAULT;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Wave;
    Wave.name           = "Rainbow Wave";
    Wave.value          = ROCCAT_VULCAN_MODE_WAVE;
    Wave.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.speed_min      = ROCCAT_VULCAN_SPEED_MIN;
    Wave.speed_max      = ROCCAT_VULCAN_SPEED_MAX;
    Wave.speed          = ROCCAT_VULCAN_SPEED_DEFAULT;
    Wave.brightness_min = ROCCAT_VULCAN_BRIGHTNESS_MIN;
    Wave.brightness_max = ROCCAT_VULCAN_BRIGHTNESS_MAX;
    Wave.brightness     = ROCCAT_VULCAN_BRIGHTNESS_DEFAULT;
    Wave.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Wave);

    SetupZones();
}

RGBController_RoccatVulcanAimo::~RGBController_RoccatVulcanAimo()
{
    delete controller;
}

void RGBController_RoccatVulcanAimo::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones and leds per zone                            |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name               = ZONE_EN_KEYBOARD;
    new_zone.type               = ZONE_TYPE_MATRIX;
    new_zone.leds_min           = zone_size;
    new_zone.leds_max           = zone_size;
    new_zone.leds_count         = zone_size;
    new_zone.matrix_map         = new matrix_map_type;
    new_zone.matrix_map->height = 6;
    new_zone.matrix_map->width  = 22;
    new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
    zones.push_back(new_zone);

    for(unsigned int led_idx = 0; led_idx < zone_size; led_idx++)
    {
        led new_led;
        new_led.name = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_RoccatVulcanAimo::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RoccatVulcanAimo::DeviceUpdateLEDs()
{
    if (modes[active_mode].value == ROCCAT_VULCAN_MODE_DIRECT)
    {
        controller->SendColors(colors);
    }
    else
    {
        DeviceUpdateMode();
    }
}

void RGBController_RoccatVulcanAimo::UpdateZoneLEDs(int /*zone_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatVulcanAimo::UpdateSingleLED(int /*led_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatVulcanAimo::DeviceUpdateMode()
{
    controller->SendMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, colors);
}
