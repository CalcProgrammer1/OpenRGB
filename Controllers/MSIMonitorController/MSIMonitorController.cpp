/*---------------------------------------------------------*\
| RGBController_MSIMonitor.cpp                              |
|                                                           |
|   RGBController for MSI monitor (gaming controller)       |
|                                                           |
|   Andy Herbert                              2026 June  4  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string.h>
#include "MSIMonitorController.h"
#include "StringUtils.h"

MSIMonitorController::MSIMonitorController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

MSIMonitorController::~MSIMonitorController()
{
    hid_close(dev);
}

std::string MSIMonitorController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSIMonitorController::GetNameString()
{
    return(name);
}

std::string MSIMonitorController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void MSIMonitorController::Set(uint8_t mode_value, const std::vector<RGBColor> colors, uint8_t last_bit)
{
    /*---------------------------------------------------------*\
    | Prepare color data                                        |
    \*---------------------------------------------------------*/
    uint8_t data[MSI_MONITOR_PACKET_SIZE];
    memset(data, 0x00, MSI_MONITOR_PACKET_SIZE);

    unsigned int offset = 0;

    data[offset++] = 0x71;
    data[offset++] = 0x01;
    for(int i = 0; i < 3; i++)
    {
        data[offset++] = 0x00;
    }

    data[offset++] = 0x01;
    data[offset++] = 0x64;
    for(int i = 0; i < 5; i++)
    {
        data[offset++] = 0x00;
    }

    /*---------------------------------------------------------*\
    | put mode_value                                            |
    \*---------------------------------------------------------*/
    data[offset++] = mode_value;

    for(int i = 0; i < 3; i++)
    {
        data[offset++] = 0x00;
    }
    data[offset++] = 0x01;
    data[offset++] = 0x64;
    for(int i = 0; i < 5; i++)
    {
        data[offset++] = 0x00;
    }

    /*-----------------------------------------------------------------------*\
    | this data looks like placeholder for additional LEDs in other monitors  |
    \*-----------------------------------------------------------------------*/
    for(int i = 0; i < 9; i++)
    {
        data[offset++] = 0xff;
        data[offset++] = 0x00;
        data[offset++] = 0x00;
    }

    //RGB values begin
    for(const RGBColor color: colors)
    {
        data[offset++] = RGBGetRValue(color);
        data[offset++] = RGBGetGValue(color);
        data[offset++] = RGBGetBValue(color);
    }

    /*-------------------------------------------------------------------------------*\
    | last bit is probably a write to device bit- 0x01 saves to device, 0x00 doesn't. |
    | For direct mode, bit is set to 0x00, otherwise lights will flicker              |
    \*-------------------------------------------------------------------------------*/

    data[offset++] = last_bit;

    /*---------------------------------------------------------*\
    | Send the data (1 packet)                                  |
    \*---------------------------------------------------------*/

    hid_send_feature_report(dev, data, MSI_MONITOR_PACKET_SIZE);
}
