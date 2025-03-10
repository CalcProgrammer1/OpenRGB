/*---------------------------------------------------------*\
| CorsairVengeanceProController.cpp                         |
|                                                           |
|   Driver for Corsair Vengeance Pro RGB RAM                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <chrono>
#include "CorsairVengeanceProController.h"

using namespace std::chrono_literals;

CorsairVengeanceProController::CorsairVengeanceProController(i2c_smbus_interface* bus, corsair_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    strcpy(device_name, "Corsair Vengeance Pro RGB");
    led_count = CORSAIR_PRO_LED_COUNT;

    direct_mode = false;
    effect_mode = CORSAIR_PRO_MODE_STATIC;

    for (unsigned int i = 0; i < led_count; i++)
    {
        led_red[i]      = 0;
        led_green[i]    = 0;
        led_blue[i]     = 0;
    }
}

CorsairVengeanceProController::~CorsairVengeanceProController()
{

}

std::string CorsairVengeanceProController::GetDeviceName()
{
    return(device_name);
}

std::string CorsairVengeanceProController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned int CorsairVengeanceProController::GetLEDCount()
{
    return(led_count);
}

unsigned char CorsairVengeanceProController::GetEffect()
{
    return(effect_mode);
}

void CorsairVengeanceProController::SetAllColors(unsigned char red, unsigned char green, unsigned char blue)
{
    for (unsigned int i = 0; i < led_count; i++)
    {
        led_red[i]      = red;
        led_green[i]    = green;
        led_blue[i]     = blue;
    }
}

void CorsairVengeanceProController::SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    led_red[led]    = red;
    led_green[led]  = green;
    led_blue[led]   = blue;
}

void CorsairVengeanceProController::ApplyColors()
{
    if(direct_mode)
    {
        unsigned char   full_packet[32];
        unsigned char   crc_packet[31];

        full_packet[0]  = 0x0A;
        crc_packet[0]   = 0x0A;

        for (int i = 0; i < 10; i++)
        {
            crc_packet[(i * 3) + 1]     = led_red[i];
            full_packet[(i * 3) + 1]    = led_red[i];

            crc_packet[(i * 3) + 2]     = led_green[i];
            full_packet[(i * 3) + 2]    = led_green[i];

            crc_packet[(i * 3) + 3]     = led_blue[i];
            full_packet[(i * 3) + 3]    = led_blue[i];
        }

        uint8_t footer = CRCPP::CRC::Calculate(crc_packet, 31, CRCPP::CRC::CRC_8());

        full_packet[31] = footer;

        bus->i2c_smbus_write_block_data(dev, CORSAIR_PRO_DIRECT_COMMAND, 32, full_packet);
    }
    else
    {
        bus->i2c_smbus_write_byte_data(dev, 0x26, 0x02);
        std::this_thread::sleep_for(1ms);
        bus->i2c_smbus_write_byte_data(dev, 0x21, 0x00);
        std::this_thread::sleep_for(1ms);

        for (int i = 0; i < 10; i++)
        {
            bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, led_red[i]);
            bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, led_green[i]);
            bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, led_blue[i]);
            bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0xFF);
        }

        bus->i2c_smbus_write_byte_data(dev, 0x82, 0x02);
    }
}

void CorsairVengeanceProController::SetEffect(unsigned char mode,
                                     unsigned char speed,
                                     unsigned char direction,
                                     bool          random,
                                     unsigned char red1,
                                     unsigned char grn1,
                                     unsigned char blu1,
                                     unsigned char red2,
                                     unsigned char grn2,
                                     unsigned char blu2
                                    )
{
    if(mode == effect_mode)
    {
        return;
    }

    effect_mode = mode;

    direct_mode = (effect_mode == CORSAIR_PRO_MODE_DIRECT);

    if(direct_mode)
    {
        return;
    }

    bus->i2c_smbus_write_byte_data(dev, 0x26, 0x01);
    std::this_thread::sleep_for(1ms);
    bus->i2c_smbus_write_byte_data(dev, 0x21, 0x00);
    std::this_thread::sleep_for(1ms);

    unsigned char random_byte;

    if(random)
    {
        random_byte = CORSAIR_PRO_EFFECT_RANDOM_COLORS;
    }
    else
    {
        random_byte = CORSAIR_PRO_EFFECT_CUSTOM_COLORS;
    }

    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, effect_mode);  // Mode
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, speed);        // Speed
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, random_byte);  // Custom color
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, direction);    // Direction
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, red1);         // Custom color 1 red
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, grn1);         // Custom color 1 green
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, blu1);         // Custom color 1 blue
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0xFF);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, red2);         // Custom color 2 red
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, grn2);         // Custom color 2 green
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, blu2);         // Custom color 2 blue
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0xFF);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);

    bus->i2c_smbus_write_byte_data(dev, 0x82, 0x01);
    WaitReady();
}

bool CorsairVengeanceProController::WaitReady()
{
    int i = 0;
    while (bus->i2c_smbus_read_byte_data(dev, 0x41) != 0x00)
    {
        i++;
        std::this_thread::sleep_for(1ms);
    }

    return false;
}

void CorsairVengeanceProController::SetDirect(bool direct)
{
    direct_mode = direct;
}
