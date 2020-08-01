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
}

std::vector<RGBController*> & ResourceManager::GetRGBControllers()
{
    return rgb_controllers;
}

void ResourceManager::RegisterI2CDeviceDetector(I2CDeviceDetectorFunction detector)
{
    i2c_device_detectors.push_back(detector);
}

void ResourceManager::RegisterDeviceDetector(DeviceDetectorFunction detector)
{
    device_detectors.push_back(detector);
}

void ResourceManager::DetectDevices()
{
    for(int i2c_detector_idx = 0; i2c_detector_idx < i2c_device_detectors.size(); i2c_detector_idx++)
    {
        i2c_device_detectors[i2c_detector_idx](busses, rgb_controllers);
    }

    for(int detector_idx = 0; detector_idx < device_detectors.size(); detector_idx++)
    {
        device_detectors[detector_idx](rgb_controllers);
    }
}