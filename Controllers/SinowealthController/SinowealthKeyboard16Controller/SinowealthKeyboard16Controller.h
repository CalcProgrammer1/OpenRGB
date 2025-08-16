/*------------------------------------------*\
|  SinowealthKeyboard16Controller.h          |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard with PID:0016,                   |
|  Hopefully generic for this PID,           |
|  this was made spefically for ZUOYA X51    |
|                                            |
|  Zagorodnikov Aleksey (glooom) 26.07.2021  |
|  based on initial implementation from      |
|  Dmitri Kalinichenko (Dima-Kal) 23/06/2021 |
\*-----------------------------------------=*/

#include "RGBController.h"
#include <vector>
#include <hidapi.h>

#pragma once

#define PAYLOAD_LEN             1032
#define COLOR_PRESETS_IN_MODE   7

namespace kbd16
{
    enum
    {
        MODE_OFF                    = 0,
    /*
        MODE_COLOR_LOOP             = 1,
        MODE_RESPIRE                = 2,
        MODE_STATIC                 = 3,
        MODE_RIPPLES_SHINING        = 4,
        MODE_REACTION               = 5,
        MODE_FLASH_AWAY             = 6,
        MODE_SINE_WAVE              = 7,
        MODE_RAINDROPS              = 8,
        MODE_NEON_STREAM            = 9,
        MODE_RAINBOW_WHEEL          = 10,
        MODE_ADORN                  = 11,
        MODE_STARS_TWINKLE          = 12,
        MODE_SHADOW_DISAPPEAR       = 13,
        MODE_RETRO_SNAKE            = 14,
    */
        MODE_PER_KEY                = 0x0F, // General mode for custom presets
        MODE_PER_KEY1               = 0xF0,
        MODE_PER_KEY2               = 0xF1,
        MODE_PER_KEY3               = 0xF2,
        MODE_PER_KEY4               = 0xF3,
        MODE_PER_KEY5               = 0xF4,
    };

    enum
    {
        SPEED_SLOW                  = 0x00,
        SPEED_NORMAL                = 0x01,
        SPEED_FAST                  = 0x02,
        SPEED_FASTER                = 0x03,
        SPEED_FASTEST               = 0x04,
    };

    enum
    {
        BRIGHTNESS_OFF              = 0x00,
        BRIGHTNESS_QUARTER          = 0x01,
        BRIGHTNESS_HALF             = 0x02,
        BRIGHTNESS_THREE_QUARTERS   = 0x03,
        BRIGHTNESS_FULL             = 0x04,
    };

    enum
    {
        COLOR_PRESET_0              = 0,
        COLOR_PRESET_1              = 1,
        COLOR_PRESET_2              = 2,
        COLOR_PRESET_3              = 3,
        COLOR_PRESET_4              = 4,
        COLOR_PRESET_5              = 5,
        COLOR_PRESET_6              = 6,
        COLOR_PRESET_RANDOM         = 7,
    };
}

struct ModeCfg
{
    unsigned char color:3;
    unsigned char :4;
    unsigned char direction_left:1;
    unsigned char brightness:4;
    unsigned char speed:4;
};

struct ColorCfg
{
    unsigned char blue:8;
    unsigned char green:8;
    unsigned char red:8;
};

struct ModeColorCfg
{
    ColorCfg preset[COLOR_PRESETS_IN_MODE];
};

class SinowealthKeyboard16Controller
{
public:
    SinowealthKeyboard16Controller(hid_device* cmd_handle, hid_device* data_handle, char *_path, std::string dev_name);
    ~SinowealthKeyboard16Controller();

    unsigned int                GetLEDCount();
    std::string                 GetLocation();
    std::string                 GetName();
    std::string                 GetSerialString();
    unsigned char               GetCurrentMode();
    std::vector<ModeCfg>        GetDeviceModes();
    std::vector<ModeColorCfg>   GetDeviceColors();
    std::vector<RGBColor>       GetPerLedColors();

    void                        SetLEDColor(RGBColor* color_buf);
    void                        SetMode(unsigned char mode, unsigned char brightness, unsigned char speed, RGBColor* color_buf);
    void                        ClearMode();
    void                        SetMode(unsigned char mode, unsigned char brightness, unsigned char speed, bool direction_left, unsigned char color_mode);
    void                        SetColorsForMode(unsigned char mode, RGBColor profiles[COLOR_PRESETS_IN_MODE]);
    void                        GetProfile();
    void                        ReadFirmwareInfo();
    void                        SetLEDsDirect(std::vector<RGBColor> colors);

    const int per_key_mode_idx      = 20;
    const int current_mode_idx      = 21;
    const int profiles_start_idx    = 32;
    const int profiles_count        = 15;
    const int colors_start_idx      = 8;

private:
    hid_device*             dev_cmd;
    hid_device*             dev_data;
    device_type             type;
    std::string             name;

    unsigned int            led_count;

    unsigned char           current_mode;
    struct ModeCfg*         device_modes;
    struct ModeColorCfg*    modes_colors;

    std::string             location;

    unsigned char           mode_config_buf[PAYLOAD_LEN];
    unsigned char           colors_config_buf[PAYLOAD_LEN];
    unsigned char           per_button_color_buf[PAYLOAD_LEN];
    bool                    read_config_error = false;
    unsigned char           current_custom_preset = 0;

    void GetModesConfig(unsigned char *buf);
    void GetColorsConfig(unsigned char *buf);
    void GetButtonColorsConfig(unsigned char *buf);

    void initCommunication();
    bool getConfig(unsigned char reqest[], unsigned char *buf);
    bool sendConfig(unsigned char *buf);

    void UpdateConfigurationFromDevice();
};
