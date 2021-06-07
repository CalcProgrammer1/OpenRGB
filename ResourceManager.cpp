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
#include "LogManager.h"
#include "filesystem.h"

#include <stdlib.h>
#include <string>
#include <hidapi/hidapi.h>

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
    /*-------------------------------------------------------------------------*\
    | Initialize Detection Variables                                            |
    \*-------------------------------------------------------------------------*/
    detection_enabled           = true;
    detection_percent           = 100;
    detection_string            = "";
    detection_is_required       = false;
    DetectDevicesThread         = nullptr;
    dynamic_detectors_processed = false;

    SetupConfigurationDirectory();

    /*-------------------------------------------------------------------------*\
    | Load settings from file                                                   |
    \*-------------------------------------------------------------------------*/
    settings_manager        = new SettingsManager();
    settings_manager->LoadSettings(GetConfigurationDirectory() + "OpenRGB.json");

    /*-------------------------------------------------------------------------*\
    | Configure the log manager                                                 |
    \*-------------------------------------------------------------------------*/
    LogManager::get()->configure(settings_manager->GetSettings("Client"), GetConfigurationDirectory());

    /*-------------------------------------------------------------------------*\
    | Initialize Server Instance                                                |
    |   If configured, pass through full controller list including clients      |
    |   Otherwise, pass only local hardware controllers                         |
    \*-------------------------------------------------------------------------*/
    json server_settings    = settings_manager->GetSettings("Server");
    bool all_controllers    = false;

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

    /*-------------------------------------------------------------------------*\
    | Load sizes list from file                                                 |
    \*-------------------------------------------------------------------------*/
    profile_manager         = new ProfileManager(GetConfigurationDirectory());
    server->SetProfileManager(profile_manager);
    rgb_controllers_sizes   = profile_manager->LoadProfileToList("sizes", true);
}

ResourceManager::~ResourceManager()
{
    Cleanup();
}

void ResourceManager::RegisterI2CBus(i2c_smbus_interface *bus)
{
    LOG_NOTICE("Registering I2C interface: %s", bus->device_name);
    busses.push_back(bus);
}

std::vector<i2c_smbus_interface*> & ResourceManager::GetI2CBusses()
{
    return busses;
}

void ResourceManager::RegisterRGBController(RGBController *rgb_controller)
{
    LOG_NOTICE("Registering RGB controller: %s", rgb_controller->name.c_str());
    rgb_controllers_hw.push_back(rgb_controller);

    UpdateDeviceList();
}

void ResourceManager::UnregisterRGBController(RGBController* rgb_controller)
{
    LOG_NOTICE("Unregistering RGB controller: %s", rgb_controller->name.c_str());

    /*-------------------------------------------------------------------------*\
    | Clear callbacks from the controller before removal                        |
    \*-------------------------------------------------------------------------*/
    rgb_controller->ClearCallbacks();

    /*-------------------------------------------------------------------------*\
    | Find the controller to remove and remove it from the hardware list        |
    \*-------------------------------------------------------------------------*/
    std::vector<RGBController*>::iterator hw_it = std::find(rgb_controllers_hw.begin(), rgb_controllers_hw.end(), rgb_controller);

    if (hw_it != rgb_controllers_hw.end())
    {
        rgb_controllers_hw.erase(hw_it);
    }

    /*-------------------------------------------------------------------------*\
    | Find the controller to remove and remove it from the master list          |
    \*-------------------------------------------------------------------------*/
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
    block.address       = (vid << 16) | pid;
    block.function      = detector;
    block.interface     = interface;
    block.usage_page    = usage_page;
    block.usage         = usage;

    hid_device_detectors.push_back(block);
    hid_device_detector_strings.push_back(name);
}

void ResourceManager::RegisterDynamicDetector(std::string name, DynamicDetectorFunction detector)
{
    dynamic_detector_strings.push_back(name);
    dynamic_detectors.push_back(detector);
}

