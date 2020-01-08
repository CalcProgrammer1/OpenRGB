#include "RGBFusion2Controller.h"
#include "RGBController_RGBFusion2.h"

#define IT8297_VID 0x048D
#define IT8297_PID 0x8297

/******************************************************************************************\
*                                                                                          *
*   DetectRGBFusion2Controllers                                                            *
*                                                                                          *
*       Detect RGB Fusion 2 devices that use IT8297 RGB controller                         *
*                                                                                          *
\******************************************************************************************/

void DetectRGBFusion2Controllers(std::vector<RGBController*> &rgb_controllers)
{
    if (hid_init() < 0)
        return;

    hid_device_info * device_list = hid_enumerate(IT8297_VID, IT8297_PID);
    if (!device_list)
        return;

    hid_device_info * device = device_list;
    while (device)
    {
        hid_device * dev = hid_open_path(device->path);
        if (dev) {
            RGBFusion2Controller * controller = new RGBFusion2Controller(dev, device_list->path);
            RGBController_RGBFusion2 * rgb_controller = new RGBController_RGBFusion2(controller);
            rgb_controllers.push_back(rgb_controller);
        }
        device = device->next;
    }

    hid_free_enumeration(device_list);
}
