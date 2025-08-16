/*---------------------------------------------------------*\
| AsusAuraUSBControllerDetect.cpp                           |
|                                                           |
|   Detector for ASUS Aura USB devices                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <stdexcept>
#include <hidapi.h>
#include "Detector.h"
#include "AsusAuraAddressableController.h"
#include "AsusAuraHeadsetStandController.h"
#include "AsusAuraKeyboardController.h"
#include "AsusAuraTUFKeyboardController.h"
#include "AsusAuraMainboardController.h"
#include "AsusAuraMouseController.h"
#include "AsusROGAllyController.h"
#include "AsusROGStrixLCController.h"
#include "AsusAuraMouseGen1Controller.h"
#include "AsusAuraMousematController.h"
#include "AsusAuraMonitorController.h"
#include "AsusAuraRyuoAIOController.h"
#include "RGBController_AsusAuraUSB.h"
#include "RGBController_AsusAuraHeadsetStand.h"
#include "RGBController_AsusAuraKeyboard.h"
#include "RGBController_AsusAuraTUFKeyboard.h"
#include "RGBController_AsusAuraMainboard.h"
#include "RGBController_AsusAuraMouse.h"
#include "RGBController_AsusAuraMousemat.h"
#include "RGBController_AsusROGAlly.h"
#include "RGBController_AsusROGStrixLC.h"
#include "RGBController_AsusROGSpatha.h"
#include "RGBController_AsusROGStrixEvolve.h"
#include "RGBController_AsusAuraMonitor.h"
#include "RGBController_AsusAuraRyuoAIO.h"
#include "dmiinfo.h"

#define AURA_USB_VID                                        0x0B05

/*-----------------------------------------------------------------*\
|  MOTHERBOARDS                                                     |
\*-----------------------------------------------------------------*/
#define AURA_ADDRESSABLE_1_PID                              0x1867
#define AURA_ADDRESSABLE_2_PID                              0x1872
#define AURA_ADDRESSABLE_3_PID                              0x18A3
#define AURA_ADDRESSABLE_4_PID                              0x18A5
#define AURA_MOTHERBOARD_1_PID                              0x18F3
#define AURA_MOTHERBOARD_2_PID                              0x1939
#define AURA_MOTHERBOARD_3_PID                              0x19AF
#define AURA_MOTHERBOARD_4_PID                              0x1AA6

/*-----------------------------------------------------------------*\
|  KEYBOARDS                                                        |
\*-----------------------------------------------------------------*/
#define AURA_ROG_AZOTH_USB_PID                              0x1A83
#define AURA_ROG_AZOTH_2_4_PID                              0x1A85
#define AURA_ROG_CLAYMORE_PID                               0x184D
#define AURA_ROG_FALCHION_WIRED_PID                         0x193C
#define AURA_ROG_FALCHION_WIRELESS_PID                      0x193E
#define AURA_ROG_STRIX_FLARE_PID                            0x1875
#define AURA_ROG_STRIX_FLARE_PNK_LTD_PID                    0x18CF
#define AURA_ROG_STRIX_FLARE_COD_BO4_PID                    0x18AF
#define AURA_ROG_STRIX_FLARE_II_ANIMATE_PID                 0x19FC
#define AURA_ROG_STRIX_FLARE_II_PID                         0x19FE
#define AURA_ROG_STRIX_SCOPE_PID                            0x18F8
#define AURA_ROG_STRIX_SCOPE_TKL_PID                        0x190C
#define AURA_ROG_STRIX_SCOPE_TKL_PNK_LTD_PID                0x1954
#define AURA_ROG_STRIX_SCOPE_RX_PID                         0x1951
#define AURA_ROG_STRIX_SCOPE_RX_EVA_02_PID                  0x1B12
#define AURA_ROG_STRIX_SCOPE_RX_TKL_DELUXE_PID              0x1A05
#define AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_USB_PID     0x19F6
#define AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_2_4_PID     0x19F8
#define AURA_ROG_STRIX_SCOPE_II_PID                         0x1AB3
#define AURA_ROG_STRIX_SCOPE_II_RX_PID                      0x1AB5
#define AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID         0x1AAE
#define AURA_TUF_K1_GAMING_PID                              0x1945
#define AURA_TUF_K3_GAMING_PID                              0x194B
#define AURA_TUF_K5_GAMING_PID                              0x1899
#define AURA_TUF_K7_GAMING_PID                              0x18AA

/*-----------------------------------------------------------------*\
|  MICE - defined in AsusAuraMouseDevices.h                         |
\*-----------------------------------------------------------------*/
#define AURA_ROG_STRIX_EVOLVE_PID                           0x185B
#define AURA_ROG_SPATHA_WIRED_PID                           0x181C
#define AURA_ROG_SPATHA_WIRELESS_PID                        0x1824

