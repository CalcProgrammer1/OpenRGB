/*---------------------------------------------------------*\
| NZXTHue2Controller.cpp                                    |
|                                                           |
|   Driver for NZXT Hue 2                                   |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      29 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include "LogManager.h"
#include "NZXTHue2Controller.h"
#include "StringUtils.h"

NZXTHue2Controller::NZXTHue2Controller(hid_device* dev_handle, unsigned int rgb_channels, unsigned int fan_channels, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    num_fan_channels = fan_channels;
    num_rgb_channels = rgb_channels;

    SendFirmwareRequest();
    UpdateDeviceList();

    fan_cmd.resize(num_fan_channels);
    fan_rpm.resize(num_fan_channels);
    UpdateStatus();
}

NZXTHue2Controller::~NZXTHue2Controller()
{
    hid_close(dev);
}

unsigned char NZXTHue2Controller::GetFanCommand
    (
    unsigned char   fan_channel
    )
{
    return(fan_cmd[fan_channel]);
}

unsigned short NZXTHue2Controller::GetFanRPM
    (
    unsigned char   fan_channel
    )
{
    return(fan_rpm[fan_channel]);
}

std::string NZXTHue2Controller::GetLocation()
{
    return("HID: " + location);
}

std::string NZXTHue2Controller::GetName()
{
    return(name);
}

unsigned int NZXTHue2Controller::GetNumFanChannels()
{
    return(num_fan_channels);
}

unsigned int NZXTHue2Controller::GetNumRGBChannels()
{
    return(num_rgb_channels);
}

std::string NZXTHue2Controller::GetFirmwareVersion()
{
    return(firmware_version);
}

std::string NZXTHue2Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void NZXTHue2Controller::SendFan
    (
        unsigned char       port,
        unsigned char       /*mode*/,
        unsigned char       speed
    )
{
    unsigned char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up RGB packet                                     |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x62;
    usb_buf[0x01]   = 0x01;
    usb_buf[0x02]   = 1 << port;
    usb_buf[port + 3] = speed;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    hid_read(dev, usb_buf, 64);
}

void NZXTHue2Controller::UpdateDeviceList()
{
    unsigned char   usb_buf[64];
    unsigned int    ret_val = 0;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Device Information Request packet              |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x20;
    usb_buf[0x01]   = 0x03;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);

    /*-----------------------------------------------------*\
    | Receive packets until 0x21 0x03 is received           |
    \*-----------------------------------------------------*/
    do
    {
        ret_val = hid_read(dev, usb_buf, sizeof(usb_buf));
    } while( (ret_val != 64) || (usb_buf[0] != 0x21) || (usb_buf[1] != 0x03) );

    for(unsigned int chan = 0; chan < num_rgb_channels; chan++)
    {
        unsigned int start = 0x0F + (6 * chan);
        unsigned int num_leds_on_channel = 0;

        for(int dev = 0; dev < 6; dev++)
        {
            unsigned int num_leds_in_device = 0;

            switch(usb_buf[start + dev])
            {
            case 0x01: //Hue 1 strip
                num_leds_in_device = 10;
                break;

            case 0x02: //Aer 1 fan
                num_leds_in_device = 8;
                break;

            case 0x04: //Hue 2 strip (10 LEDs)
                num_leds_in_device = 10;
                break;

            case 0x05: //Hue 2 strip (8 LEDs)
                num_leds_in_device = 8;
                break;

            case 0x06: //Hue 2 strip (6 LEDs)
                num_leds_in_device = 6;
                break;

            case 0x08: //Hue 2 Cable Comb (14 LEDs)
                num_leds_in_device = 14;
                break;

            case 0x09: //Hue 2 Underglow (300mm) (15 LEDs)
                num_leds_in_device = 15;
                break;

            case 0x0A: //Hue 2 Underglow (200mm) (10 LEDs)
                num_leds_in_device = 10;
                break;

            case 0x0B: //Aer 2 fan (120mm)
                num_leds_in_device = 8;
                break;

            case 0x0C: //Aer 2 fan (140mm)
                num_leds_in_device = 8;
                break;

            case 0x10: //Kraken X3 ring
                num_leds_in_device = 8;
                break;

            case 0x11: //Kraken X3 logo
                num_leds_in_device = 1;
                break;

            case 0x13: //F120 RGB fan (120mm)
                num_leds_in_device = 18;
                break;

            case 0x14: //F140 RGB fan (140mm)
                num_leds_in_device = 18;
                break;

            case 0x15: //F120 RGB Duo fan (120mm)
                num_leds_in_device = 20;
                break;

            case 0x16: //F140 RGB Duo fan (140mm)
                num_leds_in_device = 20;
                break;

            case 0x17: //F120 RGB Core fan (120mm)
                num_leds_in_device = 8;
                break;

            case 0x18: //F140 RGB Core fan (140mm)
                num_leds_in_device = 8;
                break;

            case 0x19: //F120 RGB Core fan case version (120mm)
                num_leds_in_device = 8;
                break;

            case 0x1D: //F360 RGB Core Fan Case Version (360mm)
                num_leds_in_device = 24;
                break;

            case 0x1E: //Kraken Elite Ring
                num_leds_in_device = 24;
                break;

            default:
                break;
            }

            channel_dev_ids[chan][dev] = usb_buf[start + dev];
            channel_dev_szs[chan][dev] = num_leds_in_device;

            LOG_DEBUG("[NZXT Hue 2] %d: Device ID: %02X LEDs: %d", dev, usb_buf[start + dev], num_leds_in_device);

            num_leds_on_channel += num_leds_in_device;
        }

        channel_leds[chan] = num_leds_on_channel;
    }
}

