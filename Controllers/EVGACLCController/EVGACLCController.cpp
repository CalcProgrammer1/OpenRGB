/*---------------------------------------------------------*\
| EVGACLCController.cpp                                     |
|                                                           |
|   Driver for EVGA CLC liquid coolers (Asetek 690LC)       |
|                                                           |
|   Protocol based on liquidctl's Asetek 690LC driver       |
|   https://github.com/liquidctl/liquidctl                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <iomanip>
#include <sstream>
#include "EVGACLCController.h"

EVGACLCController::EVGACLCController(libusb_device_handle* dev_handle, std::string dev_name)
{
    dev  = dev_handle;
    name = dev_name;

    if(!dev)
    {
        firmware_version = "Unknown";
        location         = "Unknown";
        return;
    }

    /*-----------------------------------------------------*\
    | Fill in location string with USB ID                   |
    \*-----------------------------------------------------*/
    libusb_device_descriptor descriptor;
    int ret = libusb_get_device_descriptor(libusb_get_device(dev_handle), &descriptor);

    if(ret < 0)
    {
        location = "Unknown";
    }
    else
    {
        std::stringstream location_stream;
        location_stream << std::hex << std::setfill('0') << std::setw(4) << descriptor.idVendor
                        << ":" << std::hex << std::setfill('0') << std::setw(4) << descriptor.idProduct;
        location = location_stream.str();
    }

    SendInit();
    SendFirmwareRequest();
}

EVGACLCController::~EVGACLCController()
{
    if(dev)
    {
        libusb_close(dev);
    }
}

std::string EVGACLCController::GetFirmwareString()
{
    return firmware_version;
}

std::string EVGACLCController::GetLocation()
{
    return "USB: " + location;
}

std::string EVGACLCController::GetNameString()
{
    return name;
}

void EVGACLCController::SetFixed(unsigned char r, unsigned char g, unsigned char b)
{
    if(!dev)
    {
        return;
    }

    if(name.empty())
    {
        return;
    }

    SendConfigPacket(r, g, b, 0, 0, 0, 0, 0, true, false, false, false);
    SendRainbowSpeed(0);
}

void EVGACLCController::SetFading
    (
        unsigned char r1, unsigned char g1, unsigned char b1,
        unsigned char r2, unsigned char g2, unsigned char b2,
        unsigned char speed
    )
{
    if(!dev)
    {
        return;
    }

    if(speed < 1)
    {
        speed = 1;
    }

    SendConfigPacket(r1, g1, b1, r2, g2, b2, speed, 0, true, true, false, false);
    SendRainbowSpeed(0);
}

void EVGACLCController::SetBlinking
    (
        unsigned char r, unsigned char g, unsigned char b,
        unsigned char speed
    )
{
    if(!dev)
    {
        return;
    }

    if(speed < 1)
    {
        speed = 1;
    }

    SendConfigPacket(r, g, b, 0, 0, 0, speed, speed, true, false, true, false);
    SendRainbowSpeed(0);
}

