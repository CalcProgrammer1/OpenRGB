/*---------------------------------------------------------*\
| ResourceManager.h                                         |
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

#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <thread>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "DetectionManager.h"
#include "hidapi_wrapper.h"
#include "i2c_smbus.h"
#include "ResourceManagerInterface.h"
#include "filesystem.h"

using json = nlohmann::json;

class NetworkClient;
class NetworkServer;
class PluginManagerInterface;
class ProfileManager;
class RGBController;
class SettingsManager;

class ResourceManager: public ResourceManagerInterface
{
public:
    ResourceManager();
    ~ResourceManager();

    /*-----------------------------------------------------*\
    | ResourceManager Global Instance Accessor              |
    \*-----------------------------------------------------*/
    static ResourceManager*             get();

    /*-----------------------------------------------------*\
    | Resource Accessors                                    |
    \*-----------------------------------------------------*/
    std::vector<NetworkClient*>&        GetClients();
    filesystem::path                    GetConfigurationDirectory();
    std::vector<i2c_smbus_interface*>&  GetI2CBusses();
    PluginManagerInterface*             GetPluginManager();
    ProfileManager*                     GetProfileManager();
    std::vector<RGBController*>&        GetRGBControllers();
    NetworkServer*                      GetServer();
    SettingsManager*                    GetSettingsManager();

    void                                SetConfigurationDirectory(const filesystem::path &directory);
    void                                SetPluginManager(PluginManagerInterface* plugin_manager_ptr);

    /*-----------------------------------------------------*\
    | Network Client Registration                           |
    \*-----------------------------------------------------*/
    void                                RegisterNetworkClient(NetworkClient* new_client);
    void                                UnregisterNetworkClient(NetworkClient* network_client);

    /*-----------------------------------------------------*\
    | Local Client Accessors                                |
    \*-----------------------------------------------------*/
    NetworkClient*                      GetLocalClient();
    unsigned int                        GetLocalClientProtocolVersion();
    bool                                IsLocalClient();

    /*-----------------------------------------------------*\
    | Callback Registration Functions                       |
    \*-----------------------------------------------------*/
    void                                RegisterResourceManagerCallback(ResourceManagerCallback new_callback, void * new_callback_arg);
    void                                UnregisterResourceManagerCallback(ResourceManagerCallback new_callback, void * new_callback_arg);

    /*-----------------------------------------------------*\
    | Functions to manage detection                         |
    \*-----------------------------------------------------*/
    bool                                GetDetectionEnabled();
    unsigned int                        GetDetectionPercent();
    std::string                         GetDetectionString();
    void                                StopDeviceDetection();
    void                                RescanDevices();
    void                                ClearLocalDevices();
    void                                UpdateDeviceList();
    void                                WaitForDetection();

    /*-----------------------------------------------------*\
    | Functions to signal update callbacks                  |
    \*-----------------------------------------------------*/
    void                                SignalResourceManagerUpdate(unsigned int update_reason);

    void                                Initialize(bool tryConnect, bool detectDevices, bool startServer, bool applyPostOptions);

    void                                WaitForInitialization();

private:
    bool                                AttemptLocalConnection();
    void                                SetupConfigurationDirectory();

    /*-----------------------------------------------------*\
    | Static pointer to shared instance of ResourceManager  |
    \*-----------------------------------------------------*/
    static ResourceManager*                     instance;

    /*-----------------------------------------------------*\
    | Auto connection permitting flag                       |
    \*-----------------------------------------------------*/
    bool                                        tryAutoConnect;

    /*-----------------------------------------------------*\
    | Detection enabled flag                                |
    \*-----------------------------------------------------*/
    bool                                        detection_enabled;

    /*-----------------------------------------------------*\
    | Auto connection active flag                           |
    \*-----------------------------------------------------*/
    bool                                        auto_connection_active;

    /*-----------------------------------------------------*\
    | Auto connection client pointer                        |
    \*-----------------------------------------------------*/
    NetworkClient *                             auto_connection_client;

    /*-----------------------------------------------------*\
    | Auto connection permitting flag                       |
    \*-----------------------------------------------------*/
    bool                                        start_server;

    /*-----------------------------------------------------*\
    | Auto connection permitting flag                       |
    \*-----------------------------------------------------*/
    bool                                        apply_post_options;

    /*-----------------------------------------------------*\
    | Initialization completion flag                        |
    \*-----------------------------------------------------*/
    std::atomic<bool>                           init_finished;

    /*-----------------------------------------------------*\
    | Plugin Manager                                        |
    \*-----------------------------------------------------*/
    PluginManagerInterface*                     plugin_manager;

    /*-----------------------------------------------------*\
    | Profile Manager                                       |
    \*-----------------------------------------------------*/
    ProfileManager*                             profile_manager;

    /*-----------------------------------------------------*\
    | Settings Manager                                      |
    \*-----------------------------------------------------*/
    SettingsManager*                            settings_manager;

    /*-----------------------------------------------------*\
    | RGBControllers                                        |
    \*-----------------------------------------------------*/
    std::vector<RGBController*>                 rgb_controllers;

    /*-----------------------------------------------------*\
    | Network Server                                        |
    \*-----------------------------------------------------*/
    NetworkServer*                              server;

    /*-----------------------------------------------------*\
    | Network Clients                                       |
    \*-----------------------------------------------------*/
    std::vector<NetworkClient*>                 clients;

    /*-----------------------------------------------------*\
    | Device List Mutex                                     |
    \*-----------------------------------------------------*/
    std::mutex                                  DeviceListChangeMutex;

    /*-----------------------------------------------------*\
    | ResourceManager Callbacks                             |
    \*-----------------------------------------------------*/
    std::vector<ResourceManagerCallback>        ResourceManagerCallbacks;
    std::vector<void *>                         ResourceManagerCallbackArgs;
    std::mutex                                  ResourceManagerCallbackMutex;

    /*-----------------------------------------------------*\
    | OpenRGB configuration directory path                  |
    \*-----------------------------------------------------*/
    filesystem::path                            config_dir;
};
