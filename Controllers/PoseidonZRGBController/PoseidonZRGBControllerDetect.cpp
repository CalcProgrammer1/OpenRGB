#include "PoseidonZRGBController.h"
#include "RGBController.h"
#include "RGBController_PoseidonZRGB.h"
#include <vector>
#include "hidapi.h"

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
    hid_device_info* info;
    hid_device* dev = NULL;

    hid_init();

    info = hid_enumerate(TT_POSEIDON_Z_RGB_VID, TT_POSEIDON_Z_RGB_PID);

    //Look for Thermaltake Poseidon Z RGB, Interface 2
    while(info)
    {
        if((info->vendor_id == TT_POSEIDON_Z_RGB_VID)
         &&(info->product_id == TT_POSEIDON_Z_RGB_PID)
         &&(info->interface_number == 2))
        {
            dev = hid_open_path(info->path);
            break;
        }
        else
        {
            info = info->next;
        }
    }

    if( dev )
    {
        PoseidonZRGBController* controller = new PoseidonZRGBController(dev);

        RGBController_PoseidonZRGB* rgb_controller = new RGBController_PoseidonZRGB(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}
