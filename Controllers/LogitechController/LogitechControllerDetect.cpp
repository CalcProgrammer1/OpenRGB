#include "Detector.h"
#include "LogitechG203LController.h"
#include "LogitechG213Controller.h"
#include "LogitechG560Controller.h"
#include "LogitechG810Controller.h"
#include "LogitechG910Controller.h"
#include "LogitechG815Controller.h"
#include "LogitechGLightsyncController.h"
#include "RGBController.h"
#include "RGBController_LogitechG203L.h"
#include "RGBController_LogitechG213.h"
#include "RGBController_LogitechG560.h"
#include "RGBController_LogitechG810.h"
#include "RGBController_LogitechG910.h"
#include "RGBController_LogitechG815.h"
#include "RGBController_LogitechGLightsync.h"
#include "RGBController_LogitechGLightsync1zone.h"
#include "RGBController_LogitechGPowerPlay.h"
#include <vector>
#include <hidapi/hidapi.h>
#include <LogManager.h>

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
#define LOGITECH_G203L_PID                      0xC092
#define LOGITECH_G303_PID                       0xC080
#define LOGITECH_G403_PID                       0xC083
#define LOGITECH_G403H_PID                      0xC08F
#define LOGITECH_G403_WIRELESS_PID              0xC082
#define LOGITECH_G403_WIRELESS_VIRTUAL_PID      0x405D
#define LOGITECH_G502_PS_PID                    0xC332
#define LOGITECH_G502H_PID                      0xC08B
#define LOGITECH_G502_WIRELESS_PID              0xC08D
#define LOGITECH_G502_WIRELESS_VIRTUAL_PID      0x407F
#define LOGITECH_G703_WIRELESS_PID              0xC087
#define LOGITECH_G703_WIRELESS_VIRTUAL_PID      0x4070
#define LOGITECH_G900_WIRELESS_PID              0xC081
#define LOGITECH_G900_WIRELESS_VIRTUAL_PID      0x4053
#define LOGITECH_G903_WIRELESS_PID              0xC086
#define LOGITECH_G903_WIRELESS_VIRTUAL_PID      0x4067
#define LOGITECH_G_LIGHTSPEED_WIRELESS_PID      0xC539
#define LOGITECH_GPRO_WIRED_PID                 0xC085
#define LOGITECH_GPRO_HERO_WIRED_PID            0xC08C
#define LOGITECH_GPRO_WIRELESS_PID              0xC088
#define LOGITECH_GPRO_WIRELESS_VIRTUAL_PID      0x4079

/*-----------------------------------------------------*\
| Mousemat product IDs                                  |
\*-----------------------------------------------------*/
#define LOGITECH_G_LIGHTSPEED_POWERPLAY_PID     0xC53A

/*-----------------------------------------------------*\
| Speaker product IDs                                   |
\*-----------------------------------------------------*/
#define LOGITECH_G560_PID                       0x0A78

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

void DetectLogitechMouseG403WW(hid_device_info* info, const std::string& name)
{
    addLogitechLightsyncMouse2zone(info, name, 0xFF, 0x18, 0x3A);
}

void DetectLogitechMouseG502PS(hid_device_info* info, const std::string& name)
{
    addLogitechLightsyncMouse2zone(info, name, 0xFF, 0x02, 0x3A);
}

void DetectLogitechMouseG502WW(hid_device_info* info, const std::string& name)
{
    addLogitechLightsyncMouse2zone(info, name, 0xFF, 0x07, 0x3A);
}

void DetectLogitechMouseG703WW(hid_device_info* info, const std::string& name)
{
    addLogitechLightsyncMouse2zone(info, name, 0x01, 0x18, 0x3C);
}

void DetectLogitechMouseG900WW(hid_device_info* info, const std::string& name)
{
    addLogitechLightsyncMouse2zone(info, name, 0x01, 0x17, 0x3A);
}

void DetectLogitechMouseG903WW(hid_device_info* info, const std::string& name)
{
    addLogitechLightsyncMouse2zone(info, name, 0x01, 0x17, 0x3A);
}

void DetectLogitechMouseGPRO(hid_device_info* info, const std::string& name)
{
    addLogitechLightsyncMouse2zone(info, name, 0x01, 0x07, 0x3C);
}

