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

AuraMouseController::AuraMouseController(hid_device* dev_handle, const char* path, uint16_t pid)
{
    dev         = dev_handle;
    location    = path;
    device_pid  = pid;
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
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void AuraMouseController::SaveMode()
{
    unsigned char usb_save_buf[ASUS_AURA_MOUSE_PACKET_SIZE] = { 0x00, 0x50, 0x03 };

    hid_write(dev, usb_save_buf, ASUS_AURA_MOUSE_PACKET_SIZE);
}

void AuraMouseController::SendUpdate
    (
    unsigned char   zone,
    unsigned char   mode,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu,
    unsigned char   dir,
    bool            random,
    unsigned char   speed,
    unsigned char   brightness
    )
{
    unsigned char usb_buf[ASUS_AURA_MOUSE_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, ASUS_AURA_MOUSE_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x28;
    usb_buf[0x03]   = zone;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = mode;
    usb_buf[0x06]   = brightness;
    usb_buf[0x07]   = red;
    usb_buf[0x08]   = grn;
    usb_buf[0x09]   = blu;
    if (device_pid == AURA_ROG_GLADIUS_II_ORIGIN_PNK_LTD_PID)
    {
        usb_buf[0x0A]   = 0;
        usb_buf[0x0B]   = 0;
        usb_buf[0x0C]   = 0;
        usb_buf[0x0D]   = dir;
        usb_buf[0x0E]   = random;
        usb_buf[0x0F]   = speed;
    }
    else
    {
        usb_buf[0x0A]   = dir;
        usb_buf[0x0B]   = random;
        usb_buf[0x0C]   = speed;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, ASUS_AURA_MOUSE_PACKET_SIZE);
}
