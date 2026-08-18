/*---------------------------------------------------------*\
| MSIMotherboardControllerDetect.cpp                        |
|                                                           |
|   Detector for MSI Mystic Light motherboards              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "dmiinfo.h"
#include "LogManager.h"
#include "MSIMotherboard112Controller.h"
#include "MSIMotherboard162Controller.h"
#include "MSIMotherboard185Controller.h"
#include "MSIMotherboard761Controller.h"
#include "RGBController_MSIMotherboard112.h"
#include "RGBController_MSIMotherboard162.h"
#include "RGBController_MSIMotherboard185.h"
#include "RGBController_MSIMotherboard761.h"

/*---------------------------------------------------------*\
| USB IDs                                                   |
\*---------------------------------------------------------*/
#define MSI_USB_VID                                 0x1462
#define MSI_USB_VID_COMMON                          0x0DB0

/*---------------------------------------------------------------------------------*\
| WARNING!                                                                          |
|                                                                                   |
|   The MSI Mystic Light controller had a bricking risk in the past.                |
|   The code has been tested on a few boards and the bricking issue has been fixed. |
|   Uncomment this line to enable for untested boards.  Do so at your own risk.     |
\*---------------------------------------------------------------------------------*/
//#define ENABLE_UNTESTED_MYSTIC_LIGHT

