/*---------------------------------------------------------*\
| CorsairHydroPlatinumController.h                          |
|                                                           |
|   Driver for Corsair Hydro Platinum coolers               |
|                                                           |
|   Kasper                                      28 Mar 2021 |
|   Nikola Jurkovic (jurkovic.nikola)           13 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"
#include "DeviceGuardManager.h"

#define CORSAIR_HYDRO_PLATINUM_PACKET_SIZE 65
#define CORSAIR_HYDRO_PLATINUM_PACKET_DELAY 5

enum
{
    CORSAIR_HYDRO_PLATINUM_MAGIC_1 = 0b001,
    CORSAIR_HYDRO_PLATINUM_MAGIC_2 = 0b010,
    CORSAIR_HYDRO_PLATINUM_MAGIC_3 = 0b011,

    CORSAIR_HYDRO_PLATINUM_SET_LIGHTING_1 = 0b100,
    CORSAIR_HYDRO_PLATINUM_SET_LIGHTING_2 = 0b101,
    CORSAIR_HYDRO_PLATINUM_SET_LIGHTING_3 = 0b110,
};

class CorsairHydroPlatinumController
{
public:
    CorsairHydroPlatinumController(hid_device* dev_handle, const char* path, bool dev_rgb_fan, std::string dev_name);
    ~CorsairHydroPlatinumController();

    std::string GetLocation();
    std::string GetFirmwareString();
    std::string GetName();
    void SetupColors(std::vector<RGBColor> colors);
    bool HaveRgbFan();

private:
    hid_device*                 dev;
    std::string                 location;
    std::string                 firmware_version;
    std::string                 name;
    std::atomic<unsigned int>   sequence_number;
    DeviceGuardManager*         guard_manager_ptr;
    bool                        have_rgb_fan = true;

    void SendMagic(const uint8_t* magic, unsigned int command);
    void SendColors(std::vector<RGBColor> colors, unsigned int start, unsigned int end, unsigned int command);
    unsigned int GetSequenceNumber();
    uint8_t ComputePEC(const void * data, size_t size);
};
