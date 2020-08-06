#include "ResourceManager.h"


std::unique_ptr<ResourceManager> ResourceManager::instance;


ResourceManager *ResourceManager::get()
{
    if(!instance)
    {
        instance = std::make_unique<ResourceManager>();
    }
        
    return instance.get();
}

ResourceManager::~ResourceManager()
{
    for(i2c_smbus_interface* bus : busses)
    {
        delete bus;
    }

    for(RGBController* rgb_controller : rgb_controllers)
    {
        delete rgb_controller;
    }
}

void ResourceManager::RegisterI2CBus(i2c_smbus_interface *bus)
{
    busses.push_back(bus);
}

std::vector<i2c_smbus_interface*> & ResourceManager::GetI2CBusses()
{
    return busses;
}

void ResourceManager::RegisterRGBController(RGBController *rgb_controller)
{
    rgb_controllers.push_back(rgb_controller);
    DeviceListChanged();
}

std::vector<RGBController*> & ResourceManager::GetRGBControllers()
{
    return rgb_controllers;
}

void ResourceManager::RegisterI2CBusDetector(I2CBusDetectorFunction detector)
{
    i2c_bus_detectors.push_back(detector);
}

void ResourceManager::RegisterI2CDeviceDetector(I2CDeviceDetectorFunction detector)
{
    i2c_device_detectors.push_back(detector);
}

void ResourceManager::RegisterDeviceDetector(DeviceDetectorFunction detector)
{
    device_detectors.push_back(detector);
}

void ResourceManager::RegisterDeviceListChangeCallback(ResourceManagerCallback new_callback, void * new_callback_arg)
{
    DeviceListChangeCallbacks.push_back(new_callback);
    DeviceListChangeCallbackArgs.push_back(new_callback_arg);
}

void ResourceManager::DeviceListChanged()
{
    DeviceListChangeMutex.lock();

    /*-------------------------------------------------*\
    | Client info has changed, call the callbacks       |
    \*-------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < DeviceListChangeCallbacks.size(); callback_idx++)
    {
        DeviceListChangeCallbacks[callback_idx](DeviceListChangeCallbackArgs[callback_idx]);
    }

    DeviceListChangeMutex.unlock();
}

void ResourceManager::DetectDevices()
{
    /*-------------------------------------------------*\
    | Start the device detection thread                 |
    \*-------------------------------------------------*/
    DetectDevicesThread = new std::thread(&ResourceManager::DetectDevicesThreadFunction, this);
}

void ResourceManager::DetectDevicesThreadFunction()
{
    unsigned int prev_count = 0;

    /*-------------------------------------------------*\
    | Detect i2c busses                                 |
    \*-------------------------------------------------*/
    for(int i2c_bus_detector_idx = 0; i2c_bus_detector_idx < i2c_bus_detectors.size(); i2c_bus_detector_idx++)
    {
        i2c_bus_detectors[i2c_bus_detector_idx](busses);
    }

    /*-------------------------------------------------*\
    | Detect i2c devices                                |
    \*-------------------------------------------------*/
    for(int i2c_detector_idx = 0; i2c_detector_idx < i2c_device_detectors.size(); i2c_detector_idx++)
    {
        i2c_device_detectors[i2c_detector_idx](busses, rgb_controllers);

        /*-------------------------------------------------*\
        | If the device list size has changed, call the     |
        | device list changed callbacks                     |
        \*-------------------------------------------------*/
        if(rgb_controllers.size() != prev_count)
        {
            DeviceListChanged();
        }
        prev_count = rgb_controllers.size();
    }

    /*-------------------------------------------------*\
    | Detect other devices                              |
    \*-------------------------------------------------*/
    for(int detector_idx = 0; detector_idx < device_detectors.size(); detector_idx++)
    {
        device_detectors[detector_idx](rgb_controllers);

        /*-------------------------------------------------*\
        | If the device list size has changed, call the     |
        | device list changed callbacks                     |
        \*-------------------------------------------------*/
        if(rgb_controllers.size() != prev_count)
        {
            DeviceListChanged();
        }
        prev_count = rgb_controllers.size();
    }
}
