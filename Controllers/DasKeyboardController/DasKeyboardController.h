/*---------------------------------------------------------*\
| DasKeyboardController.h                                   |
|                                                           |
|   Driver for Das Keyboard keyboard                        |
|                                                           |
|   Frank Niessen (denk_mal)                    16 Dec 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class DasKeyboardController
{
public:
    DasKeyboardController(hid_device *dev_handle, const char *path, std::string dev_name);

    ~DasKeyboardController();

    std::string GetLayoutString();
    std::string GetLocationString();
    std::string GetNameString();
    std::string GetSerialString();
    std::string GetVersionString();

    void SendColors(unsigned char key_id, unsigned char mode, unsigned char red, unsigned char green, unsigned char blue);

    void SendApply();

private:
    hid_device  *dev;
    std::string location;
    std::string name;
    std::string version;
    bool        useTraditionalSendData;

    void SendInitialize();

    void SendData(const unsigned char *data, unsigned int length);

    void SendDataTraditional(const unsigned char *data, unsigned int length);

    void SendDataModern(const unsigned char *data, unsigned int length);

    int ReceiveData(unsigned char *data, unsigned int max_length);
};
