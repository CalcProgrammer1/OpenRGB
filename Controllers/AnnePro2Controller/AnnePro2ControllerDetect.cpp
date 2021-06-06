#include "Detector.h"
#include "AnnePro2Controller.h"
#include "RGBController.h"
#include "RGBController_AnnePro2.h"
#include <hidapi/hidapi.h>

#define ANNE_PRO_2_VID 0x04D9
#define ANNE_PRO_2_PID_1 0x8008
#define ANNE_PRO_2_PID_2 0x8009
#define ANNE_PRO_2_PID_3 0xA292
#define ANNE_PRO_2_PID_4 0xA293

/******************************************************************************************\
*                                                                                          *
*   DetectAnnePro2Controllers                                                              *
*                                                                                          *
*       Tests the USB address to see if an Obins Lab AnnePro2 keyboard exists there.       *
*                                                                                          *
\******************************************************************************************/

void DetectAnnePro2Controllers(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        AnnePro2Controller*     controller     = new AnnePro2Controller(dev, info->path);
        RGBController_AnnePro2* rgb_controller = new RGBController_AnnePro2(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_I("Anne Pro 2", DetectAnnePro2Controllers, ANNE_PRO_2_VID, ANNE_PRO_2_PID_1, 1);
REGISTER_HID_DETECTOR_I("Anne Pro 2", DetectAnnePro2Controllers, ANNE_PRO_2_VID, ANNE_PRO_2_PID_2, 1);
REGISTER_HID_DETECTOR_I("Anne Pro 2", DetectAnnePro2Controllers, ANNE_PRO_2_VID, ANNE_PRO_2_PID_3, 1);
REGISTER_HID_DETECTOR_I("Anne Pro 2", DetectAnnePro2Controllers, ANNE_PRO_2_VID, ANNE_PRO_2_PID_4, 1);
