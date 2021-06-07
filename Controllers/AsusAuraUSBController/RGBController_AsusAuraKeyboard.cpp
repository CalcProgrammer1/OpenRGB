/*-----------------------------------------*\
|  RGBController_AsusAuraKeyboard.cpp       |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/19/2020   |
\*-----------------------------------------*/

#include "RGBController_AsusAuraKeyboard.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int flare_matrix_map[6][22] =
    { {   0,  NA,  13,  18,  23,  28,  38,  43,  49,  54,  60,  65,  69,  70,  NA,  76,  80,  85,  NA,  NA,  NA,  NA },
      {   1,   8,  14,  19,  24,  29,  34,  39,  44,  50,  55,  61,  66,  71,  NA,  77,  81,  86,  89,  94,  98, 103 },
      {   2,  NA,   9,  15,  20,  25,  30,  35,  40,  45,  51,  56,  62,  67,  72,  78,  82,  87,  90,  95,  99, 104 },
      {   3,  NA,  10,  16,  21,  26,  31,  36,  41,  46,  52,  57,  63,  68,  73,  NA,  NA,  NA,  91,  96, 100,  NA },
      {   4,   6,  11,  17,  22,  27,  32,  37,  42,  47,  53,  58,  74,  NA,  NA,  NA,  83,  NA,  92,  97, 101, 105 },
      {   5,   7,  12,  NA,  NA,  NA,  NA,  33,  NA,  48,  NA,  59,  64,  75,  NA,  79,  84,  88,  93,  NA, 102,  NA } };

static unsigned int scope_matrix_map[6][22] =
    { {   0,  NA,  13,  18,  23,  28,  38,  43,  49,  54,  60,  65,  69,  70,  NA,  76,  80,  85,  NA,  NA,  NA,  NA },
      {   1,   8,  14,  19,  24,  29,  34,  39,  44,  50,  55,  61,  66,  71,  NA,  77,  81,  86,  89,  94,  98, 103 },
      {   2,  NA,   9,  15,  20,  25,  30,  35,  40,  45,  51,  56,  62,  67,  72,  78,  82,  87,  90,  95,  99, 104 },
      {   3,  NA,  10,  16,  21,  26,  31,  36,  41,  46,  52,  57,  63,  68,  73,  NA,  NA,  NA,  91,  96, 100,  NA },
      {   4,   6,  11,  17,  22,  27,  32,  37,  42,  47,  53,  58,  74,  NA,  NA,  NA,  83,  NA,  92,  97, 101, 105 },
      {   5,   NA,  7,  12,  NA,  NA,  NA,  33,  NA,  48,  NA,  59,  64,  75,  NA,  79,  84,  88,  93,  NA, 102,  NA } };

static unsigned int scope_tkl_matrix_map[6][18] =
    { {   0,  NA,  13,  18,  23,  28,  38,  43,  49,  54,  60,  65,  69,  70,  NA,  NA,  NA,  NA },
      {   1,   8,  14,  19,  24,  29,  34,  39,  44,  50,  55,  61,  66,  71,  NA,  76,  79,  83 },
      {   2,  NA,   9,  15,  20,  25,  30,  35,  40,  45,  51,  56,  62,  67,  72,  77,  80,  84 },
      {   3,  NA,  10,  16,  21,  26,  31,  36,  41,  46,  52,  57,  63,  68,  73,  NA,  NA,  NA },
      {   4,   6,  11,  17,  22,  27,  32,  37,  42,  47,  53,  58,  74,  NA,  NA,  NA,  81,  NA },
      {   5,   NA,  7,  12,  NA,  NA,  NA,  33,  NA,  48,  NA,  59,  64,  75,  NA,  78,  82,  85 } };

