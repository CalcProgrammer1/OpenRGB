#include "Detector.h"
#include "AsusAuraAddressableController.h"
#include "AsusAuraHeadsetStandController.h"
#include "AsusAuraKeyboardController.h"
#include "AsusAuraTUFKeyboardController.h"
#include "AsusAuraMainboardController.h"
#include "AsusAuraMouseController.h"
#include "RGBController.h"
#include "RGBController_AsusAuraUSB.h"
#include "RGBController_AsusAuraHeadsetStand.h"
#include "RGBController_AsusAuraKeyboard.h"
#include "RGBController_AsusAuraTUFKeyboard.h"
#include "RGBController_AsusAuraMouse.h"
#include <stdexcept>
#include <hidapi/hidapi.h>
#include "dependencies/dmiinfo.h"

#define AURA_USB_VID                            0x0B05
#define AURA_TERMINAL_PID                       0x1889
#define AURA_ADDRESSABLE_1_PID                  0x1867
#define AURA_ADDRESSABLE_2_PID                  0x1872
#define AURA_ADDRESSABLE_3_PID                  0x18A3
#define AURA_ADDRESSABLE_4_PID                  0x18A5
#define AURA_MOTHERBOARD_1_PID                  0x18F3
#define AURA_MOTHERBOARD_2_PID                  0x1939
#define AURA_MOTHERBOARD_3_PID                  0x19AF
#define AURA_ROG_GLADIUS_II_CORE_PID            0x18DD
#define AURA_ROG_GLADIUS_II_PID                 0x1845
#define AURA_ROG_STRIX_FLARE_PID                0x1875
#define AURA_ROG_GLADIUS_II_ORIGIN_PID          0x1877
#define AURA_ROG_GLADIUS_II_ORIGIN_PNK_LTD_PID  0x18CD
#define AURA_ROG_GLADIUS_II_WIRELESS_PID        0x18A0
#define AURA_ROG_STRIX_FLARE_PNK_LTD_PID        0x18CF
#define AURA_ROG_CHAKRAM_WIRELESS_PID           0x18E5
#define AURA_ROG_CHAKRAM_WIRED_1_PID            0x18E3
#define AURA_ROG_CHAKRAM_WIRED_2_PID            0x1958
#define AURA_ROG_THRONE_QI_PID                  0x18C5
#define AURA_TUF_K7_GAMING_PID				    0x18AA

void DetectAsusAuraUSBTerminal(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AuraAddressableController* controller = new AuraAddressableController(dev, info->path);
        RGBController_AuraUSB* rgb_controller = new RGBController_AuraUSB(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBAddressable(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        DMIInfo dmi;
        AuraAddressableController* controller = new AuraAddressableController(dev, info->path);
        RGBController_AuraUSB* rgb_controller = new RGBController_AuraUSB(controller);
        rgb_controller->name = "ASUS " + dmi.getMainboard() + " Addressable";
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBMotherboards(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        try
        {
            DMIInfo dmi;
            AuraMainboardController* controller = new AuraMainboardController(dev, info->path);
            RGBController_AuraUSB* rgb_controller = new RGBController_AuraUSB(controller);
            rgb_controller->name = "ASUS " + dmi.getMainboard();
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        catch(std::runtime_error&)
        {
            // reading the config table failed
        }
    }
}

void DetectAsusAuraUSBKeyboards(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        AuraKeyboardController* controller = new AuraKeyboardController(dev, info->path);
        RGBController_AuraKeyboard* rgb_controller = new RGBController_AuraKeyboard(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBMice(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        AuraMouseController* controller = new AuraMouseController(dev, info->path);
        RGBController_AuraMouse* rgb_controller = new RGBController_AuraMouse(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraUSBHeadsetStand(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        AuraHeadsetStandController* controller = new AuraHeadsetStandController(dev, info->path);
        RGBController_AuraHeadsetStand* rgb_controller = new RGBController_AuraHeadsetStand(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusAuraTUFUSBKeyboard(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        AuraTUFKeyboardController* controller = new AuraTUFKeyboardController(dev, info->path);
        RGBController_AuraTUFKeyboard* rgb_controller = new RGBController_AuraTUFKeyboard(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR   ("ASUS ROG AURA Terminal",              DetectAsusAuraUSBTerminal,      AURA_USB_VID, AURA_TERMINAL_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Addressable",               DetectAsusAuraUSBAddressable,   AURA_USB_VID, AURA_ADDRESSABLE_1_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Addressable",               DetectAsusAuraUSBAddressable,   AURA_USB_VID, AURA_ADDRESSABLE_2_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Addressable",               DetectAsusAuraUSBAddressable,   AURA_USB_VID, AURA_ADDRESSABLE_3_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Addressable",               DetectAsusAuraUSBAddressable,   AURA_USB_VID, AURA_ADDRESSABLE_4_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Motherboard",               DetectAsusAuraUSBMotherboards,  AURA_USB_VID, AURA_MOTHERBOARD_1_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Motherboard",               DetectAsusAuraUSBMotherboards,  AURA_USB_VID, AURA_MOTHERBOARD_2_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Motherboard",               DetectAsusAuraUSBMotherboards,  AURA_USB_VID, AURA_MOTHERBOARD_3_PID);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Core",            DetectAsusAuraUSBMice,          AURA_USB_VID, AURA_ROG_GLADIUS_II_CORE_PID,             0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II",                 DetectAsusAuraUSBMice,          AURA_USB_VID, AURA_ROG_GLADIUS_II_PID,                  2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Origin",          DetectAsusAuraUSBMice,          AURA_USB_VID, AURA_ROG_GLADIUS_II_ORIGIN_PID,           2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Origin PNK LTD",  DetectAsusAuraUSBMice,          AURA_USB_VID, AURA_ROG_GLADIUS_II_ORIGIN_PNK_LTD_PID,   2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Gladius II Wireless",        DetectAsusAuraUSBMice,          AURA_USB_VID, AURA_ROG_GLADIUS_II_WIRELESS_PID,         2,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Chakram (Wireless)",         DetectAsusAuraUSBMice,          AURA_USB_VID, AURA_ROG_CHAKRAM_WIRELESS_PID,            0,  0xFF01);
REGISTER_HID_DETECTOR_IP("Asus ROG Chakram (Wired)",            DetectAsusAuraUSBMice,          AURA_USB_VID, AURA_ROG_CHAKRAM_WIRED_1_PID,             0,  0xFF01);
REGISTER_HID_DETECTOR_IP("Asus ROG Chakram (Wired)",            DetectAsusAuraUSBMice,          AURA_USB_VID, AURA_ROG_CHAKRAM_WIRED_2_PID,             0,  0xFF01);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Flare",                DetectAsusAuraUSBKeyboards,     AURA_USB_VID, AURA_ROG_STRIX_FLARE_PID,                 1,  0xFF00);
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Flare PNK LTD",        DetectAsusAuraUSBKeyboards,     AURA_USB_VID, AURA_ROG_STRIX_FLARE_PNK_LTD_PID,         1,  0xFF00);
REGISTER_HID_DETECTOR_I ("ASUS ROG Throne QI",         			DetectAsusAuraUSBHeadsetStand,  AURA_USB_VID, AURA_ROG_THRONE_QI_PID, 					0);
REGISTER_HID_DETECTOR_I ("ASUS TUF Gaming K7",         			DetectAsusAuraTUFUSBKeyboard,   AURA_USB_VID, AURA_TUF_K7_GAMING_PID, 					1);
