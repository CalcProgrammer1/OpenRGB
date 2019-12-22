/*-----------------------------------------*\
|  HyperXController.h                       |
|                                           |
|  Definitions and types for HyperX Predator|
|  RGB RAM lighting controller              |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/29/2019   |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char	hyperx_dev_id;
typedef unsigned short	hyperx_register;

enum
{
    HYPERX_REG_SLOT0_LED0_RED           = 0x11,     /* R color register for LED 0, Slot 0   */
    HYPERX_REG_SLOT0_LED0_GREEN         = 0x12,     /* G color register for LED 0, Slot 0   */
    HYPERX_REG_SLOT0_LED0_BLUE          = 0x13,     /* B color register for LED 0, Slot 0   */
    HYPERX_REG_SLOT0_LED1_RED           = 0x14,     /* R color register for LED 1, Slot 0   */
    HYPERX_REG_SLOT0_LED1_GREEN         = 0x15,     /* G color register for LED 1, Slot 0   */
    HYPERX_REG_SLOT0_LED1_BLUE          = 0x16,     /* B color register for LED 1, Slot 0   */
    HYPERX_REG_SLOT0_LED2_RED           = 0x17,     /* R color register for LED 2, Slot 0   */
    HYPERX_REG_SLOT0_LED2_GREEN         = 0x18,     /* G color register for LED 2, Slot 0   */
    HYPERX_REG_SLOT0_LED2_BLUE          = 0x19,     /* B color register for LED 2, Slot 0   */
    HYPERX_REG_SLOT0_LED3_RED           = 0x1A,     /* R color register for LED 3, Slot 0   */
    HYPERX_REG_SLOT0_LED3_GREEN         = 0x1B,     /* G color register for LED 3, Slot 0   */
    HYPERX_REG_SLOT0_LED3_BLUE          = 0x1C,     /* B color register for LED 3, Slot 0   */
    HYPERX_REG_SLOT0_LED4_RED           = 0x1D,     /* R color register for LED 4, Slot 0   */
    HYPERX_REG_SLOT0_LED4_GREEN         = 0x1E,     /* G color register for LED 4, Slot 0   */
    HYPERX_REG_SLOT0_LED4_BLUE          = 0x1F,     /* B color register for LED 4, Slot 0   */
    HYPERX_REG_SLOT0_LED0_BRIGHTNESS    = 0x21,     /* Brightness for LED 0, Slot 0 (0-100) */
    HYPERX_REG_SLOT0_LED1_BRIGHTNESS    = 0x24,     /* Brightness for LED 1, Slot 0 (0-100) */
    HYPERX_REG_SLOT0_LED2_BRIGHTNESS    = 0x27,     /* Brightness for LED 2, Slot 0 (0-100) */
    HYPERX_REG_SLOT0_LED3_BRIGHTNESS    = 0x2A,     /* Brightness for LED 3, Slot 0 (0-100) */
    HYPERX_REG_SLOT0_LED4_BRIGHTNESS    = 0x2D,     /* Brightness for LED 4, Slot 0 (0-100) */

    HYPERX_REG_SLOT1_LED0_RED           = 0x31,     /* R color register for LED 0, Slot 1   */
    HYPERX_REG_SLOT1_LED0_GREEN         = 0x32,     /* G color register for LED 0, Slot 1   */
    HYPERX_REG_SLOT1_LED0_BLUE          = 0x33,     /* B color register for LED 0, Slot 1   */
    HYPERX_REG_SLOT1_LED1_RED           = 0x34,     /* R color register for LED 1, Slot 1   */
    HYPERX_REG_SLOT1_LED1_GREEN         = 0x35,     /* G color register for LED 1, Slot 1   */
    HYPERX_REG_SLOT1_LED1_BLUE          = 0x36,     /* B color register for LED 1, Slot 1   */
    HYPERX_REG_SLOT1_LED2_RED           = 0x37,     /* R color register for LED 2, Slot 1   */
    HYPERX_REG_SLOT1_LED2_GREEN         = 0x38,     /* G color register for LED 2, Slot 1   */
    HYPERX_REG_SLOT1_LED2_BLUE          = 0x39,     /* B color register for LED 2, Slot 1   */
    HYPERX_REG_SLOT1_LED3_RED           = 0x3A,     /* R color register for LED 3, Slot 1   */
    HYPERX_REG_SLOT1_LED3_GREEN         = 0x3B,     /* G color register for LED 3, Slot 1   */
    HYPERX_REG_SLOT1_LED3_BLUE          = 0x3C,     /* B color register for LED 3, Slot 1   */
    HYPERX_REG_SLOT1_LED4_RED           = 0x3D,     /* R color register for LED 4, Slot 1   */
    HYPERX_REG_SLOT1_LED4_GREEN         = 0x3E,     /* G color register for LED 4, Slot 1   */
    HYPERX_REG_SLOT1_LED4_BLUE          = 0x3F,     /* B color register for LED 4, Slot 1   */
    HYPERX_REG_SLOT1_LED0_BRIGHTNESS    = 0x41,     /* Brightness for LED 0, Slot 1 (0-100) */
    HYPERX_REG_SLOT1_LED1_BRIGHTNESS    = 0x44,     /* Brightness for LED 1, Slot 1 (0-100) */
    HYPERX_REG_SLOT1_LED2_BRIGHTNESS    = 0x47,     /* Brightness for LED 2, Slot 1 (0-100) */
    HYPERX_REG_SLOT1_LED3_BRIGHTNESS    = 0x4A,     /* Brightness for LED 3, Slot 1 (0-100) */
    HYPERX_REG_SLOT1_LED4_BRIGHTNESS    = 0x4D,     /* Brightness for LED 4, Slot 1 (0-100) */

    HYPERX_REG_SLOT2_LED0_RED           = 0x51,     /* R color register for LED 0, Slot 2   */
    HYPERX_REG_SLOT2_LED0_GREEN         = 0x52,     /* G color register for LED 0, Slot 2   */
    HYPERX_REG_SLOT2_LED0_BLUE          = 0x53,     /* B color register for LED 0, Slot 2   */
    HYPERX_REG_SLOT2_LED1_RED           = 0x54,     /* R color register for LED 1, Slot 2   */
    HYPERX_REG_SLOT2_LED1_GREEN         = 0x55,     /* G color register for LED 1, Slot 2   */
    HYPERX_REG_SLOT2_LED1_BLUE          = 0x56,     /* B color register for LED 1, Slot 2   */
    HYPERX_REG_SLOT2_LED2_RED           = 0x57,     /* R color register for LED 2, Slot 2   */
    HYPERX_REG_SLOT2_LED2_GREEN         = 0x58,     /* G color register for LED 2, Slot 2   */
    HYPERX_REG_SLOT2_LED2_BLUE          = 0x59,     /* B color register for LED 2, Slot 2   */
    HYPERX_REG_SLOT2_LED3_RED           = 0x5A,     /* R color register for LED 3, Slot 2   */
    HYPERX_REG_SLOT2_LED3_GREEN         = 0x5B,     /* G color register for LED 3, Slot 2   */
    HYPERX_REG_SLOT2_LED3_BLUE          = 0x5C,     /* B color register for LED 3, Slot 2   */
    HYPERX_REG_SLOT2_LED4_RED           = 0x5D,     /* R color register for LED 4, Slot 2   */
    HYPERX_REG_SLOT2_LED4_GREEN         = 0x5E,     /* G color register for LED 4, Slot 2   */
    HYPERX_REG_SLOT2_LED4_BLUE          = 0x5F,     /* B color register for LED 4, Slot 2   */
    HYPERX_REG_SLOT2_LED0_BRIGHTNESS    = 0x61,     /* Brightness for LED 0, Slot 2 (0-100) */
    HYPERX_REG_SLOT2_LED1_BRIGHTNESS    = 0x64,     /* Brightness for LED 1, Slot 2 (0-100) */
    HYPERX_REG_SLOT2_LED2_BRIGHTNESS    = 0x67,     /* Brightness for LED 2, Slot 2 (0-100) */
    HYPERX_REG_SLOT2_LED3_BRIGHTNESS    = 0x6A,     /* Brightness for LED 3, Slot 2 (0-100) */
    HYPERX_REG_SLOT2_LED4_BRIGHTNESS    = 0x6D,     /* Brightness for LED 4, Slot 2 (0-100) */

    HYPERX_REG_SLOT3_LED0_RED           = 0x71,     /* R color register for LED 0, Slot 3   */
    HYPERX_REG_SLOT3_LED0_GREEN         = 0x72,     /* G color register for LED 0, Slot 3   */
    HYPERX_REG_SLOT3_LED0_BLUE          = 0x73,     /* B color register for LED 0, Slot 3   */
    HYPERX_REG_SLOT3_LED1_RED           = 0x74,     /* R color register for LED 1, Slot 3   */
    HYPERX_REG_SLOT3_LED1_GREEN         = 0x75,     /* G color register for LED 1, Slot 3   */
    HYPERX_REG_SLOT3_LED1_BLUE          = 0x76,     /* B color register for LED 1, Slot 3   */
    HYPERX_REG_SLOT3_LED2_RED           = 0x77,     /* R color register for LED 2, Slot 3   */
    HYPERX_REG_SLOT3_LED2_GREEN         = 0x78,     /* G color register for LED 2, Slot 3   */
    HYPERX_REG_SLOT3_LED2_BLUE          = 0x79,     /* B color register for LED 2, Slot 3   */
    HYPERX_REG_SLOT3_LED3_RED           = 0x7A,     /* R color register for LED 3, Slot 3   */
    HYPERX_REG_SLOT3_LED3_GREEN         = 0x7B,     /* G color register for LED 3, Slot 3   */
    HYPERX_REG_SLOT3_LED3_BLUE          = 0x7C,     /* B color register for LED 3, Slot 3   */
    HYPERX_REG_SLOT3_LED4_RED           = 0x7D,     /* R color register for LED 4, Slot 3   */
    HYPERX_REG_SLOT3_LED4_GREEN         = 0x7E,     /* G color register for LED 4, Slot 3   */
    HYPERX_REG_SLOT3_LED4_BLUE          = 0x7F,     /* B color register for LED 4, Slot 3   */
    HYPERX_REG_SLOT3_LED0_BRIGHTNESS    = 0x81,     /* Brightness for LED 0, Slot 3 (0-100) */
    HYPERX_REG_SLOT3_LED1_BRIGHTNESS    = 0x84,     /* Brightness for LED 1, Slot 3 (0-100) */
    HYPERX_REG_SLOT3_LED2_BRIGHTNESS    = 0x87,     /* Brightness for LED 2, Slot 3 (0-100) */
    HYPERX_REG_SLOT3_LED3_BRIGHTNESS    = 0x8A,     /* Brightness for LED 3, Slot 3 (0-100) */
    HYPERX_REG_SLOT3_LED4_BRIGHTNESS    = 0x8D,     /* Brightness for LED 4, Slot 3 (0-100) */

    HYPERX_REG_EFFECT_BRIGHTNESS        = 0xDD,     /* Brightness for effects (0-100)       */
    HYPERX_REG_APPLY                    = 0xE1,     /* Apply changes register               */
    HYPERX_REG_MODE1                    = 0xE3,     /* Mode control register 1              */
    HYPERX_REG_MODE2                    = 0xE4,     /* Mode control register 2              */
    HYPERX_REG_MODE3                    = 0xE5,     /* Mode control register 3              */
    HYPERX_REG_EFFECT_RED               = 0xEC,     /* Red color register for effects       */
    HYPERX_REG_EFFECT_GREEN             = 0xED,     /* Green color register for effects     */
    HYPERX_REG_EFFECT_BLUE              = 0xEE,     /* Blue color register for effects      */
};

