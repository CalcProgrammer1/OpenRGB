/*-----------------------------------------*\
|  SteelSeriesSiberiaController.cpp         |
|                                           |
|  Definitions and types for SteelSeries    |
|  Siberia lighting controller              |
|                                           |
|  E Karlsson (pilophae) 18/6/2020          |
|  
\*-----------------------------------------*/

#include "SteelSeriesSiberiaController.h"
#include <cstring>

static void send_usb_msg(hid_device* dev, char * data_pkt, unsigned int size)
{
    unsigned char usb_pkt[16];
    memset(usb_pkt, 0x00, sizeof(usb_pkt));
    
    // Report number
    usb_pkt[0] = 0x01;
    // Magic
    usb_pkt[1] = 0x00;
    // Command
    usb_pkt[2] = data_pkt[0];
    // Payload length
    usb_pkt[3] = size - 1;

    for(unsigned int i = 0; i < (size - 1); i++)
    {
        usb_pkt[4 + i] = data_pkt[1 + i];
    }
    
    hid_write(dev, usb_pkt, 16);
}

SteelSeriesSiberiaController::SteelSeriesSiberiaController
    (
    hid_device*         dev_handle,
    const char*         path
    )
{
    dev         = dev_handle;
    location    = path;
}

SteelSeriesSiberiaController::~SteelSeriesSiberiaController()
{
    hid_close(dev);
}

std::string SteelSeriesSiberiaController::GetDeviceLocation()
{
    return("HID: " + location);
}

char* SteelSeriesSiberiaController::GetDeviceName()
{
    return device_name;
}

std::string SteelSeriesSiberiaController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);
    
    if (ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void SteelSeriesSiberiaController::SetColor
    (
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    char usb_buf[4];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    
    // Command 1
    usb_buf[0] = 0x95;
    usb_buf[1] = 0x80;
    usb_buf[2] = 0xbf;
    send_usb_msg(dev, usb_buf, 3);

    // Command 2
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0] = 0x80;
    usb_buf[1] = 0x52;
    usb_buf[2] = 0x20;
    send_usb_msg(dev, usb_buf, 3);

    // Command 3 (Set color)
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0] = 0x83;
    usb_buf[1] = red;
    usb_buf[2] = green;
    usb_buf[3] = blue;
    send_usb_msg(dev, usb_buf, 4);

    // Command 4
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0] = 0x93;
    usb_buf[1] = 0x03;
    usb_buf[2] = 0x80;
    send_usb_msg(dev, usb_buf, 3);
}
