/*---------------------------------------------------------*\
| ResourceManager.cpp                                       |
|                                                           |
|   OpenRGB Resource Manager controls access to application |
|   components including RGBControllers, I2C interfaces,    |
|   and network SDK components                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                27 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#ifdef _WIN32
#include <codecvt>
#include <locale>
#endif

#include <stdlib.h>
#include <string>
#include "cli.h"
#include "DetectionManager.h"
#include "ResourceManager.h"
#include "ProfileManager.h"
#include "LogManager.h"
#include "SettingsManager.h"
#include "NetworkClient.h"
#include "NetworkServer.h"
#include "filesystem.h"

using namespace std::chrono_literals;

/*---------------------------------------------------------*\
| ResourceManager Callback Functions                        |
\*---------------------------------------------------------*/
static void ResourceManagerDetectionCallback(void * this_ptr, unsigned int update_reason)
{
    ResourceManager* this_obj = (ResourceManager *)this_ptr;

    switch(update_reason)
    {
        case DETECTIONMANAGER_UPDATE_REASON_I2C_BUS_REGISTERED:
            this_obj->SignalResourceManagerUpdate(RESOURCEMANAGER_UPDATE_REASON_I2C_BUS_LIST_UPDATED);
            break;

        case DETECTIONMANAGER_UPDATE_REASON_DETECTION_STARTED:
            this_obj->SignalResourceManagerUpdate(RESOURCEMANAGER_UPDATE_REASON_DETECTION_STARTED);
            break;

        case DETECTIONMANAGER_UPDATE_REASON_RGBCONTROLLER_REGISTERED:
        case DETECTIONMANAGER_UPDATE_REASON_RGBCONTROLLER_UNREGISTERED:
        case DETECTIONMANAGER_UPDATE_REASON_RGBCONTROLLER_LIST_CLEARED:
            this_obj->UpdateDeviceList();
            break;

        case DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED:
            this_obj->SignalResourceManagerUpdate(RESOURCEMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);
            break;

        case DETECTIONMANAGER_UPDATE_REASON_DETECTION_COMPLETE:
            this_obj->SignalResourceManagerUpdate(RESOURCEMANAGER_UPDATE_REASON_DETECTION_COMPLETE);
            break;
    }
}

static void ResourceManagerNetworkClientCallback(void* this_ptr, unsigned int update_reason)
{
    ResourceManager* this_obj = (ResourceManager*)this_ptr;

    switch(update_reason)
    {
        case NETWORKCLIENT_UPDATE_REASON_DEVICE_LIST_UPDATED:
            this_obj->SignalResourceManagerUpdate(RESOURCEMANAGER_UPDATE_REASON_CLIENT_INFO_UPDATED);
            this_obj->UpdateDeviceList();
            break;

        case NETWORKCLIENT_UPDATE_REASON_DETECTION_STARTED:
            this_obj->SignalResourceManagerUpdate(RESOURCEMANAGER_UPDATE_REASON_DETECTION_STARTED);
            break;

        case NETWORKCLIENT_UPDATE_REASON_DETECTION_PROGRESS_CHANGED:
            this_obj->SignalResourceManagerUpdate(RESOURCEMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);
            break;

        case NETWORKCLIENT_UPDATE_REASON_DETECTION_COMPLETE:
            this_obj->SignalResourceManagerUpdate(RESOURCEMANAGER_UPDATE_REASON_DETECTION_COMPLETE);
            break;

        case NETWORKCLIENT_UPDATE_REASON_PROFILEMANAGER_PROFILE_LIST_UPDATED:
            this_obj->GetProfileManager()->SignalProfileManagerUpdate(PROFILEMANAGER_UPDATE_REASON_PROFILE_LIST_UPDATED);
            break;

        case NETWORKCLIENT_UPDATE_REASON_PROFILEMANAGER_ACTIVE_PROFILE_CHANGED:
            this_obj->GetProfileManager()->SignalProfileManagerUpdate(PROFILEMANAGER_UPDATE_REASON_ACTIVE_PROFILE_CHANGED);
            break;
    }
}

