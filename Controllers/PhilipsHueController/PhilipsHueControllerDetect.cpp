#include "Detector.h"
#include "PhilipsHueController.h"
#include "PhilipsHueEntertainmentController.h"
#include "RGBController_PhilipsHue.h"
#include "RGBController_PhilipsHueEntertainment.h"
#include "SettingsManager.h"

#include "Bridge.h"
#include "HueDeviceTypes.h"

#ifdef _WIN32
#include "WinHttpHandler.h"
#else
#include "LinHttpHandler.h"
#endif

/******************************************************************************************\
*                                                                                          *
*   DetectPhilipsHueControllers                                                            *
*                                                                                          *
*       Detect Philips Hue lighting devices with RGB control                               *
*                                                                                          *
\******************************************************************************************/

void DetectPhilipsHueControllers(std::vector<RGBController*>& rgb_controllers)
{
    json                    hue_settings;

    /*-------------------------------------------------*\
    | Create an HTTP handler                            |
    \*-------------------------------------------------*/
#ifdef _WIN32
    std::shared_ptr<const hueplusplus::IHttpHandler> handler = std::make_shared<hueplusplus::WinHttpHandler>();
#else
    std::shared_ptr<const hueplusplus::IHttpHandler> handler = std::make_shared<hueplusplus::LinHttpHandler>();
#endif

    /*-------------------------------------------------*\
    | Get Philips Hue settings from settings manager    |
    \*-------------------------------------------------*/
    hue_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Setting_PhilipsHueBridges");

    /*-------------------------------------------------*\
    | Create a finder and find bridges                  |
    \*-------------------------------------------------*/
    hueplusplus::BridgeFinder finder(handler);
    std::vector<hueplusplus::BridgeFinder::BridgeIdentification> bridges;// = finder.FindBridges();

    /*-------------------------------------------------*\
    | If no bridges were detected, manually add bridge  |
    | IP and MAC (need to get these from file)          |
    \*-------------------------------------------------*/
    if(bridges.empty())
    {
        if(hue_settings.contains("bridges"))
        {
            hueplusplus::BridgeFinder::BridgeIdentification ident;

            if(hue_settings["bridges"][0].contains("ip"))
            {
                ident.ip = hue_settings["bridges"][0]["ip"];
            }

            if(hue_settings["bridges"][0].contains("mac"))
            {
                ident.mac = hue_settings["bridges"][0]["mac"];
            }

            bridges.push_back(ident);
        }
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
        if(hue_settings.contains("bridges"))
        {
            /*-------------------------------------------------*\
            | Add the username if it exists                     |
            \*-------------------------------------------------*/
            if(hue_settings["bridges"][0].contains("username"))
            {
                finder.AddUsername(bridges[0].mac, hue_settings["bridges"][0]["username"]);
            }
            
            /*-------------------------------------------------*\
            | Add the client key if it exists                   |
            \*-------------------------------------------------*/
            if(hue_settings["bridges"][0].contains("clientkey"))
            {
                finder.AddClientKey(bridges[0].mac, hue_settings["bridges"][0]["clientkey"]);
            }
        }

        /*-------------------------------------------------*\
        | If username was added, this should connect right  |
        | away.  If not, the user will have to push the     |
        | connect button on the bridge.                     |
        \*-------------------------------------------------*/
        hueplusplus::Bridge bridge = finder.GetBridge(bridges[0]);

        /*-------------------------------------------------*\
        | Check to see if we need to save the settings      |
        | Settings need to be saved if either username or   |
        | client key either do not exist or have changed    |
        \*-------------------------------------------------*/
        bool save_settings = false;
        bool use_entertainment = false;

        if(hue_settings.contains("bridges"))
        {
            if(hue_settings["bridges"][0].contains("username"))
            {
                if(hue_settings["bridges"][0]["username"] != bridge.getUsername())
                {
                    save_settings = true;
                }
            }
            else
            {
                save_settings = true;
            }

            if(hue_settings["bridges"][0].contains("clientkey"))
            {
                if(hue_settings["bridges"][0]["clientkey"] != bridge.getClientKey())
                {
                    use_entertainment = true;
                    save_settings = true;
                }
            }
            else
            {
                save_settings = true;
            }
        }

        /*-------------------------------------------------*\
        | Save the settings if needed                       |
        \*-------------------------------------------------*/
        if(save_settings)
        {
            hue_settings["bridges"][0]["username"] = bridge.getUsername();
            hue_settings["bridges"][0]["clientkey"] = bridge.getClientKey();
            hue_settings["bridges"][0]["entertainment"] = use_entertainment;
            
            ResourceManager::get()->GetSettingsManager()->SetSettings("Setting_PhilipsHueBridges", hue_settings);

            ResourceManager::get()->GetSettingsManager()->SaveSettings();
        }

        /*-------------------------------------------------*\
        | Get all groups from the bridge                    |
        \*-------------------------------------------------*/
        if(hue_settings["bridges"][0].contains("entertainment"))
        {
            use_entertainment = hue_settings["bridges"][0]["entertainment"];
        }

        /*-------------------------------------------------*\
        | Get all groups from the bridge                    |
        \*-------------------------------------------------*/
        if(use_entertainment)
        {
            std::vector<std::reference_wrapper<hueplusplus::Group>> groups = bridge.groups().getAll();

            if(groups.size() > 0)
            {
                /*-------------------------------------------------*\
                | Loop through all available groups and check to    |
                | see if any are Entertainment groups               |
                \*-------------------------------------------------*/
                for(unsigned int group_idx = 0; group_idx < groups.size(); group_idx++)
                {
                    if(groups[group_idx].get().getType() == "Entertainment")
                    {
                        PhilipsHueEntertainmentController*       new_controller = new PhilipsHueEntertainmentController(bridge, groups[group_idx].get());
                        RGBController_PhilipsHueEntertainment*   new_rgbcontroller = new RGBController_PhilipsHueEntertainment(new_controller);
                        rgb_controllers.push_back(new_rgbcontroller);
                    }
                }
            }
        }

        /*-------------------------------------------------*\
        | Get all lights from the bridge                    |
        \*-------------------------------------------------*/
        else
        {
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
                        PhilipsHueController*       new_controller = new PhilipsHueController(lights[light_idx].get(), bridge.getBridgeIP());
                        RGBController_PhilipsHue*   new_rgbcontroller = new RGBController_PhilipsHue(new_controller);
                        rgb_controllers.push_back(new_rgbcontroller);
                    }
                }
            }
        }
    }
}   /* DetectPhilipsHueControllers() */

REGISTER_DETECTOR("Philips Hue", DetectPhilipsHueControllers);
