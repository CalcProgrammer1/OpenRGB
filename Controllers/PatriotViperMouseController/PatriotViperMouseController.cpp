/*---------------------------------------------------------*\
| PatriotViperMouseController.cpp                           |
|                                                           |
|   Detector for Patriot Viper Mouse                        |
|                                                           |
|   mi4code                                     23 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <PatriotViperMouseController.h>

PatriotViperMouseController::PatriotViperMouseController(hid_device* dev_handle, const char* path) 
{
    _dev                            = dev_handle;
    _location                       = path;

    const unsigned char init_packet[64] = {0x01, 0x00, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x50, 0xDE, 0x8D, 0x77, 0x09, 0xDF, 0x8D, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x58, 0x7C, 0x77, 0x78, 0x81, 0x43, 0x00, 0x30, 0x58, 0x7C, 0x77, 0x8C, 0x5D, 0x9B, 0x77, 0x00, 0x00, 0x3D, 0x00, 0x98, 0xF5, 0x19, 0x08, 0x00, 0x00, 0x00, 0xEE};
    hid_send_feature_report(_dev, init_packet, 64);
}

PatriotViperMouseController::~PatriotViperMouseController()
{
    hid_close(_dev);
}

std::string PatriotViperMouseController::GetLocation()
{
    return "HID " + _location;
}

std::string PatriotViperMouseController::GetSerial() 
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(_dev, serial_string, 128);
    if(ret != 0)
    {
        serial_string[0] = '\0'; 
    }
    return StringUtils::wstring_to_string(serial_string);
}

void PatriotViperMouseController::SetRGB(std::vector<RGBColor> colors)
{

    /*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\
    |                                        led         red   green blue                                                                                                                                                                                                                                                                                                                                                  checksum  |
    \*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    unsigned char buffer[64] = { 0x01, 0x13, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEC};
    
    for(unsigned char led_index = 0x00; led_index < 0x07; led_index++)
    {
        buffer[2] = led_index;
        buffer[4] = RGBGetRValue(colors[led_index]);
        buffer[5] = RGBGetGValue(colors[led_index]);
        buffer[6] = RGBGetBValue(colors[led_index]);


        /*--------------------------------------*\
        | calculate the last checksum byte       |
        \*--------------------------------------*/

        unsigned char xor_value = 0;

        for(int i = 0; i < 63; ++i)
        {
            xor_value ^= buffer[i];
        }

        if(xor_value % 2 == 0)
        {
            buffer[63] = (xor_value + 1) % 256;
        } 
        else 
        {
            buffer[63] = (xor_value - 1) % 256;
        }

        hid_send_feature_report(_dev, buffer, 64);
    }

}