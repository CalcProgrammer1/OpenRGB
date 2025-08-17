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
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#ifdef _WIN32
#include <codecvt>
#include <locale>
#endif

#include <stdlib.h>
#include <string>
#include <hidapi.h>
#include "cli.h"
#include "pci_ids/pci_ids.h"
#include "ResourceManager.h"
#include "ProfileManager.h"
#include "LogManager.h"
#include "SettingsManager.h"
#include "NetworkClient.h"
#include "NetworkServer.h"
#include "filesystem.h"
#include "StringUtils.h"

/*---------------------------------------------------------*\
| Translation Strings                                       |
\*---------------------------------------------------------*/
const char* I2C_ERR_WIN =   QT_TRANSLATE_NOOP("ResourceManager",
                                              "<h2>Some internal devices may not be detected:</h2>"
                                              "<p>One or more I2C or SMBus interfaces failed to initialize.</p>"
                                              "<p><b>RGB DRAM modules, some motherboards' onboard RGB lighting, and RGB Graphics Cards, will not be available in OpenRGB</b> without I2C or SMBus.</p>"
                                              "<h4>How to fix this:</h4>"
                                              "<p>On Windows, this is usually caused by a failure to load the WinRing0 driver.</p>"
                                              "<p>You must run OpenRGB as administrator at least once to allow WinRing0 to set up.</p>"
                                              "<p>See <a href='https://help.openrgb.org/'>help.openrgb.org</a> for additional troubleshooting steps if you keep seeing this message.<br></p>"
                                              "<h3>If you are not using internal RGB on a desktop this message is not important to you.</h3>");
const char* I2C_ERR_LINUX = QT_TRANSLATE_NOOP("ResourceManager",
                                              "<h2>Some internal devices may not be detected:</h2>"
                                              "<p>One or more I2C or SMBus interfaces failed to initialize.</p>"
                                              "<p><b>RGB DRAM modules, some motherboards' onboard RGB lighting, and RGB Graphics Cards, will not be available in OpenRGB</b> without I2C or SMBus.</p>"
                                              "<h4>How to fix this:</h4>"
                                              "<p>On Linux, this is usually because the i2c-dev module is not loaded.</p>"
                                              "<p>You must load the i2c-dev module along with the correct i2c driver for your motherboard. "
                                              "This is usually i2c-piix4 for AMD systems and i2c-i801 for Intel systems.</p>"
                                              "<p>See <a href='https://help.openrgb.org/'>help.openrgb.org</a> for additional troubleshooting steps if you keep seeing this message.<br></p>"
                                              "<h3>If you are not using internal RGB on a desktop this message is not important to you.</h3>");

const char* UDEV_MISSING =  QT_TRANSLATE_NOOP("ResourceManager",
                                              "<h2>WARNING:</h2>"
                                              "<p>The OpenRGB udev rules are not installed.</p>"
                                              "<p>Most devices will not be available unless running OpenRGB as root.</p>"
                                              "<p>If using AppImage, Flatpak, or self-compiled versions of OpenRGB you must install the udev rules manually</p>"
                                              "<p>See <a href='https://openrgb.org/udev'>https://openrgb.org/udev</a> to install the udev rules manually</p>");
const char* UDEV_MUTLI =    QT_TRANSLATE_NOOP("ResourceManager",
                                              "<h2>WARNING:</h2>"
                                              "<p>Multiple OpenRGB udev rules are installed.</p>"
                                              "<p>The udev rules file 60-openrgb.rules is installed in both /etc/udev/rules.d and /usr/lib/udev/rules.d.</p>"
                                              "<p>Multiple udev rules files can conflict, it is recommended to remove one of them.</p>");


const hidapi_wrapper default_wrapper =
{
    NULL,
    (hidapi_wrapper_send_feature_report)        hid_send_feature_report,
    (hidapi_wrapper_get_feature_report)         hid_get_feature_report,
    (hidapi_wrapper_get_serial_number_string)   hid_get_serial_number_string,
    (hidapi_wrapper_open_path)                  hid_open_path,
    (hidapi_wrapper_enumerate)                  hid_enumerate,
    (hidapi_wrapper_free_enumeration)           hid_free_enumeration,
    (hidapi_wrapper_close)                      hid_close,
    (hidapi_wrapper_error)                      hid_error
};

bool BasicHIDBlock::compare(hid_device_info* info)
{
    return ( (vid == info->vendor_id)
        && (pid == info->product_id)
#ifdef USE_HID_USAGE
        && ( (usage_page == HID_USAGE_PAGE_ANY)
            || (usage_page == info->usage_page) )
        && ( (usage      == HID_USAGE_ANY)
            || (usage      == info->usage) )
        && ( (interface  == HID_INTERFACE_ANY)
            || (interface  == info->interface_number ) )
#else
        && ( (interface  == HID_INTERFACE_ANY)
            || (interface  == info->interface_number ) )
#endif
            );
}

ResourceManager* ResourceManager::instance;

using namespace std::chrono_literals;

ResourceManager *ResourceManager::get()
{
    if(!instance)
    {
        instance = new ResourceManager();
    }

    return instance;
}

ResourceManager::ResourceManager()
{
    /*-----------------------------------------------------*\
    | Initialize Detection Variables                        |
    \*-----------------------------------------------------*/
    auto_connection_client      = NULL;
    auto_connection_active      = false;
    detection_enabled           = true;
    detection_percent           = 100;
    detection_string            = "";
    detection_is_required       = false;
    dynamic_detectors_processed = false;
    init_finished               = false;
    background_thread_running   = true;

    /*-----------------------------------------------------*\
    | Start the background detection thread in advance; it  |
    | will be suspended until necessary                     |
    \*-----------------------------------------------------*/
    DetectDevicesThread         = new std::thread(&ResourceManager::BackgroundThreadFunction, this);

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
    bool all_controllers    = false;
    bool legacy_workaround  = false;

    if(server_settings.contains("all_controllers"))
    {
        all_controllers     = server_settings["all_controllers"];
    }

    if(all_controllers)
    {
        server              = new NetworkServer(rgb_controllers);
    }
    else
    {
        server              = new NetworkServer(rgb_controllers_hw);
    }

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
    rgb_controllers_sizes   = profile_manager->LoadProfileToList("sizes", true);
}

ResourceManager::~ResourceManager()
{
    Cleanup();

    /*-----------------------------------------------------*\
    | Mark the background detection thread as not running   |
    | and then wake it up so it knows that it has to stop   |
    \*-----------------------------------------------------*/
    background_thread_running = false;
    BackgroundFunctionStartTrigger.notify_one();

    /*-----------------------------------------------------*\
    | Stop the background thread                            |
    \*-----------------------------------------------------*/
    if(DetectDevicesThread)
    {
        DetectDevicesThread->join();
        delete DetectDevicesThread;
        DetectDevicesThread = nullptr;
    }
}

void ResourceManager::RegisterI2CBus(i2c_smbus_interface *bus)
{
    LOG_INFO("[ResourceManager] Registering I2C interface: %s Device %04X:%04X Subsystem: %04X:%04X", bus->device_name, bus->pci_vendor, bus->pci_device,bus->pci_subsystem_vendor,bus->pci_subsystem_device);
    busses.push_back(bus);
}

std::vector<i2c_smbus_interface*> & ResourceManager::GetI2CBusses()
{
    return busses;
}

void ResourceManager::RegisterRGBController(RGBController *rgb_controller)
{
    /*-----------------------------------------------------*\
    | Mark this controller as locally owned                 |
    \*-----------------------------------------------------*/
    rgb_controller->flags &= ~CONTROLLER_FLAG_REMOTE;
    rgb_controller->flags |= CONTROLLER_FLAG_LOCAL;

    LOG_INFO("[%s] Registering RGB controller", rgb_controller->GetName().c_str());
    rgb_controllers_hw.push_back(rgb_controller);

    /*-----------------------------------------------------*\
    | If the device list size has changed, call the device  |
    | list changed callbacks                                |
    |                                                       |
    | TODO: If all detection is reworked to use             |
    | RegisterRGBController, tracking of previous list size |
    | can be removed and profile can be loaded per          |
    | controller before adding to list                      |
    \*-----------------------------------------------------*/
    if(rgb_controllers_hw.size() != detection_prev_size)
    {
        /*-------------------------------------------------*\
        | First, load sizes for the new controllers         |
        \*-------------------------------------------------*/
        for(unsigned int controller_size_idx = detection_prev_size; controller_size_idx < rgb_controllers_hw.size(); controller_size_idx++)
        {
            profile_manager->LoadDeviceFromListWithOptions(rgb_controllers_sizes, detection_size_entry_used, rgb_controllers_hw[controller_size_idx], true, false);
        }

        UpdateDeviceList();
    }

    detection_prev_size = (unsigned int)rgb_controllers_hw.size();

    UpdateDeviceList();
}

