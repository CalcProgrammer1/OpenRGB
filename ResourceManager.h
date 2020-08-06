#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <thread>

#include "i2c_smbus.h"
#include "RGBController.h"

typedef std::function<void(std::vector<i2c_smbus_interface*>&)>                                 I2CBusDetectorFunction;
typedef std::function<void(std::vector<RGBController*>&)>                                       DeviceDetectorFunction;
typedef std::function<void(std::vector<i2c_smbus_interface*>&, std::vector<RGBController*>&)>   I2CDeviceDetectorFunction;

typedef void (*ResourceManagerCallback)(void *);

class ResourceManager
{
public:
    static ResourceManager *get();
    
    ResourceManager() = default;
    ~ResourceManager();
    
    void RegisterI2CBus(i2c_smbus_interface *);
    std::vector<i2c_smbus_interface*> & GetI2CBusses();
    
    void RegisterRGBController(RGBController *);
    std::vector<RGBController*> & GetRGBControllers();
    
    void RegisterI2CBusDetector         (I2CBusDetectorFunction     detector);
    void RegisterDeviceDetector         (DeviceDetectorFunction     detector);
    void RegisterI2CDeviceDetector      (I2CDeviceDetectorFunction  detector);
    
    void RegisterDeviceListChangeCallback(ResourceManagerCallback new_callback, void * new_callback_arg);

    void DeviceListChanged();

    void DetectDevices();

    void DetectDevicesThreadFunction();

private:
    static std::unique_ptr<ResourceManager>     instance;

    std::vector<i2c_smbus_interface*>           busses;
    std::vector<RGBController*>                 rgb_controllers;
    std::vector<DeviceDetectorFunction>         device_detectors;
    std::vector<I2CBusDetectorFunction>         i2c_bus_detectors;
    std::vector<I2CDeviceDetectorFunction>      i2c_device_detectors;

    std::thread *                               DetectDevicesThread;

    std::mutex                                  DeviceListChangeMutex;
    std::vector<ResourceManagerCallback>        DeviceListChangeCallbacks;
    std::vector<void *>                         DeviceListChangeCallbackArgs;
};
