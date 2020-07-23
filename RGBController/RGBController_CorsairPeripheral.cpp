/*-----------------------------------------*\
|  RGBController_CorsairPeripheral.cpp      |
|                                           |
|  Generic RGB Interface for Corsair RGB    |
|  keyboard, mouse, and mousemat devices    |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/9/2020    |
\*-----------------------------------------*/

#include "RGBController_CorsairPeripheral.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  10,  18,  28,  36,  NA,  46,  55,  64,  74,  NA,  84,  93, 102,   6,  15,  24,  33,  26,  35,  44,  53 },
      {   1,  11,  19,  29,  37,  47,  56,  65,  75,  85,  94,  NA, 103,   7,  25,  NA,  42,  51,  60,  62,  72,  82,  91 },
      {   2,  NA,  12,  20,  30,  38,  NA,  48,  57,  66,  76,  86,  95, 104,  70,  80,  34,  43,  52,  9,   17,  27, 100 },
      {   3,  NA,  13,  21,  31,  39,  NA,  49,  58,  67,  77,  87,  96, 105,  98,  NA,  NA,  NA,  NA,  45,  54,  63,  NA },
      {   4,  NA,  22,  32,  40,  50,  NA,  59,  NA,  68,  78,  88,  97, 106,  61,  NA,  NA,  81,  NA,  73,  83,  92, 109 },
      {   5,  14,  23,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  69,  79,  89,  71,  90,  99, 108, 101,  NA, 110,  NA } };

static const char* zone_names[] =
{
    "Keyboard"
};

static const unsigned int zone_sizes[] =
{
    111
};

static const zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX
};

static const char* led_names[] =
{
    "Key: Escape",          //0
    "Key: `",               //1
    "Key: Tab",             //2
    "Key: Caps Lock",       //3
    "Key: Left Shift",      //4
    "Key: Left Control",    //5
    "Key: F12",             //6
    "Key: =",               //7
    "Key: Lock",            //8
    "Key: Number Pad 7",    //9
    "Key: F1",              //12
    "Key: 1",               //13
    "Key: Q",               //14
    "Key: A",               //15
    "Key: Left Windows",    //17
    "Key: Print Screen",    //18
    "Key: Media Mute",      //20
    "Key: Number Pad 8",    //21
    "Key: F2",              //24
    "Key: 2",               //25
    "Key: W",               //26
    "Key: S",               //27
    "Key: Z",               //28
    "Key: Left Alt",        //29
    "Key: Scroll Lock",     //30
    "Key: Backspace",       //31
    "Key: Media Stop",      //32
    "Key: Number Pad 9",    //33
    "Key: F3",              //36
    "Key: 3",               //37
    "Key: E",               //38
    "Key: D",               //39
    "Key: X",               //40
    "Key: Pause/Break",     //42
    "Key: Delete",          //43
    "Key: Media Previous",  //44
    "Key: F4",              //48
    "Key: 4",               //49
    "Key: R",               //50
    "Key: F",               //51
    "Key: C",               //52
    "Key: Space",           //53
    "Key: Insert",          //54
    "Key: End",             //55
    "Key: Media Play/Pause",//56
    "Key: Number Pad 4",    //57
    "Key: F5",              //60
    "Key: 5",               //61
    "Key: T",               //62
    "Key: G",               //63
    "Key: V",               //64
    "Key: Home",            //66
    "Key: Page Down",       //67
    "Key: Media Next",      //68
    "Key: Number Pad 5",    //69
    "Key: F6",              //72
    "Key: 6",               //73
    "Key: Y",               //74
    "Key: H",               //75
    "Key: B",               //76
    "Key: Page Up",         //78
    "Key: Right Shift",     //79
    "Key: Num Lock",        //80
    "Key: Number Pad 6",    //81
    "Key: F7",              //84
    "Key: 7",               //85
    "Key: U",               //86
    "Key: J",               //87
    "Key: N",               //88
    "Key: Right Alt",       //89
    "Key: ]",               //90
    "Key: Right Control",   //91
    "Key: Number Pad /",    //92
    "Key: Number Pad 1",    //93
    "Key: F8",              //96
    "Key: 8",               //97
    "Key: I",               //98
    "Key: K",               //99
    "Key: M",               //100
    "Key: Right Windows",   //101
    "Key: \\ (ANSI)",       //102
    "Key: Up Arrow",        //103
    "Key: Number Pad *",    //104
    "Key: Number Pad 2",    //105
    "Key: F9",              //108
    "Key: 9",               //109
    "Key: O",               //110
    "Key: L",               //111
    "Key: ,",               //112
    "Key: Context",         //113
    "Key: Left Arrow",      //115
    "Key: Number Pad -",    //116
    "Key: Number Pad 3",    //117
    "Key: F10",             //120
    "Key: 0",               //121
    "Key: P",               //122
    "Key: ;",               //123
    "Key: .",               //124
    "Key: Enter",           //126
    "Key: Down Arrow",      //127
    "Key: Number Pad +",    //128
    "Key: Number Pad 0",    //129
    "Key: F11",             //132
    "Key: -",               //133
    "Key: [",               //134
    "Key: '",               //135
    "Key: /",               //136
    "Key: Brightness",      //137
    "Key: Right Arrow",     //139
    "Key: Number Pad Enter",//140
    "Key: Number Pad .",    //141
};