void ResourceManager::RegisterDeviceListChangeCallback(DeviceListChangeCallback new_callback, void * new_callback_arg)
{
    DeviceListChangeCallbacks.push_back(new_callback);
    DeviceListChangeCallbackArgs.push_back(new_callback_arg);
}

void ResourceManager::RegisterI2CBusListChangeCallback(I2CBusListChangeCallback new_callback, void * new_callback_arg)
{
    I2CBusListChangeCallbacks.push_back(new_callback);
    I2CBusListChangeCallbackArgs.push_back(new_callback_arg);
}

void ResourceManager::RegisterDetectionProgressCallback(DetectionProgressCallback new_callback, void *new_callback_arg)
{
    DetectionProgressCallbacks.push_back(new_callback);
    DetectionProgressCallbackArgs.push_back(new_callback_arg);
}

void ResourceManager::RegisterDetectionStartCallback(DetectionStartCallback new_callback, void *new_callback_arg)
{
    DetectionStartCallbacks.push_back(new_callback);
    DetectionStartCallbackArgs.push_back(new_callback_arg);
}

void ResourceManager::RegisterDetectionEndCallback(DetectionEndCallback new_callback, void *new_callback_arg)
{
    DetectionEndCallbacks.push_back(new_callback);
    DetectionEndCallbackArgs.push_back(new_callback_arg);
}

void ResourceManager::UpdateDeviceList()
{
    DeviceListChangeMutex.lock();

    /*-------------------------------------------------*\
    | Insert hardware controllers into controller list  |
    \*-------------------------------------------------*/
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

    /*-------------------------------------------------*\
    | Device list has changed, call the callbacks       |
    \*-------------------------------------------------*/
    DeviceListChanged();

    /*-------------------------------------------------*\
    | Device list has changed, inform all clients       |
    | connected to this server                          |
    \*-------------------------------------------------*/
    server->DeviceListChanged();

    DeviceListChangeMutex.unlock();
}

void ResourceManager::DeviceListChanged()
{
    /*-------------------------------------------------*\
    | Device list has changed, call the callbacks       |
    \*-------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < DeviceListChangeCallbacks.size(); callback_idx++)
    {
        ResourceManager::DeviceListChangeCallbacks[callback_idx](DeviceListChangeCallbackArgs[callback_idx]);
    }
}

void ResourceManager::DetectionProgressChanged()
{
    DetectionProgressMutex.lock();

    /*-------------------------------------------------*\
    | Detection progress has changed, call the callbacks|
    \*-------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < DetectionProgressCallbacks.size(); callback_idx++)
    {
        DetectionProgressCallbacks[callback_idx](DetectionProgressCallbackArgs[callback_idx]);
    }

    DetectionProgressMutex.unlock();
}

void ResourceManager::I2CBusListChanged()
{
    I2CBusListChangeMutex.lock();

    /*-------------------------------------------------*\
    | Detection progress has changed, call the callbacks|
    \*-------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < I2CBusListChangeCallbacks.size(); callback_idx++)
    {
        I2CBusListChangeCallbacks[callback_idx](I2CBusListChangeCallbackArgs[callback_idx]);
    }

    I2CBusListChangeMutex.unlock();
}

void ResourceManager::SetupConfigurationDirectory()
{
    config_dir.clear();
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
        filesystem::create_directories(config_dir);

        /*-------------------------------------------------------------------------*\
        | Create OpenRGB plugins directory                                          |
        \*-------------------------------------------------------------------------*/
        std::string plugins_dir = config_dir + "plugins";

        filesystem::create_directories(plugins_dir);
    }
    else
    {
        config_dir = "./";
    }
}

std::string ResourceManager::GetConfigurationDirectory()
{
    return(config_dir);
}

NetworkServer* ResourceManager::GetServer()
{
    return(server);
}

static void NetworkClientInfoChangeCallback(void* this_ptr)
{
    ResourceManager* this_obj = (ResourceManager*)this_ptr;

    this_obj->DeviceListChanged();
}

