/*---------------------------------------------------------*\
| ArcticController.cpp                                      |
|                                                           |
|   Driver for Arctic devices                               |
|                                                           |
|   Armin Wolf (Wer-Wolf)                       09 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "ArcticController.h"

using namespace std::chrono_literals;

#define ARCTIC_COMMAND_SET_RGB          0x00
#define ARCTIC_COMMAND_IDENTIFY         0x5C

#define ARCTIC_RESPONSE_BUFFER_LENGTH   15
#define ARCTIC_RESPONSE_COMMAND_OFFSET  0
#define ARCTIC_RESPONSE_DATA_OFFSET     1
#define ARCTIC_RESPONSE_DATA_LENGTH     12
#define ARCTIC_RESPONSE_XOR_CSUM_OFFSET 13
#define ARCTIC_RESPONSE_ADD_CSUM_OFFSET 14

#define ARCTIC_COMMAND_BUFFER_LENGTH(payload_size)  (sizeof(header) + 1 + payload_size)
#define ARCTIC_COMMAND_COMMAND_OFFSET               (sizeof(header))
#define ARCTIC_COMMAND_PAYLOAD_OFFSET               (sizeof(header) + 1)

const unsigned char header[] =
{
    0x01,
    0x02,
    0x03,
    0xFF,
    0x05,
    0xFF,
    0x02,
    0x03
};

const unsigned char identify_payload[] =
{
    0x01,
    0xFE,
    0x01,
    0xFE
};

ArcticController::ArcticController(const std::string &portname)
: serialport(portname.c_str(), 250000, SERIAL_PORT_PARITY_NONE, SERIAL_PORT_SIZE_8, SERIAL_PORT_STOP_BITS_2, false)
{
    port_name = portname;
    serialport.serial_set_dtr(true);
}

ArcticController::~ArcticController()
{
    serialport.serial_set_dtr(false);
}

static void FormatCommandBuffer(char *buffer, char command)
{
    std::memcpy(buffer, header, sizeof(header));
    buffer[ARCTIC_COMMAND_COMMAND_OFFSET] = command;
}

void ArcticController::SetChannels(std::vector<RGBColor> colors)
{
    char* buffer = new char[ARCTIC_COMMAND_BUFFER_LENGTH(colors.size() * 3)];

    FormatCommandBuffer(buffer, ARCTIC_COMMAND_SET_RGB);

    for(unsigned int channel = 0; channel < colors.size(); channel++)
    {
        const unsigned int offset = ARCTIC_COMMAND_PAYLOAD_OFFSET + channel * 3;

        buffer[offset + 0x00] = (char)std::min<unsigned int>(254, RGBGetRValue(colors[channel]));
        buffer[offset + 0x01] = (char)std::min<unsigned int>(254, RGBGetGValue(colors[channel]));
        buffer[offset + 0x02] = (char)std::min<unsigned int>(254, RGBGetBValue(colors[channel]));
    }

    serialport.serial_write(buffer, sizeof(buffer));

    delete[] buffer;
}

static char XORChecksum(char *data, int length)
{
    char sum = 0;

    for(int i = 0; i < length; i++)
    {
        sum ^= data[i];
    }

    return sum;
}

static char AddChecksum(char *data, int length)
{
    char sum = 0;

    for(int i = 0; i < length; i++)
    {
        sum = (char)(sum + data[i]);
    }

    return sum;
}

bool ArcticController::IsPresent()
{
    char buffer[ARCTIC_COMMAND_BUFFER_LENGTH(sizeof(identify_payload))];
    char response[ARCTIC_RESPONSE_BUFFER_LENGTH];
    int ret;

    FormatCommandBuffer(buffer, ARCTIC_COMMAND_IDENTIFY);
    std::memcpy(buffer + ARCTIC_COMMAND_PAYLOAD_OFFSET, identify_payload, sizeof(identify_payload));

    serialport.serial_flush_rx();
    ret = serialport.serial_write(buffer, sizeof(buffer));
    if(ret != sizeof(buffer))
    {
        return false;
    }

    std::this_thread::sleep_for(100ms);

    ret = serialport.serial_read(response, sizeof(response));
    if(ret != sizeof(response))
    {
        return false;
    }

    if(response[ARCTIC_RESPONSE_COMMAND_OFFSET] != ARCTIC_COMMAND_IDENTIFY)
    {
        return false;
    }

    if(response[ARCTIC_RESPONSE_XOR_CSUM_OFFSET] != XORChecksum(&response[ARCTIC_RESPONSE_DATA_OFFSET], ARCTIC_RESPONSE_DATA_LENGTH))
    {
        return false;
    }

    if(response[ARCTIC_RESPONSE_ADD_CSUM_OFFSET] != AddChecksum(&response[ARCTIC_RESPONSE_DATA_OFFSET], ARCTIC_RESPONSE_DATA_LENGTH))
    {
        return false;
    }

    return true;
}

std::string ArcticController::GetLocation()
{
    return port_name;
}
