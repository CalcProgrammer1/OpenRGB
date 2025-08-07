/*---------------------------------------------------------*\
| EVGAGP102Controller.cpp                                   |
|                                                           |
|   Driver for EVGA GP102 GPU                               |
|                                                           |
|   Fabricio Murta (avengerx)                   31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "EVGAGP102Controller.h"
#include "LogManager.h"

EVGAGP102Controller::EVGAGP102Controller(i2c_smbus_interface* bus_ptr, zoneinfo info, std::string dev_name)
{
    bus     = bus_ptr;
    zi      = info;
    name    = dev_name;
}

EVGAGP102Controller::~EVGAGP102Controller()
{
}

std::string EVGAGP102Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", zi.dev_addr);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string EVGAGP102Controller::GetDeviceName()
{
    return(name);
}

std::string EVGAGP102Controller::GetZoneName()
{
    return(zi.zone_name);
}

void EVGAGP102Controller::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    SendCommand(EVGA_GP102_CMD_BEGIN);
    SendCommand(EVGA_GP102_CMD_COLOR);

    if (CommandAcknowledged())
    {
        unsigned char rgb[] = { red, green, blue };

        for (int i = 0; i < 3; i++)
        {
            bus->i2c_smbus_write_byte_data(zi.dev_addr, zi.color_addrs[i], rgb[i]);
        }

        SendCommand(EVGA_GP102_CMD_END);

        if (!CommandCompleted())
        {
            LOG_WARNING("[%s] Non-clear status report from hardware.", EVGA_GP102_CONTROLLER_NAME);
        }
    }
}
std::array<unsigned char, 3> EVGAGP102Controller::GetColor()
{
    return { GetRed(), GetGreen(), GetBlue() };
}

bool EVGAGP102Controller::IsValid()
{
    for (int i = 0; i < 3; i++)
    {
        unsigned char res = bus->i2c_smbus_read_byte_data(zi.dev_addr, EVGA_GP102_REG_VALID);
        if (res == 0x1F || res == 0x91)
        {
            LOG_TRACE("[%s] Zone discovery successful on address: 0x%02X.", EVGA_GP102_CONTROLLER_NAME, zi.dev_addr);
            return true;
        }
        LOG_DEBUG("[%s] Zone discovery failed on address: 0x%02X expected: 0x1F received: 0x%02X.", EVGA_GP102_CONTROLLER_NAME, zi.dev_addr, res);
    }
    return false;
}

void EVGAGP102Controller::SetMode(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(zi.dev_addr, EVGA_GP102_REG_MODE, mode);
}

unsigned char EVGAGP102Controller::GetMode()
{
    return(bus->i2c_smbus_read_byte_data(zi.dev_addr, EVGA_GP102_REG_MODE));
}

void EVGAGP102Controller::SendCommand(s32 command)
{
    bus->i2c_smbus_write_byte_data(zi.dev_addr, EVGA_GP102_REG_CMD, command);
}

s32 EVGAGP102Controller::QueryCommand(s32 command)
{
    return bus->i2c_smbus_read_byte_data(zi.dev_addr, command);
}

bool EVGAGP102Controller::CommandAcknowledged()
{
    return QueryCommand(EVGA_GP102_REG_CMD) == zi.resp_ready;
}

bool EVGAGP102Controller::CommandCompleted()
{
    return QueryCommand(EVGA_GP102_REG_CMD) == zi.resp_clear;
}

unsigned char EVGAGP102Controller::GetRed()
{
    return(bus->i2c_smbus_read_byte_data(zi.dev_addr, zi.color_addrs[EVGA_GP102_CIDX_RED]));
}

unsigned char EVGAGP102Controller::GetGreen()
{
    return(bus->i2c_smbus_read_byte_data(zi.dev_addr, zi.color_addrs[EVGA_GP102_CIDX_GREEN]));
}

unsigned char EVGAGP102Controller::GetBlue()
{
    return(bus->i2c_smbus_read_byte_data(zi.dev_addr, zi.color_addrs[EVGA_GP102_CIDX_BLUE]));
}

void EVGAGP102Controller::SaveSettings()
{
    //Tested and not worked
    //bus->i2c_smbus_write_byte_data(zi.dev_addr, 0x21,   0xE5);
    //bus->i2c_smbus_write_byte_data(zi.dev_addr, 0x22,   0xE7);
}
