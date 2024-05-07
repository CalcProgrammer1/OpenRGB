/*---------------------------------------------------------*\
| HIDLampArrayController.h                                  |
|                                                           |
|   Driver for HID LampArray Devices                        |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      26 Mar 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi/hidapi.h>

#include "RGBController.h"

typedef struct
{
    hid_device *    hid_dev_LampArray;
    hid_device *    hid_dev_LampArrayAttributesReport;
    hid_device *    hid_dev_LampAttributesRequestReport;
    hid_device *    hid_dev_LampAttributesResponseReport;
    hid_device *    hid_dev_LampMultiUpdateReport;
    hid_device *    hid_dev_LampRangeUpdateReport;
    hid_device *    hid_dev_LampArrayControlReport;
} lamparray_hid_devs;

enum
{
    HID_LAMPARRAY_KIND_UNDEFINED        = 0x00,     /* Undefined                    */
    HID_LAMPARRAY_KIND_KEYBOARD         = 0x01,     /* LampArrayKindKeyboard        */
    HID_LAMPARRAY_KIND_MOUSE            = 0x02,     /* LampArrayKindMouse           */
    HID_LAMPARRAY_KIND_GAME_CONTROLLER  = 0x03,     /* LampArrayKindGameController  */
    HID_LAMPARRAY_KIND_PERIPHERAL       = 0x04,     /* LampArrayKindPeripheral      */
    HID_LAMPARRAY_KIND_SCENE            = 0x05,     /* LampArrayKindScene           */
    HID_LAMPARRAY_KIND_NOTIFICATIOn     = 0x06,     /* LampArrayKindNotification    */
    HID_LAMPARRAY_KIND_CHASSIS          = 0x07,     /* LampArrayKindChassis         */
    HID_LAMPARRAY_KIND_WEARABLE         = 0x08,     /* LampArrayKindWearable        */
    HID_LAMPARRAY_KIND_FURNITURE        = 0x09,     /* LampArrayKindFurniture       */
    HID_LAMPARRAY_KIND_ART              = 0x0A,     /* LampArrayKindArt             */
};

class HIDLampArrayController
{
public:
    HIDLampArrayController(lamparray_hid_devs *devs_handle);
    ~HIDLampArrayController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    unsigned int GetLampCount();

private:

    void GetLampArrayAttributesReport();

    lamparray_hid_devs * devs;
    std::string location;

    /*-----------------------------------------------------*\
    | Parameters from LampArrayAttributesReport             |
    \*-----------------------------------------------------*/
    unsigned short  lamp_count;
    unsigned int    bounding_width;
    unsigned int    bounding_height;
    unsigned int    bounding_depth;
    unsigned int    kind;
    unsigned int    min_interval;
};
