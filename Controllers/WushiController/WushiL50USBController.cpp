/*-------------------------------------*\
|  WushiL50USBController.h              |
|                                       |
|  interface for Wushi L50 Devices      |
\*-------------------------------------*/

#include "WushiL50USBController.h"

WushiL50USBController::WushiL50USBController(hid_device* dev_handle, const char* path)
{
    const uint8_t   sz      = HID_MAX_STR;
    wchar_t         tmp[sz];
    wchar_t serial_string[128];
    dev                     = dev_handle;
    location                = path;

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
}

WushiL50USBController::~WushiL50USBController()
{
    hid_close(dev);
}

void WushiL50USBController::setMode(WushiL50State * in_mode)
{
    hid_send_feature_report(dev, (uint8_t *)in_mode, WUSHI_L50_HID_PACKET_SIZE);
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
