/*---------------------------------------------------------*\
| AsusAuraGPUController.cpp                                 |
|                                                           |
|   Driver for ASUS Aura GPU                                |
|                                                           |
|   Jan Rettig (Klapstuhl)                      14 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "AsusAuraGPUController.h"
#include "pci_ids.h"

AuraGPUController::AuraGPUController(i2c_smbus_interface* bus, aura_gpu_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

AuraGPUController::~AuraGPUController()
{

}

std::string AuraGPUController::GetDeviceName()
{
    return(name);
}

std::string AuraGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return(return_string);
}

unsigned char AuraGPUController::GetLEDRed()
{
    return(AuraGPURegisterRead(AURA_GPU_REG_RED));
}

unsigned char AuraGPUController::GetLEDGreen()
{
    return(AuraGPURegisterRead(AURA_GPU_REG_GREEN));
}

unsigned char AuraGPUController::GetLEDBlue()
{
    return(AuraGPURegisterRead(AURA_GPU_REG_BLUE));
}

void AuraGPUController::SetLEDColors(unsigned char red, unsigned char green, unsigned char blue)
{
    AuraGPURegisterWrite(AURA_GPU_REG_RED, red);
    AuraGPURegisterWrite(AURA_GPU_REG_GREEN, green);
    AuraGPURegisterWrite(AURA_GPU_REG_BLUE, blue);
}

void AuraGPUController::SetMode(unsigned char mode)
{
    AuraGPURegisterWrite(AURA_GPU_REG_MODE, mode);
}

unsigned char AuraGPUController::AuraGPURegisterRead(unsigned char reg)
{
    return(bus->i2c_smbus_read_byte_data(dev, reg));
}

void AuraGPUController::AuraGPURegisterWrite(unsigned char reg, unsigned char val)
{
    bus->i2c_smbus_write_byte_data(dev, reg, val);
}

bool AuraGPUController::SaveOnlyApplies()
{
    switch (bus->pci_subsystem_device)
    {
        case ASUS_VEGA64_STRIX:
            return false;
    }
    // Behavior on other GPU models is unknown and needs to be tested.
    // Assume the safest option to prevent damaage from excessive writes.
    return false;
}

void AuraGPUController::Save()
{
    AuraGPURegisterWrite(AURA_GPU_REG_APPLY, AURA_GPU_APPLY_VAL);
}
