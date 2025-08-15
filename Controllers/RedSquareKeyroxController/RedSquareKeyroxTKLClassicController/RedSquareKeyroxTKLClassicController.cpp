/*---------------------------------------------------------*\
| RedSquareKeyroxTKLClassicController.cpp                   |
|                                                           |
|   Driver for Red Square Keyrox TKL Classic                |
|                                                           |
|   vlack                                       03 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "LogManager.h"
#include "RedSquareKeyroxTKLClassicController.h"

using namespace std::chrono_literals;

RedSquareKeyroxTKLClassicController::RedSquareKeyroxTKLClassicController(hid_device *dev_handle, const hid_device_info &info, std::string dev_name)
{
    dev             = dev_handle;
    location        = info.path;
    name            = dev_name;
}

RedSquareKeyroxTKLClassicController::~RedSquareKeyroxTKLClassicController()
{
    hid_close(dev);
}

std::string RedSquareKeyroxTKLClassicController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RedSquareKeyroxTKLClassicController::GetNameString()
{
    return(name);
}

std::string RedSquareKeyroxTKLClassicController::GetSerialString()
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

int RedSquareKeyroxTKLClassicController::GetDirection(int direction)
{
    switch(direction)
    {
        case MODE_DIRECTION_LEFT:
            return 0x00;
        case MODE_DIRECTION_RIGHT:
            return 0x01;
        case MODE_DIRECTION_UP:
            return 0x02;
        case MODE_DIRECTION_DOWN:
            return 0x03;
        case MODE_DIRECTION_HORIZONTAL: // actually direction out
            return 0x04;
        case MODE_DIRECTION_VERTICAL:   // actually direction in
            return 0x05;
        default:
            return 0x00;
    }
}

int RedSquareKeyroxTKLClassicController::GetDirectionRound(int direction)
{
    switch(direction)
    {
        case MODE_DIRECTION_LEFT:   // actually anticlockwise
                return 0x07;
        case MODE_DIRECTION_RIGHT:  // actually clockwise
                return 0x06;
        default:
                return 0x00;
    }
}

void RedSquareKeyroxTKLClassicController::SetMode(mode m)
{
    /*---------------------------------------------*\
    |  Mode set command                             |
    \*---------------------------------------------*/

    unsigned char usb_buf[CLASSIC_PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, CLASSIC_PACKET_DATA_LENGTH);

    usb_buf[0] = 0x01;
    usb_buf[1] = 0x07;
    usb_buf[6] = m.value;
    usb_buf[7] = m.brightness; // brightness
    usb_buf[8] = m.speed; // speed

    if(m.colors_max == 1 || m.colors_max == 2)
    {
        usb_buf[9]  = RGBGetRValue(m.colors[0]); // front R
        usb_buf[10] = RGBGetGValue(m.colors[0]); // front G
        usb_buf[11] = RGBGetBValue(m.colors[0]); // front B
    }

    if(m.colors_max == 2 && m.color_mode != MODE_COLORS_RANDOM)
    {
        usb_buf[12] = RGBGetRValue(m.colors[1]); // back R
        usb_buf[13] = RGBGetGValue(m.colors[1]); // back G
        usb_buf[14] = RGBGetBValue(m.colors[1]); // back B
    }

    if(m.value == CLASSIC_RADAR_MODE_VALUE || m.value == CLASSIC_RUNNING_LINE_MODE_VALUE)
    {
        usb_buf[15] = GetDirectionRound(m.direction);
    }
    else
    {
        usb_buf[15] = GetDirection(m.direction);
    }

    usb_buf[16] = m.color_mode == MODE_COLORS_RANDOM;

    hid_write(dev, usb_buf, CLASSIC_PACKET_DATA_LENGTH);
    // sleep is necessary
    std::this_thread::sleep_for(10ms);
}

void RedSquareKeyroxTKLClassicController::SetLEDsData(std::vector<RGBColor> colors, std::vector<led> leds)
{
    /*---------------------------------------------*\
    |  LEDs data set command                        |
    \*---------------------------------------------*/

    unsigned char usb_buf[CLASSIC_PACKET_DATA_LENGTH * 8];
    memset(usb_buf, 0x00, CLASSIC_PACKET_DATA_LENGTH * 8);

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        int offset          = leds[i].value;
        usb_buf[offset - 1] = RGBGetRValue(colors[i]);
        usb_buf[offset]     = RGBGetGValue(colors[i]);
        usb_buf[offset + 1] = RGBGetBValue(colors[i]);
    }

    for(unsigned int i = 0; i < 8; i++)
    {
        unsigned char packet[CLASSIC_PACKET_DATA_LENGTH];
        memset(packet, 0x00, CLASSIC_PACKET_DATA_LENGTH);

        packet[0] = 0x01;
        packet[1] = 0x0F;
        packet[4] = i;  // package number
        packet[5] = i == 7 ? 0x12 : 0x36; // package size

        for (int x = 6; x < CLASSIC_PACKET_DATA_LENGTH; x++)
        {
            packet[x] = usb_buf[CLASSIC_PACKET_DATA_LENGTH * i + x];
        }
        hid_write(dev, packet, CLASSIC_PACKET_DATA_LENGTH);
    }

    // sleep is necessary
    std::this_thread::sleep_for(10ms);
}
