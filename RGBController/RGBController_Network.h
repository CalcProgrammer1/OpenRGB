/*---------------------------------------------------------*\
| RGBController_Network.h                                   |
|                                                           |
|   RGBController implementation that represents a remote   |
|   RGBController instance from a connected OpenRGB server  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "NetworkClient.h"

class RGBController_Network : public RGBController
{
public:
    RGBController_Network(NetworkClient * client_ptr, unsigned int dev_idx_val);
    ~RGBController_Network();

    unsigned int    GetID();

    void            ClearSegments(int zone);
    void            AddSegment(int zone, segment new_segment);
    void            ResizeZone(int zone, int new_size);

    void            DeviceUpdateLEDs();
    void            DeviceUpdateZoneLEDs(int zone);
    void            DeviceUpdateSingleLED(int led);

    void            SetCustomMode();
    void            DeviceUpdateMode();
    void            DeviceUpdateZoneMode(int zone);
    void            DeviceSaveMode();

    void            UpdateLEDs();
    void            UpdateMode();

private:
    NetworkClient * client;
    unsigned int    dev_id;

    unsigned char * CreateUpdateLEDsPacket(unsigned int protocol_version);
    unsigned char * CreateUpdateModePacket(int mode_idx, unsigned int* size, unsigned int protocol_version);
    unsigned char * CreateUpdateSingleLEDPacket(int led);
    unsigned char * CreateUpdateZoneLEDsPacket(int zone);
    unsigned char * CreateUpdateZoneModePacket(int zone_idx, int mode_idx, unsigned int* size, unsigned int protocol_version);
};
