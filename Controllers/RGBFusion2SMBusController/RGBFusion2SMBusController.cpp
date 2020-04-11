/*-----------------------------------------*\
|  RGBFusion2SMBusController.cpp            |
|                                           |
|  Driver for Gigabyte Aorus RGB Fusion 2   |
|  SMBus lighting controller                |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/12/2020   |
\*-----------------------------------------*/

#include "RGBFusion2SMBusController.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

RGBFusion2SMBusController::RGBFusion2SMBusController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

std::string RGBFusion2SMBusController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return(return_string);
}

void RGBFusion2SMBusController::SetLEDEffect
    (
    unsigned int    led,
    int             mode,
    unsigned int    speed,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
led_data[led][RGB_FUSION_2_IDX_MODE]        = mode;
led_data[led][RGB_FUSION_2_IDX_RED]         = red;
led_data[led][RGB_FUSION_2_IDX_GREEN]       = green;
led_data[led][RGB_FUSION_2_IDX_BLUE]        = blue;
}