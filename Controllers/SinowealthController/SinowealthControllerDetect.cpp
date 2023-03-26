#include "Detector.h"
#include "SinowealthController.h"
#include "SinowealthController1007.h"
#include "SinowealthKeyboardController.h"
#include "SinowealthKeyboard16Controller.h"
#include "SinowealthKeyboard90Controller.h"
#include "SinowealthGMOWController.h"
#include "RGBController.h"
#include "RGBController_Sinowealth.h"
#include "RGBController_Sinowealth1007.h"
#include "RGBController_SinowealthKeyboard.h"
#include "RGBController_SinowealthKeyboard16.h"
#include "RGBController_SinowealthKeyboard90.h"
#include "RGBController_SinowealthGMOW.h"
#include <hidapi/hidapi.h>
#include "LogManager.h"

#define SINOWEALTH_VID          0x258A

#define Glorious_Model_O_PID    0x0036
#define Glorious_Model_OW_PID1  0x2022 // wireless
#define Glorious_Model_OW_PID2  0x2011 // when connected via cable
#define Glorious_Model_D_PID    0x0033
#define Glorious_Model_DW_PID1  0x2023 // Wireless
#define Glorious_Model_DW_PID2  0x2012 // When connected via cable
#define Everest_GT100_PID       0x0029
#define ZET_FURY_PRO_PID        0x1007
#define Fl_Esports_F11_PID      0x0049
#define RGB_KEYBOARD_0016PID    0x0016
#define GENESIS_THOR_300_PID    0x0090

/******************************************************************************************\
*                                                                                          *
*   DetectSinowealthControllers                                                            *
*                                                                                          *
*       Tests the USB address to see if a Sinowealth controller exists there.              *
*                                                                                          *
\******************************************************************************************/

#define MAX_EXPECTED_REPORT_SIZE 2048

struct expected_report
{
    unsigned int   id;
    unsigned int   size; // Up to MAX_EXPECTED_REPORT_SIZE!
    unsigned char* cmd_buf    = nullptr;
    unsigned int   cmd_size;
    hid_device*    cmd_device = nullptr;
    hid_device*    device     = nullptr;

    expected_report(unsigned int id, unsigned size) : id(id), size(size) {}
    expected_report(unsigned int id, unsigned size, unsigned char* cmd_buf, unsigned int cmd_size) : id(id), size(size), cmd_buf(cmd_buf), cmd_size(cmd_size) {}
};

typedef std::vector<expected_report*> expected_reports;

int GetDeviceCount(hid_device_info* info, unsigned int &device_count_total, unsigned int device_count_expected)
{
    hid_device_info* info_temp = info;

    while(info_temp)
    {
        if (info_temp->vendor_id        == info->vendor_id        // constant SINOWEALTH_VID
        &&  info_temp->product_id       == info->product_id       // NON-constant
        &&  info_temp->usage_page       == info->usage_page)      // constant 0xFF00
        {
            device_count_total++;
        }
        info_temp = info_temp->next;
    }

    /*----------------------------------------------------------------------*\
    | If we have an expected number and what's left is a multiple of it      |
    \*----------------------------------------------------------------------*/
    if(device_count_expected == 0 || device_count_total % device_count_expected == 0)
    {
        return true;
    }

    return false;
}

