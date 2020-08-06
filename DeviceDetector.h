#pragma once

#include <functional>
#include <vector>

#include "ResourceManager.h"

class DeviceDetector
{
public:
	DeviceDetector(DeviceDetectorFunction detector)
	{
		ResourceManager::get()->RegisterDeviceDetector(detector);
	}
};

class I2CDeviceDetector
{
public:
	I2CDeviceDetector(I2CDeviceDetectorFunction detector)
	{
		ResourceManager::get()->RegisterI2CDeviceDetector(detector);
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
