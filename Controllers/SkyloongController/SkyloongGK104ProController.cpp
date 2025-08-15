/*---------------------------------------------------------*\
| SkyloongGK104ProController.cpp                            |
|                                                           |
|   Driver for Skyloong GK104 Pro                           |
|                                                           |
|   Givo (givowo)                               30 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "SkyloongGK104ProController.h"

using namespace std::chrono_literals;

enum command
{
    ping        = 0x0C,
    mode        = 0xB,
    le_define   = 0x1A
};

SkyloongGK104ProController::SkyloongGK104ProController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    SendCommand(command::ping, SUBCOMMAND_NONE);
    SendCommand(command::mode, MODE_ONLINE);
    SendCommand(command::ping, SUBCOMMAND_NONE);
}

SkyloongGK104ProController::~SkyloongGK104ProController()
{
    SendCommand(command::mode, MODE_OFFLINE);
    hid_close(dev);
}

std::string SkyloongGK104ProController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string SkyloongGK104ProController::GetDeviceName()
{
    return(name);
}

void SkyloongGK104ProController::SendCommand(char command, char sub_command)
{
    unsigned char buf[PACKET_SIZE];
    memset(buf, 0x00, PACKET_SIZE);

    buf[0x01]       = command;
    buf[0x02]       = sub_command;

    uint16_t crc    = Crc16CcittFalse(buf, PACKET_SIZE);

    buf[0x07]       = crc & 0xFF;
    buf[0x08]       = crc >> 8;

    hid_write(dev, buf, PACKET_SIZE);
}

void SkyloongGK104ProController::SendColorPacket(std::vector<RGBColor> colors, std::vector<led> *leds, int brightness)
{
    unsigned char le_data[TOTAL_LED_BYTES];
    memset(le_data, 0x00, TOTAL_LED_BYTES);

    for(unsigned int i = 0; i < leds->size(); i++)
    {
        int index           = leds->at(i).value * 4;
        le_data[index++]    = RGBGetRValue(colors[i]);
        le_data[index++]    = RGBGetGValue(colors[i]);
        le_data[index++]    = RGBGetBValue(colors[i]);
        le_data[index++]    = brightness;
    }

    for(int n = 0; n < TOTAL_LED_BYTES; n += LED_BYTES_IN_CHUNK) {
        if(n + LED_BYTES_IN_CHUNK <= TOTAL_LED_BYTES)
        {
            SetLEDefine(n, &le_data[n], LED_BYTES_IN_CHUNK);
        }
        else
        {
            SetLEDefine(n, &le_data[n], TOTAL_LED_BYTES - n);
        }
    }

    SendCommand(command::le_define, LE_DEFINE_SAVE);
}

void SkyloongGK104ProController::SetLEDefine(int address, unsigned char *le_data, int le_data_length)
{
    unsigned char buf[PACKET_SIZE];
    memset(buf, 0x00, PACKET_SIZE);

    buf[0x01]       = command::le_define;
    buf[0x02]       = LE_DEFINE_SET;

    int header      = (address + ((le_data_length << 24) & 0xFF000000)) | 0;
    buf[0x03]       = header & 0xFF;
    buf[0x04]       = (header >> 8) & 0xFF;
    buf[0x05]       = (header >> 16) & 0xFF;
    buf[0x06]       = (header >> 24) & 0xFF;

    std::copy(le_data, le_data + le_data_length, buf + 9);

    uint16_t crc    = Crc16CcittFalse(buf, PACKET_SIZE);
    buf[0x07]       = crc & 0xFF;
    buf[0x08]       = crc >> 8;

    hid_write(dev, buf, PACKET_SIZE);
}

uint16_t SkyloongGK104ProController::Crc16CcittFalse(const uint8_t *buffer, uint16_t size)
{
    uint16_t crc = 0xFFFF;

    while(size--)
    {
        crc ^= (*buffer++ << 8);

        for(uint8_t i = 0; i < 8; ++i)
        {
            if(crc & 0x8000)
            {
                crc = (crc << 1) ^ 0x1021;
            }
            else
            {
                crc = crc << 1;
            }
        }
    }

    return crc;
}
