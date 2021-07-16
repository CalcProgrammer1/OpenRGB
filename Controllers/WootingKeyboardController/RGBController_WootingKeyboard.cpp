/*-----------------------------------------*\
|  RGBController_WootingKeyboard.cpp        |
|                                           |
|  Generic RGB Interface for Wooting RGB    |
|  keyboard devices                         |
|                                           |
|  Diogo Trindade (diogotr7)    3/4/2021    |
\*-----------------------------------------*/

#include "RGBController_WootingKeyboard.h"
#include "LogManager.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA 0xFFFFFFFF
#define RGB_RAW_BUFFER_SIZE 96

static unsigned int matrix_map_tkl[6][17] =
{
    { 0,  NA,  12,  18,  24,  30,  36,  42,  48,  54,  60,  66,  72,  78,  84,  90,  96},
    { 1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  73,  79,  85,  91,  97},
    { 2,   8,  14,  20,  26,  32,  38,  44,  50,  56,  62,  68,  74,  80,  86,  92,  98},
    { 3,   9,  15,  21,  27,  33,  39,  45,  51,  57,  63,  69,  75,  81,  NA,  NA,  NA},
    { 4,  10,  16,  22,  28,  34,  40,  46,  52,  58,  64,  70,  NA,  82,  NA,  94,  NA},
    { 5,  11,  17,  NA,  NA,  NA,  41,  NA,  NA,  NA,  65,  71,  77,  83,  89,  95,  101}
};

static unsigned int matrix_map_full[6][21] =
{
    {  0,  NA,  12,  18,  24,  30,  36,  42,  48,  54,  60,  66,  72,  78,  84,  90, 100, 102, 108, 114, 120},
    {  1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  73,  79,  85,  91,  97, 103, 109, 115, 121},
    {  2,   8,  14,  20,  26,  32,  38,  44,  50,  56,  62,  68,  74,  80,  86,  92,  98, 104, 110, 116, 122},
    {  3,   9,  15,  21,  27,  33,  39,  45,  51,  57,  63,  69,  75,  81,  NA,  NA,  NA, 105, 111, 117,  NA},
    {  4,  10,  16,  22,  28,  34,  40,  46,  52,  58,  64,  70,  NA,  82,  NA,  94,  NA, 106, 112, 118, 124},
    {  5,  11,  17,  NA,  NA,  NA,  41,  NA,  NA,  NA,  65,  71,  77,  83,  89,  95, 101,  NA, 113, 119,  NA}
};

static const unsigned int zone_sizes[] =
{
    102,
    126
};

static const char *led_names[] =
{
    "Key: Escape",
    "Key: `",
    "Key: Tab",
    "Key: Caps Lock",
    "Key: Left Shift",
    "Key: Left Control",
    "Unused",
    "Key: 1",
    "Key: Q",
    "Key: A",
    "Key: \\ (ISO)",        //iso key - 10
    "Key: Left Windows",
    "Key: F1",
    "Key: 2",
    "Key: W",
    "Key: S",
    "Key: Z",
    "Key: Left Alt",
    "Key: F2",
    "Key: 3",
    "Key: E",               //20
    "Key: D",
    "Key: X",
    "Unused", //space
    "Key: F3",
    "Key: 4",
    "Key: R",
    "Key: F",
    "Key: C",
    "Unused",  //space
    "Key: F4",              //30
    "Key: 5",
    "Key: T",
    "Key: G",
    "Key: V",
    "Unused", //space
    "Key: F5",
    "Key: 6",
    "Key: Y",
    "Key: H",
    "Key: B",               //40
    "Key: Space",
    "Key: F6",
    "Key: 7",
    "Key: U",
    "Key: J",
    "Key: N",
    "Unused", //space
    "Key: F7",
    "Key: 8",
    "Key: I",               //50
    "Key: K",
    "Key: M",
    "Unused", //space
    "Key: F8",
    "Key: 9",
    "Key: O",
    "Key: L",
    "Key: ,",
    "Unused", //space
    "Key: F9",              //60
    "Key: 0",
    "Key: P",
    "Key: ;",
    "Key: .",
    "Key: Right Alt",
    "Key: F10",
    "Key: -",
    "Key: [",
    "Key: '",
    "Key: /",               //70
    "Key: Right Windows",
    "Key: F11",
    "Key: =",
    "Key: ]",
    "Key: #", //iso only
    "Unused",
    "Key: Right Fn",
    "Key: F12",
    "Key: Backspace",
    "Key: \\ (ANSI)",       //80
    "Key: Enter",
    "Key: Right Shift",
    "Key: Right Control",
    "Key: Print Screen",
    "Key: Insert",
    "Key: Delete",
    "Unused",
    "Unused",
    "Key: Left Arrow",
    "Key: Pause/Break",     //90
    "Key: Home",
    "Key: End",
    "Unused",
    "Key: Up Arrow",
    "Key: Down Arrow",
    "Key: Mode",
    "Key: Page Up",
    "Key: Page Down",
    "Unused",
    "Key: Scroll Lock",     //100 - Scroll lock for WootingTwo KB's
    "Key: Right Arrow",
    "Key: A1",
    "Key: Num Lock",
    "Key: Number Pad 7",
    "Key: Number Pad 4",
    "Key: Number Pad 1",
    "Unused",
    "Key: A2",
    "Key: Number Pad /",
    "Key: Number Pad 8",    //110
    "Key: Number Pad 5",
    "Key: Number Pad 2",
    "Key: Number Pad 0",
    "Key: A3",
    "Key: Number Pad *",
    "Key: Number Pad 9",
    "Key: Number Pad 6",
    "Key: Number Pad 3",
    "Key: Number Pad .",
    "Key: Mode",            //120 - Mode key for WootingTwo KB's
    "Key: Number Pad -",
    "Key: Number Pad +",
    "Unused",
    "Key: Number Pad Enter",
    "Unused"
};

