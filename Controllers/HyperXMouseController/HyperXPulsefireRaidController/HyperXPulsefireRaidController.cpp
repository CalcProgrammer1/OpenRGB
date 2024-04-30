/*-------------------------------------------*\
|  HyperXPulsefireRaidController.cpp          |
|                                             |
|  Driver for HyperX Pulsefire Raid           |
|  lighting controller                        |
|                                             |
|  Morgan Guimard (morg) 04/06/2022           |
\*-------------------------------------------*/

#include "HyperXPulsefireRaidController.h"
#include <cstring>

using namespace std::chrono_literals;

HyperXPulsefireRaidController::HyperXPulsefireRaidController(hid_device* dev_handle, const hid_device_info& info)
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

HyperXPulsefireRaidController::~HyperXPulsefireRaidController()
{
    hid_close(dev);
}

std::string HyperXPulsefireRaidController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string HyperXPulsefireRaidController::GetSerialString()
{
    return(serial_number);
}

std::string HyperXPulsefireRaidController::GetFirmwareVersion()
{
    return(version);
}

void HyperXPulsefireRaidController::SendColors(std::vector<RGBColor> colors)
{
    unsigned char usb_buf[HYPERX_PULSFIRE_RAID_PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, HYPERX_PULSFIRE_RAID_PACKET_DATA_LENGTH);

    usb_buf[0] = HYPERX_PULSFIRE_RAID_REPORT_ID;
    usb_buf[1] = HYPERX_PULSFIRE_RAID_DIRECT_MODE_START_PACKET;

    for(unsigned int i = 0; i < 2; i++)
    {
        usb_buf[3 * i + 2] = RGBGetRValue(colors[i]);
        usb_buf[3 * i + 3] = RGBGetGValue(colors[i]);
        usb_buf[3 * i + 4] = RGBGetBValue(colors[i]);
    }

    usb_buf[8] = HYPERX_PULSFIRE_RAID_DIRECT_MODE_END_PACKET;

    Send(usb_buf);
}


void HyperXPulsefireRaidController::Send(unsigned char* packet)
{
    hid_send_feature_report(dev, packet, HYPERX_PULSFIRE_RAID_PACKET_DATA_LENGTH);    
    std::this_thread::sleep_for(10ms);
}
