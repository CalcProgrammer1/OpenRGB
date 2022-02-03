/*-----------------------------------------*\
|  GigabyteRGBFusion2DRAMController.cpp     |
|                                           |
|  Driver for Gigabyte Aorus RGB Fusion 2   |
|  DRAM lighting controller                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/7/2020    |
\*-----------------------------------------*/

#include "GigabyteRGBFusion2DRAMController.h"

#include <cstring>
#include <stdio.h>
#include <stdlib.h>

RGBFusion2DRAMController::RGBFusion2DRAMController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev)
{
    /*-----------------------------------------------------*\
    | Initialize pointers                                   |
    \*-----------------------------------------------------*/
    this->bus = bus;
    this->dev = dev;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(led_data, 0, sizeof(led_data));

    /*-----------------------------------------------------*\
    | Initialize controller with 6 LEDs                     |
    | This is hard coded for Aorus RGB RAM                  |
    \*-----------------------------------------------------*/
    led_count = 6;

    /*-----------------------------------------------------*\
    | Initialize brightness to 100%                         |
    \*-----------------------------------------------------*/
    led_data[RGB_FUSION_2_DRAM_IDX_BRIGHTNESS] = 0x64;
}

RGBFusion2DRAMController::~RGBFusion2DRAMController()
{

}

unsigned int RGBFusion2DRAMController::GetLEDCount()
{
    return(led_count);
}

std::string RGBFusion2DRAMController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

void RGBFusion2DRAMController::Apply()
{
    bus->i2c_smbus_write_byte_data(dev, RGB_FUSION_2_DRAM_APPLY_ADDR, RGB_FUSION_2_DRAM_ACTION_APPLY);
}

void RGBFusion2DRAMController::SetLEDEffect
    (
    unsigned int    led,
    int             mode,
    unsigned int    /*speed*/,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    if(mode == RGB_FUSION_2_DRAM_MODE_DIRECT)
    {
        led_data[RGB_FUSION_2_DRAM_LED_EN_MASK] = (1 << led);

        // hack for direct mode
        led_data[16] = 1;
        led_data[22] = 2;
        led_data[29] = 1;
        led_data[30] = 1;

        mode = RGB_FUSION_2_DRAM_MODE_PULSE;
    }
    else
    {
        led_data[RGB_FUSION_2_DRAM_LED_EN_MASK] = 0x3F;
    }

    led_data[RGB_FUSION_2_DRAM_IDX_MODE]        = mode;
    led_data[RGB_FUSION_2_DRAM_IDX_RED]         = red;
    led_data[RGB_FUSION_2_DRAM_IDX_GREEN]       = green;
    led_data[RGB_FUSION_2_DRAM_IDX_BLUE]        = blue;

    bus->i2c_smbus_write_block_data(dev, RGB_FUSION_2_DRAM_LED_START_ADDR, 32, led_data);

    Apply();
}
