/*---------------------------------------------------------*\
| MSIMysticLightControllerDetect.cpp                        |
|                                                           |
|   Detector for MSI Mystic Light motherboards              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "MSIMysticLight64Controller.h"
#include "MSIMysticLight112Controller.h"
#include "MSIMysticLight162Controller.h"
#include "MSIMysticLight185Controller.h"
#include "MSIMysticLight761Controller.h"
#include "RGBController_MSIMysticLight64.h"
#include "RGBController_MSIMysticLight112.h"
#include "RGBController_MSIMysticLight162.h"
#include "RGBController_MSIMysticLight185.h"
#include "RGBController_MSIMysticLight761.h"
#include "dmiinfo.h"
#include "LogManager.h"

#define MSI_USB_VID         0x1462
#define MSI_USB_VID_COMMON  0x0DB0

/*---------------------------------------------------------------------------------*\
| WARNING!                                                                          |
|                                                                                   |
|   The MSI Mystic Light controller had a bricking risk in the past.                |
|   The code has been tested on a few boards and the bricking issue has been fixed. |
|   Uncomment this line to enable for untested boards.  Do so at your own risk.     |
\*---------------------------------------------------------------------------------*/
//#define ENABLE_UNTESTED_MYSTIC_LIGHT

/*----------------------------------------------------------------------------------------*\
|                                                                                          |
|   DetectMSIMysticLightControllers                                                        |
|                                                                                          |
|       Detect MSI Mystic Light devices                                                    |
|                                                                                          |
\*----------------------------------------------------------------------------------------*/
void DetectMSIMysticLightControllers
    (
    hid_device_info*    info,
    const std::string&  /*name*/
    )
{
    hid_device* dev = hid_open_path(info->path);
    if(dev != nullptr)
    {
        unsigned char temp_buffer[200];
        temp_buffer[0]              = 0x52;

        size_t      packet_length   = hid_get_feature_report(dev, temp_buffer, 200);

        DMIInfo     dmi;
        std::string dmi_name        = "MSI " + dmi.getMainboard();

        if((packet_length >= sizeof(FeaturePacket_185)) && (packet_length <= (sizeof(FeaturePacket_185) + 1)))  //WHY r we doing this ? why not ==
        {
            MSIMysticLight185Controller*     controller     = new MSIMysticLight185Controller(dev, info->path, info->product_id, dmi_name);
            RGBController_MSIMysticLight185* rgb_controller = new RGBController_MSIMysticLight185(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else if((packet_length >= sizeof(FeaturePacket_162)) && (packet_length <= (sizeof(FeaturePacket_162) + 1)))
        {
            MSIMysticLight162Controller*     controller     = new MSIMysticLight162Controller(dev, info->path, info->product_id, dmi_name);
            RGBController_MSIMysticLight162* rgb_controller = new RGBController_MSIMysticLight162(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else if((packet_length >= sizeof(FeaturePacket_112)) && (packet_length <= (sizeof(FeaturePacket_112) + 1)))
        {
            MSIMysticLight112Controller*     controller     = new MSIMysticLight112Controller(dev, info->path, dmi_name);
            RGBController_MSIMysticLight112* rgb_controller = new RGBController_MSIMysticLight112(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else    // no supported length returned
        {
            unsigned char second_buffer[1000];
            second_buffer[0] = 0x51;

            for(int i = 1; i < 1000; i++)
            {
                second_buffer[i] = 0x0;
            }

            size_t packet_length_new_attempt = hid_get_feature_report(dev, second_buffer, 1000);

            if(packet_length_new_attempt >=290 && packet_length_new_attempt <= 291)
            {
                MSIMysticLight761Controller*     controller     = new MSIMysticLight761Controller(dev, info->path, info->product_id, dmi_name);
                RGBController_MSIMysticLight761* rgb_controller = new RGBController_MSIMysticLight761(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);

            }
            else
            {
                LOG_INFO("No matching driver found for %s, packet length = %d", dmi_name.c_str(), packet_length);
                return;
            }
        }
    }
}

void DetectMSIMysticLight64Controllers
    (
    hid_device_info*    info,
    const std::string&  /*name*/
    )
{
    hid_device* dev = hid_open_path(info->path);

    if(dev != nullptr)
    {
        MSIMysticLight64Controller*     controller     = new MSIMysticLight64Controller(dev, info->path);
        RGBController_MSIMysticLight64* rgb_controller = new RGBController_MSIMysticLight64(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}


REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_1562",    DetectMSIMysticLight64Controllers, MSI_USB_VID,    0x1562,   0x00FF, 0x01);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_1563",    DetectMSIMysticLight64Controllers, MSI_USB_VID,    0x1563,   0x00FF, 0x01);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_1564",    DetectMSIMysticLight64Controllers, MSI_USB_VID,    0x1564,   0x00FF, 0x01);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_1720",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x1720,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B12",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7B12,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B16",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7B16,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B17",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7B17,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B18",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7B18,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B50",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7B50,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B85",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7B85,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B93",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7B93,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C34",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C34,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C35",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C35,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C36",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C36,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C37",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C37,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C56",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C56,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C59",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C59,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C60",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C60,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C67",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C67,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C71",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C71,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C73",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C73,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C75",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C75,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C76",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C76,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C77",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C77,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C79",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C79,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C80",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C80,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C81",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C81,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C82",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C82,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C83",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C83,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C84",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C84,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C86",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C86,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C87",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C87,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C90",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C90,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C91",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C91,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C92",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C92,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C94",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C94,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C95",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C95,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C98",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C98,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D03",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D03,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D04",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D04,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D06",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D06,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D07",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D07,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D08",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D08,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D09",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D09,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D13",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D13,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D14",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D14,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D15",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D15,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D17",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D17,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D18",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D18,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D19",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D19,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D20",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D20,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D25",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D25,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D27",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D27,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D28",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D28,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D29",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D29,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D30",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D30,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D31",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D31,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D32",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D32,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D33",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D33,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D36",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D36,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D38",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D38,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D40",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D40,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D41",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D41,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D42",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D42,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D43",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D43,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D46",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D46,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D50",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D50,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D51",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D51,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D52",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D52,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D53",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D53,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D54",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D54,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D59",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D59,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D67",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D67,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D69",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D69,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D70",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D70,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D73",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D73,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D74",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D74,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D75",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D75,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D76",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D76,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D77",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D77,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D78",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D78,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D86",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D86,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D89",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D89,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D90",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D90,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D91",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D91,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D93",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D93,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D96",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D96,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D97",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D97,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D98",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D98,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7D99",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7D99,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E01",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7E01,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E03",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7E03,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E06",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7E06,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E07",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7E07,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7E10",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7E10,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_B926",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0xB926,   0x0001, 0x00);
// Detector for the set of common boards
REGISTER_HID_DETECTOR_PU("MSI Mystic Light Common",     DetectMSIMysticLightControllers,   MSI_USB_VID_COMMON,  MSI_USB_PID_COMMON, 0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light X870",     DetectMSIMysticLightControllers,   MSI_USB_VID_COMMON,  MSI_USB_PID_COMMON, 0xFF00, 0x01);
/*---------------------------------------------------------------------------------------------------------*\
| Dummy entries for boards using commwn VID and PID                                                         |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("MSI Mystic Light MS_7E12", DetectMSIMysticLightControllers, 0x1462, 0x7E12 )       |
| DUMMY_DEVICE_DETECTOR("MSI Mystic Light MS_7E16", DetectMSIMysticLightControllers, 0x1462, 0x7E16 )       |
| DUMMY_DEVICE_DETECTOR("MSI Mystic Light MS_7E24", DetectMSIMysticLightControllers, 0x1462, 0x7E24 )       |
| DUMMY_DEVICE_DETECTOR("MSI Mystic Light MS_7E26", DetectMSIMysticLightControllers, 0x1462, 0x7E26 )       |
| DUMMY_DEVICE_DETECTOR("MSI Mystic Light MS_7E27", DetectMSIMysticLightControllers, 0x1462, 0x7E27 )       |
\*---------------------------------------------------------------------------------------------------------*/


#ifdef ENABLE_UNTESTED_MYSTIC_LIGHT
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_3EA4",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x3EA4,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_4459",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x4459,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B10",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7B10,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B94",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7B94,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7B96",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7B96,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C42",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C42,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C70",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C70,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C85",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C85,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C88",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C88,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C89",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C89,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C96",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C96,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_7C99",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x7C99,   0x0001, 0x00);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_905D",    DetectMSIMysticLightControllers,   MSI_USB_VID,    0x905D,   0x0001, 0x00);
#endif
