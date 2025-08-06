/*---------------------------------------------------------*\
| CougarRevengerSTController.cpp                            |
|                                                           |
|   Driver for Cougar Revenger ST                           |
|                                                           |
|   Morgan Guimard (morg)                       17 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "CougarRevengerSTController.h"
#include "StringUtils.h"

CougarRevengerSTController::CougarRevengerSTController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
    version             = "";

    ActivateMode(0, DIRECT_MODE_VALUE);
    ActivateMode(1, DIRECT_MODE_VALUE);
    ActivateMode(2, DIRECT_MODE_VALUE);
}

CougarRevengerSTController::~CougarRevengerSTController()
{
    hid_close(dev);
}

std::string CougarRevengerSTController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CougarRevengerSTController::GetNameString()
{
    return(name);
}

std::string CougarRevengerSTController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string CougarRevengerSTController::GetFirmwareVersion()
{
    return(version);
}

void CougarRevengerSTController::SetDirect(unsigned char zone, RGBColor color, unsigned char brightness)
{
    const cougar_mode& m = modes_mapping.at(DIRECT_MODE_VALUE);

    unsigned char usb_buf[PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[0x01] = PACKET_START;
    usb_buf[0x02] = ACTION_SET;
    usb_buf[0x05] = m.zone_mode_byte[zone];

    /*-----------------------------------------*\
    | Set RGB Values                            |
    \*-----------------------------------------*/
    SendColourPacket(m.zone_rgb_mapping[zone],      RGBGetRValue(color), usb_buf);
    SendColourPacket(m.zone_rgb_mapping[zone] + 1,  RGBGetGValue(color), usb_buf);
    SendColourPacket(m.zone_rgb_mapping[zone] + 2,  RGBGetBValue(color), usb_buf);

    /*-----------------------------------------*\
    | Set Brightness                            |
    \*-----------------------------------------*/
    usb_buf[0x04] = m.zone_brightness_mapping[zone];
    usb_buf[0x06] = brightness;

    hid_send_feature_report(dev,usb_buf, PACKET_DATA_LENGTH);
}

void CougarRevengerSTController::SetModeData(unsigned char zone, unsigned char mode_value, std::vector<RGBColor> colors, unsigned char brightness, unsigned char speed)
{
    unsigned char usb_buf[PACKET_DATA_LENGTH];

    const cougar_mode& m = modes_mapping.at(mode_value);

    /*-----------------------------------------*\
    | Define colors                             |
    \*-----------------------------------------*/
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[0x01] = PACKET_START;
    usb_buf[0x02] = ACTION_SET;

    unsigned char offset = m.zone_rgb_mapping[zone];

    usb_buf[0x05] = m.zone_rgb_byte[zone];

    for(unsigned int i = 0; i < COLORS_SIZE; i++)
    {
        if(mode_value == BREATHING_MODE_VALUE && i >= 3 && zone == 2)
        {
             usb_buf[0x05] = 0x01;
        }

        SendColourPacket(offset,        RGBGetRValue(colors[i]), usb_buf);
        SendColourPacket(offset + 1,    RGBGetGValue(colors[i]), usb_buf);
        SendColourPacket(offset + 2,    RGBGetBValue(colors[i]), usb_buf);

        offset += 4;
    }

    /*-----------------------------------------*\
    | Define speed                              |
    \*-----------------------------------------*/
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[0x01] = PACKET_START;
    usb_buf[0x02] = ACTION_SET;
    usb_buf[0x04] = m.zone_speed_mapping[zone];
    usb_buf[0x05] = m.zone_speed_byte[zone];

    /*-----------------------------------------*\
    | Dirty hack here:                          |
    | The OEM app or firmware seems to be       |
    | broken, at full speed it flashes on       |
    | the bottom zone. Let's adjust a bit the   |
    | value so it's almost consistent with      |
    | the other zones at full speed.            |
    \*-----------------------------------------*/
    if(mode_value == SWIFT_MODE_VALUE && zone == 0)
    {
        usb_buf[0x06] = (speed + 1) * 3;
    }
    else
    {
        usb_buf[0x06] = speed;
    }

    hid_send_feature_report(dev,usb_buf, PACKET_DATA_LENGTH);

    /*-----------------------------------------*\
    | Define brightness                         |
    \*-----------------------------------------*/
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[0x01] = PACKET_START;
    usb_buf[0x02] = ACTION_SET;

    usb_buf[0x05] = m.zone_brightness_byte[zone];
    usb_buf[0x06] = brightness;

    offset = m.zone_brightness_mapping[zone];

    for(unsigned int i = 0; i < COLORS_SIZE; i++)
    {
        if(mode_value == BREATHING_MODE_VALUE && i >= 4 && zone == 2)
        {
             usb_buf[0x05] = 0x01;
        }

        usb_buf[0x04] = offset;
        hid_send_feature_report(dev,usb_buf, PACKET_DATA_LENGTH);

        offset += 4;
    }

    Apply();
}

void CougarRevengerSTController::SendColourPacket(unsigned char address, unsigned char value, unsigned char *buffer)
{
    buffer[0x04] = address;
    buffer[0x06] = value;

    hid_send_feature_report(dev, buffer, PACKET_DATA_LENGTH);
}

void CougarRevengerSTController::ActivateMode(unsigned char zone, unsigned char mode_value)
{
    const cougar_mode& m = modes_mapping.at(mode_value);

    unsigned char usb_buf[PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[0x01] = PACKET_START;
    usb_buf[0x02] = ACTION_SET;
    usb_buf[0x04] = m.zone_mode_mapping[zone];        // zone dependent?
    usb_buf[0x05] = m.zone_mode_byte[zone];
    usb_buf[0x06] = mode_value;

    hid_send_feature_report(dev,usb_buf, PACKET_DATA_LENGTH);
}

void CougarRevengerSTController::Apply()
{
    unsigned char usb_buf[PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[0x01] = PACKET_START;
    usb_buf[0x02] = 0x03;
    usb_buf[0x03] = 0x03;
    usb_buf[0x04] = 0x03;

    hid_send_feature_report(dev,usb_buf, PACKET_DATA_LENGTH);
}
