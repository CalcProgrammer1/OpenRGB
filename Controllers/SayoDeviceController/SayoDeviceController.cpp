/*---------------------------------------------------------*\
| SayoDeviceController.cpp                                  |
|                                                           |
|   Controller for Sayo Devices (USB HID)                   |
|                                                           |
|   Richard Harris                              24 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "LogManager.h"
#include "SayoDeviceController.h"
#include "RGBController.h"
#include "Colors.h"

SayoDeviceController::SayoDeviceController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

SayoDeviceController::~SayoDeviceController()
{
    hid_close(dev);
}

std::string SayoDeviceController::GetDeviceLocation()
{
    return location;
}

std::string SayoDeviceController::GetDeviceName()
{
    return name;
}

void SayoDeviceController::SetMode(unsigned int mode, unsigned int speed, RGBColor color, bool random)
{
    /*-----------------------------------------------------*\
    | Loop color table creates a rainbow effect which looks |
    | better than truly random colors, so use that.         |
    \*-----------------------------------------------------*/
    unsigned int color_mode = random ? SAYO_COLOR_LOOP_TABLE : SAYO_COLOR_STATIC;
    unsigned char mode_byte = SAYO_MODE_PACK(speed, color_mode, mode);

    /*-----------------------------------------------------*\
    | 0x1C (0x00) is potentially length (28) as the payload |
    | is 27 bytes in size. Could include a zero as many     |
    | fields seem to be 2-byte.                             |
    | Other fields have not been mapped out at this time.   |
    \*-----------------------------------------------------*/
    std::vector<unsigned char> payload =
    {
        0x1C, 0x00, SAYO_CMD_LIGHTING_SET, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x15, 0x00, 0x28, 0x00, 0x26, 0x00, 0x4C, 0x00,
        0x26, 0x00, 0x00, 0x00, mode_byte, 0x00, 0x80, 0x80,
        (unsigned char)RGBGetRValue(color),
        (unsigned char)RGBGetGValue(color),
        (unsigned char)RGBGetBValue(color),
    };
    SendPacket(payload, true);
}

/*---------------------------------------------------------*\
| Persist LED settings to flash memory.                     |
\*---------------------------------------------------------*/
void SayoDeviceController::Save()
{
    /*-----------------------------------------------------*\
    | 0x06 0x00 feels like a length too - 6 bytes           |
    \*-----------------------------------------------------*/
    std::vector<unsigned char> payload = { 0x06, 0x00, 0x0d, 0x00, 0x96, 0x72 };
    SendPacket(payload, true);
}

void SayoDeviceController::SendPacket(const std::vector<unsigned char>& command, bool flush)
{
    unsigned char length = (unsigned char)(command.size() + 4);

    if(length > 64)
    {
        LOG_ERROR("[SayoDevice] SendPacket: command size (%d) is too large, rejecting", (int)command.size());
        return;
    }

    std::vector<unsigned char> packet = {0x21, 0x12};

    /*-----------------------------------------------------*\
    | Checksum of header + command, as little endian 2-byte |
    | pairs. Checksum is in same format.                    |
    \*-----------------------------------------------------*/
    unsigned short checksum = (unsigned short)(packet[0] | (packet[1] << 8));
    for(std::size_t i = 0; i < command.size(); i += 2)
    {
        unsigned short word = command[i];

        if(i + 1 < command.size())
        {
            word |= (unsigned short)(command[i + 1] << 8);
        }

        checksum = (checksum + word) & 0xFFFF;
    }

    packet.push_back(checksum & 0xFF);
    packet.push_back(checksum >> 8);
    packet.insert(packet.end(), command.begin(), command.end());
    packet.resize(64, 0u);

    hid_write(dev, packet.data(), packet.size());

    if(flush)
    {
        /*-------------------------------------------------*\
        | Flush away any awaiting IN packets                |
        \*-------------------------------------------------*/
        unsigned char buf[64];

        int res = 1;
        while(res > 0)
        {
            res = hid_read_timeout(dev, buf, 64, 0);
        }
    }
}
