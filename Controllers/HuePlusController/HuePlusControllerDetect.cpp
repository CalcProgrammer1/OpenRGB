#include "HuePlusController.h"
#include "RGBController.h"
#include "RGBController_HuePlus.h"
#include "find_usb_serial_port.h"
#include <vector>

/******************************************************************************************\
*                                                                                          *
*   DetectHuePlusControllers                                                               *
*                                                                                          *
*       Detect devices supported by the HuePlus driver                                     *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectHuePlusControllers(std::vector<RGBController*> &rgb_controllers)
{
    HuePlusController* new_hueplus;
    RGBController_HuePlus* new_controller;

    std::string portname = find_usb_serial_port(0x04D8, 0x00DF);
    if( portname != "" )
    {
        new_hueplus = new HuePlusController();
        new_hueplus->Initialize((char *)portname.c_str());

        new_controller = new RGBController_HuePlus(new_hueplus);
        rgb_controllers.push_back(new_controller);
    }
}   /* DetectHuePlusControllers() */
