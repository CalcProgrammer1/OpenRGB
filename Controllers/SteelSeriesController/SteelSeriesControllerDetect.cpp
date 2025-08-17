/*---------------------------------------------------------*\
| SteelSeriesControllerDetect.cpp                           |
|                                                           |
|   Detector for SteelSeries devices                        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "SteelSeriesGeneric.h"
#include "SteelSeriesAeroxWirelessController.h"
#include "SteelSeriesAerox5Controller.h"
#include "SteelSeriesArctis5Controller.h"
#include "SteelSeriesApex8ZoneController.h"
#include "SteelSeriesApexController.h"
#include "SteelSeriesApexMController.h"
#include "SteelSeriesApexTZoneController.h"
#include "SteelSeriesOldApexController.h"
#include "SteelSeriesQCKMatController.h"
#include "SteelSeriesRivalController.h"
#include "SteelSeriesRival3Controller.h"
#include "SteelSeriesSenseiController.h"
#include "SteelSeriesSiberiaController.h"
#include "RGBController_SteelSeriesArctis5.h"
#include "RGBController_SteelSeriesApex.h"
#include "RGBController_SteelSeriesApex3.h"
#include "RGBController_SteelSeriesOldApex.h"
#include "RGBController_SteelSeriesQCKMat.h"
#include "RGBController_SteelSeriesRival.h"
#include "RGBController_SteelSeriesRival3.h"
#include "RGBController_SteelSeriesSensei.h"
#include "RGBController_SteelSeriesSiberia.h"

/*-----------------------------------------------------*\
| Vendor ID                                             |
\*-----------------------------------------------------*/
#define STEELSERIES_VID                             0x1038

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define STEELSERIES_AEROX_3_PID                         0x1836
#define STEELSERIES_AEROX_3_WIRELESS_PID                0x1838
#define STEELSERIES_AEROX_3_WIRELESS_WIRED_PID          0x183A
#define STEELSERIES_AEROX_5_WIRELESS_PID                0x1852
#define STEELSERIES_AEROX_5_WIRELESS_WIRED_PID          0x1854
#define STEELSERIES_AEROX_5_DESTINY_WIRELESS_PID        0x185C
#define STEELSERIES_AEROX_5_DESTINY_WIRELESS_WIRED_PID  0x185E
#define STEELSERIES_AEROX_5_DIABLO_WIRELESS_PID         0x1860
#define STEELSERIES_AEROX_5_DIABLO_WIRELESS_WIRED_PID   0x1862
#define STEELSERIES_AEROX_9_WIRELESS_PID                0x1858
#define STEELSERIES_AEROX_9_WIRELESS_WIRED_PID          0x185A
#define STEELSERIES_AEROX_5_PID                         0x1850
#define STEELSERIES_AEROX_9_PID                         0x185A
#define STEELSERIES_RIVAL_100_PID                       0x1702
#define STEELSERIES_RIVAL_100_DOTA_PID                  0x170C
#define STEELSERIES_RIVAL_105_PID                       0x1814
#define STEELSERIES_RIVAL_106_PID                       0x1816
#define STEELSERIES_RIVAL_110_PID                       0x1729
#define STEELSERIES_RIVAL_300_PID                       0x1710
#define ACER_PREDATOR_RIVAL_300_PID                     0x1714
#define STEELSERIES_RIVAL_300_CSGO_PID                  0x1394
#define STEELSERIES_RIVAL_300_CSGO_STM32_PID            0x1716
#define STEELSERIES_RIVAL_300_CSGO_HYPERBEAST_PID       0x171A
#define STEELSERIES_RIVAL_300_DOTA_PID                  0x1392
#define STEELSERIES_RIVAL_300_HP_PID                    0x1718
#define STEELSERIES_RIVAL_300_BLACKOPS_PID              0x1710
#define STEELSERIES_RIVAL_310_PID                       0x1720
#define STEELSERIES_RIVAL_310_CSGO_HOWL_PID             0x171E
#define STEELSERIES_RIVAL_310_PUBG_PID                  0x1736
#define STEELSERIES_RIVAL_600_PID                       0x1724
#define STEELSERIES_RIVAL_600_DOTA_2_PID                0x172E
#define STEELSERIES_RIVAL_650_PID                       0x172B
#define STEELSERIES_RIVAL_650_WIRELESS_PID              0x1726
#define STEELSERIES_RIVAL_700_PID                       0x1700
#define STEELSERIES_RIVAL_710_PID                       0x1730
#define STEELSERIES_RIVAL_3_OLD_PID                     0x1824
#define STEELSERIES_RIVAL_3_PID                         0x184C
#define STEELSERIES_SENSEI_TEN_PID                      0x1832
#define STEELSERIES_SENSEI_TEN_CSGO_NEON_RIDER_PID      0x1834
#define STEELSERIES_SENSEI_310_PID                      0x1722

