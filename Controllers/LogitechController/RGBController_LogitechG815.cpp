/*-----------------------------------------*\
|  RGBController_LogitechG815.cpp           |
|                                           |
|  Generic RGB Interface for Logitech G815  |
|  RGB Mechanical Gaming Keyboard           |
|                                           |
|  Cheerpipe      3/20/2021                 |
\*-----------------------------------------*/

#include "RGBController_LogitechG815.h"
#include <cstring>
#include <iterator>
#include <map>

#define NA  0xFFFFFFFF
const size_t max_key_per_color  = 13;
const size_t data_size          = 16;

static unsigned int matrix_map[7][27] =
    { { 110, NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  111, NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA  },
      { NA,  NA,  37,  NA,  54,  55,  56,  57,  NA,  58,  59,  60,  61,  NA,  62,  63,  64,  65,  NA,  66,  67,  68,  NA,  106, 107, 108, 109 },
      { 112, NA,  49,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  NA,  41,  42,  38,  NA,  NA,  69,  70,  71,  NA,  79,  80,  81,  82  },
      { 113, NA,  39,  NA,  16,  22,  4,   17,  NA,  19,  24,  20,  8,   14,  15,  43,  44,  45,  NA,  72,  73,  74,  NA,  91,  92,  93,  83  },
      { 114, NA,  53,  NA,  0,   18,  3,   5,   NA,  6,   7,   9,   10,  11,  47,  48,  46,  36,  NA,  NA,  NA,  NA,  NA,  88,  89,  90,  NA  },
      { 115, NA,  99,  NA,  25,  23,  2,   21,  NA,  1,   NA,  13,  12,  50,  51,  52,  103, NA,  NA,  NA,  78,  NA,  NA,  85,  86,  87,  84  },
      { 116, NA,  98,  101, 100, NA,  NA,  NA,  NA,  40,  NA,  NA,  NA,  NA,  104, 105, 97,  102, NA,  76,  77,  75,  NA,  94,  NA,  95,  NA  }  };

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
    117,
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
    { "Key: A",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x04    },
    { "Key: B",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x05    },
    { "Key: C",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x06    },
    { "Key: D",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x07    },
    { "Key: E",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x08    },
    { "Key: F",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x09    },
    { "Key: G",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x0A    },
    { "Key: H",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x0B    },
    { "Key: I",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x0C    },
    { "Key: J",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x0D    },
    { "Key: K",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x0E    },
    { "Key: L",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x0F    },
    { "Key: M",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x10    },
    { "Key: N",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x11    },
    { "Key: O",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x12    },
    { "Key: P",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x13    },
    { "Key: Q",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x14    },
    { "Key: R",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x15    },
    { "Key: S",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x16    },
    { "Key: T",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x17    },
    { "Key: U",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x18    },
    { "Key: V",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x19    },
    { "Key: W",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x1A    },
    { "Key: X",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x1B    },
    { "Key: Y",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x1C    },
    { "Key: Z",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x1D    },
    { "Key: 1",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x1E    },
    { "Key: 2",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x1F    },
    { "Key: 3",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x20    },
    { "Key: 4",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x21    },
    { "Key: 5",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x22    },
    { "Key: 6",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x23    },
    { "Key: 7",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x24    },
    { "Key: 8",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x25    },
    { "Key: 9",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x26    },
    { "Key: 0",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x27    },
    { "Key: Enter",             LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x28    },
    { "Key: Escape",            LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x29    },
    { "Key: Backspace",         LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x2A    },
    { "Key: Tab",               LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x2B    },
    { "Key: Space",             LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x2C    },
    { "Key: -",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x2D    },
    { "Key: =",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x2E    },
    { "Key: [",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x2F    },
    { "Key: ]",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x30    },
    { "Key: \\ (ANSI)",         LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x31    },//ANSI only
    { "Key: #",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x32    },//ISO only
    { "Key: ;",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x33    },
    { "Key: '",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x34    },
    { "Key: `",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x35    },
    { "Key: ,",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x36    },
    { "Key: .",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x37    },
    { "Key: /",                 LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x38    },
    { "Key: Caps Lock",         LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x39    },
    { "Key: F1",                LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x3A    },
    { "Key: F2",                LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x3B    },
    { "Key: F3",                LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x3C    },
    { "Key: F4",                LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x3D    },
    { "Key: F5",                LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x3E    },
    { "Key: F6",                LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x3F    },
    { "Key: F7",                LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x40    },
    { "Key: F8",                LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x41    },
    { "Key: F9",                LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x42    },
    { "Key: F10",               LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x43    },
    { "Key: F11",               LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x44    },
    { "Key: F12",               LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x45    },
    { "Key: Print Screen",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x46    },
    { "Key: Scroll Lock",       LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x47    },
    { "Key: Pause/Break",       LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x48    },
    { "Key: Insert",            LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x49    },
    { "Key: Home",              LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x4A    },
    { "Key: Page Up",           LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x4B    },
    { "Key: Delete",            LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x4C    },
    { "Key: End",               LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x4D    },
    { "Key: Page Down",         LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x4E    },
    { "Key: Right Arrow",       LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x4F    },
    { "Key: Left Arrow",        LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x50    },
    { "Key: Down Arrow",        LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x51    },
    { "Key: Up Arrow",          LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x52    },
    { "Key: Num Lock",          LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x53    },
    { "Key: Number Pad /",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x54    },
    { "Key: Number Pad *",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x55    },
    { "Key: Number Pad -",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x56    },
    { "Key: Number Pad +",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x57    },
    { "Key: Number Pad Enter",  LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x58    },
    { "Key: Number Pad 1",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x59    },
    { "Key: Number Pad 2",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x5A    },
    { "Key: Number Pad 3",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x5B    },
    { "Key: Number Pad 4",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x5C    },
    { "Key: Number Pad 5",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x5D    },
    { "Key: Number Pad 6",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x5E    },
    { "Key: Number Pad 7",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x5F    },
    { "Key: Number Pad 8",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x60    },
    { "Key: Number Pad 9",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x61    },
    { "Key: Number Pad 0",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x62    },
    { "Key: Number Pad .",      LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x63    },
    { "Key: \\ (ISO)",          LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x64    },//ISO only
    { "Key: Menu",              LOGITECH_G815_ZONE_DIRECT_KEYBOARD,     0x65    },
    { "Key: Left Control",      LOGITECH_G815_ZONE_MODE_MODIFIERS,      0xE0    },
    { "Key: Left Shift",        LOGITECH_G815_ZONE_MODE_MODIFIERS,      0xE1    },
    { "Key: Left Alt",          LOGITECH_G815_ZONE_MODE_MODIFIERS,      0xE2    },
    { "Key: Left Windows",      LOGITECH_G815_ZONE_MODE_MODIFIERS,      0xE3    },
    { "Key: Right Control",     LOGITECH_G815_ZONE_MODE_MODIFIERS,      0xE4    },
    { "Key: Right Shift",       LOGITECH_G815_ZONE_MODE_MODIFIERS,      0xE5    },
    { "Key: Right Alt",         LOGITECH_G815_ZONE_MODE_MODIFIERS,      0xE6    },
    { "Key: Right Windows",     LOGITECH_G815_ZONE_MODE_MODIFIERS,      0xE7    },
    { "Key: Media Previous",    LOGITECH_G815_ZONE_DIRECT_MEDIA,        0x9E    },
    { "Key: Media Play/Pause",  LOGITECH_G815_ZONE_DIRECT_MEDIA,        0x9B    },
    { "Key: Media Next",        LOGITECH_G815_ZONE_DIRECT_MEDIA,        0x9D    },
    { "Key: Media Mute",        LOGITECH_G815_ZONE_DIRECT_MEDIA,        0x9C    },
    { "Logo",                   LOGITECH_G815_ZONE_DIRECT_LOGO,         0x01    },
    { "Lighting",               LOGITECH_G815_ZONE_DIRECT_INDICATORS,   0x99    },
    { "Key: G1",                LOGITECH_G815_ZONE_MODE_GKEYS,          0x01    },
    { "Key: G2",                LOGITECH_G815_ZONE_MODE_GKEYS,          0x02    },
    { "Key: G3",                LOGITECH_G815_ZONE_MODE_GKEYS,          0x03    },
    { "Key: G4",                LOGITECH_G815_ZONE_MODE_GKEYS,          0x04    },
    { "Key: G5",                LOGITECH_G815_ZONE_MODE_GKEYS,          0x05    },
};

