/*-----------------------------------------*\
|  ResourceManager.cpp                      |
|                                           |
|  OpenRGB Resource Manager controls access |
|  to application components including      |
|  RGBControllers, I2C interfaces, and      |
|  network SDK components                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/27/2020   |
\*-----------------------------------------*/

#include "ResourceManager.h"
#include "ProfileManager.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <stdlib.h>
#include <string>

std::unique_ptr<ResourceManager> ResourceManager::instance;

using namespace std::chrono_literals;

ResourceManager *ResourceManager::get()
{
    if(!instance)
    {
        instance = std::make_unique<ResourceManager>();
    }

    return instance.get();
}

ResourceManager::ResourceManager()
{
    /*-------------------------------------------------------------------------*\
    | Initialize Detection Variables                                            |
    \*-------------------------------------------------------------------------*/
    detection_percent     = 100;
    detection_string      = "";
    detection_is_required = false;
    DetectDevicesThread   = nullptr;

    /*-------------------------------------------------------------------------*\
    | Initialize Server Instance                                                |
    \*-------------------------------------------------------------------------*/
    server = new NetworkServer(rgb_controllers_hw);

    /*-------------------------------------------------------------------------*\
    | Load sizes list from file                                                 |
    \*-------------------------------------------------------------------------*/
    profile_manager             = new ProfileManager(rgb_controllers_remote, GetConfigurationDirectory());
    rgb_controllers_hw_sizes    = profile_manager->LoadProfileToList("sizes.ors");

    /*-------------------------------------------------------------------------*\
    | Load settings from file                                                   |
    \*-------------------------------------------------------------------------*/
    settings_manager        = new SettingsManager();
    settings_manager->LoadSettings(GetConfigurationDirectory() + "OpenRGB.json");
}

ResourceManager::~ResourceManager()
{
    Cleanup();
}

void ResourceManager::RegisterI2CBus(i2c_smbus_interface *bus)
{
    busses.push_back(bus);
}

std::vector<i2c_smbus_interface*> & ResourceManager::GetI2CBusses()
{
    return busses;
}

void ResourceManager::RegisterRGBController(RGBController *rgb_controller, ControllerList list)
{
    switch(list)
    {
        case CONTROLLER_LIST_HW:
            rgb_controllers_hw.push_back(rgb_controller);
            profile_manager->LoadDeviceFromListWithOptions(rgb_controllers_hw_sizes, rgb_controllers_hw_sizes_used, rgb_controller, true, false);
            DeviceListChanged(rgb_controllers_hw.size() - 1, list, false);
            break;

        case CONTROLLER_LIST_REMOTE:
            rgb_controllers_remote.push_back(rgb_controller);
            DeviceListChanged(rgb_controllers_remote.size() - 1, list, false);
            break;

        default:
            break;
    }
}

bool ResourceManager::RemoveRGBController(RGBController* rgb_controller, ControllerList list)
{
    switch(list)
    {
        case CONTROLLER_LIST_HW:
            for(size_t idx = 0; idx < rgb_controllers_hw.size(); ++idx)
            {
                if(rgb_controllers_hw[idx] == rgb_controller)
                {
                    RGBController * controller_to_delete = rgb_controllers_hw[idx];
                    rgb_controllers_hw.erase(rgb_controllers_hw.begin() + idx);
                    DeviceListChanged(idx, CONTROLLER_LIST_HW, true);
                    delete controller_to_delete;
                    return true;
                }
            }
            return false;

        case CONTROLLER_LIST_REMOTE:
            for(size_t idx = 0; idx < rgb_controllers_remote.size(); ++idx)
            {
                if(rgb_controllers_remote[idx] == rgb_controller)
                {
                    RGBController * controller_to_delete = rgb_controllers_remote[idx];
                    rgb_controllers_remote.erase(rgb_controllers_remote.begin() + idx);
                    DeviceListChanged(idx, CONTROLLER_LIST_REMOTE, true);
                    delete controller_to_delete;
                    return true;
                }
            }
            return false;

        /*-------------------------------------------------------------------------*\
        | If the list ID is invalid, try all of the lists                           |
        \*-------------------------------------------------------------------------*/
        default:
            return RemoveRGBController(rgb_controller, CONTROLLER_LIST_HW) |
                RemoveRGBController(rgb_controller, CONTROLLER_LIST_REMOTE);
    }
}