bool DetectUsages(hid_device_info* info, std::string name, unsigned int device_count_expected, expected_reports& reports)
{
    hid_device_info* info_temp = info;
    hid_device* device         = nullptr;

    bool          restart_flag       = false;
    unsigned int  device_count       = 0;
    unsigned int  device_count_total = 0;
    unsigned char tmp_buf[MAX_EXPECTED_REPORT_SIZE];

    /*-----------------------------------------------------------------------------------------------*\
    | Yeah, it might seem suboptimal to go over this list twice, but read this first:                 |
    | Sinowealth controllers report many collections on the same interface, usage page and usage id   |
    | We can't know if detector was called for the 1st time (first collection), or 2nd, 3rd, etc...   |
    | Relying on pure luck in this question is... not the best approach IMO, so here's how it works:  |
    | 1. Count remaining devices with our expected VID + PID + Usage Page                             |
    | 2. We know in advance how many collections currently expected device reports, so we compare     |
    |    remaining amount with expected amount                                                        |
    | 3. If remaining amount is a multiple of expected amount - we're on the first collection of one  |
    |    of connected devices, and proceed with finding expected reports                              |
    \*-----------------------------------------------------------------------------------------------*/
    if(!GetDeviceCount(info, device_count_total, device_count_expected))
    {
        LOG_DEBUG("[%s] Detection stage skipped - devices left %d (expected %d) ", name.c_str(), device_count_total, device_count_expected);
        reports.clear();
        return false;
    }

    /*---------------------------------------------------------------*\
    | Check all devices provided in hid_device_info                   |
    \*---------------------------------------------------------------*/
    while(info_temp)
    {
        /*----------------------------------------------------------------*\
        | If it's still our device                                         |
        \*----------------------------------------------------------------*/
        if(info_temp->vendor_id        == info->vendor_id        // constant SINOWEALTH_VID
        && info_temp->product_id       == info->product_id       // NON-constant
        && info_temp->usage_page       == info->usage_page)      // constant 0xFF00
        {
            /*----------------------------------------------------------*\
            | Open current device to check if it has expected report IDs |
            \*----------------------------------------------------------*/
            bool report_found = false;
            device = hid_open_path(info_temp->path);

            if(!device)
            {
                LOG_ERROR("[%s] Couldn't open path \"HID: %s\", do we have enough permissions?", name.c_str(), info_temp->path);
                reports.clear();
                return false;
            }

            for (expected_report* report: reports)
            {
                /*-----------------------------------------------------------*\
                | We shouldn't do any checks if device is already found       |
                \*-----------------------------------------------------------*/
                if(report->device != nullptr) continue;

                memset(tmp_buf, 0x00, sizeof(tmp_buf));
                tmp_buf[0] = report->id;

                /*--------------------------------------------------------------------------------------*\
                | If we need to send a command before requesting data, send it and flag the report       |
                | (DON'T TRY TO CREATE MORE THAN 1 EXPECTED REPORT SENDING COMMANDS)                     |
                \*--------------------------------------------------------------------------------------*/
                if(report->cmd_buf != nullptr && report->cmd_device == nullptr)
                {
                    if (hid_send_feature_report(device, report->cmd_buf, report->cmd_size) > -1)
                    {
                        restart_flag       = true; // Because Windows
                        report->cmd_device = device;
                        LOG_TRACE("[%s] Successfully sent command for ReportId 0x%02X to device at location \"HID: %s\", handle: %08X", name.c_str(), report->id, info_temp->path, device);
                    }
                }

                /*------------------------------------------------------*\
                | Now we try to request data for expected feature report |
                \*------------------------------------------------------*/
                if(report->cmd_buf == nullptr || report->cmd_device != nullptr)
                {
                    /*---------------------------------------------------------------------------*\
                    | If device actually responds to expected report ID, set a flag               |
                    \*---------------------------------------------------------------------------*/
                    if(hid_get_feature_report(device, tmp_buf, report->size) > -1)
                    {
                        device_count++;
                        report_found   = true;
                        report->device = device;
                        LOG_TRACE("[%s] Successfully requested feature ReportId 0x%02X from device at location \"HID: %s\", handle: %08X", name.c_str(), report->id, info_temp->path, device);
                    }
                }
            }

            /*-----------------------------------------------------------*\
            | If it doesn't - make sure to close it!                      |
            | Don't close if restart flag is set because we found cmd_dev |
            \*-----------------------------------------------------------*/
            if(!report_found && !restart_flag) hid_close(device);
        }

        info_temp    = restart_flag ? info : info_temp->next;
        restart_flag = false;

        /*-------------------------------------------------------------------------*\
        | If we found everything we expected, stop going through devices list       |
        | We don't want to go too far in case there are multiple Sinowealth devices |
        | with the same VID & PID                                                   |
        | (I don't care how unlikely it is, we must be prepared for everything)     |
        \*-------------------------------------------------------------------------*/
        if(device_count == reports.size()) info_temp = nullptr;
    }

    /*-----------------------------------------------------------*\
    | If we found less devices than expected - sad, lets clean up |
    \*-----------------------------------------------------------*/
    if(device_count < reports.size())
    {
        for (expected_report* report: reports)
        {
            if(report->device != nullptr)
            {
                hid_close(report->device);
            }
        }

        reports.clear();
        return false;
    }

    return true;
}

