#include "Detector.h"
#include "ThermaltakePoseidonZRGBController.h"
#include "RGBController.h"
#include "RGBController_ThermaltakePoseidonZRGB.h"
#include <vector>
#include <hidapi/hidapi.h>

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

void DetectPoseidonZRGBControllers(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        PoseidonZRGBController* controller = new PoseidonZRGBController(dev, info->path);
        RGBController_PoseidonZRGB* rgb_controller = new RGBController_PoseidonZRGB(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectPoseidonZRGBControllers() */

REGISTER_HID_DETECTOR_IP("Thermaltake Poseidon Z RGB", DetectPoseidonZRGBControllers, TT_POSEIDON_Z_RGB_VID, TT_POSEIDON_Z_RGB_PID, 1, 0xFF01);
