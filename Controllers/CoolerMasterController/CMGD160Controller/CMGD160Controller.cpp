/*---------------------------------------------------------*\
| CMGD160Controller.cpp                                     |
|                                                           |
|   Driver for Cooler Master GD160 ARGB Gaming Desk         |
|                                                           |
|   Logan Phillips (Eclipse)                    16 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   Adapted from CMMonitor controller code                  |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "CMGD160Controller.h"
#include "StringUtils.h"

CMGD160Controller::CMGD160Controller(hid_device* dev_handle, const hid_device_info& info, const std::string& name)
{
    dev                 = dev_handle;
    device_name         = name;
    location            = info.path;
    ResetDevice();
}

CMGD160Controller::~CMGD160Controller()
{
    hid_close(dev);
}

std::string CMGD160Controller::GetDeviceName()
{
    return(device_name);
}

std::string CMGD160Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CMGD160Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

/*------------------------------------------------------------*\
| Desk requires 2 sets of packets sent for the front and back  |
| Technically you could have both sides do something different |
| Not sure why you would though....                            |
| Cooler Master's software doesn't allow that anyways          |
\*------------------------------------------------------------*/

void CMGD160Controller::SetMode(uint8_t mode_value, uint8_t speed, uint8_t brightness, const RGBColor& color)
{
    if(is_software_mode_enabled)
    {
        SetControlMode(false);
    }

    uint8_t usb_buf[CM_GD160_PACKET_LENGTH];

    for(int side = 1; side <= 2; side++)
    {
        memset(usb_buf, 0x00, CM_GD160_PACKET_LENGTH);

        usb_buf[1]  = 0x80;
        usb_buf[2]  = (mode_value == CM_GD160_OFF_MODE) ? 0x0F : 0x0B;
        usb_buf[3]  = 0x02;
        usb_buf[4]  = side;  // 0x01 for front, 0x02 for back
        usb_buf[5]  = mode_value;
        usb_buf[6]  = (mode_value == CM_GD160_OFF_MODE) ? 0x00 : 0x08;
        usb_buf[7]  = speed;
        usb_buf[8]  = brightness;
        usb_buf[9]  = RGBGetRValue(color);
        usb_buf[10] = RGBGetGValue(color);
        usb_buf[11] = RGBGetBValue(color);

        hid_write(dev, usb_buf, CM_GD160_PACKET_LENGTH);
    }
}

/*-------------------------------------------------*\
| How to request current color in custom mode.      |
| Not like it matters since we default to direct    |
| mode and it seems to clear the current colors...  |
|                                                   |
| memset(usb_buf, 0x00, CM_GD160_PACKET_LENGTH);    |
| usb_buf[1] = 0x80;                                |
| usb_buf[2] = 0x10;                                |
| usb_buf[3] = 0x01; or 0x02                        |
| usb_buf[4] = 0x02;                                |
| usb_buf[5] = 0x80;                                |
| hid_write(dev, usb_buf, CM_GD160_PACKET_LENGTH);  |
\*-------------------------------------------------*/

void CMGD160Controller::SendColorData(const std::vector<RGBColor>& colors, uint8_t command, uint8_t mode_byte, uint8_t brightness, bool desired_control_mode)
{
    if(is_software_mode_enabled != desired_control_mode)
    {
        SetControlMode(desired_control_mode);
    }

    uint8_t color_data[CM_GD160_COLOR_DATA_LENGTH];
    memset(color_data, 0x00, CM_GD160_COLOR_DATA_LENGTH);

    for(unsigned int i = 0; i < colors.size() && i < (CM_GD160_LEDS_PER_SIDE * 2); i++)
    {
        unsigned int side             = i / CM_GD160_LEDS_PER_SIDE;
        unsigned int led_in_side      = i % CM_GD160_LEDS_PER_SIDE;
        unsigned int buffer_offset    = (side * CM_GD160_SIDE_DATA_LENGTH) + (led_in_side * 3);

        color_data[buffer_offset]     = RGBGetRValue(colors[i]);
        color_data[buffer_offset + 1] = RGBGetGValue(colors[i]);
        color_data[buffer_offset + 2] = RGBGetBValue(colors[i]);
    }

    uint8_t usb_buf[CM_GD160_PACKET_LENGTH];

    for(unsigned int side = 1; side <= 2; side++)
    {
        unsigned int offset = (side - 1) * CM_GD160_SIDE_DATA_LENGTH;

        for(unsigned int packet = 0; packet < 7; packet++)
        {
            memset(usb_buf, 0x00, CM_GD160_PACKET_LENGTH);

            usb_buf[1] = (packet < 6) ? packet : 0x86;  // Last packet uses 0x86

            /*---------------------------------------------------------*\
            | First packet contains static data                         |
            \*---------------------------------------------------------*/

            if(packet == 0)
            {
                usb_buf[2] = command;
                usb_buf[3] = 0x02;
                usb_buf[4] = side;  // 0x01 for front, 0x02 for back
                usb_buf[5] = mode_byte;
                usb_buf[6] = brightness;

                memcpy(&usb_buf[7], &color_data[offset], CM_GD160_FIRST_PACKET_DATA_SIZE);
                offset += CM_GD160_FIRST_PACKET_DATA_SIZE;
            }
            else
            {
                memcpy(&usb_buf[2], &color_data[offset], CM_GD160_PACKET_DATA_SIZE);
                offset += CM_GD160_PACKET_DATA_SIZE;
            }

            hid_write(dev, usb_buf, CM_GD160_PACKET_LENGTH);
        }
    }
}

/*------------------------------------------------------*\
| True enables software mode                             |
| False enables hardware mode                            |
\*------------------------------------------------------*/

void CMGD160Controller::SetControlMode(bool software_mode)
{
    uint8_t usb_buf[CM_GD160_PACKET_LENGTH];

    for(int side = 1; side <= 2; side++)
    {
        memset(usb_buf, 0x00, CM_GD160_PACKET_LENGTH);

        usb_buf[1]  = 0x80;
        usb_buf[2]  = 0x07;
        usb_buf[3]  = 0x02;
        usb_buf[4]  = side;  // 0x01 for front, 0x02 for back
        usb_buf[6]  = software_mode;

        hid_write(dev, usb_buf, CM_GD160_PACKET_LENGTH);
    }

    is_software_mode_enabled = software_mode;
}

/*------------------------------------------------------*\
| Reset device on discovery in case it somehow landed    |
| in a bad / unresponsive state                          |
\*------------------------------------------------------*/

void CMGD160Controller::ResetDevice()
{
    uint8_t usb_buf[CM_GD160_PACKET_LENGTH];

    memset(usb_buf, 0x00, CM_GD160_PACKET_LENGTH);
    usb_buf[1] = 0x80;
    usb_buf[2] = 0x11;
    hid_write(dev, usb_buf, CM_GD160_PACKET_LENGTH);

    memset(usb_buf, 0x00, CM_GD160_PACKET_LENGTH);
    usb_buf[1] = 0x80;
    usb_buf[2] = 0x0B;
    usb_buf[3] = 0x01;
    usb_buf[4] = 0x02;
    hid_write(dev, usb_buf, CM_GD160_PACKET_LENGTH);

    memset(usb_buf, 0x00, CM_GD160_PACKET_LENGTH);
    usb_buf[1] = 0x80;
    usb_buf[2] = 0x18;
    usb_buf[3] = 0x01;
    usb_buf[4] = 0x02;
    hid_write(dev, usb_buf, CM_GD160_PACKET_LENGTH);

    is_software_mode_enabled = false;
}
