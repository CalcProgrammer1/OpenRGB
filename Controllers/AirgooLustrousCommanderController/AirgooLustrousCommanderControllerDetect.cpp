#include "Detector.h"
#include "AirgooLustrousCommanderController.h"
#include "RGBController.h"
#include "RGBController_AirgooLustrousCommander.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Airgoo vendor ID                                      |
\*-----------------------------------------------------*/
#define AIRGOO_VID                         0x1A86

/*-----------------------------------------------------*\
| Lustrous Commander product IDs                        |
\*-----------------------------------------------------*/
#define AIRGOO_LUSTROUS_COMMANDER_PID    0xE6E0

/******************************************************************************************\
*                                                                                          *
*   DetectAirgooCommanderControllers                                                       *
*                                                                                          *
*       Tests the USB address to see if a Airgoo RGB controller exists there.              *
*                                                                                          *
\******************************************************************************************/

void DetectAirgooLustrousCommanderHIDControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AirgooLustrousCommanderController*     controller     = new AirgooLustrousCommanderController(dev, info->path);
        RGBController_AirgooLustrousCommander* rgb_controller = new RGBController_AirgooLustrousCommander(controller);

        rgb_controller->name = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("Airgoo Lustrous Commander",  DetectAirgooLustrousCommanderHIDControllers, AIRGOO_VID, AIRGOO_LUSTROUS_COMMANDER_PID);
