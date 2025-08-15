/*---------------------------------------------------------*\
| PowerColorRedDevilGPUController.cpp                       |
|                                                           |
|   Driver for PowerColor Red Devil GPU                     |
|                                                           |
|   Jana Rettig (SapphicKitten)                 14 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "pci_ids.h"
#include "PowerColorRedDevilGPUController.h"

using namespace std::chrono_literals;

RedDevilGPUController::RedDevilGPUController(i2c_smbus_interface* bus, red_devil_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;

    if(bus->pci_device > AMD_NAVI10_DEV)                // Only Navi 2 cards have this mode
    {
        this->has_sync_mode = true;
    }
}

RedDevilGPUController::~RedDevilGPUController()
{

}

std::string RedDevilGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C:" + return_string);
}

std::string RedDevilGPUController::GetDeviceName()
{
    return(name);
}

void RedDevilGPUController::SetLEDColor(int led, RGBColor color)
{
    if(led > RED_DEVIL_GPU_LED_MAX_COUNT)
    {
        return;
    }

    unsigned char data[3] =
    {
        (unsigned char)RGBGetRValue(color),
        (unsigned char)RGBGetGValue(color),
        (unsigned char)RGBGetBValue(color),
    };

    RegisterWrite(RED_DEVIL_GPU_REG_LED_1 + led, data);
}

RGBColor RedDevilGPUController::GetLEDColor(int led)
{
    if(led > RED_DEVIL_GPU_LED_MAX_COUNT)
    {
        return RGBColor(0);
    }

    unsigned char data[3] = {0};
    RegisterRead(RED_DEVIL_GPU_REG_LED_1 + RED_DEVIL_GPU_READ_OFFSET, data);
    return ToRGBColor(data[0], data[1], data[2]);
}

void RedDevilGPUController::SetLEDColorAll(RGBColor color)
{
    unsigned char data[3] =
    {
        (unsigned char)RGBGetRValue(color),
        (unsigned char)RGBGetGValue(color),
        (unsigned char)RGBGetBValue(color),
    };

    RegisterWrite(RED_DEVIL_GPU_REG_LED_ALL, data);
}

void RedDevilGPUController::SetModeColor(RGBColor color)
{
    unsigned char data[3] =
    {
        (unsigned char)RGBGetRValue(color),
        (unsigned char)RGBGetGValue(color),
        (unsigned char)RGBGetBValue(color),
    };

    RegisterWrite(RED_DEVIL_GPU_REG_MODE_COLOR, data);
}

RGBColor RedDevilGPUController::GetModeColor()
{
    unsigned char data[3] = {0};
    RegisterRead(RED_DEVIL_GPU_REG_MODE_COLOR + RED_DEVIL_GPU_READ_OFFSET, data);
    return ToRGBColor(data[0], data[1], data[2]);
}

void RedDevilGPUController::SetMode(red_devil_mode_config config)
{
    if(config.mode == RED_DEVIL_GPU_MODE_MB_SYNC)
    {
        unsigned char data[3] = {1, 0, 1};
        RegisterWrite(RED_DEVIL_GPU_REG_MB_SYNC, data);
    }
    else
    {
        unsigned char data[3] = {0};
        RegisterWrite(RED_DEVIL_GPU_REG_MB_SYNC, data);
        RegisterWrite(RED_DEVIL_GPU_REG_MODE, (unsigned char *)&config);
    }
}

red_devil_mode_config RedDevilGPUController::GetMode()
{
    unsigned char data[3] = {0};
    RegisterRead(RED_DEVIL_GPU_REG_MB_SYNC + RED_DEVIL_GPU_READ_OFFSET, data);
    if(data[0] != 0 && this->has_sync_mode)
    {
        return red_devil_mode_config{RED_DEVIL_GPU_MODE_MB_SYNC, 0, 0};
    }

    RegisterRead(RED_DEVIL_GPU_REG_MODE + RED_DEVIL_GPU_READ_OFFSET, data);
    return red_devil_mode_config{data[0], data[1], data[2]};
}

int RedDevilGPUController::RegisterRead(unsigned char reg, unsigned char *data)
{
    int ret = bus->i2c_smbus_read_i2c_block_data(dev, reg, 3, data);
    std::this_thread::sleep_for(32ms);
    return ret;
}

int RedDevilGPUController::RegisterWrite(unsigned char reg, unsigned char *data)
{
    int ret = bus->i2c_smbus_write_i2c_block_data(dev, reg, 3, data);
    std::this_thread::sleep_for(32ms);
    return ret;
}
