/*---------------------------------------------------------*\
| RealtekBridgeControllerDetect.cpp                         |
|                                                           |
|   Controller for Realtek USB to SSD Bridge ICs            |
|                                                           |
|   Jerry Fan (JerryFan0612)                    13 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "RGBController_RealtekBridge.h"

#define REALTEK_BRIDGE_VID                 0x0BDA
#define REALTEK_BRIDGE_PID0                0x9220
#define REALTEK_BRIDGE_PID1                0x9201
#define REALTEK_BRIDGE_PID2                0x9210

#define REALTEK_HID2SCSI_PG                0xFF00
#define REALTEK_HID2SCSI_USAGE             0x0001

/******************************************************************************************\
*                                                                                          *
*   DetectRealtekBridgeControllers                                                         *
*                                                                                          *
*       Tests the USB address to see if an Realtek Bridge controller exists there          *
*                                                                                          *
\******************************************************************************************/
void DetectRealtekBridgeControllers(hid_device_info* info, const std::string& /*name*/)
{
    RealtekBridgeController*     controller      = NULL;
    RGBController_RealtekBridge* rgb_controller  = NULL;
    hid_device*                  dev             = hid_open_path(info->path);

    if(dev)
    {
        controller = new RealtekBridgeController(dev, info);
        if(controller->get_support_openrgb())
        {
            rgb_controller = new RGBController_RealtekBridge(controller);
            if(rgb_controller->GetDeviceType() != DEVICE_TYPE_UNKNOWN)
            {
                DetectionManager::get()->RegisterRGBController(rgb_controller);
            }
            else
            {
                delete rgb_controller;
            }
        }
        else
        {
            delete controller;
        }
    }
    return;
}

REGISTER_HID_DETECTOR_PU("RTL9220", DetectRealtekBridgeControllers, REALTEK_BRIDGE_VID, REALTEK_BRIDGE_PID0, REALTEK_HID2SCSI_PG, REALTEK_HID2SCSI_USAGE);
REGISTER_HID_DETECTOR_PU("RTL9201", DetectRealtekBridgeControllers, REALTEK_BRIDGE_VID, REALTEK_BRIDGE_PID1, REALTEK_HID2SCSI_PG, REALTEK_HID2SCSI_USAGE);
REGISTER_HID_DETECTOR_PU("RTL9210", DetectRealtekBridgeControllers, REALTEK_BRIDGE_VID, REALTEK_BRIDGE_PID2, REALTEK_HID2SCSI_PG, REALTEK_HID2SCSI_USAGE);
