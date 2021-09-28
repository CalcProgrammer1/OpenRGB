#include "Detector.h"
#include "AP2Controller.h"
#include "RGBController.h"
#include "RGBController_AP2.h"
#include <hidapi/hidapi.h>

#define AP2_VID 0x04D9
#define AP2_PID_1 0xA291 /* C18 */
#define AP2_PID_2 0xA290 /* C15 */

/******************************************************************************************\
*   DetectAnnePro2Controllers                                                              *
*       Tests the USB address to see if an AnnePro2 w/QMK firmware keyboard is there.      *
\******************************************************************************************/
void DetectAP2Controllers(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if (dev)
    {
        AP2Controller*     controller     = new AP2Controller(dev, info->path);
        RGBController_AP2* rgb_controller = new RGBController_AP2(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_I("Anne Pro 2 C18 QMK/Shine", DetectAP2Controllers, AP2_VID, AP2_PID_1, 1);
REGISTER_HID_DETECTOR_I("Anne Pro 2 C15 QMK/Shine", DetectAP2Controllers, AP2_VID, AP2_PID_2, 1);
