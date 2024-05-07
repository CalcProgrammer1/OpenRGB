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

/*---------------------------------------------------------*\
| Struct packing macro for GCC and MSVC                     |
\*---------------------------------------------------------*/
#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

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

PACK(struct LampArrayAttributes
{
    unsigned short  LampCount;
    unsigned int    BoundingBoxWidthInMillimeters;
    unsigned int    BoundingBoxHeightInMillimeters;
    unsigned int    BoundingBoxDepthInMillimeters;
    unsigned int    LampArrayKind;
    unsigned int    MinUpdateIntervalInMilliseconds;
});

PACK(struct LampArrayColor
{
    unsigned char   RedChannel;
    unsigned char   GreenChannel;
    unsigned char   BlueChannel;
    unsigned char   IntensityChannel;
});

PACK(struct LampArrayControl
{
    unsigned char   AutonomousMode;
});

PACK(struct LampAttributes
{
    unsigned short  LampId;
    unsigned int    PositionXInMillimeters;
    unsigned int    PositionYInMillimeters;
    unsigned int    PositionZInMillimeters;
    unsigned int    UpdateLatencyInMilliseconds;
    unsigned int    LampPurposes;
    unsigned char   RedLevelCount;
    unsigned char   GreenLevelCount;
    unsigned char   BlueLevelCount;
    unsigned char   IntensityLevelCount;
    unsigned char   IsProgrammable;
    unsigned char   LampKey;
});

PACK(struct LampAttributesRequest
{
    unsigned short  LampId;
});

#define LAMP_MULTI_UPDATE_LAMP_COUNT 8
PACK(struct LampMultiUpdate
{
    unsigned char   LampCount;
    unsigned char   LampUpdateFlags;
    unsigned char   LampIds[LAMP_MULTI_UPDATE_LAMP_COUNT];
    LampArrayColor  UpdateColors[LAMP_MULTI_UPDATE_LAMP_COUNT];
});

enum
{
    HID_LAMPARRAY_LAMP_ARRAY_ATTRIBUTES_REPORT_ID       = 0x01, /* LampArrayAttributesReport ID     */
    HID_LAMPARRAY_LAMP_ATTRIBUTES_REQUEST_REPORT_ID     = 0x02, /* LampAttributesRequestReport ID   */
    HID_LAMPARRAY_LAMP_ATTRIBUTES_RESPONSE_REPORT_ID    = 0x03, /* LampAttributesResponseReport ID  */
    HID_LAMPARRAY_LAMP_MULTI_UPDATE_REPORT_ID           = 0x04, /* LampMultiUpdateReport ID         */
    HID_LAMPARRAY_LAMP_ARRAY_CONTROL_REPORT_ID          = 0x06, /* LampArrayControlReport ID        */
};

enum
{
    HID_LAMPARRAY_KIND_UNDEFINED        = 0x00,     /* Undefined                    */
    HID_LAMPARRAY_KIND_KEYBOARD         = 0x01,     /* LampArrayKindKeyboard        */
    HID_LAMPARRAY_KIND_MOUSE            = 0x02,     /* LampArrayKindMouse           */
    HID_LAMPARRAY_KIND_GAME_CONTROLLER  = 0x03,     /* LampArrayKindGameController  */
    HID_LAMPARRAY_KIND_PERIPHERAL       = 0x04,     /* LampArrayKindPeripheral      */
    HID_LAMPARRAY_KIND_SCENE            = 0x05,     /* LampArrayKindScene           */
    HID_LAMPARRAY_KIND_NOTIFICATION     = 0x06,     /* LampArrayKindNotification    */
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
    void GetLampAttributesResponseReport();
    void SetLampArrayControlReport(unsigned char AutonomousMode);
    void SetLampAttributesRequestReport(unsigned short LampId);
    void SetLampMultiUpdateReport(unsigned char LampCount, unsigned char LampUpdateFlags, unsigned char * LampIds, LampArrayColor * UpdateColors);

    lamparray_hid_devs * devs;
    std::string location;

    /*-----------------------------------------------------*\
    | Vector to store lamp attributes for each lamp         |
    \*-----------------------------------------------------*/
    std::vector<LampAttributes> Lamps;

    /*-----------------------------------------------------*\
    | Parameters from LampArrayAttributesReport             |
    \*-----------------------------------------------------*/
    LampArrayAttributes         LampArray;
};
