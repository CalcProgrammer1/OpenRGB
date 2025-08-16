/*---------------------------------------------------------*\
| RoccatKoneProAirController.cpp                            |
|                                                           |
|   Driver for Roccat Kone Pro Air                          |
|                                                           |
|   Plunti                                      10 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "RoccatKoneProAirController.h"
#include "StringUtils.h"

RoccatKoneProAirController::RoccatKoneProAirController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

RoccatKoneProAirController::~RoccatKoneProAirController()
{
    hid_close(dev);
}

std::string RoccatKoneProAirController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RoccatKoneProAirController::GetNameString()
{
    return(name);
}

std::string RoccatKoneProAirController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void RoccatKoneProAirController::SendDirect(std::vector<RGBColor> colors)
{
    SendRGB(true, colors, ROCCAT_KONE_PRO_AIR_DIRECT_MODE_VALUE, ROCCAT_KONE_PRO_AIR_SPEED_MAX, ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX);
}

void RoccatKoneProAirController::SetMode(std::vector<RGBColor> colors, unsigned char mode_value, unsigned char speed, unsigned char brightness, unsigned int mode_flags)
{
    /*---------------------------------------------------------*\
    | 1. Read settings                                          |
    \*---------------------------------------------------------*/
    unsigned char active_settings[ROCCAT_KONE_PRO_AIR_SETTINGS_READ_PACKET_LENGTH];

    unsigned char settings_request[] = {0x00, 0x90, 0x00, 0x04, 0x00, 0x18, 0x25, 0x51, 0x32};
    hid_write(dev, settings_request, sizeof(settings_request));

    do
    {
        hid_read(dev, active_settings, ROCCAT_KONE_PRO_AIR_SETTINGS_READ_PACKET_LENGTH);
    } while( (active_settings[0] != 0x90) || (active_settings[2] != 0x26) );

    /*---------------------------------------------------------*\
    | 2. Send settings and select profile                       |
    \*---------------------------------------------------------*/
    unsigned char usb_buf[ROCCAT_KONE_PRO_AIR_SETTINGS_WRITE_PACKET_LENGTH];
    memset(usb_buf, 0x00, ROCCAT_KONE_PRO_AIR_SETTINGS_WRITE_PACKET_LENGTH);

    usb_buf[1] = 0x10;
    usb_buf[2] = 0x50;
    usb_buf[3] = 0x14;
    memcpy(usb_buf + 5, active_settings + 10, 19);
    usb_buf[24] = active_settings[41];

    hid_write(dev, usb_buf, ROCCAT_KONE_PRO_AIR_SETTINGS_WRITE_PACKET_LENGTH);

    /*---------------------------------------------------------*\
    | 3. Send RGB                                               |
    \*---------------------------------------------------------*/
    SendRGB(false,
            colors,
            mode_value,
            (mode_flags & MODE_FLAG_HAS_SPEED     ) ? speed      : (unsigned char)ROCCAT_KONE_PRO_AIR_SPEED_MAX,
            (mode_flags & MODE_FLAG_HAS_BRIGHTNESS) ? brightness : (unsigned char)ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX
           );
}

void RoccatKoneProAirController::SendRGB(bool direct, std::vector<RGBColor> colors, unsigned char mode_value, unsigned char speed, unsigned char brightness)
{
    unsigned char usb_buf[ROCCAT_KONE_PRO_AIR_RGB_PACKET_LENGTH];
    memset(usb_buf, 0x00, ROCCAT_KONE_PRO_AIR_RGB_PACKET_LENGTH);

    usb_buf[1] = 0x10;
    usb_buf[2] = direct ? 0x10 : 0x50;
    usb_buf[3] = 0x0B;
    usb_buf[4] = direct ? 0x00 : 0x01;
    usb_buf[5] = mode_value;
    usb_buf[6] = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX;  // Explicit brightness for first LED, not used by OpenRGB
    usb_buf[7] = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX;  // Explicit brightness for second LED, not used by OpenRGB
    usb_buf[8] = brightness;
    usb_buf[9] = speed;

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        usb_buf[10 + 3 * i] = RGBGetRValue(colors[i]);
        usb_buf[11 + 3 * i] = RGBGetGValue(colors[i]);
        usb_buf[12 + 3 * i] = RGBGetBValue(colors[i]);
    }

    hid_write(dev, usb_buf, ROCCAT_KONE_PRO_AIR_RGB_PACKET_LENGTH);
}
