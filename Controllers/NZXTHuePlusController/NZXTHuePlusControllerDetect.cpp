#include "Detector.h"
#include "NZXTHuePlusController.h"
#include "RGBController.h"
#include "RGBController_NZXTHuePlus.h"
#include "find_usb_serial_port.h"
#include <vector>

#define NZXT_HUE_PLUS_VID 0x04D8
#define NZXT_HUE_PLUS_PID 0x00DF

/******************************************************************************************\
*                                                                                          *
*   DetectNZXTHuePlusControllers                                                           *
*                                                                                          *
*       Detect devices supported by the NZXTHuePlus driver                                 *
*                                                                                          *
\******************************************************************************************/

void DetectNZXTHuePlusControllers(std::vector<RGBController*> &rgb_controllers)
{
    size_t i;
    HuePlusController* new_hueplus;
    RGBController_HuePlus* new_controller;

    std::vector<std::string *> ports = find_usb_serial_port(NZXT_HUE_PLUS_VID, NZXT_HUE_PLUS_PID);

    for (i = 0; i < ports.size(); i++)
    {
        if( *ports[i] != "" )
        {
            new_hueplus = new HuePlusController();
            new_hueplus->Initialize((char *)ports[i]->c_str());

            new_controller = new RGBController_HuePlus(new_hueplus);
            rgb_controllers.push_back(new_controller);
        }
    }
}   /* DetectHuePlusControllers() */

REGISTER_DETECTOR("NZXT Hue+", DetectNZXTHuePlusControllers);
