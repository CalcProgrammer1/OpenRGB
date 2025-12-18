/*---------------------------------------------------------*\
| LianLiUniversalScreenController.cpp                       |
|                                                           |
|   Driver for Lian Li 8.8" Universal Screen LEDs           |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      17 Dec 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <iomanip>
#include <sstream>
#include "LianLiUniversalScreenController.h"

LianLiUniversalScreenController::LianLiUniversalScreenController(libusb_device_handle* device)
{
    dev = device;

    /*-----------------------------------------------------*\
    | Fill in location string with USB ID                   |
    \*-----------------------------------------------------*/
    libusb_device_descriptor descriptor;
    libusb_get_device_descriptor(libusb_get_device(dev), &descriptor);

    std::stringstream location_stream;
    location_stream << std::hex << std::setfill('0') << std::setw(4) << descriptor.idVendor << ":" << std::hex << std::setfill('0') << std::setw(4) << descriptor.idProduct;
    location = location_stream.str();

    /*-----------------------------------------------------*\
    | Fill in the serial string from the string descriptor  |
    \*-----------------------------------------------------*/
    char serialStr[64];

    int ret = libusb_get_string_descriptor_ascii(dev, descriptor.iSerialNumber, reinterpret_cast<unsigned char*>(serialStr), sizeof(serialStr));

    if(ret > 0)
    {
        serial = std::string(serialStr, ret);
    }
}

LianLiUniversalScreenController::~LianLiUniversalScreenController()
{
    if(dev)
    {
        libusb_close(dev);
    }
}

std::string LianLiUniversalScreenController::GetLocation()
{
    return("USB: " + location);
}

std::string LianLiUniversalScreenController::GetSerial()
{
    return(serial);
}

std::string LianLiUniversalScreenController::GetVersion()
{
    return("");
}

void LianLiUniversalScreenController::SetLedColors(RGBColor* colors, size_t count)
{
    unsigned char leds_in_packet    = 20;
    unsigned char offset            = 0;
    unsigned char usb_buf[64];

    memset(usb_buf, 0, sizeof(usb_buf));

    do
    {
        usb_buf[0] = 0x11;
        usb_buf[1] = offset;
        usb_buf[2] = 0;
        usb_buf[3] = 0;

        if((count - offset) < leds_in_packet)
        {
            leds_in_packet = count - offset;
        }

        for(std::size_t led_idx = 0; led_idx < leds_in_packet; led_idx++)
        {
            usb_buf[4 + (led_idx * 3)] = RGBGetRValue(colors[offset + led_idx]);
            usb_buf[5 + (led_idx * 3)] = RGBGetGValue(colors[offset + led_idx]);
            usb_buf[6 + (led_idx * 3)] = RGBGetBValue(colors[offset + led_idx]);
        }

        offset += leds_in_packet;

        int actual_length = sizeof(usb_buf);
        libusb_bulk_transfer(dev, 1, usb_buf, sizeof(usb_buf), &actual_length, 25);
    } while( offset < count );
}
