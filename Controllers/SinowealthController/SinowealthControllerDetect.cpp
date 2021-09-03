#include "Detector.h"
#include "SinowealthController.h"
#include "SinowealthKeyboardController.h"
#include "SinowealthKeyboard16Controller.h"
#include "RGBController.h"
#include "RGBController_Sinowealth.h"
#include "RGBController_SinowealthKeyboard.h"
#include "RGBController_SinowealthKeyboard16.h"
#include <hidapi/hidapi.h>
#include "LogManager.h"

#define SINOWEALTH_VID       0x258A

#define Glorious_Model_O_PID 0x0036
#define Glorious_Model_D_PID 0x0033
#define Everest_GT100_PID    0x0029
#define Fl_Esports_F11_PID   0x0049
#define RGB_KEYBOARD_0016PID 0x0016

/******************************************************************************************\
*                                                                                          *
*   DetectSinowealthControllers                                                            *
*                                                                                          *
*       Tests the USB address to see if a Sinowealth controller exists there.              *
*                                                                                          *
\******************************************************************************************/

int GetUsageCount(hid_device_info* info, hid_device* usages[])
{
    int usage_count = 0;
    hid_device_info* info_temp = info;

    while(info_temp)
    {
        if(info_temp->vendor_id        == info->vendor_id        // constant SINOWEALTH_VID
        && info_temp->product_id       == info->product_id       // NON-constant
        && info_temp->usage_page       == info->usage_page)      // constant 0xFF00
        {
            if(usage_count > 3)
            {
                // Error! We only know what to do with those with 3 entries
                break;
            }
            usages[usage_count] = hid_open_path(info_temp->path);
            if(usages[usage_count])
            {
                ++usage_count;
            }
            // An error otherwise?
        }
        info_temp = info_temp->next;
    }

    return usage_count;
}


