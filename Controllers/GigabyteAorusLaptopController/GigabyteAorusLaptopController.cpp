/*---------------------------------------------------------*\
| GigabyteAorusLaptopController.cpp                         |
|                                                           |
|   Driver for Gigabyte Aorus laptop                        |
|                                                           |
|   Morgan Guimard (morg)                       05 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cmath>
#include <string.h>
#include "GigabyteAorusLaptopController.h"
#include "StringUtils.h"

/*---------------------------------------------------------*\
| Indexed colors mapping                                    |
| blue        04                                            |
| green       02                                            |
| orange      05                                            |
| purple      06                                            |
| red         01                                            |
| white       07                                            |
| yellow      03                                            |
\*---------------------------------------------------------*/
static unsigned char argb_colour_index_data[2][2][2] =
    {       //B0    B1
        { { 0x01, 0x04 },         //G0 R0
            { 0x02, 0x04 }, },      //G1 R0
        { { 0x01, 0x06 },         //G0 R1
            { 0x05, 0x07 }, }       //G1 R1
};

GigabyteAorusLaptopController::GigabyteAorusLaptopController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

GigabyteAorusLaptopController::~GigabyteAorusLaptopController()
{
    hid_close(dev);
}

std::string GigabyteAorusLaptopController::GetNameString()
{
    return(name);
}

std::string GigabyteAorusLaptopController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string GigabyteAorusLaptopController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void GigabyteAorusLaptopController::SetDirect(uint8_t brightness, RGBColor color)
{
    /*---------------------------------------------------------*\
    | Direct mode protocol                                      |
    |    ID C  R  G  B  Br C  Ch                                |
    |    08 01 00 00 FF 32 00 C5                                |
    |                                                           |
    |    C = constant                                           |
    |    RGB = color                                            |
    |    Br= brightness                                         |
    |    Ch = checksum: 0xFF - all bytes                        |
    \*---------------------------------------------------------*/

    unsigned char usb_buf[GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1];
    memset(usb_buf, 0x00, GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1);

    usb_buf[1] = GIGABYTE_AORUS_LAPTOP_REPORT_ID;
    usb_buf[2] = 0x01;
    usb_buf[3] = RGBGetRValue(color);
    usb_buf[4] = RGBGetGValue(color);
    usb_buf[5] = RGBGetBValue(color);
    usb_buf[6] = brightness;

    unsigned char checksum = 0xFF;

    for(unsigned int i = 1; i < 8; i++)
    {
        checksum -= usb_buf[i];
    }

    usb_buf[8] = checksum;

    hid_send_feature_report(dev, usb_buf, GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1);
}

void GigabyteAorusLaptopController::SetMode(uint8_t mode_value, uint8_t speed,  uint8_t brightness, uint8_t direction, RGBColor color)
{
    /*---------------------------------------------------------*\
    | Hardware mode protocol                                    |
    |    ID C  M  Sp Br Cl Dr  Ch                               |
    |    08 00 01 08 32 01 00 XX                                |
    |                                                           |
    |    C = constant                                           |
    |    M = mode                                               |
    |    Sp = speed                                             |
    |    Br = brightness                                        |
    |    Cl = Indexed color                                     |
    |    Dr = Direction                                         |
    |    Ch = checksum: 0xFF - all bytes                        |
    \*---------------------------------------------------------*/

    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    unsigned char indexed_color = GetColourIndex(red,grn,blu);

    unsigned char usb_buf[GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1];
    memset(usb_buf, 0x00, GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1);

    usb_buf[1] = GIGABYTE_AORUS_LAPTOP_REPORT_ID;              // report id

    usb_buf[3] = mode_value;        // mode value
    usb_buf[4] = speed;             // speed 0x01 -> 0x09
    usb_buf[5] = brightness;        // brightness 0x00 -> 0x32
    usb_buf[6] = indexed_color;     // color  (red, orange, yellow, green, blue, purple, white)
    usb_buf[7] = direction;         // direction 0x01: right, 0x02 left, 0x03 up , 0x04 down

    unsigned char checksum = 0xFF;

    for(unsigned int i = 1; i < 8; i++)
    {
        checksum -= usb_buf[i];
    }

    usb_buf[8] = checksum;          // checksum

    hid_send_feature_report(dev, usb_buf, GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1);
}