static const std::vector<led_type> default_led_names =
{
    /* Key Label                Index  */
    { "Key: Escape",            0x00    },
    { "Key: `",                 0x01    },
    { "Key: Tab",               0x02    },
    { "Key: Caps Lock",         0x03    },
    { "Key: Left Shift",        0x04    },
    { "Key: Left Control",      0x05    },
    { "Key: \\ (ISO)",          0x0C    },
    { "Key: 1",                 0x11    },
    { "Key: Q",                 0x12    },
    { "Key: A",                 0x13    },
    { "Key: Z",                 0x14    },
    { "Key: F1",                0x18    },
    { "Key: 2",                 0x19    },
    { "Key: W",                 0x1A    },
    { "Key: S",                 0x1B    },
    { "Key: X",                 0x1C    },
    { "Key: F2",                0x20    },
    { "Key: 3",                 0x21    },
    { "Key: E",                 0x22    },
    { "Key: D",                 0x23    },
    { "Key: C",                 0x24    },
    { "Key: F3",                0x28    },
    { "Key: 4",                 0x29    },
    { "Key: R",                 0x2A    },
    { "Key: F",                 0x2B    },
    { "Key: V",                 0x2C    },
    { "Key: F4",                0x30    },
    { "Key: 5",                 0x31    },
    { "Key: T",                 0x32    },
    { "Key: G",                 0x33    },
    { "Key: B",                 0x34    },
    { "Key: Space",             0x35    },
    { "Key: 6",                 0x39    },
    { "Key: Y",                 0x3A    },
    { "Key: H",                 0x3B    },
    { "Key: N",                 0x3C    },
    { "Key: F5",                0x40    },
    { "Key: 7",                 0x41    },
    { "Key: U",                 0x42    },
    { "Key: J",                 0x43    },
    { "Key: M",                 0x44    },
    { "Key: F6",                0x48    },
    { "Key: 8",                 0x49    },
    { "Key: I",                 0x4A    },
    { "Key: K",                 0x4B    },
    { "Key: ,",                 0x4C    },
    { "Key: Right Alt",         0x4D    },
    { "Key: F7",                0x50    },
    { "Key: 9",                 0x51    },
    { "Key: O",                 0x52    },
    { "Key: L",                 0x53    },
    { "Key: .",                 0x54    },
    { "Key: F8",                0x58    },
    { "Key: 0",                 0x59    },
    { "Key: P",                 0x5A    },
    { "Key: ;",                 0x5B    },
    { "Key: /",                 0x5C    },
    { "Key: Right Fn",          0x5D    },
    { "Key: F9",                0x60    },
    { "Key: -",                 0x61    },
    { "Key: [",                 0x62    },
    { "Key: '",                 0x63    },
    { "Key: Menu",              0x65    },
    { "Key: F10",               0x68    },
    { "Key: =",                 0x69    },
    { "Key: ]",                 0x6A    },
    { "Key: #",                 0x6B    },
    { "Key: F11",               0x70    },
    { "Key: F12",               0x78    },
    { "Key: Backspace",         0x79    },
    { "Key: \\ (ANSI)",         0x7A    },
    { "Key: Enter",             0x7B    },
    { "Key: Right Shift",       0x7C    },
    { "Key: Right Control",     0x7D    },
    { "Key: Print Screen",      0x80    },
    { "Key: Insert",            0x81    },
    { "Key: Delete",            0x82    },
    { "Key: Left Arrow",        0x85    },
    { "Key: Scroll Lock",       0x88    },
    { "Key: Home",              0x89    },
    { "Key: End",               0x8A    },
    { "Key: Up Arrow",          0x8C    },
    { "Key: Down Arrow",        0x8D    },
    { "Key: Pause/Break",       0x90    },
    { "Key: Page Up",           0x91    },
    { "Key: Page Down",         0x92    },
    { "Key: Right Arrow",       0x95    },
    { "Key: Num Lock",          0x99    },
    { "Key: Number Pad 7",      0x9A    },
    { "Key: Number Pad 4",      0x9B    },
    { "Key: Number Pad 1",      0x9C    },
    { "Key: Number Pad 0",      0x9D    },
    { "Key: Number Pad /",      0xA1    },
    { "Key: Number Pad 8",      0xA2    },
    { "Key: Number Pad 5",      0xA3    },
    { "Key: Number Pad 2",      0xA4    },
    { "Key: Number Pad *",      0xA9    },
    { "Key: Number Pad 9",      0xAA    },
    { "Key: Number Pad 6",      0xAB    },
    { "Key: Number Pad 3",      0xAC    },
    { "Key: Number Pad .",      0xAD    },
    { "Key: Number Pad -",      0xB1    },
    { "Key: Number Pad +",      0xB2    },
    { "Key: Number Pad Enter",  0xB4    },
};

