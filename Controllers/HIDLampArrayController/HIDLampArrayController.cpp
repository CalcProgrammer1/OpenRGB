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

#include <cstdio>
#include <cstring>
#include "hid_util.h"
#include "HIDLampArrayController.h"
#include "StringUtils.h"

HIDLampArrayController::HIDLampArrayController(hid_device *dev_handle, const char *path)
{
    dev                                 = dev_handle;
    location                            = path;

    /*-----------------------------------------------------*\
    | Parse report IDs from descriptor                      |
    \*-----------------------------------------------------*/
    unsigned int  curr_collection_usage = 0;
    unsigned char data_len              = 0;
    unsigned char key                   = 0;
    unsigned char key_cmd               = 0;
    unsigned char key_size              = 0;
    unsigned int  pos                   = 0;
    unsigned char report_descriptor[HID_API_MAX_REPORT_DESCRIPTOR_SIZE];
    unsigned char report_id             = 0;
    unsigned int  size                  = hid_get_report_descriptor(dev, report_descriptor, sizeof(report_descriptor));
    unsigned int  usage                 = 0;
    unsigned char usage_page            = 0;

    /*-----------------------------------------------------*\
    | Create a list of usages and their report IDs          |
    \*-----------------------------------------------------*/
    std::unordered_map<unsigned int, unsigned int> usage_to_report_id;

    while(pos < size)
    {
        get_hid_item_size(report_descriptor, size, pos, &data_len, &key_size);

        key                             = report_descriptor[pos];
        key_cmd                         = key & 0xFC;

        switch(key)
        {
            case 0xA1:
                curr_collection_usage   = usage;
                if(usage_page == 0x59 && report_id)
                {
                    usage_to_report_id[curr_collection_usage] = report_id;
                }
                break;

            case 0xC0:
                curr_collection_usage   = 0;
                report_id               = 0;
                break;
        }

        switch(key_cmd)
        {
            case 0x4:
                usage_page              = get_hid_report_bytes(report_descriptor, size, data_len, pos);
                break;

            case 0x8:
                usage                   = get_hid_report_bytes(report_descriptor, size, data_len, pos);
                if(data_len == 4)
                {
                    int local_usage_page = usage >> 16;
                    usage &= 0x0000FFFF;
                    if (local_usage_page == 0x59 && report_id)
                    {
                        usage_to_report_id[usage] = report_id;
                    }
                }
                break;

            case 0x84:
                report_id               = get_hid_report_bytes(report_descriptor, size, data_len, pos);
                if (usage_page == 0x59 && curr_collection_usage)
                {
                    usage_to_report_id[curr_collection_usage] = report_id;
                }
                break;
        }
        pos += data_len + key_size;
    }

    /*-----------------------------------------------------*\
    | Get the report IDs for each report                    |
    \*-----------------------------------------------------*/
    for(const std::pair<unsigned int, unsigned int>& pair : usage_to_report_id)
    {
        switch(pair.first)
        {
            case 0x02:
                ids.LampArrayAttributesReportID     = pair.second;
                break;

            case 0x20:
                ids.LampAttributesRequestReportID   = pair.second;
                break;

            case 0x22:
                ids.LampAttributesResponseReportID  = pair.second;
                break;

            case 0x50:
                ids.LampMultiUpdateReportID         = pair.second;
                break;

            case 0x60:
                ids.LampRangeUpdateReportID         = pair.second;
                break;

            case 0x70:
                ids.LampArrayControlReportID        = pair.second;
                break;
        }
    }

    /*-----------------------------------------------------*\
    | If reports are missing, return                        |
    \*-----------------------------------------------------*/
    bool report_missing                 = (ids.LampArrayAttributesReportID == 0)
                                       || (ids.LampAttributesRequestReportID == 0)
                                       || (ids.LampAttributesResponseReportID == 0)
                                       || (ids.LampMultiUpdateReportID == 0)
                                       || (ids.LampRangeUpdateReportID == 0)
                                       || (ids.LampArrayControlReportID == 0);

    if(report_missing)
    {
        LampArray.LampCount = 0;
        LampArray.LampArrayKind = HID_LAMPARRAY_KIND_UNDEFINED;
        return;
    }

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
    wchar_t name_string[128];
    int ret = hid_get_product_string(dev, name_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(name_string));
}

std::string HIDLampArrayController::GetDeviceSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string HIDLampArrayController::GetDeviceVendor()
{
    wchar_t vendor_string[128];
    int ret = hid_get_manufacturer_string(dev, vendor_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(vendor_string));
}

unsigned int HIDLampArrayController::GetLampArrayKind()
{
    return(LampArray.LampArrayKind);
}

unsigned int HIDLampArrayController::GetLampCount()
{
    return(LampArray.LampCount);
}

std::vector<LampAttributes> HIDLampArrayController::GetLamps()
{
    return(Lamps);
}

void HIDLampArrayController::GetLampArrayAttributesReport()
{
    unsigned char   usb_buf[sizeof(LampArrayAttributes) + 1];
    int             report_size;

    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | First byte is the report ID                           |
    \*-----------------------------------------------------*/
    usb_buf[0]          = ids.LampArrayAttributesReportID;

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
    usb_buf[0]      = ids.LampAttributesResponseReportID;

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
    usb_buf[0] = ids.LampArrayControlReportID;

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
    usb_buf[0] = ids.LampAttributesRequestReportID;

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
    usb_buf[0] = ids.LampMultiUpdateReportID;

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