void NZXTHue2Controller::UpdateStatus()
{
    unsigned char usb_buf[64];
    unsigned int  ret_val = 0;

    if(false)//num_fan_channels > 0)
    {
        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | Read packet                                           |
        \*-----------------------------------------------------*/
        do
        {
            ret_val = hid_read(dev, usb_buf, sizeof(usb_buf));
        } while( (ret_val != 64) || (usb_buf[0] != 0x67) || (usb_buf[1] != 0x02) );

        /*-----------------------------------------------------*\
        | Extract fan information                               |
        \*-----------------------------------------------------*/
        for(unsigned int fan_idx = 0; fan_idx < num_fan_channels; fan_idx++)
        {
            unsigned char  cmd;
            unsigned short rpm;

            cmd = usb_buf[40 + fan_idx];
            rpm = ( usb_buf[25 + (2 * fan_idx)] << 8 ) | usb_buf[24 + (2 * fan_idx)];

            fan_cmd[fan_idx] = cmd;
            fan_rpm[fan_idx] = rpm;
        }
    }
}

void NZXTHue2Controller::SetChannelEffect
    (
    unsigned char   channel,
    unsigned char   mode,
    unsigned char   speed,
    bool            direction,
    RGBColor *      colors,
    unsigned int    num_colors
    )
{
    unsigned char color_data[24];

    /*-----------------------------------------------------*\
    | Fill in color data (up to 8 colors)                  |
    \*-----------------------------------------------------*/
    for (std::size_t idx = 0; idx < num_colors; idx++)
    {
        int pixel_idx = (int)idx * 3;
        RGBColor color = colors[idx];
        color_data[pixel_idx + 0x00] = RGBGetGValue(color);
        color_data[pixel_idx + 0x01] = RGBGetRValue(color);
        color_data[pixel_idx + 0x02] = RGBGetBValue(color);
    }

    /*-----------------------------------------------------*\
    | Send effect packet                                    |
    \*-----------------------------------------------------*/
    SendEffect(channel, mode, speed, direction, num_colors, &color_data[0]);
}

