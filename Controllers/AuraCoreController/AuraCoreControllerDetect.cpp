#include "Detector.h"
#include "AuraCoreController.h"
#include "RGBController.h"
#include "RGBController_AuraCore.h"
#include <vector>
#include <hidapi/hidapi.h>

#define AURA_CORE_VID 0x0B05

#define NUM_PIDS 3
static const unsigned short pid_table[] =
    {
    0x1854,
    0x1869,
    0x1866
    };

/******************************************************************************************\
*                                                                                          *
*   DetectAuraCoreControllers                                                              *
*                                                                                          *
*       Tests the USB address to see if an Asus ROG Aura Core controller exists there      *
*                                                                                          *
\******************************************************************************************/

void DetectAuraCoreControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device* dev;

    //Look for Asus ROG Aura Core RGB controller
    hid_init();

    for(int pid_idx = 0; pid_idx < NUM_PIDS; pid_idx++)
    {
        dev = hid_open(AURA_CORE_VID, pid_table[pid_idx], 0);

        if( dev )
        {
            AuraCoreController* controller = new AuraCoreController(dev);

            RGBController_AuraCore* rgb_controller = new RGBController_AuraCore(controller);

            rgb_controllers.push_back(rgb_controller);
        }
    }
}

REGISTER_DETECTOR("ASUS Aura Core", DetectAuraCoreControllers);