void ResourceManager::UnregisterRGBController(RGBController* rgb_controller)
{
    LOG_INFO("[%s] Unregistering RGB controller", rgb_controller->GetName().c_str());

    /*-----------------------------------------------------*\
    | Clear callbacks from the controller before removal    |
    \*-----------------------------------------------------*/
    rgb_controller->ClearCallbacks();

    /*-----------------------------------------------------*\
    | Find the controller to remove and remove it from the  |
    | hardware list                                         |
    \*-----------------------------------------------------*/
    std::vector<RGBController*>::iterator hw_it = std::find(rgb_controllers_hw.begin(), rgb_controllers_hw.end(), rgb_controller);

    if (hw_it != rgb_controllers_hw.end())
    {
        rgb_controllers_hw.erase(hw_it);
    }

    /*-----------------------------------------------------*\
    | Find the controller to remove and remove it from the  |
    | master list                                           |
    \*-----------------------------------------------------*/
    std::vector<RGBController*>::iterator rgb_it = std::find(rgb_controllers.begin(), rgb_controllers.end(), rgb_controller);

    if (rgb_it != rgb_controllers.end())
    {
        rgb_controllers.erase(rgb_it);
    }

    UpdateDeviceList();
}

std::vector<RGBController*> & ResourceManager::GetRGBControllers()
{
    return rgb_controllers;
}

void ResourceManager::RegisterI2CBusDetector(I2CBusDetectorFunction detector)
{
    i2c_bus_detectors.push_back(detector);
}

void ResourceManager::RegisterI2CDeviceDetector(std::string name, I2CDeviceDetectorFunction detector)
{
    i2c_device_detector_strings.push_back(name);
    i2c_device_detectors.push_back(detector);
}

void ResourceManager::RegisterI2CDIMMDeviceDetector(std::string name, I2CDIMMDeviceDetectorFunction detector, uint16_t jedec_id, uint8_t dimm_type)
{
    I2CDIMMDeviceDetectorBlock block;

    block.name          = name;
    block.function      = detector;
    block.jedec_id      = jedec_id;
    block.dimm_type     = dimm_type;

    i2c_dimm_device_detectors.push_back(block);
}

void ResourceManager::RegisterI2CPCIDeviceDetector(std::string name, I2CPCIDeviceDetectorFunction detector, uint16_t ven_id, uint16_t dev_id, uint16_t subven_id, uint16_t subdev_id, uint8_t i2c_addr)
{
    I2CPCIDeviceDetectorBlock block;

    block.name          = name;
    block.function      = detector;
    block.ven_id        = ven_id;
    block.dev_id        = dev_id;
    block.subven_id     = subven_id;
    block.subdev_id     = subdev_id;
    block.i2c_addr      = i2c_addr;

    i2c_pci_device_detectors.push_back(block);
}

void ResourceManager::RegisterDeviceDetector(std::string name, DeviceDetectorFunction detector)
{
    device_detector_strings.push_back(name);
    device_detectors.push_back(detector);
}

void ResourceManager::RegisterHIDDeviceDetector(std::string name,
                               HIDDeviceDetectorFunction  detector,
                               uint16_t vid,
                               uint16_t pid,
                               int interface,
                               int usage_page,
                               int usage)
{
    HIDDeviceDetectorBlock block;

    block.name          = name;
    block.vid           = vid;
    block.pid           = pid;
    block.function      = detector;
    block.interface     = interface;
    block.usage_page    = usage_page;
    block.usage         = usage;

    hid_device_detectors.push_back(block);
}

void ResourceManager::RegisterHIDWrappedDeviceDetector(std::string name,
                                                       HIDWrappedDeviceDetectorFunction  detector,
                                                       uint16_t vid,
                                                       uint16_t pid,
                                                       int interface,
                                                       int usage_page,
                                                       int usage)
{
    HIDWrappedDeviceDetectorBlock block;

    block.name          = name;
    block.vid           = vid;
    block.pid           = pid;
    block.function      = detector;
    block.interface     = interface;
    block.usage_page    = usage_page;
    block.usage         = usage;

    hid_wrapped_device_detectors.push_back(block);
}

void ResourceManager::RegisterDynamicDetector(std::string name, DynamicDetectorFunction detector)
{
    dynamic_detector_strings.push_back(name);
    dynamic_detectors.push_back(detector);
}

void ResourceManager::RegisterPreDetectionHook(PreDetectionHookFunction hook)
{
    pre_detection_hooks.push_back(hook);
}

void ResourceManager::RegisterClientInfoChangeCallback(ClientInfoChangeCallback new_callback, void * new_callback_arg)
{
    ClientInfoChangeCallbacks.push_back(new_callback);
    ClientInfoChangeCallbackArgs.push_back(new_callback_arg);

    LOG_TRACE("[ResourceManager] Registered client info change callback.  Total callbacks registered: %d", ClientInfoChangeCallbacks.size());
}

void ResourceManager::UnregisterClientInfoChangeCallback(ClientInfoChangeCallback callback, void * callback_arg)
{
    for(size_t idx = 0; idx < ClientInfoChangeCallbacks.size(); idx++)
    {
        if(ClientInfoChangeCallbacks[idx] == callback && ClientInfoChangeCallbackArgs[idx] == callback_arg)
        {
            ClientInfoChangeCallbacks.erase(ClientInfoChangeCallbacks.begin() + idx);
            ClientInfoChangeCallbackArgs.erase(ClientInfoChangeCallbackArgs.begin() + idx);
        }
    }

    LOG_TRACE("[ResourceManager] Unregistered client info change callback.  Total callbacks registered: %d", ClientInfoChangeCallbacks.size());
}

void ResourceManager::RegisterDeviceListChangeCallback(DeviceListChangeCallback new_callback, void * new_callback_arg)
{
    DeviceListChangeCallbacks.push_back(new_callback);
    DeviceListChangeCallbackArgs.push_back(new_callback_arg);

    LOG_TRACE("[ResourceManager] Registered device list change callback.  Total callbacks registered: %d", DeviceListChangeCallbacks.size());
}

void ResourceManager::UnregisterDeviceListChangeCallback(DeviceListChangeCallback callback, void * callback_arg)
{
    for(size_t idx = 0; idx < DeviceListChangeCallbacks.size(); idx++)
    {
        if(DeviceListChangeCallbacks[idx] == callback && DeviceListChangeCallbackArgs[idx] == callback_arg)
        {
            DeviceListChangeCallbacks.erase(DeviceListChangeCallbacks.begin() + idx);
            DeviceListChangeCallbackArgs.erase(DeviceListChangeCallbackArgs.begin() + idx);
        }
    }

    LOG_TRACE("[ResourceManager] Unregistered device list change callback.  Total callbacks registered: %d", DeviceListChangeCallbacks.size());
}

void ResourceManager::RegisterI2CBusListChangeCallback(I2CBusListChangeCallback new_callback, void * new_callback_arg)
{
    I2CBusListChangeCallbacks.push_back(new_callback);
    I2CBusListChangeCallbackArgs.push_back(new_callback_arg);
}

