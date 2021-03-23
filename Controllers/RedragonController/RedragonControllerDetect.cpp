#include "Detector.h"
#include "RedragonM711Controller.h"
#include "RGBController.h"
#include "RGBController_RedragonM711.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define REDRAGON_MOUSE_VID              0x04D9
#define REDRAGON_MOUSE_USAGE_PAGE       0xFFA0
#define REDRAGON_M711_PID               0xFC30
#define REDRAGON_M715_PID               0xFC39
#define REDRAGON_M908_PID               0xFC4D

/******************************************************************************************\
*                                                                                          *
*   DetectRedragonMice                                                                     *
*                                                                                          *
*       Tests the USB address to see if a Redragon Mouse controller exists there.          *
*                                                                                          *
\******************************************************************************************/

void DetectRedragonMice(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        RedragonM711Controller* controller = new RedragonM711Controller(dev, info->path);
        RGBController_RedragonM711* rgb_controller = new RGBController_RedragonM711(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------*\
| Mice                                                                                                                                          |
\*---------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Redragon M711 Cobra",     DetectRedragonMice,      REDRAGON_MOUSE_VID,    REDRAGON_M711_PID,         2, REDRAGON_MOUSE_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Redragon M715 Dagger",    DetectRedragonMice,      REDRAGON_MOUSE_VID,    REDRAGON_M715_PID,         2, REDRAGON_MOUSE_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Redragon M908 Impact",    DetectRedragonMice,      REDRAGON_MOUSE_VID,    REDRAGON_M908_PID,         2, REDRAGON_MOUSE_USAGE_PAGE);