DetectedControllers DetectMSIMotherboardControllers(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev != nullptr)
    {
        unsigned char temp_buffer[200];
        temp_buffer[0]              = 0x52;

        size_t      packet_length   = hid_get_feature_report(dev, temp_buffer, 200);

        DMIInfo     dmi;
        std::string dmi_name        = "MSI " + dmi.getMainboard();

        if((packet_length >= sizeof(FeaturePacket_185)) && (packet_length <= (sizeof(FeaturePacket_185) + 1)))  //WHY r we doing this ? why not ==
        {
            MSIMotherboard185Controller*     controller     = new MSIMotherboard185Controller(dev, info->path, info->product_id, dmi_name);
            RGBController_MSIMotherboard185* rgb_controller = new RGBController_MSIMotherboard185(controller);

            detected_controllers.push_back(rgb_controller);
        }
        else if((packet_length >= sizeof(FeaturePacket_162)) && (packet_length <= (sizeof(FeaturePacket_162) + 1)))
        {
            MSIMotherboard162Controller*     controller     = new MSIMotherboard162Controller(dev, info->path, info->product_id, dmi_name);
            RGBController_MSIMotherboard162* rgb_controller = new RGBController_MSIMotherboard162(controller);

            detected_controllers.push_back(rgb_controller);
        }
        else if((packet_length >= sizeof(FeaturePacket_112)) && (packet_length <= (sizeof(FeaturePacket_112) + 1)))
        {
            MSIMotherboard112Controller*     controller     = new MSIMotherboard112Controller(dev, info->path, dmi_name);
            RGBController_MSIMotherboard112* rgb_controller = new RGBController_MSIMotherboard112(controller);

            detected_controllers.push_back(rgb_controller);
        }
        else    // no supported length returned
        {
            unsigned char second_buffer [761];
            second_buffer[0] = 0x50;

            memset(second_buffer + sizeof(unsigned char), 0x0, sizeof(second_buffer) - sizeof(unsigned char));

            //Using this enables subsequent reads to work for some reason
            size_t enable_reading_packet = hid_get_feature_report(dev, second_buffer, 290);
            LOG_INFO("Read %i bytes from read enable packet, subsequent get reports should work", enable_reading_packet);

            memset(second_buffer + sizeof(unsigned char), 0x0, sizeof(second_buffer) - sizeof(unsigned char));


            second_buffer[0] = 0x51;

            size_t packet_length_new_attempt = hid_send_feature_report(dev, second_buffer, 761);

            if(packet_length_new_attempt > 0)
            {
                try
                {
                    MSIMotherboard761Controller*     controller     = new MSIMotherboard761Controller(dev, (const char *) info->path, dmi_name);
                    RGBController_MSIMotherboard761* rgb_controller = new RGBController_MSIMotherboard761(controller);

                    detected_controllers.push_back(rgb_controller);
                }
                catch(const std::runtime_error& e)
                {
                    if(strcmp(e.what(), BOARD_UNSUPPORTED_ERROR) != 0)
                    {
                        throw e;
                    }
                    else
                    {
                        LOG_INFO("Found Board %s but does not have valid config", dmi_name.c_str());
                    }
                }
            }
            else
            {
                LOG_INFO("No matching driver found for %s, packet length = %d", dmi_name.c_str(), packet_length);
            }
        }
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_1720",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x1720,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B12",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7B12,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B16",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7B16,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B17",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7B17,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B18",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7B18,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B50",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7B50,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B85",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7B85,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B92",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7B92,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B93",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7B93,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C02",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C02,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C34",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C34,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C35",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C35,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C36",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C36,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C37",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C37,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C56",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C56,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C59",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C59,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C60",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C60,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C67",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C67,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C71",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C71,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C73",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C73,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C75",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C75,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C76",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C76,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C77",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C77,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C79",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C79,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C80",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C80,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C81",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C81,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C82",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C82,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C83",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C83,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C84",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C84,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C86",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C86,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C87",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C87,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C90",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C90,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C91",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C91,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C92",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C92,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C94",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C94,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C95",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C95,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C98",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C98,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D03",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D03,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D04",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D04,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D06",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D06,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D07",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D07,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D08",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D08,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D09",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D09,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D13",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D13,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D14",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D14,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D15",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D15,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D17",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D17,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D18",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D18,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D19",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D19,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D20",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D20,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D25",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D25,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D27",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D27,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D28",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D28,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D29",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D29,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D30",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D30,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D31",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D31,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D32",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D32,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D33",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D33,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D36",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D36,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D37",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D37,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D38",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D38,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D40",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D40,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D41",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D41,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D42",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D42,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D43",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D43,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D46",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D46,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D50",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D50,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D51",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D51,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D52",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D52,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D53",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D53,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D54",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D54,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D59",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D59,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D67",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D67,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D69",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D69,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D70",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D70,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D73",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D73,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D74",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D74,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D75",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D75,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D76",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D76,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D77",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D77,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D78",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D78,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D86",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D86,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D88",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D88,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D89",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D89,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D90",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D90,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D91",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D91,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D93",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D93,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D96",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D96,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D97",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D97,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D98",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D98,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D99",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7D99,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E01",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E01,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E03",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E03,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E06",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E06,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E07",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E07,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E09",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E09,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E10",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E10,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_B926",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0xB926,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E70",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E70,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E59",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E59,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E80",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E80,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E81",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E81,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E34",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E34,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E32",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E32,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E20",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7E20,   0x0001, 0x00);
// Detector for the set of common boards
REGISTER_HID_DETECTOR_PU("MSI Mystic Light Common",     DetectMSIMotherboardControllers,   MSI_USB_VID_COMMON,  MSI_USB_PID_COMMON, 0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light X870",       DetectMSIMotherboardControllers,   MSI_USB_VID_COMMON,  MSI_USB_PID_COMMON, 0xFF00, 0x01);
/*---------------------------------------------------------------------------------------------------------*\
| Dummy entries for boards using common VID and PID                                                         |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("MSI Mystic Light MS_7E12", DetectMSIMotherboardControllers, 0x1462, 0x7E12 )       |
| DUMMY_DEVICE_DETECTOR("MSI Mystic Light MS_7E16", DetectMSIMotherboardControllers, 0x1462, 0x7E16 )       |
| DUMMY_DEVICE_DETECTOR("MSI Mystic Light MS_7E24", DetectMSIMotherboardControllers, 0x1462, 0x7E24 )       |
| DUMMY_DEVICE_DETECTOR("MSI Mystic Light MS_7E26", DetectMSIMotherboardControllers, 0x1462, 0x7E26 )       |
| DUMMY_DEVICE_DETECTOR("MSI Mystic Light MS_7E27", DetectMSIMotherboardControllers, 0x1462, 0x7E27 )       |
| DUMMY_DEVICE_DETECTOR("MSI Mystic Light MS_7E49", DetectMSIMotherboardControllers, 0x1462, 0x7E49 )       |
\*---------------------------------------------------------------------------------------------------------*/

REGISTER_CUSTOM_UDEV_RULE(msi_7e12, "MSI Mystic Light MS_7E12", "SUBSYSTEMS==\"usb|hidraw\", ATTRS{idVendor}==\"1462\", ATTRS{idProduct}==\"7e12\", TAG+=\"uaccess\", TAG+=\"MSI_Mystic_Light_MS_7E12\"");
REGISTER_CUSTOM_UDEV_RULE(msi_7e16, "MSI Mystic Light MS_7E16", "SUBSYSTEMS==\"usb|hidraw\", ATTRS{idVendor}==\"1462\", ATTRS{idProduct}==\"7e16\", TAG+=\"uaccess\", TAG+=\"MSI_Mystic_Light_MS_7E16\"");
REGISTER_CUSTOM_UDEV_RULE(msi_7e24, "MSI Mystic Light MS_7E24", "SUBSYSTEMS==\"usb|hidraw\", ATTRS{idVendor}==\"1462\", ATTRS{idProduct}==\"7e24\", TAG+=\"uaccess\", TAG+=\"MSI_Mystic_Light_MS_7E24\"");
REGISTER_CUSTOM_UDEV_RULE(msi_7e26, "MSI Mystic Light MS_7E26", "SUBSYSTEMS==\"usb|hidraw\", ATTRS{idVendor}==\"1462\", ATTRS{idProduct}==\"7e26\", TAG+=\"uaccess\", TAG+=\"MSI_Mystic_Light_MS_7E26\"");
REGISTER_CUSTOM_UDEV_RULE(msi_7e27, "MSI Mystic Light MS_7E27", "SUBSYSTEMS==\"usb|hidraw\", ATTRS{idVendor}==\"1462\", ATTRS{idProduct}==\"7e27\", TAG+=\"uaccess\", TAG+=\"MSI_Mystic_Light_MS_7E27\"");
REGISTER_CUSTOM_UDEV_RULE(msi_7e49, "MSI Mystic Light MS_7E49", "SUBSYSTEMS==\"usb|hidraw\", ATTRS{idVendor}==\"1462\", ATTRS{idProduct}==\"7e49\", TAG+=\"uaccess\", TAG+=\"MSI_Mystic_Light_MS_7E49\"");

#ifdef ENABLE_UNTESTED_MYSTIC_LIGHT
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_3EA4",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x3EA4,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_4459",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x4459,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B10",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7B10,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B94",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7B94,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B96",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7B96,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C42",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C42,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C70",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C70,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C85",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C85,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C88",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C88,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C89",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C89,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C96",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C96,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C99",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x7C99,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_905D",    DetectMSIMotherboardControllers,   MSI_USB_VID,    0x905D,   0x0001, 0x00);
#endif
