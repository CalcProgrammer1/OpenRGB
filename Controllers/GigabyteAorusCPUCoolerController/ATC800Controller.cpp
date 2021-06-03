/*-----------------------------------------*\
|  ATC800Controller.cpp                     |
|                                           |
|  Driver for Aorus ATC800 CPU Cooler       |
|                                           |
|                                           |
|  Felipe Cavalcanti    08/13/2020          |
\*-----------------------------------------*/

#include "ATC800Controller.h"

#include <cstring>

ATC800Controller::ATC800Controller(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

ATC800Controller::~ATC800Controller()
{
    hid_close(dev);
}

std::string ATC800Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string ATC800Controller::GetSerialString()
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

void ATC800Controller::SendCoolerMode
    (
    unsigned char       mode,
    unsigned short      /*speed*/,
    unsigned char       channel,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    unsigned char usb_buf[9];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/
    if (channel == AORUS_ATC800_TOP_ZONE)
    {
        usb_buf[0x00] = 0x00;
        usb_buf[0x01] = 0xbe;
        usb_buf[0x08] = 0xcc;

        hid_send_feature_report(dev, usb_buf, 9);
        memset(usb_buf, 0x00, sizeof(usb_buf));

        usb_buf[0x00] = 0x00;
        usb_buf[0x01] = 0xc9;
        usb_buf[0x02] = mode;
        usb_buf[0x03] = 0x3c;
        usb_buf[0x04] = 0x02;
        usb_buf[0x05] = 0x00;
        usb_buf[0x06] = 0x00;

        hid_send_feature_report(dev, usb_buf, 9);
        memset(usb_buf, 0x00, sizeof(usb_buf));

        usb_buf[0x00] = 0x00;
        usb_buf[0x01] = 0xbc;
        usb_buf[0x02] = red;
        usb_buf[0x03] = green;
        usb_buf[0x04] = blue;

        hid_send_feature_report(dev, usb_buf, 9);
        memset(usb_buf, 0x00, sizeof(usb_buf));

        usb_buf[0x00] = 0x00;
        usb_buf[0x01] = 0xb6;
    }
    else if (channel == AORUS_ATC800_FANS_ZONE)
    {
        hid_send_feature_report(dev, usb_buf, 9);
        memset(usb_buf, 0x00, sizeof(usb_buf));

        usb_buf[0x00]           = 0x00;
        usb_buf[0x01]           = 0xc9;
        usb_buf[0x02]           = mode;
        usb_buf[0x03]           = 0x3c;
        usb_buf[0x04]           = 0x02;
        usb_buf[0x05]           = 0x00;
        usb_buf[0x06]           = 0x01;
    
        hid_send_feature_report(dev, usb_buf, 9);
        memset(usb_buf, 0x00, sizeof(usb_buf));

        usb_buf[0x00]           = 0x00;
        usb_buf[0x01]           = 0xb0;
        usb_buf[0x02]           = mode;
        usb_buf[0x03]           = red;
        usb_buf[0x04]           = green;
        usb_buf[0x05]           = blue;

        usb_buf[0x06]           = red;

        usb_buf[0x07]           = green;
        usb_buf[0x08]           = blue;

        for(int i = 0xb0; i <= 0xb3; i++)
        {
            usb_buf[0x01]       = i; //zone b0->b3
            hid_send_feature_report(dev, usb_buf, 9);
        }
    }

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = 0xb6;

    hid_send_feature_report(dev, usb_buf, 9);
}
