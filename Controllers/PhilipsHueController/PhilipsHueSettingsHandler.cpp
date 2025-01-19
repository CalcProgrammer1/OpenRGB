#include "PhilipsHueSettingsHandler.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

#define HUE_SETTINGS ((hue_settings_type *)hue_settings)->hue_settings

typedef struct
{
    json hue_settings;
} hue_settings_type;

PhilipsHueSettingsHandler::PhilipsHueSettingsHandler()
{
    /*-------------------------------------------------*\
    | Create an object to hold the hue settings json    |
    | This cannot be a class member as json must not    |
    | be included in the header file, so it is held as  |
    | a void pointer instead.                           |
    \*-------------------------------------------------*/
    hue_settings = (void *)(new hue_settings_type);

    /*-------------------------------------------------*\
    | Get Philips Hue settings from settings manager    |
    \*-------------------------------------------------*/
    HUE_SETTINGS = ResourceManager::get()->GetSettingsManager()->GetSettings("PhilipsHueDevices");
}

PhilipsHueSettingsHandler::~PhilipsHueSettingsHandler()
{
    delete (hue_settings_type *)hue_settings;
}

std::size_t PhilipsHueSettingsHandler::GetBridgeCount()
{
    if(HUE_SETTINGS.contains("bridges"))
    {
        return(HUE_SETTINGS["bridges"].size());
    }
    else
    {
        return(0);
    }
}

std::string PhilipsHueSettingsHandler::GetBridgeIP(unsigned int bridge_idx)
{
    if(HUE_SETTINGS["bridges"][bridge_idx].contains("ip"))
    {
        return(HUE_SETTINGS["bridges"][bridge_idx]["ip"]);
    }
    else
    {
        return("");
    }
}

std::string PhilipsHueSettingsHandler::GetBridgeMAC(unsigned int bridge_idx)
{
    if(HUE_SETTINGS["bridges"][bridge_idx].contains("mac"))
    {
        return(HUE_SETTINGS["bridges"][bridge_idx]["mac"]);
    }
    else
    {
        return("");
    }
}

std::string PhilipsHueSettingsHandler::GetBridgeUsername(unsigned int bridge_idx)
{
    if(HUE_SETTINGS["bridges"][bridge_idx].contains("username"))
    {
        return(HUE_SETTINGS["bridges"][bridge_idx]["username"]);
    }
    else
    {
        return("");
    }
}

std::string PhilipsHueSettingsHandler::GetBridgeClientKey(unsigned int bridge_idx)
{
    if(HUE_SETTINGS["bridges"][bridge_idx].contains("clientkey"))
    {
        return(HUE_SETTINGS["bridges"][bridge_idx]["clientkey"]);
    }
    else
    {
        return("");
    }
}

bool PhilipsHueSettingsHandler::GetBridgeAutoconnect(unsigned int bridge_idx)
{
    if(HUE_SETTINGS["bridges"][bridge_idx].contains("autoconnect"))
    {
        return(HUE_SETTINGS["bridges"][bridge_idx]["autoconnect"]);
    }
    else
    {
        return(false);
    }
}

bool PhilipsHueSettingsHandler::GetBridgeUseEntertainment(unsigned int bridge_idx)
{
    if(HUE_SETTINGS["bridges"][bridge_idx].contains("entertainment"))
    {
        return(HUE_SETTINGS["bridges"][bridge_idx]["entertainment"]);
    }
    else
    {
        return(false);
    }
}

bool PhilipsHueSettingsHandler::BridgeHasUsername(unsigned int bridge_idx)
{
    return(HUE_SETTINGS["bridges"][bridge_idx].contains("username"));
}

bool PhilipsHueSettingsHandler::BridgeHasClientKey(unsigned int bridge_idx)
{
    return(HUE_SETTINGS["bridges"][bridge_idx].contains("clientkey"));
}

void PhilipsHueSettingsHandler::SetBridgeUsername(unsigned int bridge_idx, std::string username)
{
    HUE_SETTINGS["bridges"][bridge_idx]["username"] = username;
}

void PhilipsHueSettingsHandler::SetBridgeClientKey(unsigned int bridge_idx, std::string clientkey)
{
    HUE_SETTINGS["bridges"][bridge_idx]["clientkey"] = clientkey;
}

void PhilipsHueSettingsHandler::SetBridgeAutoconnect(unsigned int bridge_idx, bool auto_connect)
{
    HUE_SETTINGS["bridges"][bridge_idx]["autoconnect"] = auto_connect;
}

void PhilipsHueSettingsHandler::SetBridgeUseEntertainment(unsigned int bridge_idx, bool use_entertainment)
{
    HUE_SETTINGS["bridges"][bridge_idx]["entertainment"] = use_entertainment;
}

void PhilipsHueSettingsHandler::SaveSettings()
{
    ResourceManager::get()->GetSettingsManager()->SetSettings("PhilipsHueDevices", HUE_SETTINGS);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
