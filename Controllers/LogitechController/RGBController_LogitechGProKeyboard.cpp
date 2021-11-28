/*-----------------------------------------*\
|  RGBController_LogitechGPro.cpp           |
|                                           |
|  Generic RGB Interface for Logitech G Pro |
|  Keyboard                                 |
|                                           |
|  sanchezzzs                   20/10/2021  |
\*-----------------------------------------*/

#include "RGBController_LogitechGProKeyboard.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[7][19] =
    { {  89,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  93,  92,  NA,  NA,  91,  NA,  90 },
      {  37,  NA,  54,  55,  56,  57,  NA,  58,  59,  60,  61,  NA,  62,  63,  64,  65,  66,  67,  68 },
      {  49,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  NA,  41,  42,  38,  NA,  69,  70,  71 },
      {  39,  NA,  16,  22,   4,  17,  NA,  19,  24,  20,   8,  14,  15,  43,  44,  45,  72,  73,  74 },
      {  53,  NA,   0,  18,   3,   5,  NA,   6,   7,   9,  10,  11,  47,  48,  46,  36,  NA,  NA,  NA },
      {  82,  79,  25,  23,   2,  21,  NA,   1,  NA,  13,  12,  50,  51,  52,  86,  NA,  NA,  78,  NA },
      {  81,  84,  83,  NA,  NA,  NA,  NA,  40,  NA,  NA,  NA,  NA,  87,  88,  80,  85,  76,  77,  75 } };

static const char* zone_names[] =
{
    "Keyboard",
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes[] =
{
    94,
};

typedef struct
{
    const char *        name;
    const unsigned char zone;
    const unsigned char idx;
} led_type;

static const led_type led_names[] =
{
    /* Key Label                Zone,                                   Index  */
    { "Key: A",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x04    },//00
    { "Key: B",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x05    },
    { "Key: C",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x06    },
    { "Key: D",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x07    },
    { "Key: E",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x08    },
    { "Key: F",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x09    },
    { "Key: G",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x0A    },
    { "Key: H",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x0B    },
    { "Key: I",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x0C    },
    { "Key: J",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x0D    },
    { "Key: K",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x0E    },//10
    { "Key: L",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x0F    },
    { "Key: M",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x10    },
    { "Key: N",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x11    },
    { "Key: O",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x12    },
    { "Key: P",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x13    },
    { "Key: Q",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x14    },
    { "Key: R",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x15    },
    { "Key: S",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x16    },
    { "Key: T",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x17    },
    { "Key: U",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x18    },//20
    { "Key: V",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x19    },
    { "Key: W",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x1A    },
    { "Key: X",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x1B    },
    { "Key: Y",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x1C    },
    { "Key: Z",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x1D    },
    { "Key: 1",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x1E    },
    { "Key: 2",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x1F    },
    { "Key: 3",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x20    },
    { "Key: 4",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x21    },
    { "Key: 5",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x22    },//30
    { "Key: 6",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x23    },
    { "Key: 7",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x24    },
    { "Key: 8",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x25    },
    { "Key: 9",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x26    },
    { "Key: 0",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x27    },
    { "Key: Enter",             LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x28    },
    { "Key: Escape",            LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x29    },
    { "Key: Backspace",         LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x2A    },
    { "Key: Tab",               LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x2B    },
    { "Key: Space",             LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x2C    },//40
    { "Key: -",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x2D    },
    { "Key: =",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x2E    },
    { "Key: [",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x2F    },
    { "Key: ]",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x30    },
    { "Key: \\ (ANSI)",         LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x31    },//ANSI only
    { "Key: #",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x32    },//ISO only
    { "Key: ;",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x33    },
    { "Key: '",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x34    },
    { "Key: `",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x35    },
    { "Key: ,",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x36    },//50
    { "Key: .",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x37    },
    { "Key: /",                 LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x38    },
    { "Key: Caps Lock",         LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x39    },
    { "Key: F1",                LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x3A    },
    { "Key: F2",                LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x3B    },
    { "Key: F3",                LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x3C    },
    { "Key: F4",                LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x3D    },
    { "Key: F5",                LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x3E    },
    { "Key: F6",                LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x3F    },
    { "Key: F7",                LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x40    },//60
    { "Key: F8",                LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x41    },
    { "Key: F9",                LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x42    },
    { "Key: F10",               LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x43    },
    { "Key: F11",               LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x44    },
    { "Key: F12",               LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x45    },
    { "Key: Print Screen",      LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x46    },
    { "Key: Scroll Lock",       LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x47    },
    { "Key: Pause/Break",       LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x48    },
    { "Key: Insert",            LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x49    },
    { "Key: Home",              LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x4A    },//70
    { "Key: Page Up",           LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x4B    },
    { "Key: Delete",            LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x4C    },
    { "Key: End",               LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x4D    },
    { "Key: Page Down",         LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x4E    },
    { "Key: Right Arrow",       LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x4F    },
    { "Key: Left Arrow",        LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x50    },
    { "Key: Down Arrow",        LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x51    },
    { "Key: Up Arrow",          LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x52    },    
    { "Key: \\ (ISO)",          LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x64    },//ISO only
    { "Key: Menu",              LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0x65    },//80
    { "Key: Left Control",      LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0xE0    },
    { "Key: Left Shift",        LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0xE1    },
    { "Key: Left Alt",          LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0xE2    },
    { "Key: Left Windows",      LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0xE3    },
    { "Key: Right Control",     LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0xE4    },
    { "Key: Right Shift",       LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0xE5    },
    { "Key: Right Alt",         LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0xE6    },
    { "Key: Fn",                LOGITECH_GPRO_ZONE_DIRECT_KEYBOARD,     0xE7    },   
    { "Logo",                   LOGITECH_GPRO_ZONE_DIRECT_LOGO,         0x01    },
    { "Lighting",               LOGITECH_GPRO_ZONE_DIRECT_INDICATORS,   0x01    },//90
    { "Game Mode",              LOGITECH_GPRO_ZONE_DIRECT_INDICATORS,   0x02    },
    { "Caps Lock Indicator",    LOGITECH_GPRO_ZONE_DIRECT_INDICATORS,   0x03    },
    { "Scroll Lock Indicator",  LOGITECH_GPRO_ZONE_DIRECT_INDICATORS,   0x04    },//93
};

RGBController_LogitechGProKeyboard::RGBController_LogitechGProKeyboard(LogitechGProKeyboardController* logitech_ptr)
{
    logitech = logitech_ptr;

    name        = "Logitech Keyboard Device";
    vendor      = "Logitech";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Logitech Keyboard Device";
    serial      = logitech->GetSerialString();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = 0xFFFF;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = LOGITECH_GPRO_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);
    
    mode Off;
    Off.name                        = "Off";
    Off.value                       = LOGITECH_GPRO_MODE_OFF;
    Off.flags                       = 0;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Cycle;
    Cycle.name                      = "Cycle";
    Cycle.value                     = LOGITECH_GPRO_MODE_CYCLE;
    Cycle.flags                     = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode                = MODE_COLORS_NONE;
    Cycle.speed_min                 = LOGITECH_GPRO_SPEED_SLOWEST;
    Cycle.speed_max                 = LOGITECH_GPRO_SPEED_FASTEST;
    Cycle.speed                     = LOGITECH_GPRO_SPEED_NORMAL;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = LOGITECH_GPRO_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.speed_min             = LOGITECH_GPRO_SPEED_SLOWEST;
    Breathing.speed_max             = LOGITECH_GPRO_SPEED_FASTEST;
    Breathing.speed                 = LOGITECH_GPRO_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_LogitechGProKeyboard::~RGBController_LogitechGProKeyboard()
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

    delete logitech;
}