unsigned char GigabyteAorusLaptopController::GetColourIndex(unsigned char red, unsigned char green, unsigned char blue)
{
    /*-----------------------------------------------------*\
    | This device uses a limited colour pallette referenced |
    | by an index                                           |
    | 0x01 red                                              |
    | 0x02 orange                                           |
    | 0x03 yellow                                           |
    | 0x04 green                                            |
    | 0x05 blue                                             |
    | 0x06 purple                                           |
    | 0x07 white                                            |
    \*-----------------------------------------------------*/
    unsigned int divisor    = GetLargestColour( red, green, blue);
    unsigned int r          = (int)round( red / divisor );
    unsigned int g          = (int)round( green / divisor );
    unsigned int b          = (int)round( blue / divisor );
    unsigned char idx       = argb_colour_index_data[r][g][b];
    return idx;
}

unsigned int GigabyteAorusLaptopController::GetLargestColour(unsigned int red, unsigned int green, unsigned int blue)
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

void GigabyteAorusLaptopController::SetCustom(std::vector<RGBColor> colors, std::vector<unsigned int> positions, unsigned char brightness)
{
    /*---------------------------------------------------------*\
    | Custom mode protocol                                      |
    |   ID C  M  Sp Br Cl Dr Ch                                 |
    |   08 00 33 01 2D 05 01 90                                 |
    |   92 00 00 00 00 00 00 6D     ->                          |
    |   GET REPORT REQUEST          <-                          |
    |   read 8x64 bytes             <-                          |
    |   12 00 00 08 00 00 00 E5     ->                          |
    |   00 RR GG BB .....           -> data (8 * 64 bytes)      |
    \*---------------------------------------------------------*/

    /*---------------------------------------------------------*\
    | Sends mode  packet                                        |
    \*---------------------------------------------------------*/
    unsigned char usb_buf[GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1];
    memset(usb_buf, 0x00, GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1);

    usb_buf[1] = GIGABYTE_AORUS_LAPTOP_REPORT_ID;
    usb_buf[3] = GIGABYTE_AORUS_LAPTOP_CUSTOM_MODE_VALUE;
    usb_buf[4] = 0x01;
    usb_buf[5] = brightness;
    usb_buf[6] = 0x05;
    usb_buf[7] = 0x01;

    unsigned char checksum = 0xFF;

    for(unsigned int i = 1; i < 8; i++)
    {
        checksum -= usb_buf[i];
    }

    usb_buf[8] = checksum;

    hid_send_feature_report(dev, usb_buf, GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1);

    /*---------------------------------------------------------*\
    | Sends first packet                                        |
    \*---------------------------------------------------------*/
    unsigned char start_packet[GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1] =
    {
        0x00, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6D
    };

    hid_send_feature_report(dev, start_packet, GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1);

    /*---------------------------------------------------------*\
    | GET REPORT REQUEST                                        |
    \*---------------------------------------------------------*/
    unsigned char report_packet[GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1];
    memset(report_packet, 0x00, GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1);

    report_packet[1] = GIGABYTE_AORUS_LAPTOP_REPORT_ID;
    hid_get_feature_report(dev, report_packet, GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1);

    /*---------------------------------------------------------*\
    | Sends 2nd packet                                          |
    \*---------------------------------------------------------*/
    unsigned char second_packet[GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1] =
    {
        0x00, 0x12, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xE5
    };

    hid_send_feature_report(dev, second_packet, GIGABYTE_AORUS_LAPTOP_REPORT_SIZE+1);

    /*---------------------------------------------------------*\
    | Creates the data packets                                  |
    \*---------------------------------------------------------*/
    unsigned char color_data[64 * 8];
    memset(color_data, 0x00, 64 * 8);

    for(unsigned int i = 0; i < positions.size(); i++)
    {
        color_data[positions[i] * 4 + 1] = RGBGetRValue(colors[i]);
        color_data[positions[i] * 4 + 2] = RGBGetGValue(colors[i]);
        color_data[positions[i] * 4 + 3] = RGBGetBValue(colors[i]);
    }

    unsigned char color_buf[64 + 1];
    color_buf[0] = 0;

    for(unsigned int i = 0; i < 8; i++)
    {
        memcpy(&color_buf[1], &(color_data[64*i]), 64);
        hid_write(dev, color_buf, 65);
    }
}