enum
{
    HYPERX_MODE1_RAINBOW                = 0x05,     /* Mode 1 rainbow effect                */
    HYPERX_MODE1_CYCLE                  = 0x04,     /* Mode 1 cycle effect                  */
};

enum
{
    HYPERX_MODE2_BOUNCE                 = 0x02,     /* Mode 2 bounce effect                 */
    HYPERX_MODE2_BREATHING              = 0x03,     /* Mode 2 breathing effect              */
    HYPERX_MODE2_BLINK                  = 0x06,     /* Mode 2 blink effect                  */
    HYPERX_MODE2_HEARTBEAT              = 0x07,     /* Mode 2 heartbeat effect              */
    HYPERX_MODE2_COMET                  = 0x08,     /* Mode 2 comet effect                  */
    HYPERX_MODE2_STATIC                 = 0x09,     /* Mode 2 static effect                 */
};

enum
{
    HYPERX_MODE3_DIRECT                 = 0x21,     /* Mode 3 direct control                */
};

enum
{
    HYPERX_MODE_DIRECT                  = 0,        /* Direct control mode                  */
    HYPERX_MODE_STATIC                  = 1,        /* Static color mode                    */
    HYPERX_MODE_RAINBOW                 = 2,        /* Rainbow wave mode                    */
    HYPERX_MODE_COMET                   = 3,        /* Comet (chase) mode                   */
    HYPERX_MODE_HEARTBEAT               = 4,        /* Heartbeat (pulsing) mode             */
    HYPERX_MODE_CYCLE                   = 5,        /* Spectrum cycle mode                  */
    HYPERX_MODE_BREATHING               = 6,        /* Breathing mode                       */
    HYPERX_MODE_BOUNCE                  = 7,        /* Bounce mode                          */
    HYPERX_MODE_BLINK                   = 8,        /* Blinking mode                        */
    HYPERX_NUMBER_MODES                             /* Number of HyperX modes               */
};


class HyperXController
{
public:
    HyperXController(i2c_smbus_interface* bus, hyperx_dev_id dev);
    ~HyperXController();

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    unsigned int    GetLEDCount();
    unsigned int    GetMode();
    void            SetMode(unsigned char new_mode);

    void            SetAllColors(unsigned char red, unsigned char green, unsigned char blue);
    void            SetEffectColor(unsigned char red, unsigned char green, unsigned char blue);
    void            SetLEDColor(unsigned int slot, unsigned int led, unsigned char red, unsigned char green, unsigned char blue);

private:
    char                    device_name[32];
    unsigned int            led_count;
    i2c_smbus_interface*    bus;
    hyperx_dev_id           dev;
    unsigned int            mode;
};
