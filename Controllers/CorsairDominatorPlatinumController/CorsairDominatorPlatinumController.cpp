/*---------------------------------------------------------*\
| CorsairDominatorPlatinumController.cpp                    |
|                                                           |
|   Driver for Corsair Dominator Platinum RAM               |
|                                                           |
|   Erik Gilling (konkers)                      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <chrono>
#include "CorsairDominatorPlatinumController.h"

using namespace std::chrono_literals;

CorsairDominatorPlatinumController::CorsairDominatorPlatinumController(i2c_smbus_interface *bus, corsair_dev_id dev, unsigned int leds_count, std::string dev_name)
{
    this->bus           = bus;
    this->dev           = dev;
    this->leds_count    = leds_count;
    this->name          = dev_name;

    led_data[0] = 0xc;
}

CorsairDominatorPlatinumController::~CorsairDominatorPlatinumController()
{
}

unsigned int CorsairDominatorPlatinumController::GetLEDCount()
{
    return leds_count;
}

std::string CorsairDominatorPlatinumController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string CorsairDominatorPlatinumController::GetDeviceName()
{
    return(name);
}

void CorsairDominatorPlatinumController::SetAllColors
    (
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    for(unsigned int led = 0; led < leds_count; led++)
    {
        SetLEDColor(led, red, green, blue);
    }
}

void CorsairDominatorPlatinumController::SetLEDColor
    (
    unsigned int    led,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    if(led >= leds_count)
    {
        return;
    }

    unsigned int offset     = (led * 3) + 1;
    led_data[offset]        = red;
    led_data[offset + 1]    = green;
    led_data[offset + 2]    = blue;
}

unsigned char CorsairDominatorPlatinumController::crc8
    (
    unsigned char   init,
    unsigned char   poly,
    unsigned char*  data,
    unsigned char   len
    )
{
    unsigned char crc = init;

    for(unsigned int i = 0; i < len; i++)
    {
        unsigned char val = data[i];
        for(unsigned char mask = 0x80; mask != 0; mask >>= 1)
        {
            unsigned char bit;
            if ((val & mask) != 0)
            {
                bit = (crc & 0x80) ^ 0x80;
            }
            else
            {
                bit = crc & 0x80;
            }

            if (bit == 0)
            {
                crc <<= 1;
            }
            else
            {
                crc = (crc << 1) ^ poly;
            }
        }
    }

    return crc;
}

void CorsairDominatorPlatinumController::ApplyColors()
{
    unsigned char data[sizeof(led_data)];
    memcpy(data, led_data, sizeof(led_data));
    unsigned char crc = crc8(0x0, 0x7, data, sizeof(data) - 1);
    data[sizeof(data) - 1] = crc;

    bus->i2c_smbus_write_block_data(dev, 0x31, 0x20, data);
    std::this_thread::sleep_for(800us);
    bus->i2c_smbus_write_block_data(dev, 0x32, sizeof(data) - 0x20, data + 0x20);
    std::this_thread::sleep_for(200us);
}
