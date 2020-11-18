/******************************************************************************************\
*                                                                                          *
*   DetectRoccatControllers                                                                *
*                                                                                          *
*       Tests the USB address to see if a Roccat Kone Aimo controller exists there.        *
*                                                                                          *
\******************************************************************************************/

#include "Detector.h"
#include "RoccatKoneAimoController.h"
#include "RGBController.h"
#include "RGBController_RoccatKoneAimo.h"
#include <vector>
#include <hidapi/hidapi.h>

#define ROCCAT_VID                  0x1E7D
#define ROCCAT_KONE_AIMO_IFC        0
#define ROCCAT_KONE_AIMO_U          0x0
#define ROCCAT_KONE_AIMO_UP         0x0B
#define COUNT_ROCCATKONEAIMO_PIDS   (sizeof(RoccatKoneAimo_pids) / sizeof(RoccatKoneAimo_pids[ 0 ]))

static const unsigned short RoccatKoneAimo_pids[] =
{
    0x2E2C,     // Roccat Kone Aimo 16K
    0x2E27      // Roccat Kone Aimo
};

void DetectRoccatControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info*    info;
    hid_device*         dev;
    unsigned short      tmpPID;

    if (hid_init() < 0)
    {
        return;
    }

    for(unsigned int dev_idx = 0; dev_idx < COUNT_ROCCATKONEAIMO_PIDS; dev_idx++)
    {
        dev = NULL;

        tmpPID = RoccatKoneAimo_pids[dev_idx];
        info = hid_enumerate(ROCCAT_VID, tmpPID);

        while(info)
        {
            if((info->vendor_id        == ROCCAT_VID)
#ifdef USE_HID_USAGE
            &&(info->product_id        == tmpPID)
            &&(info->usage             == ROCCAT_KONE_AIMO_U)     //Usage and usage page required to get the correct interface
            &&(info->usage_page        == ROCCAT_KONE_AIMO_UP))
#else
            &&(info->interface_number  == ROCCAT_KONE_AIMO_IFC)   //Interface is only valid on Windows where there is > 1 interface
            &&(info->product_id        == tmpPID))
#endif
            {
                dev = hid_open_path(info->path);
                
                if (dev)
                {
                    RoccatKoneAimoController * controller = new RoccatKoneAimoController(dev, info->path);
                    RGBController_RoccatKoneAimo * rgb_controller = new RGBController_RoccatKoneAimo(controller);
                    rgb_controllers.push_back(rgb_controller);
                }
            }
            info = info->next;
        }
    }

    hid_free_enumeration(info);

}   /* DetectRoccatControllers() */

REGISTER_DETECTOR("Roccat", DetectRoccatControllers);
