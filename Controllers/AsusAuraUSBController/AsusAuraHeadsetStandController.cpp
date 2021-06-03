/*-----------------------------------------*\
|  AsusAuraHeadsetStandController.cpp       |
|                                           |
|  Driver for ASUS Aura RGB USB             |
|  lighting controller                      |
|                                           |
|  Mola19 06/04/2021                        |
\*-----------------------------------------*/

#include "AsusAuraHeadsetStandController.h"

#include <cstring>

AuraHeadsetStandController::AuraHeadsetStandController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

AuraHeadsetStandController::~AuraHeadsetStandController()
{
	hid_close(dev);
}

std::string AuraHeadsetStandController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraHeadsetStandController::GetSerialString()
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

void AuraHeadsetStandController::UpdateLeds
    (
    std::vector<RGBColor>    colors
    )
{
    unsigned char usb_buf_0[365];

    memset(usb_buf_0, 0x00, sizeof(usb_buf_0));

    usb_buf_0[0x00]   = 0x00;
    usb_buf_0[0x01]   = 0xC0;
    usb_buf_0[0x02]   = 0x81;
    usb_buf_0[0x03]   = 0x00;
    usb_buf_0[0x04]   = 0x00;

    for(unsigned int i = 0; i < 60; i += 4)
    {
        usb_buf_0[5 + i]   = 0x00;
        usb_buf_0[6 + i]   = RGBGetRValue(colors[i / 4]);
        usb_buf_0[7 + i]   = RGBGetGValue(colors[i / 4]);
        usb_buf_0[8 + i]   = RGBGetBValue(colors[i / 4]);
    }

    hid_write(dev, usb_buf_0, 65);

    unsigned char usb_buf_1[65];

    memset(usb_buf_1, 0x00, sizeof(usb_buf_1));

    usb_buf_1[0x00]   = 0x00;
    usb_buf_1[0x01]   = 0xC0;
    usb_buf_1[0x02]   = 0x81;
    usb_buf_1[0x03]   = 0x01;
    usb_buf_1[0x04]   = 0x00;

    for(unsigned int i = 0; i < 12; i += 4)
    {
        usb_buf_1[5 + i]   = 0x00;
        usb_buf_1[6 + i]   = RGBGetRValue(colors[(i / 4) + 15]);
        usb_buf_1[7 + i]   = RGBGetGValue(colors[(i / 4) + 15]);
        usb_buf_1[8 + i]   = RGBGetBValue(colors[(i / 4) + 15]);
    }

    hid_write(dev, usb_buf_1, 65);
}

void AuraHeadsetStandController::UpdateDevice
    (
    unsigned char   mode,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu,
    unsigned char   speed,
    bool            save
    )
{
    unsigned char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x28;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = mode;
    usb_buf[0x06]   = speed;
    usb_buf[0x07]   = 0x04;
    usb_buf[0x08]   = 0x00;
    usb_buf[0x09]   = 0x00;
    usb_buf[0x0a]   = red;
    usb_buf[0x0b]   = grn;
    usb_buf[0x0c]   = blu;
    hid_write(dev, usb_buf, 65);

    if(save)
    {
        unsigned char usb_save_buf[65];

        memset(usb_save_buf, 0x00, sizeof(usb_save_buf));
        
        usb_save_buf[0x00]   = 0x00;
        usb_save_buf[0x01]   = 0x50;
        usb_save_buf[0x02]   = 0x55;
        
        hid_write(dev, usb_save_buf, 65);
    }
}
