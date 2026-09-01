/*---------------------------------------------------------*\
| LogitechControllerDetect.cpp                              |
|                                                           |
|   Detector for Logitech devices                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <map>
#include <set>
#include <thread>
#include <hidapi.h>
#include "DetectionManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "LogitechProtocolCommon.h"
#include "LogitechG203LController.h"
#include "LogitechG213Controller.h"
#include "LogitechG600Controller.h"
#include "LogitechG933Controller.h"
#include "LogitechG815Controller.h"
#include "LogitechG915Controller.h"
#include "LogitechGLightsyncController.h"
#include "LogitechLightspeedController.h"
#include "LogitechX56Controller.h"
#include "RGBController_LogitechG203L.h"
#include "RGBController_LogitechG213.h"
#include "RGBController_LogitechG600.h"
#include "RGBController_LogitechG933.h"
#include "RGBController_LogitechG815.h"
#include "RGBController_LogitechG915.h"
#include "RGBController_LogitechGLightsync.h"
#include "RGBController_LogitechGLightsync1zone.h"
#include "RGBController_LogitechLightspeed.h"
#include "RGBController_LogitechGPowerPlay.h" // Linux-only
#include "RGBController_LogitechX56.h"
#include "LogitechHIDPP20Controller.h"
#include "LogitechHIDPP20ReceiverWatcher.h"
#include "RGBController_LogitechHIDPP20.h"
#include "StringUtils.h"

#include <memory>

using namespace std::chrono_literals;

/*---------------------------------------------------------*\
| Logitech vendor ID                                        |
\*---------------------------------------------------------*/
#define LOGITECH_VID                                0x046D
#define LOGITECH_LIGHTSPEED_DETECT_MAX_RETRY        10

/*---------------------------------------------------------*\
| Keyboard product IDs                                      |
\*---------------------------------------------------------*/
#define LOGITECH_G213_PID                           0xC336
#define LOGITECH_G813_PID                           0xC232
#define LOGITECH_G815_PID                           0xC33F
#define LOGITECH_G915_WIRED_PID                     0xC33E
#define LOGITECH_G915_RECEIVER_PID                  0xC541
#define LOGITECH_G915_RECEIVER_2_PID                0xC547
#define LOGITECH_G915TKL_WIRED_PID                  0xC343
#define LOGITECH_G915TKL_RECEIVER_PID               0xC545

/*---------------------------------------------------------*\
| Mouse product IDs                                         |
\*---------------------------------------------------------*/
#define LOGITECH_G203_PID                           0xC084
#define LOGITECH_G203_LIGHTSYNC_PID                 0xC092
#define LOGITECH_G203_LIGHTSYNC_PID_2               0xC09D
#define LOGITECH_G303_PID                           0xC080
#define LOGITECH_G403_PID                           0xC083
#define LOGITECH_G403_HERO_PID                      0xC08F
#define LOGITECH_G403_LIGHTSPEED_PID                0xC082
#define LOGITECH_G502_PROTEUS_SPECTRUM_PID          0xC332
#define LOGITECH_G502_HERO_PID                      0xC08B
#define LOGITECH_G502_LIGHTSPEED_PID                0xC08D
#define LOGITECH_G502_X_PLUS_PID                    0xC095
#define LOGITECH_G515_LS_TKL_PID                    0xC355
#define LOGITECH_G522_LIGHTSPEED_USB_PID            0x0B19
#define LOGITECH_G522_LIGHTSPEED_DONGLE_PID         0x0B18
#define LOGITECH_G600_PID                           0xC24A
#define LOGITECH_G703_LIGHTSPEED_PID                0xC087
#define LOGITECH_G703_HERO_LIGHTSPEED_PID           0xC090
#define LOGITECH_G900_LIGHTSPEED_PID                0xC081
#define LOGITECH_G903_LIGHTSPEED_PID                0xC086
#define LOGITECH_G903_LIGHTSPEED_HERO_PID           0xC091
#define LOGITECH_G_PRO_PID                          0xC085
#define LOGITECH_G_PRO_HERO_PID                     0xC08C
#define LOGITECH_G_PRO_WIRELESS_PID                 0xC088

/*---------------------------------------------------------*\
| Mousemat product IDs                                      |
\*---------------------------------------------------------*/
#define LOGITECH_G_LIGHTSPEED_POWERPLAY_PID         0xC53A

/*---------------------------------------------------------*\
| Headset product IDs                                       |
\*---------------------------------------------------------*/
#define LOGITECH_G633_PID                           0x0A5C
#define LOGITECH_G635_PID                           0x0A89
#define LOGITECH_G733_PID                           0x0AB5
#define LOGITECH_G733_2_PID                         0x0AFE
#define LOGITECH_G733_3_PID                         0x0B1F
#define LOGITECH_G933_PID                           0x0A5B
#define LOGITECH_G935_PID                           0x0A87

/*---------------------------------------------------------*\
| Unifying Device IDs (Including Lightspeed receivers)      |
|   NB: Not used but preserved for debugging                |
\*---------------------------------------------------------*/
#define LOGITECH_G_UNIFYING_RECEIVER_1_PID          0xC52B
#define LOGITECH_G_NANO_RECEIVER_PID                0xC52F
#define LOGITECH_G_G700_RECEIVER_PID                0xC531
#define LOGITECH_G_UNIFYING_RECEIVER_2_PID          0xC532
#define LOGITECH_G_G602_RECEIVER_PID                0xC537

#define LOGITECH_G_LIGHTSPEED_RECEIVER_PID          0xC539
#define LOGITECH_G403_LIGHTSPEED_VIRTUAL_PID        0x405D
#define LOGITECH_G502_LIGHTSPEED_VIRTUAL_PID        0x407F
#define LOGITECH_G703_LIGHTSPEED_VIRTUAL_PID        0x4070
#define LOGITECH_G703_HERO_LIGHTSPEED_VIRTUAL_PID   0x4086
#define LOGITECH_G900_LIGHTSPEED_VIRTUAL_PID        0x4053
#define LOGITECH_G903_LIGHTSPEED_VIRTUAL_PID        0x4067
#define LOGITECH_G903_LIGHTSPEED_VIRTUAL_HERO_PID   0x4087
#define LOGITECH_G_PRO_WIRELESS_VIRTUAL_PID         0x4079
#define LOGITECH_POWERPLAY_MAT_VIRTUAL_PID          0x405F
#define LOGITECH_G502_X_PLUS_LIGHTSPEED_VIRTUAL_PID 0x4099
#define LOGITECH_G515_LS_TKL_LIGHTSPEED_VIRTUAL_PID 0x40B4

/*---------------------------------------------------------*\
| Joystick product IDs                                      |
\*---------------------------------------------------------*/
#define LOGITECH_X56_VID                            0x0738
#define LOGITECH_X56_JOYSTICK_PID                   0x2221
#define LOGITECH_X56_THROTTLE_PID                   0xA221

