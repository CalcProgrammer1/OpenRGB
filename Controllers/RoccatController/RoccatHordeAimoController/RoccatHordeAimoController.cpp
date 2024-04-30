/*-------------------------------------------------------------------*\
|  RoccatHordeAimoController.cpp                                      |
|                                                                     |
|  Driver for Roccat Horde Aimo Keyboard                              |
|                                                                     |
|  Morgan Guimard (morg)          2/24/2022                           |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RoccatHordeAimoController.h"

#include <cstring>

RoccatHordeAimoController::RoccatHordeAimoController(hid_device* dev_handle, const hid_device_info& info)
{
    dev                 = dev_handle;
    version             = "";
    location            = info.path;

    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        serial_number = "";
    }
    else
    {
        std::wstring return_wstring = serial_string;
        serial_number = std::string(return_wstring.begin(), return_wstring.end());
    }

    InitialPacket();
}

RoccatHordeAimoController::~RoccatHordeAimoController()
{
    hid_close(dev);
}

void RoccatHordeAimoController::InitialPacket()
{
    unsigned char usb_buf[8];

    memset(usb_buf, 0x00,8);

    usb_buf[0x00] = 0x13;
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x01;

    hid_send_feature_report(dev, usb_buf, 8);
}

std::string RoccatHordeAimoController::GetFirmwareVersion()
{
    return version;
}

std::string RoccatHordeAimoController::GetSerialString()
{
    return serial_number;
}

std::string RoccatHordeAimoController::GetDeviceLocation()
{
    return("HID: " + location);
}

void RoccatHordeAimoController::SetColors(std::vector<RGBColor> colors)
{
    unsigned char usb_buf[WRITE_PACKET_LENGTH];

    usb_buf[0x00] = REPORT_ID;
    usb_buf[0x01] = WRITE_PACKET_LENGTH;
    usb_buf[0x02] = 0xFF;
    usb_buf[0x03] = 0xFF;

    for(unsigned int i = 0; i < 6; i++)
    {
        usb_buf[0x04 + (i * 3)] = RGBGetRValue(colors[i]);
        usb_buf[0x05 + (i * 3)] = RGBGetGValue(colors[i]);
        usb_buf[0x06 + (i * 3)] = RGBGetBValue(colors[i]);
    }

    int crc = 0;

    for(unsigned int i = 0; i < WRITE_PACKET_LENGTH - 2; i++)
    {
        crc += usb_buf[i];
    }

    usb_buf[22] = crc;
    usb_buf[23] = crc >> 8;

    hid_send_feature_report(dev, usb_buf, WRITE_PACKET_LENGTH);

    unsigned char usb_read_buf[READ_PACKET_LENGTH];
    hid_get_feature_report(dev, usb_read_buf, READ_PACKET_LENGTH);
}
