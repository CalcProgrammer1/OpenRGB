/*-----------------------------------------*\
|  RGBController_DuckyKeyboard.cpp          |
|                                           |
|  Generic RGB Interface for Ducky RGB      |
|  keyboard devices                         |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/4/2020    |
\*-----------------------------------------*/

#include "RGBController_DuckyKeyboard.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  12,  18,  24,  30,  NA,  42,  48,  54,  60,  NA,  66,  72,  78,  84,  90,  96, 102, 108, 114, 120, 126 },
      {   1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  NA,  67,  73,  85,  NA,  91,  97, 103, 109, 115, 121, 127 },
      {   2,  NA,   8,  14,  20,  26,  NA,  32,  38,  44,  50,  56,  62,  68,  74,  86,  92,  98, 104, 110, 116, 122, 128 },
      {   3,  NA,   9,  15,  21,  27,  NA,  33,  39,  45,  51,  57,  63,  69,  87,  NA,  NA,  NA,  NA, 111, 117, 123,  NA },
      {   4,  NA,  16,  22,  28,  34,  NA,  40,  NA,  46,  52,  58,  64,  70,  82,  NA,  NA, 100,  NA, 112, 118, 124, 131 },
      {   5,  11,  17,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  65,  77,  83,  89,  95, 101, 107, 113,  NA, 125,  NA } };

RGBController_DuckyKeyboard::RGBController_DuckyKeyboard(DuckyKeyboardController* ducky_ptr)
{
    ducky = ducky_ptr;

    name        = "Ducky Keyboard Device";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Ducky Keyboard Device";

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_DuckyKeyboard::~RGBController_DuckyKeyboard()
{

}

void RGBController_DuckyKeyboard::SetupZones()
{
    zone new_zone;

    new_zone.name               = "Keyboard";
    new_zone.type               = ZONE_TYPE_MATRIX;
    new_zone.leds_min           = 155;
    new_zone.leds_max           = 155;
    new_zone.leds_count         = 155;
    new_zone.matrix_map         = new matrix_map_type;
    new_zone.matrix_map->height = 6;
    new_zone.matrix_map->width  = 23;
    new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
    
    zones.push_back(new_zone);

    for(int led_idx = 0; led_idx < 155; led_idx++)
    {
        led new_led;

        new_led.name = "Keyboard LED ";
        new_led.name.append(std::to_string(led_idx));

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_DuckyKeyboard::ResizeZone(int zone, int new_size)
{
    
}
    
void RGBController_DuckyKeyboard::DeviceUpdateLEDs()
{
    unsigned char colordata[155*3];
    
    for(int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        colordata[(color_idx*3)+0] = RGBGetRValue(colors[color_idx]);
        colordata[(color_idx*3)+1] = RGBGetGValue(colors[color_idx]);
        colordata[(color_idx*3)+2] = RGBGetBValue(colors[color_idx]);
    }

    ducky->SendColors(colordata, sizeof(colordata));
}

void RGBController_DuckyKeyboard::UpdateZoneLEDs(int zone)
{
    DeviceUpdateLEDs();
}

void RGBController_DuckyKeyboard::UpdateSingleLED(int led)
{
    DeviceUpdateLEDs();
}

void RGBController_DuckyKeyboard::SetCustomMode()
{

}

void RGBController_DuckyKeyboard::DeviceUpdateMode()
{

}