unsigned int ResourceManager::GetControllerCount(ControllerList list)
{
    switch(list)
    {
        case CONTROLLER_LIST_UI:
            return rgb_controllers_hw.size() + rgb_controllers_remote.size();

        case CONTROLLER_LIST_HW:
            return rgb_controllers_hw.size();

        case CONTROLLER_LIST_REMOTE:
            return rgb_controllers_remote.size();
    }
    return 0;
}

RGBController* ResourceManager::GetController(size_t id, ControllerList list)
{
    switch(list)
    {
        case CONTROLLER_LIST_UI:
            /*-------------------------------------------------------------------------*\
            | If the ID is within the hardware list, return the controller              |
            \*-------------------------------------------------------------------------*/
            if(id < rgb_controllers_hw.size())
            {
                return rgb_controllers_hw[id];
            }

            /*-------------------------------------------------------------------------*\
            | Otherwise, offset the ID and check the remote list                        |
            \*-------------------------------------------------------------------------*/
            id -= rgb_controllers_hw.size();
            
            if(id < rgb_controllers_remote.size())
            {
                return rgb_controllers_remote[id];
            }

            /*-------------------------------------------------------------------------*\
            | If the ID is not in either list, return null pointer                      |
            \*-------------------------------------------------------------------------*/
            return nullptr;

        case CONTROLLER_LIST_HW:
            if(id < rgb_controllers_hw.size())
            {
                return rgb_controllers_hw[id];
            }
            return nullptr;

        case CONTROLLER_LIST_REMOTE:
            if(id < rgb_controllers_remote.size())
            {
                return rgb_controllers_remote[id];
            }
            return nullptr;
    }

    return nullptr;
}

