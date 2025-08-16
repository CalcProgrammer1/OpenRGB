/*------------------------------------------*\
|  SinowealthKeyboard90Controller.h          |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard with PID:0090,                   |
|  made spefically for Genesis Thor 300      |
|                                            |
|  Jan Baier 30/06/2022                      |
\*-----------------------------------------=*/

#include "RGBController.h"
#include "RGBControllerKeyNames.h"
#include <vector>
#include <hidapi.h>

#pragma once

#define NA 0xFFFFFF

namespace thor300
{
    static const unsigned int matrix_map[6][23] =
    { {  0,   NA,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  NA,  13,  14,  15,  NA,  NA,  NA,  NA,  NA },
      {  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  NA,  30,  31,  32,  NA,  33,  34,  35,  36 },
      {  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  NA,  NA,  50,  51,  52,  NA,  53,  54,  55,  NA },
      {  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  NA,  NA,  NA,  NA,  NA,  70,  71,  72,  73 },
      {  74,  NA,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  NA,  85,  NA,  NA,  86,  NA,  NA,  87,  88,  89,  NA },
      {  90,  91,  92,  NA,  NA,  NA,  93,  NA,  NA,  NA,  94,  95,  96,  97,  NA,  98,  99, 100,  NA, 101,  NA, 102, 103 }
    };

    typedef struct
    {
        const char *        name;
        const unsigned char idx;
    } led_type;

    static const led_type led_names[] =
    {
        /* Key Label                Index  */
        { KEY_EN_ESCAPE,            0x01    },
        { KEY_EN_F1,                0x03    },
        { KEY_EN_F2,                0x04    },
        { KEY_EN_F3,                0x05    },
        { KEY_EN_F4,                0x06    },
        { KEY_EN_F5,                0x07    },
        { KEY_EN_F6,                0x08    },
        { KEY_EN_F7,                0x09    },
        { KEY_EN_F8,                0x0A    },
        { KEY_EN_F9,                0x0B    },
        { KEY_EN_F10,               0x0C    },
        { KEY_EN_F11,               0x0D    },
        { KEY_EN_F12,               0x0F    },
        { KEY_EN_PRINT_SCREEN,      0x10    },
        { KEY_EN_SCROLL_LOCK,       0x11    },
        { KEY_EN_PAUSE_BREAK,       0x12    },
        { KEY_EN_BACK_TICK,         0x18    },
        { KEY_EN_1,                 0x19    },
        { KEY_EN_2,                 0x1A    },
        { KEY_EN_3,                 0x1B    },
        { KEY_EN_4,                 0x1C    },
        { KEY_EN_5,                 0x1D    },
        { KEY_EN_6,                 0x1E    },
        { KEY_EN_7,                 0x1F    },
        { KEY_EN_8,                 0x20    },
        { KEY_EN_9,                 0x21    },
        { KEY_EN_0,                 0x22    },
        { KEY_EN_MINUS,             0x23    },
        { KEY_EN_EQUALS,            0x24    },
        { KEY_EN_BACKSPACE,         0x26    },
        { KEY_EN_INSERT,            0x27    },
        { KEY_EN_HOME,              0x28    },
        { KEY_EN_PAGE_UP,           0x29    },
        { KEY_EN_NUMPAD_LOCK,       0x2A    },
        { KEY_EN_NUMPAD_DIVIDE,     0x2B    },
        { KEY_EN_NUMPAD_TIMES,      0x2C    },
        { KEY_EN_NUMPAD_MINUS,      0x2D    },
        { KEY_EN_TAB,               0x2F    },
        { KEY_EN_Q,                 0x30    },
        { KEY_EN_W,                 0x31    },
        { KEY_EN_E,                 0x32    },
        { KEY_EN_R,                 0x33    },
        { KEY_EN_T,                 0x34    },
        { KEY_EN_Y,                 0x35    },
        { KEY_EN_U,                 0x36    },
        { KEY_EN_I,                 0x37    },
        { KEY_EN_O,                 0x38    },
        { KEY_EN_P,                 0x39    },
        { KEY_EN_LEFT_BRACKET,      0x3A    },
        { KEY_EN_RIGHT_BRACKET,     0x3B    },
        { KEY_EN_DELETE,            0x3E    },
        { KEY_EN_END,               0x3F    },
        { KEY_EN_PAGE_DOWN,         0x40    },
        { KEY_EN_NUMPAD_7,          0x41    },
        { KEY_EN_NUMPAD_8,          0x42    },
        { KEY_EN_NUMPAD_9,          0x43    },
        { KEY_EN_CAPS_LOCK,         0x46    },
        { KEY_EN_A,                 0x47    },
        { KEY_EN_S,                 0x48    },
        { KEY_EN_D,                 0x49    },
        { KEY_EN_F,                 0x4A    },
        { KEY_EN_G,                 0x4B    },
        { KEY_EN_H,                 0x4C    },
        { KEY_EN_J,                 0x4D    },
        { KEY_EN_K,                 0x4E    },
        { KEY_EN_L,                 0x4F    },
        { KEY_EN_SEMICOLON,         0x50    },
        { KEY_EN_QUOTE,             0x51    },
        { KEY_EN_ANSI_BACK_SLASH,   0x52    },
        { KEY_EN_ANSI_ENTER,        0x54    },
        { KEY_EN_NUMPAD_4,          0x58    },
        { KEY_EN_NUMPAD_5,          0x59    },
        { KEY_EN_NUMPAD_6,          0x5A    },
        { KEY_EN_NUMPAD_PLUS,       0x44    },
        { KEY_EN_LEFT_SHIFT,        0x5D    },
        { KEY_EN_Z,                 0x5F    },
        { KEY_EN_X,                 0x60    },
        { KEY_EN_C,                 0x61    },
        { KEY_EN_V,                 0x62    },
        { KEY_EN_B,                 0x63    },
        { KEY_EN_N,                 0x64    },
        { KEY_EN_M,                 0x65    },
        { KEY_EN_COMMA,             0x66    },
        { KEY_EN_PERIOD,            0x67    },
        { KEY_EN_FORWARD_SLASH,     0x68    },
        { KEY_EN_RIGHT_SHIFT,       0x6B    },
        { KEY_EN_UP_ARROW,          0x6D    },
        { KEY_EN_NUMPAD_1,          0x6F    },
        { KEY_EN_NUMPAD_2,          0x70    },
        { KEY_EN_NUMPAD_3,          0x71    },
        { KEY_EN_LEFT_CONTROL,      0x74    },
        { KEY_EN_LEFT_WINDOWS,      0x75    },
        { KEY_EN_LEFT_ALT,          0x76    },
        { KEY_EN_SPACE,             0x79    },
        { KEY_EN_RIGHT_ALT,         0x7C    },
        { KEY_EN_RIGHT_FUNCTION,    0x7D    },
        { KEY_EN_MENU,              0x7E    },
        { KEY_EN_RIGHT_CONTROL,     0x80    },
        { KEY_EN_LEFT_ARROW,        0x83    },
        { KEY_EN_DOWN_ARROW,        0x84    },
        { KEY_EN_RIGHT_ARROW,       0x85    },
        { KEY_EN_NUMPAD_0,          0x86    },
        { KEY_EN_NUMPAD_PERIOD,     0x88    },
        { KEY_EN_NUMPAD_ENTER,      0x72    }
    };

