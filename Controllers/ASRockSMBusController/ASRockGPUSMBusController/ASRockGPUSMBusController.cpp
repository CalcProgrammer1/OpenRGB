/*---------------------------------------------------------*\
| ASRockGPUSMBusController.cpp                              |
|                                                           |
|   Driver for ASRock GPU RGB controllers                   |
|                                                           |
|   Based on SignalRGB ASRock GPU.js plugin                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "ASRockGPUSMBusController.h"
#include "LogManager.h"
#include <chrono>
#include <cstdio>
#include <cstring>
#include <thread>

using namespace std::chrono_literals;

ASRockGPUSMBusController::ASRockGPUSMBusController(i2c_smbus_interface* bus, asrock_gpu_dev_id dev, std::string name)
{
    this->bus          = bus;
    this->dev          = dev;
    this->device_name  = name;
    this->num_channels = 0;

    memset(enabled_channels, 0, sizeof(enabled_channels));
    memset(led_counts, 0, sizeof(led_counts));
}

ASRockGPUSMBusController::~ASRockGPUSMBusController()
{
}

std::string ASRockGPUSMBusController::GetDeviceLocation()
{
    std::string return_string(bus->info.device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string ASRockGPUSMBusController::GetDeviceName()
{
    return(device_name);
}

bool ASRockGPUSMBusController::WriteBlock(uint8_t command, uint8_t subcommand, const uint8_t* data, uint8_t len)
{
    /*-----------------------------------------------------*\
    | SignalRGB packet format                              |
    | [command, 0, subcommand, data...]                    |
    | Uses pure I2C rather than SMBus                      |
    \*-----------------------------------------------------*/
    uint8_t packet[12];
    memset(packet, 0, sizeof(packet));

    packet[0] = command;
    packet[1] = 0x00;
    packet[2] = subcommand;

    if(data != nullptr && len > 0)
    {
        uint8_t copy_len = (len > 9) ? 9 : len;
        memcpy(&packet[3], data, copy_len);
    }

    int size   = 12;
    int result = bus->i2c_write_block(dev, size, packet);

    if(result < 0)
    {
        LOG_DEBUG("[ASRock GPU] Write block failed: %d", result);
        return(false);
    }

    return(true);
}

bool ASRockGPUSMBusController::ReadBlock(uint8_t* response, uint8_t len)
{
    memset(response, 0, len);
    int size   = len;
    int result = bus->i2c_read_block(dev, &size, response);

    if(result < 0)
    {
        LOG_DEBUG("[ASRock GPU] Read block failed: %d", result);
        return(false);
    }

    return(true);
}

bool ASRockGPUSMBusController::DetectChannels()
{
    /*-----------------------------------------------------*\
    | Get enabled channels                                 |
    | Command 0x14, subcommand 0x01                        |
    \*-----------------------------------------------------*/
    if(!WriteBlock(0x14, 0x01, nullptr, 0))
    {
        LOG_WARNING("[ASRock GPU] Failed to request enabled channels");
        return(false);
    }

    std::this_thread::sleep_for(20ms);

    uint8_t response[I2C_SMBUS_BLOCK_MAX];
    if(!ReadBlock(response, sizeof(response)))
    {
        LOG_WARNING("[ASRock GPU] Failed to read enabled channels");
        return(false);
    }

    LOG_TRACE("[ASRock GPU] Enabled channels response: %02X %02X %02X %02X %02X %02X", response[0], response[1], response[2], response[3], response[4], response[5]);

    /*-----------------------------------------------------*\
    | Response format                                      |
    | Bytes 4-5 contain the channel bitmap                 |
    \*-----------------------------------------------------*/
    uint16_t channel_bits = response[4] | (response[5] << 8);
    LOG_TRACE("[ASRock GPU] Channel bits: 0x%04X", channel_bits);

    /*-----------------------------------------------------*\
    | Get LED counts                                       |
    | Command 0x14, subcommand 0x02                        |
    \*-----------------------------------------------------*/
    if(!WriteBlock(0x14, 0x02, nullptr, 0))
    {
        LOG_WARNING("[ASRock GPU] Failed to request LED counts");
        return(false);
    }

    std::this_thread::sleep_for(20ms);

    if(!ReadBlock(response, sizeof(response)))
    {
        LOG_WARNING("[ASRock GPU] Failed to read LED counts");
        return(false);
    }

    num_channels = 0;
    for(int i = 0; i < 16; i++)
    {
        if(channel_bits & (1 << i))
        {
            enabled_channels[num_channels] = i;
            led_counts[num_channels]       = response[4 + i];

            const char* name = "Unknown";
            if(i == ASROCK_GPU_CHANNEL_ARGB_HEADER)
            {
                name = "ARGB Header";
            }
            else if(i == ASROCK_GPU_CHANNEL_TOP_SIDE)
            {
                name = "Top Side";
            }
            else if(i == ASROCK_GPU_CHANNEL_FAN)
            {
                name = "Fan";
            }

            LOG_DEBUG("[ASRock GPU] Channel %d (%s) active with %d LEDs", i, name, response[4 + i]);
            num_channels++;
        }
    }

    LOG_DEBUG("[ASRock GPU] Found %d active channels", num_channels);
    return(num_channels > 0);
}

void ASRockGPUSMBusController::SetChannelColor(uint8_t channel_idx, uint8_t red, uint8_t green, uint8_t blue)
{
    /*-----------------------------------------------------*\
    | SignalRGB color packet                               |
    | Command 0x10, subcommand is the channel index        |
    \*-----------------------------------------------------*/
    uint8_t data[8] =
    {
        0x01,  // Mode: direct/static
        red,
        green,
        blue,
        0x80,  // Brightness
        0xFF,  // Effect speed
        0x00,  // Effect direction
        0x1A   // Unknown constant
    };

    if(!WriteBlock(0x10, channel_idx, data, sizeof(data)))
    {
        LOG_WARNING("[ASRock GPU] Failed to set color for channel %d", channel_idx);
    }
}
