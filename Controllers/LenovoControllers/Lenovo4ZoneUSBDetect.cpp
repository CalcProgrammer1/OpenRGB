/*-------------------------------------------------------------------*\
|  Lenovo4ZoneUSBDetect.h                                             |
|                                                                     |
|  Describes zones for Lenovo 4-Zone Device                           |
\*-------------------------------------------------------------------*/

#include "Detector.h"
#include "LogManager.h"
#include "RGBController.h"
#include "Lenovo4ZoneUSBController.h"
#include "LenovoDevices4Zone.h"
#include "RGBController_Lenovo4ZoneUSB.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| vendor IDs                                            |
\*-----------------------------------------------------*/
#define ITE_VID                                 0x048D

/*-----------------------------------------------------*\
| Interface, Usage, and Usage Page                      |
\*-----------------------------------------------------*/
enum
{
    LENOVO_PAGE  = 0xFF89,
    LENOVO_USAGE = 0xCC
};

void DetectLenovo4ZoneUSBControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        Lenovo4ZoneUSBController*     controller      = new Lenovo4ZoneUSBController(dev, info->path, info->product_id);
        RGBController_Lenovo4ZoneUSB* rgb_controller  = new RGBController_Lenovo4ZoneUSB(controller);
        rgb_controller->name                     = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("Lenovo Ideapad 3-15ach6", DetectLenovo4ZoneUSBControllers, ITE_VID, IDEAPAD_315ACH6, LENOVO_PAGE, LENOVO_USAGE);
REGISTER_HID_DETECTOR_PU("Lenovo 5 2022", DetectLenovo4ZoneUSBControllers, ITE_VID, LEGION_5_2022_PID, LENOVO_PAGE, LENOVO_USAGE);
REGISTER_HID_DETECTOR_PU("Lenovo 5 2021", DetectLenovo4ZoneUSBControllers, ITE_VID, LEGION_5_2021_PID, LENOVO_PAGE, LENOVO_USAGE);
REGISTER_HID_DETECTOR_PU("Lenovo 5 2020", DetectLenovo4ZoneUSBControllers, ITE_VID, LEGION_5_2020_PID, LENOVO_PAGE, LENOVO_USAGE);
