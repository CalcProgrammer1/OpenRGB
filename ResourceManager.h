#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <thread>
#include <string>

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
    
    ResourceManager();
    ~ResourceManager();
    
    void RegisterI2CBus(i2c_smbus_interface *);
    std::vector<i2c_smbus_interface*> & GetI2CBusses();
    
    void RegisterRGBController(RGBController *);
    std::vector<RGBController*> & GetRGBControllers();
    
    void RegisterI2CBusDetector         (I2CBusDetectorFunction     detector);
    void RegisterDeviceDetector         (std::string name, DeviceDetectorFunction     detector);
    void RegisterI2CDeviceDetector      (std::string name, I2CDeviceDetectorFunction  detector);
    
    void RegisterDeviceListChangeCallback(ResourceManagerCallback new_callback, void * new_callback_arg);

    unsigned int GetDetectionPercent();
    std::string  GetDetectionString();

    void DeviceListChanged();

    void DetectDevices();

    void DetectDevicesThreadFunction();

    void WaitForDeviceDetection();

private:
    static std::unique_ptr<ResourceManager>     instance;

    unsigned int                                detection_percent;
    std::string                                 detection_string;

    std::vector<i2c_smbus_interface*>           busses;
    std::vector<RGBController*>                 rgb_controllers;
    std::vector<DeviceDetectorFunction>         device_detectors;
    std::vector<std::string>                    device_detector_strings;
    std::vector<I2CBusDetectorFunction>         i2c_bus_detectors;
    std::vector<I2CDeviceDetectorFunction>      i2c_device_detectors;
    std::vector<std::string>                    i2c_device_detector_strings;

    std::thread *                               DetectDevicesThread;

    std::mutex                                  DetectDeviceMutex;
    
    std::mutex                                  DeviceListChangeMutex;
    std::vector<ResourceManagerCallback>        DeviceListChangeCallbacks;
    std::vector<void *>                         DeviceListChangeCallbackArgs;
};
