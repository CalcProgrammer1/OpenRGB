/*---------------------------------------------------------*\
| EVGAACX30SMBusController.cpp                              |
|                                                           |
|   Driver for SMBus EVGA ACX 30 motherboards               |
|                                                           |
|   Balázs Triszka (balika011)                  21 May 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "EVGAACX30SMBusController.h"
#include "dmiinfo.h"
#include "LogManager.h"

using namespace std::chrono_literals;

EVGAACX30SMBusController::EVGAACX30SMBusController(i2c_smbus_interface *bus, uint8_t dev)
{
    this->bus = bus;
    this->dev = dev;

    DMIInfo dmi;

    device_name =   "EVGA " + dmi.getMainboard();
}

EVGAACX30SMBusController::~EVGAACX30SMBusController()
{

}

std::string EVGAACX30SMBusController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string EVGAACX30SMBusController::GetDeviceName()
{
    return(device_name);
}

std::string EVGAACX30SMBusController::GetFirmwareVersion()
{
    uint16_t version = bus->i2c_smbus_read_byte_data(dev, ACX30_REG_VER_HIGH) << 8 | bus->i2c_smbus_read_byte_data(dev, ACX30_REG_VER_LOW);
    uint8_t ptype = bus->i2c_smbus_read_byte_data(dev, ACX30_REG_PTYPE);

    char ver[9];
    snprintf(ver, 9, "0x%X", version);
    char pt[9];
    snprintf(pt, 9, "0x%X", ptype);

    std::string return_string;
    return_string.append(ver);
    return_string.append(", ptype ");
    return_string.append(pt);
    return return_string;
}

uint8_t EVGAACX30SMBusController::GetMode()
{
    return bus->i2c_smbus_read_byte_data(dev, ACX30_REG_MODE);
}

void EVGAACX30SMBusController::Unlock()
{
    bus->i2c_smbus_write_byte_data(dev, ACX30_REG_CONTROL, 0xE5);
    bus->i2c_smbus_write_byte_data(dev, ACX30_REG_CONTROL, 0xE9);
    bus->i2c_smbus_read_byte_data(dev, ACX30_REG_CONTROL);
}

void EVGAACX30SMBusController::Lock()
{
    bus->i2c_smbus_write_byte_data(dev, ACX30_REG_CONTROL, 0xE0);
    bus->i2c_smbus_read_byte_data(dev, ACX30_REG_CONTROL);
}

void EVGAACX30SMBusController::SetColors(uint8_t red, uint8_t green, uint8_t blue)
{
    bus->i2c_smbus_write_byte_data(dev, ACX30_REG_RED, red);
    bus->i2c_smbus_write_byte_data(dev, ACX30_REG_GREEN, green);
    bus->i2c_smbus_write_byte_data(dev, ACX30_REG_BLUE, blue);
}

void EVGAACX30SMBusController::SetMode(uint8_t mode)
{
    if (mode == ACX30_MODE_OFF)
    {
        bus->i2c_smbus_write_byte_data(dev, ACX30_REG_21, 0xE7);
        bus->i2c_smbus_write_byte_data(dev, ACX30_REG_22, 0xCE);
    }
    else
    {
        bus->i2c_smbus_write_byte_data(dev, ACX30_REG_21, 0xE5);
        bus->i2c_smbus_write_byte_data(dev, ACX30_REG_22, 0xE7);
    }

    bus->i2c_smbus_write_byte_data(dev, ACX30_REG_MODE, mode);
}

void EVGAACX30SMBusController::SetSpeed(uint8_t speed)
{
    bus->i2c_smbus_write_byte_data(dev, ACX30_REG_SPEED, speed);
}
