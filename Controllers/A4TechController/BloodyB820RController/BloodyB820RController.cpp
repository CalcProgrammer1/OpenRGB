/*---------------------------------------------------------*\
| BloodyB820RController.cpp                                 |
|                                                           |
|   Driver for A4Tech Bloody B820R Keyboard                 |
|                                                           |
|   Mohammed Julfikar Ali Mahbub (o-julfikar)   01 Apr 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "BloodyB820RController.h"
#include "StringUtils.h"

/*-------------------------------------------------------------------------------------*\
| The controller for this device should pass a packet of 64 bytes where the subsequent  |
| two packets are for RED, GREEN, and BLUE respectively. The first 6 bytes are control  |
| or information bytes and colors bytes starts from index 6. Moreover, the first pack-  |
| et of each RGB contains color for 58 keys and the rest (104 - 58 == 46) are send on   |
| the second packet.                                                                    |
\*-------------------------------------------------------------------------------------*/

BloodyB820RController::BloodyB820RController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev                 = dev_handle;
    location            = path;
    name                = dev_name;

    SendControlPacket(BLOODY_B820R_GAIN_CONTROL);
}

BloodyB820RController::~BloodyB820RController()
{
    SendControlPacket(BLOODY_B820R_RELEASE_CONTROL);
    hid_close(dev);
}

std::string BloodyB820RController::GetSerial()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string BloodyB820RController::GetLocation()
{
    return("HID: " + location);
}

std::string BloodyB820RController::GetName()
{
    return(name);
}

void  BloodyB820RController::SendControlPacket(uint8_t data)
{
    uint8_t buffer[BLOODY_B820R_PACKET_SIZE]    = { 0x07, 0x03, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00 };

    hid_send_feature_report(dev, buffer, BLOODY_B820R_PACKET_SIZE);

    buffer[BLOODY_B820R_MODE_BYTE]              = 0;
    buffer[BLOODY_B820R_DATA_BYTE]              = data;

    hid_send_feature_report(dev, buffer, BLOODY_B820R_PACKET_SIZE);
}

void BloodyB820RController::SetLEDDirect(std::vector<RGBColor> colors)
{

    uint8_t r1_buffer[BLOODY_B820R_PACKET_SIZE] = { 0x07, 0x03, 0x06, 0x07, 0x00, 0x00 };
    uint8_t r2_buffer[BLOODY_B820R_PACKET_SIZE] = { 0x07, 0x03, 0x06, 0x08, 0x00, 0x00 };
    uint8_t g1_buffer[BLOODY_B820R_PACKET_SIZE] = { 0x07, 0x03, 0x06, 0x09, 0x00, 0x00 };
    uint8_t g2_buffer[BLOODY_B820R_PACKET_SIZE] = { 0x07, 0x03, 0x06, 0x0A, 0x00, 0x00 };
    uint8_t b1_buffer[BLOODY_B820R_PACKET_SIZE] = { 0x07, 0x03, 0x06, 0x0B, 0x00, 0x00 };
    uint8_t b2_buffer[BLOODY_B820R_PACKET_SIZE] = { 0x07, 0x03, 0x06, 0x0C, 0x00, 0x00 };

    /*-----------------------------------------------------------------*\
    | Set up Direct packet                                              |
    |   packet_map is the index of the Key from full_matrix_map and     |
    |   the value is the position in the direct packet buffer           |
    \*-----------------------------------------------------------------*/
    for(size_t i = 0; i < colors.size(); i++)
    {
        RGBColor    color                       = colors[i];
        uint8_t     offset                      = BLOODY_B820R_RGB_OFFSET;
        uint8_t     buffer_idx                  = offset + i % BLOODY_B820R_RGB_BUFFER_SIZE;
        uint8_t*    buffers[2][3]               =
                {
                        {r1_buffer, g1_buffer, b1_buffer},
                        {r2_buffer, g2_buffer, b2_buffer}
                };

        buffers[i >= BLOODY_B820R_RGB_BUFFER_SIZE][0][buffer_idx] = RGBGetRValue(color);
        buffers[i >= BLOODY_B820R_RGB_BUFFER_SIZE][1][buffer_idx] = RGBGetGValue(color);
        buffers[i >= BLOODY_B820R_RGB_BUFFER_SIZE][2][buffer_idx] = RGBGetBValue(color);
    }

    hid_send_feature_report(dev, r1_buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, r2_buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, g1_buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, g2_buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, b1_buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, b2_buffer, BLOODY_B820R_PACKET_SIZE);
}