/*---------------------------------------------------------*\
| ResourceManager name for log entries                      |
\*---------------------------------------------------------*/
const char* RESOURCEMANAGER = "ResourceManager";

/*---------------------------------------------------------*\
| ResourceManager Global Instance Pointer                   |
\*---------------------------------------------------------*/
ResourceManager* ResourceManager::instance;

ResourceManager::ResourceManager()
{
    /*-----------------------------------------------------*\
    | Initialize global instance pointer the when created   |
    | There should only ever be one instance of             |
    | ResourceManager                                       |
    \*-----------------------------------------------------*/
    if(!instance)
    {
        instance = this;
    }
    /*-----------------------------------------------------*\
    | If, for whatever reason, ResourceManager already      |
    | exists, delete this instance as only one should exist |
    \*-----------------------------------------------------*/
    else
    {
        delete this;
        return;
    }

    /*-----------------------------------------------------*\
    | Initialize Detection Variables                        |
    \*-----------------------------------------------------*/
    auto_connection_client      = NULL;
    auto_connection_active      = false;
    detection_enabled           = true;
    init_finished               = false;
    plugin_manager              = NULL;

    SetupConfigurationDirectory();

    /*-----------------------------------------------------*\
    | Load settings from file                               |
    \*-----------------------------------------------------*/
    settings_manager        = new SettingsManager();

    settings_manager->LoadSettings(GetConfigurationDirectory() / "OpenRGB.json");

    /*-----------------------------------------------------*\
    | Configure the log manager                             |
    \*-----------------------------------------------------*/
    LogManager::get()->configure(settings_manager->GetSettings("LogManager"), GetConfigurationDirectory());

    /*-----------------------------------------------------*\
    | Initialize Server Instance                            |
    |   If configured, pass through full controller list    |
    |   including clients.  Otherwise, pass only local      |
    |   hardware controllers                                |
    \*-----------------------------------------------------*/
    json server_settings    = settings_manager->GetSettings("Server");
    bool legacy_workaround  = false;

    server                  = new NetworkServer();

    /*-----------------------------------------------------*\
    | Set server name                                       |
    \*-----------------------------------------------------*/
    std::string titleString = "OpenRGB ";
    titleString.append(VERSION_STRING);

    server->SetName(titleString);
    server->SetSettingsManager(settings_manager);

    /*-----------------------------------------------------*\
    | Enable legacy SDK workaround in server if configured  |
    \*-----------------------------------------------------*/
    if(server_settings.contains("legacy_workaround"))
    {
        legacy_workaround   = server_settings["legacy_workaround"];
    }

    if(legacy_workaround)
    {
        server->SetLegacyWorkaroundEnable(true);
    }

    /*-----------------------------------------------------*\
    | Load sizes list from file                             |
    \*-----------------------------------------------------*/
    profile_manager         = new ProfileManager(GetConfigurationDirectory());
    server->SetProfileManager(profile_manager);
}

ResourceManager::~ResourceManager()
{

}

/*---------------------------------------------------------*\
| ResourceManager Global Instance Accessor                  |
\*---------------------------------------------------------*/
ResourceManager* ResourceManager::get()
{
    /*-----------------------------------------------------*\
    | If ResourceManager does not exist yet, create it      |
    \*-----------------------------------------------------*/
    if(!instance)
    {
        instance = new ResourceManager();
    }

    return instance;
}

/*---------------------------------------------------------*\
| Resource Accessors                                        |
\*---------------------------------------------------------*/
std::vector<NetworkClient*>& ResourceManager::GetClients()
{
    return(clients);
}

filesystem::path ResourceManager::GetConfigurationDirectory()
{
    return(config_dir);
}

std::vector<i2c_smbus_interface*> & ResourceManager::GetI2CBusses()
{
    return DetectionManager::get()->GetI2CBuses();
}

LogManager* ResourceManager::GetLogManager()
{
    return LogManager::get();
}

PluginManagerInterface* ResourceManager::GetPluginManager()
{
    return(plugin_manager);
}

