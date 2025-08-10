/*---------------------------------------------------------*\
| HyperXDRAMController.h                                    |
|                                                           |
|   Driver for HyperX/Kingston Fury RAM                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

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

    HYPERX_REG_SLOT1_LED0_RED           = 0x41,     /* R color register for LED 0, Slot 1   */
    HYPERX_REG_SLOT1_LED0_GREEN         = 0x42,     /* G color register for LED 0, Slot 1   */
    HYPERX_REG_SLOT1_LED0_BLUE          = 0x43,     /* B color register for LED 0, Slot 1   */
    HYPERX_REG_SLOT1_LED1_RED           = 0x44,     /* R color register for LED 1, Slot 1   */
    HYPERX_REG_SLOT1_LED1_GREEN         = 0x45,     /* G color register for LED 1, Slot 1   */
    HYPERX_REG_SLOT1_LED1_BLUE          = 0x46,     /* B color register for LED 1, Slot 1   */
    HYPERX_REG_SLOT1_LED2_RED           = 0x47,     /* R color register for LED 2, Slot 1   */
    HYPERX_REG_SLOT1_LED2_GREEN         = 0x48,     /* G color register for LED 2, Slot 1   */
    HYPERX_REG_SLOT1_LED2_BLUE          = 0x49,     /* B color register for LED 2, Slot 1   */
    HYPERX_REG_SLOT1_LED3_RED           = 0x4A,     /* R color register for LED 3, Slot 1   */
    HYPERX_REG_SLOT1_LED3_GREEN         = 0x4B,     /* G color register for LED 3, Slot 1   */
    HYPERX_REG_SLOT1_LED3_BLUE          = 0x4C,     /* B color register for LED 3, Slot 1   */
    HYPERX_REG_SLOT1_LED4_RED           = 0x4D,     /* R color register for LED 4, Slot 1   */
    HYPERX_REG_SLOT1_LED4_GREEN         = 0x4E,     /* G color register for LED 4, Slot 1   */
    HYPERX_REG_SLOT1_LED4_BLUE          = 0x4F,     /* B color register for LED 4, Slot 1   */
    HYPERX_REG_SLOT1_LED0_BRIGHTNESS    = 0x51,     /* Brightness for LED 0, Slot 1 (0-100) */
    HYPERX_REG_SLOT1_LED1_BRIGHTNESS    = 0x54,     /* Brightness for LED 1, Slot 1 (0-100) */
    HYPERX_REG_SLOT1_LED2_BRIGHTNESS    = 0x57,     /* Brightness for LED 2, Slot 1 (0-100) */
    HYPERX_REG_SLOT1_LED3_BRIGHTNESS    = 0x5A,     /* Brightness for LED 3, Slot 1 (0-100) */
    HYPERX_REG_SLOT1_LED4_BRIGHTNESS    = 0x5D,     /* Brightness for LED 4, Slot 1 (0-100) */

    HYPERX_REG_SLOT2_LED0_RED           = 0x71,     /* R color register for LED 0, Slot 2   */
    HYPERX_REG_SLOT2_LED0_GREEN         = 0x72,     /* G color register for LED 0, Slot 2   */
    HYPERX_REG_SLOT2_LED0_BLUE          = 0x73,     /* B color register for LED 0, Slot 2   */
    HYPERX_REG_SLOT2_LED1_RED           = 0x74,     /* R color register for LED 1, Slot 2   */
    HYPERX_REG_SLOT2_LED1_GREEN         = 0x75,     /* G color register for LED 1, Slot 2   */
    HYPERX_REG_SLOT2_LED1_BLUE          = 0x76,     /* B color register for LED 1, Slot 2   */
    HYPERX_REG_SLOT2_LED2_RED           = 0x77,     /* R color register for LED 2, Slot 2   */
    HYPERX_REG_SLOT2_LED2_GREEN         = 0x78,     /* G color register for LED 2, Slot 2   */
    HYPERX_REG_SLOT2_LED2_BLUE          = 0x79,     /* B color register for LED 2, Slot 2   */
    HYPERX_REG_SLOT2_LED3_RED           = 0x7A,     /* R color register for LED 3, Slot 2   */
    HYPERX_REG_SLOT2_LED3_GREEN         = 0x7B,     /* G color register for LED 3, Slot 2   */
    HYPERX_REG_SLOT2_LED3_BLUE          = 0x7C,     /* B color register for LED 3, Slot 2   */
    HYPERX_REG_SLOT2_LED4_RED           = 0x7D,     /* R color register for LED 4, Slot 2   */
    HYPERX_REG_SLOT2_LED4_GREEN         = 0x7E,     /* G color register for LED 4, Slot 2   */
    HYPERX_REG_SLOT2_LED4_BLUE          = 0x7F,     /* B color register for LED 4, Slot 2   */
    HYPERX_REG_SLOT2_LED0_BRIGHTNESS    = 0x81,     /* Brightness for LED 0, Slot 2 (0-100) */
    HYPERX_REG_SLOT2_LED1_BRIGHTNESS    = 0x84,     /* Brightness for LED 1, Slot 2 (0-100) */
    HYPERX_REG_SLOT2_LED2_BRIGHTNESS    = 0x87,     /* Brightness for LED 2, Slot 2 (0-100) */
    HYPERX_REG_SLOT2_LED3_BRIGHTNESS    = 0x8A,     /* Brightness for LED 3, Slot 2 (0-100) */
    HYPERX_REG_SLOT2_LED4_BRIGHTNESS    = 0x8D,     /* Brightness for LED 4, Slot 2 (0-100) */

    HYPERX_REG_SLOT3_LED0_RED           = 0xA1,     /* R color register for LED 0, Slot 3   */
    HYPERX_REG_SLOT3_LED0_GREEN         = 0xA2,     /* G color register for LED 0, Slot 3   */
    HYPERX_REG_SLOT3_LED0_BLUE          = 0xA3,     /* B color register for LED 0, Slot 3   */
    HYPERX_REG_SLOT3_LED1_RED           = 0xA4,     /* R color register for LED 1, Slot 3   */
    HYPERX_REG_SLOT3_LED1_GREEN         = 0xA5,     /* G color register for LED 1, Slot 3   */
    HYPERX_REG_SLOT3_LED1_BLUE          = 0xA6,     /* B color register for LED 1, Slot 3   */
    HYPERX_REG_SLOT3_LED2_RED           = 0xA7,     /* R color register for LED 2, Slot 3   */
    HYPERX_REG_SLOT3_LED2_GREEN         = 0xA8,     /* G color register for LED 2, Slot 3   */
    HYPERX_REG_SLOT3_LED2_BLUE          = 0xA9,     /* B color register for LED 2, Slot 3   */
    HYPERX_REG_SLOT3_LED3_RED           = 0xAA,     /* R color register for LED 3, Slot 3   */
    HYPERX_REG_SLOT3_LED3_GREEN         = 0xAB,     /* G color register for LED 3, Slot 3   */
    HYPERX_REG_SLOT3_LED3_BLUE          = 0xAC,     /* B color register for LED 3, Slot 3   */
    HYPERX_REG_SLOT3_LED4_RED           = 0xAD,     /* R color register for LED 4, Slot 3   */
    HYPERX_REG_SLOT3_LED4_GREEN         = 0xAE,     /* G color register for LED 4, Slot 3   */
    HYPERX_REG_SLOT3_LED4_BLUE          = 0xAF,     /* B color register for LED 4, Slot 3   */
    HYPERX_REG_SLOT3_LED0_BRIGHTNESS    = 0xB1,     /* Brightness for LED 0, Slot 3 (0-100) */
    HYPERX_REG_SLOT3_LED1_BRIGHTNESS    = 0xB4,     /* Brightness for LED 1, Slot 3 (0-100) */
    HYPERX_REG_SLOT3_LED2_BRIGHTNESS    = 0xB7,     /* Brightness for LED 2, Slot 3 (0-100) */
    HYPERX_REG_SLOT3_LED3_BRIGHTNESS    = 0xBA,     /* Brightness for LED 3, Slot 3 (0-100) */
    HYPERX_REG_SLOT3_LED4_BRIGHTNESS    = 0xBD,     /* Brightness for LED 4, Slot 3 (0-100) */

    HYPERX_REG_TIMER_MSB                = 0xD1,     /* Timer MSB                            */
    HYPERX_REG_TIMER_LSB                = 0xD2,     /* Timer LSB                            */
    HYPERX_REG_ON_TIME_MSB              = 0xD3,     /* Effect on time MSB                   */
    HYPERX_REG_ON_TIME_LSB              = 0xD4,     /* Effect on time LSB                   */
    HYPERX_REG_CHANGE_TIME_MSB          = 0xD5,     /* Change time MSB                      */
    HYPERX_REG_CHANGE_TIME_LSB          = 0xD6,     /* Change time LSB                      */
    HYPERX_REG_FADE_IN_TIME_MSB         = 0xD7,     /* Fade in time MSB                     */
    HYPERX_REG_FADE_IN_TIME_LSB         = 0xD8,     /* Fade in time LSB                     */
    HYPERX_REG_FADE_OUT_TIME_MSB        = 0xD9,     /* Fade out time MSB                    */
    HYPERX_REG_FADE_OUT_TIME_LSB        = 0xDA,     /* Fade out time LSB                    */
    HYPERX_REG_OFF_TIME_MSB             = 0xDB,     /* Effect off time MSB                  */
    HYPERX_REG_OFF_TIME_LSB             = 0xDC,     /* Effect off time LSB                  */
    HYPERX_REG_EFFECT_BRIGHTNESS        = 0xDD,     /* Brightness for effects (0-100)       */
    HYPERX_REG_APPLY                    = 0xE1,     /* Apply changes register               */
    HYPERX_REG_MODE_RANDOM              = 0xE3,     /* Mode control register, random colors */
    HYPERX_REG_MODE_CUSTOM              = 0xE4,     /* Mode control register, custom colors */
    HYPERX_REG_MODE_INDEPENDENT         = 0xE5,     /* Mode control register, independent   */
    HYPERX_REG_DELAY_TIME_MSB           = 0xEA,     /* Delay time MSB                       */
    HYPERX_REG_DELAY_TIME_LSB           = 0xEB,     /* Delay time LSB                       */
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

