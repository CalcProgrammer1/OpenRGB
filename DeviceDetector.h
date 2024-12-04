/*---------------------------------------------------------*\
| DeviceDetector.h                                          |
|                                                           |
|   Device detector functionality                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

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

class I2CDIMMDeviceDetector
{
public:
    I2CDIMMDeviceDetector(std::string name, I2CDIMMDeviceDetectorFunction detector, uint16_t jedec_id, uint8_t dimm_type)
	{
        ResourceManager::get()->RegisterI2CDIMMDeviceDetector(name, detector, jedec_id, dimm_type);
	}
};

class I2CPCIDeviceDetector
{
public:
    I2CPCIDeviceDetector(std::string name, I2CPCIDeviceDetectorFunction detector, uint16_t ven_id, uint16_t dev_id, uint16_t subven_id, uint16_t subdev_id, uint8_t i2c_addr)
    {
        ResourceManager::get()->RegisterI2CPCIDeviceDetector(name, detector, ven_id, dev_id, subven_id, subdev_id, i2c_addr);
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
    HIDDeviceDetector(std::string name, HIDDeviceDetectorFunction detector, uint16_t vid, uint16_t pid, int interface, int usage_page, int usage)
    {
        ResourceManager::get()->RegisterHIDDeviceDetector(name, detector, vid, pid, interface, usage_page, usage);
    }
};

class HIDWrappedDeviceDetector
{
public:
    HIDWrappedDeviceDetector(std::string name, HIDWrappedDeviceDetectorFunction detector, uint16_t vid, uint16_t pid, int interface, int usage_page, int usage)
    {
        ResourceManager::get()->RegisterHIDWrappedDeviceDetector(name, detector, vid, pid, interface, usage_page, usage);
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

class PreDetectionHook
{
public:
    PreDetectionHook(PreDetectionHookFunction hook)
    {
        ResourceManager::get()->RegisterPreDetectionHook(hook);
    }
};
