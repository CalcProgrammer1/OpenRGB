#include "Detector.h"
#include "LogitechG203Controller.h"
#include "LogitechG203LController.h"
#include "LogitechG403Controller.h"
#include "LogitechG502PSController.h"
#include "LogitechG810Controller.h"
#include "LogitechG213Controller.h"
#include "LogitechGProWirelessController.h"
#include "LogitechGPowerPlayController.h"
#include "RGBController.h"
#include "RGBController_LogitechG203.h"
#include "RGBController_LogitechG203L.h"
#include "RGBController_LogitechG403.h"
#include "RGBController_LogitechG502PS.h"
#include "RGBController_LogitechG810.h"
#include "RGBController_LogitechG213.h"
#include "RGBController_LogitechGProWireless.h"
#include "RGBController_LogitechGPowerPlay.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Logitech vendor ID                                    |
\*-----------------------------------------------------*/
#define LOGITECH_VID                            0x046D
/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define LOGITECH_G213_PID                       0xC336
#define LOGITECH_G512_PID                       0xC342
#define LOGITECH_G512_RGB_PID                   0xC33C
#define LOGITECH_G610_1_PID                     0xC333
#define LOGITECH_G610_2_PID                     0xC338
#define LOGITECH_G810_1_PID                     0xC331
#define LOGITECH_G810_2_PID                     0xC337
/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define LOGITECH_G203_PID                       0xC084
#define LOGITECH_G203L_PID                      0xC092
#define LOGITECH_G403_PID                       0xC083
#define LOGITECH_G403H_PID                      0xC08F
#define LOGITECH_G502_PS_PID                    0xC332
#define LOGITECH_G502H_PID                      0xC08B
#define LOGITECH_G_LIGHTSPEED_WIRELESS_PID      0xC539
#define LOGITECH_GPRO_WIRED_PID                 0xC085
#define LOGITECH_GPRO_HERO_WIRED_PID            0xC08C
#define LOGITECH_GPRO_WIRELESS_PID              0xC088
/*-----------------------------------------------------*\
| Mousemat product IDs                                  |
\*-----------------------------------------------------*/
#define LOGITECH_G_LIGHTSPEED_POWERPLAY_PID     0xC53A