void DetectSinowealthMouse(hid_device_info* info, const std::string& name)
{
    unsigned int pid = info->product_id;
#ifdef USE_HID_USAGE

    /*-------------------------------------------------------------------------------------------------*\
    | Sinowealth devices use 3 (or more) different Report IDs on the same Usage Page.                   |
    | The 4 on 0xFF00 is for RGB, 7 is Unknown and 5 (or 8, or whatever...) is for Commands.            |
    \*-------------------------------------------------------------------------------------------------*/
    expected_reports* reports = new expected_reports();
    RGBController *rgb_controller;

    if(pid == ZET_FURY_PRO_PID)
    {
        reports->emplace_back(new expected_report(0x04, 59));

        if(!DetectUsages(info, name, 5, *reports)) return;

        SinowealthController1007* controller = new SinowealthController1007(reports->at(0)->device, info->path);
        rgb_controller                       = new RGBController_Sinowealth1007(controller);

    }
    /*-------------------------------------------------------------------------*\
    | When the GMOW is connected only via the wireless dongle, only one         |
    | device shows up (PID=2022), and RGB packets go to that device.            |
    | Same for when it is only plugged in via a cable but not a dongle (except  |
    | the device is PID=2011). However, when both are plugged in, packets       |
    | should only go to the cable connected device                              |
    \*-------------------------------------------------------------------------*/
    else if(pid == Glorious_Model_OW_PID1 || pid == Glorious_Model_DW_PID1) // if dongle
    {
        LOG_DEBUG("[%s] Detected connection via wireless dongle", name.c_str());
        hid_device_info* start = hid_enumerate(SINOWEALTH_VID,0);
        hid_device_info* curr  = start;

        while(curr)
        {
            if(curr->product_id == Glorious_Model_OW_PID2 || curr->product_id == Glorious_Model_DW_PID2)
            {
                delete reports;
                return;
            }
            curr = curr->next;
        }

        hid_device *dev                      = hid_open_path(info->path);
        SinowealthGMOWController* controller = new SinowealthGMOWController(dev, info->path, GMOW_DONGLE_CONNECTED);
        rgb_controller                       = new RGBController_GMOW(controller);
    }
    else if(pid == Glorious_Model_OW_PID2 || pid == Glorious_Model_DW_PID2)
    {
        LOG_DEBUG("[%s] Detected connection via USB cable", name.c_str());
        hid_device *dev                      = hid_open_path(info->path);
        SinowealthGMOWController* controller = new SinowealthGMOWController(dev, info->path, GMOW_CABLE_CONNECTED);
        rgb_controller                       = new RGBController_GMOW(controller);
    }
    else
    {
        unsigned char command[6] = {0x05, 0x11, 0x00, 0x00, 0x00, 0x00};
        reports->emplace_back(new expected_report(0x04, 520, command, sizeof(command)));

        if(!DetectUsages(info, name, 3, *reports)) return;

        SinowealthController* controller = new SinowealthController(reports->at(0)->device, reports->at(0)->cmd_device, info->path);
        rgb_controller                   = new RGBController_Sinowealth(controller);
    }

    rgb_controller->name = name;
    ResourceManager::get()->RegisterRGBController(rgb_controller);

    reports->clear();
#else
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        RGBController *rgb_controller;

        if(pid == ZET_FURY_PRO_PID)
        {
            SinowealthController1007* controller = new SinowealthController1007(dev, info->path);
            rgb_controller                       = new RGBController_Sinowealth1007(controller);
        }
        /*-------------------------------------------------------------------------*\
        | See above where USE_HID_USAGE is true for explanation of the detection    |
        | process for the GMOW                                                      |
        \*-------------------------------------------------------------------------*/
        else if(pid == Glorious_Model_OW_PID1 || pid == Glorious_Model_DW_PID1) // if dongle
        {
            LOG_DEBUG("[%s] Detected connection via wireless dongle", name.c_str());
            hid_device_info* start = hid_enumerate(SINOWEALTH_VID,0);
            hid_device_info* curr  = start;

            while(curr)
            {
                if(curr->product_id == Glorious_Model_OW_PID2 || curr->product_id == Glorious_Model_DW_PID2)
                {
                    return;
                }
                curr = curr->next;
            }

            hid_device *dev                      = hid_open_path(info->path);
            SinowealthGMOWController* controller = new SinowealthGMOWController(dev, info->path, GMOW_DONGLE_CONNECTED);
            rgb_controller                       = new RGBController_GMOW(controller);
        }
        else if(pid == Glorious_Model_OW_PID2 || pid == Glorious_Model_DW_PID2)
        {
            LOG_DEBUG("[%s] Detected connection via USB cable", name.c_str());
            hid_device *dev                      = hid_open_path(info->path);
            SinowealthGMOWController* controller = new SinowealthGMOWController(dev, info->path, GMOW_CABLE_CONNECTED);
            rgb_controller                       = new RGBController_GMOW(controller);
        }
        else
        {
            SinowealthController* controller = new SinowealthController(dev, dev, info->path);
            rgb_controller                   = new RGBController_Sinowealth(controller);
        }
        rgb_controller->name                 = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
#endif
}

