/*---------------------------------------------------------*\
| LogitechG203LController.cpp                               |
|                                                           |
|   Driver for Logitech G203L                               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogitechG203LController.h"
#include "StringUtils.h"

#define PACKET_SIZE     20

LogitechG203LController::LogitechG203LController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    // enable software control
    unsigned char usb_buf[PACKET_SIZE];

    memset(usb_buf, 0x00, PACKET_SIZE);

    usb_buf[0x00] = 0x11;
    usb_buf[0x01] = 0xFF;
    usb_buf[0x02] = 0x0E;
    usb_buf[0x03] = 0x50;
    usb_buf[0x04] = 0x01;
    usb_buf[0x05] = 0x03;
    usb_buf[0x06] = 0x07;

    SendPacket(usb_buf);
}

LogitechG203LController::~LogitechG203LController()
{
    if(dev != nullptr)
    {
        hid_close(dev);
    }
}

std::string LogitechG203LController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LogitechG203LController::GetNameString()
{
    return(name);
}

std::string LogitechG203LController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void LogitechG203LController::SendApply()
{
    unsigned char usb_buf[PACKET_SIZE];

    memset(usb_buf, 0x00, PACKET_SIZE);

    usb_buf[0x00] = 0x11;
    usb_buf[0x01] = 0xFF;
    usb_buf[0x02] = 0x12;
    usb_buf[0x03] = 0x70;

    SendPacket(usb_buf);
}

void LogitechG203LController::SetSingleLED(int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char usb_buf[PACKET_SIZE];

    memset(usb_buf, 0x00, PACKET_SIZE);

    usb_buf[0x00] = 0x11;
    usb_buf[0x01] = 0xFF;
    usb_buf[0x02] = 0x12;
    usb_buf[0x03] = 0x10;

    usb_buf[0x04] = (unsigned char)led;
    usb_buf[0x05] = red;
    usb_buf[0x06] = green;
    usb_buf[0x07] = blue;

    usb_buf[0x08] = 0xFF;

    SendPacket(usb_buf);

    SendApply();
}

void LogitechG203LController::SetMode(
    int             mode,
    int             speed,
    unsigned char   bright,
    unsigned char   dir,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue)
{
    unsigned char usb_buf[PACKET_SIZE];
    unsigned char brightness = bright * 5;

    if(brightness == 0)
    {
        brightness = 1;
    }

    memset(usb_buf, 0x00, PACKET_SIZE);

    //Header
    usb_buf[0x00] = 0x11;
    usb_buf[0x01] = 0xFF;
    usb_buf[0x02] = 0x0E;
    usb_buf[0x03] = 0x10;
    //Common Data
    usb_buf[0x04] = 0x00;
    usb_buf[0x05] = (unsigned char)mode;
    usb_buf[0x06] = red;
    usb_buf[0x07] = green;
    usb_buf[0x08] = blue;
    //mode specific Data and position
    if(mode == LOGITECH_G203L_MODE_STATIC) usb_buf[0x09] = 0x02;
    if(mode == LOGITECH_G203L_MODE_CYCLE)
    {
        usb_buf[0x0B] = (unsigned char)((speed>>8) & 0x000000FF);
        usb_buf[0x0C] = (unsigned char)(speed & 0x000000FF);
        usb_buf[0x0D] = brightness;
    }
    if(mode == LOGITECH_G203L_MODE_BREATHING)
    {
        usb_buf[0x09] = (unsigned char)((speed>>8) & 0x000000FF);
        usb_buf[0x0A] = (unsigned char)(speed & 0x000000FF);
        usb_buf[0x0C] = brightness;
    }
    if(mode == LOGITECH_G203L_MODE_WAVE)
    {
        usb_buf[0x0C] = (unsigned char)(speed & 0x000000FF);
        usb_buf[0x0D] = dir ? 0x01 : 0x06; //0x01: Left->Right   0x06: Right->Left
        usb_buf[0x0E] = brightness;
        usb_buf[0x0F] = (unsigned char)((speed>>8) & 0x000000FF);
    }
    if(mode == LOGITECH_G203L_MODE_COLORMIXING)
    {
        usb_buf[0x0C] = (unsigned char)(speed & 0x000000FF);
        usb_buf[0x0D] = (unsigned char)((speed>>8) & 0x000000FF);
        usb_buf[0x0E] = brightness;
    }

    //END BYTE
    usb_buf[0x10] = 0x01;

    SendPacket(usb_buf);
}

void LogitechG203LController::SetDevice(std::vector<RGBColor> colors)
{
    unsigned char usb_buf[PACKET_SIZE];

    memset(usb_buf, 0x00, PACKET_SIZE);

    usb_buf[0x00] = 0x11;
    usb_buf[0x01] = 0xFF;
    usb_buf[0x02] = 0x12;
    usb_buf[0x03] = 0x10;

    usb_buf[0x04] = 0x01;
    usb_buf[0x05] = RGBGetRValue(colors[0]);
    usb_buf[0x06] = RGBGetGValue(colors[0]);
    usb_buf[0x07] = RGBGetBValue(colors[0]);

    usb_buf[0x08] = 0x02;
    usb_buf[0x09] = RGBGetRValue(colors[1]);
    usb_buf[0x0A] = RGBGetGValue(colors[1]);
    usb_buf[0x0B] = RGBGetBValue(colors[1]);

    usb_buf[0x0C] = 0x03;
    usb_buf[0x0D] = RGBGetRValue(colors[2]);
    usb_buf[0x0E] = RGBGetGValue(colors[2]);
    usb_buf[0x0F] = RGBGetBValue(colors[2]);

    usb_buf[0x10] = 0xFF;

    SendPacket(usb_buf);

    SendApply();
}

void LogitechG203LController::SendPacket(unsigned char* buffer)
{
    if(dev != nullptr)
    {
        if(hid_write(dev, buffer, PACKET_SIZE) == -1)
        {
            hid_close(dev);
            dev = hid_open_path(location.c_str());
            return;
        }
    }

    if(dev != nullptr)
    {
        if(hid_read_timeout(dev, buffer, PACKET_SIZE, 10) <= 0)
        {
            hid_close(dev);
            dev = hid_open_path(location.c_str());
            return;
        }
    }
}