RGBController_LogitechG815::RGBController_LogitechG815(LogitechG815Controller* logitech_ptr)
{
    logitech    = logitech_ptr;

    name        = "Logitech G815 Keyboard Device";
    vendor      = "Logitech";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Logitech G815 Keyboard Device";
    serial      = logitech->GetSerialString();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = LOGITECH_G815_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = LOGITECH_G815_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = LOGITECH_G815_MODE_OFF;
    Off.flags                       = 0;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Cycle;
    Cycle.name                      = "Cycle";
    Cycle.value                     = LOGITECH_G815_MODE_CYCLE;
    Cycle.flags                     = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode                = MODE_COLORS_NONE;
    Cycle.speed_min                 = LOGITECH_G815_SPEED_SLOWEST;
    Cycle.speed_max                 = LOGITECH_G815_SPEED_FASTEST;
    Cycle.speed                     = LOGITECH_G815_SPEED_NORMAL;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = LOGITECH_G815_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.speed_min             = LOGITECH_G815_SPEED_SLOWEST;
    Breathing.speed_max             = LOGITECH_G815_SPEED_FASTEST;
    Breathing.speed                 = LOGITECH_G815_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
    std::copy(colors.begin(), colors.end(),std::back_inserter(current_colors));
}

RGBController_LogitechG815::~RGBController_LogitechG815()
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
}

