/*---------------------------------------------------------*\
| HyperXMousematController.cpp                              |
|                                                           |
|   Driver for HyperX mousemat                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HyperXMousematController.h"
#include "StringUtils.h"

HyperXMousematController::HyperXMousematController(hidapi_wrapper hid_wrapper, hid_device* dev_handle, const char* path, std::string dev_name)
{
    wrapper     = hid_wrapper;
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

HyperXMousematController::~HyperXMousematController()
{
    wrapper.hid_close(dev);
}

std::string HyperXMousematController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string HyperXMousematController::GetNameString()
{
    return(name);
}

std::string HyperXMousematController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = wrapper.hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void HyperXMousematController::SendDirect
    (
    RGBColor*       color_data
    )
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    buf[0x01]   = 0x04;
    buf[0x02]   = 0xF2;

    buf[0x09]   = 0x02;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    wrapper.hid_send_feature_report(dev, buf, 65);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;

    for(int i = 0; i < 16; i++)
    {
        buf[(i * 4) + 1] = 0x81;
        buf[(i * 4) + 2] = RGBGetRValue(color_data[i]);
        buf[(i * 4) + 3] = RGBGetGValue(color_data[i]);
        buf[(i * 4) + 4] = RGBGetBValue(color_data[i]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    wrapper.hid_send_feature_report(dev, buf, 65);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;

    for(int i = 0; i < 16; i++)
    {
        buf[(i * 4) + 1] = 0x81;
        buf[(i * 4) + 2] = RGBGetRValue(color_data[16 + i]);
        buf[(i * 4) + 3] = RGBGetGValue(color_data[16 + i]);
        buf[(i * 4) + 4] = RGBGetBValue(color_data[16 + i]);
    }

    wrapper.hid_send_feature_report(dev, buf, 65);
}