/*-----------------------------------------------------------------*\
| MOUSEMATS                                                         |
\*-----------------------------------------------------------------*/
#define AURA_ROG_BALTEUS_PID                                0x1891
#define AURA_ROG_BALTEUS_QI_PID                             0x1890

/*-----------------------------------------------------------------*\
| MONITORS                                                          |
\*-----------------------------------------------------------------*/
#define AURA_ROG_STRIX_XG27AQ_PID                           0x198C
#define AURA_ROG_STRIX_XG27AQM_PID                          0x19BB
#define AURA_ROG_STRIX_XG279Q_PID                           0x1919
#define AURA_ROG_STRIX_XG27W_PID                            0x1933
#define AURA_ROG_STRIX_XG32VC_PID                           0x1968
#define AURA_ROG_PG32UQ_PID                                 0x19B9

/*-----------------------------------------------------------------*\
| HEADSET STANDS                                                    |
\*-----------------------------------------------------------------*/
#define AURA_ROG_THRONE_PID                                 0x18D9
#define AURA_ROG_THRONE_QI_PID                              0x18C5
#define AURA_ROG_THRONE_QI_GUNDAM_PID                       0x1994

/*-----------------------------------------------------------------*\
|  OTHER                                                            |
\*-----------------------------------------------------------------*/
#define AURA_TERMINAL_PID                                   0x1889
#define ROG_STRIX_LC120_PID                                 0x879E
#define AURA_RYUO_AIO_PID                                   0x1887
#define ASUS_ROG_ALLY_PID                                   0x1ABE
#define ASUS_ROG_ALLY_X_PID                                 0x1B4C

AuraKeyboardMappingLayoutType GetKeyboardMappingLayoutType(int pid)
{
    switch(pid)
    {
        case AURA_ROG_STRIX_SCOPE_PID:
            return SCOPE_LAYOUT;

        case AURA_ROG_STRIX_SCOPE_RX_PID:
            return SCOPE_RX_LAYOUT;

        case AURA_ROG_STRIX_SCOPE_TKL_PID:
        case AURA_ROG_STRIX_SCOPE_RX_TKL_DELUXE_PID:
        case AURA_ROG_STRIX_SCOPE_TKL_PNK_LTD_PID:
            return SCOPE_TKL_LAYOUT;

        default:
            return FLARE_LAYOUT;
    }
}