enum
{
    HYPERX_SPEED_BOUNCE_SLOW            = 0x07D0,   /* Slowest speed for bounce mode        */
    HYPERX_SPEED_BOUNCE_NORMAL          = 0x07D0,   /* Normal speed for bounce mode         */
    HYPERX_SPEED_BOUNCE_FAST            = 0x0064,   /* Fastest speed for bounce mode        */
    HYPERX_SPEED_BREATHING_SLOW         = 0x07D0,   /* Slowest speed for breathing mode     */
    HYPERX_SPEED_BREATHING_NORMAL       = 0x07D0,   /* Normal speed for breathing mode      */
    HYPERX_SPEED_BREATHING_FAST         = 0x0064,   /* Fastest speed for breathing mode     */
    HYPERX_SPEED_CYCLE_SLOW             = 0x05DC,   /* Slowest speed for cycle mode         */
    HYPERX_SPEED_CYCLE_NORMAL           = 0x05DC,   /* Normal speed for cycle mode          */
    HYPERX_SPEED_CYCLE_FAST             = 0x00FA,   /* Fastest speed for cycle mode         */
    HYPERX_SPEED_RAINBOW_SLOW           = 0x07D0,   /* Slowest speed for rainbow mode       */
    HYPERX_SPEED_RAINBOW_NORMAL         = 0x07D0,   /* Normal speed for rainbow mode        */
    HYPERX_SPEED_RAINBOW_FAST           = 0x0064,   /* Fastest speed for rainbow mode       */
    HYPERX_SPEED_BLINK_SLOW             = 0x07D0,   /* Slowest speed for blink mode         */
    HYPERX_SPEED_BLINK_NORMAL           = 0x07D0,   /* Normal speed for blink mode          */
    HYPERX_SPEED_BLINK_FAST             = 0x01F4,   /* Fastest speed for blink mode         */
    HYPERX_SPEED_HEARTBEAT_SLOW         = 0x07D0,   /* Slowest speed for heartbeat mode     */
    HYPERX_SPEED_HEARTBEAT_NORMAL       = 0x07D0,   /* Normal speed for heartbeat mode      */
    HYPERX_SPEED_HEARTBEAT_FAST         = 0x01F4,   /* Fastest speed for heartbeat mode     */
    HYPERX_SPEED_COMET_SLOW             = 0x07D0,   /* Slowest speed for comet mode         */
    HYPERX_SPEED_COMET_NORMAL           = 0x07D0,   /* Normal speed for comet mode          */
    HYPERX_SPEED_COMET_FAST             = 0x0064,   /* Fastest speed for comet mode         */
};

