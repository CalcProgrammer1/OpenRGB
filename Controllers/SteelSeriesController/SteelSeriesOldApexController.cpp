/*-----------------------------------------*\
|  SteelSeriesOldApexController.cpp         |
|                                           |
|  RGB Lighting Library for SteelSeries     |
|  older Apex KB lighting controller        |
|  (Apex, Apex Fnatic, Apex 350)            |
|                                           |
|  David Lee (RAMChYLD) 15/11/2020          |
|                                           |
|  Based on work for SteelSeries Rival by   |
|  B Horn    (bahorn)   13/5/2020           |
|                                           |
|  Based on findings in ApexCtl by          |
|  Audrius/tuxmark5, et. al                 |
|  https://github.com/tuxmark5/ApexCtl      |
\*-----------------------------------------*/


#include "SteelSeriesOldApexController.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

static void send_usb_msg(hid_device* dev, char * data_pkt, unsigned int size)
{
    char* usb_pkt = new char[size + 1];
    
    usb_pkt[0] = 0x00;
    for(unsigned int i = 1; i < size + 1; i++)
    {
        usb_pkt[i] = data_pkt[i-1];
    }
    
    hid_write(dev, (unsigned char *)usb_pkt, size + 1);
    
    delete usb_pkt;
}

SteelSeriesOldApexController::SteelSeriesOldApexController
    (
    hid_device*         dev_handle,
    steelseries_type    proto_type,
    const char*         path
    )
{
    dev         = dev_handle;
    location    = path;
    proto       = proto_type;
}

SteelSeriesOldApexController::~SteelSeriesOldApexController()
{
    hid_close(dev);
}

std::string SteelSeriesOldApexController::GetDeviceLocation()
{
    return("HID: " + location);
}

char* SteelSeriesOldApexController::GetDeviceName()
{
    return device_name;
}

std::string SteelSeriesOldApexController::GetSerialString()
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

steelseries_type SteelSeriesOldApexController::GetKeyboardType()
{
    return proto;
}

void SteelSeriesOldApexController::SetColorDetailed(color32 qwerty, color32 tenkey, color32 functionkey, color32 mxkey, color32 logo)
{
    char usb_buf[32];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x07;
    usb_buf[0x01]   = 0x00; // All zones

    // QWERTY Zone
    usb_buf[0x02]       = qwerty.red;
    usb_buf[0x03]       = qwerty.green;
    usb_buf[0x04]       = qwerty.blue;
    usb_buf[0x05]       = qwerty.alpha;

    // Tenkey Zone
    usb_buf[0x06]       = tenkey.red;
    usb_buf[0x07]       = tenkey.green;
    usb_buf[0x08]       = tenkey.blue;
    usb_buf[0x09]       = tenkey.alpha;

    // FunctionKey Zone
    usb_buf[0x0A]       = functionkey.red;
    usb_buf[0x0B]       = functionkey.green;
    usb_buf[0x0C]       = functionkey.blue;
    usb_buf[0x0D]       = functionkey.alpha;

    // MXKey Zone
    usb_buf[0x0E]       = mxkey.red;
    usb_buf[0x0F]       = mxkey.green;
    usb_buf[0x10]       = mxkey.blue;
    usb_buf[0x11]       = mxkey.alpha;

    //Logo Zone
    usb_buf[0x12]       = logo.red;
    usb_buf[0x13]       = logo.green;
    usb_buf[0x14]       = logo.blue;
    usb_buf[0x15]       = logo.alpha;

    send_usb_msg(dev, usb_buf, 32);
}