void RGBController_LogitechGProKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        zone new_zone;
        new_zone.name                   = zone_names[zone_idx];
        new_zone.type                   = zone_types[zone_idx];
        new_zone.leds_min               = zone_sizes[zone_idx];
        new_zone.leds_max               = zone_sizes[zone_idx];
        new_zone.leds_count             = zone_sizes[zone_idx];

        if(zone_types[zone_idx] == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map         = new matrix_map_type;
            new_zone.matrix_map->height = 7;
            new_zone.matrix_map->width  = 19;
            new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
        }
        else
        {
            new_zone.matrix_map         = NULL;
        }

        zones.push_back(new_zone);

        total_led_count += zone_sizes[zone_idx];
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name  = led_names[led_idx].name;
        new_led.value = ( led_names[led_idx].zone << 8 ) + led_names[led_idx].idx;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_LogitechGProKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechGProKeyboard::DeviceUpdateLEDs()
{
    #define MAX_FRAMES_PER_PACKET 0x0E

    unsigned char frame_buf[MAX_FRAMES_PER_PACKET * 4];
    unsigned char frame_cnt = 0;
    unsigned char prev_zone = 0;
    unsigned char zone      = 0;
    unsigned char idx       = 0;

    /*---------------------------------------------------------*\
    | TODO: Send packets with multiple LED frames               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        zone = ( leds[led_idx].value >> 8 );
        idx  = ( leds[led_idx].value & 0xFF );

        if((zone != prev_zone) && (frame_cnt != 0))
        {
            logitech->SetDirect(prev_zone, frame_cnt, frame_buf);
            frame_cnt = 0;
        }

        frame_buf[(frame_cnt * 4) + 0] = idx;
        frame_buf[(frame_cnt * 4) + 1] = RGBGetRValue(colors[led_idx]);
        frame_buf[(frame_cnt * 4) + 2] = RGBGetGValue(colors[led_idx]);
        frame_buf[(frame_cnt * 4) + 3] = RGBGetBValue(colors[led_idx]);
        
        frame_cnt++;
        prev_zone = zone;

        if(frame_cnt == MAX_FRAMES_PER_PACKET)
        {
            logitech->SetDirect(prev_zone, frame_cnt, frame_buf);
            frame_cnt = 0;
        }
    }

    if(frame_cnt != 0)
    {
        logitech->SetDirect(prev_zone, frame_cnt, frame_buf);
    }

    logitech->Commit();
}

void RGBController_LogitechGProKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechGProKeyboard::UpdateSingleLED(int led)
{
    unsigned char frame[4];
    unsigned char zone;
    unsigned char idx;

    zone = ( leds[led].value >> 8 );
    idx  = ( leds[led].value & 0xFF );

    frame[0] = idx;
    frame[1] = RGBGetRValue(colors[led]);
    frame[2] = RGBGetGValue(colors[led]);
    frame[3] = RGBGetBValue(colors[led]);

    logitech->SetDirect(zone, 1, frame);
    logitech->Commit();
}

void RGBController_LogitechGProKeyboard::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_LogitechGProKeyboard::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | Direct mode does not send a mode packet                   |
    | Call UpdateLEDs to send direct packet                     |
    \*---------------------------------------------------------*/
    if(active_mode == 0xFFFF)
    {
        UpdateLEDs();
        return;
    }

    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    logitech->SetMode(modes[active_mode].value, modes[active_mode].speed, red, grn, blu);
}