void RGBController_LogitechG815::SetupZones()
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
            new_zone.matrix_map->width  = 27;
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

void RGBController_LogitechG815::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechG815::DeviceUpdateLEDs()
{
    std::map<RGBColor, std::vector<char>> ledsByColors;
    std::vector<RGBColor>   new_colors;
    unsigned char zone      = 0;
    unsigned char idx       = 0;
    unsigned char frame_buffer_big_mode[data_size];
    unsigned char frame_buffer_little_mode[data_size];
    RGBColor colorkey;

    /*---------------------------------------------------------*\
    | Freeze colors array because prepare framebuffers          |
    | may take some time.                                       |
    \*---------------------------------------------------------*/
    std::copy(colors.begin(), colors.end(),std::back_inserter(new_colors));

    /*---------------------------------------------------------*\
    | Get unique colors to create mode 1F and 6F frame_buffers  |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        zone = ( leds[led_idx].value >> 8 );
        idx  = ( leds[led_idx].value );

        if (current_colors[led_idx]==new_colors[led_idx])
        {
            /*-------------------------------------------------*\
            | Don't send if key color is not changed            |
            \*-------------------------------------------------*/
            continue;
        }

        switch (zone)
        {
            case LOGITECH_G815_ZONE_MODE_GKEYS:
                idx = ((idx & 0x00ff) + 0xb3);
                break;

            case LOGITECH_G815_ZONE_MODE_MODIFIERS:
                idx = ((idx & 0x00ff) - 0x78);
                break;

            case LOGITECH_G815_ZONE_DIRECT_KEYBOARD:
                idx = ((idx & 0x00ff) - 0x03);
                break;

            case LOGITECH_G815_ZONE_DIRECT_LOGO:
                idx = ((idx & 0x00ff) +  0xd1);
                break;

            default:
                idx = (idx & 0x00ff);
                break;
        }

        colorkey = new_colors[led_idx];

        if (ledsByColors.count(colorkey) == 0)
        {
            ledsByColors.insert(std::pair<RGBColor, std::vector<char>>(colorkey, {}));
        }

        ledsByColors[colorkey].push_back(idx);
    }

    uint8_t led_in_little_frame = 0;
    uint8_t bi                  = 0;
    size_t frame_pos            = 3;
    uint8_t li                  = 0;

    /*---------------------------------------------------------*\
    | Create frame_buffers of type 1F (Little, up to 4 leds     |
    | per packet) and 6F (big, up to 13 leds per packet).       |
    \*---------------------------------------------------------*/
    for(std::pair<const RGBColor, std::vector<char>>& x: ledsByColors)
    {
        /*-----------------------------------------------------*\
        | For colors with more than 4 keys. Better to use big   |
        | (6F) packets to save USB transfers.                   |
        \*-----------------------------------------------------*/
        if(x.second.size() > 4)
        {
            bi = 0;

            while(bi < x.second.size())
            {
                frame_buffer_big_mode[0] = RGBGetRValue(x.first);
                frame_buffer_big_mode[1] = RGBGetGValue(x.first);
                frame_buffer_big_mode[2] = RGBGetBValue(x.first);
                frame_pos                = 3;

                for(uint8_t i = 0; i < max_key_per_color; i++)
                {
                    if(bi + i < x.second.size())
                    {
                        frame_buffer_big_mode[frame_pos] = x.second[bi+i];
                        frame_pos++;
                    }
                }

                if (frame_pos < data_size)
                {
                    /*-----------------------------------------*\
                    | Zeroing just what is needed and if needed |
                    \*-----------------------------------------*/
                    memset(frame_buffer_big_mode + frame_pos, 0x00, sizeof(frame_buffer_big_mode) - frame_pos);

                    /*-----------------------------------------*\
                    | End of Data byte                          |
                    \*-----------------------------------------*/
                    frame_buffer_big_mode[frame_pos] = 0xFF;
                }

                /*-----------------------------------------------------*\
                | Zeroing just what is needed                           |
                \*-----------------------------------------------------*/
                logitech->SetDirect(LOGITECH_G815_ZONE_FRAME_TYPE_BIG, frame_buffer_big_mode);
                bi = bi + max_key_per_color;
            }
        }
        /*-----------------------------------------------------*\
        | For colors with up to 4 keys. Use 1F packet to send   |
        | up to 4 colors-keys combinations per packet.          |
        \*-----------------------------------------------------*/
        else
        {
            li = 0;

            while(li < x.second.size())
            {
                frame_buffer_little_mode[led_in_little_frame*4 + 0] = x.second[li];
                frame_buffer_little_mode[led_in_little_frame*4 + 1] = RGBGetRValue(x.first);
                frame_buffer_little_mode[led_in_little_frame*4 + 2] = RGBGetGValue(x.first);
                frame_buffer_little_mode[led_in_little_frame*4 + 3] = RGBGetBValue(x.first);
                li++;
                led_in_little_frame++;

                if (led_in_little_frame == 4)
                {
                    /*-----------------------------------------*\
                    | No End of Data byte if the packet is full |
                    \*-----------------------------------------*/
                    logitech->SetDirect(LOGITECH_G815_ZONE_FRAME_TYPE_LITTLE, frame_buffer_little_mode);
                    led_in_little_frame=0;
                }
            }
        }
    }

    /*---------------------------------------------------------*\
    | If there is a left 1F packet with less than 4 keys, send  |
    | it and add an End of Data byte.                           |
    \*---------------------------------------------------------*/
    if(led_in_little_frame > 0)
    {
        /*-----------------------------------------------------*\
        | Zeroing just what is needed                           |
        \*-----------------------------------------------------*/
        memset(frame_buffer_little_mode + (led_in_little_frame * 4 + 1), 0x00, sizeof(frame_buffer_little_mode) - led_in_little_frame * 4);

        /*-----------------------------------------------------*\
        | Data byte                                             |
        \*-----------------------------------------------------*/
        frame_buffer_little_mode[led_in_little_frame*4 + 0] = 0xFF;

        /*-----------------------------------------------------*\
        | Send little frame and clear little frame buffer       |
        \*-----------------------------------------------------*/
        logitech->SetDirect(LOGITECH_G815_ZONE_FRAME_TYPE_LITTLE, frame_buffer_little_mode);
    }
    if(ledsByColors.size() > 0)
    {
        /*-----------------------------------------------------*\
        | Copy the current color vector to avoid set keys that  |
        | has not being                                         |
        \*-----------------------------------------------------*/
        logitech->Commit();
        std::copy(new_colors.begin(), new_colors.end(),current_colors.begin());
    }
}

void RGBController_LogitechG815::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG815::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG815::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_LogitechG815::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | Direct mode does not send a mode packet                   |
    | Call UpdateLEDs to send direct packet                     |
    \*---------------------------------------------------------*/
    if(modes[active_mode].value == LOGITECH_G815_MODE_DIRECT)
    {
        /*-----------------------------------------------------*\
        | Send real direct mode initialization. I used same     |
        | sequence as GHUB for screen capture.                  |
        \*-----------------------------------------------------*/
        logitech->InitializeDirect();

        /*-----------------------------------------------------*\
        | Set one key to get direct mode engaged.               |
        \*-----------------------------------------------------*/
        logitech->SendSingleLed(0x29,0,0,0);
        logitech->Commit();
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
