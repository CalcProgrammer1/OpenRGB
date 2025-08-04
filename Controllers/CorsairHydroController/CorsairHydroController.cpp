/*---------------------------------------------------------*\
| CorsairHydroController.cpp                                |
|                                                           |
|   Driver for Corsair Hydro Series coolers                 |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      17 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <iomanip>
#include <sstream>
#include "CorsairHydroController.h"

CorsairHydroController::CorsairHydroController(libusb_device_handle* dev_handle, std::string dev_name)
{
    dev         = dev_handle;
    name        = dev_name;

    /*-----------------------------------------------------*\
    | Fill in location string with USB ID                   |
    \*-----------------------------------------------------*/
    libusb_device_descriptor descriptor;
    libusb_get_device_descriptor(libusb_get_device(dev_handle), &descriptor);

    std::stringstream location_stream;
    location_stream << std::hex << std::setfill('0') << std::setw(4) << descriptor.idVendor << ":" << std::hex << std::setfill('0') << std::setw(4) << descriptor.idProduct;
    location = location_stream.str();

    SendInit();

    SendFirmwareRequest();
}

CorsairHydroController::~CorsairHydroController()
{
    if(dev)
    {
        libusb_close(dev);
    }
}

std::string CorsairHydroController::GetFirmwareString()
{
    return(firmware_version);
}

std::string CorsairHydroController::GetLocation()
{
    return("USB: " + location);
}

std::string CorsairHydroController::GetNameString()
{
    return(name);
}

void CorsairHydroController::SetBlink
    (
        std::vector<RGBColor> & colors,
        unsigned char           speed
    )
{
    SendColors(colors);
    SendSpeed(speed);
    SendApplyBlink();
}

void CorsairHydroController::SetFixed
    (
        std::vector<RGBColor> & colors
    )
{
    SendColors(colors);

    /*-----------------------------------------------------*\
    | Fixed mode seems to just be shift mode with the same  |
    | value for both colors                                 |
    \*-----------------------------------------------------*/
    SendApplyShift();
}

void CorsairHydroController::SetPulse
    (
        std::vector<RGBColor> & colors,
        unsigned char           speed
    )
{
    SendColors(colors);
    SendSpeed(speed);
    SendApplyPulse();
}

void CorsairHydroController::SetShift
    (
        std::vector<RGBColor> & colors,
        unsigned char           speed
    )
{
    SendColors(colors);
    SendSpeed(speed);
    SendApplyShift();
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void CorsairHydroController::SendApplyBlink()
{
    unsigned char usb_buf[3];
    int           actual;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Apply Blink packet                             |
    \*-----------------------------------------------------*/
    usb_buf[0] = 0x58;
    usb_buf[1] = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    libusb_bulk_transfer(dev, 0x01, usb_buf, 2, &actual, 1000);
    libusb_bulk_transfer(dev, 0x81, usb_buf, 3, &actual, 1000);
}

void CorsairHydroController::SendApplyPulse()
{
    unsigned char usb_buf[3];
    int           actual;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Apply Pulse packet                             |
    \*-----------------------------------------------------*/
    usb_buf[0] = 0x52;
    usb_buf[1] = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    libusb_bulk_transfer(dev, 0x01, usb_buf, 2, &actual, 1000);
    libusb_bulk_transfer(dev, 0x81, usb_buf, 3, &actual, 1000);
}

void CorsairHydroController::SendApplyShift()
{
    unsigned char usb_buf[3];
    int           actual;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Apply Shift packet                             |
    \*-----------------------------------------------------*/
    usb_buf[0] = 0x55;
    usb_buf[1] = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    libusb_bulk_transfer(dev, 0x01, usb_buf, 2, &actual, 1000);
    libusb_bulk_transfer(dev, 0x81, usb_buf, 3, &actual, 1000);
}

void CorsairHydroController::SendFirmwareRequest()
{
    unsigned char usb_buf[8];
    int           actual;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Firmware Request packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0] = 0xAA;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    libusb_bulk_transfer(dev, 0x01, usb_buf, 1, &actual, 1000);
    libusb_bulk_transfer(dev, 0x81, usb_buf, 7, &actual, 1000);

    firmware_version = std::to_string(usb_buf[3]) + "." + std::to_string(usb_buf[4]) + "." + std::to_string(usb_buf[5]) + "." + std::to_string(usb_buf[6]);
}

void CorsairHydroController::SendColors
    (
        std::vector<RGBColor> & colors
    )
{
    unsigned char usb_buf[23];
    int           actual;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Send Colors packet                             |
    \*-----------------------------------------------------*/
    usb_buf[0] = 0x56;
    usb_buf[1] = (unsigned char)colors.size();

    /*---------------------------------------------------------*\
    | Fill in colors from vector                                |
    \*---------------------------------------------------------*/
    for(std::size_t color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        usb_buf[(color_idx * 3) + 2] = RGBGetRValue(colors[color_idx]);
        usb_buf[(color_idx * 3) + 3] = RGBGetGValue(colors[color_idx]);
        usb_buf[(color_idx * 3) + 4] = RGBGetBValue(colors[color_idx]);

        /*---------------------------------------------------------*\
        | If the color vector only has one entry, duplicate it, as  |
        | the controller appears to require two colors in order to  |
        | update.                                                   |
        \*---------------------------------------------------------*/
        if((color_idx == 0) && colors.size() == 1)
        {
            usb_buf[1]                   = (unsigned char)colors.size() + 1;
            usb_buf[(color_idx * 3) + 5] = RGBGetRValue(colors[color_idx]);
            usb_buf[(color_idx * 3) + 6] = RGBGetGValue(colors[color_idx]);
            usb_buf[(color_idx * 3) + 7] = RGBGetBValue(colors[color_idx]);
        }
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    libusb_bulk_transfer(dev, 0x01, usb_buf, 2 + (usb_buf[1] * 3), &actual, 1000);
    libusb_bulk_transfer(dev, 0x81, usb_buf, 3, &actual, 1000);
}

void CorsairHydroController::SendInit()
{
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    libusb_control_transfer( dev, 0x40, 0x00, 0xffff, 0x0000, NULL, 0, 0 );
    libusb_control_transfer( dev, 0x40, 0x02, 0x0002, 0x0000, NULL, 0, 0 );
}

void CorsairHydroController::SendSpeed
    (
        unsigned char           speed
    )
{
    unsigned char usb_buf[3];
    int           actual;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Send Speed packet                              |
    \*-----------------------------------------------------*/
    usb_buf[0] = 0x53;
    usb_buf[1] = speed;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    libusb_bulk_transfer(dev, 0x01, usb_buf, 2, &actual, 1000);
    libusb_bulk_transfer(dev, 0x81, usb_buf, 3, &actual, 1000);
}
