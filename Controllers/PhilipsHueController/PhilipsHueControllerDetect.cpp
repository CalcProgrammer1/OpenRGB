#include "Detector.h"
#include "PhilipsHueController.h"
#include "RGBController_PhilipsHue.h"
#include "Bridge.h"
#include "HueDeviceTypes.h"
#include "WinHttpHandler.h"

#include <fstream>
#include <iostream>

/******************************************************************************************\
*                                                                                          *
*   DetectPhilipsHueControllers                                                            *
*                                                                                          *
*       Detect Philips Hue lighting devices with RGB control                               *
*                                                                                          *
\******************************************************************************************/

void DetectPhilipsHueControllers(std::vector<RGBController*>& rgb_controllers)
{
    /*-------------------------------------------------*\
    | Create an HTTP handler                            |
    \*-------------------------------------------------*/
    std::shared_ptr<const hueplusplus::IHttpHandler> handler = std::make_shared<hueplusplus::WinHttpHandler>();

    /*-------------------------------------------------*\
    | Create a finder and find bridges                  |
    \*-------------------------------------------------*/
    hueplusplus::BridgeFinder finder(handler);
    std::vector<hueplusplus::BridgeFinder::BridgeIdentification> bridges = finder.FindBridges();

    /*-------------------------------------------------*\
    | If no bridges were detected, manually add bridge  |
    | IP and MAC (need to get these from file)          |
    \*-------------------------------------------------*/
    if(bridges.empty())
    {
        hueplusplus::BridgeFinder::BridgeIdentification ident;
        ident.ip = "192.168.3.242";
        ident.mac = "00:17:88:0A:23:60";
        bridges.push_back(ident);
    }

    /*-------------------------------------------------*\
    | If no bridges were found, return, otherwise       |
    | connect to the first bridge                       |
    \*-------------------------------------------------*/
    if(bridges.empty())
    {
        return;
    }
    else
    {
        /*-------------------------------------------------*\
        | Check if a saved username exists                  |
        \*-------------------------------------------------*/
        std::ifstream infile;
        infile.open("hue.txt");

        if(infile.good())
        {
            std::string username;
            std::getline(infile, username);

            /*-------------------------------------------------*\
            | Add the username if it exists                     |
            \*-------------------------------------------------*/
            finder.AddUsername(bridges[0].mac, username);
        }

        infile.close();

        /*-------------------------------------------------*\
        | If username was added, this should connect right  |
        | away.  If not, the user will have to push the     |
        | connect button on the bridge.                     |
        \*-------------------------------------------------*/
        hueplusplus::Bridge bridge = finder.GetBridge(bridges[0]);

        /*-------------------------------------------------*\
        | Save the username                                 |
        \*-------------------------------------------------*/
        std::ofstream outfile;
        outfile.open("hue.txt");

        outfile << bridge.getUsername();

        outfile.close();

        /*-------------------------------------------------*\
        | Get all lights from the bridge                    |
        \*-------------------------------------------------*/
        std::vector<std::reference_wrapper<hueplusplus::Light>> lights = bridge.lights().getAll();

        if(lights.size() > 0)
        {
            /*-------------------------------------------------*\
            | Loop through all available lights and add those   |
            | that have color (RGB) control capability          |
            \*-------------------------------------------------*/
            for(unsigned int light_idx = 0; light_idx < lights.size(); light_idx++)
            {
                if(lights[light_idx].get().hasColorControl())
                {
                    PhilipsHueController*       new_controller = new PhilipsHueController(lights[light_idx].get());
                    RGBController_PhilipsHue*   new_rgbcontroller = new RGBController_PhilipsHue(new_controller);
                    rgb_controllers.push_back(new_rgbcontroller);
                }
            }
        }
    }
}   /* DetectPhilipsHueControllers() */

REGISTER_DETECTOR("Philips Hue", DetectPhilipsHueControllers);
