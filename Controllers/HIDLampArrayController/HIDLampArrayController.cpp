/*---------------------------------------------------------*\
| HIDLampArrayController.cpp                                |
|                                                           |
|   Driver for HID LampArray Devices                        |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      26 Mar 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HIDLampArrayController.h"

HIDLampArrayController::HIDLampArrayController(hid_device *dev_handle, const char *path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    /*-----------------------------------------------------*\
    | Get LampArrayAttributesReport                         |
    \*-----------------------------------------------------*/
    GetLampArrayAttributesReport();

    /*-----------------------------------------------------*\
    | Set LampAttributesRequestReport for LampId 0          |
    \*-----------------------------------------------------*/
    SetLampAttributesRequestReport(0);

    /*-----------------------------------------------------*\
    | Get LampAttributesResponseReport for each LampId      |
    \*-----------------------------------------------------*/
    for(unsigned int LampId = 0; LampId < LampArray.LampCount; LampId++)
    {
        GetLampAttributesResponseReport();
    }

    SetLampArrayControlReport(false);
}

HIDLampArrayController::~HIDLampArrayController()
{
    hid_close(dev);
}

std::string HIDLampArrayController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string HIDLampArrayController::GetDeviceName()
{
    return(name);
}

std::string HIDLampArrayController::GetSerialString()
{
    return("");
}

unsigned int HIDLampArrayController::GetLampCount()
{
    return(LampArray.LampCount);
}

void HIDLampArrayController::GetLampArrayAttributesReport()
{
    unsigned char   usb_buf[sizeof(LampArrayAttributes) + 1];
    int             report_size;

    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | First byte is the report ID                           |
    \*-----------------------------------------------------*/
    usb_buf[0]          = HID_LAMPARRAY_LAMP_ARRAY_ATTRIBUTES_REPORT_ID;

    /*-----------------------------------------------------*\
    | Get the report                                        |
    \*-----------------------------------------------------*/
    report_size         = hid_get_feature_report(dev, usb_buf, sizeof(usb_buf));

    memcpy(&LampArray, &usb_buf[1], sizeof(LampArray));
}

void HIDLampArrayController::GetLampAttributesResponseReport()
{
    unsigned char   usb_buf[65];
    int             report_size;
    LampAttributes  attributes;

    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | First byte is the report ID                           |
    \*-----------------------------------------------------*/
    usb_buf[0]      = HID_LAMPARRAY_LAMP_ATTRIBUTES_RESPONSE_REPORT_ID;

    /*-----------------------------------------------------*\
    | Get the report                                        |
    \*-----------------------------------------------------*/
    report_size = hid_get_feature_report(dev, usb_buf, sizeof(usb_buf));

    memcpy(&attributes, &usb_buf[1], sizeof(attributes));

    /*-----------------------------------------------------*\
    | Store the attributes                                  |
    \*-----------------------------------------------------*/
    Lamps.push_back(attributes);
}

void HIDLampArrayController::SetLampArrayControlReport(unsigned char AutonomousMode)
{
    unsigned char   usb_buf[sizeof(LampArrayControl) + 1];

    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | First byte is the report ID                           |
    \*-----------------------------------------------------*/
    usb_buf[0] = HID_LAMPARRAY_LAMP_ARRAY_CONTROL_REPORT_ID;

    /*-----------------------------------------------------*\
    | Fill in control data                                  |
    \*-----------------------------------------------------*/
    ((LampArrayControl *)&usb_buf[1])->AutonomousMode = AutonomousMode;

    /*-----------------------------------------------------*\
    | Send the report                                       |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void HIDLampArrayController::SetLampAttributesRequestReport(unsigned short LampId)
{
    unsigned char   usb_buf[sizeof(LampAttributesRequest) + 1];

    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | First byte is the report ID                           |
    \*-----------------------------------------------------*/
    usb_buf[0] = HID_LAMPARRAY_LAMP_ATTRIBUTES_REQUEST_REPORT_ID;

    /*-----------------------------------------------------*\
    | Fill in request data                                  |
    \*-----------------------------------------------------*/
    ((LampAttributesRequest *)&usb_buf[1])->LampId = LampId;

    /*-----------------------------------------------------*\
    | Send the report                                       |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void HIDLampArrayController::SetLampMultiUpdateReport(unsigned char LampCount, unsigned char LampUpdateFlags, unsigned short * LampIds, LampArrayColor * UpdateColors)
{
    unsigned char   usb_buf[sizeof(LampMultiUpdate) + 1];

    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | First byte is the report ID                           |
    \*-----------------------------------------------------*/
    usb_buf[0] = HID_LAMPARRAY_LAMP_MULTI_UPDATE_REPORT_ID;

    /*-----------------------------------------------------*\
    | Fill in multi update data                             |
    \*-----------------------------------------------------*/
    ((LampMultiUpdate *)&usb_buf[1])->LampCount         = LampCount;
    ((LampMultiUpdate *)&usb_buf[1])->LampUpdateFlags   = LampUpdateFlags;

    for(unsigned char LampId = 0; LampId < LampCount; LampId++)
    {
        ((LampMultiUpdate *)&usb_buf[1])->LampIds[LampId] = LampIds[LampId];
        ((LampMultiUpdate *)&usb_buf[1])->UpdateColors[LampId] = UpdateColors[LampId];
    }

    /*-----------------------------------------------------*\
    | Send the report                                       |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}
