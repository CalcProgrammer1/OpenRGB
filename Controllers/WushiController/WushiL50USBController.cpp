/*-------------------------------------*\
|  WushiL50USBController.h              |
|                                       |
|  interface for Wushi L50 Devices      |
\*-------------------------------------*/

#include "WushiL50USBController.h"

WushiL50USBController::WushiL50USBController(hidapi_wrapper hid_wrapper, hid_device* dev_handle, const char* path)
{
    wrapper     = hid_wrapper;
    dev         = dev_handle;
    location    = path;
}

WushiL50USBController::~WushiL50USBController()
{
    wrapper.hid_close(dev);
}

void WushiL50USBController::setMode(WushiL50State * in_mode)
{
    wrapper.hid_send_feature_report(dev, (uint8_t *)in_mode, WUSHI_L50_HID_PACKET_SIZE);
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
