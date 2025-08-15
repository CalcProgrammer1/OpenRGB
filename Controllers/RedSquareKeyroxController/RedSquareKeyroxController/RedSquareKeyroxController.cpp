/*---------------------------------------------------------*\
| RedSquareKeyroxController.cpp                             |
|                                                           |
|   Driver for Red Square Keyrox                            |
|                                                           |
|   cafeed28                                    03 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <iostream>
#include "LogManager.h"
#include "RedSquareKeyroxController.h"

using namespace std::chrono_literals;

RedSquareKeyroxController::RedSquareKeyroxController(hid_device *dev_handle, const hid_device_info &info, int variant, std::string dev_name)
{
    dev             = dev_handle;
    location        = info.path;
    name            = dev_name;
    this->variant   = variant;
}

RedSquareKeyroxController::~RedSquareKeyroxController()
{
    hid_close(dev);
}

int RedSquareKeyroxController::GetVariant()
{
    return variant;
}

std::string RedSquareKeyroxController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RedSquareKeyroxController::GetNameString()
{
    return(name);
}

std::string RedSquareKeyroxController::GetSerialString()
{
    wchar_t serial_wchar[128];
    hid_get_serial_number_string(dev, serial_wchar, 128);
    std::wstring serial_wstring(serial_wchar);

    std::string serial_string;
    std::transform(serial_wstring.begin(), serial_wstring.end(), std::back_inserter(serial_string), [] (wchar_t i)
    {
        return (char)i;
    });

    return serial_string;
}

int RedSquareKeyroxController::GetDirectionLRUD(int direction)
{
    switch(direction)
    {
        case MODE_DIRECTION_LEFT:
            return 0x10;
        case MODE_DIRECTION_RIGHT:
            return 0x00;
        case MODE_DIRECTION_UP:
            return 0x20;
        case MODE_DIRECTION_DOWN:
            return 0x30;
        default:
            return 0x00;
    }
}

int RedSquareKeyroxController::GetDirectionUD(int direction)
{
    switch(direction)
    {
        case MODE_DIRECTION_UP:
            return 0xA0;
        case MODE_DIRECTION_DOWN:
            return 0xB0;
        default:
            return 0xA0;
    }
}

void RedSquareKeyroxController::SetLedSequencePositions(std::vector<unsigned int> positions)
{
    led_sequence_positions = positions;
}

void RedSquareKeyroxController::SetMode(std::vector<mode> modes, int active_mode)
{
    /*---------------------------------------------*\
    |  Mode set command                             |
    \*---------------------------------------------*/
    mode m = modes[active_mode];

    unsigned char usb_buf[PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[4] = 0x01;
    usb_buf[6] = 0x04;
    usb_buf[8] = m.value;

    Send(usb_buf);
}

void RedSquareKeyroxController::SetModeData(std::vector<mode> modes, int active_mode)
{
    /*---------------------------------------------*\
    |  Mode specific data set command               |
    \*---------------------------------------------*/
    mode m = modes[active_mode];

    if(m.value == CUSTOM_MODE_VALUE)
    {
        return;
    }

    unsigned char usb_buf[PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[4] = 0x09;
    usb_buf[6] = 0x05;
    usb_buf[7] = m.value;
    usb_buf[8] = m.brightness;
    usb_buf[9] = 0xFF;

    if(m.flags & MODE_FLAG_HAS_SPEED)
    {
        usb_buf[10] = m.speed;
        usb_buf[11] = 0xFF;

        if(m.value == SPECTRUM_MODE_VALUE)
        {
            usb_buf[10] += 0x80;
        }
    }

    if(m.flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        if(m.flags & MODE_FLAG_HAS_RANDOM_COLOR && m.color_mode == MODE_COLORS_RANDOM)
        {
            usb_buf[10] += 0x80;
        }
        else
        {
            usb_buf[11] = RGBGetRValue(m.colors[0]);
            usb_buf[12] = RGBGetGValue(m.colors[0]);
            usb_buf[13] = RGBGetBValue(m.colors[0]);
        }
    }

    if((m.flags & MODE_FLAG_HAS_DIRECTION_LR) && (m.flags & MODE_FLAG_HAS_DIRECTION_UD))
    {
        usb_buf[10] += GetDirectionLRUD(m.direction);
    }
    else if((m.flags & MODE_FLAG_HAS_DIRECTION_UD) && !(m.flags & MODE_FLAG_HAS_DIRECTION_LR))
    {
        usb_buf[10] += GetDirectionUD(m.direction);
    }

    Send(usb_buf);
}

void RedSquareKeyroxController::SetLEDsData(std::vector<mode> modes, int active_mode, std::vector<RGBColor> colors)
{
    /*---------------------------------------------*\
    |  LEDs data set command                        |
    \*---------------------------------------------*/
    mode m = modes[active_mode];

    if(m.value != CUSTOM_MODE_VALUE)
    {
        return;
    }

    unsigned char usb_buf[PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[4] = 0xB0;
    usb_buf[5] = 0x01;
    usb_buf[6] = 0x07;

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        int offset = 7 + led_sequence_positions[i] * 4;
        usb_buf[offset + 1] = RGBGetRValue(colors[i]);
        usb_buf[offset + 2] = RGBGetGValue(colors[i]);
        usb_buf[offset + 3] = RGBGetBValue(colors[i]);
        usb_buf[offset + 4] = m.brightness;
    }

    Send(usb_buf);
}

void RedSquareKeyroxController::Send(unsigned char data[PACKET_DATA_LENGTH])
{
    unsigned char usb_buf[PACKET_DATA_LENGTH + 1];

    usb_buf[0] = 0x00; // Report ID

    for(int x = 0; x < PACKET_DATA_LENGTH; x++)
    {
        usb_buf[x + 1] = data[x];
    }

    hid_send_feature_report(dev, usb_buf, PACKET_DATA_LENGTH + 1);

    std::this_thread::sleep_for(10ms);
}
