/*---------------------------------------------------------*\
| AsusAuraKeyboardControllerDetect.cpp                      |
|                                                           |
|   Detector for ASUS Aura keyboard devices                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusAuraKeyboardController.h"
#include "RGBController_AsusAuraKeyboard.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define AURA_USB_VID                                0x0B05

/*---------------------------------------------------------*\
| Keyboard PIDs                                             |
\*---------------------------------------------------------*/
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
#define AURA_ROG_STRIX_SCOPE_NX_TKL_MOONLIGHT_WHITE_PID     0x19D0
#define AURA_ROG_STRIX_SCOPE_TKL_PNK_LTD_PID                0x1954
#define AURA_ROG_STRIX_SCOPE_RX_PID                         0x1951
#define AURA_ROG_STRIX_SCOPE_RX_EVA_02_PID                  0x1B12
#define AURA_ROG_STRIX_SCOPE_RX_TKL_DELUXE_PID              0x1A05
#define AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_USB_PID     0x19F6
#define AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_2_4_PID     0x19F8
#define AURA_ROG_STRIX_SCOPE_II_PID                         0x1AB3
#define AURA_ROG_STRIX_SCOPE_II_RX_PID                      0x1AB5
#define AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID         0x1AAE
#define AURA_ROG_STRIX_SCOPE_II_96_RX_WIRELESS_USB_PID      0x1B78
#define AURA_TUF_K1_GAMING_PID                              0x1945
#define AURA_TUF_K3_GAMING_PID                              0x194B
#define AURA_TUF_K3_GAMING_GEN_II_PID                       0x1B30
#define AURA_TUF_K5_GAMING_PID                              0x1899
#define AURA_TUF_K7_GAMING_PID                              0x18AA
#define AURA_TUF_K3_GENII_MIKU_EDITION_PID                  0x1C5E

DetectedControllers DetectAsusAuraUSBKeyboard(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AuraKeyboardController*     controller           = new AuraKeyboardController(dev, info->path, info->product_id, info->release_number, name);
        RGBController_AuraKeyboard* rgb_controller       = new RGBController_AuraKeyboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| Keyboard Detectors                                        |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope TKL",                        DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_TKL_PID,                     1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope TKL PNK LTD",                DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_TKL_PNK_LTD_PID,             1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope RX TKL Wireless Deluxe",     DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_RX_TKL_DELUXE_PID,           1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope NX TKL Moonlight White",     DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_NX_TKL_MOONLIGHT_WHITE_PID,  1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Azoth USB",                              DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_AZOTH_USB_PID,                           1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Azoth 2.4GHz",                           DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_AZOTH_2_4_PID,                           1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Claymore",                               DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_CLAYMORE_PID,                            1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Falchion (Wired)",                       DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_FALCHION_WIRED_PID,                      1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Falchion (Wireless)",                    DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_FALCHION_WIRELESS_PID,                   1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Flare",                            DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_FLARE_PID,                         1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Flare PNK LTD",                    DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_FLARE_PNK_LTD_PID,                 1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Flare CoD Black Ops 4 Edition",    DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_FLARE_COD_BO4_PID,                 1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Flare II Animate",                 DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_FLARE_II_ANIMATE_PID,              1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Flare II",                         DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_FLARE_II_PID,                      1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope",                            DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_PID,                         1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope RX",                         DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_RX_PID,                      1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope RX EVA-02 Edition",          DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_RX_EVA_02_PID,               1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope NX Wireless Deluxe USB",     DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_USB_PID,  1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope NX Wireless Deluxe 2.4GHz",  DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_2_4_PID,  1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope II",                         DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_II_PID,                      1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope II RX",                      DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_II_RX_PID,                   1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope II 96 Wireless USB",         DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID,      1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Scope II 96 RX Wireless USB",      DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_ROG_STRIX_SCOPE_II_96_RX_WIRELESS_USB_PID,   1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming K1",                              DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_TUF_K1_GAMING_PID,                           2,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming K3",                              DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_TUF_K3_GAMING_PID,                           1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming K3 GEN II",                       DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_TUF_K3_GAMING_GEN_II_PID,                    1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming K5",                              DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_TUF_K5_GAMING_PID,                           2,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming K7",                              DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_TUF_K7_GAMING_PID,                           1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming K3 GEN II MIKU EDITION",          DetectAsusAuraUSBKeyboard,   AURA_USB_VID, AURA_TUF_K3_GENII_MIKU_EDITION_PID,               1,  0xFF00);
