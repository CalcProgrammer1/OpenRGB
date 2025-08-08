/*---------------------------------------------------------*\
| EVGAMouseController.cpp                                   |
|                                                           |
|   Driver for EVGA mouse                                   |
|                                                           |
|   Cooper Knaak                                23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include "EVGAMouseController.h"
#include "LogManager.h"
#include "StringUtils.h"

#define HID_MAX_STR 255
#define EVGA_PERIPHERAL_LED_SOURCE_OF_TRUTH EVGA_PERIPHERAL_LED_LOGO
/*----------------------------------------------------------------*\
| Maximum number of attempts to read from a device before failing. |
\*----------------------------------------------------------------*/
#define EVGA_PERIPHERAL_MAX_ATTEMPTS 100
/*-----------------------------------------------------------------*\
| The delay between sending packets to the device in wireless mode. |
| In wireless mode, sending packets too close to each other causes  |
| them to have no effect, despite the device responding properly.   |
\*-----------------------------------------------------------------*/
#define EVGA_PERIPHERAL_PACKET_DELAY std::chrono::milliseconds(10)

/*--------------------------------------------------------------------------------*\
| Returns true if both buffers have equal bytes at each position, false otherwise. |
| Each buffer must be an array of bytes at least size bytes long.                  |
\*--------------------------------------------------------------------------------*/
static bool BuffersAreEqual(unsigned char *buffer1, unsigned char *buffer2, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(buffer1[i] != buffer2[i])
        {
            return false;
        }
    }
    return true;
}

EVGAMouseController::EVGAMouseController(hid_device* dev_handle, char * path, int connection_type, std::string dev_name)
{
    dev                     = dev_handle;
    location                = path;
    name                    = dev_name;
    this->connection_type   = connection_type;

    led_states.resize(EVGA_PERIPHERAL_LED_COUNT);
    for(EVGAMouseControllerDeviceState &led_state : led_states)
    {
        led_state.mode          = EVGA_PERIPHERAL_MODE_STATIC;
        led_state.brightness    = 255;
        led_state.speed         = 100;
        led_state.colors.resize(1);
        led_state.colors[0]     = ToRGBColor(255, 255, 255);
    }
}

EVGAMouseController::~EVGAMouseController()
{
    hid_close(dev);
}

std::string EVGAMouseController::GetName()
{
    return(name);
}

