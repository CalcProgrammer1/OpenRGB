/*---------------------------------------------------------*\
| CorsairK95PlatinumXTController.cpp                        |
|                                                           |
|   Driver for Corsair K95 Platinum XT keyboard             |
|                                                           |
|   Morgan Guimard (morg)                       07 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "CorsairK95PlatinumXTController.h"
#include "StringUtils.h"

CorsairK95PlatinumXTController::CorsairK95PlatinumXTController(hid_device* dev_handle, const char* path)
{
    dev             = dev_handle;
    location        = path;

    Init();
}

CorsairK95PlatinumXTController::~CorsairK95PlatinumXTController()
{
    hid_close(dev);
}

std::string CorsairK95PlatinumXTController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CorsairK95PlatinumXTController::GetFirmwareString()
{
    return "";
}

std::string CorsairK95PlatinumXTController::GetName()
{
    return name;
}

std::string CorsairK95PlatinumXTController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void CorsairK95PlatinumXTController::Init()
{
    unsigned char usb_buf[K95_PLATINUM_XT_REPORT_LENGTH];

    memset(usb_buf, 0x00, K95_PLATINUM_XT_REPORT_LENGTH);
    usb_buf[0x01] = K95_PLATINUM_XT_REPORT_ID;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = 0x03;
    usb_buf[0x05] = 0x02;
    hid_write(dev, usb_buf, K95_PLATINUM_XT_REPORT_LENGTH);

    memset(usb_buf, 0x00, K95_PLATINUM_XT_REPORT_LENGTH);
    usb_buf[0x01] = K95_PLATINUM_XT_REPORT_ID;
    usb_buf[0x02] = 0x02;
    usb_buf[0x03] = 0x6E;
    hid_write(dev, usb_buf, K95_PLATINUM_XT_REPORT_LENGTH);

    memset(usb_buf, 0x00, K95_PLATINUM_XT_REPORT_LENGTH);
    usb_buf[0x01] = K95_PLATINUM_XT_REPORT_ID;
    usb_buf[0x02] = 0x0D;
    usb_buf[0x03] = 0x01;
    usb_buf[0x04] = 0x01;
    hid_write(dev, usb_buf, K95_PLATINUM_XT_REPORT_LENGTH);
}

void CorsairK95PlatinumXTController::SendDirect(const std::vector<RGBColor>& colors, const std::vector<std::tuple<std::string,unsigned int>>& leds_positions)
{
    /*-----------------------------------------------------*\
    | RGB indexes                                           |
    \*-----------------------------------------------------*/
    unsigned int red_start = 0x008;
    unsigned int grn_start = 0x0A4;
    unsigned int blu_start = 0x140;

    /*-----------------------------------------------------*\
    | Fill the color data array                             |
    \*-----------------------------------------------------*/
    unsigned char color_data[K95_PLATINUM_XT_COLOR_DATA_LENGTH];
    memset(color_data, 0x00, K95_PLATINUM_XT_COLOR_DATA_LENGTH);

    color_data[0x00] = 0xD4;
    color_data[0x01] = 0x01;

    for(unsigned int c = 0; c < colors.size(); c++)
    {
        unsigned char red               = RGBGetRValue(colors[c]);
        unsigned char grn               = RGBGetGValue(colors[c]);
        unsigned char blu               = RGBGetBValue(colors[c]);
        unsigned int  led_pos           = std::get<1>(leds_positions[c]);

        color_data[led_pos + red_start] = red;
        color_data[led_pos + grn_start] = grn;
        color_data[led_pos + blu_start] = blu;
    }

    /*-----------------------------------------------------*\
    | Send the 8 pages of colors                            |
    \*-----------------------------------------------------*/
    unsigned char usb_buf[K95_PLATINUM_XT_REPORT_LENGTH];

    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = K95_PLATINUM_XT_REPORT_ID;
    usb_buf[0x02] = 0x06;
    usb_buf[0x03] = 0x01;

    for(unsigned int i = 0; i < K95_PLATINUM_XT_NUMBER_OF_PAGES; i++)
    {
        /*-----------------------------------------------------*\
        | Copy colors in current page                           |
        \*-----------------------------------------------------*/
        memcpy
        (
            &usb_buf[0x04],
            &color_data[(K95_PLATINUM_XT_REPORT_LENGTH - 4) * i],
            K95_PLATINUM_XT_REPORT_LENGTH - 4
        );

        /*-----------------------------------------------------*\
        | Send the buffer                                       |
        \*-----------------------------------------------------*/
        hid_write(dev, usb_buf, K95_PLATINUM_XT_REPORT_LENGTH);

        /*-----------------------------------------------------*\
        | Next pages start with 0x07                            |
        \*-----------------------------------------------------*/
        usb_buf[0x02] = 0x07;
    }

}
