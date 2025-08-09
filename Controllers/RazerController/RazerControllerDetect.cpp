/*---------------------------------------------------------*\
| RazerControllerDetect.cpp                                 |
|                                                           |
|   Detector for Razer devices                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                22 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <unordered_set>
#include <hidapi.h>
#include "Detector.h"
#include "RazerController.h"
#include "RazerKrakenController.h"
#include "RazerHanboController.h"
#include "RazerDevices.h"
#include "ResourceManager.h"
#include "RGBController_Razer.h"
#include "RGBController_RazerAddressable.h"
#include "RGBController_RazerKraken.h"
#include "RGBController_RazerHanbo.h"

/******************************************************************************************\
*                                                                                          *
*   DetectRazerControllers                                                                 *
*                                                                                          *
*       Tests the USB address to see if a Razer controller exists there.                   *
*                                                                                          *
\******************************************************************************************/

void DetectRazerControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RazerController* controller = new RazerController(dev, dev, info->path, info->product_id, name);

        RGBController_Razer* rgb_controller = new RGBController_Razer(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectRazerControllers() */

/******************************************************************************************\
*                                                                                          *
*   DetectRazerARGBControllers                                                             *
*                                                                                          *
*       Tests the USB address to see if a Razer ARGB controller exists there.              *
*                                                                                          *
\******************************************************************************************/


/*---------------------------------------------------------------------*\
| Tracks the paths used in DetectRazerARGBControllers so multiple Razer |
| devices can be detected without all controlling the same device.      |
\*---------------------------------------------------------------------*/
static std::unordered_set<std::string> used_paths;

/*--------------------------------------------------------------------------------*\
| Removes all entries in used_paths so device discovery does not skip any of them. |
\*--------------------------------------------------------------------------------*/
void ResetRazerARGBControllersPaths()
{
    used_paths.clear();
}

void DetectRazerARGBControllers(hid_device_info* info, const std::string& name)
{
    /*-------------------------------------------------------------------------------------------------*\
    | Razer's ARGB controller uses two different interfaces, one for 90-byte Razer report packets and   |
    | one for 320-byte ARGB packets.  Interface 0 for 90-byte and interface 1 for 320-byte.             |
    |                                                                                                   |
    | Create a local copy of the HID enumerations for the Razer ARGB controller VID/PID and iterate     |
    | through it.  This prevents detection from failing if interface 1 comes before interface 0 in the  |
    | main info list.                                                                                   |
    \*-------------------------------------------------------------------------------------------------*/
     hid_device* dev_interface_0 = nullptr;
     hid_device* dev_interface_1 = nullptr;
     hid_device_info* info_full = hid_enumerate(RAZER_VID, RAZER_CHROMA_ADDRESSABLE_RGB_CONTROLLER_PID);
     hid_device_info* info_temp = info_full;
    /*--------------------------------------------------------------------------------------------*\
    | Keep track of paths so they can be added to used_paths only if both interfaces can be found. |
    \*--------------------------------------------------------------------------------------------*/
     std::string dev_interface_0_path;
     std::string dev_interface_1_path;

     while(info_temp)
     {
        /*----------------------------------------------------------------------------*\
        | Check for paths used on an already registered Razer ARGB controller to avoid |
        | registering multiple controllers that refer to the same physical hardware.   |
        \*----------------------------------------------------------------------------*/
         if(info_temp->vendor_id             == info->vendor_id
         && info_temp->product_id            == info->product_id
         && used_paths.find(info_temp->path) == used_paths.end() )
         {
             if(info_temp->interface_number == 0)
             {
                 dev_interface_0 = hid_open_path(info_temp->path);
                 dev_interface_0_path = info_temp->path;
             }
             else if(info_temp->interface_number == 1)
             {
                 dev_interface_1 = hid_open_path(info_temp->path);
                 dev_interface_1_path = info_temp->path;
             }
         }
         if(dev_interface_0 && dev_interface_1)
         {
             break;
         }
         info_temp = info_temp->next;
     }

     hid_free_enumeration(info_full);

     if(dev_interface_0 && dev_interface_1)
     {
         RazerController* controller                    = new RazerController(dev_interface_0, dev_interface_1, info->path, info->product_id, name);
         RGBController_RazerAddressable* rgb_controller = new RGBController_RazerAddressable(controller);
         ResourceManager::get()->RegisterRGBController(rgb_controller);
         used_paths.insert(dev_interface_0_path);
         used_paths.insert(dev_interface_1_path);
     }
     else
     {
         // Not all of them could be opened, do some cleanup
         hid_close(dev_interface_0);
         hid_close(dev_interface_1);
     }
}

/******************************************************************************************\
*                                                                                          *
*   DetectRazerKrakenController                                                            *
*                                                                                          *
*       Tests the USB address to see if a Razer Kraken controller exists there.            *
*                                                                                          *
\******************************************************************************************/

void DetectRazerKrakenControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RazerKrakenController* controller = new RazerKrakenController(dev, info->path, info->product_id, name);

        RGBController_RazerKraken* rgb_controller = new RGBController_RazerKraken(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectRazerKrakenControllers() */

/******************************************************************************************\
*                                                                                          *
*   DetectRazerHanboController                                                             *
*                                                                                          *
*       Tests the USB address to see if a Razer Hanbo controller exists there.             *
*                                                                                          *
\******************************************************************************************/

void DetectRazerHanboControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RazerHanboController* controller = new RazerHanboController(dev, info->path, info->product_id, name);

        RGBController_RazerHanbo* rgb_controller = new RGBController_RazerHanbo(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectRazerHanboControllers() */

/*-----------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                             |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow 2019",                           DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_2019_PID,                      0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow Chroma",                         DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_CHROMA_PID,                    0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow Chroma Tournament Edition",      DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_CHROMA_TE_PID,                 0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow Chroma V2",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_CHROMA_V2_PID,                 0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow Elite",                          DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_ELITE_PID,                     0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow Overwatch",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_OVERWATCH_PID,                 0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow V3",                             DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_V3_PID,                        0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow V3 Pro (Wired)",                 DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_V3_PRO_WIRED_PID,              0x02,   0x01,   0x02);
// REGISTER_HID_DETECTOR_PU ("Razer Blackwidow V3 Pro (Bluetooth)",             DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_V3_PRO_BLUETOOTH_PID,          0x01,   0x00);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow V3 Pro (Wireless)",              DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_V3_PRO_WIRELESS_PID,           0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow V3 TKL",                         DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_V3_TKL_PID,                    0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow V3 Mini (Wired)",                DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_V3_MINI_WIRED_PID,             0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow V3 Mini (Wireless)",             DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_V3_MINI_WIRELESS_PID,          0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow V4",                             DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_V4_PID,                        0x03,   0x01,   0x00);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow V4 Pro",                         DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_V4_PRO_PID,                    0x03,   0x01,   0x00);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow V4 Pro 75% (Wired)",             DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_V4_PRO_75_WIRED_PID,           0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow V4 X",                           DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_V4_X_PID,                      0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow X Chroma",                       DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_X_CHROMA_PID,                  0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow X Chroma Tournament Edition",    DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_X_CHROMA_TE_PID,               0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Cynosa Chroma",                             DetectRazerControllers,        RAZER_VID,  RAZER_CYNOSA_CHROMA_PID,                        0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Cynosa Chroma V2",                          DetectRazerControllers,        RAZER_VID,  RAZER_CYNOSA_V2_PID,                            0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Cynosa Lite",                               DetectRazerControllers,        RAZER_VID,  RAZER_CYNOSA_LITE_PID,                          0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathstalker Chroma",                       DetectRazerControllers,        RAZER_VID,  RAZER_DEATHSTALKER_CHROMA_PID,                  0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathstalker V2",                           DetectRazerControllers,        RAZER_VID,  RAZER_DEATHSTALKER_V2_PID,                      0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Deathstalker V2 Pro TKL (Wired)",           DetectRazerControllers,        RAZER_VID,  RAZER_DEATHSTALKER_V2_PRO_TKL_WIRED_PID,        0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Deathstalker V2 Pro TKL (Wireless)",        DetectRazerControllers,        RAZER_VID,  RAZER_DEATHSTALKER_V2_PRO_TKL_WIRELESS_PID,     0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathstalker V2 Pro (Wired)",               DetectRazerControllers,        RAZER_VID,  RAZER_DEATHSTALKER_V2_PRO_WIRED_PID,            0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Deathstalker V2 Pro (Wireless)",            DetectRazerControllers,        RAZER_VID,  RAZER_DEATHSTALKER_V2_PRO_WIRELESS_PID,         0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman",                                  DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_PID,                             0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman Elite",                            DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_ELITE_PID,                       0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman Mini",                             DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_MINI_PID,                        0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman Mini Analog",                      DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_MINI_ANALOG_PID,                 0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman Tournament Edition",               DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_TE_PID,                          0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman V2 Analog",                        DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_V2_ANALOG_PID,                   0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman V2 TKL",                           DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_V2_TKL_PID,                      0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman V2",                               DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_V2_PID,                          0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman V3 Pro",                           DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_V3_PRO_PID,                      0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman V3 Pro TKL White",                 DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_V3_PRO_TKL_WHITE_PID,            0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Ornata Chroma",                             DetectRazerControllers,        RAZER_VID,  RAZER_ORNATA_CHROMA_PID,                        0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Ornata Chroma V2",                          DetectRazerControllers,        RAZER_VID,  RAZER_ORNATA_CHROMA_V2_PID,                     0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Ornata V3",                                 DetectRazerControllers,        RAZER_VID,  RAZER_ORNATA_V3_PID,                            0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Ornata V3 Rev2",                            DetectRazerControllers,        RAZER_VID,  RAZER_ORNATA_V3_REV2_PID,                       0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Ornata V3 TKL",                             DetectRazerControllers,        RAZER_VID,  RAZER_ORNATA_V3_TKL_PID,                        0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Ornata V3 X",                               DetectRazerControllers,        RAZER_VID,  RAZER_ORNATA_V3_X_PID,                          0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Ornata V3 X Rev2",                          DetectRazerControllers,        RAZER_VID,  RAZER_ORNATA_V3_X_REV2_PID,                     0x02,   0x01,   0x02);
/*-----------------------------------------------------------------------------------------------------*\
| Laptops                                                                                               |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Blade (2016)",                              DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2016_PID,                           0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade (Late 2016)",                         DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_LATE_2016_PID,                      0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 14 (2021)",                           DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_14_2021_PID,                        0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 14 (2022)",                           DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_14_2022_PID,                        0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 14 (2023)",                           DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_14_2023_PID,                        0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2022)",                           DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_15_2022_PID,                        0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2018 Advanced)",                  DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2018_ADVANCED_PID,                  0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2018 Base)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2018_BASE_PID,                      0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2018 Mercury)",                   DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2018_MERCURY_PID,                   0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2019 Advanced)",                  DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2019_ADVANCED_PID,                  0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2019 Base)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2019_BASE_PID,                      0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2019 Mercury)",                   DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2019_MERCURY_PID,                   0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2019 Studio)",                    DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2019_STUDIO_PID,                    0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2020 Advanced)",                  DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2020_ADVANCED_PID,                  0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2020 Base)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2020_BASE_PID,                      0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (Late 2020)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_LATE_2020_PID,                      0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2021 Advanced)",                  DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2021_ADVANCED_PID,                  0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (Late 2021 Advanced)",             DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_LATE_2021_ADVANCED_PID,             0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2021 Base)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2021_BASE_PID,                      0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2021 Base)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2021_BASE_V2_PID,                   0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Pro (2016)",                          DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_PRO_2016_PID,                       0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Pro (2017)",                          DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_PRO_2017_PID,                       0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Pro (2017 FullHD)",                   DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_PRO_2017_FULLHD_PID,                0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Pro (2019)",                          DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_PRO_2019_PID,                       0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Pro (Late 2019)",                     DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_PRO_LATE_2019_PID,                  0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Pro 17 (2020)",                       DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_PRO_17_2020_PID,                    0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Pro 17 (2021)",                       DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_PRO_17_2021_PID,                    0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (2016)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_2016_PID,                   0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (Late 2016)",                 DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_LATE_2016_PID,              0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (2017)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_2017_PID,                   0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (Late 2017)",                 DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_LATE_2017_PID,              0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (2019)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_2019_PID,                   0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (Late 2019)",                 DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_LATE_2019_PID,              0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (2020)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_2020_PID,                   0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (Late 2020)",                 DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_LATE_2020_PID,              0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Book 13 (2020)",                            DetectRazerControllers,        RAZER_VID,  RAZER_BOOK_13_2020_PID,                         0x02,   0x01,   0x02);

/*-----------------------------------------------------------------------------------------------------*\
| Mice                                                                                                  |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Abyssus Elite D.Va Edition",                DetectRazerControllers,        RAZER_VID,  RAZER_ABYSSUS_ELITE_DVA_EDITION_PID,            0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Abyssus Essential",                         DetectRazerControllers,        RAZER_VID,  RAZER_ABYSSUS_ESSENTIAL_PID,                    0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk",                                  DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_PID,                             0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk Essential",                        DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_ESSENTIAL_PID,                   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk Ultimate (Wired)",                 DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_ULTIMATE_WIRED_PID,              0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk Ultimate (Wireless)",              DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_ULTIMATE_WIRELESS_PID,           0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk V2",                               DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_V2_PID,                          0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk V3",                               DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_V3_PID,                          0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk V3 35K",                           DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_V3_35K_PID,                      0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk V3 Pro (Wired)",                   DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_V3_PRO_WIRED_PID,                0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk V3 Pro (Wireless)",                DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_V3_PRO_WIRELESS_PID,             0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk V3 Pro 35K (Wired)",               DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_V3_PRO_35K_WIRED_PID,            0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk V3 Pro 35K (Wireless)",            DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_V3_PRO_35K_WIRELESS_PID,         0x00,   0x01,   0x02);
// REGISTER_HID_DETECTOR_PU ("Razer Basilisk V3 Pro (Bluetooth)",               DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_V3_PRO_BLUETOOTH_PID,                    0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk V3 X HyperSpeed",                  DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_V3_X_HYPERSPEED_PID,             0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Cobra",                                     DetectRazerControllers,        RAZER_VID,  RAZER_COBRA_PID,                                0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Cobra Pro (Wired)",                         DetectRazerControllers,        RAZER_VID,  RAZER_COBRA_PRO_WIRED_PID,                      0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Cobra Pro (Wireless)",                      DetectRazerControllers,        RAZER_VID,  RAZER_COBRA_PRO_WIRELESS_PID,                   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder Chroma",                         DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_CHROMA_PID,                    0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder Elite",                          DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_ELITE_PID,                     0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder Essential",                      DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_ESSENTIAL_PID,                 0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder Essential V2",                   DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_ESSENTIAL_V2_PID,              0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder Essential White Edition",        DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_ESSENTIAL_WHITE_EDITION_PID,   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder V2",                             DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_V2_PID,                        0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder V2 Mini",                        DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_V2_MINI_PID,                   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder V2 Pro (Wired)",                 DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_V2_PRO_WIRED_PID,              0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder V2 Pro (Wireless)",              DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_V2_PRO_WIRELESS_PID,           0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Diamondback",                               DetectRazerControllers,        RAZER_VID,  RAZER_DIAMONDBACK_CHROMA_PID,                   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Lancehead 2017 (Wired)",                    DetectRazerControllers,        RAZER_VID,  RAZER_LANCEHEAD_2017_WIRED_PID,                 0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Lancehead 2017 (Wireless)",                 DetectRazerControllers,        RAZER_VID,  RAZER_LANCEHEAD_2017_WIRELESS_PID,              0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Lancehead 2019 (Wired)",                    DetectRazerControllers,        RAZER_VID,  RAZER_LANCEHEAD_2019_WIRED_PID,                 0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Lancehead 2019 (Wireless)",                 DetectRazerControllers,        RAZER_VID,  RAZER_LANCEHEAD_2019_WIRELESS_PID,              0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Lancehead Tournament Edition",              DetectRazerControllers,        RAZER_VID,  RAZER_LANCEHEAD_TE_WIRED_PID,                   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Mamba 2012 (Wired)",                        DetectRazerControllers,        RAZER_VID,  RAZER_MAMBA_2012_WIRED_PID,                     0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Mamba 2012 (Wireless)",                     DetectRazerControllers,        RAZER_VID,  RAZER_MAMBA_2012_WIRELESS_PID,                  0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Mamba 2015 (Wired)",                        DetectRazerControllers,        RAZER_VID,  RAZER_MAMBA_2015_WIRED_PID,                     0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Mamba 2015 (Wireless)",                     DetectRazerControllers,        RAZER_VID,  RAZER_MAMBA_2015_WIRELESS_PID,                  0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Mamba 2018 (Wired)",                        DetectRazerControllers,        RAZER_VID,  RAZER_MAMBA_2018_WIRED_PID,                     0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Mamba 2018 (Wireless)",                     DetectRazerControllers,        RAZER_VID,  RAZER_MAMBA_2018_WIRELESS_PID,                  0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Mamba Elite",                               DetectRazerControllers,        RAZER_VID,  RAZER_MAMBA_ELITE_PID,                          0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Mamba Hyperflux (Wired)",                   DetectRazerControllers,        RAZER_VID,  RAZER_MAMBA_HYPERFLUX_PID,                      0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Mamba Tournament Edition",                  DetectRazerControllers,        RAZER_VID,  RAZER_MAMBA_TE_PID,                             0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Chroma",                               DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_CHROMA_PID,                          0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Classic",                              DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_CLASSIC_PID,                         0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Epic Chroma",                          DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_EPIC_CHROMA_PID,                     0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Left Handed",                          DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_LEFT_HANDED_PID,                     0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Hex V2",                               DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_HEX_V2_PID,                          0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Trinity",                              DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_TRINITY_PID,                         0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Pro (Wired)",                          DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_PRO_WIRED_PID,                       0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Pro (Wireless)",                       DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_PRO_WIRELESS_PID,                    0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Pro V2 (Wired)",                       DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_PRO_V2_WIRED_PID,                    0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Pro V2 (Wireless)",                    DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_PRO_V2_WIRELESS_PID,                 0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Viper",                                     DetectRazerControllers,        RAZER_VID,  RAZER_VIPER_PID,                                0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Viper 8kHz",                                DetectRazerControllers,        RAZER_VID,  RAZER_VIPER_8KHZ_PID,                           0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Viper Mini",                                DetectRazerControllers,        RAZER_VID,  RAZER_VIPER_MINI_PID,                           0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Viper Ultimate (Wired)",                    DetectRazerControllers,        RAZER_VID,  RAZER_VIPER_ULTIMATE_WIRED_PID,                 0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Viper Ultimate (Wireless)",                 DetectRazerControllers,        RAZER_VID,  RAZER_VIPER_ULTIMATE_WIRELESS_PID,              0x00,   0x01,   0x02);

/*-----------------------------------------------------------------------------------------------------*\
| Keypads                                                                                               |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Orbweaver Chroma",                          DetectRazerControllers,        RAZER_VID,  RAZER_ORBWEAVER_CHROMA_PID,                     0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Tartarus Chroma",                           DetectRazerControllers,        RAZER_VID,  RAZER_TARTARUS_CHROMA_PID,                      0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Tartarus Pro",                              DetectRazerControllers,        RAZER_VID,  RAZER_TARTARUS_PRO_PID,                         0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Tartarus V2",                               DetectRazerControllers,        RAZER_VID,  RAZER_TARTARUS_V2_PID,                          0x02,   0x01,   0x02);

/*-----------------------------------------------------------------------------------------------------*\
| Headsets                                                                                              |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Kraken 7.1",                                DetectRazerKrakenControllers,  RAZER_VID,  RAZER_KRAKEN_CLASSIC_PID,                       0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Kraken 7.1",                                DetectRazerKrakenControllers,  RAZER_VID,  RAZER_KRAKEN_CLASSIC_ALT_PID,                   0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Kraken 7.1 Chroma",                         DetectRazerKrakenControllers,  RAZER_VID,  RAZER_KRAKEN_PID,                               0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Kraken 7.1 V2",                             DetectRazerKrakenControllers,  RAZER_VID,  RAZER_KRAKEN_V2_PID,                            0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Kraken Kitty Edition",                      DetectRazerControllers,        RAZER_VID,  RAZER_KRAKEN_KITTY_EDITION_PID,                 0x01,   0x01,   0x03);
REGISTER_HID_DETECTOR_IPU("Razer Kraken Kitty Black Edition",                DetectRazerControllers,        RAZER_VID,  RAZER_KRAKEN_KITTY_BLACK_EDITION_PID,           0x01,   0x01,   0x03);
REGISTER_HID_DETECTOR_IPU("Razer Kraken Kitty Black Edition V2",             DetectRazerKrakenControllers,  RAZER_VID,  RAZER_KRAKEN_KITTY_BLACK_EDITION_V2_PID,        0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Kraken Ultimate",                           DetectRazerKrakenControllers,  RAZER_VID,  RAZER_KRAKEN_ULTIMATE_PID,                      0x03,   0x0C,   0x01);
REGISTER_HID_DETECTOR_I(  "Razer Tiamat 7.1 V2",                             DetectRazerControllers,        RAZER_VID,  RAZER_TIAMAT_71_V2_PID,                         0x00                );

/*-----------------------------------------------------------------------------------------------------*\
| Mousemats                                                                                             |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Firefly",                                   DetectRazerControllers,        RAZER_VID,  RAZER_FIREFLY_PID,                              0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Firefly V2",                                DetectRazerControllers,        RAZER_VID,  RAZER_FIREFLY_V2_PID,                           0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Firefly V2 Pro",                            DetectRazerControllers,        RAZER_VID,  RAZER_FIREFLY_V2_PRO_PID,                       0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Firefly Hyperflux",                         DetectRazerControllers,        RAZER_VID,  RAZER_FIREFLY_HYPERFLUX_PID,                    0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Goliathus",                                 DetectRazerControllers,        RAZER_VID,  RAZER_GOLIATHUS_CHROMA_PID,                     0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Goliathus Chroma 3XL",                      DetectRazerControllers,        RAZER_VID,  RAZER_GOLIATHUS_CHROMA_3XL_PID,                 0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Goliathus Extended",                        DetectRazerControllers,        RAZER_VID,  RAZER_GOLIATHUS_CHROMA_EXTENDED_PID,            0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Strider Chroma",                            DetectRazerControllers,        RAZER_VID,  RAZER_STRIDER_CHROMA_PID,                       0x00,   0x01,   0x02);

/*-----------------------------------------------------------------------------------------------------*\
| Accessories                                                                                           |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Base Station Chroma",                       DetectRazerControllers,        RAZER_VID,  RAZER_BASE_STATION_CHROMA_PID,                  0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Base Station V2 Chroma",                    DetectRazerControllers,        RAZER_VID,  RAZER_BASE_STATION_V2_CHROMA_PID,               0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Charging Pad Chroma",                       DetectRazerControllers,        RAZER_VID,  RAZER_CHARGING_PAD_CHROMA_PID,                  0x00,   0x0C,   0x01);
REGISTER_HID_DETECTOR_I("Razer Chroma Addressable RGB Controller",           DetectRazerARGBControllers,    RAZER_VID,  RAZER_CHROMA_ADDRESSABLE_RGB_CONTROLLER_PID,    0x00                );
REGISTER_HID_DETECTOR_IPU("Razer Chroma HDK",                                DetectRazerControllers,        RAZER_VID,  RAZER_CHROMA_HDK_PID,                           0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Chroma Mug Holder",                         DetectRazerControllers,        RAZER_VID,  RAZER_CHROMA_MUG_PID,                           0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Chroma PC Case Lighting Kit",               DetectRazerControllers,        RAZER_VID,  RAZER_CHROMA_PC_CASE_LIGHTING_KIT_PID,          0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Core",                                      DetectRazerControllers,        RAZER_VID,  RAZER_CORE_PID,                                 0x00,   0xFF00, 0x01);
REGISTER_HID_DETECTOR_IPU("Razer Core X",                                    DetectRazerControllers,        RAZER_VID,  RAZER_CORE_X_PID,                               0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Laptop Stand Chroma",                       DetectRazerControllers,        RAZER_VID,  RAZER_LAPTOP_STAND_CHROMA_PID,                  0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Laptop Stand Chroma V2",                    DetectRazerControllers,        RAZER_VID,  RAZER_LAPTOP_STAND_CHROMA_V2_PID,               0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Leviathan V2",                              DetectRazerControllers,        RAZER_VID,  RAZER_LEVIATHAN_V2_PID,                         0x02,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Leviathan V2 X",                            DetectRazerControllers,        RAZER_VID,  RAZER_LEVIATHAN_V2X_PID,                        0x00,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Mouse Bungee V3 Chroma",                    DetectRazerControllers,        RAZER_VID,  RAZER_MOUSE_BUNGEE_V3_CHROMA_PID,               0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Mouse Dock Chroma",                         DetectRazerControllers,        RAZER_VID,  RAZER_MOUSE_DOCK_CHROMA_PID,                    0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Mouse Dock Pro",                            DetectRazerControllers,        RAZER_VID,  RAZER_MOUSE_DOCK_PRO_PID,                       0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Lian Li O11 Dynamic - Razer Edition",             DetectRazerControllers,        RAZER_VID,  RAZER_O11_DYNAMIC_PID,                          0x02,   0x01,   0x02);
REGISTER_HID_DETECTOR_PU("Razer Seiren Emote",                               DetectRazerControllers,        RAZER_VID,  RAZER_SEIREN_EMOTE_PID,                         0x0C,   0x01        );
REGISTER_HID_DETECTOR_PU("Razer Thunderbolt 4 Dock Chroma",                  DetectRazerControllers,        RAZER_VID,  RAZER_THUNDERBOLT_4_DOCK_CHROMA_PID,            0x0C,   0x01        );
REGISTER_HID_DETECTOR_IPU("Razer Hanbo Chroma",                              DetectRazerHanboControllers,   RAZER_VID,  RAZER_HANBO_CHROMA_PID,                         0x00,   0xFF00, 0x01);

/*-----------------------------------------------------------------------------------------------------*\
| Nommo devices seem to have an issue where interface 1 doesn't show on Linux or MacOS.  Due to the way |
| hidapi works on these operating systems, it is acceptable to use interface 0 instead.  Interface 1    |
| must be used on Windows.                                                                              |
\*-----------------------------------------------------------------------------------------------------*/
#ifdef _WIN32
REGISTER_HID_DETECTOR_IPU("Razer Nommo Chroma",                              DetectRazerControllers,        RAZER_VID,  RAZER_NOMMO_CHROMA_PID,                         0x01,   0x01,   0x03);
REGISTER_HID_DETECTOR_IPU("Razer Nommo Pro",                                 DetectRazerControllers,        RAZER_VID,  RAZER_NOMMO_PRO_PID,                            0x01,   0x01,   0x03);
#else
REGISTER_HID_DETECTOR_IPU("Razer Nommo Chroma",                              DetectRazerControllers,        RAZER_VID,  RAZER_NOMMO_CHROMA_PID,                         0x00,   0x01,   0x00);
REGISTER_HID_DETECTOR_IPU("Razer Nommo Pro",                                 DetectRazerControllers,        RAZER_VID,  RAZER_NOMMO_PRO_PID,                            0x00,   0x01,   0x00);
#endif

/*-----------------------------------------------------------------------------------------------------*\
| Need to clean up some stuff before we scan/rescan                                                     |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_PRE_DETECTION_HOOK(ResetRazerARGBControllersPaths);