    enum
    {
        SPEED_SLOWEST               = 0x00,
        SPEED_SLOW                  = 0x01,
        SPEED_NORMAL                = 0x02,
        SPEED_FAST                  = 0x03,
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
        COLOR_RED                   = 0x00,
        COLOR_GREEN                 = 0x01,
        COLOR_BLUE                  = 0x02,
        COLOR_YELLOW                = 0x03,
        COLOR_VIOLET                = 0x04,
        COLOR_CYAN                  = 0x05,
        COLOR_WHITE                 = 0x06,
        COLOR_RAINBOW               = 0x07,
    };

    enum
    {
        MODE_PRISMO                 = 0x00,
        MODE_BREATHING              = 0x01,
        MODE_WAVE_1                 = 0x02,
        MODE_FLOWERS_BLOSSOM        = 0x03,
        MODE_RAINBOW                = 0x04,
        MODE_WAVE_2                 = 0x05,
        MODE_CW_ROTATION            = 0x06,
        MODE_WAVE_3                 = 0x07,
        MODE_RESPONSE               = 0x08,
        MODE_CCW_ROTATION           = 0x09,
        MODE_SNAKE                  = 0x0A,
        MODE_WAVE_4                 = 0x0B,
        MODE_TORNADO                = 0x0C,
        MODE_NEON                   = 0x0D,
        MODE_TWINKLING              = 0x0E,
        MODE_RESPONSE_SINGLE        = 0x0F,
        MODE_STATIC                 = 0x10,
        MODE_RAINDROPS              = 0x11,
        MODE_WAVE_5                 = 0x12,
        MODE_CUSTOM                 = 0x13,
    };
}

class SinowealthKeyboard90Controller
{
public:
    SinowealthKeyboard90Controller(hid_device* dev_handle, const char* path, const unsigned short pid, std::string dev_name);
    ~SinowealthKeyboard90Controller();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    unsigned short  GetUSBPID();

    void            SendMode
                        (
                        unsigned char mode          = thor300::MODE_CUSTOM,
                        unsigned char brightness    = thor300::BRIGHTNESS_HALF,
                        unsigned char speed         = thor300::SPEED_NORMAL,
                        unsigned char color         = thor300::COLOR_RAINBOW
                        );

    void            SendSingleLED
                        (
                        unsigned char key,
                        unsigned char red           = 0x00,
                        unsigned char green         = 0x00,
                        unsigned char blue          = 0x00
                        );

    void            SendCommit();

private:
    hid_device*     dev;
    std::string     name;
    std::string     location;
    unsigned short  usb_pid;

    void            SendFeatureReport
                        (
                        unsigned char cmd,
                        unsigned char arg1          = 0x00,
                        unsigned char arg2          = 0x00,
                        unsigned char arg3          = 0x00,
                        unsigned char arg4          = 0x00,
                        unsigned char arg5          = 0x00
                        );
};