ProfileManager* ResourceManager::GetProfileManager()
{
    return(profile_manager);
}

std::vector<RGBController*> & ResourceManager::GetRGBControllers()
{
    return rgb_controllers;
}

NetworkServer* ResourceManager::GetServer()
{
    return(server);
}

SettingsManager* ResourceManager::GetSettingsManager()
{
    return(settings_manager);
}

void ResourceManager::SetConfigurationDirectory(const filesystem::path &directory)
{
    config_dir = directory;
    settings_manager->LoadSettings(directory / "OpenRGB.json");
    profile_manager->SetConfigurationDirectory(directory);
}

void ResourceManager::SetPluginManager(PluginManagerInterface* plugin_manager_ptr)
{
    plugin_manager = plugin_manager_ptr;
    server->SetPluginManager(plugin_manager);
}

/*---------------------------------------------------------*\
| Network Client Registration                               |
\*---------------------------------------------------------*/
void ResourceManager::RegisterNetworkClient(NetworkClient* new_client)
{
    new_client->RegisterNetworkClientCallback(ResourceManagerNetworkClientCallback, this);

    clients.push_back(new_client);
}

void ResourceManager::UnregisterNetworkClient(NetworkClient* network_client)
{
    /*-----------------------------------------------------*\
    | Stop the disconnecting client                         |
    \*-----------------------------------------------------*/
    network_client->StopClient();

    /*-----------------------------------------------------*\
    | Clear callbacks from the client before removal        |
    \*-----------------------------------------------------*/
    network_client->ClearCallbacks();

    /*-----------------------------------------------------*\
    | Find the client to remove and remove it from the      |
    | clients list                                          |
    \*-----------------------------------------------------*/
    std::vector<NetworkClient*>::iterator client_it = std::find(clients.begin(), clients.end(), network_client);

    if(client_it != clients.end())
    {
        clients.erase(client_it);
    }

    /*-----------------------------------------------------*\
    | Delete the client                                     |
    \*-----------------------------------------------------*/
    delete network_client;

    UpdateDeviceList();
}

/*---------------------------------------------------------*\
| Local Client Accessors                                    |
\*---------------------------------------------------------*/
NetworkClient* ResourceManager::GetLocalClient()
{
    return(auto_connection_client);
}

unsigned int ResourceManager::GetLocalClientProtocolVersion()
{
    return(auto_connection_client->GetProtocolVersion());
}

bool ResourceManager::IsLocalClient()
{
    return(auto_connection_active && (auto_connection_client != NULL) && auto_connection_client->GetLocal());
}

/*---------------------------------------------------------*\
| Callback Registration Functions                           |
\*---------------------------------------------------------*/
void ResourceManager::RegisterResourceManagerCallback(ResourceManagerCallback new_callback, void * new_callback_arg)
{
    ResourceManagerCallbackMutex.lock();

    for(size_t idx = 0; idx < ResourceManagerCallbacks.size(); idx++)
    {
        if(ResourceManagerCallbacks[idx] == new_callback && ResourceManagerCallbackArgs[idx] == new_callback_arg)
        {
            ResourceManagerCallbackMutex.unlock();

            LOG_TRACE("[%s] Tried to register an already registered ResourceManager callback, skipping.  Total callbacks registered: %d", RESOURCEMANAGER, ResourceManagerCallbacks.size());

            return;
        }
    }

    ResourceManagerCallbacks.push_back(new_callback);
    ResourceManagerCallbackArgs.push_back(new_callback_arg);

    ResourceManagerCallbackMutex.unlock();

    LOG_TRACE("[%s] Registered ResourceManager callback.  Total callbacks registered: %d", RESOURCEMANAGER, ResourceManagerCallbacks.size());
}