int ResourceManager::GetUIListIndex(size_t id, ControllerList list)
{
    switch(list)
    {
        case CONTROLLER_LIST_REMOTE:
            id += rgb_controllers_hw.size();
            break;

        default:
            break;
    }

    return id;
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

void ResourceManager::RegisterDeviceDetector(std::string name, DeviceDetectorFunction detector)
{
    device_detector_strings.push_back(name);
    device_detectors.push_back(detector);
}

void ResourceManager::RegisterDeviceListChangeCallback(DeviceListChangeCallback callback, void * receiver)
{
    DeviceListChangeCallbackBlock block;

    block.callback = callback;
    block.receiver = receiver;

    DeviceListChangeCallbacks.push_back(block);
}

void ResourceManager::RegisterDetectionProgressCallback(DetectionProgressCallback callback, void * receiver)
{
    DetectionProgressCallbackBlock block;

    block.callback = callback;
    block.receiver = receiver;

    DetectionProgressCallbacks.push_back(block);
}

void ResourceManager::UnregisterDeviceListChangeCallback(DeviceListChangeCallback callback, void * receiver)
{
    /*-------------------------------------------------------------------------*\
    | Loop through the Device List Change Callbacks list and remove the given   |
    | entry                                                                     |
    \*-------------------------------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < DeviceListChangeCallbacks.size(); callback_idx++)
    {
        if(DeviceListChangeCallbacks[callback_idx].callback == callback && DeviceListChangeCallbacks[callback_idx].receiver == receiver)
        {
            DeviceListChangeCallbacks.erase(DeviceListChangeCallbacks.begin() + callback_idx);
            break;
        }
    }
}

void ResourceManager::UnregisterDetectionProgressCallback(DetectionProgressCallback callback, void * receiver)
{
    /*-------------------------------------------------------------------------*\
    | Loop through the Detection Progress Callbacks list and remove the given   |
    | entry                                                                     |
    \*-------------------------------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < DetectionProgressCallbacks.size(); callback_idx++)
    {
        if(DetectionProgressCallbacks[callback_idx].callback == callback && DetectionProgressCallbacks[callback_idx].receiver == receiver)
        {
            DetectionProgressCallbacks.erase(DetectionProgressCallbacks.begin() + callback_idx);
            break;
        }
    }
}

void ResourceManager::DeviceListChanged(int id, ControllerList list, bool removed)
{
    DeviceListChangeMutex.lock();

    /*-------------------------------------------------*\
    | Device list has changed, call the callbacks       |
    \*-------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < DeviceListChangeCallbacks.size(); callback_idx++)
    {
        DeviceListChangeCallbacks[callback_idx].callback(DeviceListChangeCallbacks[callback_idx].receiver, id, list, removed);
    }

    /*-------------------------------------------------*\
    | Device list has changed, inform all clients       |
    | connected to this server                          |
    \*-------------------------------------------------*/
    if(server)
    {
        server->DeviceListChanged();
    }

    DeviceListChangeMutex.unlock();
}

void ResourceManager::DetectionProgressChanged()
{
    DetectionProgressMutex.lock();

    /*-------------------------------------------------*\
    | Detection progress has changed, call the callbacks|
    \*-------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < DetectionProgressCallbacks.size(); callback_idx++)
    {
        DetectionProgressCallbacks[callback_idx].callback(DetectionProgressCallbacks[callback_idx].receiver);
    }

    DetectionProgressMutex.unlock();
}

std::string ResourceManager::GetConfigurationDirectory()
{
    std::string config_dir      = "";
    const char* xdg_config_home = getenv("XDG_CONFIG_HOME");
    const char* home            = getenv("HOME");
    const char* appdata         = getenv("APPDATA");

    /*-----------------------------------------------------*\
    | Check both XDG_CONFIG_HOME and APPDATA environment    |
    | variables.  If neither exist, use current directory   |
    \*-----------------------------------------------------*/
    if(xdg_config_home != NULL)
    {
        config_dir = xdg_config_home;
    }
    else if(appdata != NULL)
    {
        config_dir = appdata;
    }
    else if(home != NULL)
    {
        config_dir = home;
        config_dir = config_dir + "/.config";
    }

    /*-----------------------------------------------------*\
    | If a configuration directory was found, append OpenRGB|
    \*-----------------------------------------------------*/
    if(config_dir != "")
    {
        config_dir = config_dir + "/OpenRGB/";

        /*-------------------------------------------------------------------------*\
        | Create OpenRGB configuration directory if it doesn't exist                |
        \*-------------------------------------------------------------------------*/
        std::experimental::filesystem::create_directories(config_dir);
    }
    else
    {
        config_dir = "./";
    }

    return(config_dir);
}

NetworkServer* ResourceManager::GetServer()
{
    return(server);
}

unsigned int ResourceManager::GetClientCount()
{
    return clients.size();
}

static void OnNetworkClientDeviceListChange(void*, NetworkClient*, RGBController* controller, bool removed)
{
    if(removed)
    {
        ResourceManager::get()->RemoveRGBController(controller, CONTROLLER_LIST_REMOTE);
    }
    else
    {
        ResourceManager::get()->RegisterRGBController(controller, CONTROLLER_LIST_REMOTE);
    }
}

void ResourceManager::RegisterClient(NetworkClient* client)
{
    client->RegisterDeviceListChangeCallback(OnNetworkClientDeviceListChange, NULL);
    clients.push_back(client);
    // Notify about client list change?
}

NetworkClient* ResourceManager::GetClient(size_t id)
{
    /*-------------------------------------------------------------------------*\
    | Verify ID is valid, return null pointer if it is out of range             |
    \*-------------------------------------------------------------------------*/
    if(id >= clients.size())
    {
        return nullptr;
    }

    return clients[id];
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

    for(int controller_idx = GetControllerCount() - 1; controller_idx >= 0; --controller_idx)
    {
        RemoveRGBController(GetController(controller_idx));
    }

    std::vector<i2c_smbus_interface *> busses_copy = busses;

    busses.clear();

    for(i2c_smbus_interface* bus : busses_copy)
    {
        delete bus;
    }

    if(DetectDevicesThread)
    {
        DetectDevicesThread->join();
        delete DetectDevicesThread;
        DetectDevicesThread = nullptr;
    }
}

void ResourceManager::DetectDevices()
{
    /*-------------------------------------------------*\
    | Do nothing is it is already detecting devices     |
    \*-------------------------------------------------*/
    if(detection_is_required.load())
    {
        return;
    }

    /*-------------------------------------------------*\
    | If there's anything left from the last time,      |
    | we shall remove it first                          |
    \*-------------------------------------------------*/
    detection_percent = 0;
    detection_string  = "";

    DetectionProgressChanged();

    Cleanup();

    /*-------------------------------------------------*\
    | Start the device detection thread                 |
    \*-------------------------------------------------*/
    detection_is_required = true;
    DetectDevicesThread = new std::thread(&ResourceManager::DetectDevicesThreadFunction, this);

    /*-------------------------------------------------*\
    | Release the current thread to allow detection     |
    | thread to start                                   |
    \*-------------------------------------------------*/
    std::this_thread::sleep_for(1ms);
}

void ResourceManager::DetectDevicesThreadFunction()
{
    DetectDeviceMutex.lock();

    float               percent = 0.0f;
    json                detector_settings;
    bool                save_settings = false;

    /*-------------------------------------------------*\
    | Open device disable list and read in disabled     |
    | device strings                                    |
    \*-------------------------------------------------*/
    detector_settings = settings_manager->GetSettings("Setting_Detectors");

    /*-------------------------------------------------*\
    | Start at 0% detection progress                    |
    \*-------------------------------------------------*/
    detection_percent = 0;

    /*-------------------------------------------------*\
    | Detect i2c busses                                 |
    \*-------------------------------------------------*/
    for(unsigned int i2c_bus_detector_idx = 0; i2c_bus_detector_idx < i2c_bus_detectors.size() && detection_is_required.load(); i2c_bus_detector_idx++)
    {
        i2c_bus_detectors[i2c_bus_detector_idx](busses);
    }

    /*-------------------------------------------------*\
    | Detect i2c devices                                |
    \*-------------------------------------------------*/
    for(unsigned int i2c_detector_idx = 0; i2c_detector_idx < i2c_device_detectors.size() && detection_is_required.load(); i2c_detector_idx++)
    {
        detection_string = i2c_device_detector_strings[i2c_detector_idx].c_str();
        DetectionProgressChanged();

        bool this_device_enabled = true;
        if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
        {
            this_device_enabled = detector_settings["detectors"][detection_string];
        }
        else
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }

        if(this_device_enabled)
        {
            std::vector<RGBController*> tempVector; // To be removed
            i2c_device_detectors[i2c_detector_idx](busses, tempVector);
            for(size_t id = 0; id < tempVector.size(); ++id)
            {
                RegisterRGBController(tempVector[id]);
            }
        }

        percent = (i2c_detector_idx + 1.0f) / (i2c_device_detectors.size() + device_detectors.size());

        detection_percent = percent * 100.0f;
    }

    /*-------------------------------------------------*\
    | Detect other devices                              |
    \*-------------------------------------------------*/
    for(unsigned int detector_idx = 0; detector_idx < device_detectors.size() && detection_is_required.load(); detector_idx++)
    {
        detection_string = device_detector_strings[detector_idx].c_str();
        DetectionProgressChanged();

        bool this_device_enabled = true;
        if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
        {
            this_device_enabled = detector_settings["detectors"][detection_string];
        }
        else
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }

        if(this_device_enabled)
        {
            std::vector<RGBController*> tempVector; // To be removed
            device_detectors[detector_idx](tempVector);
            for(size_t id = 0; id < tempVector.size(); ++id)
            {
                RegisterRGBController(tempVector[id]);
            }
        }

        percent = (detector_idx + 1.0f + i2c_device_detectors.size()) / (i2c_device_detectors.size() + device_detectors.size());

        detection_percent = percent * 100.0f;
    }

    /*-------------------------------------------------*\
    | Make sure that when the detection is done,        |
    | progress bar is set to 100%                       |
    \*-------------------------------------------------*/

    detection_is_required = false;
    detection_percent = 100;
    detection_string = "";

    DetectionProgressChanged();
    
    DetectDeviceMutex.unlock();

    if(save_settings)
    {
        settings_manager->SetSettings("Setting_Detectors", detector_settings);

        settings_manager->SaveSettings();
    }
}

void ResourceManager::StopDeviceDetection()
{
    detection_is_required = false;
    detection_percent = 100;
    detection_string = "Stopping";
}

void ResourceManager::WaitForDeviceDetection()
{
    DetectDeviceMutex.lock();
    DetectDeviceMutex.unlock();
}
