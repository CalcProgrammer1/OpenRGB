#include "RGBFusion2Controller.h"
#include "RGBController.h"
#include "RGBController_RGBFusion2.h"
#include <vector>
#include <hidapi/hidapi.h>

#define RGB_FUSION_2_VID 0x048D
#define RGB_FUSION_2_PID 0x8297

/******************************************************************************************\
*                                                                                          *
*   DetectRGBFusion2Controllers                                                            *
*                                                                                          *
*       Detect RGB Fusion 2.0 controllers on the USB interface.                            *
*                                                                                          *
\******************************************************************************************/

void DetectRGBFusion2Controllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device* dev;

    //Look for RGB Fusion 2.0 Controller
    hid_init();

    dev = hid_open(RGB_FUSION_2_VID, RGB_FUSION_2_PID, 0);

    if( dev )
    {
        RGBFusion2Controller* controller = new RGBFusion2Controller(dev);

        RGBController_RGBFusion2* rgb_controller = new RGBController_RGBFusion2(controller);

        rgb_controllers.push_back(rgb_controller);
    }

}   /* DetectRGBFusionControllers() */