void NZXTHue2Controller::SetChannelLEDs
    (
    unsigned char   channel,
    RGBColor *      colors,
    unsigned int    num_colors
    )
{
    unsigned char color_data[120];

    /*-----------------------------------------------------*\
    | Fill in color data (up to 40 colors)                  |
    \*-----------------------------------------------------*/
    for (std::size_t idx = 0; idx < num_colors; idx++)
    {
        int pixel_idx = (int)idx * 3;
        RGBColor color = colors[idx];
        color_data[pixel_idx + 0x00] = RGBGetGValue(color);
        color_data[pixel_idx + 0x01] = RGBGetRValue(color);
        color_data[pixel_idx + 0x02] = RGBGetBValue(color);
    }

    /*-----------------------------------------------------*\
    | Send first group of color data                        |
    \*-----------------------------------------------------*/
    SendDirect(channel, 0, 20, &color_data[0]);

    /*-----------------------------------------------------*\
    | Send second group of color data if necessary          |
    \*-----------------------------------------------------*/
    if(num_colors > 20)
    {
        SendDirect(channel, 1, 20, &color_data[60]);
    }

    /*-----------------------------------------------------*\
    | Send apply packet                                     |
    \*-----------------------------------------------------*/
    SendApply(channel);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void NZXTHue2Controller::SendApply
    (
    unsigned char   channel
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Apply packet                                   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x22;
    usb_buf[0x01]   = 0xA0;
    usb_buf[0x02]   = (unsigned char)(1 << channel);
    usb_buf[0x04]   = 0x01;
    usb_buf[0x07]   = 0x28;
    usb_buf[0x0A]   = 0x80;
    usb_buf[0x0C]   = 0x32;
    usb_buf[0x0F]   = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    //hid_read(dev, usb_buf, 64);
}

void NZXTHue2Controller::SendDirect
    (
    unsigned char   channel,
    unsigned char   group,
    unsigned char   color_count,
    unsigned char*  color_data
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x22;
    usb_buf[0x01]   = 0x10 | group;
    usb_buf[0x02]   = (unsigned char)(1 << channel);
    usb_buf[0x03]   = 0x00;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x04], color_data, color_count * 3);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    //hid_read(dev, usb_buf, 64);
}

void NZXTHue2Controller::SendEffect
    (
    unsigned char   channel,
    unsigned char   mode,
    unsigned char   speed,
    bool            direction,
    unsigned char   color_count,
    unsigned char*  color_data
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Effect packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x28;
    usb_buf[0x01]   = 0x03;
    usb_buf[0x02]   = (unsigned char)(1 << channel);
    usb_buf[0x03]   = 0x28;

    /*-----------------------------------------------------*\
    | Set mode in USB packet                                |
    \*-----------------------------------------------------*/
    usb_buf[0x04]   = mode;

    /*-----------------------------------------------------*\
    | Set speed in USB packet                               |
    \*-----------------------------------------------------*/
    usb_buf[0x05]   = speed;

    /*-----------------------------------------------------*\
    | Set moving flag to true in USB packet                 |
    \*-----------------------------------------------------*/
    usb_buf[0x06]   = true;

    /*-----------------------------------------------------*\
    | Set direction in USB packet                           |
    \*-----------------------------------------------------*/
    usb_buf[0x07]   = direction ? 0x01 : 0x00;

    /*-----------------------------------------------------*\
    | Set color count in USB packet                         |
    \*-----------------------------------------------------*/
    usb_buf[0x08]   = color_count;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x0A], color_data, color_count * 3);

    hid_write(dev, usb_buf, 64);
    //hid_read(dev, usb_buf, 64);
}

void NZXTHue2Controller::SendFirmwareRequest()
{
    unsigned char   usb_buf[64];
    unsigned int    ret_val = 0;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Firmware Request packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x10;
    usb_buf[0x01]   = 0x01;

    hid_write(dev, usb_buf, 64);

    /*-----------------------------------------------------*\
    | Receive packets until 0x11 0x01 is received           |
    \*-----------------------------------------------------*/
    do
    {
        ret_val = hid_read(dev, usb_buf, sizeof(usb_buf));
    } while( (ret_val != 64) || (usb_buf[0] != 0x11) || (usb_buf[1] != 0x01) );

    snprintf(firmware_version, 16, "%u.%u.%u", usb_buf[0x11], usb_buf[0x12], usb_buf[0x13]);
}