void ResourceManager::RegisterNetworkClient(NetworkClient* new_client)
{
    new_client->RegisterClientInfoChangeCallback(NetworkClientInfoChangeCallback, this);

    clients.push_back(new_client);
}

void ResourceManager::UnregisterNetworkClient(NetworkClient* network_client)
{
    /*-------------------------------------------------------------------------*\
    | Stop the disconnecting client                                             |
    \*-------------------------------------------------------------------------*/
    network_client->StopClient();

    /*-------------------------------------------------------------------------*\
    | Clear callbacks from the client before removal                            |
    \*-------------------------------------------------------------------------*/
    network_client->ClearCallbacks();

    /*-------------------------------------------------------------------------*\
    | Find the client to remove and remove it from the clients list             |
    \*-------------------------------------------------------------------------*/
    std::vector<NetworkClient*>::iterator client_it = std::find(clients.begin(), clients.end(), network_client);

    if(client_it != clients.end())
    {
        clients.erase(client_it);
    }

    /*-------------------------------------------------------------------------*\
    | Delete the client                                                         |
    \*-------------------------------------------------------------------------*/
    delete network_client;

    UpdateDeviceList();
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

void ResourceManager::SetConfigurationDirectory(std::string directory)
{
    settings_manager->LoadSettings(directory + "OpenRGB.json");
    profile_manager->SetConfigurationDirectory(directory);

    rgb_controllers_sizes.clear();
    rgb_controllers_sizes   = profile_manager->LoadProfileToList("sizes", true);
}

void ResourceManager::Cleanup()
{
    ResourceManager::get()->WaitForDeviceDetection();

    std::vector<RGBController *> rgb_controllers_hw_copy = rgb_controllers_hw;

    for(unsigned int hw_controller_idx = 0; hw_controller_idx < rgb_controllers_hw.size(); hw_controller_idx++)
    {
        for(unsigned int controller_idx = 0; controller_idx < rgb_controllers.size(); controller_idx++)
        {
            if(rgb_controllers[controller_idx] == rgb_controllers_hw[hw_controller_idx])
            {
                rgb_controllers.erase(rgb_controllers.begin() + controller_idx);
                break;
            }
        }
    }

    rgb_controllers_hw.clear();

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

    /*-------------------------------------------------*\
    | Cleanup HID interface                             |
    \*-------------------------------------------------*/
    int hid_status = hid_exit();

    LOG_DEBUG("Closing HID interfaces: %s", ((hid_status == 0) ? "Success" : "Failed"));

    if(DetectDevicesThread)
    {
        DetectDevicesThread->join();
        delete DetectDevicesThread;
        DetectDevicesThread = nullptr;
    }
}

void ResourceManager::ProcessDynamicDetectors()
{
    for(unsigned int detector_idx = 0; detector_idx < dynamic_detectors.size(); detector_idx++)
    {
        dynamic_detectors[detector_idx]();
    }

    dynamic_detectors_processed = true;
}

void ResourceManager::DetectDevices()
{
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
    for(unsigned int callback_idx = 0; callback_idx < DetectionStartCallbacks.size(); callback_idx++)
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

        UpdateDeviceList();

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
    else
    {
        /*-------------------------------------------------*\
        | Signal that detection is complete                 |
        \*-------------------------------------------------*/
        detection_percent     = 100;
        DetectionProgressChanged();

        /*-----------------------------------------------------*\
        | Call detection end callbacks                          |
        \*-----------------------------------------------------*/
        for(unsigned int callback_idx = 0; callback_idx < DetectionEndCallbacks.size(); callback_idx++)
        {
            DetectionEndCallbacks[callback_idx](DetectionEndCallbackArgs[callback_idx]);
        }
    }   
}

void ResourceManager::DisableDetection()
{
    detection_enabled = false;
}

void ResourceManager::DetectDevicesThreadFunction()
{
    DetectDeviceMutex.lock();

    hid_device_info*    current_hid_device;
    float               percent             = 0.0f;
    float               percent_denominator = 0.0f;
    json                detector_settings;
    unsigned int        hid_device_count    = 0;
    hid_device_info*    hid_devices         = NULL;
    bool                hid_safe_mode       = false;
    unsigned int        prev_count          = 0;
    std::vector<bool>   size_used;

    LOG_NOTICE("Detection started");

    size_used.resize(rgb_controllers_sizes.size());

    for(unsigned int size_idx = 0; size_idx < size_used.size(); size_idx++)
    {
        size_used[size_idx] = false;
    }

    /*-------------------------------------------------*\
    | Open device disable list and read in disabled     |
    | device strings                                    |
    \*-------------------------------------------------*/
    detector_settings = settings_manager->GetSettings("Detectors");

    /*-------------------------------------------------*\
    | Initialize HID interface for detection            |
    \*-------------------------------------------------*/
    int hid_status = hid_init();

    LOG_NOTICE("Initializing HID interfaces: %s", ((hid_status == 0) ? "Success" : "Failed"));

    /*-------------------------------------------------*\
    | Check HID safe mode setting                       |
    \*-------------------------------------------------*/
    if(detector_settings.contains("hid_safe_mode"))
    {
        hid_safe_mode = detector_settings["hid_safe_mode"];
    }

    /*-------------------------------------------------*\
    | Calculate the percentage denominator by adding    |
    | the number of I2C and miscellaneous detectors and |
    | the number of enumerated HID devices              |
    |                                                   |
    | Start by iterating through all HID devices in     |
    | list to get a total count                         |
    \*-------------------------------------------------*/
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

    percent_denominator = i2c_device_detectors.size() + device_detectors.size() + hid_device_count;

    /*-------------------------------------------------*\
    | Start at 0% detection progress                    |
    \*-------------------------------------------------*/
    detection_percent = 0;

    /*-------------------------------------------------*\
    | Detect i2c interfaces                             |
    \*-------------------------------------------------*/
    LOG_NOTICE("Detecting I2C interfaces");

    for(unsigned int i2c_bus_detector_idx = 0; i2c_bus_detector_idx < i2c_bus_detectors.size() && detection_is_required.load(); i2c_bus_detector_idx++)
    {
        i2c_bus_detectors[i2c_bus_detector_idx]();
        I2CBusListChanged();
    }

    /*-------------------------------------------------*\
    | Detect i2c devices                                |
    \*-------------------------------------------------*/
    LOG_NOTICE("Detecting I2C devices");

    for(unsigned int i2c_detector_idx = 0; i2c_detector_idx < i2c_device_detectors.size() && detection_is_required.load(); i2c_detector_idx++)
    {
        detection_string = i2c_device_detector_strings[i2c_detector_idx].c_str();

        /*-------------------------------------------------*\
        | Check if this detector is enabled                 |
        \*-------------------------------------------------*/
        bool this_device_enabled = true;
        if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
        {
            this_device_enabled = detector_settings["detectors"][detection_string];
        }

        if(this_device_enabled)
        {
            DetectionProgressChanged();
            
            i2c_device_detectors[i2c_detector_idx](busses);
        }

        /*-------------------------------------------------*\
        | If the device list size has changed, call the     |
        | device list changed callbacks                     |
        \*-------------------------------------------------*/
        if(rgb_controllers_hw.size() != prev_count)
        {
            /*-------------------------------------------------*\
            | First, load sizes for the new controllers         |
            \*-------------------------------------------------*/
            for(unsigned int controller_size_idx = prev_count; controller_size_idx < rgb_controllers_hw.size(); controller_size_idx++)
            {
                profile_manager->LoadDeviceFromListWithOptions(rgb_controllers_sizes, size_used, rgb_controllers_hw[controller_size_idx], true, false);
            }

            UpdateDeviceList();
        }
        prev_count = rgb_controllers_hw.size();

        /*-------------------------------------------------*\
        | Update detection percent                          |
        \*-------------------------------------------------*/
        percent = (i2c_detector_idx + 1.0f) / percent_denominator;

        detection_percent = percent * 100.0f;
    }

    /*-------------------------------------------------*\
    | Detect HID devices                                |
    |                                                   |
    | Reset current device pointer to first device      |
    \*-------------------------------------------------*/
    current_hid_device = hid_devices;

    if(hid_safe_mode)
    {
        LOG_NOTICE("Detecting HID devices in safe mode");

        /*-----------------------------------------------------------------------------*\
        | Loop through all available detectors.  If all required information matches,   |
        | run the detector                                                              |
        \*-----------------------------------------------------------------------------*/
        for(unsigned int hid_detector_idx = 0; hid_detector_idx < hid_device_detectors.size() && detection_is_required.load(); hid_detector_idx++)
        {
            hid_devices = hid_enumerate(hid_device_detectors[hid_detector_idx].address >> 16, hid_device_detectors[hid_detector_idx].address & 0x0000FFFF);

            LOG_VERBOSE("Trying to run detector for [%s] (for 0x%08hx)", hid_device_detectors[hid_detector_idx].name.c_str(), hid_device_detectors[hid_detector_idx].address);

            current_hid_device = hid_devices;

            while(current_hid_device)
            {
                unsigned int addr = (current_hid_device->vendor_id << 16) | current_hid_device->product_id;

                if(( ( hid_device_detectors[hid_detector_idx].address      == addr                                   ) )
#ifdef USE_HID_USAGE
                && ( (   hid_device_detectors[hid_detector_idx].usage_page == HID_USAGE_PAGE_ANY                     )
                  || (   hid_device_detectors[hid_detector_idx].usage_page == current_hid_device->usage_page         ) )
                && ( (   hid_device_detectors[hid_detector_idx].usage      == HID_USAGE_ANY                          )
                  || (   hid_device_detectors[hid_detector_idx].usage      == current_hid_device->usage              ) )
                && ( (   hid_device_detectors[hid_detector_idx].interface  == HID_INTERFACE_ANY                      )
                  || ( ( hid_device_detectors[hid_detector_idx].usage      != HID_USAGE_PAGE_ANY                   )
                    || ( hid_device_detectors[hid_detector_idx].usage_page != HID_USAGE_PAGE_ANY                   )
                    || ( hid_device_detectors[hid_detector_idx].interface  == current_hid_device->interface_number ) ) )
#else
                && ( ( hid_device_detectors[hid_detector_idx].interface  == HID_INTERFACE_ANY                        )
                  || ( hid_device_detectors[hid_detector_idx].interface  == current_hid_device->interface_number     ) )
#endif
                )
                {
                    detection_string = hid_device_detectors[hid_detector_idx].name.c_str();

                    /*-------------------------------------------------*\
                    | Check if this detector is enabled or needs to be  |
                    | added to the settings list                        |
                    \*-------------------------------------------------*/
                    bool this_device_enabled = true;
                    if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
                    {
                        this_device_enabled = detector_settings["detectors"][detection_string];
                    }

                    if(this_device_enabled)
                    {
                        DetectionProgressChanged();

                        hid_device_detectors[hid_detector_idx].function(current_hid_device, hid_device_detectors[hid_detector_idx].name);

                        /*-------------------------------------------------*\
                        | If the device list size has changed, call the     |
                        | device list changed callbacks                     |
                        \*-------------------------------------------------*/
                        if(rgb_controllers_hw.size() != prev_count)
                        {
                            /*-------------------------------------------------*\
                            | First, load sizes for the new controllers         |
                            \*-------------------------------------------------*/
                            for(unsigned int controller_size_idx = prev_count; controller_size_idx < rgb_controllers_hw.size(); controller_size_idx++)
                            {
                                profile_manager->LoadDeviceFromListWithOptions(rgb_controllers_sizes, size_used, rgb_controllers_hw[controller_size_idx], true, false);
                            }

                            UpdateDeviceList();
                        }
                        prev_count = rgb_controllers_hw.size();
                    }
                }

                current_hid_device = current_hid_device->next;
            }

            hid_free_enumeration(hid_devices);
        }
    }
    else
    {
        LOG_NOTICE("Detecting HID devices");

        /*-------------------------------------------------*\
        | Iterate through all devices in list and run       |
        | detectors                                         |
        \*-------------------------------------------------*/
        hid_device_count = 0;

        while(current_hid_device)
        {
            LOG_DEBUG("HID device [%04X:%04X]", current_hid_device->vendor_id, current_hid_device->product_id);

            detection_string = "";
            DetectionProgressChanged();
            
            unsigned int addr = (current_hid_device->vendor_id << 16) | current_hid_device->product_id;

            /*-----------------------------------------------------------------------------*\
            | Loop through all available detectors.  If all required information matches,   |
            | run the detector                                                              |
            \*-----------------------------------------------------------------------------*/
            for(unsigned int hid_detector_idx = 0; hid_detector_idx < hid_device_detectors.size() && detection_is_required.load(); hid_detector_idx++)
            {
                if(( ( hid_device_detectors[hid_detector_idx].address      == addr                                   ) )
#ifdef USE_HID_USAGE
                && ( (   hid_device_detectors[hid_detector_idx].usage_page == HID_USAGE_PAGE_ANY                     )
                  || (   hid_device_detectors[hid_detector_idx].usage_page == current_hid_device->usage_page         ) )
                && ( (   hid_device_detectors[hid_detector_idx].usage      == HID_USAGE_ANY                          )
                  || (   hid_device_detectors[hid_detector_idx].usage      == current_hid_device->usage              ) )
                && ( (   hid_device_detectors[hid_detector_idx].interface  == HID_INTERFACE_ANY                      )
                  || ( ( hid_device_detectors[hid_detector_idx].usage      != HID_USAGE_PAGE_ANY                   )
                    || ( hid_device_detectors[hid_detector_idx].usage_page != HID_USAGE_PAGE_ANY                   )
                    || ( hid_device_detectors[hid_detector_idx].interface  == current_hid_device->interface_number ) ) )
#else
                && ( ( hid_device_detectors[hid_detector_idx].interface  == HID_INTERFACE_ANY                        )
                  || ( hid_device_detectors[hid_detector_idx].interface  == current_hid_device->interface_number     ) )
#endif
                )
                {
                    detection_string = hid_device_detectors[hid_detector_idx].name.c_str();
                    LOG_DEBUG("Detector found: %s", detection_string);

                    /*-------------------------------------------------*\
                    | Check if this detector is enabled or needs to be  |
                    | added to the settings list                        |
                    \*-------------------------------------------------*/
                    bool this_device_enabled = true;
                    if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
                    {
                        this_device_enabled = detector_settings["detectors"][detection_string];
                    }

                    if(this_device_enabled)
                    {
                        LOG_DEBUG("Detector %s is enabled, running it", detection_string);

                        DetectionProgressChanged();

                        hid_device_detectors[hid_detector_idx].function(current_hid_device, hid_device_detectors[hid_detector_idx].name);
                    }
                }
            }

            /*-------------------------------------------------*\
            | Update detection percent                          |
            \*-------------------------------------------------*/
            hid_device_count++;

            percent = (i2c_device_detectors.size() + hid_device_count) / percent_denominator;

            detection_percent = percent * 100.0f;

            /*-------------------------------------------------*\
            | Move on to the next HID device                    |
            \*-------------------------------------------------*/
            current_hid_device = current_hid_device->next;
        }

        /*-------------------------------------------------*\
        | Done using the device list, free it               |
        \*-------------------------------------------------*/
        hid_free_enumeration(hid_devices);
    }

    /*-------------------------------------------------*\
    | Detect other devices                              |
    \*-------------------------------------------------*/
    LOG_NOTICE("Detecting other devices");

    for(unsigned int detector_idx = 0; detector_idx < device_detectors.size() && detection_is_required.load(); detector_idx++)
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

        if(this_device_enabled)
        {
            DetectionProgressChanged();
            
            device_detectors[detector_idx](rgb_controllers_hw);
        }

        /*-------------------------------------------------*\
        | If the device list size has changed, call the     |
        | device list changed callbacks                     |
        \*-------------------------------------------------*/
        if(rgb_controllers_hw.size() != prev_count)
        {
            /*-------------------------------------------------*\
            | First, load sizes for the new controllers         |
            \*-------------------------------------------------*/
            for(unsigned int controller_size_idx = prev_count; controller_size_idx < rgb_controllers_hw.size(); controller_size_idx++)
            {
                profile_manager->LoadDeviceFromListWithOptions(rgb_controllers_sizes, size_used, rgb_controllers_hw[controller_size_idx], true, false);
            }

            UpdateDeviceList();
        }
        prev_count = rgb_controllers_hw.size();

        /*-------------------------------------------------*\
        | Update detection percent                          |
        \*-------------------------------------------------*/
        percent = (i2c_device_detectors.size() + hid_device_count + detector_idx + 1.0f) / percent_denominator;

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

    /*-----------------------------------------------------*\
    | Call detection end callbacks                          |
    \*-----------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < DetectionEndCallbacks.size(); callback_idx++)
    {
        DetectionEndCallbacks[callback_idx](DetectionEndCallbackArgs[callback_idx]);
    }

    LOG_NOTICE("Detection completed");
}

void ResourceManager::StopDeviceDetection()
{
    LOG_NOTICE("Detection abort requested");
    detection_is_required = false;
    detection_percent = 100;
    detection_string = "Stopping";
}

void ResourceManager::UpdateDetectorSettings()
{
    json                detector_settings;
    bool                save_settings       = false;
    
    /*-------------------------------------------------*\
    | Open device disable list and read in disabled     |
    | device strings                                    |
    \*-------------------------------------------------*/
    detector_settings = settings_manager->GetSettings("Detectors");

    /*-------------------------------------------------*\
    | Loop through all I2C detectors and see if any     |
    | need to be saved to the settings                  |
    \*-------------------------------------------------*/
    for(unsigned int i2c_detector_idx = 0; i2c_detector_idx < i2c_device_detectors.size(); i2c_detector_idx++)
    {
        detection_string = i2c_device_detector_strings[i2c_detector_idx].c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-------------------------------------------------*\
    | Loop through all HID detectors and see if any     |
    | need to be saved to the settings                  |
    \*-------------------------------------------------*/
    for(unsigned int hid_detector_idx = 0; hid_detector_idx < hid_device_detectors.size(); hid_detector_idx++)
    {
        detection_string = hid_device_detectors[hid_detector_idx].name.c_str();
        
        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-------------------------------------------------*\
    | Loop through remaining detectors and see if any   |
    | need to be saved to the settings                  |
    \*-------------------------------------------------*/
    for(unsigned int detector_idx = 0; detector_idx < device_detectors.size(); detector_idx++)
    {
        detection_string = device_detector_strings[detector_idx].c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            /*-------------------------------------------------*\
            | Default the OpenRazer detector to disabled, as it |
            | overrides RazerController when enabled            |
            \*-------------------------------------------------*/
            if(strcmp(detection_string, "OpenRazer") == 0 || strcmp(detection_string, "OpenRazer-Win32") == 0)
            {
                detector_settings["detectors"][detection_string] = false;
            }
            else
            {
                detector_settings["detectors"][detection_string] = true;
            }
            save_settings = true;
        }
    }

    if(save_settings)
    {
        LOG_NOTICE("Saving detector settings");

        settings_manager->SetSettings("Detectors", detector_settings);

        settings_manager->SaveSettings();
    }
}

void ResourceManager::WaitForDeviceDetection()
{
    DetectDeviceMutex.lock();
    DetectDeviceMutex.unlock();
}
