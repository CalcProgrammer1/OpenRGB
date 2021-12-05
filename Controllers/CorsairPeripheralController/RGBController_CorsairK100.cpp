/*-----------------------------------------*\
|  RGBController_CorsairK100.cpp            |
|                                           |
|  Driver for Corsair K100 Keyboard         |
\*-----------------------------------------*/

#include "RGBController_CorsairK100.h"
#include "LogManager.h"

using namespace std::chrono_literals;

#define NA  0xFFFFFFFF

static unsigned int matrix_map_k100[7][24] =
    { {  NA, 112,  NA,   8,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  16,  NA,  NA,  NA,},
      { 113,   0,  NA,  10,  18,  28,  36,  NA,  46,  55,  64,  74,  NA,  84,  93, 102,   6,  15,  24,  33,  26,  35,  44,  53 },
      { 114,   1,  11,  19,  29,  37,  47,  56,  65,  75,  85,  94,  NA, 103,   7,  25,  NA,  42,  51,  60,  62,  72,  82,  91 },
      { 115,   2,  NA,  12,  20,  30,  38,  NA,  48,  57,  66,  76,  86,  95, 104,  70,  80,  34,  43,  52,  9,   17,  27, 100 },
      { 116,   3,  NA,  13,  21,  31,  39,  NA,  49,  58,  67,  77,  87,  96, 105,  98, 111,  NA,  NA,  NA,  45,  54,  63,  NA },
      { 117,   4, 110,  22,  32,  40,  50,  NA,  59,  NA,  68,  78,  88,  97, 106,  61,  NA,  NA,  81,  NA,  73,  83,  92, 108 },
      { 118,   5,  14,  23,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  69,  79,  89,  71,  90,  99, 107, 101,  NA, 109,  NA } };

static const char* led_names_k100[] =
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
    "Key: Menu",            //113
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
//    "Key: Brightness",      //137
    "Key: Right Arrow",     //139
    "Key: Number Pad Enter",//140
    "Key: Number Pad .",    //141
    "Key: / (ISO)",
    "Key: \\ (ISO)",
    "Key: Preset",
    "Key: G1",
    "Key: G2",
    "Key: G3",
    "Key: G4",
    "Key: G5",
    "Key: G6",
};
static const char* led_names_lightbar[] =
{
    "Underglow 1",
    "Underglow 2",
    "Underglow 3",
    "Underglow 4",
    "Underglow 5",
    "Underglow 6",
    "Underglow 7",
    "Underglow 8",
    "Underglow 9",
    "Underglow 10",
    "Underglow 11",
    "Underglow 12",
    "Underglow 13",
    "Underglow 14",
    "Underglow 15",
    "Underglow 16",
    "Underglow 17",
    "Underglow 18",
    "Underglow 19",
    "Underglow 20",
    "Underglow 21",
    "Underglow 22",
    "Underglow 23",
    "Underglow 24",
    "Underglow 25",
    "Underglow 26",
    "Underglow 27",
    "Underglow 28",
    "Underglow 29",
    "Underglow 30",
    "Underglow 31",
    "Underglow 32",
    "Underglow 33",
    "Underglow 34",
    "Underglow 35",
    "Underglow 36",
    "Underglow 37",
    "Underglow 38",
    "Underglow 39",
    "Underglow 40",
    "Underglow 41",
    "Underglow 42",
    "Underglow 43",
    "Underglow 44",
};
static const char* led_names_nameplate[] =
{
    "Nameplate 1",
    "Nameplate 2",
    "Nameplate 3",
    "Nameplate 4",
    "Nameplate 5",
    "Nameplate 6",
    "Nameplate 7",
    "Nameplate 8",
    "Nameplate 9",
};
static const char* led_names_logo[] =
{
    "Logo",
    "Logo",
    "Logo",
};

typedef struct
{
    const char* name;
    const unsigned int size;
    const zone_type type;
    const char** led_names;
    matrix_map_type* matrix;
} led_zone_layout;

RGBController_CorsairK100::RGBController_CorsairK100(CorsairK100Controller* corsair_ptr)
{
    corsair         = corsair_ptr;

    name            = corsair->GetName();
    vendor          = "Corsair";
    description     = "Corsair K100 Keyboard Device";
    type            = DEVICE_TYPE_KEYBOARD;
    version         = corsair->GetFirmwareString();
    location        = corsair->GetDeviceLocation();
    serial          = corsair->GetSerialString();

    logical_layout  = corsair->GetKeyboardType();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
    /*-----------------------------------------------------*\
    | The Corsair K100 requires a packet within             |
    | 1 minutes of sending the lighting change in order     |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 50 sec  |
    \*-----------------------------------------------------*/
    keepalive_thread_run = true;
    keepalive_thread = new std::thread(&RGBController_CorsairK100::KeepaliveThread, this);
}

RGBController_CorsairK100::~RGBController_CorsairK100()
{
    /*-----------------------------------------------------*\
    | Close keepalive thread                                |
    \*-----------------------------------------------------*/
    keepalive_thread_run = false;
    keepalive_thread->join();
    delete keepalive_thread;

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

    delete corsair;
}

void RGBController_CorsairK100::SetupZones()
{
    std::vector<led_zone_layout> selected_zone;

    switch(logical_layout)
    {
        case CORSAIR_TYPE_K100:
            selected_zone.push_back({"Keyboard",    119,    ZONE_TYPE_MATRIX, led_names_k100, new matrix_map_type{7, 24, (unsigned int *)&matrix_map_k100}});
            selected_zone.push_back({"Underglow",   44,     ZONE_TYPE_LINEAR, led_names_lightbar});
            selected_zone.push_back({"Nameplate",   9,      ZONE_TYPE_LINEAR, led_names_nameplate});
            selected_zone.push_back({"Logo",        3,      ZONE_TYPE_LINEAR, led_names_logo});
            break;
    }

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < selected_zone.size(); zone_idx++)
    {
        zone new_zone;

        new_zone.name                   = selected_zone[zone_idx].name;
        new_zone.type                   = selected_zone[zone_idx].type;
        new_zone.leds_min               = selected_zone[zone_idx].size;
        new_zone.leds_max               = selected_zone[zone_idx].size;
        new_zone.leds_count             = selected_zone[zone_idx].size;

        if (new_zone.type == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map         = selected_zone[zone_idx].matrix;
        }
        else
        {
            new_zone.matrix_map         = NULL;
        }

        zones.push_back(new_zone);

        for(unsigned int led_idx = 0; led_idx < selected_zone[zone_idx].size; led_idx++)
        {
            led new_led;
            new_led.name = selected_zone[zone_idx].led_names[led_idx];
            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_CorsairK100::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairK100::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    corsair->SetLEDs(colors);
}

void RGBController_CorsairK100::UpdateZoneLEDs(int /*zone*/)
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairK100::UpdateSingleLED(int /*led*/)
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairK100::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_CorsairK100::DeviceUpdateMode()
{

}

void RGBController_CorsairK100::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50000))
            {
                DeviceUpdateLEDs();
            }
        }
        std::this_thread::sleep_for(3000ms);
    }
}
