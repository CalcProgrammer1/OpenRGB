/*---------------------------------------------------------*\
| CMMMController.cpp                                        |
|                                                           |
|   Driver for Cooler Master mouse                          |
|                                                           |
|   Chris M (Dr_No)                             14 Feb 2021 |
|   Dracrius                                    12 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "CMMMController.h"
#include "StringUtils.h"

CMMMController::CMMMController(hid_device* dev_handle, char *_path, uint16_t pid, std::string dev_name)
{
    dev                                  = dev_handle;
    location                             = _path;
    name                                 = dev_name;
    current_speed                        = CM_MM_SPEED_3;
    product_id                           = pid;

    if(product_id == CM_MM530_PID || product_id == CM_MM531_PID)
    {
        command_code = CM_MM5XX_COMMAND;
        if(pid == CM_MM530_PID)
        {
            buttons_bytes[0]             = CM_MM_MODE_BYTE;
            buttons_bytes[1]             = CM_MM_SPEED_BYTE;
            buttons_bytes[2]             = CM_MM_NFI_1;
            wheel_bytes[0]               = CM_MM_RED_BYTE;
            wheel_bytes[1]               = CM_MM_GREEN_BYTE;
            wheel_bytes[2]               = CM_MM_BLUE_BYTE;
        }
        else if(product_id == CM_MM531_PID) //Still Need Captures for Proper Mapping From a MM531 User
        {
            buttons_bytes[0]             = CM_MM_MODE_BYTE;
            buttons_bytes[1]             = CM_MM_SPEED_BYTE;
            buttons_bytes[2]             = CM_MM_NFI_1;
            wheel_bytes[0]               = CM_MM_RED_BYTE;
            wheel_bytes[1]               = CM_MM_GREEN_BYTE;
            wheel_bytes[2]               = CM_MM_BLUE_BYTE;
        }
    }
    else
    {
        command_code                     = CM_MM7XX_COMMAND;

        buttons_bytes[0]                 = CM_MM_RED_BYTE;
        buttons_bytes[1]                 = CM_MM_GREEN_BYTE;
        buttons_bytes[2]                 = CM_MM_BLUE_BYTE;
        wheel_bytes[0]                   = CM_MM_MODE_BYTE;
        wheel_bytes[1]                   = CM_MM_SPEED_BYTE;
        wheel_bytes[2]                   = CM_MM_NFI_1;
    }

    logo_bytes[0]                        = CM_MM_NFI_2;
    logo_bytes[1]                        = CM_MM_NFI_3;
    logo_bytes[2]                        = CM_MM_BRIGHTNESS_BYTE;

    SendInitPacket();
    GetColourStatus();
    GetCustomStatus();
    GetModeStatus();
}

CMMMController::~CMMMController()
{
    hid_close(dev);
}

void CMMMController::GetColourStatus()
{
    uint8_t buffer[CM_MM_PACKET_SIZE]    = { 0x00, 0x52, command_code };

    hid_write(dev, buffer, CM_MM_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM_PACKET_SIZE, CM_MM_INTERRUPT_TIMEOUT);

    current_brightness                   = buffer[CM_MM_BRIGHTNESS_BYTE - 1];
    current_red                          = buffer[CM_MM_RED_BYTE - 1];
    current_green                        = buffer[CM_MM_GREEN_BYTE - 1];
    current_blue                         = buffer[CM_MM_BLUE_BYTE - 1];
}

void CMMMController::GetCustomStatus()
{
    uint8_t buffer[CM_MM_PACKET_SIZE]    = { 0x00, 0x52, 0xA8 };
    int read_size                        = CM_MM_PACKET_SIZE - 1;
    int result                           = 0;

    hid_write(dev, buffer, CM_MM_PACKET_SIZE);
    do
    {
        result = hid_read_timeout(dev, buffer, read_size, CM_MM_INTERRUPT_TIMEOUT);
    }while(buffer[1] != 0xA8 && result == read_size);

    if(result == read_size)
    {
        buttons_colour                   = ToRGBColor(buffer[4], buffer[5], buffer[6]);
        logo_colour                      = ToRGBColor(buffer[7], buffer[8], buffer[9]);
        wheel_colour                     = ToRGBColor(buffer[10], buffer[11], buffer[12]);
    }
}

void CMMMController::GetModeStatus()
{
    uint8_t buffer[CM_MM_PACKET_SIZE]    = { 0x00, 0x52, 0x28 };
    int buffer_size                      = (sizeof(buffer) / sizeof(buffer[0]));

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_MM_INTERRUPT_TIMEOUT);

    current_mode                         = buffer[CM_MM_MODE_BYTE - 1];
}

std::string CMMMController::GetDeviceVendor()
{
    wchar_t vendor_string[HID_MAX_STR];
    int ret = hid_get_manufacturer_string(dev, vendor_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(vendor_string));
}

std::string CMMMController::GetSerial()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_indexed_string(dev, 2, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string CMMMController::GetLocation()
{
    return("HID: " + location);
}

std::string CMMMController::GetName()
{
    return(name);
}

uint16_t CMMMController::GetProductID()
{
    return product_id;
}

unsigned char CMMMController::GetMode()
{
    return current_mode;
}

unsigned char CMMMController::GetLedRed()
{
    return current_red;
}

unsigned char CMMMController::GetLedGreen()
{
    return current_green;
}

unsigned char CMMMController::GetLedBlue()
{
    return current_blue;
}

unsigned char CMMMController::GetLedSpeed()
{
    return current_speed;
}

RGBColor CMMMController::GetWheelColour()
{
    return wheel_colour;
}

RGBColor CMMMController::GetButtonsColour()
{
    return buttons_colour;
}

RGBColor CMMMController::GetLogoColour()
{
    return logo_colour;
}

void CMMMController::SetLedsDirect(RGBColor wheel_colour, RGBColor buttons_colour, RGBColor logo_colour)
{
    unsigned char buffer[CM_MM_PACKET_SIZE] = { 0x00, 0x51, 0xA8, 0x00, 0x00 };

    buffer[buttons_bytes[0]]                = RGBGetRValue(buttons_colour);
    buffer[buttons_bytes[1]]                = RGBGetGValue(buttons_colour);
    buffer[buttons_bytes[2]]                = RGBGetBValue(buttons_colour);
    buffer[logo_bytes[0]]                   = RGBGetRValue(logo_colour);
    buffer[logo_bytes[1]]                   = RGBGetGValue(logo_colour);
    buffer[logo_bytes[2]]                   = RGBGetBValue(logo_colour);
    buffer[wheel_bytes[0]]                  = RGBGetRValue(wheel_colour);
    buffer[wheel_bytes[1]]                  = RGBGetGValue(wheel_colour);
    buffer[wheel_bytes[2]]                  = RGBGetBValue(wheel_colour);

    hid_write(dev, buffer, CM_MM_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM_PACKET_SIZE, CM_MM_INTERRUPT_TIMEOUT);
}

void CMMMController::SendUpdate(uint8_t mode, uint8_t speed, RGBColor colour, uint8_t brightness)
{
    if (mode == CM_MM_MODE_CUSTOM || mode == CM_MM_MODE_MULTILAYER)
    {
        SendUsingZonesPacket(mode);
    }
    else
    {
        SendInitPacket();
        SendApplyPacket(mode);
    }

    uint8_t nfi_1                               = 0x20;

    if (mode == CM_MM_MODE_STATIC || mode == CM_MM_MODE_SPECTRUM_CYCLE)
    {
        nfi_1                                   = 0x00;
    }

    if (mode != CM_MM_MODE_OFF)
    {
        unsigned char buffer[CM_MM_PACKET_SIZE] = { 0x00, 0x51, command_code, 0x00, 0x00 };

        buffer[CM_MM_MODE_BYTE]                 = mode;
        buffer[CM_MM_SPEED_BYTE]                = speed;
        buffer[CM_MM_NFI_1]                     = nfi_1;
        buffer[CM_MM_NFI_2]                     = 0xFF;
        buffer[CM_MM_NFI_3]                     = 0xFF;
        buffer[CM_MM_BRIGHTNESS_BYTE]           = brightness;
        buffer[CM_MM_RED_BYTE]                  = RGBGetRValue(colour);
        buffer[CM_MM_GREEN_BYTE]                = RGBGetGValue(colour);
        buffer[CM_MM_BLUE_BYTE]                 = RGBGetBValue(colour);
        buffer[CM_MM_SKY_RED_BYTE]              = 0x00;
        buffer[CM_MM_SKY_GREEN_BYTE]            = 0x00;
        buffer[CM_MM_SKY_BLUE_BYTE]             = 0x00;

        for (int i = 17; i < CM_MM_PACKET_SIZE; i++)
        {
            buffer[i]                           = 0xFF;
        }

        hid_write(dev, buffer, CM_MM_PACKET_SIZE);
        hid_read_timeout(dev, buffer, CM_MM_PACKET_SIZE, CM_MM_INTERRUPT_TIMEOUT);

    }

    if (mode == CM_MM_MODE_CUSTOM || mode == CM_MM_MODE_MULTILAYER)
    {
        SendApplyPacket(mode); //Post Apply for Zoned Modes
    }
}

void CMMMController::SendUpdate(uint8_t mode, uint8_t speed, RGBColor mode_one, RGBColor mode_two, uint8_t brightness)
{
    SendApplyPacket(mode);

    unsigned char buffer[CM_MM_PACKET_SIZE]  = { 0x00, 0x51, command_code, 0x00, 0x00 };

    buffer[CM_MM_MODE_BYTE]                  = mode;
    buffer[CM_MM_SPEED_BYTE]                 = speed;
    buffer[CM_MM_NFI_1]                      = 0x00;
    buffer[CM_MM_NFI_2]                      = 0x21;
    buffer[CM_MM_NFI_3]                      = 0xFF;
    buffer[CM_MM_BRIGHTNESS_BYTE]            = brightness;
    buffer[CM_MM_RED_BYTE]                   = RGBGetRValue(mode_one);
    buffer[CM_MM_GREEN_BYTE]                 = RGBGetGValue(mode_one);
    buffer[CM_MM_BLUE_BYTE]                  = RGBGetBValue(mode_one);
    buffer[CM_MM_SKY_RED_BYTE]               = RGBGetRValue(mode_two);
    buffer[CM_MM_SKY_GREEN_BYTE]             = RGBGetGValue(mode_two);
    buffer[CM_MM_SKY_BLUE_BYTE]              = RGBGetBValue(mode_two);

    for (int i = 17; i < CM_MM_PACKET_SIZE; i++)
    {
        buffer[i] = 0xFF;
    }

    hid_write(dev, buffer, CM_MM_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM_PACKET_SIZE, CM_MM_INTERRUPT_TIMEOUT);
}

void CMMMController::SendInitPacket()
{
    unsigned char buffer[CM_MM_PACKET_SIZE]  = { 0x00, 0x41, 0x80 };

    hid_write(dev, buffer, CM_MM_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM_PACKET_SIZE, CM_MM_INTERRUPT_TIMEOUT);
}

void CMMMController::SendUsingZonesPacket(uint8_t mode)
{
    unsigned char buffer[CM_MM_PACKET_SIZE] = { 0x00, 0x51, 0x30, 0x00, 0x00 };

    if (mode == CM_MM_MODE_MULTILAYER)
    {
        buffer[CM_MM_MODE_BYTE] = 0x01;
    }

    hid_write(dev, buffer, CM_MM_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM_PACKET_SIZE, CM_MM_INTERRUPT_TIMEOUT);
}

void CMMMController::SendApplyPacket(uint8_t mode)
{
    unsigned char buffer[CM_MM_PACKET_SIZE]  = { 0x00, 0x51, 0x28, 0x00, 0x00 };

    buffer[CM_MM_MODE_BYTE]                  = mode;

    hid_write(dev, buffer, CM_MM_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM_PACKET_SIZE, CM_MM_INTERRUPT_TIMEOUT);
}

void CMMMController::SendMultilayerPacket(uint8_t zones[3])
{
    unsigned char buffer[CM_MM_PACKET_SIZE] = { 0x00, 0x51, 0xA0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00 };

    buffer[CM_MM_NFI_3]                     = zones[0];
    buffer[CM_MM_BRIGHTNESS_BYTE]           = zones[1];
    buffer[CM_MM_RED_BYTE]                  = zones[2];

    hid_write(dev, buffer, CM_MM_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM_PACKET_SIZE, CM_MM_INTERRUPT_TIMEOUT);
}

void CMMMController::SendSavePacket()
{
    unsigned char buffer[CM_MM_PACKET_SIZE]  = { 0x00, 0x50, 0x55 };

    hid_write(dev, buffer, CM_MM_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM_PACKET_SIZE, CM_MM_INTERRUPT_TIMEOUT);
}
