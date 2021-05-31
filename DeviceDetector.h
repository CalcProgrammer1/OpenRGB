#pragma once

#include <functional>
#include <string>
#include <vector>

#include "ResourceManager.h"

class DeviceDetector
{
public:
    DeviceDetector(std::string name, DeviceDetectorFunction detector)
	{
        ResourceManager::get()->RegisterDeviceDetector(name, detector);
	}
};

class I2CDeviceDetector
{
public:
    I2CDeviceDetector(std::string name, I2CDeviceDetectorFunction detector)
	{
        ResourceManager::get()->RegisterI2CDeviceDetector(name, detector);
	}
};

class I2CBusDetector
{
public:
    I2CBusDetector(I2CBusDetectorFunction detector)
    {
        ResourceManager::get()->RegisterI2CBusDetector(detector);
    }
};

class HIDDeviceDetector
{
public:
    HIDDeviceDetector(std::string name, HIDDeviceDetectorFunction detector, uint16_t vid, uint16_t pid, int64_t interface, int usage_page, int usage)
    {
        ResourceManager::get()->RegisterHIDDeviceDetector(name, detector, vid, pid, interface, usage_page, usage);
    }
};

class DynamicDetector
{
public:
    DynamicDetector(std::string name, DynamicDetectorFunction detector)
    {
        ResourceManager::get()->RegisterDynamicDetector(name, detector);
    }
};