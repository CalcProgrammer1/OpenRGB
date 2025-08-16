/*---------------------------------------------------------*\
| SinowealthGMOWController.cpp                              |
|                                                           |
|   Driver for Glorious Model O Wireless                    |
|                                                           |
|   Matt Silva (thesilvanator)                     May 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <iostream>
#include "LogManager.h"
#include "SinowealthGMOWController.h"
#include "StringUtils.h"

SinowealthGMOWController::SinowealthGMOWController(hid_device* dev_handle, char *_path, int _type, std::string dev_name)
{
    dev         = dev_handle;
    location    = _path;
    name        = dev_name;
    type        = _type;

    memset(mode_packet,0x00, GMOW_PACKET_SIZE);
    mode_packet[0x03]  = 0x02;
    mode_packet[0x05]  = 0x02;
    mode_packet[0x07]  = 0x01;
    mode_packet[0x08]  = 0xFF;

    memset(wired_packet,0x00, GMOW_PACKET_SIZE);
    wired_packet[0x03] = 0x02;
    wired_packet[0x04] = 0x02;
    wired_packet[0x05] = 0x02;
    wired_packet[0x06] = 0x02;
    wired_packet[0x07] = 0x01;

    memcpy(less_packet, wired_packet, GMOW_PACKET_SIZE);
    less_packet[0x07]  = 0x00;
}

SinowealthGMOWController::~SinowealthGMOWController()
{
    hid_close(dev);
}

std::string SinowealthGMOWController::GetLocation()
{
    return("HID: " + location);
}

std::string SinowealthGMOWController::GetName()
{
    return(name);
}

std::string SinowealthGMOWController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string SinowealthGMOWController::GetFirmwareVersion()
{
    using namespace std::chrono_literals;

    unsigned char buf_send[GMOW_PACKET_SIZE] = {0};
    unsigned char buf_receive[GMOW_PACKET_SIZE] = {0};

    if(type == GMOW_CABLE_CONNECTED)
    {
        buf_send[0x03] = 0x02;
    }

    buf_send[0x04] = 0x03;
    buf_send[0x06] = 0x81;

    hid_send_feature_report(dev,buf_send, GMOW_PACKET_SIZE);

    std::this_thread::sleep_for(50ms);

    hid_get_feature_report(dev,buf_receive,GMOW_PACKET_SIZE);

    char str[128] = {0};
    snprintf(str,128,"%d.%d.%d.%d", buf_receive[7],
                                    buf_receive[8],
                                    buf_receive[9],
                                    buf_receive[10]);

    return std::string(str);
}

void SinowealthGMOWController::SetMode(unsigned char mode,
                                       unsigned char speed,
                                       unsigned char wired_brightness,
                                       unsigned char less_brightness,
                                       RGBColor* color_buf,
                                       unsigned char color_count)
{
    using namespace std::chrono_literals;

    unsigned char mode_buff[GMOW_PACKET_SIZE];
    memcpy(mode_buff, mode_packet, GMOW_PACKET_SIZE);
    mode_buff[0x09] = mode;

    unsigned char wired_buff[GMOW_PACKET_SIZE];
    memcpy(wired_buff, wired_packet, GMOW_PACKET_SIZE);
    wired_buff[0x08] = wired_brightness;

    unsigned char less_buff[GMOW_PACKET_SIZE];
    memcpy(less_buff, less_packet, GMOW_PACKET_SIZE);
    less_buff[0x08] = less_brightness;


    switch (mode)
    {
        case GMOW_MODE_RAINBOW_WAVE:
            mode_buff[0x04] = 0x05;
            mode_buff[0x0B] = speed;
            break;
        case GMOW_MODE_SPECTRUM_CYCLE:
            mode_buff[0x04] = 0x05;
            mode_buff[0x0B] = speed;
            mode_buff[0x0C] = 0xFF;
            break;
        case GMOW_MODE_CUSTOM_BREATHING:
            {
                mode_buff[0x04] = color_count * 3 + 5;
                mode_buff[0x0B] = speed;

                for(unsigned int i = 0; i < color_count; i++)
                {
                    unsigned char r = (char)RGBGetRValue(color_buf[i]);
                    unsigned char g = (char)RGBGetGValue(color_buf[i]);
                    unsigned char b = (char)RGBGetBValue(color_buf[i]);

                    if(r == 0x00 && g == 0x00 && b == 0x00)
                    {
                        r = 0x01;
                    }

                    mode_buff[0x0C + 3*i + 0] = r;
                    mode_buff[0x0C + 3*i + 1] = g;
                    mode_buff[0x0C + 3*i + 2] = b;
                }

            }
            break;
        case GMOW_MODE_STATIC:
            {
                mode_buff[0x04] = 0x05;
                mode_buff[0x0B] = 0x09; // rate is replaced with 9

                unsigned char r = (char)RGBGetRValue(color_buf[0]);
                unsigned char g = (char)RGBGetGValue(color_buf[0]);
                unsigned char b = (char)RGBGetBValue(color_buf[0]);

                if(r == 0 && g == 0 && b == 0)
                {
                    r = 1;
                }

                mode_buff[0x0C] = r;
                mode_buff[0x0D] = g;
                mode_buff[0x0E] = b;
            }
            break;
        case GMOW_MODE_BREATHING:
            {
                mode_buff[0x04] = 0x08;
                mode_buff[0x0B] = speed;

                unsigned char r = (char)RGBGetRValue(color_buf[0]);
                unsigned char g = (char)RGBGetGValue(color_buf[0]);
                unsigned char b = (char)RGBGetBValue(color_buf[0]);

                if(r == 0 && g == 0 && b == 0)
                {
                    r = 1;
                }

                mode_buff[0x0C] = r;
                mode_buff[0x0D] = g;
                mode_buff[0x0E] = b;
            }
            break;
        case GMOW_MODE_TAIL:
            mode_buff[0x04] = 0x05;
            mode_buff[0x0B] = speed;
            break;
        case GMOW_MODE_RAVE:
            {
                mode_buff[0x04] = color_count * 3 + 5;
                mode_buff[0x0B] = speed;

                for(unsigned int i = 0; i < color_count; i++) {
                    unsigned char r = (char)RGBGetRValue(color_buf[i]);
                    unsigned char g = (char)RGBGetGValue(color_buf[i]);
                    unsigned char b = (char)RGBGetBValue(color_buf[i]);

                    if(r == 0 && g == 0 && b == 0)
                    {
                        r = 1;
                    }

                    mode_buff[0x0C + 3*i + 0] = r;
                    mode_buff[0x0C + 3*i + 1] = g;
                    mode_buff[0x0C + 3*i + 2] = b;
                }
            }
            break;
        case GMOW_MODE_WAVE:
            mode_buff[0x04] = 0x05;
            mode_buff[0x0B] = speed;
            break;
        case GMOW_MODE_OFF:
            mode_buff[0x04] = 0x05;
            mode_buff[0x0B] = 0x09;
            break;
        default:
            break;
    }

    std::this_thread::sleep_for(50ms);
    hid_send_feature_report(dev, mode_buff, GMOW_PACKET_SIZE);

    std::this_thread::sleep_for(50ms);
    hid_send_feature_report(dev, wired_buff, GMOW_PACKET_SIZE);

    std::this_thread::sleep_for(50ms);
    hid_send_feature_report(dev, less_buff, GMOW_PACKET_SIZE);


}
