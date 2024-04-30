/*-------------------------------------------------------------------*\
|  RoccatKoneAimoController.cpp                                         |
|                                                                     |
|  Driver for Roccat Kone Aimo Mouse                                  |
|                                                                     |
|  Thibaud M (enlight3d)          17th Nov 2020                       |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RoccatKoneAimoController.h"

#include <cstring>

RoccatKoneAimoController::RoccatKoneAimoController(hid_device* dev_handle, char *_path)
{
    dev                 = dev_handle;
    location            = _path;

    const int szTemp    = 256;
    wchar_t tmpName[szTemp];

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    std::wstring wName = std::wstring(tmpName);
    device_name        = std::string(wName.begin(), wName.end());

    hid_get_product_string(dev, tmpName, szTemp);
    wName = std::wstring(tmpName);
    device_name.append(" ").append(std::string(wName.begin(), wName.end()));

    hid_get_serial_number_string(dev, tmpName, szTemp);
    wName  = std::wstring(tmpName);
    serial = std::string(wName.begin(), wName.end());

    /*-----------------------------------------------------*\
    | Init usb buffer to 0 and add first two bytes          |
    \*-----------------------------------------------------*/
    memset(usb_colors_buf, 0x00, USB_COLOR_BUFF_LEN);
    usb_colors_buf[0x00]   = 0x0D;
    usb_colors_buf[0x01]   = 0x2E;

    SendInit();
}

RoccatKoneAimoController::~RoccatKoneAimoController()
{
    hid_close(dev);
}

std::string RoccatKoneAimoController::GetDeviceName()
{
    return device_name;
}

std::string RoccatKoneAimoController::GetSerial()
{
    return serial;
}

std::string RoccatKoneAimoController::GetLocation()
{
    return("HID: " + location);
}

void RoccatKoneAimoController::SendInit()
{
    unsigned char usb_buf[6] = {00};

    /*-----------------------------------------------------*\
    | Read first a packet from mouse (swarm does it)        |
    \*-----------------------------------------------------*/
    hid_get_feature_report(dev, usb_buf, 3);

    /*-----------------------------------------------------*\
    | Set up Init packet                                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x0E;
    usb_buf[0x01]   = 0x06;
    usb_buf[0x02]   = 0x01;
    usb_buf[0x03]   = 0x01;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = 0xFF;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, 6);
}

void RoccatKoneAimoController::SetChannelColors(ROCCAT_KONE_AIMO_CHANNEL channel, RGBColor * colors, unsigned int num_colors)
{
    /*---------------------------------------------------------*\
    | Receiving update request for only one channel             |
    | and updating usb buffer to match colors                   |
    \*---------------------------------------------------------*/
    for(unsigned char i = 0; i < num_colors; i++)
    {
        std::size_t color   = channel + i;
        int usb_idx         = 0x02 + (color * 4);

        usb_colors_buf[usb_idx + R_OFFSET] = RGBGetRValue(colors[i]);
        usb_colors_buf[usb_idx + G_OFFSET] = RGBGetGValue(colors[i]);
        usb_colors_buf[usb_idx + B_OFFSET] = RGBGetBValue(colors[i]);
    }
}

void RoccatKoneAimoController::SendUpdate()
{
    /*-----------------------------------------------------*\
    | Send packet (whole buffer needs to be sent everytime) |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_colors_buf, 46);
    /*-----------------------------------------------------*\
    | Read a packet from mouse (swarm does it)              |
    \*-----------------------------------------------------*/
    hid_get_feature_report(dev, usb_colors_buf, 3);
}
