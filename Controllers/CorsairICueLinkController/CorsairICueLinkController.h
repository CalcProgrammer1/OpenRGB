/*---------------------------------------------------------*\
| CorsairICueLinkController.h                               |
|                                                           |
|   Driver for Corsair iCue Link System Hub                 |
|                                                           |
|   Aiden Vigue (acvigue)                       02 Mar 2025 |
|   Adam Honse <calcprogrammer1@gmail.com>      01 Aug 2025 |
|   Nikola Jurkovic                             11 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <vector>
#include <hidapi.h>

#include "CorsairICueLinkProtocol.h"
#include "DeviceGuardManager.h"
#include "RGBController.h"

class CorsairICueLinkController
{
public:
    CorsairICueLinkController(hid_device* dev_handle, const char* path, std::string name);
    ~CorsairICueLinkController();

    std::string                                 GetFirmwareString();
    std::string                                 GetNameString();
    std::string                                 GetLocationString();
    std::string                                 GetSerialString();

    std::vector<const CorsairICueLinkDevice *>  GetEndpoints();

    void                                        UpdateLights(RGBColor* colors, std::size_t num_colors);

private:
    hid_device*                                 dev;
    std::string                                 name;
    std::string                                 location;
    std::string                                 serial;
    std::vector<unsigned short>                 version;

    DeviceGuardManager*                         guard_manager_ptr;

    std::vector<const CorsairICueLinkDevice *>  endpoints;

    void                                        GetControllerFirmware();
    void                                        GetControllerDevices();
    void                                        SetControllerSoftwareMode();
    void                                        SetControllerHardwareMode();

    std::vector<std::vector<unsigned char>>     ProcessMultiChunkPacket(const std::vector<unsigned char>& data, size_t max_chunk_size);
    std::vector<unsigned char>                  SendCommand(std::vector<unsigned char> command, std::vector<unsigned char> data, std::vector<unsigned char> waitForDataType);

    std::vector<unsigned char>                  Read(std::vector<unsigned char> endpoint, std::vector<unsigned char> data_type);
    void                                        Write(std::vector<unsigned char> endpoint, std::vector<unsigned char> data_type, std::vector<unsigned char> data, CORSAIR_ICUE_ENDPOINT_TYPE endpoint_type);
};