void DetectAsusAuraUSBTerminal(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AuraAddressableController* controller               = new AuraAddressableController(dev, info->path, name);
        RGBController_AuraUSB*     rgb_controller           = new RGBController_AuraUSB(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBAddressable(hid_device_info* info, const std::string& /*name*/)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        DMIInfo                    dmi;
        AuraAddressableController* controller               = new AuraAddressableController(dev, info->path, "ASUS " + dmi.getMainboard() + " Addressable");
        RGBController_AuraUSB*     rgb_controller           = new RGBController_AuraUSB(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBMotherboards(hid_device_info* info, const std::string& /*name*/)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        try
        {
            DMIInfo                      dmi;
            AuraMainboardController*     controller         = new AuraMainboardController(dev, info->path, "ASUS " + dmi.getMainboard());
            RGBController_AuraMainboard* rgb_controller     = new RGBController_AuraMainboard(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        catch(const std::runtime_error& ex)
        {
            // reading the config table failed
            LOG_ERROR("[AsusAuraUSB] An error occured while reading the config table: %s", ex.what());
        }
    }
}

void DetectAsusAuraUSBKeyboards(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AuraKeyboardController*       controller            = new AuraKeyboardController(dev, info->path, name);
        AuraKeyboardMappingLayoutType layout                = GetKeyboardMappingLayoutType(info->product_id);
        RGBController_AuraKeyboard*   rgb_controller        = new RGBController_AuraKeyboard(controller, layout);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBMice(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        uint16_t pid = (name == "Asus ROG Spatha X Dock") ? AURA_ROG_SPATHA_X_DOCK_FAKE_PID : info->product_id;
        AuraMouseController*     controller                 = new AuraMouseController(dev, info->path, pid, name);
        RGBController_AuraMouse* rgb_controller             = new RGBController_AuraMouse(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBMousemats(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AuraMousematController*     controller              = new AuraMousematController(dev, info->path, name);
        RGBController_AuraMousemat* rgb_controller          = new RGBController_AuraMousemat(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBROGStrixLC(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AsusROGStrixLCController*     controller     = new AsusROGStrixLCController(dev, info->path, name);
        RGBController_AsusROGStrixLC* rgb_controller = new RGBController_AsusROGStrixLC(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBRyuoAIO(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AsusAuraRyuoAIOController* controller         = new AsusAuraRyuoAIOController(dev, info->path, name);
        RGBController_AsusAuraRyuoAIO* rgb_controller = new RGBController_AsusAuraRyuoAIO(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBStrixEvolve(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AsusAuraMouseGen1Controller*     controller         = new AsusAuraMouseGen1Controller(dev, info->path, info->product_id, name);
        RGBController_AsusROGStrixEvolve* rgb_controller    = new RGBController_AsusROGStrixEvolve(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBSpatha(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AsusAuraMouseGen1Controller*     controller         = new AsusAuraMouseGen1Controller(dev, info->path, info->product_id, name);
        RGBController_AsusROGSpatha* rgb_controller         = new RGBController_AsusROGSpatha(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBHeadsetStand(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AuraHeadsetStandController*     controller          = new AuraHeadsetStandController(dev, info->path, name);
        RGBController_AuraHeadsetStand* rgb_controller      = new RGBController_AuraHeadsetStand(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraTUFUSBKeyboard(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AuraTUFKeyboardController*     controller           = new AuraTUFKeyboardController(dev, info->path, info->product_id, info->release_number, name);
        RGBController_AuraTUFKeyboard* rgb_controller       = new RGBController_AuraTUFKeyboard(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBMonitor(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AuraMonitorController*     controller           = new AuraMonitorController(dev, info->path, info->product_id, name);
        RGBController_AuraMonitor* rgb_controller       = new RGBController_AuraMonitor(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusROGAlly(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        ROGAllyController*         controller           = new ROGAllyController(dev, info->path, name);
        RGBController_AsusROGAlly* rgb_controller       = new RGBController_AsusROGAlly(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*-----------------------------------------------------------------*\
|  MOTHERBOARDS                                                     |
\*-----------------------------------------------------------------*/
REGISTER_HID_DETECTOR   ("ASUS Aura Addressable",                       DetectAsusAuraUSBAddressable,   AURA_USB_VID, AURA_ADDRESSABLE_1_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Addressable",                       DetectAsusAuraUSBAddressable,   AURA_USB_VID, AURA_ADDRESSABLE_2_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Addressable",                       DetectAsusAuraUSBAddressable,   AURA_USB_VID, AURA_ADDRESSABLE_3_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Addressable",                       DetectAsusAuraUSBAddressable,   AURA_USB_VID, AURA_ADDRESSABLE_4_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Motherboard",                       DetectAsusAuraUSBMotherboards,  AURA_USB_VID, AURA_MOTHERBOARD_1_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Motherboard",                       DetectAsusAuraUSBMotherboards,  AURA_USB_VID, AURA_MOTHERBOARD_2_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Motherboard",                       DetectAsusAuraUSBMotherboards,  AURA_USB_VID, AURA_MOTHERBOARD_3_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Motherboard",                       DetectAsusAuraUSBMotherboards,  AURA_USB_VID, AURA_MOTHERBOARD_4_PID);

/*-----------------------------------------------------------------*\
|  KEYBOARDS                                                        |
\*-----------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope TKL",                        DetectAsusAuraUSBKeyboards,     AURA_USB_VID, AURA_ROG_STRIX_SCOPE_TKL_PID,                     1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope RX TKL Wireless Deluxe",     DetectAsusAuraUSBKeyboards,     AURA_USB_VID, AURA_ROG_STRIX_SCOPE_RX_TKL_DELUXE_PID,           1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope TKL PNK LTD",                DetectAsusAuraUSBKeyboards,     AURA_USB_VID, AURA_ROG_STRIX_SCOPE_TKL_PNK_LTD_PID,             1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Azoth USB",                              DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_AZOTH_USB_PID,                           1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Azoth 2.4GHz",                           DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_AZOTH_2_4_PID,                           1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Claymore",                               DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_CLAYMORE_PID,                            1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Falchion (Wired)",                       DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_FALCHION_WIRED_PID,                      1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Falchion (Wireless)",                    DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_FALCHION_WIRELESS_PID,                   1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Flare",                            DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_FLARE_PID,                         1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Flare PNK LTD",                    DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_FLARE_PNK_LTD_PID,                 1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Flare CoD Black Ops 4 Edition",    DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_FLARE_COD_BO4_PID,                 1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Flare II Animate",                 DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_FLARE_II_ANIMATE_PID,              1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Flare II",                         DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_FLARE_II_PID,                      1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope",                            DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_PID,                         1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope RX",                         DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_RX_PID,                      1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope RX EVA-02 Edition",          DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_RX_EVA_02_PID,               1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope NX Wireless Deluxe USB",     DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_USB_PID,  1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope NX Wireless Deluxe 2.4GHz",  DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_2_4_PID,  1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope II",                         DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_II_PID,                      1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope II RX",                      DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_II_RX_PID,                   1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope II 96 Wireless USB",         DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID,      1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming K1",                              DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_TUF_K1_GAMING_PID,                           2,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming K3",                              DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_TUF_K3_GAMING_PID,                           1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming K5",                              DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_TUF_K5_GAMING_PID,                           2,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming K7",                              DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_TUF_K7_GAMING_PID,                           1,  0xFF00);

/*-----------------------------------------------------------------*\
|  MICE                                                             |
\*-----------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Core",                            DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_CORE_PID,                     0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II",                                 DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_PID,                          2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Origin",                          DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_ORIGIN_PID,                   2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Origin PNK LTD",                  DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_ORIGIN_PNK_LTD_PID,           2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Origin COD",                      DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_ORIGIN_COD_PID,               2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Wireless",                        DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_WIRELESS_1_PID,               1,  0xFF13);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Wireless",                        DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_WIRELESS_2_PID,               2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III",                                DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_PID,                         0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III Wireless USB",                   DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_WIRELESS_USB_PID,            0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III Wireless 2.4Ghz",                DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_WIRELESS_2_4_PID,            0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III Wireless Bluetooth",             DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_WIRELESS_BT_PID,             0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III Wireless AimPoint USB",          DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_WIRELESS_AIMPOINT_USB_PID,   0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III Wireless AimPoint 2.4Ghz",       DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_WIRELESS_AIMPOINT_2_4_PID,   0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Chakram (Wireless)",                         DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_CHAKRAM_WIRELESS_PID,                    0,  0xFF01);
REGISTER_HID_DETECTOR_IP("Asus ROG Chakram (Wired)",                            DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_CHAKRAM_WIRED_1_PID,                     0,  0xFF01);
REGISTER_HID_DETECTOR_IP("Asus ROG Chakram Core",                               DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_CHAKRAM_CORE_PID,                        0,  0xFF01);
REGISTER_HID_DETECTOR_IP("Asus ROG Chakram X USB",                              DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_CHAKRAM_X_USB_PID,                       0,  0xFF01);
REGISTER_HID_DETECTOR_IP("Asus ROG Chakram X 2.4GHz",                           DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_CHAKRAM_X_2_4_PID,                       0,  0xFF01);
REGISTER_HID_DETECTOR_IP("Asus ROG Spatha X USB",                               DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_SPATHA_X_USB_PID,                        0,  0xFF01);
REGISTER_HID_DETECTOR_IP("Asus ROG Spatha X 2.4GHz",                            DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_SPATHA_X_2_4_PID,                        0,  0xFF01);
REGISTER_HID_DETECTOR_IP("Asus ROG Spatha X Dock",                              DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_SPATHA_X_DOCK_PID,                       0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Pugio",                                      DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_PUGIO_PID,                               2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Pugio II (Wired)",                           DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_PUGIO_II_WIRED_PID,                      0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Pugio II (Wireless)",                        DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_PUGIO_II_WIRELESS_PID,                   0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact",                               DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_PID,                        2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact II",                            DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_II_PID,                     0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact II Gundam",                     DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_II_GUNDAM_PID,              0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact II Electro Punk",               DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_II_PUNK_PID,                0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact II Moonlight White",            DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_II_WHITE_PID,               0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact II Wireless USB",               DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_II_WIRELESS_USB_PID,        0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact II Wireless 2.4 Ghz",           DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_II_WIRELESS_2_4_PID,        0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact III",                           DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_III_PID,                    0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Keris",                                      DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_KERIS,                                   0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Keris Wireless USB",                         DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_KERIS_WIRELESS_USB_PID,                  0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Keris Wireless 2.4Ghz",                      DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_KERIS_WIRELESS_2_4_PID,                  0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Keris Wireless Bluetooth",                   DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_KERIS_WIRELESS_BT_PID,                   0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Keris Wireless AimPoint USB",                DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_KERIS_WIRELESS_AIMPOINT_USB_PID,         0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Keris Wireless AimPoint 2.4Ghz",             DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_KERIS_WIRELESS_AIMPOINT_2_4_PID,         0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming M3",                                  DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_TUF_M3_PID,                                  1,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming M3 Gen II",                           DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_TUF_M3_GEN_II_PID,                           2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming M5",                                  DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_TUF_M5_PID,                                  2,  0xFF01);

REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Evolve",                       DetectAsusAuraUSBStrixEvolve,   AURA_USB_VID, AURA_ROG_STRIX_EVOLVE_PID,                    1,  0x0008);
REGISTER_HID_DETECTOR_IP("ASUS ROG Spatha Wired",                       DetectAsusAuraUSBSpatha,        AURA_USB_VID, AURA_ROG_SPATHA_WIRED_PID,                    1,  0x0008);
REGISTER_HID_DETECTOR_IP("ASUS ROG Spatha Wireless",                    DetectAsusAuraUSBSpatha,        AURA_USB_VID, AURA_ROG_SPATHA_WIRELESS_PID,                 1,  0x0008);

/*-----------------------------------------------------------------*\
| MOUSEMATS                                                         |
\*-----------------------------------------------------------------*/
REGISTER_HID_DETECTOR_PU("ASUS ROG Balteus",                            DetectAsusAuraUSBMousemats,     AURA_USB_VID, AURA_ROG_BALTEUS_PID,                         0xFF06, 1);
REGISTER_HID_DETECTOR_PU("ASUS ROG Balteus Qi",                         DetectAsusAuraUSBMousemats,     AURA_USB_VID, AURA_ROG_BALTEUS_QI_PID,                      0xFF06, 1);

/*-----------------------------------------------------------------*\
| MONITORS                                                          |
\*-----------------------------------------------------------------*/
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix XG27AQ",                       DetectAsusAuraUSBMonitor,       AURA_USB_VID, AURA_ROG_STRIX_XG27AQ_PID,                    0xFFA0, 1);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix XG27AQM",                      DetectAsusAuraUSBMonitor,       AURA_USB_VID, AURA_ROG_STRIX_XG27AQM_PID,                   0xFFA0, 1);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix XG279Q",                       DetectAsusAuraUSBMonitor,       AURA_USB_VID, AURA_ROG_STRIX_XG279Q_PID,                    0xFFA0, 1);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix XG27W",                        DetectAsusAuraUSBMonitor,       AURA_USB_VID, AURA_ROG_STRIX_XG27W_PID,                     0xFFA0, 1);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix XG32VC",                       DetectAsusAuraUSBMonitor,       AURA_USB_VID, AURA_ROG_STRIX_XG32VC_PID,                    0xFFA0, 1);
REGISTER_HID_DETECTOR_PU("ASUS ROG PG32UQ",                             DetectAsusAuraUSBMonitor,       AURA_USB_VID, AURA_ROG_PG32UQ_PID,                          0xFFA0, 1);

/*-----------------------------------------------------------------*\
|  OTHER                                                            |
\*-----------------------------------------------------------------*/
REGISTER_HID_DETECTOR    ("ASUS ROG AURA Terminal",                     DetectAsusAuraUSBTerminal,      AURA_USB_VID, AURA_TERMINAL_PID);
REGISTER_HID_DETECTOR_PU ("ASUS ROG Strix LC",                          DetectAsusAuraUSBROGStrixLC,    AURA_USB_VID, ROG_STRIX_LC120_PID,                          0x00FF, 1);
REGISTER_HID_DETECTOR_PU ("ASUS ROG Ryuo AIO",                          DetectAsusAuraUSBRyuoAIO,       AURA_USB_VID, AURA_RYUO_AIO_PID,                        0xFF72, 0x00A1);
REGISTER_HID_DETECTOR_I  ("ASUS ROG Throne",                            DetectAsusAuraUSBHeadsetStand,  AURA_USB_VID, AURA_ROG_THRONE_PID,                          0);
REGISTER_HID_DETECTOR_I  ("ASUS ROG Throne QI",                         DetectAsusAuraUSBHeadsetStand,  AURA_USB_VID, AURA_ROG_THRONE_QI_PID,                       0);
REGISTER_HID_DETECTOR_I  ("ASUS ROG Throne QI GUNDAM",                  DetectAsusAuraUSBHeadsetStand,  AURA_USB_VID, AURA_ROG_THRONE_QI_GUNDAM_PID,                0);
REGISTER_HID_DETECTOR_IPU("ASUS ROG Ally",                              DetectAsusROGAlly,              AURA_USB_VID, ASUS_ROG_ALLY_PID,                            2,  0xFF31, 0x0076);
REGISTER_HID_DETECTOR_IPU("ASUS ROG Ally X",                            DetectAsusROGAlly,              AURA_USB_VID, ASUS_ROG_ALLY_X_PID,                          2,  0xFF31, 0x0076);
