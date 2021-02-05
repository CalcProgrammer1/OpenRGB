/*-----------------------------------------*\
|  AsusAuraCoreController.cpp               | 
|                                           |
|  Driver for ASUS ROG Aura Core RGB        |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/13/2020   |
\*-----------------------------------------*/

#include "AsusAuraCoreController.h"
#include <cstring>

AuraCoreController::AuraCoreController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

AuraCoreController::~AuraCoreController()
{
    hid_close(dev);
}

std::string AuraCoreController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraCoreController::GetSerialString()
{
    wchar_t serial_string[128];
    hid_get_serial_number_string(dev, serial_string, 128);

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void AuraCoreController::SendBrightness
    (
    unsigned char   brightness
    )
{
    unsigned char usb_buf[17];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x5A;
    usb_buf[0x01]   = AURA_CORE_COMMAND_BRIGHTNESS;
    usb_buf[0x02]   = 0xC5;
    usb_buf[0x03]   = 0xC4;
    usb_buf[0x04]   = brightness;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, 17);
}

void AuraCoreController::SendUpdate
    (
    unsigned char   zone,
    unsigned char   mode,
    unsigned char   speed,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    unsigned char usb_buf[17];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x5D;
    usb_buf[0x01]   = AURA_CORE_COMMAND_UPDATE;
    usb_buf[0x02]   = zone;
    usb_buf[0x03]   = mode;
    usb_buf[0x04]   = red;
    usb_buf[0x05]   = green;
    usb_buf[0x06]   = blue;
    usb_buf[0x07]   = speed;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, 17);
}

void AuraCoreController::SendSet()
{
    unsigned char usb_buf[17];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x5D;
    usb_buf[0x01]   = AURA_CORE_COMMAND_SET;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, 17);
}

void AuraCoreController::SendApply()
{
    unsigned char usb_buf[17];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x5D;
    usb_buf[0x01]   = AURA_CORE_COMMAND_APPLY;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, 17);
}