void DetectSinowealthKeyboard(hid_device_info* info, const std::string& name)
{
    unsigned int pid = info->product_id;
#ifdef USE_HID_USAGE
    expected_reports* reports = new expected_reports();

    RGBController *rgb_controller;
    if(pid == RGB_KEYBOARD_0016PID)
    {
        unsigned char command[6] = {0x05, 0x83, 0x00, 0x00, 0x00, 0x00};
        reports->emplace_back(new expected_report(0x06, 1032, command, sizeof(command)));

        if(!DetectUsages(info, name, 3, *reports)) return;

        SinowealthKeyboard16Controller* controller = new SinowealthKeyboard16Controller(reports->at(0)->cmd_device, reports->at(0)->device, info->path, name);
        rgb_controller                             = new RGBController_SinowealthKeyboard16(controller);
    }
    else
    {
        unsigned char command[6] = {0x05, 0x83, 0xB6, 0x00, 0x00, 0x00};
        reports->emplace_back(new expected_report(0x06, 1032, command, sizeof(command)));

        if(!DetectUsages(info, name, 3, *reports)) return;

        SinowealthKeyboardController* controller = new SinowealthKeyboardController(reports->at(0)->cmd_device, reports->at(0)->device, info->path);
        rgb_controller                           = new RGBController_SinowealthKeyboard(controller);
    }
    rgb_controller->name = name;
    ResourceManager::get()->RegisterRGBController(rgb_controller);

    reports->clear();
#else
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        RGBController *rgb_controller;

        if(pid == RGB_KEYBOARD_0016PID)
        {
            SinowealthKeyboard16Controller* controller = new SinowealthKeyboard16Controller(dev, dev, info->path, name);
            rgb_controller                             = new RGBController_SinowealthKeyboard16(controller);
        }
        else
        {
            SinowealthController* controller = new SinowealthController(dev, dev, info->path);
            rgb_controller                   = new RGBController_Sinowealth(controller);
        }
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
#endif
}

