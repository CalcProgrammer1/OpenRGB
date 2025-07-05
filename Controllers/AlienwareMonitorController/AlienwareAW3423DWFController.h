/*---------------------------------------------------------*\
| AlienwareAW3423DWFController.h                            |
|                                                           |
|   Driver for the Alienware AW3423DWF monitor              |
|                                                           |
|   Ferréol DUBOIS COLI (Fefe_du_973)           23 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include <vector>
#include <string>

class AlienwareAW3423DWFController
{
public:
    AlienwareAW3423DWFController(hid_device* dev_handle, const char* path);
    ~AlienwareAW3423DWFController();

    std::string GetLocation();
    std::string GetSerialString();
    void SendColor(unsigned char led_id, unsigned char r, unsigned char g, unsigned char b);

private:
    hid_device* dev;
    std::string location;

    static const std::vector<std::vector<unsigned char>> OEM_KEYS;

    void PerformLogin();
    std::vector<unsigned char> GenerateKey(const std::vector<unsigned char>& response);
    void SendControlPacket(const unsigned char* data, size_t length);
    std::vector<unsigned char> GetReportResponse();
};