void ResourceManager::UnregisterResourceManagerCallback(ResourceManagerCallback callback, void * callback_arg)
{
    ResourceManagerCallbackMutex.lock();

    for(size_t idx = 0; idx < ResourceManagerCallbacks.size(); idx++)
    {
        if(ResourceManagerCallbacks[idx] == callback && ResourceManagerCallbackArgs[idx] == callback_arg)
        {
            ResourceManagerCallbacks.erase(ResourceManagerCallbacks.begin() + idx);
            ResourceManagerCallbackArgs.erase(ResourceManagerCallbackArgs.begin() + idx);
        }
    }

    ResourceManagerCallbackMutex.unlock();

    LOG_TRACE("[%s] Unregistered ResourceManager callback.  Total callbacks registered: %d", RESOURCEMANAGER, ResourceManagerCallbackArgs.size());
}

/*---------------------------------------------------------*\
| Functions to manage detection                             |
\*---------------------------------------------------------*/

bool ResourceManager::GetDetectionEnabled()
{
    return(detection_enabled);
}

unsigned int ResourceManager::GetDetectionPercent()
{
    if(auto_connection_active && (auto_connection_client != NULL) && auto_connection_client->GetLocal())
    {
        return auto_connection_client->DetectionManager_GetDetectionPercent();
    }
    else
    {
        return DetectionManager::get()->GetDetectionPercent();
    }
}

std::string ResourceManager::GetDetectionString()
{
    if(auto_connection_active && (auto_connection_client != NULL) && auto_connection_client->GetLocal())
    {
        return auto_connection_client->DetectionManager_GetDetectionString();
    }
    else
    {
        return DetectionManager::get()->GetDetectionString();
    }
}

void ResourceManager::RescanDevices()
{
    /*-----------------------------------------------------*\
    | If automatic local connection is active, the primary  |
    | instance is the local server, so send rescan requests |
    | to the automatic local connection client              |
    \*-----------------------------------------------------*/
    if(auto_connection_active && (auto_connection_client != NULL) && auto_connection_client->GetLocal())
    {
        auto_connection_client->SendRequest_RescanDevices();
    }

    /*-----------------------------------------------------*\
    | If detection is disabled and there is exactly one     |
    | client, the primary instance is the connected server, |
    | so send rescan requests to the first (and only)       |
    | client                                                |
    \*-----------------------------------------------------*/
    else if(!detection_enabled && clients.size() == 1)
    {
        clients[0]->SendRequest_RescanDevices();
    }

    /*-----------------------------------------------------*\
    | If detection is enabled, start detection              |
    \*-----------------------------------------------------*/
    if(detection_enabled)
    {
        DetectionManager::get()->BeginDetection();
    }
}

void ResourceManager::StopDeviceDetection()
{
    //TODO: Call DetectionManager::AbortDetection() or send SDK command if local client
}

void ResourceManager::UpdateDeviceList()
{
    DeviceListChangeMutex.lock();

    /*-----------------------------------------------------*\
    | Clear the controller list                             |
    \*-----------------------------------------------------*/
    rgb_controllers.clear();

    /*-----------------------------------------------------*\
    | Insert hardware controllers into controller list      |
    \*-----------------------------------------------------*/
    std::vector<RGBController*> rgb_controllers_hw          = DetectionManager::get()->GetRGBControllers();

    for(std::size_t rgb_controller_idx = 0; rgb_controller_idx < rgb_controllers_hw.size(); rgb_controller_idx++)
    {
        rgb_controllers.push_back(rgb_controllers_hw[rgb_controller_idx]);
    }

    /*-----------------------------------------------------*\
    | Insert client controllers into controller list        |
    \*-----------------------------------------------------*/
    for(std::size_t client_idx = 0; client_idx < clients.size(); client_idx++)
    {
        std::vector<RGBController*> rgb_controllers_client  = clients[client_idx]->GetRGBControllers();

        for(std::size_t rgb_controller_idx = 0; rgb_controller_idx < rgb_controllers_client.size(); rgb_controller_idx++)
        {
            rgb_controllers.push_back(rgb_controllers_client[rgb_controller_idx]);
        }
    }

    /*-----------------------------------------------------*\
    | Update server list                                    |
    \*-----------------------------------------------------*/
    json server_settings    = settings_manager->GetSettings("Server");
    bool all_controllers    = false;

    if(server_settings.contains("all_controllers"))
    {
        all_controllers     = server_settings["all_controllers"];
    }

    if(all_controllers)
    {
        server->SetControllers(rgb_controllers);
    }
    else
    {
        server->SetControllers(rgb_controllers_hw);
    }

    /*-----------------------------------------------------*\
    | Signal list has changed                               |
    \*-----------------------------------------------------*/
    DeviceListChangeMutex.unlock();

    /*-----------------------------------------------------*\
    | Signal device list update                             |
    \*-----------------------------------------------------*/
    SignalResourceManagerUpdate(RESOURCEMANAGER_UPDATE_REASON_DEVICE_LIST_UPDATED);
}