/*-----------------------------------------------------*\
| Headset product IDs                                   |
\*-----------------------------------------------------*/
#define STEELSERIES_SIBERIA_350_PID                 0x1229
#define STEELSERIES_ARCTIS_5_PID                    0x1250
#define STEELSERIES_ARCTIS_5_V2_PID                 0x12AA

/*------------------------------------------------------------------*\
| Mousemat product IDs                                               |
\*------------------------------------------------------------------*/
#define STEELSERIES_QCK_PRISM_CLOTH_MED_PID                     0x150A
#define STEELSERIES_QCK_PRISM_CLOTH_XL_PID                      0x150D
#define STEELSERIES_QCK_PRISM_CLOTH_XL_DESTINY_PID              0x151E
#define STEELSERIES_QCK_PRISM_CLOTH_XL_CSGO_NEON_RIDER_PID      0x1514
#define STEELSERIES_QCK_PRISM_CLOTH_XL_CSGO_NEO_NOIR_PID        0x151C
#define STEELSERIES_QCK_PRISM_CLOTH_3XL_PID                     0x1516
#define STEELSERIES_QCK_PRISM_CLOTH_4XL_PID                     0x1518
#define STEELSERIES_QCK_PRISM_CLOTH_XL_DESTINY_LIGHTFALL_ED_PID 0X1520

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define STEELSERIES_APEX_3_PID                      0x161A
#define STEELSERIES_APEX_3_TKL_PID                  0x1622
#define STEELSERIES_APEX_5_PID                      0x161C
#define STEELSERIES_APEX_7_PID                      0x1612
#define STEELSERIES_APEX_7_TKL_PID                  0x1618
#define STEELSERIES_APEX_PRO_PID                    0x1610
#define STEELSERIES_APEX_PRO_TKL_PID                0x1614
#define STEELSERIES_APEX_PRO_TKL_2023_PID           0x1628
#define STEELSERIES_APEX_M750_PID                   0x0616
#define STEELSERIES_APEX_OG_PID                     0x1202
#define STEELSERIES_APEX_350_PID                    0x1206
#define STEELSERIES_APEX_PRO3_PID                   0x1640

