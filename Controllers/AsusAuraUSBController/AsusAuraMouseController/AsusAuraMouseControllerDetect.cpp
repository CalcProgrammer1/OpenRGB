/*---------------------------------------------------------*\
| AsusAuraMouseControllerDetect.cpp                         |
|                                                           |
|   Detector for ASUS Aura Mouse devices                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusAuraMouseController.h"
#include "RGBController_AsusAuraMouse.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define AURA_USB_VID                                0x0B05

DetectedControllers DetectAsusAuraUSBMice(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        uint16_t pid = (name == "Asus ROG Spatha X Dock") ? AURA_ROG_SPATHA_X_DOCK_FAKE_PID : info->product_id;
        AuraMouseController*     controller                 = new AuraMouseController(dev, info->path, pid, name);
        RGBController_AuraMouse* rgb_controller             = new RGBController_AuraMouse(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| Mouse Detectors                                           |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Core",                        DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_CORE_PID,                     0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II",                             DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_PID,                          2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Origin",                      DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_ORIGIN_PID,                   2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Origin PNK LTD",              DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_ORIGIN_PNK_LTD_PID,           2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Origin COD",                  DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_ORIGIN_COD_PID,               2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Wireless",                    DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_WIRELESS_1_PID,               1,  0xFF13);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Wireless",                    DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_II_WIRELESS_2_PID,               2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III",                            DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_PID,                         0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III Core",                       DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_CORE_PID,                    0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III Wireless USB",               DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_WIRELESS_USB_PID,            0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III Wireless 2.4Ghz",            DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_WIRELESS_2_4_PID,            0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III Wireless Bluetooth",         DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_WIRELESS_BT_PID,             0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III Wireless AimPoint USB",      DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_WIRELESS_AIMPOINT_USB_PID,   0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius III Wireless AimPoint 2.4Ghz",   DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_GLADIUS_III_WIRELESS_AIMPOINT_2_4_PID,   0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Chakram (Wireless)",                     DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_CHAKRAM_WIRELESS_PID,                    0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Chakram (Wired)",                        DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_CHAKRAM_WIRED_1_PID,                     0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Chakram Core",                           DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_CHAKRAM_CORE_PID,                        0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Chakram X USB",                          DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_CHAKRAM_X_USB_PID,                       0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Chakram X 2.4GHz",                       DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_CHAKRAM_X_2_4_PID,                       0,  0xFF01);
REGISTER_HID_DETECTOR_IP("Asus ROG Spatha X USB",                           DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_SPATHA_X_USB_PID,                        0,  0xFF01);
REGISTER_HID_DETECTOR_IP("Asus ROG Spatha X 2.4GHz",                        DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_SPATHA_X_2_4_PID,                        0,  0xFF01);
REGISTER_HID_DETECTOR_IP("Asus ROG Spatha X Dock",                          DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_SPATHA_X_DOCK_PID,                       0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Pugio",                                  DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_PUGIO_PID,                               2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Pugio II (Wired)",                       DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_PUGIO_II_WIRED_PID,                      0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Pugio II (Wireless)",                    DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_PUGIO_II_WIRELESS_PID,                   0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact",                           DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_PID,                        2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact II",                        DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_II_PID,                     0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact II Gundam",                 DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_II_GUNDAM_PID,              0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact II Electro Punk",           DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_II_PUNK_PID,                0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact II Moonlight White",        DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_II_WHITE_PID,               0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact II Wireless USB",           DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_II_WIRELESS_USB_PID,        0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact II Wireless 2.4 Ghz",       DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_II_WIRELESS_2_4_PID,        0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Impact III",                       DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_STRIX_IMPACT_III_PID,                    0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Keris",                                  DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_KERIS,                                   0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Keris Wireless USB",                     DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_KERIS_WIRELESS_USB_PID,                  0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Keris Wireless 2.4Ghz",                  DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_KERIS_WIRELESS_2_4_PID,                  0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Keris Wireless Bluetooth",               DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_KERIS_WIRELESS_BT_PID,                   0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Keris Wireless AimPoint USB",            DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_KERIS_WIRELESS_AIMPOINT_USB_PID,         0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Keris Wireless AimPoint 2.4Ghz",         DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_ROG_KERIS_WIRELESS_AIMPOINT_2_4_PID,         0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming M3",                              DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_TUF_M3_PID,                                  1,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming M3 Gen II",                       DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_TUF_M3_GEN_II_PID,                           2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS TUF Gaming M5",                              DetectAsusAuraUSBMice,  AURA_USB_VID, AURA_TUF_M5_PID,                                  2,  0xFF01);