void ResourceManager::WaitForDetection()
{
    DetectionManager::get()->WaitForDetection();
}

/*---------------------------------------------------------*\
| Function to signal update callbacks                       |
\*---------------------------------------------------------*/
void ResourceManager::SignalResourceManagerUpdate(unsigned int update_reason)
{
    server->SignalResourceManagerUpdate(update_reason);

    ResourceManagerCallbackMutex.lock();

    for(std::size_t callback_idx = 0; callback_idx < ResourceManagerCallbacks.size(); callback_idx++)
    {
        ResourceManagerCallbacks[callback_idx](ResourceManagerCallbackArgs[callback_idx], update_reason);
    }

    ResourceManagerCallbackMutex.unlock();

    LOG_TRACE("[%s] ResourceManager update signalled: %d", RESOURCEMANAGER, update_reason);
}

void ResourceManager::SetupConfigurationDirectory()
{
    config_dir.clear();
#ifdef _WIN32
    const wchar_t* appdata = _wgetenv(L"APPDATA");
    if(appdata != NULL)
    {
        config_dir = appdata;
    }
#else
    const char* xdg_config_home = getenv("XDG_CONFIG_HOME");
    const char* home            = getenv("HOME");
    /*-----------------------------------------------------*\
    | Check both XDG_CONFIG_HOME and APPDATA environment    |
    | variables.  If neither exist, use current directory   |
    \*-----------------------------------------------------*/
    if(xdg_config_home != NULL)
    {
        config_dir = xdg_config_home;
    }
    else if(home != NULL)
    {
        config_dir = home;
        config_dir /= ".config";
    }
#endif


    /*-----------------------------------------------------*\
    | If a configuration directory was found, append OpenRGB|
    \*-----------------------------------------------------*/
    if(config_dir != "")
    {
        config_dir.append("OpenRGB");

        /*-------------------------------------------------*\
        | Create OpenRGB configuration directory if it      |
        | doesn't exist                                     |
        \*-------------------------------------------------*/
        filesystem::create_directories(config_dir);
    }
    else
    {
        config_dir = "./";
    }
}

bool ResourceManager::AttemptLocalConnection()
{
    LOG_DEBUG("[%s] Attempting local server connection...", RESOURCEMANAGER);

    bool success = false;

    auto_connection_client = new NetworkClient();

    std::string titleString = "OpenRGB ";
    titleString.append(VERSION_STRING);

    auto_connection_client->RequestLocalClient(true);
    auto_connection_client->SetName(titleString.c_str());
    auto_connection_client->StartClient();

    for(int timeout = 0; timeout < 10; timeout++)
    {
        if(auto_connection_client->GetConnected())
        {
            break;
        }
        std::this_thread::sleep_for(5ms);
    }

    if(!auto_connection_client->GetConnected())
    {
        LOG_TRACE("[%s] Client failed to connect", RESOURCEMANAGER);
        auto_connection_client->StopClient();
        LOG_TRACE("[%s] Client stopped", RESOURCEMANAGER);

        delete auto_connection_client;

        auto_connection_client = NULL;
    }
    else
    {
        ResourceManager::get()->RegisterNetworkClient(auto_connection_client);
        LOG_TRACE("[%s] Registered network client", RESOURCEMANAGER);

        success = true;

        /*-------------------------------------------------*\
        | Wait up to 5 seconds for the client connection to |
        | retrieve all controllers                          |
        \*-------------------------------------------------*/
        for(int timeout = 0; timeout < 1000; timeout++)
        {
            if(auto_connection_client->GetOnline())
            {
                break;
            }
            std::this_thread::sleep_for(5ms);
        }
    }

    return success;
}