static const std::vector<led_type> default_tkl_led_names =
{
    /* Key Label                Index  */
    { "Key: Escape",            0x00    },
    { "Key: `",                 0x01    },
    { "Key: Tab",               0x02    },
    { "Key: Caps Lock",         0x03    },
    { "Key: Left Shift",        0x04    },
    { "Key: Left Control",      0x05    },
    { "Key: \\ (ISO)",          0x0C    },
    { "Key: 1",                 0x11    },
    { "Key: Q",                 0x12    },
    { "Key: A",                 0x13    },
    { "Key: Z",                 0x14    },
    { "Key: F1",                0x18    },
    { "Key: 2",                 0x19    },
    { "Key: W",                 0x1A    },
    { "Key: S",                 0x1B    },
    { "Key: X",                 0x1C    },
    { "Key: F2",                0x20    },
    { "Key: 3",                 0x21    },
    { "Key: E",                 0x22    },
    { "Key: D",                 0x23    },
    { "Key: C",                 0x24    },
    { "Key: F3",                0x28    },
    { "Key: 4",                 0x29    },
    { "Key: R",                 0x2A    },
    { "Key: F",                 0x2B    },
    { "Key: V",                 0x2C    },
    { "Key: F4",                0x30    },
    { "Key: 5",                 0x31    },
    { "Key: T",                 0x32    },
    { "Key: G",                 0x33    },
    { "Key: B",                 0x34    },
    { "Key: Space",             0x35    },
    { "Key: 6",                 0x39    },
    { "Key: Y",                 0x3A    },
    { "Key: H",                 0x3B    },
    { "Key: N",                 0x3C    },
    { "Key: F5",                0x40    },
    { "Key: 7",                 0x41    },
    { "Key: U",                 0x42    },
    { "Key: J",                 0x43    },
    { "Key: M",                 0x44    },
    { "Key: F6",                0x48    },
    { "Key: 8",                 0x49    },
    { "Key: I",                 0x4A    },
    { "Key: K",                 0x4B    },
    { "Key: ,",                 0x4C    },
    { "Key: Right Alt",         0x4D    },
    { "Key: F7",                0x50    },
    { "Key: 9",                 0x51    },
    { "Key: O",                 0x52    },
    { "Key: L",                 0x53    },
    { "Key: .",                 0x54    },
    { "Key: F8",                0x58    },
    { "Key: 0",                 0x59    },
    { "Key: P",                 0x5A    },
    { "Key: ;",                 0x5B    },
    { "Key: /",                 0x5C    },
    { "Key: Right Fn",          0x5D    },
    { "Key: F9",                0x60    },
    { "Key: -",                 0x61    },
    { "Key: [",                 0x62    },
    { "Key: '",                 0x63    },
    { "Key: Menu",              0x65    },
    { "Key: F10",               0x68    },
    { "Key: =",                 0x69    },
    { "Key: ]",                 0x6A    },
    { "Key: #",                 0x6B    },
    { "Key: F11",               0x70    },
    { "Key: F12",               0x78    },
    { "Key: Backspace",         0x79    },
    { "Key: \\ (ANSI)",         0x7A    },
    { "Key: Enter",             0x7B    },
    { "Key: Right Shift",       0x7C    },
    { "Key: Right Control",     0x7D    },
    { "Key: Insert",            0x81    },
    { "Key: Delete",            0x82    },
    { "Key: Left Arrow",        0x85    },
    { "Key: Home",              0x89    },
    { "Key: End",               0x8A    },
    { "Key: Up Arrow",          0x8C    },
    { "Key: Down Arrow",        0x8D    },
    { "Key: Page Up",           0x91    },
    { "Key: Page Down",         0x92    },
    { "Key: Right Arrow",       0x95    },
    { "Logo 1",                 0x80    },
    { "Logo 2",                 0x90    },
    { "Underglow 1",            0x06    },
    { "Underglow 2",            0x0E    },
    { "Underglow 3",            0x16    },
    { "Underglow 4",            0x1E    },
    { "Underglow 5",            0x26    },
    { "Underglow 6",            0x2E    },
    { "Underglow 7",            0x36    },
    { "Underglow 8",            0x3E    },
    { "Underglow 9",            0x46    },
    { "Underglow 10",           0x4E    },
    { "Underglow 11",           0x56    },
    { "Underglow 12",           0x5E    },
    { "Underglow 13",           0x66    },
    { "Underglow 14",           0x6E    },
    { "Underglow 15",           0x76    },
    { "Underglow 16",           0x7E    },
    { "Underglow 17",           0x86    },
    { "Underglow 18",           0x8E    },
    { "Underglow 19",           0x96    },
    { "Underglow 20",           0x9E    },
    { "Underglow 21",           0xA6    },
    { "Underglow 22",           0xAE    },
    { "Underglow 23",           0xB6    },
    { "Underglow 24",           0xBE    },
    { "Underglow 25",           0xC6    },
    { "Underglow 26",           0xCE    },

};
RGBController_AuraKeyboard::RGBController_AuraKeyboard(AuraKeyboardController* aura_ptr, AuraKeyboardMappingLayoutType keyboard_layout)
{
    aura        = aura_ptr;
    layout      = keyboard_layout;

    name        = "ASUS Aura Keyboard";
    vendor      = "ASUS";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "ASUS Aura Keyboard Device";
    location    = aura->GetDeviceLocation();
    serial      = aura->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_AuraKeyboard::~RGBController_AuraKeyboard()
{
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(zones[zone_idx].type == ZONE_TYPE_MATRIX)
        {
            delete zones[zone_idx].matrix_map;
        }
    }
    delete aura;
}

