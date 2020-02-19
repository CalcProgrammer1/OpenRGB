#include "AuraAddressableController.h"
#include "RGBController.h"
#include "RGBController_AuraAddressable.h"
#include <vector>
#include <hidapi/hidapi.h>

#define AURA_ADDRESSABLE_VID 0x0B05

#define NUM_PIDS 2
static const unsigned short pid_table[] =
    {
    0x1867,
    0x1872
    };

/******************************************************************************************\
*                                                                                          *
*   DetectAuraAddressableControllers                                                       *
*                                                                                          *
*       Tests the USB address to see if an Asus Aura addressable RGB header controller     *
*       exists there.                                                                      *
*                                                                                          *
\******************************************************************************************/

void DetectAuraAddressableControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device* dev;

    //Look for Asus Aura addressable RGB header controller
    hid_init();

    for(int pid_idx = 0; pid_idx < NUM_PIDS; pid_idx++)
    {
        dev = hid_open(AURA_ADDRESSABLE_VID, pid_table[pid_idx], 0);

        if( dev )
        {
            AuraAddressableController* controller = new AuraAddressableController(dev);

            RGBController_AuraAddressable* rgb_controller = new RGBController_AuraAddressable(controller);

            rgb_controllers.push_back(rgb_controller);
        }
    }
}
