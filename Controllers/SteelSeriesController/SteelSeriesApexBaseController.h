/*---------------------------------------------------------*\
| SteelSeriesApexBaseController.h                           |
|                                                           |
|   Driver base for SteelSeries Apex                        |
|                                                           |
|   Florian Heilmann (FHeilmann)                19 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <cstring>
#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "SteelSeriesDevices.h"

#define STEELSERIES_PACKET_IN_SIZE      64
#define STEELSERIES_PACKET_OUT_SIZE     STEELSERIES_PACKET_IN_SIZE + 1

/*-------------------------------------------------*\
| Protocol quirk applicability                      |
|                                                   |
| Gen 1: 2019-22 models with original case design,  |
|        all FW releases.                           |
| Gen 2: '2023' USB-C + Apex 9 series TKL and       |
|        smaller, wired/wireless with pre-prism FW  |
| Gen 3: All models with Omnipoint 3.0 switches and |
|        Gen 2 models with prism FW update          |
\*-------------------------------------------------*/
typedef enum
{
    APEX_GEN1                 = 0x00,
    APEX_GEN2                 = 0x01,
    APEX_GEN3                 = 0x02,

} protocol_quirk;

enum
{
    APEX_MODE_DIRECT,
    APEX_MODE_ONBOARD,
};

enum
{
    APEX_PACKET_LENGTH                  = 643,
    APEX_9_PACKET_LENGTH                = 513,      /* Test if required           */
    APEX_PACKET_ID_PROFILE              = 0x89,     /* Profile                    */
    APEX_PACKET_ID_FIRMWARE             = 0x90,     /* Firmware version           */
    APEX_GEN1_PACKET_ID_DIRECT          = 0x3A,     /* Direct mode                */
    APEX_GEN1_PACKET_ID_ONBOARD         = 0x3B,     /* Onboard mode               */
    APEX_GEN1_PACKET_ID_SERIAL          = 0xFF,     /* Serial number              */
    APEX_2023_PACKET_ID_DIRECT          = 0x40,     /* New Wired Direct mode      */
    APEX_2023_PACKET_ID_DIRECT_WIRELESS = 0x61,     /* New Wireless Direct mode   */
    APEX_2023_PACKET_ID_INIT            = 0x4B,     /* New Initialization         */
    APEX_2023_PACKET_ID_ONBOARD         = 0x41,     /* New Wired onboard mode     */
    APEX_2023_PACKET_ID_REGION          = 0xF5,     /* Region byte                */
};

class SteelSeriesApexBaseController
{
public:
    SteelSeriesApexBaseController(hid_device* dev_handle, const char* path, std::string dev_name);
    virtual ~SteelSeriesApexBaseController();

    std::string         GetLocation();
    std::string         GetName();
    virtual std::string GetSerial();
    virtual std::string GetVersion();

    virtual void        SetMode(unsigned char mode, std::vector<RGBColor> colors)  = 0;

    virtual void        SetLEDsDirect(std::vector<RGBColor> colors)                = 0;

    steelseries_type    proto_type;

protected:
    hid_device*         dev;
    unsigned char       active_mode;
    std::string         location;
    std::string         name;
    protocol_quirk      kbd_quirk;
    unsigned short      device_pid;
    unsigned char       reset_cmd;
};
