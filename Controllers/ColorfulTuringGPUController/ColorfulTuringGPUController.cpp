/*---------------------------------------------------------*\
| ColorfulTuringGPUController.cpp                           |
|                                                           |
|   Driver for Colorful Turing GPU                          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "ColorfulTuringGPUController.h"

ColorfulTuringGPUController::ColorfulTuringGPUController(i2c_smbus_interface* bus, colorful_gpu_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

ColorfulTuringGPUController::~ColorfulTuringGPUController()
{

}

std::string ColorfulTuringGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string ColorfulTuringGPUController::GetDeviceName()
{
    return(name);
}

int ColorfulTuringGPUController::GetMode()
{
    uint8_t data_pkt[COLORFUL_MODE_PACKET_LENGTH];
    int size = COLORFUL_MODE_PACKET_LENGTH;
    bus->i2c_read_block(dev, &size, data_pkt);

    int mode = data_pkt[2]<<16 | data_pkt[3]<<8 | data_pkt[4];

    return mode;
}

RGBColor ColorfulTuringGPUController::GetColor()
{
    uint8_t data_pkt[COLORFUL_MODE_PACKET_LENGTH];
    int size = COLORFUL_MODE_PACKET_LENGTH;
    bus->i2c_read_block(dev, &size, data_pkt);

    RGBColor color = ToRGBColor(data_pkt[5], data_pkt[6], data_pkt[7]);

    return color;
}

void ColorfulTuringGPUController::SetStateDisplay(RGBColor color)
{
    uint8_t r = RGBGetRValue(color);
    uint8_t g = RGBGetGValue(color);
    uint8_t b = RGBGetBValue(color);
    uint8_t data_pkt[COLORFUL_COLOR_PACKET_LENGTH] = { 0x08, 0x01, 0x32, 0x04, r, g, b};

    int crc = 1;

    for(int i = 0; i < COLORFUL_COLOR_PACKET_LENGTH - 1; ++i)
    {
        crc += data_pkt[i];
    }
    crc &= 0xFF;
    crc = 256-crc;

    data_pkt[COLORFUL_COLOR_PACKET_LENGTH - 1] = crc & 0xFF;

    bus->i2c_write_block(dev, COLORFUL_COLOR_PACKET_LENGTH, data_pkt);
}

void ColorfulTuringGPUController::SetDirect(RGBColor color, bool save)
{
    uint8_t r = RGBGetRValue(color);
    uint8_t g = RGBGetGValue(color);
    uint8_t b = RGBGetBValue(color);
    uint8_t data_pkt[COLORFUL_COLOR_PACKET_LENGTH] = { 0x08, 0x0, 0x20, 0x10, r, g, b};
    if(save)
    {
        data_pkt[0] = 0x88;
        data_pkt[1] = 0x02;
        data_pkt[2] = 0x32;
        data_pkt[3] = 0x02;
    }

    int crc = 1;

    for(int i = 0; i < COLORFUL_COLOR_PACKET_LENGTH - 1; ++i)
    {
        crc += data_pkt[i];
    }
    crc &= 0xFF;
    crc = 256-crc;

    data_pkt[COLORFUL_COLOR_PACKET_LENGTH - 1] = crc & 0xFF;

    bus->i2c_write_block(dev, COLORFUL_COLOR_PACKET_LENGTH, data_pkt);
}

void ColorfulTuringGPUController::SetBreathing(RGBColor color)
{
    uint8_t r = RGBGetRValue(color);
    uint8_t g = RGBGetGValue(color);
    uint8_t b = RGBGetBValue(color);
    uint8_t data_pkt[COLORFUL_COLOR_PACKET_LENGTH] = { 0x88, 0x01, 0x32, 0x02, r, g, b};

    int crc = 1;

    for(int i = 0; i < COLORFUL_COLOR_PACKET_LENGTH - 1; ++i)
    {
        crc += data_pkt[i];
    }
    crc &= 0xFF;
    crc = 256-crc;

    data_pkt[COLORFUL_COLOR_PACKET_LENGTH - 1] = crc & 0xFF;

    bus->i2c_write_block(dev, COLORFUL_COLOR_PACKET_LENGTH, data_pkt);
}

void ColorfulTuringGPUController::SetOff()
{
    uint8_t data_pkt[COLORFUL_NON_COLOR_PACKET_LENGTH] = { 0x85, 0x00, 0x00, 0x0A };

    int crc = 1;

    for(int i = 0; i < COLORFUL_NON_COLOR_PACKET_LENGTH - 1; ++i)
    {
        crc += data_pkt[i];
    }
    crc &= 0xFF;
    crc = 256-crc;

    data_pkt[COLORFUL_NON_COLOR_PACKET_LENGTH - 1] = crc & 0xFF;

    bus->i2c_write_block(dev, COLORFUL_NON_COLOR_PACKET_LENGTH, data_pkt);
}

void ColorfulTuringGPUController::SetRainbow()
{
    uint8_t data_pkt[COLORFUL_NON_COLOR_PACKET_LENGTH] = { 0x85, 0x04, 0x32, 0x02 };

    int crc = 1;

    for(int i = 0; i < COLORFUL_NON_COLOR_PACKET_LENGTH - 1; ++i)
    {
        crc += data_pkt[i];
    }
    crc &= 0xFF;
    crc = 256-crc;

    data_pkt[COLORFUL_NON_COLOR_PACKET_LENGTH - 1] = crc & 0xFF;

    bus->i2c_write_block(dev, COLORFUL_NON_COLOR_PACKET_LENGTH, data_pkt);
}
