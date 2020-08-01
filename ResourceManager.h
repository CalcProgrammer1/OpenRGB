#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "i2c_smbus.h"
#include "RGBController.h"

typedef std::function<void(std::vector<RGBController*>&)>                                       DeviceDetectorFunction;
typedef std::function<void(std::vector<i2c_smbus_interface*>&, std::vector<RGBController*>&)>   I2CDeviceDetectorFunction;

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
    
    void RegisterDeviceDetector         (DeviceDetectorFunction     detector);
    void RegisterI2CDeviceDetector      (I2CDeviceDetectorFunction  detector);
    
    void DetectDevices();

private:
    static std::unique_ptr<ResourceManager>     instance;

    std::vector<i2c_smbus_interface*>           busses;
    std::vector<RGBController*>                 rgb_controllers;
    std::vector<DeviceDetectorFunction>         device_detectors;
    std::vector<I2CDeviceDetectorFunction>      i2c_device_detectors;
};
