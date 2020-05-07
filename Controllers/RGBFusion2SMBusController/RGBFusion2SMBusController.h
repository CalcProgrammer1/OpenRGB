/*-----------------------------------------*\
|  RGBFusion2SMBusController.h              |
|                                           |
|  Definitions and types for Gigabyte Aorus |
|  RGB Fusion 2 SMBus lighting controller   |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/12/2020   |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char	rgb_fusion_dev_id;

enum
{
    RGB_FUSION_2_IDX_MODE           = 0x01,     /* Mode index                       */
    RGB_FUSION_2_IDX_BRIGHTNESS     = 0x02,     /* Brightness index                 */
    RGB_FUSION_2_IDX_BLUE           = 0x04,     /* Blue index                       */
    RGB_FUSION_2_IDX_GREEN          = 0x05,     /* Green index                      */
    RGB_FUSION_2_IDX_RED            = 0x06,     /* Red index                        */
    RGB_FUSION_2_TIMER_1_LSB        = 0x08,     /* Timer 1 LSB                      */
    RGB_FUSION_2_TIMER_1_MSB        = 0x09,     /* Timer 1 MSB                      */
    RGB_FUSION_2_TIMER_2_LSB        = 0x0A,     /* Timer 2 LSB                      */
    RGB_FUSION_2_TIMER_2_MSB        = 0x0B,     /* Timer 2 MSB                      */
};

enum
{
    RGB_FUSION_2_MODE_PULSE         = 0x01,     /* Pulse mode                       */
    RGB_FUSION_2_MODE_COLOR_CYCLE   = 0x03,     /* Color cycle mode                 */
    RGB_FUSION_2_MODE_STATIC        = 0x04,     /* Static color mode                */
    RGB_FUSION_2_MODE_FLASHING      = 0x05,     /* Flashing / Double Flashing mode  */
};

enum
{
    RGB_FUSION_2_SPEED_FAST         = 0x01,
    RGB_FUSION_2_SPEED_NORMAL       = 0x02,
    RGB_FUSION_2_SPEED_SLOW         = 0x04
};

class RGBFusion2SMBusController
{
public:
    RGBFusion2SMBusController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev);
    ~RGBFusion2SMBusController();

    std::string     GetDeviceLocation();
    unsigned int    GetLEDCount();
    
    void            SetLEDEffect
                        (
                        unsigned int    led,
                        int             mode,
                        unsigned int    speed,
                        unsigned char   red,
                        unsigned char   green,
                        unsigned char   blue
                        );

private:
    unsigned int            led_count;
    i2c_smbus_interface*    bus;
    rgb_fusion_dev_id       dev;

    unsigned char           led_data[10][16];
};