void ResourceManager::UnregisterI2CBusListChangeCallback(I2CBusListChangeCallback callback, void * callback_arg)
{
    for(size_t idx = 0; idx < I2CBusListChangeCallbacks.size(); idx++)
    {
        if(I2CBusListChangeCallbacks[idx] == callback && I2CBusListChangeCallbackArgs[idx] == callback_arg)
        {
            I2CBusListChangeCallbacks.erase(I2CBusListChangeCallbacks.begin() + idx);
            I2CBusListChangeCallbackArgs.erase(I2CBusListChangeCallbackArgs.begin() + idx);
        }
    }
}

void ResourceManager::RegisterDetectionProgressCallback(DetectionProgressCallback new_callback, void *new_callback_arg)
{
    DetectionProgressCallbacks.push_back(new_callback);
    DetectionProgressCallbackArgs.push_back(new_callback_arg);

    LOG_TRACE("[ResourceManager] Registered detection progress callback.  Total callbacks registered: %d", DetectionProgressCallbacks.size());
}

void ResourceManager::UnregisterDetectionProgressCallback(DetectionProgressCallback callback, void *callback_arg)
{
    for(size_t idx = 0; idx < DetectionProgressCallbacks.size(); idx++)
    {
        if(DetectionProgressCallbacks[idx] == callback && DetectionProgressCallbackArgs[idx] == callback_arg)
        {
            DetectionProgressCallbacks.erase(DetectionProgressCallbacks.begin() + idx);
            DetectionProgressCallbackArgs.erase(DetectionProgressCallbackArgs.begin() + idx);
        }
    }

    LOG_TRACE("[ResourceManager] Unregistered detection progress callback.  Total callbacks registered: %d", DetectionProgressCallbacks.size());
}

void ResourceManager::RegisterDetectionStartCallback(DetectionStartCallback new_callback, void *new_callback_arg)
{
    DetectionStartCallbacks.push_back(new_callback);
    DetectionStartCallbackArgs.push_back(new_callback_arg);
}

void ResourceManager::UnregisterDetectionStartCallback(DetectionStartCallback callback, void *callback_arg)
{
    for(size_t idx = 0; idx < DetectionStartCallbacks.size(); idx++)
    {
        if(DetectionStartCallbacks[idx] == callback && DetectionStartCallbackArgs[idx] == callback_arg)
        {
            DetectionStartCallbacks.erase(DetectionStartCallbacks.begin() + idx);
            DetectionStartCallbackArgs.erase(DetectionStartCallbackArgs.begin() + idx);
        }
    }
}

void ResourceManager::RegisterDetectionEndCallback(DetectionEndCallback new_callback, void *new_callback_arg)
{
    DetectionEndCallbacks.push_back(new_callback);
    DetectionEndCallbackArgs.push_back(new_callback_arg);
}

void ResourceManager::UnregisterDetectionEndCallback(DetectionEndCallback callback, void *callback_arg)
{
    for(size_t idx = 0; idx < DetectionEndCallbacks.size(); idx++)
    {
        if(DetectionEndCallbacks[idx] == callback && DetectionEndCallbackArgs[idx] == callback_arg)
        {
            DetectionEndCallbacks.erase(DetectionEndCallbacks.begin() + idx);
            DetectionEndCallbackArgs.erase(DetectionEndCallbackArgs.begin() + idx);
        }
    }
}

void ResourceManager::UpdateDeviceList()
{
    DeviceListChangeMutex.lock();

    /*-----------------------------------------------------*\
    | Insert hardware controllers into controller list      |
    \*-----------------------------------------------------*/
    for(unsigned int hw_controller_idx = 0; hw_controller_idx < rgb_controllers_hw.size(); hw_controller_idx++)
    {
        /*-------------------------------------------------*\
        | Check if the controller is already in the list    |
        | at the correct index                              |
        \*-------------------------------------------------*/
        if(hw_controller_idx < rgb_controllers.size())
        {
            if(rgb_controllers[hw_controller_idx] == rgb_controllers_hw[hw_controller_idx])
            {
                continue;
            }
        }

        /*-------------------------------------------------*\
        | If not, check if the controller is already in the |
        | list at a different index                         |
        \*-------------------------------------------------*/
        for(unsigned int controller_idx = 0; controller_idx < rgb_controllers.size(); controller_idx++)
        {
            if(rgb_controllers[controller_idx] == rgb_controllers_hw[hw_controller_idx])
            {
                rgb_controllers.erase(rgb_controllers.begin() + controller_idx);
                rgb_controllers.insert(rgb_controllers.begin() + hw_controller_idx, rgb_controllers_hw[hw_controller_idx]);
                break;
            }
        }

        /*-------------------------------------------------*\
        | If it still hasn't been found, add it to the list |
        \*-------------------------------------------------*/
        rgb_controllers.insert(rgb_controllers.begin() + hw_controller_idx, rgb_controllers_hw[hw_controller_idx]);
    }

    /*-----------------------------------------------------*\
    | Device list has changed, call the callbacks           |
    \*-----------------------------------------------------*/
    DeviceListChanged();

    /*-----------------------------------------------------*\
    | Device list has changed, inform all clients connected |
    | to this server                                        |
    \*-----------------------------------------------------*/
    server->DeviceListChanged();

    DeviceListChangeMutex.unlock();
}

void ResourceManager::ClientInfoChanged()
{
    /*-----------------------------------------------------*\
    | Client info has changed, call the callbacks           |
    \*-----------------------------------------------------*/
    LOG_TRACE("[ResourceManager] Calling client info change callbacks.");

    for(std::size_t callback_idx = 0; callback_idx < ClientInfoChangeCallbacks.size(); callback_idx++)
    {
        ResourceManager::ClientInfoChangeCallbacks[callback_idx](ClientInfoChangeCallbackArgs[callback_idx]);
    }
}

void ResourceManager::DeviceListChanged()
{
    /*-----------------------------------------------------*\
    | Device list has changed, call the callbacks           |
    \*-----------------------------------------------------*/
    LOG_TRACE("[ResourceManager] Calling device list change callbacks.");

    for(std::size_t callback_idx = 0; callback_idx < DeviceListChangeCallbacks.size(); callback_idx++)
    {
        ResourceManager::DeviceListChangeCallbacks[callback_idx](DeviceListChangeCallbackArgs[callback_idx]);
    }
}

void ResourceManager::DetectionProgressChanged()
{
    DetectionProgressMutex.lock();

    /*-----------------------------------------------------*\
    | Detection progress has changed, call the callbacks    |
    \*-----------------------------------------------------*/
    LOG_TRACE("[ResourceManager] Calling detection progress callbacks.");

    for(std::size_t callback_idx = 0; callback_idx < (unsigned int)DetectionProgressCallbacks.size(); callback_idx++)
    {
        DetectionProgressCallbacks[callback_idx](DetectionProgressCallbackArgs[callback_idx]);
    }

    DetectionProgressMutex.unlock();
}