RGBController_WootingKeyboard::RGBController_WootingKeyboard(WootingKeyboardController *wooting_ptr)
{
    wooting     = wooting_ptr;

    LOG_DEBUG("%sAdding meta data", WOOTING_CONTROLLER_NAME);
    name        = wooting_ptr->GetName();
    vendor      = wooting_ptr->GetVendor();
    type        = DEVICE_TYPE_KEYBOARD;
    description = wooting_ptr->GetDescription();
    location    = wooting_ptr->GetLocation();
    serial      = wooting_ptr->GetSerial();

    LOG_DEBUG("%sAdding modes", WOOTING_CONTROLLER_NAME);
    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_WootingKeyboard::~RGBController_WootingKeyboard()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for (unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if (zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }
}

void RGBController_WootingKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    uint8_t         wooting_type    = wooting->GetWootingType();
    unsigned int    total_led_count = zone_sizes[wooting_type];

    LOG_DEBUG("%sCreating New Zone", WOOTING_CONTROLLER_NAME);
    zone new_zone;

    new_zone.name                   = name.append(" zone");
    new_zone.type                   = ZONE_TYPE_MATRIX;
    new_zone.leds_min               = total_led_count;
    new_zone.leds_max               = total_led_count;
    new_zone.leds_count             = total_led_count;
    new_zone.matrix_map             = new matrix_map_type;
    new_zone.matrix_map->height     = WOOTING_RGB_ROWS;

    if(wooting_type == WOOTING_KB_TKL)
    {
        new_zone.matrix_map->width  =  WOOTING_ONE_RGB_COLUMNS;
        new_zone.matrix_map->map    = (unsigned int *)&matrix_map_tkl;
    }
    else
    {
        new_zone.matrix_map->width  =  WOOTING_TWO_RGB_COLUMNS;
        new_zone.matrix_map->map    = (unsigned int *)&matrix_map_full;
    }

    zones.push_back(new_zone);

    LOG_DEBUG("%sCreating LED array - total_led_count %03i", WOOTING_CONTROLLER_NAME, total_led_count);
    for (unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;

        new_led.name                = led_names[led_idx];

        LOG_DEBUG("%sPushing LED %03i - %s into vector", WOOTING_CONTROLLER_NAME, led_idx, new_led.name.c_str());
        leds.push_back(new_led);
    }

    LOG_DEBUG("%sLEDs created - Initialising Colours", WOOTING_CONTROLLER_NAME);

    SetupColors();
}

void RGBController_WootingKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_WootingKeyboard::DeviceUpdateLEDs()
{
    wooting->SendDirect(&colors[0], colors.size());
}

void RGBController_WootingKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WootingKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WootingKeyboard::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_WootingKeyboard::DeviceUpdateMode()
{
}
