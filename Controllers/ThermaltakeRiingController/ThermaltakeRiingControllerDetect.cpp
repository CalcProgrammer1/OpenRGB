#include "Detector.h"
#include "ThermaltakeRiingController.h"
#include "RGBController.h"
#include "RGBController_ThermaltakeRiing.h"
#include <vector>
#include <hidapi/hidapi.h>

#define THERMALTAKE_RIING_VID       0x264A
#define THERMALTAKE_RIING_PID_BEGIN 0x1FA5
#define THERMALTAKE_RIING_PID_END   0x1FB5

/******************************************************************************************\
*                                                                                          *
*   DetectThermaltakeRiingControllers                                                      *
*                                                                                          *
*       Tests the USB address to see if an AMD Wraith Prism controller exists there.       *
*                                                                                          *
\******************************************************************************************/

void DetectThermaltakeRiingControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(int pid = THERMALTAKE_RIING_PID_BEGIN; pid <= THERMALTAKE_RIING_PID_END; pid++)
    {
        dev = NULL;

        info = hid_enumerate(THERMALTAKE_RIING_VID, pid);

        //Look for Thermaltake Riing device
        while(info)
        {
            if((info->vendor_id == THERMALTAKE_RIING_VID)
            &&(info->product_id == pid))
            {
                dev = hid_open_path(info->path);
                break;
            }
            else
            {
                info = info->next;
            }
        }

        if(dev)
        {
            ThermaltakeRiingController* controller = new ThermaltakeRiingController(dev);

            RGBController_ThermaltakeRiing* rgb_controller = new RGBController_ThermaltakeRiing(controller);

            rgb_controllers.push_back(rgb_controller);
        }
    }
}   /* DetectThermaltakeRiingControllers() */

REGISTER_DETECTOR("Thermaltake Riing", DetectThermaltakeRiingControllers);