void ResourceManager::I2CBusListChanged()
{
    I2CBusListChangeMutex.lock();

    /*-----------------------------------------------------*\
    | Detection progress has changed, call the callbacks    |
    \*-----------------------------------------------------*/
    for(std::size_t callback_idx = 0; callback_idx < (unsigned int)I2CBusListChangeCallbacks.size(); callback_idx++)
    {
        I2CBusListChangeCallbacks[callback_idx](I2CBusListChangeCallbackArgs[callback_idx]);
    }

    I2CBusListChangeMutex.unlock();
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

filesystem::path ResourceManager::GetConfigurationDirectory()
{
    return(config_dir);
}

void ResourceManager::SetConfigurationDirectory(const filesystem::path &directory)
{
    config_dir = directory;
    settings_manager->LoadSettings(directory / "OpenRGB.json");
    profile_manager->SetConfigurationDirectory(directory);

    rgb_controllers_sizes.clear();
    rgb_controllers_sizes   = profile_manager->LoadProfileToList("sizes", true);
}

NetworkServer* ResourceManager::GetServer()
{
    return(server);
}

static void NetworkClientInfoChangeCallback(void* this_ptr)
{
    ResourceManager* this_obj = (ResourceManager*)this_ptr;

    this_obj->ClientInfoChanged();
    this_obj->DeviceListChanged();
}

void ResourceManager::RegisterNetworkClient(NetworkClient* new_client)
{
    new_client->RegisterClientInfoChangeCallback(NetworkClientInfoChangeCallback, this);

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


/******************************************************************************************\
*                                                                                          *
*   AttemptLocalConnection                                                                 *
*                                                                                          *
*       Attempts an SDK connection to the local server.  Returns true if success           *
*                                                                                          *
\******************************************************************************************/

bool ResourceManager::AttemptLocalConnection()
{
    detection_percent = 0;
    detection_string  = "Attempting local server connection...";
    DetectionProgressChanged();

    LOG_DEBUG("[ResourceManager] Attempting local server connection...");

    bool success = false;

    auto_connection_client = new NetworkClient(ResourceManager::get()->GetRGBControllers());

    std::string titleString = "OpenRGB ";
    titleString.append(VERSION_STRING);

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
        LOG_TRACE("[ResourceManager] Client failed to connect");
        auto_connection_client->StopClient();
        LOG_TRACE("[ResourceManager] Client stopped");

        delete auto_connection_client;

        auto_connection_client = NULL;
    }
    else
    {
        ResourceManager::get()->RegisterNetworkClient(auto_connection_client);
        LOG_TRACE("[ResourceManager] Registered network client");

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

std::vector<NetworkClient*>& ResourceManager::GetClients()
{
    return(clients);
}

ProfileManager* ResourceManager::GetProfileManager()
{
    return(profile_manager);
}

SettingsManager* ResourceManager::GetSettingsManager()
{
    return(settings_manager);
}

bool ResourceManager::GetDetectionEnabled()
{
    return(detection_enabled);
}

unsigned int ResourceManager::GetDetectionPercent()
{
    return (detection_percent.load());
}

const char *ResourceManager::GetDetectionString()
{
    return (detection_string);
}

void ResourceManager::Cleanup()
{
    ResourceManager::get()->WaitForDeviceDetection();

    std::vector<RGBController *> rgb_controllers_hw_copy = rgb_controllers_hw;

    for(std::size_t hw_controller_idx = 0; hw_controller_idx < rgb_controllers_hw.size(); hw_controller_idx++)
    {
        for(std::size_t controller_idx = 0; controller_idx < rgb_controllers.size(); controller_idx++)
        {
            if(rgb_controllers[controller_idx] == rgb_controllers_hw[hw_controller_idx])
            {
                rgb_controllers.erase(rgb_controllers.begin() + controller_idx);
                break;
            }
        }
    }

    /*-----------------------------------------------------*\
    | Clear the hardware controllers list and set the       |
    | previous hardware controllers list size to zero       |
    \*-----------------------------------------------------*/
    rgb_controllers_hw.clear();
    detection_prev_size = 0;

    for(RGBController* rgb_controller : rgb_controllers_hw_copy)
    {
        delete rgb_controller;
    }

    std::vector<i2c_smbus_interface *> busses_copy = busses;

    busses.clear();

    for(i2c_smbus_interface* bus : busses_copy)
    {
        delete bus;
    }

    RunInBackgroundThread(std::bind(&ResourceManager::HidExitCoroutine, this));
}

void ResourceManager::ProcessPreDetectionHooks()
{
    for(std::size_t hook_idx = 0; hook_idx < pre_detection_hooks.size(); hook_idx++)
    {
        pre_detection_hooks[hook_idx]();
    }
}

void ResourceManager::ProcessDynamicDetectors()
{
    for(std::size_t detector_idx = 0; detector_idx < dynamic_detectors.size(); detector_idx++)
    {
        dynamic_detectors[detector_idx]();
    }

    dynamic_detectors_processed = true;
}

/*---------------------------------------------------------*\
| Handle ALL pre-detection routines                         |
| The system should be ready to start a detection thread    |
| (returns false if detection can not proceed)              |
\*---------------------------------------------------------*/
bool ResourceManager::ProcessPreDetection()
{
    /*-----------------------------------------------------*\
    | Process pre-detection hooks                           |
    \*-----------------------------------------------------*/
    ProcessPreDetectionHooks();

    /*-----------------------------------------------------*\
    | Process Dynamic Detectors                             |
    \*-----------------------------------------------------*/
    if(!dynamic_detectors_processed)
    {
        ProcessDynamicDetectors();
    }

    /*-----------------------------------------------------*\
    | Call detection start callbacks                        |
    \*-----------------------------------------------------*/
    LOG_TRACE("[ResourceManager] Calling detection start callbacks.");

    for(std::size_t callback_idx = 0; callback_idx < DetectionStartCallbacks.size(); callback_idx++)
    {
        DetectionStartCallbacks[callback_idx](DetectionStartCallbackArgs[callback_idx]);
    }

    /*-----------------------------------------------------*\
    | Update the detector settings                          |
    \*-----------------------------------------------------*/
    UpdateDetectorSettings();
    if(detection_enabled)
    {
        /*-------------------------------------------------*\
        | Do nothing is it is already detecting devices     |
        \*-------------------------------------------------*/
        if(detection_is_required.load())
        {
            return false;
        }

        /*-------------------------------------------------*\
        | If there's anything left from the last time,      |
        | we shall remove it first                          |
        \*-------------------------------------------------*/
        detection_percent = 0;
        detection_string  = "";

        DetectionProgressChanged();

        Cleanup();

        UpdateDeviceList();

        /*-------------------------------------------------*\
        | Initialize HID interface for detection            |
        \*-------------------------------------------------*/
        int hid_status = hid_init();

        LOG_INFO("[ResourceManager] Initializing HID interfaces: %s", ((hid_status == 0) ? "Success" : "Failed"));

        /*-------------------------------------------------*\
        | Mark the detection as ongoing                     |
        | So the detection thread may proceed               |
        \*-------------------------------------------------*/
        detection_is_required = true;

        return true;
    }
    return false;
}

void ResourceManager::DetectDevices()
{
    if(ProcessPreDetection())
    {
        // Run the detection coroutine
        RunInBackgroundThread(std::bind(&ResourceManager::DetectDevicesCoroutine, this));
    }

    if(!detection_enabled)
    {
        ProcessPostDetection();
    }
}

void ResourceManager::RescanDevices()
{
    /*-----------------------------------------------------*\
    | If automatic local connection is active, the primary  |
    | instance is the local server, so send rescan requests |
    | to the automatic local connection client              |
    \*-----------------------------------------------------*/
    if(auto_connection_active && auto_connection_client != NULL)
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
    | Perform local rescan                                  |
    \*-----------------------------------------------------*/
    DetectDevices();
}

void ResourceManager::ProcessPostDetection()
{
    /*-----------------------------------------------------*\
    | Signal that detection is complete                     |
    \*-----------------------------------------------------*/
    detection_percent     = 100;
    DetectionProgressChanged();

    LOG_INFO("[ResourceManager] Calling Post-detection callbacks");
    /*-----------------------------------------------------*\
    | Call detection end callbacks                          |
    \*-----------------------------------------------------*/
    for(std::size_t callback_idx = 0; callback_idx < DetectionEndCallbacks.size(); callback_idx++)
    {
        DetectionEndCallbacks[callback_idx](DetectionEndCallbackArgs[callback_idx]);
    }

    detection_is_required = false;

    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|                Detection completed                 |");
    LOG_INFO("------------------------------------------------------");
}

void ResourceManager::DisableDetection()
{
    detection_enabled = false;
}

void ResourceManager::DetectDevicesCoroutine()
{
    DetectDeviceMutex.lock();

    hid_device_info*    current_hid_device;
    float               percent             = 0.0f;
    float               percent_denominator = 0.0f;
    json                detector_settings;
    unsigned int        hid_device_count    = 0;
    hid_device_info*    hid_devices         = NULL;
    bool                hid_safe_mode       = false;

    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|               Start device detection               |");
    LOG_INFO("------------------------------------------------------");

    /*-----------------------------------------------------*\
    | Reset the size entry used flags vector                |
    \*-----------------------------------------------------*/
    detection_size_entry_used.resize(rgb_controllers_sizes.size());

    for(std::size_t size_idx = 0; size_idx < (unsigned int)detection_size_entry_used.size(); size_idx++)
    {
        detection_size_entry_used[size_idx] = false;
    }

    /*-----------------------------------------------------*\
    | Open device disable list and read in disabled         |
    | device strings                                        |
    \*-----------------------------------------------------*/
    detector_settings = settings_manager->GetSettings("Detectors");

    /*-----------------------------------------------------*\
    | Check HID safe mode setting                           |
    \*-----------------------------------------------------*/
    if(detector_settings.contains("hid_safe_mode"))
    {
        hid_safe_mode = detector_settings["hid_safe_mode"];
    }

    /*-----------------------------------------------------*\
    | Calculate the percentage denominator by adding the    |
    | number of I2C and miscellaneous detectors and the     |
    | number of enumerated HID devices                      |
    |                                                       |
    | Start by iterating through all HID devices in list to |
    | get a total count                                     |
    \*-----------------------------------------------------*/
    if(!hid_safe_mode)
    {
        hid_devices = hid_enumerate(0, 0);
    }

    current_hid_device = hid_devices;

    while(current_hid_device)
    {
        hid_device_count++;

        current_hid_device = current_hid_device->next;
    }

    percent_denominator = (float)(i2c_device_detectors.size() + i2c_dimm_device_detectors.size() + i2c_pci_device_detectors.size() + device_detectors.size()) + (float)hid_device_count;

    /*-----------------------------------------------------*\
    | Start at 0% detection progress                        |
    \*-----------------------------------------------------*/
    detection_percent = 0;

#ifdef __linux__
    /*-----------------------------------------------------*\
    | Check if the udev rules exist                         |
    \*-----------------------------------------------------*/
    bool udev_not_exist     = false;
    bool udev_multiple      = false;

    if(access("/etc/udev/rules.d/60-openrgb.rules", F_OK) != 0)
    {
        if(access("/usr/lib/udev/rules.d/60-openrgb.rules", F_OK) != 0)
        {
            udev_not_exist  = true;
        }
    }
    else
    {
        if(access("/usr/lib/udev/rules.d/60-openrgb.rules", F_OK) == 0)
        {
            udev_multiple   = true;
        }
    }
#endif

    /*-----------------------------------------------------*\
    | Detect i2c interfaces                                 |
    \*-----------------------------------------------------*/
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|             Detecting I2C interfaces               |");
    LOG_INFO("------------------------------------------------------");

    bool i2c_interface_fail = false;

    for(unsigned int i2c_bus_detector_idx = 0; i2c_bus_detector_idx < (unsigned int)i2c_bus_detectors.size() && detection_is_required.load(); i2c_bus_detector_idx++)
    {
        if(i2c_bus_detectors[i2c_bus_detector_idx]() == false)
        {
            i2c_interface_fail = true;
        }

        I2CBusListChanged();
    }

    /*-----------------------------------------------------*\
    | Detect i2c devices                                    |
    \*-----------------------------------------------------*/
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|               Detecting I2C devices                |");
    LOG_INFO("------------------------------------------------------");
    for(unsigned int i2c_detector_idx = 0; i2c_detector_idx < (unsigned int)i2c_device_detectors.size() && detection_is_required.load(); i2c_detector_idx++)
    {
        std::size_t controller_size = rgb_controllers_hw.size();
        detection_string = i2c_device_detector_strings[i2c_detector_idx].c_str();

        /*-------------------------------------------------*\
        | Check if this detector is enabled                 |
        \*-------------------------------------------------*/
        bool this_device_enabled = true;
        if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
        {
            this_device_enabled = detector_settings["detectors"][detection_string];
        }

        LOG_DEBUG("[%s] is %s", detection_string, ((this_device_enabled == true) ? "enabled" : "disabled"));
        if(this_device_enabled)
        {
            DetectionProgressChanged();

            i2c_device_detectors[i2c_detector_idx](busses);
        }

        /*-------------------------------------------------*\
        | If the device list size has changed, call the     |
        | device list changed callbacks                     |
        \*-------------------------------------------------*/
        if(rgb_controllers_hw.size() == controller_size)
        {
            LOG_DEBUG("[%s] no devices found", detection_string);
        }

        LOG_TRACE("[%s] detection end", detection_string);

        /*-------------------------------------------------*\
        | Update detection percent                          |
        \*-------------------------------------------------*/
        percent = ((float)i2c_detector_idx + 1.0f) / percent_denominator;

        detection_percent = (unsigned int)(percent * 100.0f);
    }

    /*-----------------------------------------------------*\
    | Detect i2c DIMM modules                               |
    \*-----------------------------------------------------*/
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|            Detecting I2C DIMM modules              |");
    LOG_INFO("------------------------------------------------------");

    detection_string = "Reading DRAM SPD Information";
    DetectionProgressChanged();

    for(unsigned int bus = 0; bus < busses.size() && IsAnyDimmDetectorEnabled(detector_settings); bus++)
    {
        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            std::vector<SPDWrapper> slots;
            SPDMemoryType dimm_type = SPD_RESERVED;

            for(uint8_t spd_addr = 0x50; spd_addr < 0x58; spd_addr++)
            {
                SPDDetector spd(busses[bus], spd_addr, dimm_type);
                if(spd.is_valid())
                {
                    SPDWrapper accessor(spd);
                    dimm_type = spd.memory_type();
                    LOG_INFO("[ResourceManager] Detected occupied slot %d, bus %d, type %s", spd_addr - 0x50 + 1, bus, spd_memory_type_name[dimm_type]);
                    LOG_DEBUG("[ResourceManager] Jedec ID: 0x%04x", accessor.jedec_id());
                    slots.push_back(accessor);
                }
            }

            for(unsigned int i2c_detector_idx = 0; i2c_detector_idx < i2c_dimm_device_detectors.size() && detection_is_required.load(); i2c_detector_idx++)
            {
                if((i2c_dimm_device_detectors[i2c_detector_idx].dimm_type == dimm_type) && is_jedec_in_slots(slots, i2c_dimm_device_detectors[i2c_detector_idx].jedec_id))
                {
                    detection_string = i2c_dimm_device_detectors[i2c_detector_idx].name.c_str();

                    /*-------------------------------------*\
                    | Check if this detector is enabled     |
                    \*-------------------------------------*/
                    bool this_device_enabled = true;
                    if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
                    {
                        this_device_enabled = detector_settings["detectors"][detection_string];
                    }

                    LOG_DEBUG("[%s] is %s", detection_string, ((this_device_enabled == true) ? "enabled" : "disabled"));
                    if(this_device_enabled)
                    {
                        DetectionProgressChanged();

                        std::vector<SPDWrapper*> matching_slots = slots_with_jedec(slots, i2c_dimm_device_detectors[i2c_detector_idx].jedec_id);
                        i2c_dimm_device_detectors[i2c_detector_idx].function(busses[bus], matching_slots, i2c_dimm_device_detectors[i2c_detector_idx].name);
                    }

                    LOG_TRACE("[%s] detection end", detection_string);
                }

                /*-----------------------------------------*\
                | Update detection percent                  |
                \*-----------------------------------------*/
                percent = (i2c_device_detectors.size() + i2c_detector_idx + 1.0f) / percent_denominator;

                detection_percent = (unsigned int)(percent * 100.0f);
            }
        }
    }

    /*-----------------------------------------------------*\
    | Detect i2c PCI devices                                |
    \*-----------------------------------------------------*/
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|               Detecting I2C PCI devices            |");
    LOG_INFO("------------------------------------------------------");
    for(unsigned int i2c_detector_idx = 0; i2c_detector_idx < (unsigned int)i2c_pci_device_detectors.size() && detection_is_required.load(); i2c_detector_idx++)
    {
        detection_string = i2c_pci_device_detectors[i2c_detector_idx].name.c_str();

        /*-------------------------------------------------*\
        | Check if this detector is enabled                 |
        \*-------------------------------------------------*/
        bool this_device_enabled = true;
        if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
        {
            this_device_enabled = detector_settings["detectors"][detection_string];
        }

        LOG_DEBUG("[%s] is %s", detection_string, ((this_device_enabled == true) ? "enabled" : "disabled"));
        if(this_device_enabled)
        {
            DetectionProgressChanged();

            for(unsigned int bus = 0; bus < busses.size(); bus++)
            {
                if(busses[bus]->pci_vendor           == i2c_pci_device_detectors[i2c_detector_idx].ven_id    &&
                   busses[bus]->pci_device           == i2c_pci_device_detectors[i2c_detector_idx].dev_id    &&
                   busses[bus]->pci_subsystem_vendor == i2c_pci_device_detectors[i2c_detector_idx].subven_id &&
                   busses[bus]->pci_subsystem_device == i2c_pci_device_detectors[i2c_detector_idx].subdev_id)
                {
                    i2c_pci_device_detectors[i2c_detector_idx].function(busses[bus], i2c_pci_device_detectors[i2c_detector_idx].i2c_addr, i2c_pci_device_detectors[i2c_detector_idx].name);
                }
            }
        }

        LOG_TRACE("[%s] detection end", detection_string);

        /*-------------------------------------------------*\
        | Update detection percent                          |
        \*-------------------------------------------------*/
        percent = (i2c_device_detectors.size() + i2c_dimm_device_detectors.size() + i2c_detector_idx + 1.0f) / percent_denominator;

        detection_percent = (unsigned int)(percent * 100.0f);
    }

    /*-----------------------------------------------------*\
    | Detect HID devices                                    |
    |                                                       |
    | Reset current device pointer to first device          |
    \*-----------------------------------------------------*/
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|               Detecting HID devices                |");
    if (hid_safe_mode)
    LOG_INFO("|                  with safe mode                    |");
    LOG_INFO("------------------------------------------------------");
    current_hid_device = hid_devices;

    if(hid_safe_mode)
    {
        /*-------------------------------------------------*\
        | Loop through all available detectors.  If all     |
        | required information matches, run the detector    |
        \*-------------------------------------------------*/
        for(unsigned int hid_detector_idx = 0; hid_detector_idx < (unsigned int)hid_device_detectors.size() && detection_is_required.load(); hid_detector_idx++)
        {
            HIDDeviceDetectorBlock & detector = hid_device_detectors[hid_detector_idx];
            hid_devices = hid_enumerate(detector.vid, detector.pid);

            LOG_VERBOSE("[ResourceManager] Trying to run detector for [%s] (for %04x:%04x)", detector.name.c_str(), detector.vid, detector.pid);

            current_hid_device = hid_devices;

            while(current_hid_device)
            {

                if(detector.compare(current_hid_device))
                {
                    detection_string = detector.name.c_str();

                    /*-------------------------------------*\
                    | Check if this detector is enabled or  |
                    | needs to be added to the settings list|
                    \*-------------------------------------*/
                    bool this_device_enabled = true;
                    if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
                    {
                        this_device_enabled = detector_settings["detectors"][detection_string];
                    }

                    LOG_DEBUG("[%s] is %s", detection_string, ((this_device_enabled == true) ? "enabled" : "disabled"));

                    if(this_device_enabled)
                    {
                        DetectionProgressChanged();

                        detector.function(current_hid_device, hid_device_detectors[hid_detector_idx].name);

                        LOG_TRACE("[%s] detection end", detection_string);
                    }
                }

                current_hid_device = current_hid_device->next;
            }

            hid_free_enumeration(hid_devices);
        }
    }
    else
    {
        /*-------------------------------------------------*\
        | Iterate through all devices in list and run       |
        | detectors                                         |
        \*-------------------------------------------------*/
        hid_device_count = 0;

        while(current_hid_device)
        {
            if(LogManager::get()->getLoglevel() >= LL_DEBUG)
            {
                const char* manu_name = StringUtils::wchar_to_char(current_hid_device->manufacturer_string);
                const char* prod_name = StringUtils::wchar_to_char(current_hid_device->product_string);
                LOG_DEBUG("[%04X:%04X U=%04X P=0x%04X I=%d] %-25s - %s", current_hid_device->vendor_id, current_hid_device->product_id, current_hid_device->usage, current_hid_device->usage_page, current_hid_device->interface_number, manu_name, prod_name);
            }
            detection_string = "";
            DetectionProgressChanged();

            /*---------------------------------------------*\
            | Loop through all available detectors.  If all |
            | required information matches, run the detector|
            \*---------------------------------------------*/
            for(unsigned int hid_detector_idx = 0; hid_detector_idx < (unsigned int)hid_device_detectors.size() && detection_is_required.load(); hid_detector_idx++)
            {
                HIDDeviceDetectorBlock & detector = hid_device_detectors[hid_detector_idx];
                if(detector.compare(current_hid_device))
                {
                    detection_string = detector.name.c_str();

                    /*-------------------------------------*\
                    | Check if this detector is enabled or  |
                    | needs to be added to the settings list|
                    \*-------------------------------------*/
                    bool this_device_enabled = true;
                    if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
                    {
                        this_device_enabled = detector_settings["detectors"][detection_string];
                    }

                    LOG_DEBUG("[%s] is %s", detection_string, ((this_device_enabled == true) ? "enabled" : "disabled"));

                    if(this_device_enabled)
                    {
                        DetectionProgressChanged();

                        detector.function(current_hid_device, hid_device_detectors[hid_detector_idx].name);
                    }
                }
            }

            /*---------------------------------------------*\
            | Loop through all available wrapped HID        |
            | detectors.  If all required information       |
            | matches, run the detector                     |
            \*---------------------------------------------*/
            for(unsigned int hid_detector_idx = 0; hid_detector_idx < (unsigned int)hid_wrapped_device_detectors.size() && detection_is_required.load(); hid_detector_idx++)
            {
                HIDWrappedDeviceDetectorBlock & detector = hid_wrapped_device_detectors[hid_detector_idx];
                if(detector.compare(current_hid_device))
                {
                    detection_string = detector.name.c_str();

                    /*-------------------------------------*\
                    | Check if this detector is enabled or  |
                    | needs to be added to the settings list|
                    \*-------------------------------------*/
                    bool this_device_enabled = true;
                    if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
                    {
                        this_device_enabled = detector_settings["detectors"][detection_string];
                    }

                    LOG_DEBUG("[%s] is %s", detection_string, ((this_device_enabled == true) ? "enabled" : "disabled"));

                    if(this_device_enabled)
                    {
                        DetectionProgressChanged();

                        detector.function(default_wrapper, current_hid_device, hid_wrapped_device_detectors[hid_detector_idx].name);
                    }
                }
            }

            /*---------------------------------------------*\
            | Update detection percent                      |
            \*---------------------------------------------*/
            hid_device_count++;

            percent = (i2c_device_detectors.size() + i2c_dimm_device_detectors.size() + i2c_pci_device_detectors.size() + hid_device_count) / percent_denominator;

            detection_percent = (unsigned int)(percent * 100.0f);

            /*---------------------------------------------*\
            | Move on to the next HID device                |
            \*---------------------------------------------*/
            current_hid_device = current_hid_device->next;
        }

        /*-------------------------------------------------*\
        | Done using the device list, free it               |
        \*-------------------------------------------------*/
        hid_free_enumeration(hid_devices);
    }

    /*-----------------------------------------------------*\
    | Detect HID devices                                    |
    |                                                       |
    | Reset current device pointer to first device          |
    \*-----------------------------------------------------*/
#ifdef __linux__
#ifdef __GLIBC__
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|            Detecting libusb HID devices            |");
    LOG_INFO("------------------------------------------------------");

    void *         dyn_handle = NULL;
    hidapi_wrapper wrapper;

    /*-----------------------------------------------------*\
    | Load the libhidapi-libusb library                     |
    \*-----------------------------------------------------*/
#ifdef __GLIBC__
    if((dyn_handle = dlopen("libhidapi-libusb.so", RTLD_NOW | RTLD_NODELETE | RTLD_DEEPBIND)))
#else
    if(dyn_handle = dlopen("libhidapi-libusb.so", RTLD_NOW | RTLD_NODELETE ))
#endif
    {
        /*-------------------------------------------------*\
        | Create a wrapper with the libusb functions        |
        \*-------------------------------------------------*/
        wrapper =
        {
            .dyn_handle                     = dyn_handle,
            .hid_send_feature_report        = (hidapi_wrapper_send_feature_report)          dlsym(dyn_handle,"hid_send_feature_report"),
            .hid_get_feature_report         = (hidapi_wrapper_get_feature_report)           dlsym(dyn_handle,"hid_get_feature_report"),
            .hid_get_serial_number_string   = (hidapi_wrapper_get_serial_number_string)     dlsym(dyn_handle,"hid_get_serial_number_string"),
            .hid_open_path                  = (hidapi_wrapper_open_path)                    dlsym(dyn_handle,"hid_open_path"),
            .hid_enumerate                  = (hidapi_wrapper_enumerate)                    dlsym(dyn_handle,"hid_enumerate"),
            .hid_free_enumeration           = (hidapi_wrapper_free_enumeration)             dlsym(dyn_handle,"hid_free_enumeration"),
            .hid_close                      = (hidapi_wrapper_close)                        dlsym(dyn_handle,"hid_close"),
            .hid_error                      = (hidapi_wrapper_error)                        dlsym(dyn_handle,"hid_free_enumeration")
        };

        hid_devices = wrapper.hid_enumerate(0, 0);

        current_hid_device = hid_devices;

        /*-------------------------------------------------*\
        | Iterate through all devices in list and run       |
        | detectors                                         |
        \*-------------------------------------------------*/
        hid_device_count = 0;

        while(current_hid_device)
        {
            if(LogManager::get()->getLoglevel() >= LL_DEBUG)
            {
                const char* manu_name = StringUtils::wchar_to_char(current_hid_device->manufacturer_string);
                const char* prod_name = StringUtils::wchar_to_char(current_hid_device->product_string);
                LOG_DEBUG("[%04X:%04X U=%04X P=0x%04X I=%d] %-25s - %s", current_hid_device->vendor_id, current_hid_device->product_id, current_hid_device->usage, current_hid_device->usage_page, current_hid_device->interface_number, manu_name, prod_name);
            }
            detection_string = "";
            DetectionProgressChanged();

            /*---------------------------------------------*\
            | Loop through all available wrapped HID        |
            | detectors.  If all required information       |
            | matches, run the detector                     |
            \*---------------------------------------------*/
            for(unsigned int hid_detector_idx = 0; hid_detector_idx < (unsigned int)hid_wrapped_device_detectors.size() && detection_is_required.load(); hid_detector_idx++)
            {
                HIDWrappedDeviceDetectorBlock & detector = hid_wrapped_device_detectors[hid_detector_idx];
                if(detector.compare(current_hid_device))
                {
                    detection_string = detector.name.c_str();

                    /*-------------------------------------*\
                    | Check if this detector is enabled or  |
                    | needs to be added to the settings list|
                    \*-------------------------------------*/
                    bool this_device_enabled = true;
                    if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
                    {
                        this_device_enabled = detector_settings["detectors"][detection_string];
                    }

                    LOG_DEBUG("[%s] is %s", detection_string, ((this_device_enabled == true) ? "enabled" : "disabled"));

                    if(this_device_enabled)
                    {
                        DetectionProgressChanged();

                        detector.function(wrapper, current_hid_device, detector.name);
                    }
                }
            }

            /*---------------------------------------------*\
            | Update detection percent                      |
            \*---------------------------------------------*/
            hid_device_count++;

            percent = (i2c_device_detectors.size() + i2c_dimm_device_detectors.size() + i2c_pci_device_detectors.size() + hid_device_count) / percent_denominator;

            detection_percent = percent * 100.0f;

            /*---------------------------------------------*\
            | Move on to the next HID device                |
            \*---------------------------------------------*/
            current_hid_device = current_hid_device->next;
        }

        /*-------------------------------------------------*\
        | Done using the device list, free it               |
        \*-------------------------------------------------*/
        wrapper.hid_free_enumeration(hid_devices);
    }
#endif
#endif

    /*-----------------------------------------------------*\
    | Detect other devices                                  |
    \*-----------------------------------------------------*/
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|              Detecting other devices               |");
    LOG_INFO("------------------------------------------------------");

    for(unsigned int detector_idx = 0; detector_idx < (unsigned int)device_detectors.size() && detection_is_required.load(); detector_idx++)
    {
        detection_string = device_detector_strings[detector_idx].c_str();

        /*-------------------------------------------------*\
        | Check if this detector is enabled                 |
        \*-------------------------------------------------*/
        bool this_device_enabled = true;
        if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
        {
            this_device_enabled = detector_settings["detectors"][detection_string];
        }

        LOG_DEBUG("[%s] is %s", detection_string, ((this_device_enabled == true) ? "enabled" : "disabled"));

        if(this_device_enabled)
        {
            DetectionProgressChanged();

            device_detectors[detector_idx]();
        }

        LOG_TRACE("[%s] detection end", detection_string);

        /*-------------------------------------------------*\
        | Update detection percent                          |
        \*-------------------------------------------------*/
        percent = (i2c_device_detectors.size() + hid_device_count + detector_idx + 1.0f) / percent_denominator;

        detection_percent = (unsigned int)(percent * 100.0f);
    }

    /*-----------------------------------------------------*\
    | Make sure that when the detection is done, progress   |
    | bar is set to 100%                                    |
    \*-----------------------------------------------------*/
    ProcessPostDetection();

    DetectDeviceMutex.unlock();

#ifdef __linux__
    /*-----------------------------------------------------*\
    | If the udev rules file is not installed, show a dialog|
    \*-----------------------------------------------------*/
    if(udev_not_exist)
    {
        LOG_DIALOG("%s", UDEV_MISSING);

        udev_multiple       = false;
        i2c_interface_fail  = false;
    }

    /*-----------------------------------------------------*\
    | If multiple udev rules files are installed, show a    |
    | dialog                                                |
    \*-----------------------------------------------------*/
    if(udev_multiple)
    {
        LOG_DIALOG("%s", UDEV_MUTLI);

        i2c_interface_fail  = false;
    }

#endif

    /*-----------------------------------------------------*\
    | If any i2c interfaces failed to detect due to an      |
    | error condition, show a dialog                        |
    \*-----------------------------------------------------*/
    if(i2c_interface_fail)
    {
#ifdef _WIN32
        LOG_DIALOG("%s", I2C_ERR_WIN);
#endif
#ifdef __linux__
        LOG_DIALOG("%s", I2C_ERR_LINUX);
#endif
    }
}

void ResourceManager::StopDeviceDetection()
{
    LOG_INFO("[ResourceManager] Detection abort requested");
    detection_is_required = false;
    detection_percent = 100;
    detection_string = "Stopping";
}

void ResourceManager::Initialize(bool tryConnect, bool detectDevices, bool startServer, bool applyPostOptions)
{
    /*-----------------------------------------------------*\
    | Cache the parameters                                  |
    | TODO: Possibly cache them in the CLI file somewhere   |
    \*-----------------------------------------------------*/
    tryAutoConnect     = tryConnect;
    detection_enabled  = detectDevices;
    start_server       = startServer;
    apply_post_options = applyPostOptions;

    RunInBackgroundThread(std::bind(&ResourceManager::InitCoroutine, this));
}

void ResourceManager::InitCoroutine()
{
    /*-----------------------------------------------------*\
    | If enabled, try connecting to local server instead of |
    | detecting devices from this instance of OpenRGB       |
    \*-----------------------------------------------------*/
    if(tryAutoConnect)
    {
        detection_percent = 0;
        detection_string  = "Attempting server connection...";
        DetectionProgressChanged();

        /*-------------------------------------------------*\
        | Attempt connection to local server                |
        \*-------------------------------------------------*/
        if(AttemptLocalConnection())
        {
            LOG_DEBUG("[ResourceManager] Local OpenRGB server connected, running in client mode");

            /*---------------------------------------------*\
            | Set auto connection active flag and disable   |
            | detection if the local server was connected   |
            \*---------------------------------------------*/
            auto_connection_active = true;
            DisableDetection();
        }

        tryAutoConnect = false;
    }

    /*-----------------------------------------------------*\
    | Initialize Saved Client Connections                   |
    \*-----------------------------------------------------*/
    json client_settings    = settings_manager->GetSettings("Client");

    if(client_settings.contains("clients"))
    {
        for(unsigned int client_idx = 0; client_idx < client_settings["clients"].size(); client_idx++)
        {
            NetworkClient * client = new NetworkClient(rgb_controllers);

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
    | Perform actual detection if enabled                   |
    | Done in the same thread (InitThread), as we need to   |
    | wait for completion anyway                            |
    \*-----------------------------------------------------*/
    if(detection_enabled)
    {
        LOG_DEBUG("[ResourceManager] Running standalone");
        if(ProcessPreDetection())
        {
            /*---------------------------------------------*\
            | We are currently in a coroutine, so run       |
            | detection directly with no scheduling         |
            \*---------------------------------------------*/
            DetectDevicesCoroutine();
        }
    }
    else
    {
        ProcessPostDetection();
    }

    /*-----------------------------------------------------*\
    | Start server if requested                             |
    \*-----------------------------------------------------*/
    if(start_server)
    {
        detection_percent = 100;
        detection_string = "Starting server";
        DetectionProgressChanged();

        GetServer()->StartServer();
        if(!GetServer()->GetOnline())
        {
            LOG_DEBUG("[ResourceManager] Server failed to start");
        }
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

void ResourceManager::HidExitCoroutine()
{
    /*-----------------------------------------------------*\
    | Cleanup HID interface                                 |
    | WARNING: may not be ran from any other thread!!!      |
    \*-----------------------------------------------------*/
    int hid_status = hid_exit();

    LOG_DEBUG("[ResourceManager] Closing HID interfaces: %s", ((hid_status == 0) ? "Success" : "Failed"));
}

void ResourceManager::RunInBackgroundThread(std::function<void()> coroutine)
{
    if(std::this_thread::get_id() == DetectDevicesThread->get_id())
    {
        /*-------------------------------------------------*\
        | We are already in the background thread - don't   |
        | schedule the call, run it immediately             |
        \*-------------------------------------------------*/
        coroutine();
    }
    else
    {
        BackgroundThreadStateMutex.lock();
        if(ScheduledBackgroundFunction != nullptr)
        {
            LOG_WARNING("[ResourceManager] Detection coroutine: assigned a new coroutine when one was already scheduled - probably two rescan events sent at once");
        }
        ScheduledBackgroundFunction = coroutine;
        BackgroundThreadStateMutex.unlock();
        BackgroundFunctionStartTrigger.notify_one();
    }
}

void ResourceManager::BackgroundThreadFunction()
{
    /*-----------------------------------------------------*\
    | The background thread that runs scheduled coroutines  |
    | when applicable                                       |
    | Stays asleep if nothing is scheduled                  |
    | NOTE: this thread owns the HIDAPI library internal    |
    | objects on MacOS                                      |
    | hid_init and hid_exit may not be called outside of    |
    | this thread.  Calling hid_exit outside of this thread |
    | WILL cause an immediate CRASH on MacOS.               |
    | BackgroundThreadStateMutex will be UNLOCKED as long   |
    | as the thread is suspended.  It locks automatically   |
    | when any coroutine is running.  However, it seems to  |
    | be necessary to be separate from the                  |
    | DeviceDetectionMutex, even though their states are    |
    | nearly identical.                                     |
    \------------------------------------------------------*/

    std::unique_lock lock(BackgroundThreadStateMutex);
    while(background_thread_running)
    {
        if(ScheduledBackgroundFunction)
        {
            std::function<void()> coroutine = nullptr;
            std::swap(ScheduledBackgroundFunction, coroutine);
            try
            {
                coroutine();
            }
            catch(std::exception& e)
            {
                LOG_ERROR("[ResourceManager] Unhandled exception in coroutine; e.what(): %s", e.what());
            }
            catch(...)
            {
                LOG_ERROR("[ResourceManager] Unhandled exception in coroutine");
            }
        }
        /*-------------------------------------------------*\
        | This line will cause the thread to suspend until  |
        | the condition variable is triggered               |
        | NOTE: it may be subject to "spurious wakeups"     |
        \*-------------------------------------------------*/
        BackgroundFunctionStartTrigger.wait(lock);
    }
}

void ResourceManager::UpdateDetectorSettings()
{
    json                detector_settings;
    bool                save_settings       = false;

    /*-----------------------------------------------------*\
    | Open device disable list and read in disabled device  |
    | strings                                               |
    \*-----------------------------------------------------*/
    detector_settings = settings_manager->GetSettings("Detectors");

    /*-----------------------------------------------------*\
    | Loop through all I2C detectors and see if any need to |
    | be saved to the settings                              |
    \*-----------------------------------------------------*/
    for(unsigned int i2c_detector_idx = 0; i2c_detector_idx < (unsigned int)i2c_device_detectors.size(); i2c_detector_idx++)
    {
        detection_string = i2c_device_detector_strings[i2c_detector_idx].c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-----------------------------------------------------*\
    | Loop through all I2C DIMM detectors and see if any    |
    | need to be saved to the settings                      |
    \*-----------------------------------------------------*/
    for(unsigned int i2c_detector_idx = 0; i2c_detector_idx < (unsigned int)i2c_dimm_device_detectors.size(); i2c_detector_idx++)
    {
        detection_string = i2c_dimm_device_detectors[i2c_detector_idx].name.c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-----------------------------------------------------*\
    | Loop through all I2C PCI detectors and see if any     |
    | need to be saved to the settings                      |
    \*-----------------------------------------------------*/
    for(unsigned int i2c_pci_detector_idx = 0; i2c_pci_detector_idx < (unsigned int)i2c_pci_device_detectors.size(); i2c_pci_detector_idx++)
    {
        detection_string = i2c_pci_device_detectors[i2c_pci_detector_idx].name.c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-----------------------------------------------------*\
    | Loop through all HID detectors and see if any need to |
    | be saved to the settings                              |
    \*-----------------------------------------------------*/
    for(unsigned int hid_detector_idx = 0; hid_detector_idx < (unsigned int)hid_device_detectors.size(); hid_detector_idx++)
    {
        detection_string = hid_device_detectors[hid_detector_idx].name.c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-----------------------------------------------------*\
    | Loop through all HID wrapped detectors and see if any |
    | need to be saved to the settings                      |
    \*-----------------------------------------------------*/
    for(unsigned int hid_wrapped_detector_idx = 0; hid_wrapped_detector_idx < (unsigned int)hid_wrapped_device_detectors.size(); hid_wrapped_detector_idx++)
    {
        detection_string = hid_wrapped_device_detectors[hid_wrapped_detector_idx].name.c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-----------------------------------------------------*\
    | Loop through remaining detectors and see if any need  |
    | to be saved to the settings                           |
    \*-----------------------------------------------------*/
    for(unsigned int detector_idx = 0; detector_idx < (unsigned int)device_detectors.size(); detector_idx++)
    {
        detection_string = device_detector_strings[detector_idx].c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-----------------------------------------------------*\
    | If there were any setting changes that need to be     |
    | saved, set the settings in the settings manager and   |
    | save them.                                            |
    \*-----------------------------------------------------*/
    if(save_settings)
    {
        LOG_INFO("[ResourceManager] Saving detector settings");

        settings_manager->SetSettings("Detectors", detector_settings);

        settings_manager->SaveSettings();
    }
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

void ResourceManager::WaitForDeviceDetection()
{
    DetectDeviceMutex.lock();
    DetectDeviceMutex.unlock();
}

bool ResourceManager::IsAnyDimmDetectorEnabled(json &detector_settings)
{
    for(unsigned int i2c_detector_idx = 0; i2c_detector_idx < i2c_dimm_device_detectors.size() && detection_is_required.load(); i2c_detector_idx++)
    {
        std::string detector_name_string = i2c_dimm_device_detectors[i2c_detector_idx].name.c_str();
        /*-------------------------------------------------*\
        | Check if this detector is enabled                 |
        \*-------------------------------------------------*/
        if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detector_name_string) &&
           detector_settings["detectors"][detector_name_string] == true)
        {
            return true;
        }
    }
    return false;
}
