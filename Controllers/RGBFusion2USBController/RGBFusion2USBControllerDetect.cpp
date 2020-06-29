#include "RGBFusion2USBController.h"
#include "RGBController_RGBFusion2USB.h"
#include "dependencies/dmiinfo.h"

#define IT8297_VID 0x048D
#define IT8297_PID 0x8297

/******************************************************************************************\
*                                                                                          *
*   DetectRGBFusion2USBControllers                                                         *
*                                                                                          *
*       Detect RGB Fusion 2 devices that use IT8297 RGB controller                         *
*                                                                                          *
\******************************************************************************************/

void DetectRGBFusion2USBControllers(std::vector<RGBController*> &rgb_controllers)
{
    DMIInfo info;

    if (hid_init() < 0)
    {
        return;
    }

    hid_device_info * device_list = hid_enumerate(IT8297_VID, IT8297_PID);
    
    if (!device_list)
    {
        return;
    }

    hid_device_info * device = device_list;

    while (device)
    {
        hid_device * dev = hid_open_path(device->path);
        if (dev)
        {
            RGBFusion2USBController * controller = new RGBFusion2USBController(dev, device_list->path, info.getMainboard());
            RGBController_RGBFusion2USB * rgb_controller = new RGBController_RGBFusion2USB(controller);
            rgb_controllers.push_back(rgb_controller);
        }
        device = device->next;
    }

    hid_free_enumeration(device_list);
}
