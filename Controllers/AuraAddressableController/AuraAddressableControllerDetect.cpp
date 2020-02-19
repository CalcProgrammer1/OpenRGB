#include "AuraAddressableController.h"
#include "RGBController.h"
#include "RGBController_AuraAddressable.h"
#include <vector>
#include <hidapi/hidapi.h>

#define AURA_ADDRESSABLE_VID 0x0B05
#define AURA_ADDRESSABLE_PID 0x1872

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

    dev = hid_open(AURA_ADDRESSABLE_VID, AURA_ADDRESSABLE_PID, 0);

    if( dev )
    {
        AuraAddressableController* controller = new AuraAddressableController(dev);

        RGBController_AuraAddressable* rgb_controller = new RGBController_AuraAddressable(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}
