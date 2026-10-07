/*---------------------------------------------------------*\
| SinowealthControllerDetect.cpp                            |
|                                                           |
|   Detector for Sinowealth, Genesis and Everest brand Mice |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "RGBController_SinowealthKeyboard10c.h"
#include "SinowealthController.h"
#include "SinowealthController1007.h"
#include "SinowealthKeyboard10cController.h"
#include "SinowealthKeyboard10cDevices.h"
#include "SinowealthKeyboardController.h" // Disabled
#include "SinowealthKeyboard16Controller.h" // Disabled
#include "SinowealthKeyboard90Controller.h"
#include "SinowealthGMOWController.h"
#include "GenesisXenon200Controller.cpp"
#include "RGBController.h"
#include "RGBController_Sinowealth.h"
#include "RGBController_Sinowealth1007.h"
#include "RGBController_SinowealthKeyboard.h" // Disabled
#include "RGBController_SinowealthKeyboard16.h" // Disabled
#include "RGBController_SinowealthKeyboard90.h"
#include "RGBController_SinowealthGMOW.h"
#include "RGBController_GenesisXenon200.h"
#include <hidapi.h>
#include <set>
#include "LogManager.h"

#define SINOWEALTH_VID                      0x258A

#define Glorious_Model_O_PID                0x0036
#define Glorious_Model_OW_PID1              0x2022 // wireless
#define Glorious_Model_OW_PID2              0x2011 // when connected via cable
#define Glorious_Model_D_PID                0x0033
#define Glorious_Model_DW_PID1              0x2023 // Wireless
#define Glorious_Model_DW_PID2              0x2012 // When connected via cable
#define Everest_GT100_PID                   0x0029
#define ZET_FURY_PRO_PID                    0x1007
#define Fl_Esports_F11_PID                  0x0049
#define RGB_KEYBOARD_0016PID                0x0016
#define GENESIS_THOR_300_PID                0x0090
#define GENESIS_XENON_200_PID               0x1007
#define RGB_KEYBOARD_010CPID                0x010C

#define MAX_EXPECTED_REPORT_SIZE 2048

struct expected_report
{
    unsigned int   id;
    unsigned int   size; // Up to MAX_EXPECTED_REPORT_SIZE!
    unsigned char* cmd_buf    = nullptr;
    unsigned int   cmd_size;
    hid_device*    cmd_device = nullptr;
    hid_device*    device     = nullptr;
    std::string    cmd_path;
    std::string    dev_path;
    unsigned char* response   = nullptr;

    expected_report(unsigned int id, unsigned size) : id(id), size(size) {}
    expected_report(unsigned int id, unsigned size, unsigned char* cmd_buf, unsigned int cmd_size) : id(id), size(size), cmd_buf(cmd_buf), cmd_size(cmd_size) {}
};

typedef std::vector<expected_report> expected_reports;


static std::set<std::string> sinowealth_claimed_devices;
static std::recursive_mutex sinowealth_registry_mutex;
/*---------------------------------------------------------*\
| Has this device already been built this pass?             |
\*---------------------------------------------------------*/
static bool SinowealthClaimDevice(const std::string& device_id)
{
    std::lock_guard<std::recursive_mutex> lock(sinowealth_registry_mutex);

    bool succ = sinowealth_claimed_devices.insert(device_id).second;
    return succ;
}

/*---------------------------------------------------------*\
| Free the device claim when the device is unplugged        |
\*---------------------------------------------------------*/
static bool SinowealthUnclaimDevice(const std::string& device_id)
{
    std::lock_guard<std::recursive_mutex> lock(sinowealth_registry_mutex);

    return sinowealth_claimed_devices.erase(device_id);
}

