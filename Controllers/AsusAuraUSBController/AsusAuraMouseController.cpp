/*-----------------------------------------*\
|  AsusAuraMouseController.cpp              |
|                                           |
|  Driver for ASUS Aura RGB USB             |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/23/2020  |
\*-----------------------------------------*/

#include "AsusAuraMouseController.h"

#include <cstring>

AuraMouseController::AuraMouseController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

AuraMouseController::~AuraMouseController()
{
    hid_close(dev);
}

std::string AuraMouseController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraMouseController::GetSerialString()
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

void AuraMouseController::SendUpdate
    (
    unsigned char   zone,
    unsigned char   mode,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu
    )
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x28;
    usb_buf[0x03]   = zone;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = mode;
    usb_buf[0x06]   = 0x04;
    usb_buf[0x07]   = red;
    usb_buf[0x08]   = grn;
    usb_buf[0x09]   = blu;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}
