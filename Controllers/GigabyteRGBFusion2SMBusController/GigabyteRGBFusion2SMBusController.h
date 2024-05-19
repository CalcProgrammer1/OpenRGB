/*---------------------------------------------------------*\
| GigabyteRGBFusion2SMBusController.h                       |
|                                                           |
|   Driver for Gigabyte Aorus RGB Fusion 2 SMBus            |
|   motherboard                                             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                12 Mar 2020 |
|   Matt Harper                                 05 May 2020 |
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
    RGB_FUSION_2_IDX_MODE           = 0x01,     /* Mode index                                   */
    RGB_FUSION_2_IDX_BRIGHTNESS     = 0x02,     /* Brightness index                             */
    RGB_FUSION_2_IDX_MAX_BRIGHTNESS = 0x02,     /* Max brightness index                         */
    RGB_FUSION_2_IDX_MIN_BRIGHTNESS = 0x03,     /* Minimum brightness index                     */
    RGB_FUSION_2_IDX_BLUE           = 0x04,     /* Blue index                                   */
    RGB_FUSION_2_IDX_GREEN          = 0x05,     /* Green index                                  */
    RGB_FUSION_2_IDX_RED            = 0x06,     /* Red index                                    */
    RGB_FUSION_2_IDX_WHITE	        = 0x07,	    /* White index			                        */
    RGB_FUSION_2_TIMER_1_LSB        = 0x08,     /* Timer 1 LSB. Valid timer values [0-65535] 	*/
    RGB_FUSION_2_TIMER_1_MSB        = 0x09,     /* Timer 1 MSB. Timer unis are milliseconds   	*/
    RGB_FUSION_2_TIMER_2_LSB        = 0x0A,     /* Timer 2 LSB                      		    */
    RGB_FUSION_2_TIMER_2_MSB        = 0x0B,     /* Timer 2 MSB                      		    */
    RGB_FUSION_2_TIMER_3_LSB        = 0x0C,     /* Timer 3 LSB                      		    */
    RGB_FUSION_2_TIMER_3_MSB        = 0x0D,     /* Timer 3 MSB                      		    */
    RGB_FUSION_2_IDX_OPT_1	        = 0x0E,	    /* Option 1. Use case varies by mode		    */
    RGB_FUSION_2_IDX_OPT_2	        = 0x0F,	    /* Option 2. Use case varies by mode		    */
};

enum
{
    RGB_FUSION_2_APPLY_ADDR	        = 0x17,
    RGB_FUSION_2_LED_START_ADDR     = 0x20,
    RGB_FUSION_2_SMBUS_ADDR	        = 0x68,
};

enum
{
    RGB_FUSION_2_ACTION_APPLY	    = 0x01ff,
};

enum
{
    RGB_FUSION_2_MODE_PULSE         = 0x01,     /* Pulse mode                       */
    RGB_FUSION_2_MODE_MUSIC	        = 0x02,	    /* Music mode			            */
    RGB_FUSION_2_MODE_COLOR_CYCLE   = 0x03,     /* Color cycle mode                 */
    RGB_FUSION_2_MODE_STATIC        = 0x04,     /* Static color mode                */
    RGB_FUSION_2_MODE_FLASHING      = 0x05,     /* Flashing / Double Flashing mode  */
    RGB_FUSION_2_MODE_TRANSITION    = 0x09,	    /* Gradual transition from current  */
    RGB_FUSION_2_MODE_DIGITAL_WAVE	= 0x0A,	    /* Wave mode			            */
    RGB_FUSION_2_MODE_DIGITAL_A	    = 0x0B,	    /* 				                    */
    RGB_FUSION_2_MODE_DIGITAL_B	    = 0x0C,	    /* 				                    */
    RGB_FUSION_2_MODE_DIGITAL_C	    = 0x0D,	    /* 				                    */
    RGB_FUSION_2_MODE_DIGITAL_D	    = 0x0F,	    /* 				                    */
    RGB_FUSION_2_MODE_DIGITAL_E	    = 0x10,	    /* 				                    */
    RGB_FUSION_2_MODE_DIGITAL_F	    = 0x11,	    /* 				                    */
    RGB_FUSION_2_MODE_DIGITAL_G	    = 0x12,	    /* 				                    */
//    RGB_FUSION_2_MODE_DIGITAL_H	    = 0x11,
//    RGB_FUSION_2_MODE_DIGITAL_I       = 0x12,   They are variants of DIGITAL F and G
};

enum
{
    RGB_FUSION_2_SPEED_FAST         = 0x01,
    RGB_FUSION_2_SPEED_NORMAL       = 0x02,
    RGB_FUSION_2_SPEED_SLOW         = 0x04,
    RGB_FUSION_2_DIGITAL_SPEED      = 0x91,
    RGB_FUSION_2_DIGITAL_SPEED_MIN  = 0xe6,
    RGB_FUSION_2_DIGITAL_SPEED_MAX  = 0x32,
};

enum
{
    RGB_FUSION_2_BRIGHTNESS_MAX     = 0x64,
    RGB_FUSION_2_BRIGHTNESS_MIN     = 0x0f,
};

class RGBFusion2SMBusController
{
public:
    RGBFusion2SMBusController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev, std::string mb_name);
    ~RGBFusion2SMBusController();

    std::string     GetDeviceName();
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
    std::string             name;

    unsigned char           led_data[10][16];

    void		    WriteLED(int);
};