static bool DetectUsages(hid_device_info* info, std::string name, unsigned int device_count_expected, expected_reports& reports)
{
    std::lock_guard<std::recursive_mutex> lock(sinowealth_registry_mutex);

    unsigned int  device_count       = 0;
    unsigned char tmp_buf[MAX_EXPECTED_REPORT_SIZE];

    /*-----------------------------------------------------------------------------------------------*\
    | Sinowealth controllers report many collections on the same interface, usage page and usage id   |
    | And the detector WILL be called for each one                                                    |
    | We can't know if detector was called for the 1st time (first collection), or 2nd, 3rd, etc...   |
    | To not rely on luck in this question, instead we use a "Claims" system, where each HID path is  |
    | recorded and prohibited from re-use. Any subsequent detector calls on the same device will not  |
    | find the expected number of unclaimed devices to work with, and will thus fail.                 |
    \*-----------------------------------------------------------------------------------------------*/

    if(sinowealth_claimed_devices.count(info->path) != 0)
    {
        /*-------------------------------------------------------------------------*\
        | Avoid double-enumerating if the arrived device is already in use          |
        \*-------------------------------------------------------------------------*/
        return false;
    }

    hid_device_info* info_enum = hid_enumerate(info->vendor_id, info->product_id);
    hid_device_info* info_temp = info_enum;
    std::vector<std::string> matching_paths;
    std::vector<hid_device*> matching_devs;
    std::vector<bool> path_in_use_flag;

    /*---------------------------------------------------------------*\
    | Run a preliminary check                                         |
    | We do not claim devices during this stage, but all other        |
    | threads are prevented from climing any by a mutex               |
    \*---------------------------------------------------------------*/
    while(info_temp)
    {
        /*-------------------------------------------------------------------------*\
        | NOTE: VID and PID are already fultered for by hid_enumerate()             |
        \*-------------------------------------------------------------------------*/
        if(info_temp->usage_page == info->usage_page       // constant 0xFF00
        && sinowealth_claimed_devices.count(info_temp->path) == 0)
        {
            matching_paths.push_back(info_temp->path);
        }
        info_temp = info_temp->next;
    }

    hid_free_enumeration(info_enum);

    LOG_TRACE("[%s] Found %d unclaimed matching HID devices", name.c_str(), int(matching_paths.size()));

    if(matching_paths.size() < device_count_expected)
    {
        return false;
    }

    matching_devs.resize(matching_paths.size());
    path_in_use_flag.resize(matching_paths.size());

    for(size_t i = 0; i < matching_paths.size(); ++i)
    {
        hid_device* dev = hid_open_path(matching_paths[i].c_str());
        if(dev)
        {
            matching_devs[i] = dev;
        }
        else
        {
            LOG_ERROR("[%s] Couldn't open path \"HID: %s\", do we have enough permissions?", name.c_str(), matching_paths[i].c_str());
        }
    }

    /*---------------------------------------------------------------*\
    | Find the correct devices for each expected report               |
    \*---------------------------------------------------------------*/
    for(expected_report& report: reports)
    {
        memset(tmp_buf, 0x00, sizeof(tmp_buf));
        tmp_buf[0] = report.id;

        if(report.cmd_buf != nullptr)
        {
            for(size_t i = 0; i < matching_paths.size(); ++i)
            {
                /*--------------------------------------------------------------------------------------*\
                | If we need to send a command before requesting data, send it and flag the report       |
                | NOTE: Never probe more than one report at a time! Responces may get mixed up           |
                \*--------------------------------------------------------------------------------------*/
                if(hid_send_feature_report(matching_devs[i], report.cmd_buf, report.cmd_size) > -1)
                {
                    report.cmd_device = matching_devs[i];
                    report.cmd_path   = matching_paths[i];
                    SinowealthClaimDevice(matching_paths[i]);
                    path_in_use_flag[i] = true;

                    LOG_TRACE("[%s] Successfully sent command for ReportId 0x%02X to device at location \"HID: %s\", handle: 0x%08X", name.c_str(), report.id, matching_paths[i].c_str(), matching_devs[i]);
                }
            }
        }

        /*------------------------------------------------------*\
        | Now we try to receive an answer for the command probe  |
        \*------------------------------------------------------*/
        if(report.cmd_buf == nullptr || report.cmd_device != nullptr)
        {
            for(size_t i = 0; i < matching_paths.size(); ++i)
            {
                /*---------------------------------------------------------------------------*\
                | If device actually responds to expected report ID, set a flag               |
                \*---------------------------------------------------------------------------*/
                if(hid_get_feature_report(matching_devs[i], tmp_buf, report.size) > -1)
                {
                    device_count++;
                    report.device   = matching_devs[i];
                    report.dev_path = matching_paths[i];
                    SinowealthClaimDevice(matching_paths[i]);
                    path_in_use_flag[i] = true;

                    report.response = new unsigned char[report.size];
                    std::memcpy(report.response, tmp_buf, report.size);

                    LOG_TRACE("[%s] Successfully received feature ReportId 0x%02X from device at location \"HID: %s\", handle: 0x%08X", name.c_str(), report.id, matching_paths[i].c_str(), matching_devs[i]);
                }
            }
        }
    }

    bool failed = device_count < reports.size();
    /*-----------------------------------------------------------*\
    | Clean up                                                    |
    \*-----------------------------------------------------------*/
    for(size_t i = 0; i < matching_paths.size(); ++i)
    {
        if(failed || !path_in_use_flag[i])
        {
            hid_close(matching_devs[i]);
            matching_devs[i] = nullptr;
        }
    }
    /*-----------------------------------------------------------*\
    | If we found less devices than expected - sad, lets clean up |
    \*-----------------------------------------------------------*/
    if(device_count < reports.size())
    {
        for(expected_report& report: reports)
        {
            if(report.response != nullptr)
            {
                delete[] report.response;
                report.response = nullptr;
            }
            SinowealthUnclaimDevice(report.cmd_path);
            SinowealthUnclaimDevice(report.dev_path);
        }

        reports.clear();

        return false;
    }

    return true;
}

