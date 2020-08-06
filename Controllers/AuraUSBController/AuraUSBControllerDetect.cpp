#include "Detector.h"
#include "AuraAddressableController.h"
#include "AuraMainboardController.h"
#include "RGBController.h"
#include "RGBController_AuraUSB.h"
#include <vector>
#include <stdexcept>
#include <hidapi/hidapi.h>

#define AURA_USB_VID 0x0B05

#define NUM_ADDRESSABLE_PIDS 4
static const unsigned short addressable_pid_table[] =
    {
    0x1867,
    0x1872,
    0x1889,
    0x18A3
    };

#define NUM_MAINBOARD_PIDS 2
static const unsigned short mainboard_pid_table[] =
    {
    0x18f3,
    0x1939
    };

/******************************************************************************************\
*                                                                                          *
*   DetectAuraUSBControllers                                                               *
*                                                                                          *
*       Tests the USB address to see if an Asus Aura USB RGB header controller             *
*       exists there.                                                                      *
*                                                                                          *
\******************************************************************************************/

void DetectAuraUSBControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device* dev;

    //Look for Asus Aura addressable RGB header controller
    hid_init();

    for(int pid_idx = 0; pid_idx < NUM_ADDRESSABLE_PIDS; pid_idx++)
    {
        dev = hid_open(AURA_USB_VID, addressable_pid_table[pid_idx], 0);

        if( dev )
        {
            AuraAddressableController* controller = new AuraAddressableController(dev);

            RGBController_AuraUSB* rgb_controller = new RGBController_AuraUSB(controller);

            rgb_controllers.push_back(rgb_controller);
        }
    }

    for(int pid_idx = 0; pid_idx < NUM_MAINBOARD_PIDS; pid_idx++)
    {
        dev = hid_open(AURA_USB_VID, mainboard_pid_table[pid_idx], 0);

        if( dev )
        {
            try
            {
                AuraMainboardController* controller = new AuraMainboardController(dev);

                RGBController_AuraUSB* rgb_controller = new RGBController_AuraUSB(controller);

                rgb_controllers.push_back(rgb_controller);
            }
            catch(std::runtime_error&)
            {
                // reading the config table failed
            }
        }
    }
}   /* DetectAuraUSBControllers() */

REGISTER_DETECTOR("ASUS Aura USB", DetectAuraUSBControllers);
