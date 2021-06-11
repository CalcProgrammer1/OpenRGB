#include "Detector.h"
#include "LogManager.h"
#include "LogitechProtocolCommon.h"
#include "LogitechG203LController.h"
#include "LogitechG213Controller.h"
#include "LogitechG560Controller.h"
#include "LogitechG810Controller.h"
#include "LogitechG910Controller.h"
#include "LogitechG815Controller.h"
#include "LogitechGLightsyncController.h"
#include "LogitechLightspeedController.h"
#include "LogitechX56Controller.h"
#include "RGBController.h"
#include "RGBController_LogitechG203L.h"
#include "RGBController_LogitechG213.h"
#include "RGBController_LogitechG560.h"
#include "RGBController_LogitechG810.h"
#include "RGBController_LogitechG910.h"
#include "RGBController_LogitechG815.h"
#include "RGBController_LogitechGLightsync.h"
#include "RGBController_LogitechGLightsync1zone.h"
#include "RGBController_LogitechLightspeed.h"
#include "RGBController_LogitechGPowerPlay.h"
#include "RGBController_LogitechX56.h"
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
#define LOGITECH_G813_PID                       0xC232
#define LOGITECH_G815_PID                       0xC33F
#define LOGITECH_G910_ORION_SPARK_PID           0xC32B
#define LOGITECH_G910_PID                       0xC335

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define LOGITECH_G203_PID                       0xC084
#define LOGITECH_G203_LIGHTSYNC_PID             0xC092
#define LOGITECH_G303_PID                       0xC080
#define LOGITECH_G403_PID                       0xC083
#define LOGITECH_G403_HERO_PID                  0xC08F
#define LOGITECH_G403_LIGHTSPEED_PID            0xC082
#define LOGITECH_G502_PROTEUS_SPECTRUM_PID      0xC332
#define LOGITECH_G502_HERO_PID                  0xC08B
#define LOGITECH_G502_LIGHTSPEED_PID            0xC08D
#define LOGITECH_G703_LIGHTSPEED_PID            0xC087
#define LOGITECH_G900_LIGHTSPEED_PID            0xC081
#define LOGITECH_G903_LIGHTSPEED_PID            0xC086
#define LOGITECH_G_PRO_PID                      0xC085
#define LOGITECH_G_PRO_HERO_PID                 0xC08C
#define LOGITECH_G_PRO_WIRELESS_PID             0xC088

/*-----------------------------------------------------*\
| Mousemat product IDs                                  |
\*-----------------------------------------------------*/
#define LOGITECH_G_LIGHTSPEED_POWERPLAY_PID     0xC53A

/*-----------------------------------------------------*\
| Speaker product IDs                                   |
\*-----------------------------------------------------*/
#define LOGITECH_G560_PID                       0x0A78

/*-----------------------------------------------------*\
| Unifying Device IDs (Including Lightspeed receivers)  |
|   NB: Not used but preserved for debugging            |
\*-----------------------------------------------------*/
#define LOGITECH_G_UNIFYING_RECEIVER_1_PID              0xC52B
#define LOGITECH_G_NANO_RECEIVER_PID                    0xC52F
#define LOGITECH_G_G700_RECEIVER_PID                    0xC531
#define LOGITECH_G_UNIFYING_RECEIVER_2_PID              0xC532
#define LOGITECH_G_G602_RECEIVER_PID                    0xC537

#define LOGITECH_G_LIGHTSPEED_RECEIVER_PID              0xC539
#define LOGITECH_G403_LIGHTSPEED_VIRTUAL_PID            0x405D
#define LOGITECH_G502_LIGHTSPEED_VIRTUAL_PID            0x407F
#define LOGITECH_G703_LIGHTSPEED_VIRTUAL_PID            0x4070
#define LOGITECH_G900_LIGHTSPEED_VIRTUAL_PID            0x4053
#define LOGITECH_G903_LIGHTSPEED_VIRTUAL_PID            0x4067
#define LOGITECH_G_PRO_WIRELESS_VIRTUAL_PID             0x4079

