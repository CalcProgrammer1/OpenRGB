/*---------------------------------------------------------*\
| MSIVigorGK30Controller.cpp                                |
|                                                           |
|   Driver for MSI Vigor GK30                               |
|                                                           |
|   Morgan Guimard (morg)                       01 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cmath>
#include <string.h>
#include "MSIVigorGK30Controller.h"
#include "StringUtils.h"

static unsigned char argb_colour_index_data[2][2][2] =
{     //B0    B1
      { { 0x00, 0x04 },         //G0 R0
        { 0x02, 0x03 }, },      //G1 R0
      { { 0x00, 0x05 },         //G0 R1
        { 0x01, 0x06 }, }       //G1 R1
};

MSIVigorGK30Controller::MSIVigorGK30Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev         = dev_handle;
    location    = info.path;
    name        = dev_name;
}

MSIVigorGK30Controller::~MSIVigorGK30Controller()
{
    hid_close(dev);
}

std::string MSIVigorGK30Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSIVigorGK30Controller::GetNameString()
{
    return(name);
}

std::string MSIVigorGK30Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned int MSIVigorGK30Controller::GetLargestColour(unsigned int red, unsigned int green, unsigned int blue)
{
    unsigned int largest;

    if ( red > green )
    {
        ( red > blue ) ? largest = red : largest = blue;
    }
    else
    {
        ( green > blue ) ? largest = green : largest = blue;
    }

    return (largest == 0) ? 1 : largest;
}

unsigned char MSIVigorGK30Controller::GetColourIndex(unsigned char red, unsigned char green, unsigned char blue)
{
    /*-----------------------------------------------------*\
    | This device uses a limited colour pallette referenced |
    | by an index                                           |
    | 0x00 red                                              |
    | 0x01 yellow                                           |
    | 0x02 green                                            |
    | 0x03 cyan                                             |
    | 0x04 blue                                             |
    | 0x05 magen                                            |
    | 0x06 white                                            |
    \*-----------------------------------------------------*/
    unsigned int divisor    = GetLargestColour( red, green, blue);
    unsigned int r          = (unsigned int)round( red / divisor );
    unsigned int g          = (unsigned int)round( green / divisor );
    unsigned int b          = (unsigned int)round( blue / divisor );
    unsigned char idx       = argb_colour_index_data[r][g][b];
    return idx;
}

void MSIVigorGK30Controller::SetMode(std::vector<RGBColor> colors, unsigned char brightness, unsigned char speed, unsigned char mode_value, unsigned int mode_flags, unsigned int color_mode, unsigned char direction)
{
    unsigned char usb_buf[MSI_VIGOR_GK30_REPORT_SIZE];

    memset(usb_buf, 0x00, MSI_VIGOR_GK30_REPORT_SIZE);

    usb_buf[0x00] = MSI_VIGOR_GK30_REPORT_ID;
    usb_buf[0x01] = 0xFF;
    usb_buf[0x02] = mode_value;

    /*-----------------------------------------------------*\
    | Parse one color for specific color mode               |
    \*-----------------------------------------------------*/
    if(color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        /*-----------------------------------------------------*\
        | Use OFF mode if color is black, black isnt an indexed |
        | color                                                 |
        \*-----------------------------------------------------*/
        if(colors[0] == 0)
        {
            usb_buf[0x02] = MSI_VIGOR_GK30_OFF_MODE_VALUE;

            hid_send_feature_report(dev, usb_buf, MSI_VIGOR_GK30_REPORT_SIZE);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            return;
        }

        unsigned char red = RGBGetRValue(colors[0]);
        unsigned char grn = RGBGetGValue(colors[0]);
        unsigned char blu = RGBGetBValue(colors[0]);

        usb_buf[0x03] = GetColourIndex(red, grn, blu);
    }
    else if(color_mode == MODE_COLORS_PER_LED)
    {
        /*-----------------------------------------------------*\
        | Use OFF mode if color is black, black isnt an indexed |
        | color                                                 |
        \*-----------------------------------------------------*/
        unsigned int black_count = 0;

        for(unsigned int i = 0; i < MSI_VIGOR_GK30_LEDS_COUNT; i ++)
        {
            if(colors[i] == 0)
            {
                black_count++;
            }
        }

        if( black_count == MSI_VIGOR_GK30_LEDS_COUNT )
        {
            usb_buf[0x02] = MSI_VIGOR_GK30_OFF_MODE_VALUE;

            hid_send_feature_report(dev, usb_buf, MSI_VIGOR_GK30_REPORT_SIZE);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            return;
        }

        /*-----------------------------------------------------*\
        | Parse and set the 6 colors in the buffer              |
        \*-----------------------------------------------------*/
        unsigned int offset = 0;

        for(unsigned int i = 0; i < MSI_VIGOR_GK30_LEDS_COUNT; i ++)
        {
            unsigned char red = RGBGetRValue(colors[i]);
            unsigned char grn = RGBGetGValue(colors[i]);
            unsigned char blu = RGBGetBValue(colors[i]);

            unsigned char index = GetColourIndex(red, grn, blu);

            if(offset % 2 == 0)
            {
                index <<= 4;
            }

            usb_buf[0x03 + offset/2] += index;

            offset++;
        }
    }

    /*-----------------------------------------------------*\
    | color index is shifted in breathing mode              |
    | 0x00 becomes random                                   |
    \*-----------------------------------------------------*/
    if(mode_value == MSI_VIGOR_GK30_BREATHING_MODE_VALUE)
    {
        if(color_mode == MODE_COLORS_RANDOM)
        {
            usb_buf[0x03] = 0x00;
        }
        else
        {
            usb_buf[0x03]++;
        }
    }

    /*-----------------------------------------------------*\
    | Direction byte                                        |
    \*-----------------------------------------------------*/
    if(mode_flags & MODE_FLAG_HAS_DIRECTION_LR)
    {
        usb_buf[0x04] = direction;
    }

    /*-----------------------------------------------------*\
    | Speed byte                                            |
    \*-----------------------------------------------------*/
    if(mode_flags & MODE_FLAG_HAS_SPEED)
    {
        usb_buf[0x05] = speed;
    }

    /*-----------------------------------------------------*\
    | Brightness byte                                       |
    \*-----------------------------------------------------*/
    if(mode_flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        usb_buf[0x06] = brightness;
    }

    hid_send_feature_report(dev, usb_buf, MSI_VIGOR_GK30_REPORT_SIZE);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
