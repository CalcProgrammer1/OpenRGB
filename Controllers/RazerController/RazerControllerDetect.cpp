#include "Detector.h"
#include "RazerController.h"
#include "RazerKrakenController.h"
#include "RazerDevices.h"
#include "ResourceManager.h"
#include "RGBController.h"
#include "RGBController_Razer.h"
#include "RGBController_RazerAddressable.h"
#include "RGBController_RazerKraken.h"
#include <hidapi/hidapi.h>

static bool openrazer_checked = false;
static bool openrazer_enabled = false;

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

    /*-------------------------------------------------*\
    | If the OpenRazer/OpenRazer-Win32 controller is    |
    | enabled, don't use this controller.               |
    \*-------------------------------------------------*/
    if(!openrazer_checked)
    {
        /*-------------------------------------------------*\
        | Open device disable list and read in disabled     |
        | device strings                                    |
        \*-------------------------------------------------*/
        json detector_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Detectors");

        /*-------------------------------------------------*\
        | Check for OpenRazer and OpenRazer-Win32 enable    |
        \*-------------------------------------------------*/
        if(detector_settings.contains("detectors"))
        {
            if(detector_settings["detectors"].contains("OpenRazer"))
            {
                if(detector_settings["detectors"]["OpenRazer"] == true)
                {
                    openrazer_enabled = true;
                }
            }
            if(detector_settings["detectors"].contains("OpenRazer-Win32"))
            {
                if(detector_settings["detectors"]["OpenRazer-Win32"] == true)
                {
                    openrazer_enabled = true;
                }
            }
        }

        /*-------------------------------------------------*\
        | Set OpenRazer checked flag to prevent having to do|
        | the settings lookup multiple times                |
        \*-------------------------------------------------*/
        openrazer_checked = true;
    }

    if(openrazer_enabled)
    {
        return;
    }

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