void EVGACLCController::SetRainbow(unsigned char speed)
{
    if(!dev)
    {
        return;
    }

    if(speed > 6)
    {
        speed = 6;
    }

    SendConfigPacket(0, 0, 0, 0, 0, 0, 0, 0, true, false, false, false);
    SendRainbowSpeed(speed);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void EVGACLCController::SendInit()
{
    if(!dev)
    {
        return;
    }

    if(!libusb_get_device(dev))
    {
        return;
    }

    /*-----------------------------------------------------*\
    | USBXpress initialization - same as Corsair Hydro      |
    | Open device and enable clear-to-send                  |
    \*-----------------------------------------------------*/
    int ret = libusb_control_transfer(dev, 0x40, 0x00, 0xFFFF, 0x0000, NULL, 0, 0);

    if(ret < 0)
    {
        return;
    }

    ret = libusb_control_transfer(dev, 0x40, 0x02, 0x0002, 0x0000, NULL, 0, 0);

    if(ret < 0)
    {
        return;
    }
}

void EVGACLCController::FlushBuffers()
{
    if(!dev)
    {
        return;
    }

    if(!libusb_get_device(dev))
    {
        return;
    }

    /*-----------------------------------------------------*\
    | USBXpress flush - must be called before each command   |
    \*-----------------------------------------------------*/
    int ret = libusb_control_transfer(dev, 0x40, 0x02, 0x0001, 0x0000, NULL, 0, 0);

    if(ret < 0)
    {
        return;
    }
}

void EVGACLCController::SendFirmwareRequest()
{
    if(!dev)
    {
        firmware_version = "Unknown";
        return;
    }

    if(!libusb_get_device(dev))
    {
        firmware_version = "Unknown";
        return;
    }

    unsigned char usb_buf[32];
    int           actual;
    int           ret;

    memset(usb_buf, 0, sizeof(usb_buf));

    FlushBuffers();

    /*-----------------------------------------------------*\
    | Send firmware/status request (command 0x14)            |
    \*-----------------------------------------------------*/
    usb_buf[0] = 0x14;

    ret = libusb_bulk_transfer(dev, 0x02, usb_buf, 4, &actual, 1000);

    if(ret < 0)
    {
        firmware_version = "Unknown";
        return;
    }

    ret = libusb_bulk_transfer(dev, 0x82, usb_buf, 32, &actual, 1000);

    if(ret < 0)
    {
        firmware_version = "Unknown";
        return;
    }

    firmware_version = std::to_string(usb_buf[0x17]) + "."
                     + std::to_string(usb_buf[0x18]) + "."
                     + std::to_string(usb_buf[0x19]) + "."
                     + std::to_string(usb_buf[0x1A]);
}

void EVGACLCController::SendConfigPacket
    (
        unsigned char r1,
        unsigned char g1,
        unsigned char b1,
        unsigned char r2,
        unsigned char g2,
        unsigned char b2,
        unsigned char interval1,
        unsigned char interval2,
        bool          not_blackout,
        bool          fading,
        bool          blinking,
        bool          alert
    )
{
    if(!dev)
    {
        return;
    }

    if(!libusb_get_device(dev))
    {
        return;
    }

    unsigned char usb_buf[32];
    int           actual;
    int           ret;

    memset(usb_buf, 0, sizeof(usb_buf));

    FlushBuffers();

    /*-----------------------------------------------------*\
    | Build the 0x10 runtime configuration packet            |
    \*-----------------------------------------------------*/
    usb_buf[0]  = 0x10;            // Command: runtime config
    usb_buf[1]  = r1;              // Color 1 R
    usb_buf[2]  = g1;              // Color 1 G
    usb_buf[3]  = b1;              // Color 1 B
    usb_buf[4]  = r2;              // Color 2 R
    usb_buf[5]  = g2;              // Color 2 G
    usb_buf[6]  = b2;              // Color 2 B
    usb_buf[7]  = 0xFF;            // Alert color R
    usb_buf[8]  = 0x00;            // Alert color G
    usb_buf[9]  = 0x00;            // Alert color B
    usb_buf[10] = 0x2D;            // Alert temp threshold (45C)
    usb_buf[11] = interval1;       // Interval 1
    usb_buf[12] = interval2;       // Interval 2
    usb_buf[13] = not_blackout ? 0x01 : 0x00;  // NOT blackout (1 = LED on)
    usb_buf[14] = fading    ? 0x01 : 0x00;     // Fading mode
    usb_buf[15] = blinking  ? 0x01 : 0x00;     // Blinking mode
    usb_buf[16] = alert     ? 0x01 : 0x00;     // Enable alert
    usb_buf[17] = 0x00;            // Unknown
    usb_buf[18] = 0x01;            // Unknown

    ret = libusb_bulk_transfer(dev, 0x02, usb_buf, 19, &actual, 1000);

    if(ret < 0)
    {
        return;
    }

    ret = libusb_bulk_transfer(dev, 0x82, usb_buf, 32, &actual, 1000);

    if(ret < 0)
    {
        return;
    }
}

void EVGACLCController::SendRainbowSpeed(unsigned char speed)
{
    if(!dev)
    {
        return;
    }

    if(!libusb_get_device(dev))
    {
        return;
    }

    unsigned char usb_buf[32];
    int           actual;
    int           ret;

    memset(usb_buf, 0, sizeof(usb_buf));

    FlushBuffers();

    /*-----------------------------------------------------*\
    | Rainbow speed command (0x23)                           |
    | speed 0 = rainbow off, 1-6 = rainbow speed            |
    \*-----------------------------------------------------*/
    usb_buf[0] = 0x23;
    usb_buf[1] = speed;

    ret = libusb_bulk_transfer(dev, 0x02, usb_buf, 2, &actual, 1000);

    if(ret < 0)
    {
        return;
    }

    ret = libusb_bulk_transfer(dev, 0x82, usb_buf, 32, &actual, 1000);

    if(ret < 0)
    {
        return;
    }
}
