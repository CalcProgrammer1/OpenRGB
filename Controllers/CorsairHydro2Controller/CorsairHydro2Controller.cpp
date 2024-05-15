/*---------------------------------------------------------*\
| CorsairHydro2Controller.cpp                               |
|                                                           |
|   Driver for Corsair H100i v2                             |
|                                                           |
|   Tim Demand (tim.dmd)                        10 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <iomanip>
#include <sstream>
#include "CorsairHydro2Controller.h"

CorsairHydro2Controller::CorsairHydro2Controller(libusb_device_handle* dev_handle)
{
    dev = dev_handle;

    libusb_device_descriptor descriptor;
    libusb_get_device_descriptor(libusb_get_device(dev_handle), &descriptor);

    std::stringstream location_stream;
    location_stream << std::hex << std::setfill('0') << std::setw(4) << descriptor.idVendor << ":" << std::hex << std::setfill('0') << std::setw(4) << descriptor.idProduct;
    location = location_stream.str();

    SendInit();
}

CorsairHydro2Controller::~CorsairHydro2Controller()
{
    if(dev)
    {
        libusb_close(dev);
    }
}

std::string CorsairHydro2Controller::GetLocation()
{
    return("USB: " + location);
}

void CorsairHydro2Controller::SetLED(std::vector<RGBColor>& colors)
{
    unsigned char usb_buf[32];
    memset(usb_buf, 0, sizeof(usb_buf));
    int actual;

    unsigned char led_enable = 0x01;

    unsigned char rr = RGBGetRValue(colors[0]);
    unsigned char gg = RGBGetBValue(colors[0]);
    unsigned char bb = RGBGetGValue(colors[0]);

    if((rr + gg + bb) == 0)
    {
        led_enable = 0x00; // needed because leds won't turn off completely if color is 00 00 00
    }

    usb_buf[0]  = 0x10;
    usb_buf[1]  = rr;
    usb_buf[2]  = bb;
    usb_buf[3]  = gg;
    usb_buf[4]  = 0x00;
    usb_buf[5]  = 0xFF;
    usb_buf[6]  = 0xFF;
    usb_buf[7]  = 0xFF;
    usb_buf[8]  = 0xFF;
    usb_buf[9]  = 0xFF;
    usb_buf[10] = 0x00;
    usb_buf[11] = 0x0A;
    usb_buf[12] = 0x05;
    usb_buf[13] = led_enable;
    usb_buf[14] = 0x00;
    usb_buf[15] = 0x00;
    usb_buf[16] = 0x00;
    usb_buf[17] = 0x00;
    usb_buf[18] = 0x01;

    libusb_bulk_transfer(dev, 0x02, usb_buf, 19, &actual, 1000);
    libusb_bulk_transfer(dev, 0x82, usb_buf, 32, &actual, 1000);
    if(actual != 32) SendInit(); // reinitialization after sleep
}

void CorsairHydro2Controller::SendInit()
{
    libusb_reset_device(dev); // needed for reinitialization after sleep

    libusb_control_transfer(dev, 0x40, 0, 0xffff, 0x0000, NULL, 0, 0);
    libusb_control_transfer(dev, 0x40, 2, 0x0002, 0x0000, NULL, 0, 0);
    libusb_control_transfer(dev, 0x40, 1, 0x0002, 0x0000, NULL, 0, 0);
    libusb_control_transfer(dev, 0x40, 4, 0x0002, 0x0000, NULL, 0, 0);
}
