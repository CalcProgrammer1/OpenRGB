/*-------------------------------------------------------------------*\
|  WushiL50USBController.cpp                                       |
|                                                                     |
|  interface for Wushi L50 Devices                               |
\*-------------------------------------------------------------------*/

#include <iostream>
#include "WushiL50USBController.h"
#include "LogManager.h"

WushiL50USBController::WushiL50USBController(hid_device* dev_handle, const char* path, uint16_t in_pid)
{
    const uint8_t   sz      = HID_MAX_STR;
    wchar_t         tmp[sz];
    wchar_t serial_string[128];
    dev                     = dev_handle;
    location                = path;
    pid                     = in_pid;



    hid_get_manufacturer_string(dev, tmp, sz);
    std::wstring w_tmp      = std::wstring(tmp);
    name                    = std::string(w_tmp.begin(), w_tmp.end());

    hid_get_product_string(dev, tmp, sz);
    w_tmp                   = std::wstring(tmp);
    name.append(" ").append(std::string(w_tmp.begin(), w_tmp.end()));

    int ret = hid_get_serial_number_string(dev, tmp, sz);

    if(ret != 0)
    {
        serial_number = "";
    }
    else
    {
        std::wstring return_wstring = tmp;//serial_string;
        serial_number = std::string(return_wstring.begin(), return_wstring.end());
    }
    version ="0.10";   //
    setDeviceSoftwareMode();
}

WushiL50USBController::~WushiL50USBController()
{
    hid_close(dev);
}

void WushiL50USBController::setMode(const KeyboardState &in_mode)
{
    uint8_t buffer[WUSHIL50_HID_PACKET_SIZE];
    memcpy(buffer, &in_mode, WUSHIL50_HID_PACKET_SIZE);
    hid_send_feature_report(dev, buffer, WUSHIL50_HID_PACKET_SIZE);
}

uint16_t WushiL50USBController::getPid()
{
    return pid;
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
    return(serial_number);
}

std::string WushiL50USBController::GetFirmwareVersion()
{
    return(version);
}

void WushiL50USBController::setDeviceSoftwareMode()
{

}

void WushiL50USBController::setDeviceHardwareMode()
{

}
