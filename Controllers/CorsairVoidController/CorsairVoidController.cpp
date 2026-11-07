/*---------------------------------------------------------*\
| CorsairVoidController.cpp                                 |
|                                                           |
|   Driver for Corsair Void Pro headsets                    |
|                                                           |
|   Derek Ross                                  15 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string.h>
#include "CorsairVoidController.h"
#include "StringUtils.h"

CorsairVoidController::CorsairVoidController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev             = dev_handle;
    location        = info.path;
    name            = dev_name;
    host_control    = false;
}

CorsairVoidController::~CorsairVoidController()
{
    /*-----------------------------------------------------*\
    | Give lighting back to the firmware so the headset's   |
    | own effects and the mic mute LED work again           |
    \*-----------------------------------------------------*/
    if(host_control)
    {
        SetLightingControl(CORSAIR_VOID_LIGHTING_FIRMWARE);
    }

    hid_close(dev);
}

std::string CorsairVoidController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CorsairVoidController::GetNameString()
{
    return(name);
}

std::string CorsairVoidController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void CorsairVoidController::SetLightingControl(unsigned char owner)
{
    unsigned char usb_buf[CORSAIR_VOID_LIGHTING_REPORT_SIZE];

    memset(usb_buf, 0x00, CORSAIR_VOID_LIGHTING_REPORT_SIZE);

    usb_buf[0x00]   = CORSAIR_VOID_LIGHTING_REPORT_ID;
    usb_buf[0x01]   = owner;

    hid_write(dev, usb_buf, CORSAIR_VOID_LIGHTING_REPORT_SIZE);
}

void CorsairVoidController::SetColors(RGBColor left, RGBColor right)
{
    /*-----------------------------------------------------*\
    | Take lighting from the firmware on the first update   |
    | rather than at detection, so the headset keeps its    |
    | own effects until OpenRGB actually sets a color.      |
    | Repeat the request periodically in case the headset   |
    | was switched off and on in the meantime.              |
    \*-----------------------------------------------------*/
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    if(!host_control || now - last_host_request >= std::chrono::milliseconds(CORSAIR_VOID_LIGHTING_REFRESH_MS))
    {
        SetLightingControl(CORSAIR_VOID_LIGHTING_HOST);
        host_control        = true;
        last_host_request   = now;
    }

    unsigned char usb_buf[CORSAIR_VOID_REGISTER_REPORT_SIZE];

    memset(usb_buf, 0x00, CORSAIR_VOID_REGISTER_REPORT_SIZE);

    usb_buf[0x00]   = CORSAIR_VOID_REGISTER_REPORT_ID;
    usb_buf[0x01]   = 0x06;

    usb_buf[0x02]   = CORSAIR_VOID_REGISTER_LEFT_RED;
    usb_buf[0x03]   = RGBGetRValue(left);
    usb_buf[0x04]   = CORSAIR_VOID_REGISTER_LEFT_GREEN;
    usb_buf[0x05]   = RGBGetGValue(left);
    usb_buf[0x06]   = CORSAIR_VOID_REGISTER_LEFT_BLUE;
    usb_buf[0x07]   = RGBGetBValue(left);

    usb_buf[0x08]   = CORSAIR_VOID_REGISTER_RIGHT_RED;
    usb_buf[0x09]   = RGBGetRValue(right);
    usb_buf[0x0A]   = CORSAIR_VOID_REGISTER_RIGHT_GREEN;
    usb_buf[0x0B]   = RGBGetGValue(right);
    usb_buf[0x0C]   = CORSAIR_VOID_REGISTER_RIGHT_BLUE;
    usb_buf[0x0D]   = RGBGetBValue(right);

    hid_write(dev, usb_buf, CORSAIR_VOID_REGISTER_REPORT_SIZE);
}