void DetectLogitechKeyboardG810(hid_device_info* info, const std::string& name)
{
    /*-------------------------------------------------------------------------------------------------*\
    | Logitech keyboards use two different usages, one for 20-byte packets and one for 64-byte packets  |
    | Usage 0x0602 for 20 byte, usage 0x0604 for 64 byte, both are on usage page 0xFF43                 |
    \*-------------------------------------------------------------------------------------------------*/
#ifdef _WIN32
     hid_device* dev_usage_0x0602 = nullptr;
     hid_device* dev_usage_0x0604 = nullptr;
     hid_device_info* info_temp = info;
     while(info_temp)
     {
         if(info_temp->vendor_id        == info->vendor_id        // constant LOGITECH_VID
         && info_temp->product_id       == info->product_id       // NON-constant
         && info_temp->interface_number == info->interface_number // constant 1
         && info_temp->usage_page       == info->usage_page)      // constant 0xFF43
         {
             if(info_temp->usage == 0x0602)
             {
                dev_usage_0x0602 = hid_open_path(info_temp->path);
             }
             else if(info_temp->usage == 0x0604)
             {
                 dev_usage_0x0604 = hid_open_path(info_temp->path);
             }
         }
         if(dev_usage_0x0602 && dev_usage_0x0604)
         {
             break;
         }
         info_temp = info_temp->next;
     }
     if(dev_usage_0x0602 && dev_usage_0x0604)
     {
         LogitechG810Controller* controller = new LogitechG810Controller(dev_usage_0x0602, dev_usage_0x0604);
         RGBController_LogitechG810* rgb_controller = new RGBController_LogitechG810(controller);
         rgb_controller->name = name;
         ResourceManager::get()->RegisterRGBController(rgb_controller);
     }
     else
     {
         // Not all of them could be opened, do some cleanup
         hid_close(dev_usage_0x0602);
         hid_close(dev_usage_0x0604);
     }
#else
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        LogitechG810Controller* controller = new LogitechG810Controller(dev, dev);
        RGBController_LogitechG810* rgb_controller = new RGBController_LogitechG810(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
#endif
}

void DetectLogitechKeyboardG213(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        LogitechG213Controller* controller = new LogitechG213Controller(dev, info->path);
        RGBController_LogitechG213* rgb_controller = new RGBController_LogitechG213(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectLogitechMouseG203(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        LogitechG203Controller* controller = new LogitechG203Controller(dev, info->path);
        RGBController_LogitechG203* rgb_controller = new RGBController_LogitechG203(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectLogitechMouseG203L(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        LogitechG203LController* controller = new LogitechG203LController(dev, info->path);
        RGBController_LogitechG203L* rgb_controller = new RGBController_LogitechG203L(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectLogitechMouseG403(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        LogitechG403Controller* controller = new LogitechG403Controller(dev, info->path);
        RGBController_LogitechG403* rgb_controller = new RGBController_LogitechG403(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectLogitechMouseG502PS(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        LogitechG502PSController* controller = new LogitechG502PSController(dev, info->path);
        RGBController_LogitechG502PS* rgb_controller = new RGBController_LogitechG502PS(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectLogitechMouseGPRO(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        LogitechGProWirelessController* controller = new LogitechGProWirelessController(dev, info->path);
        RGBController_LogitechGProWireless* rgb_controller = new RGBController_LogitechGProWireless(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectLogitechMouseGLS(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        //Add mouse
        LogitechGProWirelessController* mouse_controller = new LogitechGProWirelessController(dev, info->path);
        RGBController_LogitechGProWireless* mouse_rgb_controller = new RGBController_LogitechGProWireless(mouse_controller);
        mouse_rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(mouse_rgb_controller);

        //Add Powerplay mousemat
        LogitechGPowerPlayController* mousemat_controller = new LogitechGPowerPlayController(dev, info->path);
        RGBController_LogitechGPowerPlay* mousemat_rgb_controller = new RGBController_LogitechGPowerPlay(mousemat_controller);
        mousemat_rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(mousemat_rgb_controller);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                                                                         |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech G213",                                 DetectLogitechKeyboardG213, LOGITECH_VID, LOGITECH_G213_PID,                   1, 0xFF43, 0x0602);
REGISTER_HID_DETECTOR_IP ("Logitech G512",                                 DetectLogitechKeyboardG810, LOGITECH_VID, LOGITECH_G512_PID,                   1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G512 RGB",                             DetectLogitechKeyboardG810, LOGITECH_VID, LOGITECH_G512_RGB_PID,               1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G610 Orion",                           DetectLogitechKeyboardG810, LOGITECH_VID, LOGITECH_G610_1_PID,                 1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G610 Orion",                           DetectLogitechKeyboardG810, LOGITECH_VID, LOGITECH_G610_2_PID,                 1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G810 Orion Spectrum",                  DetectLogitechKeyboardG810, LOGITECH_VID, LOGITECH_G810_1_PID,                 1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G810 Orion Spectrum",                  DetectLogitechKeyboardG810, LOGITECH_VID, LOGITECH_G810_2_PID,                 1, 0xFF43);
/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Mice                                                                                                                                              |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech G203 Prodigy",                         DetectLogitechMouseG203,    LOGITECH_VID, LOGITECH_G203_PID,                   1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G203 Lightsync",                       DetectLogitechMouseG203L,   LOGITECH_VID, LOGITECH_G203L_PID,                  1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G403 Prodigy",                         DetectLogitechMouseG403,    LOGITECH_VID, LOGITECH_G403_PID,                   1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G403 Hero",                            DetectLogitechMouseG403,    LOGITECH_VID, LOGITECH_G403H_PID,                  1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G502 Proteus Spectrum",                DetectLogitechMouseG502PS,  LOGITECH_VID, LOGITECH_G502_PS_PID,                1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G502 Hero",                            DetectLogitechMouseG502PS,  LOGITECH_VID, LOGITECH_G502H_PID,                  1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G Pro Gaming Mouse",                   DetectLogitechMouseG203,    LOGITECH_VID, LOGITECH_GPRO_WIRED_PID,             1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G Pro (HERO) Gaming Mouse",            DetectLogitechMouseG203,    LOGITECH_VID, LOGITECH_GPRO_HERO_WIRED_PID,        1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G Lightspeed Wireless Gaming Mouse",   DetectLogitechMouseGPRO,    LOGITECH_VID, LOGITECH_G_LIGHTSPEED_WIRELESS_PID,  2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G Pro Wireless Gaming Mouse (Wired)",  DetectLogitechMouseGPRO,    LOGITECH_VID, LOGITECH_GPRO_WIRELESS_PID,          2, 0xFF00, 2);
/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Mousemats                                                                                                                                         |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech G Powerplay Mousepad with Lightspeed", DetectLogitechMouseGLS,     LOGITECH_VID, LOGITECH_G_LIGHTSPEED_POWERPLAY_PID, 2, 0xFF00, 2);
