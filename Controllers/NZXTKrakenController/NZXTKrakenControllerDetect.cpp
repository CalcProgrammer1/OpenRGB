#include "Detector.h"
#include "NZXTKrakenController.h"
#include "RGBController.h"
#include "RGBController_NZXTKraken.h"
#include <vector>
#include <hidapi/hidapi.h>

#define NZXT_KRAKEN_VID 0x1E71
#define NZXT_KRAKEN_PID 0x170E

/******************************************************************************************\
*                                                                                          *
*   DetectNZXTKrakenControllers                                                            *
*                                                                                          *
*       Detect devices supported by the NZXTKraken driver                                  *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectNZXTKrakenControllers(std::vector<RGBController*> &rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(std::size_t device_idx = 0; device_idx < 1; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(NZXT_KRAKEN_VID, NZXT_KRAKEN_PID);

        //Look for NZXT Kraken devices
        while(info)
        {
            if((info->vendor_id == NZXT_KRAKEN_VID)
            &&(info->product_id == NZXT_KRAKEN_PID))
            {
                dev = hid_open_path(info->path);
                
                if( dev )
                {
                    NZXTKrakenController* controller = new NZXTKrakenController(dev);

                    RGBController_NZXTKraken* rgb_controller = new RGBController_NZXTKraken(controller);

                    rgb_controllers.push_back(rgb_controller);
                }
            }
            info = info->next;
        }
    }
}   /* DetectNZXTKrakenControllers() */

REGISTER_DETECTOR("NZXT Kraken", DetectNZXTKrakenControllers);