/*-----------------------------------------------------*\
| Joystick product IDs                                  |
\*-----------------------------------------------------*/
#define LOGITECH_X56_VID                                0x0738
#define LOGITECH_X56_JOYSTICK_PID                       0x2221
#define LOGITECH_X56_THROTTLE_PID                       0xA221

/*-----------------------------------------------------*\
| Logitech Keyboards                                    |
\*-----------------------------------------------------*/
void DetectLogitechKeyboardG213(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LogitechG213Controller*     controller     = new LogitechG213Controller(dev, info->path);
        RGBController_LogitechG213* rgb_controller = new RGBController_LogitechG213(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectLogitechKeyboardG810(hid_device_info* info, const std::string& name)
{
    /*-------------------------------------------------------------------------------------------------*\
    | Logitech keyboards use two different usages, one for 20-byte packets and one for 64-byte packets  |
    | Usage 0x0602 for 20 byte, usage 0x0604 for 64 byte, both are on usage page 0xFF43                 |
    \*-------------------------------------------------------------------------------------------------*/
#ifdef USE_HID_USAGE
    hid_device* dev_usage_0x0602 = nullptr;
    hid_device* dev_usage_0x0604 = nullptr;
    hid_device_info* info_temp = info;

    while(info_temp)
    {
        if(info_temp->vendor_id        == info->vendor_id           // constant LOGITECH_VID
        && info_temp->product_id       == info->product_id          // NON-constant
        && info_temp->interface_number == info->interface_number    // constant 1
        && info_temp->usage_page       == info->usage_page)         // constant 0xFF43
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
        LogitechG810Controller*     controller     = new LogitechG810Controller(dev_usage_0x0602, dev_usage_0x0604);
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
        LogitechG810Controller*     controller     = new LogitechG810Controller(dev, dev);
        RGBController_LogitechG810* rgb_controller = new RGBController_LogitechG810(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
#endif
}

void DetectLogitechKeyboardG910(hid_device_info* info, const std::string& name)
{
    /*-------------------------------------------------------------------------------------------------*\
    | Logitech keyboards use two different usages, one for 20-byte packets and one for 64-byte packets  |
    | Usage 0x0602 for 20 byte, usage 0x0604 for 64 byte, both are on usage page 0xFF43                 |
    \*-------------------------------------------------------------------------------------------------*/
#ifdef USE_HID_USAGE
    hid_device* dev_usage_0x0602 = nullptr;
    hid_device* dev_usage_0x0604 = nullptr;
    hid_device_info* info_temp = info;

    while(info_temp)
    {
        if(info_temp->vendor_id        == info->vendor_id           // constant LOGITECH_VID
        && info_temp->product_id       == info->product_id          // NON-constant
        && info_temp->interface_number == info->interface_number    // constant 1
        && info_temp->usage_page       == info->usage_page)         // constant 0xFF43
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
        LogitechG910Controller*     controller     = new LogitechG910Controller(dev_usage_0x0602, dev_usage_0x0604);
        RGBController_LogitechG910* rgb_controller = new RGBController_LogitechG910(controller);
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
        LogitechG910Controller*     controller     = new LogitechG910Controller(dev, dev);
        RGBController_LogitechG910* rgb_controller = new RGBController_LogitechG910(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
#endif
}

void DetectLogitechKeyboardG815(hid_device_info* info, const std::string& name)
{
    /*-------------------------------------------------------------------------------------------------*\
    | Logitech keyboards use two different usages, one for 20-byte packets and one for 64-byte packets  |
    | Usage 0x0602 for 20 byte, usage 0x0604 for 64 byte, both are on usage page 0xFF43                 |
    \*-------------------------------------------------------------------------------------------------*/
#ifdef USE_HID_USAGE
    hid_device* dev_usage_0x0602 = nullptr;
    hid_device* dev_usage_0x0604 = nullptr;
    hid_device_info* info_temp = info;

    while(info_temp)
    {
        if(info_temp->vendor_id        == info->vendor_id           // constant LOGITECH_VID
        && info_temp->product_id       == info->product_id          // NON-constant
        && info_temp->interface_number == info->interface_number    // constant 1
        && info_temp->usage_page       == info->usage_page)         // constant 0xFF43
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
        LogitechG815Controller*     controller     = new LogitechG815Controller(dev_usage_0x0602, dev_usage_0x0604);
        RGBController_LogitechG815* rgb_controller = new RGBController_LogitechG815(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
    else
    {
        /*-------------------------------------------------*\
        | Not all of them could be opened, do some cleanup  |
        \*-------------------------------------------------*/
        if(dev_usage_0x0602)
        {
            hid_close(dev_usage_0x0602);
        }
        if(dev_usage_0x0604)
        {
            hid_close(dev_usage_0x0604);
        }
    }
#else
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LogitechG815Controller*     controller     = new LogitechG815Controller(dev, dev);
        RGBController_LogitechG815* rgb_controller = new RGBController_LogitechG815(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
#endif
}

/*-----------------------------------------------------*\
| Logitech Mice                                         |
\*-----------------------------------------------------*/
static void addLogitechLightsyncMouse1zone(hid_device_info* info, const std::string& name, unsigned char hid_dev_index, unsigned char hid_feature_index, unsigned char hid_fctn_ase_id)
{
#ifdef USE_HID_USAGE
    {
        hid_device* dev_usage_1 = nullptr;
        hid_device* dev_usage_2 = nullptr;
        hid_device_info* info_temp = info;

        while(info_temp)
        {
            if(info_temp->vendor_id        == info->vendor_id           // constant LOGITECH_VID
            && info_temp->product_id       == info->product_id          // NON-constant
            && info_temp->interface_number == info->interface_number    // constant 1
            && info_temp->usage_page       == info->usage_page)         // constant 0x00FF
            {
                if (info_temp->usage == 1)
                {
                    dev_usage_1 = hid_open_path(info_temp->path);
                }
                else if (info_temp->usage == 2)
                {
                    dev_usage_2 = hid_open_path(info_temp->path);
                }
            }
            if (dev_usage_1 && dev_usage_2)
            {
                break;
            }
            info_temp = info_temp->next;
        }
        if(dev_usage_1 && dev_usage_2)
        {
            LogitechGLightsyncController*          controller     = new LogitechGLightsyncController(dev_usage_1, dev_usage_2, info->path, hid_dev_index, hid_feature_index, hid_fctn_ase_id);
            RGBController_LogitechGLightsync1zone* rgb_controller = new RGBController_LogitechGLightsync1zone (controller);
            rgb_controller->name = name;
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            LOG_NOTICE("Unable to open all device report endpoints, unable to add device");
            hid_close(dev_usage_1);
            hid_close(dev_usage_2);
        }
    }

#else
    {
        hid_device* dev = hid_open_path(info->path);

        if(dev)
        {
            LogitechGLightsyncController*          controller     = new LogitechGLightsyncController(dev, dev, info->path, hid_dev_index, hid_feature_index, hid_fctn_ase_id);
            RGBController_LogitechGLightsync1zone* rgb_controller = new RGBController_LogitechGLightsync1zone(controller);
            rgb_controller->name = name;
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
#endif
}

static void addLogitechLightsyncMouse2zone(hid_device_info* info, const std::string& name, unsigned char hid_dev_index, unsigned char hid_feature_index, unsigned char hid_fctn_ase_id)
{
#ifdef USE_HID_USAGE
    {
        hid_device* dev_usage_1 = nullptr;
        hid_device* dev_usage_2 = nullptr;
        hid_device_info* info_temp = info;

        while(info_temp)
        {
            if(info_temp->vendor_id        == info->vendor_id           // constant LOGITECH_VID
            && info_temp->product_id       == info->product_id          // NON-constant
            && info_temp->interface_number == info->interface_number    // constant 1
            && info_temp->usage_page       == info->usage_page)         // constant 0x00FF
            {
                if(info_temp->usage == 1)
                {
                    dev_usage_1 = hid_open_path(info_temp->path);
                }
                else if(info_temp->usage == 2)
                {
                    dev_usage_2 = hid_open_path(info_temp->path);
                }
            }
            if(dev_usage_1 && dev_usage_2)
            {
                break;
            }
            info_temp = info_temp->next;
        }
        if(dev_usage_1 && dev_usage_2)
        {
            LogitechGLightsyncController*     controller     = new LogitechGLightsyncController(dev_usage_1, dev_usage_2, info->path, hid_dev_index, hid_feature_index, hid_fctn_ase_id);
            RGBController_LogitechGLightsync* rgb_controller = new RGBController_LogitechGLightsync (controller);
            rgb_controller->name = name;
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            LOG_NOTICE("Unable to open all device report endpoints, unable to add device");
            hid_close(dev_usage_1);
            hid_close(dev_usage_2);
        }
    }
#else
    {
        hid_device* dev = hid_open_path(info->path);
    
        if(dev)
        {
            LogitechGLightsyncController*     controller     = new LogitechGLightsyncController(dev, dev, info->path, hid_dev_index, hid_feature_index, hid_fctn_ase_id);
            RGBController_LogitechGLightsync* rgb_controller = new RGBController_LogitechGLightsync(controller);
            rgb_controller->name = name;
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
#endif
}

void DetectLogitechMouseG203(hid_device_info* info, const std::string& name)
{
    addLogitechLightsyncMouse1zone(info, name, 0xFF, 0x0E, 0x3A);
}

void DetectLogitechMouseG203L(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LogitechG203LController*     controller     = new LogitechG203LController(dev, info->path);
        RGBController_LogitechG203L* rgb_controller = new RGBController_LogitechG203L(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectLogitechMouseG303(hid_device_info* info, const std::string& name)
{
    addLogitechLightsyncMouse2zone(info, name, 0xFF, 0x0E, 0x3A);
}

void DetectLogitechMouseG403(hid_device_info* info, const std::string& name)
{
    addLogitechLightsyncMouse2zone(info, name, 0xFF, 0x0E, 0x3A);
}

void DetectLogitechMouseG502PS(hid_device_info* info, const std::string& name)
{
    addLogitechLightsyncMouse2zone(info, name, 0xFF, 0x02, 0x3A);
}

void DetectLogitechMouseGPRO(hid_device_info* info, const std::string& name)
{
    addLogitechLightsyncMouse1zone(info, name, 0xFF, 0x0E, 0x3C);
}

/*-----------------------------------------------------*\
| Other Logitech Devices                                |
\*-----------------------------------------------------*/
void DetectLogitechG560(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        /*---------------------------------------------*\
        | Add G560 Speaker                              |
        \*---------------------------------------------*/
        LogitechG560Controller*     controller     = new LogitechG560Controller(dev, info->path);
        RGBController_LogitechG560* rgb_controller = new RGBController_LogitechG560(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectLogitechX56(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        /*---------------------------------------------*\
        | Add X56 Devices                               |
        \*---------------------------------------------*/
        LogitechX56Controller*     controller     = new LogitechX56Controller(dev, info->path);
        RGBController_LogitechX56* rgb_controller = new RGBController_LogitechX56(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                                                                         |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech G213",                                  DetectLogitechKeyboardG213, LOGITECH_VID, LOGITECH_G213_PID,                    1, 0xFF43, 0x0602);
REGISTER_HID_DETECTOR_IP ("Logitech G512",                                  DetectLogitechKeyboardG810, LOGITECH_VID, LOGITECH_G512_PID,                    1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G512 RGB",                              DetectLogitechKeyboardG810, LOGITECH_VID, LOGITECH_G512_RGB_PID,                1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G610 Orion",                            DetectLogitechKeyboardG810, LOGITECH_VID, LOGITECH_G610_1_PID,                  1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G610 Orion",                            DetectLogitechKeyboardG810, LOGITECH_VID, LOGITECH_G610_2_PID,                  1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G810 Orion Spectrum",                   DetectLogitechKeyboardG810, LOGITECH_VID, LOGITECH_G810_1_PID,                  1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G810 Orion Spectrum",                   DetectLogitechKeyboardG810, LOGITECH_VID, LOGITECH_G810_2_PID,                  1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G813 RGB Mechanical Gaming Keyboard",   DetectLogitechKeyboardG815, LOGITECH_VID, LOGITECH_G813_PID,                    1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G815 RGB Mechanical Gaming Keyboard",   DetectLogitechKeyboardG815, LOGITECH_VID, LOGITECH_G815_PID,                    1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G910 Orion Spark",                      DetectLogitechKeyboardG910, LOGITECH_VID, LOGITECH_G910_ORION_SPARK_PID,        1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G910 Orion Spectrum",                   DetectLogitechKeyboardG910, LOGITECH_VID, LOGITECH_G910_PID,                    1, 0xFF43);
/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Mice                                                                                                                                              |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP ("Logitech G203 Prodigy",                          DetectLogitechMouseG203,    LOGITECH_VID, LOGITECH_G203_PID,                    1, 0xFF00);
REGISTER_HID_DETECTOR_IPU("Logitech G203 Lightsync",                        DetectLogitechMouseG203L,   LOGITECH_VID, LOGITECH_G203_LIGHTSYNC_PID,          1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IP ("Logitech G303 Daedalus Apex",                    DetectLogitechMouseG303,    LOGITECH_VID, LOGITECH_G303_PID,                    1, 0xFF00);
REGISTER_HID_DETECTOR_IP ("Logitech G403 Hero",                             DetectLogitechMouseG403,    LOGITECH_VID, LOGITECH_G403_HERO_PID,               1, 0xFF00);
REGISTER_HID_DETECTOR_IP ("Logitech G502 Proteus Spectrum Gaming Mouse",    DetectLogitechMouseG502PS,  LOGITECH_VID, LOGITECH_G502_PROTEUS_SPECTRUM_PID,   1, 0xFF00);
REGISTER_HID_DETECTOR_IP ("Logitech G502 Hero Gaming Mouse",                DetectLogitechMouseG502PS,  LOGITECH_VID, LOGITECH_G502_HERO_PID,               1, 0xFF00);
REGISTER_HID_DETECTOR_IP ("Logitech G Pro Gaming Mouse",                    DetectLogitechMouseGPRO,    LOGITECH_VID, LOGITECH_G_PRO_PID,                   1, 0xFF00);
REGISTER_HID_DETECTOR_IP ("Logitech G Pro (HERO) Gaming Mouse",             DetectLogitechMouseGPRO,    LOGITECH_VID, LOGITECH_G_PRO_HERO_PID,              1, 0xFF00);
/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Speakers                                                                                                                                         |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech G560 Lightsync Speaker",                DetectLogitechG560,         LOGITECH_VID, LOGITECH_G560_PID,                    2, 0xFF43, 514);
/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Joysticks                                                                                                                                         |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Logitech X56 Rhino Hotas Joystick",               DetectLogitechX56,          LOGITECH_X56_VID, LOGITECH_X56_JOYSTICK_PID,        2, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech X56 Rhino Hotas Throttle",               DetectLogitechX56,          LOGITECH_X56_VID, LOGITECH_X56_THROTTLE_PID,        2, 0xFF00);

/*---------------------------------------------------------------------------------------------------------*\
| Windows and MacOS Lightspeed Detection                                                                    |
|                                                                                                           |
| The Lightspeed receiver is a unifying receiver that will only accept 1 connection                         |
| We must probe the receiver to check what is currently connected                                           |
|                                                                                                           |
| Hat tip - kernel driver  https://github.com/torvalds/linux/blob/master/drivers/hid/hid-logitech-dj.c      |
|         - ltunify        https://github.com/Lekensteyn/ltunify/                                           |
\*---------------------------------------------------------------------------------------------------------*/
#if defined(_WIN32) || defined(__APPLE__)

usages BundleLogitechUsages(hid_device_info* info)
{
    usages      temp_usages;

    /*-----------------------------------------------------------------*\
    | Need a unique ID to group usages for 1 device if multiple exist   |
    |   Grab all usages that you can open. For normal Logitech FAP      |
    |   devices this will be usage 1, 2 and 4                           |
    \*-----------------------------------------------------------------*/
    if(info->usage == 1)
    {
        while(info)
        {
            hid_device* dev = hid_open_path(info->path);

            if(dev)
            {
                temp_usages.emplace((uint8_t)info->usage, dev);
            }

            info = info->next;
        }
    }

    return temp_usages;
}

void CreateLogitechLightspeedDevice(char *path, usages device_usages, uint8_t device_index, bool wireless, std::shared_ptr<std::mutex> mutex_ptr)
{
    LogitechLightspeedController* controller          = new LogitechLightspeedController(device_usages.find(2)->second, path);
    controller->lightspeed                              = new logitech_device(path, device_usages, device_index, wireless, mutex_ptr);
    RGBController_LogitechLightspeed* rgb_controller  = new RGBController_LogitechLightspeed(controller);
    ResourceManager::get()->RegisterRGBController(rgb_controller);
}

void DetectLogitechLightspeedReceiver(hid_device_info* info, const std::string& /*name*/)
{
    /*-----------------------------------------------------------------*\
    | Need to save the PID and the device path before iterating         |
    |    over "info" in BundleLogitechUsages()                          |
    \*-----------------------------------------------------------------*/
    char        *path           = info->path;
    uint16_t    dev_pid         = info->product_id;
    usages      device_usages   = BundleLogitechUsages(info);

    wireless_map wireless_devices;
    unsigned int device_count   = getWirelessDevice(device_usages, dev_pid, &wireless_devices);

    /*-----------------------------------------------------------------*\
    | Lightspeed Receivers will only have one paired /connected device  |
    | Unifying Receivers can have up to 6 devices paired / connected    |
    \*-----------------------------------------------------------------*/
    if(device_count > 0)
    {
        /*-------------------------------------------------*\
        | Create mutex to prevent the controllers sharing a |
        |   receiver from interfering with each other       |
        \*-------------------------------------------------*/
        std::shared_ptr<std::mutex>       logitech_mutex = std::make_shared<std::mutex>();

        for(wireless_map::iterator wd = wireless_devices.begin(); wd != wireless_devices.end(); wd++)
        {
            CreateLogitechLightspeedDevice(path, device_usages, wd->second, true, logitech_mutex);
        }
    }
}

void DetectLogitechWired(hid_device_info* info, const std::string& /*name*/)
{
    /*-----------------------------------------------------------------*\
    | Need to save the device path before iterating                     |
    |    over "info" in BundleLogitechUsages()                          |
    \*-----------------------------------------------------------------*/
    char        *path           = info->path;
    usages      device_usages   = BundleLogitechUsages(info);

    if(device_usages.size() > 0)
    {
        CreateLogitechLightspeedDevice(path, device_usages, 0xFF, false, nullptr);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Lightspeed Receivers (Windows Wireless)                                                                                                           |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Logitech Lightspeed Receiver",                        DetectLogitechLightspeedReceiver,   LOGITECH_VID, LOGITECH_G_LIGHTSPEED_RECEIVER_PID,   2, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G Powerplay Mousepad",                       DetectLogitechLightspeedReceiver,   LOGITECH_VID, LOGITECH_G_LIGHTSPEED_POWERPLAY_PID,  2, 0xFF00);

/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Lightspeed Wireless Devices (Windows Wired)                                                                                                       |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Logitech G403 Wireless Gaming Mouse (wired)",         DetectLogitechWired,                LOGITECH_VID, LOGITECH_G403_LIGHTSPEED_PID,         1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G502 Wireless Gaming Mouse (wired)",         DetectLogitechWired,                LOGITECH_VID, LOGITECH_G502_LIGHTSPEED_PID,         1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G703 Wireless Gaming Mouse (wired)",         DetectLogitechWired,                LOGITECH_VID, LOGITECH_G703_LIGHTSPEED_PID,         1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G900 Wireless Gaming Mouse (wired)",         DetectLogitechWired,                LOGITECH_VID, LOGITECH_G900_LIGHTSPEED_PID,         1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G903 Wireless Gaming Mouse (wired)",         DetectLogitechWired,                LOGITECH_VID, LOGITECH_G903_LIGHTSPEED_PID,         1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G Pro Wireless Gaming Mouse (wired)",        DetectLogitechWired,                LOGITECH_VID, LOGITECH_G_PRO_WIRELESS_PID,          2, 0xFF00);

#endif

/*---------------------------------------------------------------------------------------------------------*\
| Linux Lightspeed Detection                                                                                |
|                                                                                                           |
| The Linux kernel handles detecting wireless devices connected to a Unifying Receiver.                     |
\*---------------------------------------------------------------------------------------------------------*/
#ifdef __linux__

void CreateLogitechLightspeedDevice(hid_device_info* info, bool wireless)
{
    usages      device_usages;

    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        device_usages.emplace((uint8_t)info->usage, dev);
    }

    if(device_usages.size() > 0)
    {
        LogitechLightspeedController* controller          = new LogitechLightspeedController(device_usages.find(0)->second, info->path);
        controller->lightspeed                              = new logitech_device(info->path, device_usages, 0xFF, wireless);
        RGBController_LogitechLightspeed* rgb_controller  = new RGBController_LogitechLightspeed(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectLogitechWireless(hid_device_info* info, const std::string& /*name*/)
{
    CreateLogitechLightspeedDevice(info, true);
}

void DetectLogitechWired(hid_device_info* info, const std::string& /*name*/)
{
    CreateLogitechLightspeedDevice(info, false);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Lightspeed Devices (Linux Wireless)                                                                                                               |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech G403 Wireless Gaming Mouse",                DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G403_LIGHTSPEED_VIRTUAL_PID, 2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G502 Wireless Gaming Mouse",                DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G502_LIGHTSPEED_VIRTUAL_PID, 2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G703 Wireless Gaming Mouse",                DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G703_LIGHTSPEED_VIRTUAL_PID, 2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G900 Wireless Gaming Mouse",                DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G900_LIGHTSPEED_VIRTUAL_PID, 2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G903 Wireless Gaming Mouse",                DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G903_LIGHTSPEED_VIRTUAL_PID, 2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G Pro Wireless Gaming Mouse",               DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G_PRO_WIRELESS_VIRTUAL_PID,  2, 0xFF00, 2);

/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Lightspeed Wireless Devices (Linux Wired)                                                                                                          |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech G403 Wireless Gaming Mouse (wired)",        DetectLogitechWired,        LOGITECH_VID, LOGITECH_G403_LIGHTSPEED_PID,         1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G502 Wireless Gaming Mouse (wired)",        DetectLogitechWired,        LOGITECH_VID, LOGITECH_G502_LIGHTSPEED_PID,         1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G703 Wireless Gaming Mouse (wired)",        DetectLogitechWired,        LOGITECH_VID, LOGITECH_G703_LIGHTSPEED_PID,         1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G900 Wireless Gaming Mouse (wired)",        DetectLogitechWired,        LOGITECH_VID, LOGITECH_G900_LIGHTSPEED_PID,         1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G903 Wireless Gaming Mouse (wired)",        DetectLogitechWired,        LOGITECH_VID, LOGITECH_G903_LIGHTSPEED_PID,         1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G Pro Wireless Gaming Mouse (wired)",       DetectLogitechWired,        LOGITECH_VID, LOGITECH_G_PRO_WIRELESS_PID,          2, 0xFF00, 2);

#endif
