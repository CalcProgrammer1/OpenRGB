/*---------------------------------------------------------*\
| GigabyteRGBFusionController.h                             |
|                                                           |
|   Driver for Gigabyte Aorus RGB Fusion SMBus motherboard  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                10 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char	rgb_fusion_dev_id;

enum
{
    RGB_FUSION_BANK_0_REG_CH_0_MODE = 0x03, /* Channel 0 Mode Selection             */
    RGB_FUSION_BANK_0_REG_CH_0_TIMER_0_MSB
                                    = 0x06, /* Channel 0 Timer 0 MSB                */
    RGB_FUSION_BANK_0_REG_CH_0_TIMER_0_LSB
                                    = 0x07, /* Channel 0 Timer 0 LSB                */
    RGB_FUSION_BANK_0_REG_CH_0_TIMER_1_MSB
                                    = 0x08, /* Channel 0 Timer 1 MSB                */
    RGB_FUSION_BANK_0_REG_CH_0_TIMER_1_LSB
                                    = 0x09, /* Channel 0 Timer 1 LSB                */
    RGB_FUSION_BANK_0_REG_CH_1_MODE = 0x13, /* Channel 1 Mode Selection             */
    RGB_FUSION_BANK_0_REG_CH_1_TIMER_0_MSB
                                    = 0x16, /* Channel 1 Timer 0 MSB                */
    RGB_FUSION_BANK_0_REG_CH_1_TIMER_0_LSB
                                    = 0x17, /* Channel 1 Timer 0 LSB                */
    RGB_FUSION_BANK_0_REG_CH_1_TIMER_1_MSB
                                    = 0x18, /* Channel 1 Timer 1 MSB                */
    RGB_FUSION_BANK_0_REG_CH_1_TIMER_1_LSB
                                    = 0x19, /* Channel 1 Timer 1 LSB                */
    RGB_FUSION_BANK_1_REG_CH_0_R    = 0x00, /* Channel 0 Red Value                  */
    RGB_FUSION_BANK_1_REG_CH_0_G    = 0x01, /* Channel 0 Green Value                */
    RGB_FUSION_BANK_1_REG_CH_0_B    = 0x02, /* Channel 0 Blue Value                 */
    RGB_FUSION_BANK_1_REG_CH_1_R    = 0x08, /* Channel 1 Red Value                  */
    RGB_FUSION_BANK_1_REG_CH_1_G    = 0x09, /* Channel 1 Green Value                */
    RGB_FUSION_BANK_1_REG_CH_1_B    = 0x0A, /* Channel 1 Blue Value                 */
    RGB_FUSION_BANK_SWITCH_REG      = 0xF0, /* Bank Switch Register                 */
};

enum
{
    RGB_FUSION_SPEED_SLOW           = 0x00, /* Slowest speed                        */
    RGB_FUSION_SPEED_NORMAL         = 0x01, /* Normal speed                         */
    RGB_FUSION_SPEED_FAST           = 0x02, /* Fastest speed                        */
};

static const short speed_table[2][3] =
{
    { 0x01E0, 0x00F0, 0x0078 },
    { 0x4000, 0x2000, 0x1000 }
};

#define RGB_FUSION_NUMBER_MODES       3     /* Number of RGB Fusion modes           */
#define RGB_FUSION_WRITE_MODE_OFST    0x10  /* offset to add when writing mode      */

enum
{
    RGB_FUSION_MODE_STATIC          = 0x00, /* Static color mode                    */
    RGB_FUSION_MODE_BREATHING       = 0x01, /* Breathing effect mode                */
    RGB_FUSION_MODE_FLASHING        = 0x02, /* Flashing effect mode                 */
};

class RGBFusionController
{
public:
    RGBFusionController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev);
    ~RGBFusionController();

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    unsigned int    GetLEDCount();
    unsigned char   GetMode();
    void            SetAllColors(unsigned char red, unsigned char green, unsigned char blue);
    void            SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void            SetMode(unsigned char mode, unsigned char speed);

private:
    void            dump();

    unsigned char   get_mode_ch_0();
    unsigned char   get_mode_ch_1();

    void            set_color_ch_0(unsigned char red, unsigned char green, unsigned char blue);
    void            set_color_ch_1(unsigned char red, unsigned char green, unsigned char blue);
    void            set_mode_ch_0(unsigned char mode);
    void            set_mode_ch_1(unsigned char mode);
    void            set_timers_ch_0(unsigned short timer0, unsigned short timer1);
    void            set_timers_ch_1(unsigned short timer0, unsigned short timer1);
    void            switch_bank(unsigned char bank);

    char                    device_name[32];
    unsigned int            led_count;
    i2c_smbus_interface*    bus;
    rgb_fusion_dev_id       dev;

};