void RGBController_AuraKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    std::vector<led_zone> led_zones;
    std::vector<led_type> led_names;

    switch(layout)
    {
        /*-----------------------------------------------------*\
        | On the ROG Scope keyboards Ctrl key is double sized,  |
        | so there is a layout shift                            |
        \*-----------------------------------------------------*/
        case SCOPE_LAYOUT:
            led_names = default_led_names;
            led_zones.push_back({"Keyboard",    ZONE_TYPE_MATRIX, 106, new matrix_map_type{6, 22, (unsigned int *)&scope_matrix_map}});

            led_names.insert(led_names.begin() + 7,     {"Key: Left Windows", 0x15});
            led_names.insert(led_names.begin() + 12,    {"Key: Left Alt", 0x1D});
            break;

        case SCOPE_RX_LAYOUT:
            led_names = default_led_names;
            led_zones.push_back({"Keyboard",    ZONE_TYPE_MATRIX, 106, new matrix_map_type{6, 22, (unsigned int *)&scope_matrix_map}});
            led_zones.push_back({"Logo",        ZONE_TYPE_SINGLE, 1});

            led_names.insert(led_names.begin() + 7,     {"Key: Left Windows", 0x15});
            led_names.insert(led_names.begin() + 12,    {"Key: Left Alt", 0x1D});
            led_names.push_back({ "Logo", 0xB0});
            break;

        case SCOPE_TKL_LAYOUT:
            led_names = default_tkl_led_names;
            led_zones.push_back({"Keyboard",    ZONE_TYPE_MATRIX, 86, new matrix_map_type{6, 18, (unsigned int *)&scope_tkl_matrix_map}});
            led_zones.push_back({"Logo",        ZONE_TYPE_LINEAR, 2});
            led_zones.push_back({"Underglow",   ZONE_TYPE_LINEAR, 26});

            led_names.insert(led_names.begin() + 7,     {"Key: Left Windows", 0x15});
            led_names.insert(led_names.begin() + 12,    {"Key: Left Alt", 0x1D});
            break;

        case FLARE_LAYOUT:
            led_names = default_led_names;
            led_zones.push_back({"Keyboard",    ZONE_TYPE_MATRIX, 106, new matrix_map_type{6, 22, (unsigned int *)&flare_matrix_map}});
            led_zones.push_back({"Logo",        ZONE_TYPE_SINGLE, 1});
            led_zones.push_back({"Underglow",   ZONE_TYPE_SINGLE, 2});

            led_names.insert(led_names.begin() + 7,     {"Key: Left Windows", 0x0D});
            led_names.insert(led_names.begin() + 12,    {"Key: Left Alt", 0x15});

            led_names.push_back({ "Logo", 0xB8});
            led_names.push_back({ "Left Underglow",     0xB9});
            led_names.push_back({ "Right Underglow",    0xBA});
            break;
    }

    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < led_zones.size(); zone_idx++)
    {
        zone new_zone;
        new_zone.name                   = led_zones[zone_idx].name;
        new_zone.type                   = led_zones[zone_idx].type;
        new_zone.leds_min               = led_zones[zone_idx].size;
        new_zone.leds_max               = led_zones[zone_idx].size;
        new_zone.leds_count             = led_zones[zone_idx].size;

        if(led_zones[zone_idx].type == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map         = led_zones[zone_idx].matrix;
        }
        else
        {
            new_zone.matrix_map         = NULL;
        }

        zones.push_back(new_zone);

        total_led_count += led_zones[zone_idx].size;
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name  = led_names[led_idx].name;
        new_led.value = led_names[led_idx].idx;

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_AuraKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AuraKeyboard::DeviceUpdateLEDs()
{
    std::vector<unsigned char> frame_buf;

    /*---------------------------------------------------------*\
    | Resize the frame buffer, 4 bytes per LED                  |
    \*---------------------------------------------------------*/
    frame_buf.resize(leds.size() * 4);

    /*---------------------------------------------------------*\
    | TODO: Send packets with multiple LED frames               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        frame_buf[(led_idx * 4) + 0] = leds[led_idx].value;
        frame_buf[(led_idx * 4) + 1] = RGBGetRValue(colors[led_idx]);
        frame_buf[(led_idx * 4) + 2] = RGBGetGValue(colors[led_idx]);
        frame_buf[(led_idx * 4) + 3] = RGBGetBValue(colors[led_idx]);
    }

    aura->SendDirect(leds.size(), frame_buf.data());
}

void RGBController_AuraKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraKeyboard::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_AuraKeyboard::DeviceUpdateMode()
{

}
