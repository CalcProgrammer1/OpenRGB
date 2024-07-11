/*---------------------------------------------------------*\
| SteelSeriesArctis5Controller.cpp                          |
|                                                           |
|   Driver for SteelSeries Arctis 5                         |
|                                                           |
|   Morgan Guimard                              04 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "SteelSeriesArctis5Controller.h"

SteelSeriesArctis5Controller::SteelSeriesArctis5Controller(hid_device* dev_handle, const hid_device_info& info)
{
    dev                 = dev_handle;
    location            = info.path;
    version             = "";

    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        serial_number = "";
    }
    else
    {
        std::wstring return_wstring = serial_string;
        serial_number = std::string(return_wstring.begin(), return_wstring.end());
    }
}

SteelSeriesArctis5Controller::~SteelSeriesArctis5Controller()
{
    hid_close(dev);
}

std::string SteelSeriesArctis5Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string SteelSeriesArctis5Controller::GetSerialString()
{
    return(serial_number);
}

std::string SteelSeriesArctis5Controller::GetFirmwareVersion()
{
    return(version);
}

void SteelSeriesArctis5Controller::SetColor(unsigned char zone_id, RGBColor color)
{
    unsigned char usb_buf[ARCTIS_5_REPORT_SIZE];

    /*----------------------------------------------*\
    | Two packets are sent before a color change     |
    \*----------------------------------------------*/
    memset(usb_buf, 0x00, ARCTIS_5_REPORT_SIZE);

    usb_buf[0x00] = ARCTIS_5_REPORT_ID;
    usb_buf[0x01] = 0x81;
    usb_buf[0x02] = 0x43;
    usb_buf[0x03] = 0x01;
    usb_buf[0x04] = 0x22;

    hid_write(dev, usb_buf, ARCTIS_5_REPORT_SIZE);

    usb_buf[0x04] = 0x23;

    hid_write(dev, usb_buf, ARCTIS_5_REPORT_SIZE);

    /*----------------------------------------------*\
    | First packet: send color                       |
    \*----------------------------------------------*/
    memset(usb_buf, 0x00, ARCTIS_5_REPORT_SIZE);

    usb_buf[0x00] = ARCTIS_5_REPORT_ID;
    usb_buf[0x01] = 0x8A;
    usb_buf[0x02] = 0x42;

    usb_buf[0x04] = 0x20;
    usb_buf[0x05] = 0x41;

    usb_buf[0x07] = RGBGetRValue(color);
    usb_buf[0x08] = RGBGetGValue(color);
    usb_buf[0x09] = RGBGetBValue(color);

    usb_buf[0x0A] = 0xFF;
    usb_buf[0x0B] = 0x32;
    usb_buf[0x0C] = 0xC8;
    usb_buf[0x0D] = 0xC8;

    hid_write(dev, usb_buf, ARCTIS_5_REPORT_SIZE);

    /*-----------------------------------------*\
    | Second packet: apply to zone              |
    \*-----------------------------------------*/
    memset(usb_buf, 0x00, ARCTIS_5_REPORT_SIZE);

    usb_buf[0x00] = ARCTIS_5_REPORT_ID;
    usb_buf[0x01] = 0x8A;
    usb_buf[0x02] = 0x42;

    usb_buf[0x04] = 0x20;
    usb_buf[0x05] = 0x41;
    usb_buf[0x06] = 0x08;
    usb_buf[0x07] = zone_id;
    usb_buf[0x08] = 0x01;

    hid_write(dev, usb_buf, ARCTIS_5_REPORT_SIZE);

    /*-----------------------------------------*\
    | Thrid packet: apply to zone               |
    \*-----------------------------------------*/
    memset(usb_buf, 0x00, ARCTIS_5_REPORT_SIZE);

    usb_buf[0x00] = ARCTIS_5_REPORT_ID;
    usb_buf[0x01] = 0x8A;
    usb_buf[0x02] = 0x42;

    usb_buf[0x04] = 0x20;
    usb_buf[0x05] = 0x60;
    usb_buf[0x06] = zone_id;

    hid_write(dev, usb_buf, ARCTIS_5_REPORT_SIZE);

    /*-----------------------------------------*\
    | Last packet: apply                        |
    \*-----------------------------------------*/
    memset(usb_buf, 0x00, ARCTIS_5_REPORT_SIZE);

    usb_buf[0x00] = ARCTIS_5_REPORT_ID;
    usb_buf[0x01] = 0x8A;
    usb_buf[0x02] = 0x42;

    usb_buf[0x04] = 0x20;
    usb_buf[0x05] = 0x05;

    hid_write(dev, usb_buf, ARCTIS_5_REPORT_SIZE);
}
