/*-------------------------------------------------------------------*\
|  RoccatBurstProAirController.cpp                                    |
|                                                                     |
|  Driver for Roccat Burst Pro Air Mouse                              |
|                                                                     |
|  Morgan Guimard (morg)          6/16/2022                           |
\*-------------------------------------------------------------------*/

#include "RoccatBurstProAirController.h"

#include <cstring>

RoccatBurstProAirController::RoccatBurstProAirController(hid_device* dev_handle, const hid_device_info& info)
{
    dev                 = dev_handle;
    version             = "";
    location            = info.path;

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

RoccatBurstProAirController::~RoccatBurstProAirController()
{
    hid_close(dev);
}

std::string RoccatBurstProAirController::GetFirmwareVersion()
{
    return version;
}

std::string RoccatBurstProAirController::GetSerialString()
{
    return serial_number;
}

std::string RoccatBurstProAirController::GetDeviceLocation()
{
    return("HID: " + location);
}

void RoccatBurstProAirController::SetColors(std::vector<RGBColor> colors)
{
    unsigned char usb_buf[ROCCAT_BURST_PRO_AIR_REPORT_SIZE];
    memset(usb_buf, 0x00, ROCCAT_BURST_PRO_AIR_REPORT_SIZE);

    usb_buf[0] = ROCCAT_BURST_PRO_AIR_REPORT_ID;

    usb_buf[1] = 0x01;
    usb_buf[2] = 0x4C;
    usb_buf[3] = 0x06;
    usb_buf[4] = 0x14;

    for(unsigned char i = 0; i < colors.size(); i++)
    {
        usb_buf[5 + 5 * i] = i + 1;
        usb_buf[6 + 5 * i] = 0xFF;
        usb_buf[7 + 5 * i] = RGBGetRValue(colors[i]);
        usb_buf[8 + 5 * i] = RGBGetGValue(colors[i]);
        usb_buf[9 + 5 * i] = RGBGetBValue(colors[i]);
    }

    hid_send_feature_report(dev, usb_buf, ROCCAT_BURST_PRO_AIR_REPORT_SIZE);
}

void RoccatBurstProAirController::SetModeValues(unsigned char mode_value, unsigned char speed, unsigned char brightness)
{
    unsigned char usb_buf[ROCCAT_BURST_PRO_AIR_REPORT_SIZE];
    memset(usb_buf, 0x00, ROCCAT_BURST_PRO_AIR_REPORT_SIZE);

    usb_buf[0] = ROCCAT_BURST_PRO_AIR_REPORT_ID;

    usb_buf[1] = 0x01;
    usb_buf[2] = 0x4C;
    usb_buf[3] = 0x06;
    usb_buf[4] = 0x06;

    usb_buf[5] = mode_value;
    usb_buf[6] = speed;
    usb_buf[7] = brightness;

    usb_buf[8] = 0x0F;

    hid_send_feature_report(dev, usb_buf, ROCCAT_BURST_PRO_AIR_REPORT_SIZE);
}