void DetectRazerARGBControllers(hid_device_info* info, const std::string& name)
{
    /*-------------------------------------------------------------------------------------------------*\
    | Razer's ARGB controller uses two different interfaces, one for 90-byte Razer report packets and   |
    | one for 320-byte ARGB packets.  Interface 0 for 90-byte and interface 1 for 320-byte.             |
    \*-------------------------------------------------------------------------------------------------*/
     hid_device* dev_interface_0 = nullptr;
     hid_device* dev_interface_1 = nullptr;
     hid_device_info* info_temp = info;
     while(info_temp)
     {
         if(info_temp->vendor_id        == info->vendor_id        // constant LOGITECH_VID
         && info_temp->product_id       == info->product_id )     // NON-constant
         {
             if(info_temp->interface_number == 0)
             {
                dev_interface_0 = hid_open_path(info_temp->path);
             }
             else if(info_temp->interface_number == 1)
             {
                 dev_interface_1 = hid_open_path(info_temp->path);
             }
         }
         if(dev_interface_0 && dev_interface_1)
         {
             break;
         }
         info_temp = info_temp->next;
     }
     if(dev_interface_0 && dev_interface_1)
     {
         RazerController* controller                    = new RazerController(dev_interface_0, dev_interface_1, info->path, info->product_id, name);
         RGBController_RazerAddressable* rgb_controller = new RGBController_RazerAddressable(controller);
         ResourceManager::get()->RegisterRGBController(rgb_controller);
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

    /*-------------------------------------------------*\
    | If the OpenRazer/OpenRazer-Win32 controller is    |
    | enabled, don't use this controller.               |
    \*-------------------------------------------------*/
    if(!openrazer_checked)
    {
        /*-------------------------------------------------*\
        | Open device disable list and read in disabled     |
        | device strings                                    |
        \*-------------------------------------------------*/
        json detector_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Detectors");

        /*-------------------------------------------------*\
        | Check for OpenRazer and OpenRazer-Win32 enable    |
        \*-------------------------------------------------*/
        if(detector_settings.contains("detectors"))
        {
            if(detector_settings["detectors"].contains("OpenRazer"))
            {
                if(detector_settings["detectors"]["OpenRazer"] == true)
                {
                    openrazer_enabled = true;
                }
            }
            if(detector_settings["detectors"].contains("OpenRazer-Win32"))
            {
                if(detector_settings["detectors"]["OpenRazer-Win32"] == true)
                {
                    openrazer_enabled = true;
                }
            }
        }

        /*-------------------------------------------------*\
        | Set OpenRazer checked flag to prevent having to do|
        | the settings lookup multiple times                |
        \*-------------------------------------------------*/
        openrazer_checked = true;
    }

    if(openrazer_enabled)
    {
        return;
    }

    if(dev)
    {
        RazerKrakenController* controller = new RazerKrakenController(dev, info->path, info->product_id, name);

        RGBController_RazerKraken* rgb_controller = new RGBController_RazerKraken(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectRazerKrakenControllers() */

/*-----------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                             |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow 2019",                           DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_2019_PID,                      0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow Chroma",                         DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_CHROMA_PID,                    0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow Chroma Tournament Edition",      DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_CHROMA_TE_PID,                 0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow Chroma V2",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_CHROMA_V2_PID,                 0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow Elite",                          DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_ELITE_PID,                     0x0,    0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow Overwatch",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_OVERWATCH_PID,                 0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow X Chroma",                       DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_X_CHROMA_PID,                  0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blackwidow X Chroma Tournament Edition",    DetectRazerControllers,        RAZER_VID,  RAZER_BLACKWIDOW_X_CHROMA_TE_PID,               0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Cynosa Chroma",                             DetectRazerControllers,        RAZER_VID,  RAZER_CYNOSA_CHROMA_PID,                        0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Cynosa Chroma V2",                          DetectRazerControllers,        RAZER_VID,  RAZER_CYNOSA_V2_PID,                            0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Cynosa Lite",                               DetectRazerControllers,        RAZER_VID,  RAZER_CYNOSA_LITE_PID,                          0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathstalker Chroma",                       DetectRazerControllers,        RAZER_VID,  RAZER_DEATHSTALKER_CHROMA_PID,                  0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman",                                  DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_PID,                             0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman Elite",                            DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_ELITE_PID,                       0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Huntsman Tournament Edition",               DetectRazerControllers,        RAZER_VID,  RAZER_HUNTSMAN_TE_PID,                          0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Ornata Chroma",                             DetectRazerControllers,        RAZER_VID,  RAZER_ORNATA_CHROMA_PID,                        0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Ornata Chroma V2",                          DetectRazerControllers,        RAZER_VID,  RAZER_ORNATA_CHROMA_V2_PID,                     0x00,   0x01,   0x02);

/*-----------------------------------------------------------------------------------------------------*\
| Laptops                                                                                               |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Blade (2016)",                              DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2016_PID,                           0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade (Late 2016)",                         DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_LATE_2016_PID,                      0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2018 Advanced)",                  DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2018_ADVANCED_PID,                  0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2018 Base)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2018_BASE_PID,                      0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2018 Mercury)",                   DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2018_MERCURY_PID,                   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2019 Advanced)",                  DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2019_ADVANCED_PID,                  0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2019 Base)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2019_BASE_PID,                      0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2019 Mercury)",                   DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2019_MERCURY_PID,                   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2019 Studio)",                    DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2019_STUDIO_PID,                    0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2020 Advanced)",                  DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2020_ADVANCED_PID,                  0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2020 Base)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2020_BASE_PID,                      0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade 15 (2021 Base)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_2021_BASE_PID,                      0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Pro (2016)",                          DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_PRO_2016_PID,                       0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Pro (2017)",                          DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_PRO_2017_PID,                       0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Pro (2017 FullHD)",                   DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_PRO_2017_FULLHD_PID,                0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Pro (2019)",                          DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_PRO_2019_PID,                       0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Pro (Late 2019)",                     DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_PRO_LATE_2019_PID,                  0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (2016)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_2016_PID,                   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (Late 2016)",                 DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_LATE_2016_PID,              0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (2017)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_2017_PID,                   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (Late 2017)",                 DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_LATE_2017_PID,              0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (2019)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_2019_PID,                   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (Late 2019)",                 DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_LATE_2019_PID,              0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (2020)",                      DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_2020_PID,                   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Blade Stealth (Late 2020)",                 DetectRazerControllers,        RAZER_VID,  RAZER_BLADE_STEALTH_LATE_2020_PID,              0x00,   0x01,   0x02);

/*-----------------------------------------------------------------------------------------------------*\
| Mice                                                                                                  |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Abyssus Elite D.Va Edition",                DetectRazerControllers,        RAZER_VID,  RAZER_ABYSSUS_ELITE_DVA_EDITION_PID,            0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Abyssus Essential",                         DetectRazerControllers,        RAZER_VID,  RAZER_ABYSSUS_ESSENTIAL_PID,                    0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk",                                  DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_PID,                             0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Basilisk Essential",                        DetectRazerControllers,        RAZER_VID,  RAZER_BASILISK_ESSENTIAL_PID,                   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder Chroma",                         DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_CHROMA_PID,                    0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder Elite",                          DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_ELITE_PID,                     0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder Essential",                      DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_ESSENTIAL_PID,                 0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder Essential White Edition",        DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_ESSENTIAL_WHITE_EDITION_PID,   0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Deathadder V2",                             DetectRazerControllers,        RAZER_VID,  RAZER_DEATHADDER_V2_PID,                        0x00,   0x01,   0x02);
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
REGISTER_HID_DETECTOR_IPU("Razer Mamba Tournament Edition",                  DetectRazerControllers,        RAZER_VID,  RAZER_MAMBA_TE_PID,                             0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Chroma",                               DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_CHROMA_PID,                          0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Epic Chroma",                          DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_EPIC_CHROMA_PID,                     0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Hex V2",                               DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_HEX_V2_PID,                          0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Naga Trinity",                              DetectRazerControllers,        RAZER_VID,  RAZER_NAGA_TRINITY_PID,                         0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Viper",                                     DetectRazerControllers,        RAZER_VID,  RAZER_VIPER_PID,                                0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Viper Mini",                                DetectRazerControllers,        RAZER_VID,  RAZER_VIPER_MINI_PID,                           0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Viper Ultimate (Wired)",                    DetectRazerControllers,        RAZER_VID,  RAZER_VIPER_ULTIMATE_WIRED_PID,                 0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Viper Ultimate (Wireless)",                 DetectRazerControllers,        RAZER_VID,  RAZER_VIPER_ULTIMATE_WIRELESS_PID,              0x00,   0x01,   0x02);

/*-----------------------------------------------------------------------------------------------------*\
| Keypads                                                                                               |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Orbweaver Chroma",                          DetectRazerControllers,        RAZER_VID,  RAZER_ORBWEAVER_CHROMA_PID,                     0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Tartarus Chroma",                           DetectRazerControllers,        RAZER_VID,  RAZER_TARTARUS_CHROMA_PID,                      0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Tartarus V2",                               DetectRazerControllers,        RAZER_VID,  RAZER_TARTARUS_V2_PID,                          0x00,   0x01,   0x02);

/*-----------------------------------------------------------------------------------------------------*\
| Headsets                                                                                              |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Kraken 7.1",                                DetectRazerKrakenControllers,  RAZER_VID,  RAZER_KRAKEN_CLASSIC_PID,                       0x00,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Kraken 7.1",                                DetectRazerKrakenControllers,  RAZER_VID,  RAZER_KRAKEN_CLASSIC_ALT_PID,                   0x00,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Kraken 7.1 Chroma",                         DetectRazerKrakenControllers,  RAZER_VID,  RAZER_KRAKEN_PID,                               0x00,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Kraken 7.1 V2",                             DetectRazerKrakenControllers,  RAZER_VID,  RAZER_KRAKEN_V2_PID,                            0x00,   0x0C,   0x01);
REGISTER_HID_DETECTOR_IPU("Razer Kraken Kitty Edition",                      DetectRazerControllers,        RAZER_VID,  RAZER_KRAKEN_KITTY_EDITION_PID,                 0x00,   0x01,   0x03);
REGISTER_HID_DETECTOR_IPU("Razer Kraken Ultimate",                           DetectRazerKrakenControllers,  RAZER_VID,  RAZER_KRAKEN_ULTIMATE_PID,                      0x00,   0x0C,   0x01);
REGISTER_HID_DETECTOR_I(  "Razer Tiamat 7.1 V2",                             DetectRazerControllers,        RAZER_VID,  RAZER_TIAMAT_71_V2_PID,                         0x00                );

/*-----------------------------------------------------------------------------------------------------*\
| Mousemats                                                                                             |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Firefly",                                   DetectRazerControllers,        RAZER_VID,  RAZER_FIREFLY_PID,                              0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Firefly V2",                                DetectRazerControllers,        RAZER_VID,  RAZER_FIREFLY_V2_PID,                           0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Firefly Hyperflux",                         DetectRazerControllers,        RAZER_VID,  RAZER_FIREFLY_HYPERFLUX_PID,                    0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Goliathus",                                 DetectRazerControllers,        RAZER_VID,  RAZER_GOLIATHUS_CHROMA_PID,                     0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Goliathus Extended",                        DetectRazerControllers,        RAZER_VID,  RAZER_GOLIATHUS_CHROMA_EXTENDED_PID,            0x00,   0x01,   0x02);

/*-----------------------------------------------------------------------------------------------------*\
| Accessories                                                                                           |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Razer Base Station Chroma",                       DetectRazerControllers,        RAZER_VID,  RAZER_BASE_STATION_CHROMA_PID,                  0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Base Station V2 Chroma",                    DetectRazerControllers,        RAZER_VID,  RAZER_BASE_STATION_V2_CHROMA_PID,               0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Charging Pad Chroma",                       DetectRazerControllers,        RAZER_VID,  RAZER_CHARGING_PAD_CHROMA_PID,                  0x00,   0x0C,   0x01);
REGISTER_HID_DETECTOR_I  ("Razer Chroma Addressable RGB Controller",         DetectRazerARGBControllers,    RAZER_VID,  RAZER_CHROMA_ADDRESSABLE_RGB_CONTROLLER_PID,    0x00                );
REGISTER_HID_DETECTOR_IPU("Razer Chroma HDK",                                DetectRazerControllers,        RAZER_VID,  RAZER_CHROMA_HDK_PID,                           0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Chroma Mug Holder",                         DetectRazerControllers,        RAZER_VID,  RAZER_CHROMA_MUG_PID,                           0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Chroma PC Case Lighting Kit",               DetectRazerControllers,        RAZER_VID,  RAZER_CHROMA_PC_CASE_LIGHTING_KIT_PID,          0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Core",                                      DetectRazerControllers,        RAZER_VID,  RAZER_CORE_PID,                                 0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Mouse Bungee V3 Chroma",                    DetectRazerControllers,        RAZER_VID,  RAZER_MOUSE_BUNGEE_V3_CHROMA_PID,               0x00,   0x01,   0x02);
REGISTER_HID_DETECTOR_IPU("Razer Nommo Chroma",                              DetectRazerControllers,        RAZER_VID,  RAZER_NOMMO_CHROMA_PID,                         0x00,   0x01,   0x03);
REGISTER_HID_DETECTOR_IPU("Razer Nommo Pro",                                 DetectRazerControllers,        RAZER_VID,  RAZER_NOMMO_PRO_PID,                            0x00,   0x01,   0x03);
