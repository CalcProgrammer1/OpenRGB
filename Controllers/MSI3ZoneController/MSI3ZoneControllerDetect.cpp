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
    hid_device* dev;

    //Look for MSI/Steelseries 3-zone Keyboard
    hid_init();

    dev = hid_open(MSI_3_ZONE_KEYBOARD_VID, MSI_3_ZONE_KEYBOARD_PID, 0);

    if( dev )
    {
        MSI3ZoneController* controller = new MSI3ZoneController(dev);

        RGBController_MSI3Zone* rgb_controller = new RGBController_MSI3Zone(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}   /* DetectMSI3ZoneControllers() */

REGISTER_DETECTOR("MSI 3-Zone Laptop", DetectMSI3ZoneControllers);
