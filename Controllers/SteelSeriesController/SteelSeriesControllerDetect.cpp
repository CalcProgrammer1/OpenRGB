#include "Detector.h"
#include "SteelSeriesRivalController.h"
#include "SteelSeriesSiberiaController.h"
#include "SteelSeriesQCKMatController.h"
#include "SteelSeriesApexController.h"
#include "SteelSeriesOldApexController.h"
#include "SteelSeriesApexMController.h"
#include "SteelSeriesSenseiController.h"
#include "SteelSeriesGeneric.h"
#include "RGBController.h"
#include "RGBController_SteelSeriesRival.h"
#include "RGBController_SteelSeriesSiberia.h"
#include "RGBController_SteelSeriesQCKMat.h"
#include "RGBController_SteelSeriesApex.h"
#include "RGBController_SteelSeriesOldApex.h"
#include "RGBController_SteelSeriesSensei.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Vendor ID                                             |
\*-----------------------------------------------------*/
#define STEELSERIES_VID                             0x1038
/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define STEELSERIES_RIVAL_100_PID                   0x1702
#define STEELSERIES_RIVAL_100_DOTA_PID              0x170c
#define STEELSERIES_RIVAL_105_PID                   0x1814
#define STEELSERIES_RIVAL_110_PID                   0x1729
#define STEELSERIES_RIVAL_300_PID                   0x1710
#define ACER_PREDATOR_RIVAL_300_PID                 0x1714
#define STEELSERIES_RIVAL_300_CSGO_PID              0x1394
#define STEELSERIES_RIVAL_300_CSGO_STM32_PID        0x1716
#define STEELSERIES_RIVAL_300_CSGO_HYPERBEAST_PID   0x171a
#define STEELSERIES_RIVAL_300_DOTA_PID              0x1392
#define STEELSERIES_RIVAL_300_HP_PID                0x1718
#define STEELSERIES_RIVAL_300_BLACKOPS_PID          0x1710
#define STEELSERIES_RIVAL_310_PID                   0x1720
#define STEELSERIES_RIVAL_310_CSGO_HOWL_PID         0x171e
#define STEELSERIES_RIVAL_310_PUBG_PID              0x1736
#define STEELSERIES_RIVAL_650_PID                   0x172B
#define STEELSERIES_RIVAL_650_WIRELESS_PID          0x1726
#define STEELSERIES_SENSEI_TEN_PID                  0x1832
#define STEELSERIES_SENSEI_TEN_CSGO_NEON_RIDER_PID  0x1834
#define STEELSERIES_SENSEI_310_PID                  0x1722
/*-----------------------------------------------------*\
| Headset product IDs                                   |
\*-----------------------------------------------------*/
#define STEELSERIES_SIBERIA_350_PID                 0x1229
/*-----------------------------------------------------*\
| Mousemat product IDs                                  |
\*-----------------------------------------------------*/
#define STEELSERIES_QCK_PRISM_CLOTH                 0x150D
/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define STEELSERIES_APEX_5_PID                      0x161C
#define STEELSERIES_APEX_7_PID                      0x1612
#define STEELSERIES_APEX_7_TKL_PID                  0x1618
#define STEELSERIES_APEX_PRO_PID                    0x1610
#define STEELSERIES_APEX_PRO_TKL_PID                0x1614
#define STEELSERIES_APEX_M750_PID                   0x0616
#define STEELSERIES_APEX_OG_PID                     0x1202
#define STEELSERIES_APEX_350_PID                    0x1206

