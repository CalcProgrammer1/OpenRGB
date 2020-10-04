#include "Detector.h"
#include "MSI3ZoneController.h"
#include "RGBController.h"
#include "RGBController_MSI3Zone.h"
#include <vector>
#include <hidapi/hidapi.h>

#define MSI_3_ZONE_KEYBOARD_VID 0x1770
#define MSI_3_ZONE_KEYBOARD_PID 0xFF00

/******************************************************************************************\
*                                                                                          *
*   DetectMSI3ZoneControllers                                                              *
*                                                                                          *
*       Tests the USB address to see if an MSI/SteelSeries 3-zone Keyboard controller      *
*       exists there.                                                                      *
*                                                                                          *
\******************************************************************************************/

void DetectMSI3ZoneControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev = NULL;

    hid_init();

    info = hid_enumerate(MSI_3_ZONE_KEYBOARD_VID, MSI_3_ZONE_KEYBOARD_PID);

    //Look for MSI/Steelseries 3-zone Keyboard
    while(info)
    {
        if((info->vendor_id        == MSI_3_ZONE_KEYBOARD_VID)
         &&(info->product_id       == MSI_3_ZONE_KEYBOARD_PID))
        {
            dev = hid_open_path(info->path);

            if( dev )
            {
                MSI3ZoneController* controller = new MSI3ZoneController(dev, info->path);

                RGBController_MSI3Zone* rgb_controller = new RGBController_MSI3Zone(controller);

                rgb_controllers.push_back(rgb_controller);
            }
        }

        info = info->next;
    }
}   /* DetectMSI3ZoneControllers() */

REGISTER_DETECTOR("MSI 3-Zone Laptop", DetectMSI3ZoneControllers);
