/*---------------------------------------------------------*\
| WushiL50USBController.cpp                                 |
|                                                           |
|   Driver for Wushi L50                                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "StringUtils.h"
#include "WushiL50USBController.h"

WushiL50USBController::WushiL50USBController(hidapi_wrapper hid_wrapper, hid_device* dev_handle, const char* path, std::string dev_name)
{
    wrapper     = hid_wrapper;
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

WushiL50USBController::~WushiL50USBController()
{
    wrapper.hid_close(dev);
}

std::string WushiL50USBController::getName()
{
    return name;
}

std::string WushiL50USBController::getLocation()
{
    return location;
}

std::string WushiL50USBController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = wrapper.hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void WushiL50USBController::setMode(WushiL50State * in_mode)
{
    unsigned char usb_buf[WUSHI_L50_HID_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up custom lighting packet                         |
    \*-----------------------------------------------------*/
#ifdef _WIN32
    #define OFFSET          1
    usb_buf[0x00]           = 0xCC;
#else
    #define OFFSET          0
#endif
    usb_buf[0x00 + OFFSET]  = 0x16;
    usb_buf[0x01 + OFFSET]  = in_mode->effect;
    usb_buf[0x02 + OFFSET]  = in_mode->speed;
    usb_buf[0x03 + OFFSET]  = in_mode->brightness;

    /*-----------------------------------------------------*\
    | Copy in color data                                    |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x04 + OFFSET], in_mode->zone0_rgb, 3);
    memcpy(&usb_buf[0x07 + OFFSET], in_mode->zone1_rgb, 3);
    memcpy(&usb_buf[0x0A + OFFSET], in_mode->zone2_rgb, 3);
    memcpy(&usb_buf[0x0D + OFFSET], in_mode->zone3_rgb, 3);

    usb_buf[0x11 + OFFSET]  = in_mode->wave_ltr;
    usb_buf[0x12 + OFFSET]  = in_mode->wave_rtl;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    wrapper.hid_send_feature_report(dev, usb_buf, WUSHI_L50_HID_PACKET_SIZE);
}
