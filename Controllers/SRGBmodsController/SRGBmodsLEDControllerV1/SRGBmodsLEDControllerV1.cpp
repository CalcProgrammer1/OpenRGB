/*---------------------------------------------------------*\
| SRGBmodsLEDControllerV1.cpp                               |
|                                                           |
|   Driver for SRGBmods LED Controller V1                   |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "SRGBmodsLEDControllerV1.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

SRGBmodsLEDControllerV1::SRGBmodsLEDControllerV1(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

SRGBmodsLEDControllerV1::~SRGBmodsLEDControllerV1()
{
    hid_close(dev);
}

std::string SRGBmodsLEDControllerV1::GetLocationString()
{
    return("HID: " + location);
}

std::string SRGBmodsLEDControllerV1::GetNameString()
{
    return(name);
}

std::string SRGBmodsLEDControllerV1::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void SRGBmodsLEDControllerV1::SetChannelLEDs(unsigned char /*channel*/, RGBColor* colors, unsigned int num_colors)
{
    /*-----------------------------------------------------*\
    | Determine number of packets to send                   |
    \*-----------------------------------------------------*/
    unsigned int num_packets    = (num_colors / 20) + ((num_colors % 20) > 0);
    unsigned int color_idx      = 0;

    /*-----------------------------------------------------*\
    | Send direct mode packets until all colors sent        |
    \*-----------------------------------------------------*/
    for(unsigned int packet_idx = 0; packet_idx < num_packets; packet_idx++)
    {
        unsigned int colors_in_packet = 20;

        if(num_colors - color_idx < colors_in_packet)
        {
            colors_in_packet = num_colors - color_idx;
        }

        SendPacket(packet_idx + 1, num_packets, false, &colors[color_idx], colors_in_packet);

        color_idx += colors_in_packet;
    }
}

void SRGBmodsLEDControllerV1::SetConfiguration(unsigned char mode, unsigned char speed, unsigned char brightness, RGBColor color)
{
    SendConfiguration(0, 1, mode, speed, brightness, color, 0, 0);
}

void SRGBmodsLEDControllerV1::SetDirect()
{
    /*-----------------------------------------------------*\
    | Disable hardware lighting and color compression       |
    \*-----------------------------------------------------*/
    SendConfiguration(0, 0, 0, 0, 0, 0, 0, 0);
}

void SRGBmodsLEDControllerV1::SendPacket
    (
    unsigned char   this_packet_id,
    unsigned char   last_packet_id,
    bool            reset,
    RGBColor*       colors,
    unsigned int    num_colors
    )
{
    unsigned char   usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Direct Lighting packet                         |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;             /* hidapi Report ID*/
    usb_buf[0x01]   = this_packet_id;   /* This Packet ID  */
    usb_buf[0x02]   = last_packet_id;   /* Last Packet ID  */
    usb_buf[0x03]   = reset;            /* Reset Flag      */
    usb_buf[0x04]   = 0xAA;             /* Color update    */

    for(unsigned int color_idx = 0; color_idx < num_colors; color_idx++)
    {
        usb_buf[0x05 + (color_idx * 3)] = RGBGetRValue(colors[color_idx]);
        usb_buf[0x06 + (color_idx * 3)] = RGBGetGValue(colors[color_idx]);
        usb_buf[0x07 + (color_idx * 3)] = RGBGetBValue(colors[color_idx]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}

void SRGBmodsLEDControllerV1::SendConfiguration
    (
    bool            reset,
    unsigned char   hw_effect_enable,
    unsigned char   hw_effect_mode,
    unsigned char   hw_effect_speed,
    unsigned char   hw_effect_brightness,
    RGBColor        hw_effect_color,
    unsigned char   status_led_enable,
    unsigned char   color_compression_enable
    )
{
    unsigned char   usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Hardware Configuration packet                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;             /* hidapi Report ID*/
    usb_buf[0x01]   = 0x00;             /* This Packet ID  */
    usb_buf[0x02]   = 0x00;             /* Last Packet ID  */
    usb_buf[0x03]   = reset;            /* Reset Flag      */
    usb_buf[0x04]   = 0xBB;             /* Config update   */

    usb_buf[0x05]   = hw_effect_enable; /* HWL_enable      */
    usb_buf[0x08]   = hw_effect_mode;   /* HWL_effectMode  */
    usb_buf[0x09]   = hw_effect_speed;  /* HWL_effectSpeed */
    usb_buf[0x0A]   = hw_effect_brightness;
    usb_buf[0x0B]   = RGBGetRValue(hw_effect_color);
    usb_buf[0x0C]   = RGBGetGValue(hw_effect_color);
    usb_buf[0x0D]   = RGBGetBValue(hw_effect_color);
    usb_buf[0x0E]   = status_led_enable;
    usb_buf[0x0F]   = color_compression_enable;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Delay 200ms                                           |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(200ms);
}
