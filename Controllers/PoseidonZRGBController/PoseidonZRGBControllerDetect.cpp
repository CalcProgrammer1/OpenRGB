#include "PoseidonZRGBController.h"
#include "RGBController.h"
#include "RGBController_PoseidonZRGB.h"
#include <vector>
#include <hidapi/hidapi.h>

#define TT_POSEIDON_Z_RGB_VID 0x264A
#define TT_POSEIDON_Z_RGB_PID 0x3006

/******************************************************************************************\
*                                                                                          *
*   DetectPoseidonZRGBControllers                                                          *
*                                                                                          *
*       Tests the USB address to see if a Thermaltake Poseidon Z RGB Keyboard controller   *
*       exists there.                                                                      *
*                                                                                          *
\******************************************************************************************/

void DetectPoseidonZRGBControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device* dev;

    //Look for Thermaltake Poseidon Z RGB Keyboard
    hid_init();

    dev = hid_open(TT_POSEIDON_Z_RGB_VID, TT_POSEIDON_Z_RGB_PID, 0);

    if( dev )
    {
        PoseidonZRGBController* controller = new PoseidonZRGBController(dev);

        RGBController_PoseidonZRGB* rgb_controller = new RGBController_PoseidonZRGB(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}