void DetectSteelSeriesAerox3(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesAerox3Controller*     controller     = new SteelSeriesAerox3Controller(dev, AEROX_3, info->path, name);
        RGBController_SteelSeriesRival3* rgb_controller = new RGBController_SteelSeriesRival3(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesAeroxWireless(hid_device_info* info, const std::string& name, steelseries_type proto_type)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesAeroxWirelessController* controller     = new SteelSeriesAeroxWirelessController(dev, proto_type, info->path, name);
        RGBController_SteelSeriesRival3*    rgb_controller = new RGBController_SteelSeriesRival3(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesAerox3Wireless(hid_device_info* info, const std::string& name)
{
    DetectSteelSeriesAeroxWireless(info, name, AEROX_3_WIRELESS);
}

void DetectSteelSeriesAerox3WirelessWired(hid_device_info* info, const std::string& name)
{
    DetectSteelSeriesAeroxWireless(info, name, AEROX_3_WIRELESS_WIRED);
}

void DetectSteelSeriesAerox5(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesAerox5Controller*     controller     = new SteelSeriesAerox5Controller(dev, AEROX_3, info->path, name);
        RGBController_SteelSeriesRival3* rgb_controller = new RGBController_SteelSeriesRival3(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesAerox5Wireless(hid_device_info* info, const std::string& name)
{
    DetectSteelSeriesAeroxWireless(info, name, AEROX_5_WIRELESS);
}

void DetectSteelSeriesAerox5WirelessWired(hid_device_info* info, const std::string& name)
{
    DetectSteelSeriesAeroxWireless(info, name, AEROX_5_WIRELESS_WIRED);
}

void DetectSteelSeriesAerox5DestinyWireless(hid_device_info* info, const std::string& name)
{
    DetectSteelSeriesAeroxWireless(info, name, AEROX_5_DESTINY_WIRELESS);
}

void DetectSteelSeriesAerox5DestinyWirelessWired(hid_device_info* info, const std::string& name)
{
    DetectSteelSeriesAeroxWireless(info, name, AEROX_5_DESTINY_WIRELESS_WIRED);
}

void DetectSteelSeriesAerox5DiabloWireless(hid_device_info* info, const std::string& name)
{
    DetectSteelSeriesAeroxWireless(info, name, AEROX_5_DIABLO_WIRELESS);
}

void DetectSteelSeriesAerox5DiabloWirelessWired(hid_device_info* info, const std::string& name)
{
    DetectSteelSeriesAeroxWireless(info, name, AEROX_5_DIABLO_WIRELESS_WIRED);
}

void DetectSteelSeriesAerox9Wireless(hid_device_info* info, const std::string& name)
{
    DetectSteelSeriesAeroxWireless(info, name, AEROX_9_WIRELESS);
}

void DetectSteelSeriesAerox9WirelessWired(hid_device_info* info, const std::string& name)
{
    DetectSteelSeriesAeroxWireless(info, name, AEROX_9_WIRELESS_WIRED);
}

void DetectSteelSeriesApex3Full(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesApexTZoneController* controller     = new SteelSeriesApexTZoneController(dev, info->path, name);
        RGBController_SteelSeriesApex3* rgb_controller = new RGBController_SteelSeriesApex3(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesApex3TKL(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesApex8ZoneController* controller     = new SteelSeriesApex8ZoneController(dev, info->path, name);
        RGBController_SteelSeriesApex3* rgb_controller = new RGBController_SteelSeriesApex3(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesApex(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesApexController*     controller     = new SteelSeriesApexController(dev, APEX, info->path, name);
        RGBController_SteelSeriesApex* rgb_controller = new RGBController_SteelSeriesApex(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesApexM(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesApexMController*    controller     = new SteelSeriesApexMController(dev, APEX_M, info->path, name);
        RGBController_SteelSeriesApex* rgb_controller = new RGBController_SteelSeriesApex(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesApexOld(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesOldApexController*     controller     = new SteelSeriesOldApexController(dev, APEX_OLD, info->path, name);
        RGBController_SteelSeriesOldApex* rgb_controller = new RGBController_SteelSeriesOldApex(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesHeadset(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesSiberiaController*     controller     = new SteelSeriesSiberiaController(dev, info->path, name);
        RGBController_SteelSeriesSiberia* rgb_controller = new RGBController_SteelSeriesSiberia(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesMousemat(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesQCKMatController*     controller     = new SteelSeriesQCKMatController(dev, info->path, name);
        RGBController_SteelSeriesQCKMat* rgb_controller = new RGBController_SteelSeriesQCKMat(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesRival100(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesRivalController*     controller     = new SteelSeriesRivalController(dev, RIVAL_100, info->path, name);
        RGBController_SteelSeriesRival* rgb_controller = new RGBController_SteelSeriesRival(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesRival300(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesRivalController*     controller     = new SteelSeriesRivalController(dev, RIVAL_300, info->path, name);
        RGBController_SteelSeriesRival* rgb_controller = new RGBController_SteelSeriesRival(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesRival600(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesRivalController*     controller     = new SteelSeriesRivalController(dev, RIVAL_600, info->path, name);
        RGBController_SteelSeriesRival* rgb_controller = new RGBController_SteelSeriesRival(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}


void DetectSteelSeriesRival650(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesRivalController*     controller     = new SteelSeriesRivalController(dev, RIVAL_650, info->path, name);
        RGBController_SteelSeriesRival* rgb_controller = new RGBController_SteelSeriesRival(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesRival700(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesRivalController*     controller     = new SteelSeriesRivalController(dev, RIVAL_700, info->path, name);
        RGBController_SteelSeriesRival* rgb_controller = new RGBController_SteelSeriesRival(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}


void DetectSteelSeriesRival3(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesRival3Controller*     controller     = new SteelSeriesRival3Controller(dev, RIVAL_3, info->path, name);
        RGBController_SteelSeriesRival3* rgb_controller = new RGBController_SteelSeriesRival3(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesSensei(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesSenseiController*     controller     = new SteelSeriesSenseiController(dev, SENSEI, info->path, name);
        RGBController_SteelSeriesSensei* rgb_controller = new RGBController_SteelSeriesSensei(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesArctis5(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SteelSeriesArctis5Controller*     controller     = new SteelSeriesArctis5Controller(dev, *info, name);
        RGBController_SteelSeriesArctis5* rgb_controller = new RGBController_SteelSeriesArctis5(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\
| Mice                                                                                                                                                                      |
\*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("SteelSeries Aerox 3 Wireless",                          DetectSteelSeriesAerox3Wireless,              STEELSERIES_VID, STEELSERIES_AEROX_3_WIRELESS_PID,                3, 0xFFC0, 1 );
REGISTER_HID_DETECTOR_IPU("SteelSeries Aerox 3 Wireless Wired",                    DetectSteelSeriesAerox3WirelessWired,         STEELSERIES_VID, STEELSERIES_AEROX_3_WIRELESS_WIRED_PID,          3, 0xFFC0, 1 );
REGISTER_HID_DETECTOR_IPU("SteelSeries Aerox 3 Wired",                             DetectSteelSeriesAerox3,                      STEELSERIES_VID, STEELSERIES_AEROX_3_PID,                         3, 0xFFC0, 1 );
REGISTER_HID_DETECTOR_IPU("SteelSeries Aerox 5 Wireless",                          DetectSteelSeriesAerox5Wireless,              STEELSERIES_VID, STEELSERIES_AEROX_5_WIRELESS_PID,                3, 0xFFC0, 1 );
REGISTER_HID_DETECTOR_IPU("SteelSeries Aerox 5 Wireless Wired",                    DetectSteelSeriesAerox5WirelessWired,         STEELSERIES_VID, STEELSERIES_AEROX_5_WIRELESS_WIRED_PID,          3, 0xFFC0, 1 );
REGISTER_HID_DETECTOR_IPU("SteelSeries Aerox 5 Destiny 2 Edition Wireless",        DetectSteelSeriesAerox5DestinyWireless,       STEELSERIES_VID, STEELSERIES_AEROX_5_DESTINY_WIRELESS_PID,        3, 0xFFC0, 1 );
REGISTER_HID_DETECTOR_IPU("SteelSeries Aerox 5 Destiny 2 Edition Wireless Wired",  DetectSteelSeriesAerox5DestinyWirelessWired,  STEELSERIES_VID, STEELSERIES_AEROX_5_DESTINY_WIRELESS_WIRED_PID,  3, 0xFFC0, 1 );
REGISTER_HID_DETECTOR_IPU("SteelSeries Aerox 5 Diablo IV Edition Wireless",        DetectSteelSeriesAerox5DiabloWireless,        STEELSERIES_VID, STEELSERIES_AEROX_5_DIABLO_WIRELESS_PID,         3, 0xFFC0, 1 );
REGISTER_HID_DETECTOR_IPU("SteelSeries Aerox 5 Diablo IV Edition Wireless Wired",  DetectSteelSeriesAerox5DiabloWirelessWired,   STEELSERIES_VID, STEELSERIES_AEROX_5_DIABLO_WIRELESS_WIRED_PID,   3, 0xFFC0, 1 );
REGISTER_HID_DETECTOR_IPU("SteelSeries Aerox 5 Wired",                             DetectSteelSeriesAerox5,                      STEELSERIES_VID, STEELSERIES_AEROX_5_PID,                         3, 0xFFC0, 1 );
REGISTER_HID_DETECTOR_IPU("SteelSeries Aerox 9 Wireless",                          DetectSteelSeriesAerox9Wireless,              STEELSERIES_VID, STEELSERIES_AEROX_9_WIRELESS_PID,                3, 0xFFC0, 1 );
REGISTER_HID_DETECTOR_IPU("SteelSeries Aerox 9 Wireless Wired",                    DetectSteelSeriesAerox9WirelessWired,         STEELSERIES_VID, STEELSERIES_AEROX_9_WIRELESS_WIRED_PID,          3, 0xFFC0, 1 );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 100",                                   DetectSteelSeriesRival100,                    STEELSERIES_VID, STEELSERIES_RIVAL_100_PID,                       0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 100 DotA 2 Edition",                    DetectSteelSeriesRival100,                    STEELSERIES_VID, STEELSERIES_RIVAL_100_DOTA_PID,                  0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 105",                                   DetectSteelSeriesRival100,                    STEELSERIES_VID, STEELSERIES_RIVAL_105_PID,                       0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 106",                                   DetectSteelSeriesRival100,                    STEELSERIES_VID, STEELSERIES_RIVAL_106_PID,                       0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 110",                                   DetectSteelSeriesRival100,                    STEELSERIES_VID, STEELSERIES_RIVAL_110_PID,                       0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300",                                   DetectSteelSeriesRival300,                    STEELSERIES_VID, STEELSERIES_RIVAL_300_PID,                       0);
REGISTER_HID_DETECTOR_I("Acer Predator Gaming Mouse (Rival 300)",                  DetectSteelSeriesRival300,                    STEELSERIES_VID, ACER_PREDATOR_RIVAL_300_PID,                     0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300 CS:GO Fade Edition",                DetectSteelSeriesRival300,                    STEELSERIES_VID, STEELSERIES_RIVAL_300_CSGO_PID,                  0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300 CS:GO Fade Edition (stm32)",        DetectSteelSeriesRival300,                    STEELSERIES_VID, STEELSERIES_RIVAL_300_CSGO_STM32_PID,            0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300 CS:GO Hyperbeast Edition",          DetectSteelSeriesRival300,                    STEELSERIES_VID, STEELSERIES_RIVAL_300_CSGO_HYPERBEAST_PID,       0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300 Dota 2 Edition",                    DetectSteelSeriesRival300,                    STEELSERIES_VID, STEELSERIES_RIVAL_300_DOTA_PID,                  0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300 HP Omen Edition",                   DetectSteelSeriesRival300,                    STEELSERIES_VID, STEELSERIES_RIVAL_300_HP_PID,                    0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300 Black Ops Edition",                 DetectSteelSeriesRival300,                    STEELSERIES_VID, STEELSERIES_RIVAL_300_BLACKOPS_PID,              0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 310",                                   DetectSteelSeriesSensei,                      STEELSERIES_VID, STEELSERIES_RIVAL_310_PID,                       0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 310 CS:GO Howl Edition",                DetectSteelSeriesSensei,                      STEELSERIES_VID, STEELSERIES_RIVAL_310_CSGO_HOWL_PID,             0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 310 PUBG Edition",                      DetectSteelSeriesSensei,                      STEELSERIES_VID, STEELSERIES_RIVAL_310_PUBG_PID,                  0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 600",                                   DetectSteelSeriesRival600,                    STEELSERIES_VID, STEELSERIES_RIVAL_600_PID,                       0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 600 Dota 2 Edition",                    DetectSteelSeriesRival600,                    STEELSERIES_VID, STEELSERIES_RIVAL_600_DOTA_2_PID,                0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 650",                                   DetectSteelSeriesRival650,                    STEELSERIES_VID, STEELSERIES_RIVAL_650_PID,                       0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 650 Wireless",                          DetectSteelSeriesRival650,                    STEELSERIES_VID, STEELSERIES_RIVAL_650_WIRELESS_PID,              0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 700",                                   DetectSteelSeriesRival700,                    STEELSERIES_VID, STEELSERIES_RIVAL_700_PID,                       0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 710",                                   DetectSteelSeriesRival700,                    STEELSERIES_VID, STEELSERIES_RIVAL_710_PID,                       0);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 3 (Old Firmware)",                      DetectSteelSeriesRival3,                      STEELSERIES_VID, STEELSERIES_RIVAL_3_OLD_PID,                     3);
REGISTER_HID_DETECTOR_I("SteelSeries Rival 3",                                     DetectSteelSeriesRival3,                      STEELSERIES_VID, STEELSERIES_RIVAL_3_PID,                         3);
REGISTER_HID_DETECTOR_I("SteelSeries Sensei TEN",                                  DetectSteelSeriesSensei,                      STEELSERIES_VID, STEELSERIES_SENSEI_TEN_PID,                      0);
REGISTER_HID_DETECTOR_I("SteelSeries Sensei TEN CS:GO Neon Rider Edition",         DetectSteelSeriesSensei,                      STEELSERIES_VID, STEELSERIES_SENSEI_TEN_CSGO_NEON_RIDER_PID,      0);
REGISTER_HID_DETECTOR_I("SteelSeries Sensei 310",                                  DetectSteelSeriesSensei,                      STEELSERIES_VID, STEELSERIES_SENSEI_310_PID,                      0);

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\
| Headsets                                                                                                                                                                  |
\*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_I("SteelSeries Siberia 350",                          DetectSteelSeriesHeadset,   STEELSERIES_VID, STEELSERIES_SIBERIA_350_PID,               3  );
REGISTER_HID_DETECTOR_I("SteelSeries Arctis 5",                             DetectSteelSeriesArctis5,   STEELSERIES_VID, STEELSERIES_ARCTIS_5_PID,                  5  );
REGISTER_HID_DETECTOR_I("SteelSeries Arctis 5",                             DetectSteelSeriesArctis5,   STEELSERIES_VID, STEELSERIES_ARCTIS_5_V2_PID,               5  );

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\
| Mousemats                                                                                                                                                                                    |
\*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_I("SteelSeries QCK Prism Cloth Medium",                       DetectSteelSeriesMousemat,  STEELSERIES_VID, STEELSERIES_QCK_PRISM_CLOTH_MED_PID,                      0  );
REGISTER_HID_DETECTOR_I("SteelSeries QCK Prism Cloth XL",                           DetectSteelSeriesMousemat,  STEELSERIES_VID, STEELSERIES_QCK_PRISM_CLOTH_XL_PID,                       0  );
REGISTER_HID_DETECTOR_I("SteelSeries QCK Prism Cloth XL Destiny Ed.",               DetectSteelSeriesMousemat,  STEELSERIES_VID, STEELSERIES_QCK_PRISM_CLOTH_XL_DESTINY_PID,               0  );
REGISTER_HID_DETECTOR_I("SteelSeries QCK Prism Cloth XL Destiny Lightfall Ed.",     DetectSteelSeriesMousemat,  STEELSERIES_VID, STEELSERIES_QCK_PRISM_CLOTH_XL_DESTINY_LIGHTFALL_ED_PID,  0  );
REGISTER_HID_DETECTOR_I("SteelSeries QCK Prism Cloth XL CS:GO Neon Rider Ed.",      DetectSteelSeriesMousemat,  STEELSERIES_VID, STEELSERIES_QCK_PRISM_CLOTH_XL_CSGO_NEON_RIDER_PID,       0  );
REGISTER_HID_DETECTOR_I("SteelSeries QCK Prism Cloth XL CS:GO Neo Noir Ed.",        DetectSteelSeriesMousemat,  STEELSERIES_VID, STEELSERIES_QCK_PRISM_CLOTH_XL_CSGO_NEO_NOIR_PID,         0  );
REGISTER_HID_DETECTOR_I("SteelSeries QCK Prism Cloth 3XL",                          DetectSteelSeriesMousemat,  STEELSERIES_VID, STEELSERIES_QCK_PRISM_CLOTH_3XL_PID,                      0  );
REGISTER_HID_DETECTOR_I("SteelSeries QCK Prism Cloth 4XL",                          DetectSteelSeriesMousemat,  STEELSERIES_VID, STEELSERIES_QCK_PRISM_CLOTH_4XL_PID,                      0  );

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                                                                                                 |
\*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_I  ("SteelSeries Apex 3",                             DetectSteelSeriesApex3Full, STEELSERIES_VID, STEELSERIES_APEX_3_PID,                    	3  );
REGISTER_HID_DETECTOR_IPU("SteelSeries Apex 3 TKL",                         DetectSteelSeriesApex3TKL,  STEELSERIES_VID, STEELSERIES_APEX_3_TKL_PID,    1,  0xFFC0,     1  );
REGISTER_HID_DETECTOR_I  ("SteelSeries Apex 5",                             DetectSteelSeriesApex,      STEELSERIES_VID, STEELSERIES_APEX_5_PID,                    	1  );
REGISTER_HID_DETECTOR_I  ("SteelSeries Apex 7",                             DetectSteelSeriesApex,      STEELSERIES_VID, STEELSERIES_APEX_7_PID,                    	1  );
REGISTER_HID_DETECTOR_I  ("SteelSeries Apex 7 TKL",                         DetectSteelSeriesApex,      STEELSERIES_VID, STEELSERIES_APEX_7_TKL_PID,                	1  );
REGISTER_HID_DETECTOR_I  ("SteelSeries Apex Pro",                           DetectSteelSeriesApex,      STEELSERIES_VID, STEELSERIES_APEX_PRO_PID,                  	1  );
REGISTER_HID_DETECTOR_I  ("SteelSeries Apex Pro TKL",                       DetectSteelSeriesApex,      STEELSERIES_VID, STEELSERIES_APEX_PRO_TKL_PID,              	1  );
REGISTER_HID_DETECTOR_I  ("SteelSeries Apex Pro TKL 2023",                  DetectSteelSeriesApex,      STEELSERIES_VID, STEELSERIES_APEX_PRO_TKL_2023_PID,          	1  );
REGISTER_HID_DETECTOR_I  ("SteelSeries Apex M750",                          DetectSteelSeriesApexM,     STEELSERIES_VID, STEELSERIES_APEX_M750_PID,                 	2  );
REGISTER_HID_DETECTOR_I  ("SteelSeries Apex (OG)/Apex Fnatic",              DetectSteelSeriesApexOld,   STEELSERIES_VID, STEELSERIES_APEX_OG_PID,                   	0  );
REGISTER_HID_DETECTOR_I  ("SteelSeries Apex 350",                           DetectSteelSeriesApexOld,   STEELSERIES_VID, STEELSERIES_APEX_350_PID,                  0  );
REGISTER_HID_DETECTOR_I  ("SteelSeries Apex Pro 3",                         DetectSteelSeriesApex,      STEELSERIES_VID, STEELSERIES_APEX_PRO3_PID,                 1  );