void DetectSteelSeriesApex(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SteelSeriesApexController* controller = new SteelSeriesApexController(dev, APEX, info->path);
        RGBController_SteelSeriesApex* rgb_controller = new RGBController_SteelSeriesApex(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesApexTKL(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SteelSeriesApexController* controller = new SteelSeriesApexController(dev, APEX_TKL, info->path);
        RGBController_SteelSeriesApex* rgb_controller = new RGBController_SteelSeriesApex(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesApexM(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SteelSeriesApexMController* controller = new SteelSeriesApexMController(dev, APEX_M, info->path);
        RGBController_SteelSeriesApex* rgb_controller = new RGBController_SteelSeriesApex(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesApexOld(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SteelSeriesOldApexController* controller = new SteelSeriesOldApexController(dev, APEX_M, info->path);
        RGBController_SteelSeriesOldApex* rgb_controller = new RGBController_SteelSeriesOldApex(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesHeadset(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SteelSeriesSiberiaController* controller = new SteelSeriesSiberiaController(dev, info->path);
        RGBController_SteelSeriesSiberia* rgb_controller = new RGBController_SteelSeriesSiberia(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesMousemat(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SteelSeriesQCKMatController* controller = new SteelSeriesQCKMatController(dev, info->path);
        RGBController_SteelSeriesQCKMat* rgb_controller = new RGBController_SteelSeriesQCKMat(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesRival100(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SteelSeriesRivalController* controller = new SteelSeriesRivalController(dev, RIVAL_100, info->path);
        RGBController_SteelSeriesRival* rgb_controller = new RGBController_SteelSeriesRival(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesRival300(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SteelSeriesRivalController* controller = new SteelSeriesRivalController(dev, RIVAL_300, info->path);
        RGBController_SteelSeriesRival* rgb_controller = new RGBController_SteelSeriesRival(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}
void DetectSteelSeriesRival650(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SteelSeriesRivalController* controller = new SteelSeriesRivalController(dev, RIVAL_650, info->path);
        RGBController_SteelSeriesRival* rgb_controller = new RGBController_SteelSeriesRival(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSteelSeriesSensei(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SteelSeriesSenseiController* controller = new SteelSeriesSenseiController(dev, SENSEI, info->path);
        RGBController_SteelSeriesSensei* rgb_controller = new RGBController_SteelSeriesSensei(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*\
| Mice                                                                                                                                                                  |
\*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_I("SteelSeries Rival 100",                            DetectSteelSeriesRival100,  STEELSERIES_VID, STEELSERIES_RIVAL_100_PID,                 0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 100 DotA 2 Edition",             DetectSteelSeriesRival100,  STEELSERIES_VID, STEELSERIES_RIVAL_100_DOTA_PID,            0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 105",                            DetectSteelSeriesRival100,  STEELSERIES_VID, STEELSERIES_RIVAL_105_PID,                 0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 110",                            DetectSteelSeriesRival100,  STEELSERIES_VID, STEELSERIES_RIVAL_110_PID,                 0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300",                            DetectSteelSeriesRival300,  STEELSERIES_VID, STEELSERIES_RIVAL_300_PID,                 0  );
REGISTER_HID_DETECTOR_I("Acer Predator Gaming Mouse (Rival 300)",           DetectSteelSeriesRival300,  STEELSERIES_VID, ACER_PREDATOR_RIVAL_300_PID,               0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300 CS:GO Fade Edition",         DetectSteelSeriesRival300,  STEELSERIES_VID, STEELSERIES_RIVAL_300_CSGO_PID,            0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300 CS:GO Fade Edition (stm32)", DetectSteelSeriesRival300,  STEELSERIES_VID, STEELSERIES_RIVAL_300_CSGO_STM32_PID,      0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300 CS:GO Hyperbeast Edition",   DetectSteelSeriesRival300,  STEELSERIES_VID, STEELSERIES_RIVAL_300_CSGO_HYPERBEAST_PID, 0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300 Dota 2 Edition",             DetectSteelSeriesRival300,  STEELSERIES_VID, STEELSERIES_RIVAL_300_DOTA_PID,            0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300 HP Omen Edition",            DetectSteelSeriesRival300,  STEELSERIES_VID, STEELSERIES_RIVAL_300_HP_PID,              0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 300 Black Ops Edition",          DetectSteelSeriesRival300,  STEELSERIES_VID, STEELSERIES_RIVAL_300_BLACKOPS_PID,        0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 310",                            DetectSteelSeriesSensei,    STEELSERIES_VID, STEELSERIES_RIVAL_310_PID,                 0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 310 CS:GO Howl Edition",         DetectSteelSeriesSensei,    STEELSERIES_VID, STEELSERIES_RIVAL_310_CSGO_HOWL_PID,       0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 310 PUBG Edition",               DetectSteelSeriesSensei,    STEELSERIES_VID, STEELSERIES_RIVAL_310_PUBG_PID,            0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 650",                            DetectSteelSeriesRival650,  STEELSERIES_VID, STEELSERIES_RIVAL_650_PID,                 0  );
REGISTER_HID_DETECTOR_I("SteelSeries Rival 650 Wireless",                   DetectSteelSeriesRival650,  STEELSERIES_VID, STEELSERIES_RIVAL_650_WIRELESS_PID,        0  );
REGISTER_HID_DETECTOR_I("SteelSeries Sensei TEN",                           DetectSteelSeriesSensei,    STEELSERIES_VID, STEELSERIES_SENSEI_TEN_PID,                0  );
REGISTER_HID_DETECTOR_I("SteelSeries Sensei TEN CS:GO Neon Rider Edition",  DetectSteelSeriesSensei,    STEELSERIES_VID, STEELSERIES_SENSEI_TEN_CSGO_NEON_RIDER_PID,0  );
REGISTER_HID_DETECTOR_I("SteelSeries Sensei 310",                           DetectSteelSeriesSensei,    STEELSERIES_VID, STEELSERIES_SENSEI_310_PID,                0  );
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*\
| Headsets                                                                                                                                                              |
\*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_I("SteelSeries Siberia 350",                          DetectSteelSeriesHeadset,   STEELSERIES_VID, STEELSERIES_SIBERIA_350_PID,               3  );
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*\
| Mousemats                                                                                                                                                             |
\*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_I("SteelSeries QCK Prism Cloth",                      DetectSteelSeriesMousemat,  STEELSERIES_VID, STEELSERIES_QCK_PRISM_CLOTH,               0  );
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                                                                                             |
\*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_I("SteelSeries Apex 5",                               DetectSteelSeriesApex,      STEELSERIES_VID, STEELSERIES_APEX_5_PID,                    1  );
REGISTER_HID_DETECTOR_I("SteelSeries Apex 7",                               DetectSteelSeriesApex,      STEELSERIES_VID, STEELSERIES_APEX_7_PID,                    1  );
REGISTER_HID_DETECTOR_I("SteelSeries Apex 7 TKL",                           DetectSteelSeriesApexTKL,   STEELSERIES_VID, STEELSERIES_APEX_7_TKL_PID,                1  );
REGISTER_HID_DETECTOR_I("SteelSeries Apex Pro",                             DetectSteelSeriesApex,      STEELSERIES_VID, STEELSERIES_APEX_PRO_PID,                  1  );
REGISTER_HID_DETECTOR_I("SteelSeries Apex Pro TKL",                         DetectSteelSeriesApexTKL,   STEELSERIES_VID, STEELSERIES_APEX_PRO_TKL_PID,              1  );
REGISTER_HID_DETECTOR_I("SteelSeries Apex M750",                            DetectSteelSeriesApexM,     STEELSERIES_VID, STEELSERIES_APEX_M750_PID,                 2  );
REGISTER_HID_DETECTOR_I("SteelSeries Apex (OG)/Apex Fnatic",                DetectSteelSeriesApexOld,   STEELSERIES_VID, STEELSERIES_APEX_OG_PID,                   0  );
REGISTER_HID_DETECTOR_I("SteelSeries Apex 350",                             DetectSteelSeriesApexOld,   STEELSERIES_VID, STEELSERIES_APEX_350_PID,                  0  );