DetectedControllers DetectGenesisXenon200(hid_device_info* info, const std::string name)
{
    DetectedControllers detected_controllers;
    expected_reports    reports{expected_report(0x04, 154), expected_report(0x08, 9)};

    if(DetectUsages(info, name, 5, reports))
    {
        hid_device* dev     = reports.at(0).device;
        hid_device* cmd_dev = reports.at(1).device;

        GenesisXenon200Controller* controller         = new GenesisXenon200Controller(dev, cmd_dev, info->path, name);
        RGBController_GenesisXenon200* rgb_controller = new RGBController_GenesisXenon200(controller, [dev_path = reports.at(0).dev_path, cmd_path = reports.at(1).dev_path](){ SinowealthUnclaimDevice(dev_path); SinowealthUnclaimDevice(cmd_path); });

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectZetFuryPro(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;

    expected_reports reports{expected_report(0x04, 59)};
    if(DetectUsages(info, name, 5, reports))
    {
        hid_device* dev = reports.at(0).device;

        if(dev)
        {
            SinowealthController1007*     controller     = new SinowealthController1007(dev, info->path, name);
            RGBController_Sinowealth1007* rgb_controller = new RGBController_Sinowealth1007(controller, [dev_path = reports.at(0).dev_path](){ SinowealthUnclaimDevice(dev_path); });

            detected_controllers.push_back(rgb_controller);
        }
    }
    return(detected_controllers);
}

DetectedControllers DetectSinowealthMouse(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;

    unsigned char command[6] = {0x05, 0x11, 0x00, 0x00, 0x00, 0x00};
    expected_reports reports{expected_report(0x04, 520, command, sizeof(command))};

    if(DetectUsages(info, name, 3, reports))
    {
        hid_device *dev     = reports.at(0).device;
        hid_device *dev_cmd = reports.at(0).cmd_device;

        if(dev && dev_cmd)
        {
            SinowealthController*     controller     = new SinowealthController(dev, dev_cmd, info->path, name);
            RGBController_Sinowealth* rgb_controller = new RGBController_Sinowealth(controller, [dev_path = reports.at(0).dev_path, cmd_path = reports.at(0).cmd_path](){ SinowealthUnclaimDevice(dev_path); SinowealthUnclaimDevice(cmd_path); });

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

DetectedControllers DetectGMOW_Cable(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        SinowealthGMOWController* controller     = new SinowealthGMOWController(dev, info->path, GMOW_CABLE_CONNECTED, name);
        RGBController_GMOW*       rgb_controller = new RGBController_GMOW(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectGMOW_Dongle(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        SinowealthGMOWController* controller     = new SinowealthGMOWController(dev, info->path, GMOW_DONGLE_CONNECTED, name);
        RGBController_GMOW*       rgb_controller = new RGBController_GMOW(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

// static void DetectSinowealthKeyboard16(hid_device_info* info, const std::string& name)
// {
//     unsigned char command[6] = {0x05, 0x83, 0x00, 0x00, 0x00, 0x00};
//     expected_reports reports{expected_report(0x06, 1032, command, sizeof(command))};
//     if(!DetectUsages(info, name, 3, reports))
//     {
//         return;
//     }
//     hid_device *dev = reports.at(0).device;
//     hid_device *dev_cmd = reports.at(0).cmd_device;
//     if(dev && dev_cmd)
//     {
//         SinowealthKeyboard16Controller*     controller     = new SinowealthKeyboard16Controller(dev_cmd, dev, info->path, name);
//         RGBController_SinowealthKeyboard16* rgb_controller = new RGBController_SinowealthKeyboard16(controller, [dev_path = reports.at(0).dev_path, cmd_path = reports.at(0).cmd_path](){ SinowealthUnclaimDevice(dev_path); SinowealthUnclaimDevice(cmd_path); });
//
//         DetectionManager::get()->RegisterRGBController(rgb_controller);
//     }
// }

// static void DetectSinowealthKeyboard(hid_device_info* info, const std::string& name)
// {
//     unsigned char command[6] = {0x05, 0x83, 0xB6, 0x00, 0x00, 0x00};
//     expected_reports reports{expected_report(0x06, 1032, command, sizeof(command))};
//     if(!DetectUsages(info, name, 3, reports))
//     {
//         return;
//     }
//
//     hid_device *dev      = reports.at(0).device;
//     hid_device *dev_cmd  = reports.at(0).cmd_device;
//
//     if(dev && dev_cmd)
//     {
//         SinowealthKeyboardController*     controller     = new SinowealthKeyboardController(dev_cmd, dev, info->path, name);
//         RGBController_SinowealthKeyboard* rgb_controller = new RGBController_SinowealthKeyboard(controller, [dev_path = reports.at(0).dev_path, cmd_path = reports.at(0).cmd_path](){ SinowealthUnclaimDevice(dev_path); SinowealthUnclaimDevice(cmd_path); });
//
//         DetectionManager::get()->RegisterRGBController(rgb_controller);
//     }
// }

DetectedControllers DetectSinowealthGenesisKeyboard(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    unsigned int        pid = info->product_id;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        SinowealthKeyboard90Controller*     controller     = new SinowealthKeyboard90Controller(dev, info->path, pid, name);
        RGBController_SinowealthKeyboard90* rgb_controller = new RGBController_SinowealthKeyboard90(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectSinowealthKeyboard10c(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    unsigned char       command[7] = {0x06, 0x82, 0x01, 0x00, 0x01, 0x00, 0x06};
    expected_reports    reports{expected_report(0x06, 520, command, 520)};

    if(DetectUsages(info, name, 3, reports))
    {
        hid_device *dev        = reports.at(0).device;
        unsigned char model_id = reports.at(0).response[13];

        if(dev)
        {
            if(sinowealth_10c_keyboards.find(model_id) != sinowealth_10c_keyboards.end())
            {
                SinowealthKeyboard10cController*     controller     = new SinowealthKeyboard10cController(dev, info->path, sinowealth_10c_keyboards.at(model_id).device_name);
                RGBController_SinowealthKeyboard10c* rgb_controller = new RGBController_SinowealthKeyboard10c(controller, model_id, [dev_path = reports.at(0).dev_path](){ SinowealthUnclaimDevice(dev_path); });

                detected_controllers.push_back(rgb_controller);
            }
            else
            {
                hid_close(dev);
            }
        }
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_P("Glorious Model O / O-",            DetectSinowealthMouse,              SINOWEALTH_VID, Glorious_Model_O_PID,                   0xFF00          );
REGISTER_HID_DETECTOR_P("Glorious Model D / D-",            DetectSinowealthMouse,              SINOWEALTH_VID, Glorious_Model_D_PID,                   0xFF00          );
REGISTER_HID_DETECTOR_P("Everest GT-100 RGB",               DetectSinowealthMouse,              SINOWEALTH_VID, Everest_GT100_PID,                      0xFF00          );
REGISTER_HID_DETECTOR_IPU("ZET Fury Pro",                   DetectZetFuryPro,                   SINOWEALTH_VID, ZET_FURY_PRO_PID,                   1,  0xFF00, 1       );
REGISTER_HID_DETECTOR_PU("Glorious Model O / O- Wireless",  DetectGMOW_Dongle,                  SINOWEALTH_VID, Glorious_Model_OW_PID1,                 0xFFFF, 1       );
REGISTER_HID_DETECTOR_PU("Glorious Model O / O- Wireless",  DetectGMOW_Cable,                   SINOWEALTH_VID, Glorious_Model_OW_PID2,                 0xFFFF, 0x0000  );
REGISTER_HID_DETECTOR_PU("Glorious Model D / D- Wireless",  DetectGMOW_Dongle,                  SINOWEALTH_VID, Glorious_Model_DW_PID1,                 0xFFFF, 0x0000  );
REGISTER_HID_DETECTOR_PU("Glorious Model D / D- Wireless",  DetectGMOW_Cable,                   SINOWEALTH_VID, Glorious_Model_DW_PID2,                 0xFFFF, 0x0000  );
REGISTER_HID_DETECTOR_PU("Genesis Xenon 200",               DetectGenesisXenon200,              SINOWEALTH_VID, GENESIS_XENON_200_PID,                  0xFF00, 1       );
REGISTER_HID_DETECTOR_IPU("Genesis Thor 300",               DetectSinowealthGenesisKeyboard,    SINOWEALTH_VID, GENESIS_THOR_300_PID,               1,  0xFF00, 1       );
REGISTER_HID_DETECTOR_IPU("Sinowealth Keyboard",            DetectSinowealthKeyboard10c,        SINOWEALTH_VID, RGB_KEYBOARD_010CPID,               1,  0xFF00, 1       );

// Sinowealth keyboards are disabled due to VID/PID pairs being reused from Redragon keyboards, which ended up in bricking the latter
//REGISTER_HID_DETECTOR_P("FL ESPORTS F11",                   DetectSinowealthKeyboard,   SINOWEALTH_VID, Fl_Esports_F11_PID,                             0xFF00          );
//REGISTER_HID_DETECTOR_P("Sinowealth Keyboard",              DetectSinowealthKeyboard16, SINOWEALTH_VID, RGB_KEYBOARD_0016PID,                           0xFF00          );
