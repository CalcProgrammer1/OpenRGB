/*-----------------------------------------*\
|  RGBController_CorsairK100.cpp            |
|                                           |
|  Driver for Corsair K100 Keyboard         |
\*-----------------------------------------*/

#include "RGBControllerKeyNames.h"
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
    KEY_EN_ESCAPE,              //0
    KEY_EN_BACK_TICK,           //1
    KEY_EN_TAB,                 //2
    KEY_EN_CAPS_LOCK,           //3
    KEY_EN_LEFT_SHIFT,          //4
    KEY_EN_LEFT_CONTROL,        //5
    KEY_EN_F12,                 //6
    KEY_EN_EQUALS,              //7
    "Key: Lock",                //8
    KEY_EN_NUMPAD_7,            //9
    KEY_EN_F1,                  //12
    KEY_EN_1,                   //13
    KEY_EN_Q,                   //14
    KEY_EN_A,                   //15
    KEY_EN_LEFT_WINDOWS,        //17
    KEY_EN_PRINT_SCREEN,        //18
    KEY_EN_MEDIA_MUTE,          //20
    KEY_EN_NUMPAD_8,            //21
    KEY_EN_F2,                  //24
    KEY_EN_2,                   //25
    KEY_EN_W,                   //26
    KEY_EN_S,                   //27
    KEY_EN_Z,                   //28
    KEY_EN_LEFT_ALT,            //29
    KEY_EN_SCROLL_LOCK,         //30
    KEY_EN_BACKSPACE,           //31
    KEY_EN_MEDIA_STOP,          //32
    KEY_EN_NUMPAD_9,            //33
    KEY_EN_F3,                  //36
    KEY_EN_3,                   //37
    KEY_EN_E,                   //38
    KEY_EN_D,                   //39
    KEY_EN_X,                   //40
    KEY_EN_PAUSE_BREAK,         //42
    KEY_EN_DELETE,              //43
    KEY_EN_MEDIA_PREVIOUS,      //44
    KEY_EN_F4,                  //48
    KEY_EN_4,                   //49
    KEY_EN_R,                   //50
    KEY_EN_F,                   //51
    KEY_EN_C,                   //52
    KEY_EN_SPACE,               //53
    KEY_EN_INSERT,              //54
    KEY_EN_END,                 //55
    KEY_EN_MEDIA_PLAY_PAUSE,    //56
    KEY_EN_NUMPAD_4,            //57
    KEY_EN_F5,                  //60
    KEY_EN_5,                   //61
    KEY_EN_T,                   //62
    KEY_EN_G,                   //63
    KEY_EN_V,                   //64
    KEY_EN_HOME,                //66
    KEY_EN_PAGE_DOWN,           //67
    KEY_EN_MEDIA_NEXT,          //68
    KEY_EN_NUMPAD_5,            //69
    KEY_EN_F6,                  //72
    KEY_EN_6,                   //73
    KEY_EN_Y,                   //74
    KEY_EN_H,                   //75
    KEY_EN_B,                   //76
    KEY_EN_PAGE_UP,             //78
    KEY_EN_RIGHT_SHIFT,         //79
    KEY_EN_NUMPAD_LOCK,         //80
    KEY_EN_NUMPAD_6,            //81
    KEY_EN_F7,                  //84
    KEY_EN_7,                   //85
    KEY_EN_U,                   //86
    KEY_EN_J,                   //87
    KEY_EN_N,                   //88
    KEY_EN_RIGHT_ALT,           //89
    KEY_EN_RIGHT_BRACKET,       //90
    KEY_EN_RIGHT_CONTROL,       //91
    KEY_EN_NUMPAD_DIVIDE,       //92
    KEY_EN_NUMPAD_1,            //93
    KEY_EN_F8,                  //96
    KEY_EN_8,                   //97
    KEY_EN_I,                   //98
    KEY_EN_K,                   //99
    KEY_EN_M,                   //100
    KEY_EN_RIGHT_WINDOWS,       //101
    KEY_EN_ANSI_BACK_SLASH,     //102
    KEY_EN_UP_ARROW,            //103
    KEY_EN_NUMPAD_TIMES,        //104
    KEY_EN_NUMPAD_2,            //105
    KEY_EN_F9,                  //108
    KEY_EN_9,                   //109
    KEY_EN_O,                   //110
    KEY_EN_L,                   //111
    KEY_EN_COMMA,               //112
    KEY_EN_MENU,                //113
    KEY_EN_LEFT_ARROW,          //115
    KEY_EN_NUMPAD_MINUS,        //116
    KEY_EN_NUMPAD_3,            //117
    KEY_EN_F10,                 //120
    KEY_EN_0,                   //121
    KEY_EN_P,                   //122
    KEY_EN_SEMICOLON,           //123
    KEY_EN_PERIOD,              //124
    KEY_EN_ANSI_ENTER,          //126
    KEY_EN_DOWN_ARROW,          //127
    KEY_EN_NUMPAD_PLUS,         //128
    KEY_EN_NUMPAD_0,            //129
    KEY_EN_F11,                 //132
    KEY_EN_MINUS,               //133
    KEY_EN_LEFT_BRACKET,        //134
    KEY_EN_QUOTE,               //135
    KEY_EN_FORWARD_SLASH,       //136
    //    "Key: Brightness",    //137
    KEY_EN_RIGHT_ARROW,         //139
    KEY_EN_NUMPAD_ENTER,        //140
    KEY_EN_NUMPAD_PERIOD,       //141
    "Key: / (ISO)",
    KEY_EN_ISO_BACK_SLASH,
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

/**------------------------------------------------------------------*\
    @name Corsair K100 Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairK100Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairK100::RGBController_CorsairK100(CorsairK100Controller* controller_ptr)
{
    controller      = controller_ptr;

    name            = controller->GetName();
    vendor          = "Corsair";
    description     = "Corsair K100 Keyboard Device";
    type            = DEVICE_TYPE_KEYBOARD;
    version         = controller->GetFirmwareString();
    location        = controller->GetDeviceLocation();
    serial          = controller->GetSerialString();

    logical_layout  = controller->GetKeyboardType();

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

    delete controller;
}

void RGBController_CorsairK100::SetupZones()
{
    std::vector<led_zone_layout> selected_zone;

    switch(logical_layout)
    {
        case CORSAIR_TYPE_K100:
            selected_zone.push_back({ZONE_EN_KEYBOARD,  119,    ZONE_TYPE_MATRIX, led_names_k100, new matrix_map_type{7, 24, (unsigned int *)&matrix_map_k100}});
            selected_zone.push_back({"Underglow",       44,     ZONE_TYPE_LINEAR, led_names_lightbar, NULL});
            selected_zone.push_back({"Nameplate",       9,      ZONE_TYPE_LINEAR, led_names_nameplate, NULL});
            selected_zone.push_back({"Logo",            3,      ZONE_TYPE_LINEAR, led_names_logo, NULL});
            break;

        case CORSAIR_TYPE_UNKNOWN:
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

    controller->SetLEDs(colors);
}

void RGBController_CorsairK100::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetLEDs(colors);
}

void RGBController_CorsairK100::UpdateSingleLED(int /*led*/)
{
    controller->SetLEDs(colors);
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
