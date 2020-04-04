#include "MSIMysticLightController.h"
#include "RGBController_MSIMysticLight.h"
#include <algorithm>

#define MSI_VID 0x1462

constexpr unsigned short SupportedPIDs[41] =
{
    31847, // MS_7C67
    31504, // MS_7B10
    31879, // MS_7C87
    31635, // MS_7B93
    31796, // MS_7C34
    31797, // MS_7C35
    31798, // MS_7C36
    31799, // MS_7C37
    31810, // MS_7C42
    31876, // MS_7C84
    31636, // MS_7B94
    31638, // MS_7B96
    31833, // MS_7C59
    31840, // MS_7C60
    31856, // MS_7C70
    31857, // MS_7C71
    31859, // MS_7C73
    31861, // MS_7C75
    31862, // MS_7C76
    31863, // MS_7C77
    31865, // MS_7C79
    31872, // MS_7C80
    31896, // MS_7C98
    31897, // MS_7C99
    31873, // MS_7C81
    31874, // MS_7C82
    31875, // MS_7C83
    31877, // MS_7C85
    31878, // MS_7C86
    31880, // MS_7C88
    31881, // MS_7C89
    17497, // MS_4459
    16036, // MS_3EA4
    36957, // MS_905D
    31888, // MS_7C90
    31889, // MS_7C91
    31890, // MS_7C92
    31892, // MS_7C94
    31893, // MS_7C95
    31894, // MS_7C96
    31830  // MS_7C56
};

/******************************************************************************************\
*                                                                                          *
*   DetectMSIMysticLightControllers                                                        *
*                                                                                          *
*       Detect MSI Mystic Light devices that use NCT6775 RGB controllers                   *
*                                                                                          *
\******************************************************************************************/

void DetectMSIMysticLightControllers(std::vector<RGBController*> &rgb_controllers)
{
    if(hid_init() < 0)
    {
        return;
    }

    hid_device_info * device_list = hid_enumerate(MSI_VID, 0);
    if(!device_list)
    {
        return;
    }

    hid_device_info * device = device_list;
    while(device)
    {
        if(std::find(std::begin(SupportedPIDs), std::end(SupportedPIDs), device->product_id) != std::end(SupportedPIDs))
        {
        hid_device * dev = hid_open_path(device->path);
            if(dev)
            {
                MSIMysticLightController * controller = new MSIMysticLightController(dev, device_list->path);
                RGBController_MSIMysticLight * rgb_controller = new RGBController_MSIMysticLight(controller);
                rgb_controllers.push_back(rgb_controller);
            }
        }
        device = device->next;
    }

    hid_free_enumeration(device_list);
}
