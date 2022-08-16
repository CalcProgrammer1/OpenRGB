/*-----------------------------------------------------*\
| OpenRGB includes                                      |
\*-----------------------------------------------------*/
#include <hidapi/hidapi.h>
#include "Detector.h"
#include "LogManager.h"
#include "RGBController.h"

/*-----------------------------------------------------*\
| Corsair Peripheral specific includes                  |
\*-----------------------------------------------------*/
#include "CorsairPeripheralV2Devices.h"
#include "RGBController_CorsairV2Software.h"

#define CORSAIR_PERIPHERAL_CONTROLLER_NAME "Corsair V2 Peripheral"

/*-----------------------------------------------------*\
| Corsair vendor ID                                     |
\*-----------------------------------------------------*/
#define CORSAIR_VID                                 0x1B1C

void DetectCorsairV2SoftwareControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairPeripheralV2SWController*    controller      = new CorsairPeripheralV2SWController(dev, info->path, name, info->product_id);
        RGBController_CorsairV2SW*          rgb_controller  = new RGBController_CorsairV2SW(controller);
        rgb_controller->name                                = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectCorsairV2SoftwareControllers() */


/*-----------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                             |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Corsair K55 RGB PRO",             DetectCorsairV2SoftwareControllers, CORSAIR_VID, CORSAIR_K55_RGB_PRO_PID,       1, 0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K60 RGB PRO",             DetectCorsairV2SoftwareControllers, CORSAIR_VID, CORSAIR_K60_RGB_PRO_PID,       1, 0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K60 RGB PRO Low Profile", DetectCorsairV2SoftwareControllers, CORSAIR_VID, CORSAIR_K60_RGB_PRO_LP_PID,    1, 0xFF42);

/*-----------------------------------------------------------------------------------------------------*\
| Mousemat                                                                                              |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Corsair MM700",                   DetectCorsairV2SoftwareControllers, CORSAIR_VID, CORSAIR_MM700_PID,             1, 0xFF42);
