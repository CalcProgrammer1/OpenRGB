/*---------------------------------------------------------*\
| GigabyteRGBFusion2DRAMController.cpp                      |
|                                                           |
|   Driver for Gigabyte Aorus RGB Fusion 2 RAM              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                07 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char   rgb_fusion_dev_id;

enum
{
    RGB_FUSION_2_DRAM_LED_EN_MASK       = 0x00,     /* LED enable bitfield                          */
    RGB_FUSION_2_DRAM_IDX_MODE          = 0x09,     /* Mode index                                   */
    RGB_FUSION_2_DRAM_IDX_BRIGHTNESS    = 0x0A,     /* Brightness index                             */
    RGB_FUSION_2_DRAM_IDX_BLUE          = 0x0C,     /* Blue index                                   */
    RGB_FUSION_2_DRAM_IDX_GREEN         = 0x0D,     /* Green index                                  */
    RGB_FUSION_2_DRAM_IDX_RED           = 0x0E,     /* Red index                                    */
//  RGB_FUSION_2_DRAM_TIMER_1_LSB       = 0x08,     /* Timer 1 LSB. Valid timer values [0-65535]    */
//  RGB_FUSION_2_DRAM_TIMER_1_MSB       = 0x09,     /* Timer 1 MSB. Timer unis are milliseconds     */
//  RGB_FUSION_2_DRAM_TIMER_2_LSB       = 0x0A,     /* Timer 2 LSB                                  */
//  RGB_FUSION_2_DRAM_TIMER_2_MSB       = 0x0B,     /* Timer 2 MSB                                  */
//  RGB_FUSION_2_DRAM_TIMER_3_LSB       = 0x0C,     /* Timer 3 LSB                                  */
//  RGB_FUSION_2_DRAM_TIMER_3_MSB       = 0x0D,     /* Timer 3 MSB                                  */
//  RGB_FUSION_2_DRAM_IDX_OPT_1	        = 0x0E,	    /* Option 1. Use case varies by mode            */
//  RGB_FUSION_2_DRAM_IDX_OPT_2	        = 0x0F,	    /* Option 2. Use case varies by mode            */
};

enum
{
    RGB_FUSION_2_DRAM_LED_START_ADDR    = 0x20,
    RGB_FUSION_2_DRAM_APPLY_ADDR        = 0x28,
};

enum
{
    RGB_FUSION_2_DRAM_ACTION_APPLY	    = 0x0F,
};

enum
{
    RGB_FUSION_2_DRAM_MODE_OFF          = 0x00,     /* Off mode                                     */
    RGB_FUSION_2_DRAM_MODE_STATIC       = 0x01,     /* Static mode                                  */
    RGB_FUSION_2_DRAM_MODE_PULSE        = 0x02,     /* Pulsing mode                                 */
    RGB_FUSION_2_DRAM_MODE_FLASH        = 0x03,     /* Flashing mode                                */
    RGB_FUSION_2_DRAM_MODE_DIRECT       = 0xFF      /* Dummy mode, implements per LED using Pulse   */
};

class RGBFusion2DRAMController
{
public:
    RGBFusion2DRAMController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev);
    ~RGBFusion2DRAMController();

    std::string     GetDeviceLocation();
    unsigned int    GetLEDCount();
    void	        Apply();

    void            SetLEDEffect
                        (
                        unsigned int    led,
                        int             mode,
                        unsigned int    brightness,
                        unsigned int    speed,
                        unsigned char   red,
                        unsigned char   green,
                        unsigned char   blue
                        );

private:
    unsigned int            led_count;
    i2c_smbus_interface*    bus;
    rgb_fusion_dev_id       dev;
    bool                    direct_initialized;

    unsigned char           led_data[32];
};
