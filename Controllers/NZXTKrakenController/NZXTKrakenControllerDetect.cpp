#include "NZXTKrakenController.h"
#include "RGBController.h"
#include "RGBController_NZXTKraken.h"
#include <vector>
#include <hidapi/hidapi.h>

#define NZXT_KRAKEN_VID 0x1E71
#define NZXT_KRAKEN_PID 0x170E

/******************************************************************************************\
*                                                                                          *
*   DetectNZXTKrakenControllers                                                            *
*                                                                                          *
*       Detect devices supported by the NZXTKraken driver                                  *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectNZXTKrakenControllers(std::vector<RGBController*> &rgb_controllers)
{
    hid_init();

    hid_device* dev = hid_open(NZXT_KRAKEN_VID, NZXT_KRAKEN_PID, nullptr);

    if( dev )
    {
        NZXTKrakenController* controller = new NZXTKrakenController(dev);

        RGBController_NZXTKraken* rgb_controller = new RGBController_NZXTKraken(controller);

        rgb_controllers.push_back(rgb_controller);
    }

}