void DetectLogitechMouseGLS(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        /*---------------------------------------------*\
        | Create mutex to prevent the two controllers   |
        | from interfering with each other              |
        \*---------------------------------------------*/
        std::shared_ptr<std::mutex>       logitech_mutex = std::make_shared<std::mutex>();

        /*---------------------------------------------*\
        | Add mouse                                     |
        \*---------------------------------------------*/
        LogitechGLightsyncController*     controller     = new LogitechGLightsyncController(dev, dev, info->path, 0x01, 0x07, 0x3C, logitech_mutex);
        RGBController_LogitechGLightsync* rgb_controller = new RGBController_LogitechGLightsync(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);

        /*---------------------------------------------*\
        | Add Powerplay mousemat                        |
        \*---------------------------------------------*/
        LogitechGLightsyncController*     mousemat_controller     = new LogitechGLightsyncController(dev, dev, info->path, 0x07, 0x0B, 0x3C, logitech_mutex);
        RGBController_LogitechGPowerPlay* mousemat_rgb_controller = new RGBController_LogitechGPowerPlay(mousemat_controller);
        mousemat_rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(mousemat_rgb_controller);
    }
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
REGISTER_HID_DETECTOR_IP("Logitech G203 Prodigy",                           DetectLogitechMouseG203,    LOGITECH_VID, LOGITECH_G203_PID,                    1, 0xFF00);
REGISTER_HID_DETECTOR_IPU("Logitech G203 Lightsync",                        DetectLogitechMouseG203L,   LOGITECH_VID, LOGITECH_G203L_PID,                   1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IP("Logitech G303 Daedalus Apex",                     DetectLogitechMouseG303,    LOGITECH_VID, LOGITECH_G303_PID,                    1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G403 Prodigy",                           DetectLogitechMouseG403,    LOGITECH_VID, LOGITECH_G403_PID,                    1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G403 Hero",                              DetectLogitechMouseG403,    LOGITECH_VID, LOGITECH_G403H_PID,                   1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G403 Prodigy Wireless (wired)",          DetectLogitechMouseG403WW,  LOGITECH_VID, LOGITECH_G403_WIRELESS_PID,           1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G502 Proteus Spectrum",                  DetectLogitechMouseG502PS,  LOGITECH_VID, LOGITECH_G502_PS_PID,                 1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G502 Hero",                              DetectLogitechMouseG502PS,  LOGITECH_VID, LOGITECH_G502H_PID,                   1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G502 LIGHTSPEED Wireless (wired)",       DetectLogitechMouseG502WW,  LOGITECH_VID, LOGITECH_G502_WIRELESS_PID,           1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G703 Wireless (wired)",                  DetectLogitechMouseG703WW,  LOGITECH_VID, LOGITECH_G703_WIRELESS_PID,           1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G900 Chaos Spectrum Wireless (wired)",   DetectLogitechMouseG900WW,  LOGITECH_VID, LOGITECH_G900_WIRELESS_PID,           1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G903 LIGHTSPEED Wireless (wired)",       DetectLogitechMouseG903WW,  LOGITECH_VID, LOGITECH_G903_WIRELESS_PID,           1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G Pro Gaming Mouse",                     DetectLogitechMouseG203,    LOGITECH_VID, LOGITECH_GPRO_WIRED_PID,              1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G Pro (HERO) Gaming Mouse",              DetectLogitechMouseG203,    LOGITECH_VID, LOGITECH_GPRO_HERO_WIRED_PID,         1, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G Lightspeed Wireless Gaming Mouse",     DetectLogitechMouseGPRO,    LOGITECH_VID, LOGITECH_G_LIGHTSPEED_WIRELESS_PID,   2, 0xFF00);
REGISTER_HID_DETECTOR_IP("Logitech G Pro Wireless Gaming Mouse (Wired)",    DetectLogitechMouseGPRO,    LOGITECH_VID, LOGITECH_GPRO_WIRELESS_PID,           2, 0xFF00);
/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Mousemats                                                                                                                                         |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech G Powerplay Mousepad with Lightspeed",  DetectLogitechMouseGLS,     LOGITECH_VID, LOGITECH_G_LIGHTSPEED_POWERPLAY_PID,  2, 0xFF00, 2);
/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Speakers                                                                                                                                         |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech G560 Lightsync Speaker",                DetectLogitechG560,         LOGITECH_VID, LOGITECH_G560_PID,                    2, 0xFF43, 514);
