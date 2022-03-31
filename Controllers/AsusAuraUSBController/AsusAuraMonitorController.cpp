/*-----------------------------------------*\
|  AsusAuraMonitorController.cpp            |
|                                           |
|  Driver for ASUS Aura RGB USB             |
|  lighting controller                      |
|                                           |
|  Mola19 08/03/2021                        |
\*-----------------------------------------*/

#include "AsusAuraMonitorController.h"

#include <cstring>
#include "LogManager.h"

AuraMonitorController::AuraMonitorController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

AuraMonitorController::~AuraMonitorController()
{
    hid_close(dev);
}

std::string AuraMonitorController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraMonitorController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void AuraMonitorController::UpdateLed
    (
    int             led,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    unsigned char usb_buf[8];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x03;
    usb_buf[0x01]   = 0x02;
    usb_buf[0x02]   = 0xa1;
    usb_buf[0x03]   = 0x80;

    usb_buf[0x04]   = 16 + led * 3;
    usb_buf[0x05]   = red;
    
    hid_send_feature_report(dev, usb_buf, 8);

    usb_buf[0x04]   = 16 + led * 3 + 1;
    usb_buf[0x05]   = blue;
    
    hid_send_feature_report(dev, usb_buf, 8);

    usb_buf[0x04]   = 16 + led * 3 + 2;
    usb_buf[0x05]   = green;
    
    hid_send_feature_report(dev, usb_buf, 8);
}

void AuraMonitorController::ApplyChanges()
{
    unsigned char usb_buf[8];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x03;
    usb_buf[0x01]   = 0x02;
    usb_buf[0x02]   = 0xa1;
    usb_buf[0x03]   = 0x80;
    usb_buf[0x04]   = 0xa0;
    usb_buf[0x05]   = 0x01;
    
    hid_send_feature_report(dev, usb_buf, 8);
}