static const unsigned char slot_base[4] =
{
    HYPERX_REG_SLOT0_LED0_RED,                      /* SPD 0x50 maps to slot 0              */
    HYPERX_REG_SLOT1_LED0_RED,                      /* SPD 0x52 maps to slot 1              */
    HYPERX_REG_SLOT2_LED0_RED,                      /* SPD 0x51 maps to slot 2              */
    HYPERX_REG_SLOT3_LED0_RED                       /* SPD 0x53 maps to slot 3              */
};

static const unsigned char slot_map[4] =
{
    0,
    2,
    1,
    3
};

class HyperXDRAMController
{
public:
    HyperXDRAMController(i2c_smbus_interface* bus, hyperx_dev_id dev, unsigned char slots, std::string dev_name);
    ~HyperXDRAMController();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    unsigned int    GetLEDCount();
    unsigned int    GetSlotCount();
    unsigned int    GetMode();

    void            SendApply();

    void            SetMode(unsigned char new_mode, bool random, unsigned short new_speed);

    void            SetAllColors(unsigned char red, unsigned char green, unsigned char blue);
    void            SetEffectColor(unsigned char red, unsigned char green, unsigned char blue);
    void            SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void            SetLEDColor(unsigned int slot, unsigned int led, unsigned char red, unsigned char green, unsigned char blue);

private:
    unsigned int            led_count;
    unsigned char           slots_valid;
    i2c_smbus_interface*    bus;
    hyperx_dev_id           dev;
    unsigned int            mode;
    unsigned short          speed;
    std::string             name;
};
