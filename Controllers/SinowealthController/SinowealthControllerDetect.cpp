#include "Detector.h"
#include "SinowealthController.h"
#include "RGBController.h"
#include "RGBController_Sinowealth.h"
#include <hidapi/hidapi.h>

#define SINOWEALTH_VID       0x258A
#define Glorious_Model_O_PID 0x0036
#define Glorious_Model_D_PID 0x0033
#define Everest_GT100_PID    0x0029

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    const char *    name;
} sinowealth_device;

#define SINOWEALTH_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

/******************************************************************************************\
*                                                                                          *
*   DetectSinowealthControllers                                                            *
*                                                                                          *
*       Tests the USB address to see if a Sinowealth controller exists there.              *
*                                                                                          *
\******************************************************************************************/

void DetectSinowealthControllers(hid_device_info* info, const std::string& name)
{
#ifdef _WIN32

    /*-------------------------------------------------------------------------------------------------*\
    | Sinowealth devices use 3 different Report IDs on the same Usage Page.                             |
    | The 4 on 0xFF00 is for RGB, 7 is Unknown and 5 is for Commands.                                   |
    | HOWEVER, we can NOT get the report ids reliably, we only have USAGES and they are the SAME for    |
    | all three (1), so we try to rely on their order being the same. If it's not, we're screwed.       |
    \*-------------------------------------------------------------------------------------------------*/
     hid_device* usages[3];
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
     if(usage_count == 3)
     {
         SinowealthController* controller = new SinowealthController(usages[0], usages[2], info->path);
         RGBController_Sinowealth* rgb_controller = new RGBController_Sinowealth(controller);
         rgb_controller->name = name;
         ResourceManager::get()->RegisterRGBController(rgb_controller);
     }
     else
     {
         for(int i = 0; i < usage_count; ++i)
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

#ifdef USE_HID_USAGE
REGISTER_HID_DETECTOR_P("Glorious Model O / O-", DetectSinowealthControllers, SINOWEALTH_VID, Glorious_Model_O_PID, 0xFF00);
REGISTER_HID_DETECTOR_P("Glorious Model D / D-", DetectSinowealthControllers, SINOWEALTH_VID, Glorious_Model_D_PID, 0xFF00);
REGISTER_HID_DETECTOR_P("Everest GT-100 RGB",    DetectSinowealthControllers, SINOWEALTH_VID, Everest_GT100_PID,    0xFF00);
#else
REGISTER_HID_DETECTOR_I("Glorious Model O / O-", DetectSinowealthControllers, SINOWEALTH_VID, Glorious_Model_O_PID, 1);
REGISTER_HID_DETECTOR_I("Glorious Model D / D-", DetectSinowealthControllers, SINOWEALTH_VID, Glorious_Model_D_PID, 1);
REGISTER_HID_DETECTOR_I("Everest GT-100 RGB",    DetectSinowealthControllers, SINOWEALTH_VID, Everest_GT100_PID,    1);
#endif
