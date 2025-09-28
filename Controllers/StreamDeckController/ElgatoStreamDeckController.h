/*---------------------------------------------------------*\
| ElgatoStreamDeckController.h                              |
|                                                           |
|   Driver for Elgato Stream Deck MK.2                      |
|                                                           |
|   Ferréol DUBOIS COLI (Fefe_du_973)           23 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include <string>
#include <vector>

class ElgatoStreamDeckController
{
public:
    ElgatoStreamDeckController(hid_device* dev_handle, const char* path);
    ~ElgatoStreamDeckController();

    std::string GetLocation();
    std::string GetSerialString();
    void SetBrightness(unsigned char brightness);
    void SendFullFrame(const std::vector<std::vector<unsigned char>>& buttonImages);
    void SendButtonImage(int buttonIndex, const std::vector<unsigned char>& jpegData);

private:
    hid_device* dev;
    std::string location;

    void Reset();
};