/*---------------------------------------------------------*\
| Logitech Keyboards                                        |
\*---------------------------------------------------------*/
DetectedControllers DetectLogitechKeyboardG213(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        LogitechG213Controller*     controller     = new LogitechG213Controller(dev, info->path, name);
        RGBController_LogitechG213* rgb_controller = new RGBController_LogitechG213(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectLogitechKeyboardG815(hid_device_info* info, const std::string& name)
{
    /*-----------------------------------------------------*\
    | Logitech keyboards use two different usages, one for  |
    | 20-byte packets and one for 64-byte packets.  Usage   |
    | 0x0602 for 20 byte, usage 0x0604 for 64 byte, both    |
    | are on usage page 0xFF43                              |
    \*-----------------------------------------------------*/
    DetectedControllers detected_controllers;

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
        LogitechG815Controller*     controller     = new LogitechG815Controller(dev_usage_0x0602, dev_usage_0x0604, name);
        RGBController_LogitechG815* rgb_controller = new RGBController_LogitechG815(controller);

        detected_controllers.push_back(rgb_controller);
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

    return(detected_controllers);
}

DetectedControllers DetectLogitechKeyboardG915(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;
    bool                is_tkl = info->product_id == LOGITECH_G915TKL_RECEIVER_PID;

    dev = hid_open_path(info->path);

    if(dev)
    {
        LogitechG915Controller*     controller     = new LogitechG915Controller(dev, false, name);
        RGBController_LogitechG915* rgb_controller = new RGBController_LogitechG915(controller, is_tkl);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

static bool ProbeG915ReceiverName(hid_device* dev, std::string& out_name)
{
    /*---------------------------------------------------------*\n    | HID++ short message name probe.                           |
    | Request:  10 01 03 0E 00 00 00  (get name length)         |
    | Request:  10 01 03 1E 00 00 00  (get name string)         |
    | Response: 11 01 03 1E <name bytes...>                     |
    | Verified against G915 TKL (PID 0xC547) which returns      |
    | "G915 TKL LIGHTSP" (truncated, full: G915 TKL LIGHTSPEED) |
    \*---------------------------------------------------------*/
    const unsigned char req_len[7]  = { 0x10, 0x01, 0x03, 0x0E, 0x00, 0x00, 0x00 };
    const unsigned char req_name[7] = { 0x10, 0x01, 0x03, 0x1E, 0x00, 0x00, 0x00 };
    unsigned char resp[64] = { 0 };

    hid_write(dev, req_len, sizeof(req_len));
    hid_read_timeout(dev, resp, sizeof(resp), 100);

    hid_write(dev, req_name, sizeof(req_name));
    for(int attempt = 0; attempt < 3; attempt++)
    {
        int rd = hid_read_timeout(dev, resp, sizeof(resp), 200);
        if(rd < 8)
        {
            continue;
        }
        if(resp[0] == 0x11 && resp[1] == 0x01 && resp[2] == 0x03 && resp[3] == 0x1E)
        {
            std::string name_str;
            for(int i = 4; i < rd; i++)
            {
                if(resp[i] == 0x00)
                {
                    break;
                }
                name_str.push_back(static_cast<char>(resp[i]));
            }
            out_name = name_str;
            return true;
        }
    }
    return false;
}

DetectedControllers DetectLogitechKeyboardG915Receiver2(hid_device_info* info, const std::string& name)
{
    /*---------------------------------------------------------*\
    | PID 0xC547 is shared by multiple Logitech keyboards.      |
    | Use a HID++ name probe to identify the exact device and   |
    | route to the correct controller.                          |
    |                                                           |
    | Known devices behind this PID:                            |
    |   "G915 TKL LIGHTSP..." -> G915 TKL (is_tkl = true)       |
    |   "G915 LIGHTSP..."     -> G915 full-size (is_tkl = false)|
    |   "G515..."             -> G515 (not handled here)        |
    \*---------------------------------------------------------*/
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);
    if(dev)
    {
        std::string probed_name;
        bool ok = ProbeG915ReceiverName(dev, probed_name);

        if(ok)
        {
            /*---------------------------------------------*\
            | Route on probed name. G915 X first, then      |
            | TKL before full G915 (all contain "G915").    |
            \*---------------------------------------------*/
            if(probed_name.find("G915 X") != std::string::npos)
            {
                /*-----------------------------------------*\
                | G915 X family: the unified HID++ 2.0      |
                | controller handles it. Skip so the C547   |
                | detector claims it.                       |
                \*-----------------------------------------*/
                LOG_DEBUG("[LogitechControllerDetect] 0xC547 G915 X -> unified controller, skipping legacy");
                hid_close(dev);
            }
            else if(probed_name.find("G915 TKL") != std::string::npos)
            {
                LogitechG915Controller*     controller     = new LogitechG915Controller(dev, false, name);
                RGBController_LogitechG915* rgb_controller = new RGBController_LogitechG915(controller, true);

                detected_controllers.push_back(rgb_controller);
            }
            else if(probed_name.find("G915") != std::string::npos)
            {
                LogitechG915Controller*     controller     = new LogitechG915Controller(dev, false, name);
                RGBController_LogitechG915* rgb_controller = new RGBController_LogitechG915(controller, false);

                detected_controllers.push_back(rgb_controller);
            }
            else
            {
                /*---------------------------------------------*\
                | Unknown device (e.g. G515 or future hardware).|
                | Close and leave it for another detector to    |
                | claim.                                        |
                \*---------------------------------------------*/
                LOG_DEBUG("[LogitechControllerDetect] 0xC547 unrecognised device name \"%s\", skipping", probed_name.c_str());
                hid_close(dev);
            }
        }
        else
        {
            LOG_DEBUG("[LogitechControllerDetect] 0xC547 name probe failed, skipping device");
            hid_close(dev);
        }

    LOG_DEBUG("[LogitechControllerDetect] 0xC547 probe returned name=\"%s\"", probed_name.c_str());
    }

    return(detected_controllers);
}

DetectedControllers DetectLogitechKeyboardG915Wired(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;
    bool                is_tkl = info->product_id == LOGITECH_G915TKL_WIRED_PID;

    dev = hid_open_path(info->path);

    if(dev)
    {
        LogitechG915Controller*     controller     = new LogitechG915Controller(dev, true, name);
        RGBController_LogitechG915* rgb_controller = new RGBController_LogitechG915(controller, is_tkl);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| Logitech Mice                                             |
\*---------------------------------------------------------*/
DetectedControllers addLogitechLightsyncMouse1zone(hid_device_info* info, const std::string& name, unsigned char hid_dev_index, unsigned char hid_feature_index, unsigned char hid_fctn_ase_id)
{
    DetectedControllers detected_controllers;

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
        LogitechGLightsyncController*          controller     = new LogitechGLightsyncController(dev_usage_1, dev_usage_2, info->path, hid_dev_index, hid_feature_index, hid_fctn_ase_id, name);
        RGBController_LogitechGLightsync1zone* rgb_controller = new RGBController_LogitechGLightsync1zone (controller);

        detected_controllers.push_back(rgb_controller);
    }
    else
    {
        LOG_INFO("Unable to open all device report endpoints, unable to add device");
        hid_close(dev_usage_1);
        hid_close(dev_usage_2);
    }

    return(detected_controllers);
}

DetectedControllers addLogitechLightsyncMouse2zone(hid_device_info* info, const std::string& name, unsigned char hid_dev_index, unsigned char hid_feature_index, unsigned char hid_fctn_ase_id)
{
    DetectedControllers detected_controllers;

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
        LogitechGLightsyncController*     controller     = new LogitechGLightsyncController(dev_usage_1, dev_usage_2, info->path, hid_dev_index, hid_feature_index, hid_fctn_ase_id, name);
        RGBController_LogitechGLightsync* rgb_controller = new RGBController_LogitechGLightsync (controller);

        detected_controllers.push_back(rgb_controller);
    }
    else
    {
        LOG_INFO("Unable to open all device report endpoints, unable to add device");
        hid_close(dev_usage_1);
        hid_close(dev_usage_2);
    }

    return(detected_controllers);
}

DetectedControllers DetectLogitechMouseG203(hid_device_info* info, const std::string& name)
{
    return(addLogitechLightsyncMouse1zone(info, name, 0xFF, 0x0E, 0x3A));
}

DetectedControllers DetectLogitechMouseG203L(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        LogitechG203LController*     controller     = new LogitechG203LController(dev, info->path, name);
        RGBController_LogitechG203L* rgb_controller = new RGBController_LogitechG203L(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectLogitechMouseG303(hid_device_info* info, const std::string& name)
{
    return(addLogitechLightsyncMouse2zone(info, name, 0xFF, 0x0E, 0x3A));
}

DetectedControllers DetectLogitechMouseG403(hid_device_info* info, const std::string& name)
{
    return(addLogitechLightsyncMouse2zone(info, name, 0xFF, 0x0E, 0x3A));
}

DetectedControllers DetectLogitechMouseG600(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        LogitechG600Controller*     controller     = new LogitechG600Controller(dev, info->path, name);
        RGBController_LogitechG600* rgb_controller = new RGBController_LogitechG600(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectLogitechMouseGPRO(hid_device_info* info, const std::string& name)
{
    return(addLogitechLightsyncMouse1zone(info, name, 0xFF, 0x0E, 0x3C));
}

/*---------------------------------------------------------*\
| Other Logitech Devices                                    |
\*---------------------------------------------------------*/
DetectedControllers DetectLogitechG933(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        /*-------------------------------------------------*\
        | Add G933 Headset                                  |
        \*-------------------------------------------------*/
        LogitechG933Controller*     controller     = new LogitechG933Controller(dev, info->path, name);
        RGBController_LogitechG933* rgb_controller = new RGBController_LogitechG933(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectLogitechX56(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        /*-------------------------------------------------*\
        | Add X56 Devices                                   |
        \*-------------------------------------------------*/
        LogitechX56Controller*     controller     = new LogitechX56Controller(dev, info->path, name);
        RGBController_LogitechX56* rgb_controller = new RGBController_LogitechX56(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| Group the nodes of one physical device by path. Windows   |
| splits a multi-collection HID interface into one node per |
| collection (the RAP usage-1 and FAP usage-2 handles are   |
| separate nodes), and numbers the collection twice:        |
|                                                           |
|   ...&MI_02&Col01#8&b41415e&0&0000#{guid}   usage 1       |
|   ...&MI_02&Col02#8&b41415e&0&0001#{guid}   usage 2       |
|                                                           |
| Trim both or the two nodes key apart and never bundle.    |
| The instance id stays, so two identical receivers still   |
| key apart. Linux/macOS paths carry no &Col token, so      |
| neither trim runs and a path names itself.                |
\*---------------------------------------------------------*/
static std::string LogitechDevicePathKey(const char* path)
{
    std::string key        = (path != nullptr) ? path : "";
    bool        collection = false;

    for(size_t pos = 0; pos + 4 <= key.size(); pos++)
    {
        if((key[pos] == '&')
        && (key[pos + 1] == 'c' || key[pos + 1] == 'C')
        && (key[pos + 2] == 'o' || key[pos + 2] == 'O')
        && (key[pos + 3] == 'l' || key[pos + 3] == 'L'))
        {
            size_t end = pos + 4;

            while(end < key.size() && isxdigit((unsigned char)key[end]))
            {
                end++;
            }

            key.erase(pos, end - pos);
            collection = true;
            break;
        }
    }

    if(collection)
    {
        size_t guid = key.rfind('#');
        size_t last = (guid == std::string::npos) ? std::string::npos : key.rfind('&', guid);

        if(last != std::string::npos)
        {
            key.erase(last, guid - last);
        }
    }

    return key;
}

usages BundleLogitechUsages(hid_device_info* info)
{
    /*-----------------------------------------------------*\
    | Grab every usage of the device that triggered this    |
    | callback (usage 1, 2 and 4 on normal FAP devices).    |
    | Match by path, not VID/PID: two identical receivers   |
    | share a VID/PID, and bundling their handles sent      |
    | one receiver's pairing-table read to the other.       |
    \*-----------------------------------------------------*/
    usages temp_usages;

    std::string      device_key = LogitechDevicePathKey(info->path);
    hid_device_info* temp_info  = hid_enumerate(info->vendor_id, info->product_id);
    hid_device_info* enumerated = temp_info;

    while(temp_info)
    {
        if(temp_info->interface_number == 2
        && LogitechDevicePathKey(temp_info->path) == device_key)
        {
            LOG_DEBUG("Attempting to open dev path: %s", temp_info->path);
            hid_device* dev = hid_open_path(temp_info->path);

            if(dev)
            {
                LOG_DEBUG("Success! Adding Usage %i for device @ path %s", temp_info->usage, temp_info->path);
                temp_usages.emplace((uint8_t)temp_info->usage, dev);
            }
            else
            {
                LOG_INFO("FAILED! Can not add Usage %i for device @ path %s", temp_info->usage, temp_info->path);
            }
        }
        temp_info = temp_info->next;
    }

    hid_free_enumeration(enumerated);

    return temp_usages;
}

/*---------------------------------------------------------*\
| Paired slots owned by a legacy controller: the virtual    |
| PID the receiver reports for the slot, mapped to the      |
| name its legacy detector registers under. A slot is       |
| left to legacy only while that detector is enabled,       |
| turn it off in Settings and the slot comes here on the    |
| next detection, same as the wired nodes. A wireless       |
| PID with no legacy detector does not belong here.         |
\*---------------------------------------------------------*/
static const std::map<uint16_t, const char*> hidpp20_legacy_wireless_pids =
{
    { 0x4053, "Logitech G900 Wireless Gaming Mouse"      },
    { 0x405D, "Logitech G403 Wireless Gaming Mouse"      },
    { 0x405F, "Logitech Powerplay Mat"                   },
    { 0x4067, "Logitech G903 Wireless Gaming Mouse"      },
    { 0x4070, "Logitech G703 Wireless Gaming Mouse"      },
    { 0x4079, "Logitech G Pro Wireless Gaming Mouse"     },
    { 0x407F, "Logitech G502 Wireless Gaming Mouse"      },
    { 0x4086, "Logitech G703 HERO Wireless Gaming Mouse" },
    { 0x4087, "Logitech G903 HERO Wireless Gaming Mouse" },
};

/*---------------------------------------------------------*\
| Is the legacy controller that owns this slot still        |
| enabled? Detectors are enabled unless Settings says       |
| otherwise, which is also what DetectionManager            |
| assumes for a detector it has not seen before.            |
\*---------------------------------------------------------*/
static bool HIDPP20SlotOwnedByLegacy(uint16_t dev_pid, std::string& detector_name)
{
    std::map<uint16_t, const char*>::const_iterator legacy = hidpp20_legacy_wireless_pids.find(dev_pid);

    if(legacy == hidpp20_legacy_wireless_pids.end())
    {
        return(false);
    }

    detector_name = legacy->second;

    json detector_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Detectors");

    if(detector_settings.contains("detectors")
    && detector_settings["detectors"].contains(detector_name))
    {
        return(detector_settings["detectors"][detector_name]);
    }

    return(true);
}

/*---------------------------------------------------------*\
| A paired slot on a receiver, keyed by the serial the      |
| receiver stores for it, the same string the device        |
| reports as its unit id, so cable and receiver sightings   |
| land on one identity without either being awake. Slots    |
| share the receiver's node and the mutex serializing it.   |
\*---------------------------------------------------------*/
class HIDPP20Slot
{
public:
    std::string                 node_path;
    uint8_t                     index;
    uint16_t                    usage_page;
    std::string                 pairing_name;
    std::shared_ptr<std::mutex> node_mutex;
};

static std::map<std::string, HIDPP20Slot>                       hidpp20_slots;
static std::map<std::string, std::shared_ptr<std::mutex>>       hidpp20_node_mutexes;
static std::mutex                                               hidpp20_registry_mutex;
static bool                                                     hidpp20_hook_registered = false;

/*---------------------------------------------------------*\
| Devices already built this pass, by unit id. A device     |
| reachable on more than one node reports the same id on    |
| each, so the first node to build it registers it and      |
| later nodes skip it. Cleared by the pre-detection hook.   |
\*---------------------------------------------------------*/
static std::set<std::string>                                    hidpp20_claimed_devices;

/*---------------------------------------------------------*\
| One persistent watcher per receiver node. Watchers        |
| outlive detection passes, so a device waking between      |
| them is still detected. A watcher that lost its node is   |
| held for destruction on the detection thread:             |
| destroying one joins its threads, and its own worker      |
| may be in Create right now, wanting the same lock.        |
\*---------------------------------------------------------*/
static std::map<std::string, std::unique_ptr<LogitechHIDPP20ReceiverWatcher>>   hidpp20_watchers;
static std::vector<std::unique_ptr<LogitechHIDPP20ReceiverWatcher>>             hidpp20_watchers_to_destroy;
static std::mutex                                                               hidpp20_watcher_mutex;
static bool                                                                     hidpp20_watchers_enabled = true;

static void HIDPP20PreDetectionReset()
{
    {
        std::lock_guard<std::mutex> lock(hidpp20_registry_mutex);

        hidpp20_slots.clear();
        hidpp20_node_mutexes.clear();
        hidpp20_claimed_devices.clear();
    }

    /*-----------------------------------------------------*\
    | Destroy dead watchers outside the lock so a watcher   |
    | mid-build never deadlocks against ensure-watcher.     |
    \*-----------------------------------------------------*/
    std::vector<std::unique_ptr<LogitechHIDPP20ReceiverWatcher>> to_destroy;

    {
        std::lock_guard<std::mutex> lock(hidpp20_watcher_mutex);

        to_destroy.swap(hidpp20_watchers_to_destroy);

        for(std::map<std::string, std::unique_ptr<LogitechHIDPP20ReceiverWatcher>>::iterator it = hidpp20_watchers.begin();
            it != hidpp20_watchers.end();)
        {
            if(!it->second->IsAlive())
            {
                to_destroy.push_back(std::move(it->second));
                it = hidpp20_watchers.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    to_destroy.clear();
}

static void HIDPP20ShutdownWatchers()
{
    std::map<std::string, std::unique_ptr<LogitechHIDPP20ReceiverWatcher>> live;
    std::vector<std::unique_ptr<LogitechHIDPP20ReceiverWatcher>>           dead;

    {
        std::lock_guard<std::mutex> lock(hidpp20_watcher_mutex);

        hidpp20_watchers_enabled = false;

        live.swap(hidpp20_watchers);
        dead.swap(hidpp20_watchers_to_destroy);
    }

    live.clear();
    dead.clear();
}

/*---------------------------------------------------------*\
| Has this device already been built this pass?             |
\*---------------------------------------------------------*/
static bool HIDPP20ClaimDevice(const std::string& device_id)
{
    std::lock_guard<std::mutex> lock(hidpp20_registry_mutex);

    return hidpp20_claimed_devices.insert(device_id).second;
}

/*---------------------------------------------------------*\
| Watchers own threads and a node handle, so they have to   |
| be stopped before the process exits.                      |
\*---------------------------------------------------------*/
class HIDPP20WatcherShutdown
{
public:
    ~HIDPP20WatcherShutdown()
    {
        HIDPP20ShutdownWatchers();
    }
};

static HIDPP20WatcherShutdown hidpp20_watcher_shutdown;

static bool HIDPP20BuildForWatcher(const std::string& node_path, uint8_t index);

static void HIDPP20RegisterHook()
{
    if(!hidpp20_hook_registered)
    {
        DetectionManager::get()->RegisterPreDetectionHook(HIDPP20PreDetectionReset);
        LogitechHIDPP20ReceiverWatcher::SetBuilder(HIDPP20BuildForWatcher);
        hidpp20_hook_registered = true;
    }
}

/*---------------------------------------------------------*\
| Make sure this receiver node has a live watcher. A dead   |
| one (node unplugged) is destroyed and replaced, the same  |
| path reappearing means the receiver was replugged.        |
\*---------------------------------------------------------*/
static void HIDPP20EnsureWatcher(const std::string& node_path, uint8_t bridge_feat_idx,
                                 uint8_t bridge_report_id, bool bridge_addressed)
{
    std::lock_guard<std::mutex> lock(hidpp20_watcher_mutex);

    if(!hidpp20_watchers_enabled)
    {
        return;
    }

    std::map<std::string, std::unique_ptr<LogitechHIDPP20ReceiverWatcher>>::iterator it = hidpp20_watchers.find(node_path);

    if(it != hidpp20_watchers.end())
    {
        if(it->second->IsAlive())
        {
            return;
        }

        hidpp20_watchers_to_destroy.push_back(std::move(it->second));
        hidpp20_watchers.erase(it);
    }

    std::unique_ptr<LogitechHIDPP20ReceiverWatcher> watcher(
        new LogitechHIDPP20ReceiverWatcher(node_path, bridge_feat_idx, bridge_report_id, bridge_addressed));

    if(watcher->Start())
    {
        hidpp20_watchers[node_path] = std::move(watcher);
    }
}

static std::shared_ptr<std::mutex> HIDPP20NodeMutex(const std::string& node_path)
{
    std::lock_guard<std::mutex> lock(hidpp20_registry_mutex);

    std::map<std::string, std::shared_ptr<std::mutex>>::iterator it = hidpp20_node_mutexes.find(node_path);

    if(it != hidpp20_node_mutexes.end())
    {
        return it->second;
    }

    std::shared_ptr<std::mutex> node_mutex = std::make_shared<std::mutex>();

    hidpp20_node_mutexes[node_path] = node_mutex;

    return node_mutex;
}

/*---------------------------------------------------------*\
| Is the node in hand this device's receiver slot?          |
|                                                           |
| Keyed on the node as well as the device, because a        |
| device reachable over its receiver AND over its           |
| cable answers to the same id on both. Ask only            |
| whether THIS node is the slot, otherwise the cable        |
| gets mistaken for the receiver it is also paired          |
| to, and the link we were handed is never taken.           |
\*---------------------------------------------------------*/
static bool HIDPP20LookupSlot(const std::string& device_id, const std::string& node_path, HIDPP20Slot& slot_out)
{
    std::lock_guard<std::mutex> lock(hidpp20_registry_mutex);

    std::map<std::string, HIDPP20Slot>::iterator it = hidpp20_slots.find(device_id);

    if(it == hidpp20_slots.end() || it->second.node_path != node_path)
    {
        return false;
    }

    slot_out = it->second;

    return true;
}

/*---------------------------------------------------------*\
| Enumerate: which devices does this node reach? A device   |
| answers for itself (unit id). A receiver answers for      |
| everything paired to it out of its own registers, so a    |
| device that is asleep or away on its cable is still       |
| known, under the same identity it has everywhere else.    |
\*---------------------------------------------------------*/
static std::vector<std::string> HIDPP20Enumerate(hid_device_info* info)
{
    std::vector<std::string> device_ids;

    if(info->vendor_id != LOGITECH_VID)
    {
        return(device_ids);
    }

#ifdef __linux__
    /*-----------------------------------------------------*\
    | hid-logitech-dj virtual child nodes (0x40XX) are the  |
    | kernel's view of devices the receiver's own node      |
    | already reaches. Driving one device over two          |
    | unsynchronized handles helps nobody.                  |
    \*-----------------------------------------------------*/
    if((info->product_id & 0xFF00) == 0x4000)
    {
        LOG_DEBUG("[Logitech HID++ 2.0] skipping DJ virtual node %04X @ %s (the receiver's node owns it)",
                  info->product_id, info->path);
        return(device_ids);
    }
#endif

    HIDPP20RegisterHook();

    /*-----------------------------------------------------*\
    | Is the node itself a device? The probe controller     |
    | is a stack object that is never Initialize()d; it     |
    | changes nothing on the device, so it is safe          |
    | against hardware another controller is driving.       |
    \*-----------------------------------------------------*/
    hid_device* dev = hid_open_path(info->path);

    if(dev == nullptr)
    {
        return(device_ids);
    }

    {
        LogitechHIDPP20Controller probe(dev, info->path, LOGITECH_DEFAULT_DEVICE_INDEX, false, nullptr,
                                        info->usage_page, nullptr,
                                        info->bus_type == HID_API_BUS_BLUETOOTH);

        std::string unit_id = probe.ProbeIdentity();

        if(!unit_id.empty())
        {
            LOG_DEBUG("[Logitech HID++ 2.0] %s is device %s", info->path, unit_id.c_str());

            device_ids.push_back(unit_id);

            return(device_ids);
        }
    }

    /*-----------------------------------------------------*\
    | Not a device. On the standard transport that is the   |
    | receiver signature, ask it who is paired to it.       |
    \*-----------------------------------------------------*/
    if(info->usage_page != 0xFF00)
    {
        return(device_ids);
    }

    usages bundle = BundleLogitechUsages(info);

    /*-----------------------------------------------------*\
    | Linux and macOS expose the whole interface as one     |
    | node that accepts every report ID, so it may not key  |
    | a usage-1 handle. One handle serves RAP and FAP both. |
    | Not on Windows: the node this runs on is the usage-2  |
    | collection, which rejects the 7-byte writes the       |
    | pairing read is made of. Aliasing it turns a missing  |
    | handle into silent read timeouts.                     |
    \*-----------------------------------------------------*/
#if !defined(_WIN32)
    if(bundle.find(1) == bundle.end())
    {
        hid_device* rap = hid_open_path(info->path);

        if(rap)
        {
            bundle.emplace((uint8_t)1, rap);
        }
    }
#endif

    wireless_map            wireless_devices;
    std::map<uint8_t, bool> online;
    int                     count = 0;

    if(bundle.find(1) != bundle.end())
    {
        count = getWirelessDevice(bundle, info->product_id, &wireless_devices, &online);
    }

    if(count > 0)
    {
        LOG_INFO("[Logitech receiver %04X @ %s] %d paired device(s)", info->product_id, info->path, count);

        std::shared_ptr<std::mutex> node_mutex = HIDPP20NodeMutex(info->path);

        for(wireless_map::iterator wd = wireless_devices.begin(); wd != wireless_devices.end(); wd++)
        {
            uint16_t    dev_pid = wd->first;
            uint8_t     idx     = wd->second;

            std::string legacy_name;

            if(HIDPP20SlotOwnedByLegacy(dev_pid, legacy_name))
            {
                LOG_INFO("[Logitech receiver %04X] slot=%u PID=%04X is owned by '%s', leaving it alone "
                         "(disable that detector to hand it to HID++ 2.0)",
                         info->product_id, idx, dev_pid, legacy_name.c_str());
                continue;
            }

            if(!legacy_name.empty())
            {
                LOG_INFO("[Logitech receiver %04X] slot=%u PID=%04X: '%s' is disabled, taking the slot",
                         info->product_id, idx, dev_pid, legacy_name.c_str());
            }

            std::string serial = getWirelessDeviceSerial(bundle, idx);
            std::string name   = getWirelessDeviceName(bundle, idx);

            /*---------------------------------------------*\
            | Some receivers store no serial. The slot is   |
            | still a device, it just cannot be recognized  |
            | as the same one over another link, name it    |
            | after where it lives.                         |
            \*---------------------------------------------*/
            if(serial.empty())
            {
                serial = std::string(info->path) + "#" + std::to_string(idx);

                LOG_INFO("[Logitech receiver %04X] slot=%u '%s' has no stored serial; it will not be recognized over a second link",
                         info->product_id, idx, name.c_str());
            }

            HIDPP20Slot slot;

            slot.node_path    = info->path;
            slot.index        = idx;
            slot.usage_page   = info->usage_page;
            slot.pairing_name = name;
            slot.node_mutex   = node_mutex;

            {
                std::lock_guard<std::mutex> lock(hidpp20_registry_mutex);
                hidpp20_slots[serial] = slot;
            }

            LOG_INFO("[Logitech receiver %04X] slot=%u PID=%04X '%s' is device %s",
                     info->product_id, idx, dev_pid, name.c_str(), serial.c_str());

            device_ids.push_back(serial);
        }
    }
    else
    {
        LOG_DEBUG("[Logitech HID++ 2.0] %04X @ %s answered neither the device probe nor the pairing read, ignoring",
                  info->product_id, info->path);
    }

    for(usages::iterator u = bundle.begin(); u != bundle.end(); u++)
    {
        if(u->second)
        {
            hid_close(u->second);
        }
    }

    return(device_ids);
}

/*---------------------------------------------------------*\
| What it takes to build one device: the node, which slot   |
| on it, and how to talk to it. Kept past the pass that     |
| found it so the node watcher can build the same device    |
| when it turns up later.                                   |
\*---------------------------------------------------------*/
class HIDPP20BuildTarget
{
public:
    std::string                 node_path;
    uint8_t                     index               = LOGITECH_DEFAULT_DEVICE_INDEX;
    uint16_t                    usage_page          = 0;
    uint16_t                    vendor_id           = 0;
    uint16_t                    product_id          = 0;
    bool                        behind_receiver     = false;
    bool                        bluetooth           = false;
    std::shared_ptr<std::mutex> node_mutex;
    std::string                 pairing_name;
};

static std::map<std::pair<std::string, uint8_t>, HIDPP20BuildTarget>    hidpp20_build_targets;

/*---------------------------------------------------------*\
| Slots being built right now. A detection pass and a       |
| watcher can reach the same slot at the same moment; the   |
| first to claim it builds, the second skips it.            |
\*---------------------------------------------------------*/
static std::set<std::pair<std::string, uint8_t>>                        hidpp20_building;

class HIDPP20BuildClaim
{
public:
    HIDPP20BuildClaim(const std::string& node_path, uint8_t index)
    {
        key    = std::make_pair(node_path, index);

        std::lock_guard<std::mutex> lock(hidpp20_registry_mutex);

        held = hidpp20_building.insert(key).second;
    }

    ~HIDPP20BuildClaim()
    {
        if(!held)
        {
            return;
        }

        std::lock_guard<std::mutex> lock(hidpp20_registry_mutex);

        hidpp20_building.erase(key);
    }

    bool Held() const   { return held; }

private:
    std::pair<std::string, uint8_t> key;
    bool                            held;
};

static void HIDPP20RecordTarget(const HIDPP20BuildTarget& target)
{
    std::lock_guard<std::mutex> lock(hidpp20_registry_mutex);

    hidpp20_build_targets[std::make_pair(target.node_path, target.index)] = target;
}

static bool HIDPP20LookupTarget(const std::string& node_path, uint8_t index, HIDPP20BuildTarget& target_out)
{
    std::lock_guard<std::mutex> lock(hidpp20_registry_mutex);

    std::map<std::pair<std::string, uint8_t>, HIDPP20BuildTarget>::iterator it =
        hidpp20_build_targets.find(std::make_pair(node_path, index));

    if(it == hidpp20_build_targets.end())
    {
        return false;
    }

    target_out = it->second;

    return true;
}

/*---------------------------------------------------------*\
| Build one device. Returns nullptr for a device that is    |
| not reachable yet and for one that answers with no        |
| lighting; both leave the target on file, so a device      |
| that was asleep is built when its dongle reports it.      |
\*---------------------------------------------------------*/
static RGBController_LogitechHIDPP20* HIDPP20BuildController(const HIDPP20BuildTarget& target)
{
    HIDPP20BuildClaim claim(target.node_path, target.index);

    if(!claim.Held())
    {
        LOG_DEBUG("[Logitech HID++ 2.0] %s index=0x%02X is already being built",
                  target.node_path.c_str(), target.index);
        return(nullptr);
    }

    hid_device* dev = hid_open_path(target.node_path.c_str());

    if(dev == nullptr)
    {
        return(nullptr);
    }

    /*-----------------------------------------------------*\
    | 0x8080 fn3 rides the 0x12 very-long report, which     |
    | Windows splits onto a separate usage 0x0604           |
    | collection on the same interface (the legacy G810 /   |
    | G910 controllers opened it as a second handle). Open  |
    | that node when it exists; otherwise the main handle   |
    | carries every report ID (single node per interface    |
    | on Linux/macOS). Match by path key, not VID/PID, so   |
    | two identical keyboards do not cross wires.           |
    \*-----------------------------------------------------*/
    hid_device* perkey_vl = nullptr;

    if(target.usage_page == 0xFF43)
    {
        std::string      device_key = LogitechDevicePathKey(target.node_path.c_str());
        hid_device_info* enumerated = hid_enumerate(target.vendor_id, target.product_id);

        for(hid_device_info* n = enumerated; n != nullptr; n = n->next)
        {
            if(n->usage_page == 0xFF43 && n->usage == 0x0604
            && LogitechDevicePathKey(n->path) == device_key)
            {
                perkey_vl = hid_open_path(n->path);
                break;
            }
        }

        hid_free_enumeration(enumerated);
    }

    if(perkey_vl == nullptr)
    {
        perkey_vl = dev;
    }

    LogitechHIDPP20Controller* controller = new LogitechHIDPP20Controller(dev, target.node_path.c_str(), target.index,
                                                                          target.behind_receiver, target.node_mutex,
                                                                          target.usage_page, perkey_vl,
                                                                          target.bluetooth);

    if(target.behind_receiver)
    {
        controller->SetPairingName(target.pairing_name);
    }

    if(!controller->Probe())
    {
        /*-------------------------------------------------*\
        | The receiver reports it as paired here, but it    |
        | is not answering. The watcher builds it on the    |
        | 0x41 connection notification.                     |
        \*-------------------------------------------------*/
        delete controller;

        if(target.behind_receiver)
        {
            HIDPP20EnsureWatcher(target.node_path, 0, 0, false);
        }

        return(nullptr);
    }

    const HIDPP20DeviceCapabilities& caps = controller->GetCapabilities();

    if(caps.has_zone_effects || caps.has_perkey)
    {
        controller->Initialize();

        RGBController_LogitechHIDPP20* rgb_controller = new RGBController_LogitechHIDPP20(controller);

        /*-------------------------------------------------*\
        | Reader and power threads from the start, so we    |
        | see connection events and handle power management |
        | without waiting for a mode update.                |
        \*-------------------------------------------------*/
        if(caps.has_power_mgmt || caps.idx_wireless_status != 0)
        {
            controller->StartPowerManager();

            if(!caps.has_power_mgmt && caps.idx_wireless_status != 0)
            {
                controller->StartEventWatcher();
            }
        }
        else if(target.behind_receiver || controller->HasBridge())
        {
            /*---------------------------------------------*\
            | No power management and no WirelessStatus;    |
            | threads anyway, so the node watcher's nudge   |
            | has a power thread to land on.                |
            \*---------------------------------------------*/
            controller->StartEventWatcher();
        }

        /*-------------------------------------------------*\
        | Wireless devices answer to the node watcher's     |
        | nudges; keep it current on who is built here.     |
        \*-------------------------------------------------*/
        if(target.behind_receiver)
        {
            LogitechHIDPP20ReceiverWatcher::RegisterSubDevice(target.node_path, target.index, controller);
            HIDPP20EnsureWatcher(target.node_path, 0, 0, false);
        }
        else if(controller->HasBridge())
        {
            const HIDPP20Transport& transport = controller->GetTransport();

            LogitechHIDPP20ReceiverWatcher::RegisterSubDevice(target.node_path, target.index, controller);
            HIDPP20EnsureWatcher(target.node_path, transport.bridge_feat_idx, transport.report_id,
                                 transport.addressed);
        }

        return(rgb_controller);
    }

    if(controller->HasBridge() && controller->GetTransport().bridge_mtu == 0)
    {
        /*-------------------------------------------------*\
        | A dongle whose sub-device is not powered on. The  |
        | watcher builds it on the bridge's                 |
        | ConnectionStateChangedEvent.                      |
        \*-------------------------------------------------*/
        LOG_INFO("[%s] Dongle has no sub-device yet", caps.device_name.c_str());

        const HIDPP20Transport& transport = controller->GetTransport();

        HIDPP20EnsureWatcher(target.node_path, transport.bridge_feat_idx, transport.report_id,
                             transport.addressed);

        delete controller;

        return(nullptr);
    }

    /*-----------------------------------------------------*\
    | It answered, and it has no lighting. Not ours.        |
    \*-----------------------------------------------------*/
    LOG_INFO("[%s] No RGB features, skipping", caps.device_name.c_str());

    delete controller;

    return(nullptr);
}

/*---------------------------------------------------------*\
| Create: build the controller for a device this pass has   |
| not built yet.                                            |
|                                                           |
| Returning nothing means "it is there, we just cannot      |
| reach it yet", asleep, switched off, paired to another    |
| host. The target is on file either way.                   |
\*---------------------------------------------------------*/
static DetectedControllers HIDPP20Create(hid_device_info* info, const std::string& device_id)
{
    DetectedControllers detected;

    HIDPP20Slot         slot;
    HIDPP20BuildTarget  target;

    target.behind_receiver = HIDPP20LookupSlot(device_id, std::string(info->path), slot);
    target.node_path       = std::string(info->path);
    target.index           = target.behind_receiver ? slot.index : (uint8_t)LOGITECH_DEFAULT_DEVICE_INDEX;
    target.usage_page      = (uint16_t)info->usage_page;
    target.vendor_id       = (uint16_t)info->vendor_id;
    target.product_id      = (uint16_t)info->product_id;
    target.node_mutex      = slot.node_mutex;
    target.pairing_name    = slot.pairing_name;
    target.bluetooth       = (info->bus_type == HID_API_BUS_BLUETOOTH);

    HIDPP20RecordTarget(target);

    RGBController_LogitechHIDPP20* rgb_controller = HIDPP20BuildController(target);

    if(rgb_controller != nullptr)
    {
        LOG_INFO("[%s] Registering RGB controller (device %s)",
                 rgb_controller->GetController()->GetCapabilities().device_name.c_str(), device_id.c_str());

        detected.push_back(rgb_controller);
    }

    return(detected);
}

/*---------------------------------------------------------*\
| Build a device the node watcher reports as connected.     |
| Runs on the watcher's worker thread, never on its reader. |
|                                                           |
| A pass in flight may be building this very device, so     |
| wait it out, then check the sub-device registry, which    |
| is the live answer to whether this slot has a controller. |
| A link established is not a device ready to answer: a     |
| failed build leaves the target recorded for the next      |
| connection event.                                         |
\*---------------------------------------------------------*/
static bool HIDPP20BuildForWatcher(const std::string& node_path, uint8_t index)
{
    HIDPP20BuildTarget target;

    if(!HIDPP20LookupTarget(node_path, index, target))
    {
        LOG_DEBUG("[Logitech HID++ 2.0] %s index=0x%02X connected but was never enumerated, leaving it to the next detection",
                  node_path.c_str(), index);
        return false;
    }

    DetectionManager::get()->WaitForDetection();

    if(LogitechHIDPP20ReceiverWatcher::HasSubDevice(node_path, index))
    {
        return false;
    }

    /*-----------------------------------------------------*\
    | Nothing is registered once teardown starts.           |
    \*-----------------------------------------------------*/
    {
        std::lock_guard<std::mutex> lock(hidpp20_watcher_mutex);

        if(!hidpp20_watchers_enabled)
        {
            return false;
        }
    }

    RGBController_LogitechHIDPP20* rgb_controller = HIDPP20BuildController(target);

    if(rgb_controller == nullptr)
    {
        return false;
    }

    LOG_INFO("[%s] Connected, registering RGB controller",
             rgb_controller->GetController()->GetCapabilities().device_name.c_str());

    /*-----------------------------------------------------*\
    | The node it was built on, so the unplug callback      |
    | unregisters it with everything else on that node.     |
    \*-----------------------------------------------------*/
    DetectionManager::get()->RegisterRGBController(rgb_controller, node_path);

    return true;
}

/*---------------------------------------------------------*\
| Every device this node reaches, built once each. A device |
| on more than one node reports the same unit id on each,   |
| so the first node to build it registers it and later      |
| nodes skip it; the node it was built on is the one        |
| DetectionManager unregisters it from.                     |
\*---------------------------------------------------------*/
DetectedControllers DetectLogitechHIDPP20(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers      detected;
    std::vector<std::string> device_ids = HIDPP20Enumerate(info);

    for(size_t i = 0; i < device_ids.size(); i++)
    {
        const std::string& device_id = device_ids[i];

        if(!HIDPP20ClaimDevice(device_id))
        {
            LOG_DEBUG("[Logitech HID++ 2.0] device %s is already built this pass, skipping %s",
                      device_id.c_str(), info->path);
            continue;
        }

        DetectedControllers built = HIDPP20Create(info, device_id);

        if(built.empty())
        {
            /*---------------------------------------------*\
            | Nothing to drive, or nothing answering yet.   |
            | Let another node try it this pass.            |
            \*---------------------------------------------*/
            std::lock_guard<std::mutex> lock(hidpp20_registry_mutex);
            hidpp20_claimed_devices.erase(device_id);
            continue;
        }

        detected.insert(detected.end(), built.begin(), built.end());
    }

    return(detected);
}

/*-------------------------------------------------------------------------------------------------------------------------------------*\
| Unified HID++ 2.0: generic detection. These run only for devices with no *enabled* VID/PID-specific detector, disabling a legacy      |
| controller in Settings hands its hardware over on the next detection. That is the migration path: no code change, and no risk to a    |
| device whose legacy controller stays enabled.                                                                                         |
|                                                                                                                                       |
| The registrations cover every legacy transport signature:                                                                             |
|  any interface, 0xFF00 usage 2    standard HID++ long report (modern keyboards/mice, receivers, G915 family, wired Lightspeed mice).  |
|                                   Usage 2 is the collection we write to, on Windows, the only one that accepts our writes.            |
|  any interface, 0xFF43 any usage  keyboards (G213/G512/G610/G810/G813/G815/G910/G Pro), the G560 speaker, the                         |
|                                   G933 headset, and Bluetooth nodes. Not interface-keyed: a Bluetooth node                            |
|                                   reports interface -1, which is also HID_INTERFACE_ANY, so it can never match.                       |
|  interface 1, 0xFF00 any usage    older mice whose HID++ collection is not the usage-2 one                                            |
|  any interface, 0xFFA0 usage 1    Centurion (G522, PRO X 2)                                                                           |
|                                                                                                                                       |
| Not covered, deliberately: the G600 (page 0xFF80) and the X56 (own VID) are not HID++ 2.0. A matching non-HID++ node costs one failed |
| probe, ProbeIdentity changes nothing on the device. Receivers are recognized at runtime: the device probe fails and the pairing table |
| answers; paired slots follow the same enabled/disabled rule (hidpp20_legacy_wireless_pids).                                           |
\*-------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_PU_ONLY ("Logitech HID++ 2.0", DetectLogitechHIDPP20, 0xFF00, 2);
REGISTER_HID_DETECTOR_P_ONLY  ("Logitech HID++ 2.0", DetectLogitechHIDPP20, 0xFF43);
REGISTER_HID_DETECTOR_IP_ONLY ("Logitech HID++ 2.0", DetectLogitechHIDPP20, 1, 0xFF00);
REGISTER_HID_DETECTOR_PU_ONLY ("Logitech HID++ 2.0", DetectLogitechHIDPP20, 0xFFA0, 1);

REGISTER_CUSTOM_UDEV_RULE(logitech_hidpp20, "Logitech HID++ 2.0", "SUBSYSTEM==\"hidraw\", ATTRS{idVendor}==\"046d\", TAG+=\"uaccess\", TAG+=\"Logitech_HID_20\"\nSUBSYSTEM==\"usb\", ATTR{idVendor}==\"046d\", TAG+=\"uaccess\", TAG+=\"Logitech_HID_20\"");
REGISTER_CUSTOM_UDEV_RULE(logitech_g560, "Logitech G560 Lightsync Speaker", "SUBSYSTEMS==\"usb|hidraw\", ATTRS{idVendor}==\"046d\", ATTRS{idProduct}==\"0a78\", TAG+=\"uaccess\", TAG+=\"Logitech_G560_Lightsync_Speaker\"");

/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                                                                         |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech G213",                                  DetectLogitechKeyboardG213, LOGITECH_VID, LOGITECH_G213_PID,                    1, 0xFF43, 0x0602);
REGISTER_HID_DETECTOR_IP ("Logitech G813 RGB Mechanical Gaming Keyboard",   DetectLogitechKeyboardG815, LOGITECH_VID, LOGITECH_G813_PID,                    1, 0xFF43);
REGISTER_HID_DETECTOR_IP ("Logitech G815 RGB Mechanical Gaming Keyboard",   DetectLogitechKeyboardG815, LOGITECH_VID, LOGITECH_G815_PID,                    1, 0xFF43);

REGISTER_HID_DETECTOR_IPU("Logitech G915 Wireless RGB Mechanical Gaming Keyboard",              DetectLogitechKeyboardG915,      LOGITECH_VID, LOGITECH_G915_RECEIVER_PID,      2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G915 Wireless RGB Mechanical Gaming Keyboard (Receiver 2)", DetectLogitechKeyboardG915Receiver2, LOGITECH_VID, LOGITECH_G915_RECEIVER_2_PID, 2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G915 Wireless RGB Mechanical Gaming Keyboard (Wired)",      DetectLogitechKeyboardG915Wired, LOGITECH_VID, LOGITECH_G915_WIRED_PID,         2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G915TKL Wireless RGB Mechanical Gaming Keyboard",           DetectLogitechKeyboardG915,      LOGITECH_VID, LOGITECH_G915TKL_RECEIVER_PID,   2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G915TKL Wireless RGB Mechanical Gaming Keyboard (Wired)",   DetectLogitechKeyboardG915Wired, LOGITECH_VID, LOGITECH_G915TKL_WIRED_PID,      2, 0xFF00, 2);
/*---------------------------------------------------------*\
| C547 carve-out: the legacy G915 Receiver 2 registration   |
| above makes C547 a VID/PID-specific match, which          |
| suppresses the generic HID++ 2.0 detector for every C547  |
| node, including receivers with non-G915 devices paired.   |
| This specific entry runs after the G915 detector          |
| (registration order); when that returns nothing, the      |
| unified pairing-table enumeration takes the receiver.     |
| Literal PID: the macro token-pastes the object name and   |
| LOGITECH_G915_RECEIVER_2_PID is already used above.       |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech HID++ 2.0 (C547 receiver)", DetectLogitechHIDPP20, LOGITECH_VID, 0xC547, 2, 0xFF00, 2);
/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Mice                                                                                                                                              |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP ("Logitech G203 Prodigy",                          DetectLogitechMouseG203,    LOGITECH_VID, LOGITECH_G203_PID,                    1, 0xFF00);
REGISTER_HID_DETECTOR_IPU("Logitech G203 Lightsync",                        DetectLogitechMouseG203L,   LOGITECH_VID, LOGITECH_G203_LIGHTSYNC_PID,          1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G203 Lightsync",                        DetectLogitechMouseG203L,   LOGITECH_VID, LOGITECH_G203_LIGHTSYNC_PID_2,        1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IP ("Logitech G303 Daedalus Apex",                    DetectLogitechMouseG303,    LOGITECH_VID, LOGITECH_G303_PID,                    1, 0xFF00);
REGISTER_HID_DETECTOR_IP ("Logitech G403 HERO",                             DetectLogitechMouseG403,    LOGITECH_VID, LOGITECH_G403_HERO_PID,               1, 0xFF00);
REGISTER_HID_DETECTOR_IP ("Logitech G600 Gaming Mouse",                     DetectLogitechMouseG600,    LOGITECH_VID, LOGITECH_G600_PID,                    1, 0xFF80);
REGISTER_HID_DETECTOR_IP ("Logitech G Pro Gaming Mouse",                    DetectLogitechMouseGPRO,    LOGITECH_VID, LOGITECH_G_PRO_PID,                   1, 0xFF00);
REGISTER_HID_DETECTOR_IP ("Logitech G Pro HERO Gaming Mouse",               DetectLogitechMouseGPRO,    LOGITECH_VID, LOGITECH_G_PRO_HERO_PID,              1, 0xFF00);
/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Headsets                                                                                                                                          |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech G933 Lightsync Headset",                DetectLogitechG933,         LOGITECH_VID, LOGITECH_G933_PID,                    3, 0xFF43, 514);
/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Joysticks                                                                                                                                         |
|   Older versions of the HOTAS have the controller on usage 1 however registering a IP detector resulted in duplicate detections on Linux          |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech X56 Rhino Hotas Joystick",              DetectLogitechX56,          LOGITECH_X56_VID, LOGITECH_X56_JOYSTICK_PID,        2, 0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Logitech X56 Rhino Hotas Throttle",              DetectLogitechX56,          LOGITECH_X56_VID, LOGITECH_X56_THROTTLE_PID,        2, 0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Logitech X56 Rhino Hotas Joystick",              DetectLogitechX56,          LOGITECH_X56_VID, LOGITECH_X56_JOYSTICK_PID,        2, 0xFF00, 3);
REGISTER_HID_DETECTOR_IPU("Logitech X56 Rhino Hotas Throttle",              DetectLogitechX56,          LOGITECH_X56_VID, LOGITECH_X56_THROTTLE_PID,        2, 0xFF00, 3);





/*---------------------------------------------------------------------------------------------------------*\
| Common Lightspeed Detection                                                                               |
|                                                                                                           |
\*---------------------------------------------------------------------------------------------------------*/

DetectedControllers CreateLogitechLightspeedDevice(char *path, usages device_usages, uint8_t device_index, uint16_t pid, bool wireless, std::shared_ptr<std::mutex> mutex_ptr)
{
    DetectedControllers             detected_controllers;
    LogitechLightspeedController*   controller              = new LogitechLightspeedController(device_usages.find(2)->second, path);
    bool                            lightspeedDeviceIsValid = false;
    int                             retryCount              = 0;

    while(!lightspeedDeviceIsValid && retryCount < LOGITECH_LIGHTSPEED_DETECT_MAX_RETRY)
    {
        std::this_thread::sleep_for(50ms);
        controller->lightspeed                              = new logitech_device(path, device_usages, device_index, wireless, mutex_ptr);
        lightspeedDeviceIsValid                             = controller->lightspeed->is_valid();
        retryCount++;
    }

    if (retryCount < LOGITECH_LIGHTSPEED_DETECT_MAX_RETRY)
    {
        RGBController_LogitechLightspeed* rgb_controller    = new RGBController_LogitechLightspeed(controller);
        rgb_controller->pid                                 = pid;

        detected_controllers.push_back(rgb_controller);

        LOG_DEBUG("Added controller in %i retries", retryCount);
    }
    else
    {
        delete controller;
        LOG_DEBUG("Failed to set up device - exceeded retries");
    }

    return(detected_controllers);
}

DetectedControllers DetectLogitechWired(hid_device_info* info, const std::string& /*name*/)
{
    /*-----------------------------------------------------------------*\
    | Wired lightspeed devices don't use the FAP short message          |
    |   Be sure to specify a Page AND Usage when using this detector    |
    |   i.e. REGISTER_HID_DETECTOR_IPU                                  |
    \*-----------------------------------------------------------------*/
    DetectedControllers detected_controllers;
    usages              device_usages;
    hid_device*         dev             = hid_open_path(info->path);

    if(dev)
    {
        LOG_DEBUG("Adding Usage %i for device @ path %s", info->usage, info->path);
        device_usages.emplace((uint8_t)info->usage, dev);
    }
    else
    {
        LOG_DEBUG("Error opening Usage %i for device @ path %s", info->usage, info->path);
    }

    if(device_usages.size() > 0)
    {
        detected_controllers = CreateLogitechLightspeedDevice(info->path, device_usages, LOGITECH_DEFAULT_DEVICE_INDEX, info->product_id, false, nullptr);
    }

    return(detected_controllers);
}

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

DetectedControllers DetectLogitechLightspeedReceiver(hid_device_info* info, const std::string& /*name*/)
{
    /*-----------------------------------------------------------------*\
    | Need to save the PID and the device path before iterating         |
    |    over "info" in BundleLogitechUsages()                          |
    \*-----------------------------------------------------------------*/
    DetectedControllers detected_controllers;
    char                *path           = info->path;
    uint16_t            dev_pid         = info->product_id;
    usages              device_usages   = BundleLogitechUsages(info);

    wireless_map        wireless_devices;
    unsigned int        device_count    = getWirelessDevice(device_usages, dev_pid, &wireless_devices);

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
            detected_controllers = CreateLogitechLightspeedDevice(path, device_usages, wd->second, dev_pid, true, logitech_mutex);
        }
    }

    return(detected_controllers);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Lightspeed Receivers (Windows Wireless)                                                                                                           |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech Lightspeed Receiver",                        DetectLogitechLightspeedReceiver,   LOGITECH_VID, LOGITECH_G_LIGHTSPEED_RECEIVER_PID,   2, 0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Logitech G Powerplay Mousepad",                       DetectLogitechLightspeedReceiver,   LOGITECH_VID, LOGITECH_G_LIGHTSPEED_POWERPLAY_PID,  2, 0xFF00, 1);

#endif

/*---------------------------------------------------------------------------------------------------------*\
| Linux Lightspeed Detection                                                                                |
|                                                                                                           |
| The Linux kernel handles detecting wireless devices connected to a Unifying Receiver.                     |
\*---------------------------------------------------------------------------------------------------------*/
#ifdef __linux__

DetectedControllers DetectLogitechWireless(hid_device_info* info, const std::string& /*name*/)
{
    /*-----------------------------------------------------------------*\
    | Wireless lightspeed devices on Linux are handled by the Kernel    |
    |   and as such can largely be treated as Wired with the caveat     |
    |   that they may not be connected                                  |
    \*-----------------------------------------------------------------*/
    DetectedControllers detected_controllers;
    usages              device_usages;
    hid_device*         dev             = hid_open_path(info->path);

    if(dev)
    {
        LOG_DEBUG("Adding Usage %i for device @ path %s", info->usage, info->path);
        device_usages.emplace((uint8_t)info->usage, dev);
    }
    else
    {
        LOG_DEBUG("Error opening Usage %i for device @ path %s", info->usage, info->path);
    }

    if(device_usages.size() > 0)
    {
        /*-------------------------------------------------*\
        | Create mutex to prevent the controllers sharing a |
        |   receiver from interfering with each other       |
        \*-------------------------------------------------*/
        std::shared_ptr<std::mutex>       logitech_mutex = std::make_shared<std::mutex>();

        detected_controllers = CreateLogitechLightspeedDevice(info->path, device_usages, LOGITECH_DEFAULT_DEVICE_INDEX, info->product_id, true, logitech_mutex);
    }

    return(detected_controllers);
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------*\
| Lightspeed Devices (Linux Wireless)                                                                                                                |
\*--------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Logitech G403 Wireless Gaming Mouse",                DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G403_LIGHTSPEED_VIRTUAL_PID,         2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G502 Wireless Gaming Mouse",                DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G502_LIGHTSPEED_VIRTUAL_PID,         2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G703 Wireless Gaming Mouse",                DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G703_LIGHTSPEED_VIRTUAL_PID,         2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G703 HERO Wireless Gaming Mouse",           DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G703_HERO_LIGHTSPEED_VIRTUAL_PID,    2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G900 Wireless Gaming Mouse",                DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G900_LIGHTSPEED_VIRTUAL_PID,         2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G903 Wireless Gaming Mouse",                DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G903_LIGHTSPEED_VIRTUAL_PID,         2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G903 HERO Wireless Gaming Mouse",           DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G903_LIGHTSPEED_VIRTUAL_HERO_PID,    2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G Pro Wireless Gaming Mouse",               DetectLogitechWireless,     LOGITECH_VID, LOGITECH_G_PRO_WIRELESS_VIRTUAL_PID,          2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech Powerplay Mat",                             DetectLogitechWireless,     LOGITECH_VID, LOGITECH_POWERPLAY_MAT_VIRTUAL_PID,           2, 0xFF00, 2);

REGISTER_CUSTOM_UDEV_RULE(logitech_lightspeed, "Logitech G Lightspeed Receiver", "SUBSYSTEMS==\"usb|hidraw\", ATTRS{idVendor}==\"046d\", ATTRS{idProduct}==\"c539\", TAG+=\"uaccess\", TAG+=\"Logitech_G_Lightspeed_Receiver\"");
REGISTER_CUSTOM_UDEV_RULE(logitech_powerplay, "Logitech Powerplay Mat Receiver", "SUBSYSTEMS==\"usb|hidraw\", ATTRS{idVendor}==\"046d\", ATTRS{idProduct}==\"c53a\", TAG+=\"uaccess\", TAG+=\"Logitech_Powerplay_Mat_Receiver\"");

#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Lightspeed Wireless Devices (Common Wired)                                                                                                        |
|   G502 changed to PU to accomodate old and new firmware. Other devices may require similar update #4627                                           |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_PU("Logitech G502 Wireless Gaming Mouse (wired)",         DetectLogitechWired,        LOGITECH_VID, LOGITECH_G502_LIGHTSPEED_PID,                    0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G502 Proteus Spectrum Gaming Mouse",        DetectLogitechWired,        LOGITECH_VID, LOGITECH_G502_PROTEUS_SPECTRUM_PID,           1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G502 HERO Gaming Mouse",                    DetectLogitechWired,        LOGITECH_VID, LOGITECH_G502_HERO_PID,                       1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G403 Prodigy Gaming Mouse",                 DetectLogitechWired,        LOGITECH_VID, LOGITECH_G403_PID,                            1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G403 Wireless Gaming Mouse (wired)",        DetectLogitechWired,        LOGITECH_VID, LOGITECH_G403_LIGHTSPEED_PID,                 1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G703 Wireless Gaming Mouse (wired)",        DetectLogitechWired,        LOGITECH_VID, LOGITECH_G703_LIGHTSPEED_PID,                 1, 0xFF00, 2);
REGISTER_HID_DETECTOR_PU("Logitech G703 HERO Wireless Gaming Mouse (wired)",    DetectLogitechWired,        LOGITECH_VID, LOGITECH_G703_HERO_LIGHTSPEED_PID,               0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G900 Wireless Gaming Mouse (wired)",        DetectLogitechWired,        LOGITECH_VID, LOGITECH_G900_LIGHTSPEED_PID,                 1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G903 Wireless Gaming Mouse (wired)",        DetectLogitechWired,        LOGITECH_VID, LOGITECH_G903_LIGHTSPEED_PID,                 1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G903 HERO Wireless Gaming Mouse (wired)",   DetectLogitechWired,        LOGITECH_VID, LOGITECH_G903_LIGHTSPEED_HERO_PID,            2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G Pro Wireless Gaming Mouse (wired)",       DetectLogitechWired,        LOGITECH_VID, LOGITECH_G_PRO_WIRELESS_PID,                  2, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Logitech G633 Gaming Headset",                       DetectLogitechWired,        LOGITECH_VID, LOGITECH_G633_PID,                            3, 0xFF43, 514);
REGISTER_HID_DETECTOR_IPU("Logitech G635 Gaming Headset",                       DetectLogitechWired,        LOGITECH_VID, LOGITECH_G635_PID,                            3, 0xFF43, 514);
REGISTER_HID_DETECTOR_IPU("Logitech G733 Gaming Headset",                       DetectLogitechWired,        LOGITECH_VID, LOGITECH_G733_PID,                            3, 0xFF43, 514);
REGISTER_HID_DETECTOR_IPU("Logitech G733 Gaming Headset",                       DetectLogitechWired,        LOGITECH_VID, LOGITECH_G733_2_PID,                          3, 0xFF43, 514);
REGISTER_HID_DETECTOR_IPU("Logitech G733 Gaming Headset",                       DetectLogitechWired,        LOGITECH_VID, LOGITECH_G733_3_PID,                          3, 0xFF43, 514);
REGISTER_HID_DETECTOR_IPU("Logitech G935 Gaming Headset",                       DetectLogitechWired,        LOGITECH_VID, LOGITECH_G935_PID,                            3, 0xFF43, 514);
