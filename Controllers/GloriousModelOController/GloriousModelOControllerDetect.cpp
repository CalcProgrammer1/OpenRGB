#include "GloriousModelOController.h"
#include "RGBController.h"
#include "RGBController_GloriousModelO.h"
#include <vector>
#include <hidapi/hidapi.h>

#define Glorious_Model_O_VID 0x258A
#define Glorious_Model_O_PID 0x0036

/******************************************************************************************\
*                                                                                          *
*   DetectGloriousModelController                                                          *
*                                                                                          *
*       Tests the USB address to see if a Glorious Model O controller exists there.        *
*                                                                                          *
\******************************************************************************************/

void DetectGloriousModelOControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev = NULL;

    hid_init();

    dev = NULL;

    info = hid_enumerate(Glorious_Model_O_VID, Glorious_Model_O_PID);

    //Look for Glorious Model O
    while(info)
    {
        if((info->vendor_id       == Glorious_Model_O_VID)
        &&(info->product_id       == Glorious_Model_O_PID)
#ifdef USE_HID_USAGE
        &&(info->interface_number == 1)
        &&(info->usage_page       == 0xFF00))
#else
        &&(info->interface_number == 1))
#endif
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
        GloriousModelOController* controller = new GloriousModelOController(dev);

        RGBController_GloriousModelO* rgb_controller = new RGBController_GloriousModelO(controller);

        rgb_controller->name = "Glorious Mouse";

        rgb_controllers.push_back(rgb_controller);
    }
}
