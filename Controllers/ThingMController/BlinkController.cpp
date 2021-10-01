/*-------------------------------------------------------------------*\
|  BlinkController.cpp                                                |
|                                                                     |
|  Driver for ThingM Blink device                                     |
|                                                                     |
|  Eric S (edbgon)          1st Oct 2021                              |
\*-------------------------------------------------------------------*/

#include "BlinkController.h"
#include <cstring>

BlinkController::BlinkController(hid_device* dev_handle, char *_path)
{
    dev             = dev_handle;
    location        = _path;

    const int szTemp = 256;
    wchar_t tmpName[szTemp];

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    std::wstring wName = std::wstring(tmpName);
    device_name        = std::string(wName.begin(), wName.end());

    hid_get_product_string(dev, tmpName, szTemp);
    wName  = std::wstring(tmpName);
    device_name.append(" ").append(std::string(wName.begin(), wName.end()));

    hid_get_serial_number_string(dev, tmpName, szTemp);
    wName  = std::wstring(tmpName);
    serial = std::string(wName.begin(), wName.end());

}

BlinkController::~BlinkController()
{
    if(dev)
    {
        hid_close(dev);
    }
}

std::string BlinkController::GetDeviceName()
{
    return device_name;
}

std::string BlinkController::GetSerial()
{
    return serial;
}

std::string BlinkController::GetLocation()
{
    return("HID: " + location);
}

void BlinkController::SendUpdate(unsigned char led, unsigned char red, unsigned char green, unsigned char blue, unsigned int speed)
{    

        unsigned char buffer[BLINK_PACKET_SIZE] = { 0x00 };
        memset(buffer, 0x00, BLINK_PACKET_SIZE);

        buffer[0x00] = 0x01;
        buffer[0x01] = 0x63;
        buffer[0x02] = red;
        buffer[0x03] = green;
        buffer[0x04] = blue;

        if(speed > 0)
        {
            buffer[0x05] = (speed & 0xff00) >> 8;
            buffer[0x06] = speed & 0x00ff;
        }

        buffer[0x07] = led;

        hid_send_feature_report(dev, buffer, BLINK_PACKET_SIZE);
}