RGBController_CorsairPeripheral::RGBController_CorsairPeripheral(CorsairPeripheralController* corsair_ptr)
{
    corsair = corsair_ptr;

    name        = "Corsair RGB Peripheral Device";
    description = "Corsair RGB Peripheral Device";
    type        = corsair->GetDeviceType();
    version     = corsair->GetFirmwareString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_CorsairPeripheral::~RGBController_CorsairPeripheral()
{

}

void RGBController_CorsairPeripheral::SetupZones()
{
    /*---------------------------------------------------------*\
    | Determine number of zones                                 |
    |   For now, keyboard has 2 zones and mousemat has 1        |
    \*---------------------------------------------------------*/
    unsigned int num_zones = 0;

    switch(type)
    {
        case DEVICE_TYPE_KEYBOARD:
            num_zones = 1;
            break;

        case DEVICE_TYPE_MOUSE:
        case DEVICE_TYPE_MOUSEMAT:
            num_zones = 1;
            break;

        case DEVICE_TYPE_HEADSET_STAND:
            num_zones = 2;
            break;
    }

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < num_zones; zone_idx++)
    {
        zone new_zone;
        switch(type)
        {
            case DEVICE_TYPE_KEYBOARD:
                new_zone.name                   = zone_names[zone_idx];
                new_zone.type                   = zone_types[zone_idx];
                new_zone.leds_min               = zone_sizes[zone_idx];
                new_zone.leds_max               = zone_sizes[zone_idx];
                new_zone.leds_count             = zone_sizes[zone_idx];
                
                if(zone_types[zone_idx] == ZONE_TYPE_MATRIX)
                {
                    new_zone.matrix_map         = new matrix_map_type;
                    new_zone.matrix_map->height = 6;
                    new_zone.matrix_map->width  = 23;
                    new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
                }
                else
                {
                    new_zone.matrix_map         = NULL;
                }
                break;

            case DEVICE_TYPE_MOUSE:
                new_zone.name           = "Mousemat Zone";
                new_zone.type           = ZONE_TYPE_SINGLE;
                new_zone.leds_min       = 15;
                new_zone.leds_max       = 15;
                new_zone.leds_count     = 15;
                new_zone.matrix_map     = NULL;
                break;

            case DEVICE_TYPE_MOUSEMAT:
                new_zone.name           = "Mousemat Zone";
                new_zone.type           = ZONE_TYPE_LINEAR;
                new_zone.leds_min       = 15;
                new_zone.leds_max       = 15;
                new_zone.leds_count     = 15;
                new_zone.matrix_map     = NULL;
                break;

            case DEVICE_TYPE_HEADSET_STAND:
                if(zone_idx == 0)
                {
                    new_zone.name           = "Base LED Strip";
                    new_zone.type           = ZONE_TYPE_LINEAR;
                    new_zone.leds_min       = 8;
                    new_zone.leds_max       = 8;
                    new_zone.leds_count     = 8;
                    new_zone.matrix_map     = NULL;
                }
                else
                {
                    new_zone.name           = "Logo";
                    new_zone.type           = ZONE_TYPE_SINGLE;
                    new_zone.leds_min       = 1;
                    new_zone.leds_max       = 1;
                    new_zone.leds_count     = 1;
                    new_zone.matrix_map     = NULL;
                }
                break;
        }

        zones.push_back(new_zone);

        total_led_count += new_zone.leds_count;
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;

        switch(type)
        {
            case DEVICE_TYPE_KEYBOARD:
                new_led.name = led_names[led_idx];
                break;

            case DEVICE_TYPE_MOUSE:
            case DEVICE_TYPE_MOUSEMAT:
            case DEVICE_TYPE_HEADSET_STAND:
                new_led.name = "Mousemat LED ";
                new_led.name.append(std::to_string(led_idx + 1));
                break;
        }
        
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_CorsairPeripheral::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairPeripheral::DeviceUpdateLEDs()
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairPeripheral::UpdateZoneLEDs(int /*zone*/)
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairPeripheral::UpdateSingleLED(int /*led*/)
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairPeripheral::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_CorsairPeripheral::DeviceUpdateMode()
{

}