void DetectSinowealthMouse(hid_device_info* info, const std::string& name)
{
#ifdef USE_HID_USAGE

    /*-------------------------------------------------------------------------------------------------*\
    | Sinowealth devices use 3 different Report IDs on the same Usage Page.                             |
    | The 4 on 0xFF00 is for RGB, 7 is Unknown and 5 is for Commands.                                   |
    | HOWEVER, we can NOT get the report ids reliably, we only have USAGES and they are the SAME for    |
    | all three (1), so we try to rely on their order being the same. If it's not, we're screwed.       |
    \*-------------------------------------------------------------------------------------------------*/
     hid_device* usages[3];
     unsigned int usage_count = GetUsageCount(info, usages);
     
     if(usage_count == 3)
     {
         SinowealthController* controller = new SinowealthController(usages[0], usages[2], info->path);
         RGBController_Sinowealth* rgb_controller = new RGBController_Sinowealth(controller);
         rgb_controller->name = name;
         ResourceManager::get()->RegisterRGBController(rgb_controller);
     }
     else
     {
         for(unsigned int i = 0; i < usage_count; ++i)
         {
            if(usages[i])
            {
                hid_close(usages[i]);
            }
         }
     }
#else
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SinowealthController* controller = new SinowealthController(dev, dev, info->path);
        RGBController_Sinowealth* rgb_controller = new RGBController_Sinowealth(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
#endif
}

bool DetectCmdAndDataUsages(hid_device* usages[], int usage_count, std::string name, hid_device** dev_cmd, hid_device** dev_data)
{
    unsigned char tmp_buf[1032];

    // Try to find which device allow us to send ReportID 0x05
    for(int i = 0; i < usage_count; i++)
    {
        memset(tmp_buf, 0x00, sizeof(tmp_buf));
        tmp_buf[0] = 0x05;
        tmp_buf[1] = 0x83;

        if(hid_send_feature_report(usages[i], tmp_buf, 6) != -1)
        {
            *dev_cmd = usages[i];
            break;
        }
    }

    if(*dev_cmd == nullptr)
    {
        LOG_ERROR("[%s] Can't find working hid_device for ReportId 0x05", name.c_str());
        return false;
    }

    // Try other devices for support ReportID 0x06
    for(int i = 0; i < usage_count; i++)
    {
        if(usages[i] == *dev_cmd)
        {
            continue;
        }
        memset(tmp_buf, 0x00, sizeof(tmp_buf));
        tmp_buf[0] = 0x06;
        if(hid_get_feature_report(usages[i], tmp_buf, 1032) != -1)
        {
            *dev_data = usages[i];
            break;
        }
    }

    if(*dev_data == nullptr)
    {
        LOG_ERROR("[%s] Can't find working hid_device for ReportId 0x06", name.c_str());
        return false;
    }
    return true;
}

void DetectSinowealthKeyboard(hid_device_info* info, const std::string& name, unsigned int pid)
{
#ifdef USE_HID_USAGE
    hid_device* usages[3];

    unsigned int usage_count = GetUsageCount(info, usages);

    if(usage_count == 3)
    {
        RGBController *rgb_controller;
        if(pid == RGB_KEYBOARD_0016PID)
        {
            hid_device* dev_cmd = nullptr;
            hid_device* dev_data = nullptr;
            if(!DetectCmdAndDataUsages(usages, usage_count, name, &dev_cmd, &dev_data))
            {
                return;
            }

            SinowealthKeyboard16Controller* controller = new SinowealthKeyboard16Controller(dev_cmd, dev_data, info->path, name);
            rgb_controller = new RGBController_SinowealthKeyboard16(controller);
        }
        else
        {
            SinowealthKeyboardController* controller = new SinowealthKeyboardController(usages[1], usages[2], info->path);
            rgb_controller = new RGBController_SinowealthKeyboard(controller);
        }
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
    else
    {
        for(unsigned int i = 0; i < usage_count; ++i)
        {
           if(usages[i])
           {
               hid_close(usages[i]);
           }
        }
    }
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

void DetectSinowealthKeyboard49(hid_device_info* info, const std::string& name)
{
    DetectSinowealthKeyboard(info, name, Fl_Esports_F11_PID);
}

void DetectSinowealthKeyboard16(hid_device_info* info, const std::string& name)
{
    DetectSinowealthKeyboard(info, name, RGB_KEYBOARD_0016PID);
}


#ifdef USE_HID_USAGE
REGISTER_HID_DETECTOR_P("Glorious Model O / O-", DetectSinowealthMouse,    SINOWEALTH_VID, Glorious_Model_O_PID, 0xFF00);
REGISTER_HID_DETECTOR_P("Glorious Model D / D-", DetectSinowealthMouse,    SINOWEALTH_VID, Glorious_Model_D_PID, 0xFF00);
REGISTER_HID_DETECTOR_P("Everest GT-100 RGB",    DetectSinowealthMouse,    SINOWEALTH_VID, Everest_GT100_PID,    0xFF00);
REGISTER_HID_DETECTOR_P("FL ESPORTS F11",        DetectSinowealthKeyboard49, SINOWEALTH_VID, Fl_Esports_F11_PID,   0xFF00);
REGISTER_HID_DETECTOR_P("Sinowealth Keyboard",   DetectSinowealthKeyboard16, SINOWEALTH_VID, RGB_KEYBOARD_0016PID,   0xFF00);
#else
REGISTER_HID_DETECTOR_I("Glorious Model O / O-", DetectSinowealthMouse,    SINOWEALTH_VID, Glorious_Model_O_PID, 1);
REGISTER_HID_DETECTOR_I("Glorious Model D / D-", DetectSinowealthMouse,    SINOWEALTH_VID, Glorious_Model_D_PID, 1);
REGISTER_HID_DETECTOR_I("Everest GT-100 RGB",    DetectSinowealthMouse,    SINOWEALTH_VID, Everest_GT100_PID,    1);
REGISTER_HID_DETECTOR_I("FL ESPORTS F11",        DetectSinowealthKeyboard49, SINOWEALTH_VID, Fl_Esports_F11_PID,   1);
REGISTER_HID_DETECTOR_I("Sinowealth Keyboard",   DetectSinowealthKeyboard16, SINOWEALTH_VID, RGB_KEYBOARD_0016PID, 1);
#endif