std::string EVGAMouseController::GetSerial()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_indexed_string(dev, 2, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string EVGAMouseController::GetLocation()
{
    return location;
}

uint8_t EVGAMouseController::GetMode()
{
    return GetState().mode;
}

EVGAMouseControllerDeviceState EVGAMouseController::GetState()
{
    RefreshDeviceState(EVGA_PERIPHERAL_LED_SOURCE_OF_TRUTH);
    return led_states[EVGA_PERIPHERAL_LED_SOURCE_OF_TRUTH];
}

RGBColor EVGAMouseController::GetColorOfLed(int led)
{
    RefreshDeviceState(led);
    return led_states[led].colors[0];
}

void EVGAMouseController::SetMode(uint8_t mode, uint8_t index)
{
    unsigned char buffer[EVGA_PERIPHERAL_PACKET_SIZE] =
    {
        0x00,                   /* report id - must be 0x00 according to hid_send_feature_report */
        0x00, 0x00, 0x00, 0x1D, /* header bits - always the same */
        0x02, 0x81, 0x01        /* 0x81 sets the mode, which is specified below. */
    };

    buffer[EVGA_PERIPHERAL_LED_INDEX_BYTE]  = index;
    buffer[EVGA_PERIPHERAL_MODE_BYTE]       = mode;
    int err                                 = hid_send_feature_report(dev, buffer, EVGA_PERIPHERAL_PACKET_SIZE);
    if(err == -1)
    {
        const wchar_t* err_str = hid_error(dev);
        LOG_DEBUG("[%s] Error writing buffer %s", name.c_str(), err_str);
    }
    led_states[index].mode  = mode;
    err                     = hid_get_feature_report(dev, buffer, EVGA_PERIPHERAL_PACKET_SIZE);
    if(err == -1)
    {
        const wchar_t* err_str = hid_error(dev);
        LOG_DEBUG("[%s] Error reading buffer %s", name.c_str(), err_str);
    }
}

void EVGAMouseController::SetLed(uint8_t index, uint8_t brightness, uint8_t speed, RGBColor color)
{
    std::vector<RGBColor> colors;
    colors.push_back(color);

    SetLed(index, brightness, speed, colors, false);
}

void EVGAMouseController::SetLed(uint8_t index, uint8_t brightness, uint8_t speed, const std::vector<RGBColor>& colors)
{
    SetLed(index, brightness, speed, colors, false);
}

void EVGAMouseController::SetLedAndActivate(uint8_t index, uint8_t brightness, uint8_t speed, const std::vector<RGBColor>& colors)
{
    /*------------------------------------------------------------------------------------------------------------------------------*\
    | Activating some modes requires two identical packets: one for setting the color, and one for setting the color AND activating. |
    \*------------------------------------------------------------------------------------------------------------------------------*/
    SetLed(index, brightness, speed, colors, false);
    SetLed(index, brightness, speed, colors, true);
}

void EVGAMouseController::SetAllLeds(uint8_t brightness, uint8_t speed, const std::vector<RGBColor>& colors)
{
    for(unsigned int i = 0; i < EVGA_PERIPHERAL_LED_COUNT; i++)
    {
        SetLed(i, brightness, speed, colors);
    }
}

void EVGAMouseController::SetAllLedsAndActivate(uint8_t brightness, uint8_t speed, const std::vector<RGBColor>& colors)
{
    for(unsigned int i = 0; i < EVGA_PERIPHERAL_LED_COUNT; i++)
    {
        SetLedAndActivate(i, brightness, speed, colors);
    }
}

void EVGAMouseController::SetLed(uint8_t index, uint8_t brightness, uint8_t speed, const std::vector<RGBColor>& colors, bool activate)
{
    unsigned char buffer[EVGA_PERIPHERAL_PACKET_SIZE] =
    {
        0x00,               /* report id - must be 0x00 according to hid_send_feature_report */
        0x00, 0x00, static_cast<unsigned char>(connection_type), 0x1D,
        0x02, 0x00, 0x02    /* header bits - always the same */
    };

    /*---------------------------------------------------------------------------------------------------------------*\
    | Setting the mode to breathing sends 3 packets: first to activate the mode, second to set the list of colors and |
    | third to send a packet identical to the second but with the first byte set ot 0xA1. This "activates" the mode.  |
    \*---------------------------------------------------------------------------------------------------------------*/
    if(activate)
    {
        buffer[1] = 0xA1;
    }

    buffer[EVGA_PERIPHERAL_LED_INDEX_BYTE]  = index;
    /*-----------------------------------------------------------------------------------------*\
    | Unleash RGB supports individual modes on the LEDs, but OpenRGB does not. Use one specific |
    | LED's mode for any LED.                                                                   |
    \*-----------------------------------------------------------------------------------------*/
    buffer[EVGA_PERIPHERAL_MODE_BYTE]       = led_states[EVGA_PERIPHERAL_LED_SOURCE_OF_TRUTH].mode;
    buffer[EVGA_PERIPHERAL_BRIGHTNESS_BYTE] = brightness;
    buffer[EVGA_PERIPHERAL_SPEED_BYTE]      = speed;

    /*-----------------------------------------------------------------------*\
    | 7 is the maximum number of colors that can be set from the vendor's UI. |
    \*-----------------------------------------------------------------------*/
    unsigned char color_count                   = (unsigned char)std::min(colors.size(), static_cast<std::vector<RGBColor>::size_type>(7));
    buffer[EVGA_PERIPHERAL_COLOR_COUNT_BYTE]    = color_count;
    for(unsigned char i = 0; i < color_count; i++)
    {
        buffer[15 + i * 3] = RGBGetRValue(colors[i]);
        buffer[16 + i * 3] = RGBGetGValue(colors[i]);
        buffer[17 + i * 3] = RGBGetBValue(colors[i]);
    }
    int err = hid_send_feature_report(dev, buffer, EVGA_PERIPHERAL_PACKET_SIZE);
    if(err == -1)
    {
        const wchar_t* err_str = hid_error(dev);
        LOG_DEBUG("[%s] Error writing buffer %s", name.c_str(), err_str);
    }
    led_states[index].brightness    = brightness;
    led_states[index].speed         = speed;
    led_states[index].colors        = colors;
    /*------------------------------------------------------------------------------------*\
    | If the device returns a response not ready packet, future writes will silently fail. |
    | Wait until the device sends a valid packet to proceed.                               |
    \*------------------------------------------------------------------------------------*/
    ReadPacketOrLogErrors(buffer, EVGA_PERIPHERAL_MAX_ATTEMPTS);
}

void EVGAMouseController::RefreshDeviceState()
{
    RefreshDeviceState(EVGA_PERIPHERAL_LED_FRONT);
    RefreshDeviceState(EVGA_PERIPHERAL_LED_WHEEL);
    RefreshDeviceState(EVGA_PERIPHERAL_LED_LOGO);
}

void EVGAMouseController::RefreshDeviceState(int led)
{
    unsigned char buffer[EVGA_PERIPHERAL_PACKET_SIZE] =
    {
        0x00,
        0x00, 0x00, static_cast<unsigned char>(connection_type), 0x1D,
        0x02, 0x80, 0x02
    };
    buffer[EVGA_PERIPHERAL_LED_INDEX_BYTE]  = static_cast<unsigned char>(led);
    int err                                 = hid_send_feature_report(dev, buffer, EVGA_PERIPHERAL_PACKET_SIZE);
    if(err == -1)
    {
        const wchar_t* err_str = hid_error(dev);
        LOG_DEBUG("[%s] Error writing buffer %s", name.c_str(), err_str);
    }
    /*------------------------------------------------------------------------------*\
    | Wait in wireless mode or else packets might be sent too quickly to take effect |
    \*------------------------------------------------------------------------------*/
    Wait();
    if(ReadPacketOrLogErrors(buffer, EVGA_PERIPHERAL_MAX_ATTEMPTS))
    {
        int color_count = buffer[EVGA_PERIPHERAL_COLOR_COUNT_BYTE];
        if(color_count == 0)
        {
            LOG_VERBOSE("[%s] No colors read from response. The device is likely asleep.", name.c_str());
            return;
        }
        led_states[led].mode        = buffer[EVGA_PERIPHERAL_MODE_BYTE];
        led_states[led].brightness  = buffer[EVGA_PERIPHERAL_BRIGHTNESS_BYTE];
        led_states[led].speed       = buffer[EVGA_PERIPHERAL_SPEED_BYTE];
        led_states[led].colors.resize(std::max(color_count, 1));
        for(int i = 0; i < color_count; i++)
        {
            uint8_t r                   = buffer[EVGA_PERIPHERAL_RED_BYTE + i * 3];
            uint8_t g                   = buffer[EVGA_PERIPHERAL_GREEN_BYTE + i * 3];
            uint8_t b                   = buffer[EVGA_PERIPHERAL_BLUE_BYTE + i * 3];
            led_states[led].colors[i]   = ToRGBColor(r, g, b);
        }
    }
}

bool EVGAMouseController::ReadPacketOrLogErrors(unsigned char *buffer, int max_attempts)
{
    int bytes_read = ReadPacketOrWait(buffer, max_attempts);
    if(bytes_read == -1)
    {
        const wchar_t* err_str = hid_error(dev);
        LOG_DEBUG("[%s] Error reading buffer %s", name.c_str(), err_str);
        return false;
    }
    else if(IsResponseNotReadyPacket(buffer))
    {
        LOG_VERBOSE("[%s] Retries exhausted reading from device. Write may have failed.", name.c_str());
        return false;
    }
    else if(IsAsleepPacket(buffer))
    {
        LOG_VERBOSE("[%s] Device is asleep. Cannot send or receive packets until the device is awoken.", name.c_str());
        return false;
    }
    return true;
}

int EVGAMouseController::ReadPacketOrWait(unsigned char *buffer, int max_attempts)
{
    int attempts    = 1;
    Wait();
    int bytes_read  = hid_get_feature_report(dev, buffer, EVGA_PERIPHERAL_PACKET_SIZE);
    while(bytes_read == EVGA_PERIPHERAL_PACKET_SIZE && attempts < max_attempts && IsResponseNotReadyPacket(buffer))
    {
        Wait();
        bytes_read = hid_get_feature_report(dev, buffer, EVGA_PERIPHERAL_PACKET_SIZE);
        attempts++;
    }
    return bytes_read;
}

void EVGAMouseController::Wait()
{
    if(connection_type == EVGA_PERIPHERAL_CONNECTION_TYPE_WIRELESS)
    {
        std::this_thread::sleep_for(EVGA_PERIPHERAL_PACKET_DELAY);
    }
}

bool EVGAMouseController::IsAsleepPacket(unsigned char *buffer)
{
    const int expected_packet_size = 8;
    unsigned char expected_buffer[expected_packet_size] =
    {
        0x00,
        0xA4, 0x00, 0x02, 0x1D,
        0x02, 0x80, 0x02
    };
    return BuffersAreEqual(buffer, expected_buffer, expected_packet_size);
}

bool EVGAMouseController::IsResponseNotReadyPacket(unsigned char *buffer)
{
    const int expected_packet_size = 8;
    unsigned char expected_buffer[expected_packet_size] =
    {
        0x00,
        0xA0, 0x00, 0x02, 0x1D,
        0x02, 0x80, 0x02
    };
    return BuffersAreEqual(buffer, expected_buffer, expected_packet_size);
}