void ResourceManager::Initialize(bool tryConnect, bool detectDevices, bool startServer, bool applyPostOptions)
{
    /*-----------------------------------------------------*\
    | Cache the parameters                                  |
    | TODO: Possibly cache them in the CLI file somewhere   |
    \*-----------------------------------------------------*/
    tryAutoConnect                  = tryConnect;
    detection_enabled               = detectDevices;
    start_server                    = startServer;
    apply_post_options              = applyPostOptions;

    /*-----------------------------------------------------*\
    | If enabled, try connecting to local server instead of |
    | detecting devices from this instance of OpenRGB       |
    \*-----------------------------------------------------*/
    if(tryAutoConnect)
    {
        /*-------------------------------------------------*\
        | Attempt connection to local server                |
        \*-------------------------------------------------*/
        if(AttemptLocalConnection())
        {
            LOG_DEBUG("[%s] Local OpenRGB server connected, running in client mode", RESOURCEMANAGER);

            /*---------------------------------------------*\
            | Set auto connection active flag and disable   |
            | detection if the local server was connected   |
            \*---------------------------------------------*/
            auto_connection_active  = true;
            detection_enabled       = false;

            profile_manager->UpdateProfileList();
        }

        tryAutoConnect = false;
    }

    /*-----------------------------------------------------*\
    | Initialize Saved Client Connections                   |
    \*-----------------------------------------------------*/
    json client_settings            = settings_manager->GetSettings("Client");

    if(client_settings.contains("clients"))
    {
        for(unsigned int client_idx = 0; client_idx < client_settings["clients"].size(); client_idx++)
        {
            NetworkClient * client = new NetworkClient();

            std::string titleString = "OpenRGB ";
            titleString.append(VERSION_STRING);

            std::string     client_ip   = client_settings["clients"][client_idx]["ip"];
            unsigned short  client_port = client_settings["clients"][client_idx]["port"];

            client->SetIP(client_ip.c_str());
            client->SetName(titleString.c_str());
            client->SetPort(client_port);

            client->StartClient();

            for(int timeout = 0; timeout < 100; timeout++)
            {
                if(client->GetConnected())
                {
                    break;
                }
                std::this_thread::sleep_for(10ms);
            }

            RegisterNetworkClient(client);
        }
    }

    /*-----------------------------------------------------*\
    | Start server if requested                             |
    \*-----------------------------------------------------*/
    if(start_server)
    {
        GetServer()->StartServer();
        if(!GetServer()->GetOnline())
        {
            LOG_DEBUG("[%s] Server failed to start", RESOURCEMANAGER);
        }
    }

    /*-----------------------------------------------------*\
    | Perform actual detection if enabled                   |
    \*-----------------------------------------------------*/
    if(detection_enabled)
    {
        LOG_DEBUG("[%s] Local OpenRGB server not found, running in standalone mode", RESOURCEMANAGER);

        DetectionManager::get()->RegisterDetectionCallback(ResourceManagerDetectionCallback, this);
        DetectionManager::get()->BeginDetection();
    }

    /*-----------------------------------------------------*\
    | Process command line arguments after detection only   |
    | if the pre-detection parsing indicated it should be   |
    | run                                                   |
    \*-----------------------------------------------------*/
    if(apply_post_options)
    {
        cli_post_detection();
    }

    init_finished = true;
}

void ResourceManager::WaitForInitialization()
{
    /*-----------------------------------------------------*\
    | A reliable sychronization of this kind is impossible  |
    | without the use of a `barrier` implementation, which  |
    | is only introduced in C++20                           |
    \*-----------------------------------------------------*/
    while(!init_finished)
    {
        std::this_thread::sleep_for(1ms);
    };
}