void DetectSinowealthGenesisKeyboard(hid_device_info* info, const std::string& name)
{
    unsigned int pid = info->product_id;
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        if(pid == GENESIS_THOR_300_PID)
        {
            SinowealthKeyboard90Controller* controller         = new SinowealthKeyboard90Controller(dev, info->path, pid);
            RGBController_SinowealthKeyboard90* rgb_controller = new RGBController_SinowealthKeyboard90(controller);
            rgb_controller->name                               = name;

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}

#ifdef USE_HID_USAGE
REGISTER_HID_DETECTOR_P("Glorious Model O / O-",            DetectSinowealthMouse, SINOWEALTH_VID, Glorious_Model_O_PID,   0xFF00);
REGISTER_HID_DETECTOR_P("Glorious Model D / D-",            DetectSinowealthMouse, SINOWEALTH_VID, Glorious_Model_D_PID,   0xFF00);
REGISTER_HID_DETECTOR_P("Everest GT-100 RGB",               DetectSinowealthMouse, SINOWEALTH_VID, Everest_GT100_PID,      0xFF00);
REGISTER_HID_DETECTOR_IPU("ZET Fury Pro",                   DetectSinowealthMouse, SINOWEALTH_VID, ZET_FURY_PRO_PID, 1,    0xFF00, 1);
REGISTER_HID_DETECTOR_PU("Glorious Model O / O- Wireless",  DetectSinowealthMouse, SINOWEALTH_VID, Glorious_Model_OW_PID1, 0xFFFF, 1);
REGISTER_HID_DETECTOR_PU("Glorious Model O / O- Wireless",  DetectSinowealthMouse, SINOWEALTH_VID, Glorious_Model_OW_PID2, 0xFFFF, 0x0000);
REGISTER_HID_DETECTOR_PU("Glorious Model D / D- Wireless",  DetectSinowealthMouse, SINOWEALTH_VID, Glorious_Model_DW_PID1, 0xFFFF, 0x0000);
REGISTER_HID_DETECTOR_PU("Glorious Model D / D- Wireless",  DetectSinowealthMouse, SINOWEALTH_VID, Glorious_Model_DW_PID2, 0xFFFF, 0x0000);

REGISTER_HID_DETECTOR_IPU("Genesis Thor 300",               DetectSinowealthGenesisKeyboard, SINOWEALTH_VID, GENESIS_THOR_300_PID,   1, 0xFF00, 1);
//REGISTER_HID_DETECTOR_P("FL ESPORTS F11",        DetectSinowealthKeyboard, SINOWEALTH_VID, Fl_Esports_F11_PID,     0xFF00);
//REGISTER_HID_DETECTOR_P("Sinowealth Keyboard",   DetectSinowealthKeyboard, SINOWEALTH_VID, RGB_KEYBOARD_0016PID,   0xFF00);
#else
REGISTER_HID_DETECTOR_I("Glorious Model O / O-",            DetectSinowealthMouse, SINOWEALTH_VID, Glorious_Model_O_PID,   1);
REGISTER_HID_DETECTOR_I("Glorious Model D / D-",            DetectSinowealthMouse, SINOWEALTH_VID, Glorious_Model_D_PID,   1);
REGISTER_HID_DETECTOR_I("Everest GT-100 RGB",               DetectSinowealthMouse, SINOWEALTH_VID, Everest_GT100_PID,      1);
REGISTER_HID_DETECTOR_I("ZET Fury Pro",                     DetectSinowealthMouse, SINOWEALTH_VID, ZET_FURY_PRO_PID,       1);
REGISTER_HID_DETECTOR_I("Glorious Model O / O- Wireless",   DetectSinowealthMouse, SINOWEALTH_VID, Glorious_Model_OW_PID1, 1);
REGISTER_HID_DETECTOR_I("Glorious Model O / O- Wireless",   DetectSinowealthMouse, SINOWEALTH_VID, Glorious_Model_OW_PID2, 2);
REGISTER_HID_DETECTOR_I("Glorious Model D / D- Wireless",   DetectSinowealthMouse, SINOWEALTH_VID, Glorious_Model_DW_PID1, 2);
REGISTER_HID_DETECTOR_I("Glorious Model D / D- Wireless",   DetectSinowealthMouse, SINOWEALTH_VID, Glorious_Model_DW_PID2, 2);

REGISTER_HID_DETECTOR_I("Genesis Thor 300",      DetectSinowealthGenesisKeyboard, SINOWEALTH_VID, GENESIS_THOR_300_PID, 1);
//REGISTER_HID_DETECTOR_I("FL ESPORTS F11",        DetectSinowealthKeyboard, SINOWEALTH_VID, Fl_Esports_F11_PID,   1);
//REGISTER_HID_DETECTOR_I("Sinowealth Keyboard",   DetectSinowealthKeyboard, SINOWEALTH_VID, RGB_KEYBOARD_0016PID, 1);
#endif
