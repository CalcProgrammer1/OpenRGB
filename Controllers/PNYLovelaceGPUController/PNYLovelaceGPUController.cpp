/*---------------------------------------------------------*\
| PNYLovelaceGPUController.cpp                              |
|                                                           |
|   Driver for PNY Lovelace GPU                             |
|                                                           |
|   yufan                                       01 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "PNYLovelaceGPUController.h"

PNYLovelaceGPUController::PNYLovelaceGPUController(i2c_smbus_interface* bus, pny_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

PNYLovelaceGPUController::~PNYLovelaceGPUController()
{

}

std::string PNYLovelaceGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string PNYLovelaceGPUController::GetDeviceName()
{
    return(name);
}

void PNYLovelaceGPUController::SetOff()
{
    unsigned char data[7] = {};
    bus->i2c_smbus_write_i2c_block_data(dev, PNY_GPU_REG_LIGHTING, sizeof(data), data);
}

void PNYLovelaceGPUController::SetDirect(unsigned char led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char data[7] = {PNY_GPU_MODE_STATIC, 0xFF, led, 0x00, red, green, blue};
    bus->i2c_smbus_write_i2c_block_data(dev, PNY_GPU_REG_LIGHTING, sizeof(data), data);
}

void PNYLovelaceGPUController::SetCycle(unsigned char speed, unsigned char brightness)
{
    speed = 0xB2 - speed;
    unsigned char data[7] = {PNY_GPU_MODE_CYCLE, brightness, speed, 0x00, 0xAA, 0x00, 0x00};
    bus->i2c_smbus_write_i2c_block_data(dev, PNY_GPU_REG_LIGHTING, sizeof(data), data);
}

void PNYLovelaceGPUController::SetBreath(unsigned char speed, unsigned char red, unsigned char green, unsigned char blue)
{
    speed = 0x19 - speed;
    unsigned char data[7] = {PNY_GPU_MODE_BREATH, 0xFF, speed, 0x01, red, green, blue};
    bus->i2c_smbus_write_i2c_block_data(dev, PNY_GPU_REG_LIGHTING, sizeof(data), data);
}

void PNYLovelaceGPUController::SetWave(unsigned char speed, unsigned char brightness)
{
    speed = 0xBF - speed;
    unsigned char data[7] = {PNY_GPU_MODE_WAVE, brightness, speed, 0x00, 0xAA, 0x00, 0x00};
    bus->i2c_smbus_write_i2c_block_data(dev, PNY_GPU_REG_LIGHTING, sizeof(data), data);
}

void PNYLovelaceGPUController::SetFlash(unsigned char speed, unsigned char brightness, unsigned char red, unsigned char green, unsigned char blue)
{
    speed = 0x4D - speed;
    unsigned char data[7] = {PNY_GPU_MODE_FLASH, brightness, speed, 0x00, red, green, blue};
    bus->i2c_smbus_write_i2c_block_data(dev, PNY_GPU_REG_LIGHTING, sizeof(data), data);
}
