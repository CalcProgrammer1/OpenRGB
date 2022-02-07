/*---------------------------------------------------------*\
|  AirgooLustrousCommanderController.h                      |
|                                                           |
|  Definitions for Airgoo Lustrous Commander                |
|  Based on code by:                                        |
|  Jeff P (@jeffp1), 2020/02/07                             |
|                                                           |
|  Zachary G                                                |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include <vector>
#include <chrono>
#include <hidapi/hidapi.h>

#pragma once

#define AIRGOO_LUSTROUS_COMMANDER_PACKET_SIZE         65
#define AIRGOO_LUSTROUS_COMMANDER_NUM_RGB_CHANNELS     2
#define AIRGOO_LUSTROUS_COMMANDER_NUM_FAN_CHANNELS     4

class AirgooLustrousCommanderController
{
public:
    AirgooLustrousCommanderController(hid_device* dev_handle, const char* path);
    ~AirgooLustrousCommanderController();

    std::string GetLocationString();

    void        SetFanMode();
    void        SetMode(unsigned char mode);
    void        UpdateDevice
                (
                unsigned char   mode,
                unsigned char   red,
                unsigned char   grn,
                unsigned char   blu,
                unsigned char   speed
                );

    
private:
    hid_device*             dev;
    unsigned char           active_mode;
    std::atomic<bool>       controller_ready;
    std::string             location;

    void        InitController();
};
