/*---------------------------------------------------------*\
| ElgatoStreamDeckController.cpp                            |
|                                                           |
|   Driver for Elgato Stream Deck MK.2                      |
|                                                           |
|   Ferréol DUBOIS COLI (Fefe_du_973)           23 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <vector>
#include "ElgatoStreamDeckController.h"
#include "StringUtils.h"

ElgatoStreamDeckController::ElgatoStreamDeckController(hid_device* dev_handle, const char* path) :
    dev(dev_handle), location(path)
{
}

ElgatoStreamDeckController::~ElgatoStreamDeckController()
{
    hid_close(dev);
}

std::string ElgatoStreamDeckController::GetLocation()
{
    return location;
}

std::string ElgatoStreamDeckController::GetSerialString()
{
    wchar_t serial[256];
    if(hid_get_serial_number_string(dev, serial, 256) >= 0)
    {
        std::wstring ws(serial);
        return StringUtils::wstring_to_string(ws);
    }
    return "";
}

void ElgatoStreamDeckController::SetBrightness(unsigned char brightness)
{
    unsigned char buffer[32] = {0x03, 0x08, brightness};
    hid_send_feature_report(dev, buffer, sizeof(buffer));
}

void ElgatoStreamDeckController::SendFullFrame(const std::vector<std::vector<unsigned char>>& buttonImages)
{
    for(int btnIdx = 0; btnIdx < 15; btnIdx++)
    {
        if(btnIdx < buttonImages.size())
        {
            SendButtonImage(btnIdx, buttonImages[btnIdx]);
        }
    }
}

void ElgatoStreamDeckController::SendButtonImage(int buttonIndex, const std::vector<unsigned char>& jpegData)
{
    const size_t headerSize = 8;
    const size_t packetSize = 1024;
    unsigned char buffer[packetSize] = {0};

    buffer[0] = 0x02;
    buffer[1] = 0x07;
    buffer[2] = buttonIndex;
    buffer[3] = 0x01;
    buffer[4] = jpegData.size() & 0xFF;
    buffer[5] = (jpegData.size() >> 8) & 0xFF;
    buffer[6] = 0x00;
    buffer[7] = 0x00;

    size_t bytesToCopy = std::min(jpegData.size(), packetSize - headerSize);
    memcpy(buffer + headerSize, jpegData.data(), bytesToCopy);

    hid_write(dev, buffer, packetSize);
}

void ElgatoStreamDeckController::Reset()
{
    unsigned char resetBuffer[32] = {0x03, 0x02};
    hid_send_feature_report(dev, resetBuffer, sizeof(resetBuffer));
}
