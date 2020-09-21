#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <thread>
#include <string>

#include "i2c_smbus.h"
#include "NetworkClient.h"
#include "NetworkServer.h"
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
    const char*  GetDetectionString();
    NetworkServer* GetServer();

    void DeviceListChanged();

    void Cleanup();

    void DetectDevices();

    void DetectDevicesThreadFunction();

    void StopDeviceDetection();

    void WaitForDeviceDetection();

private:
    static std::unique_ptr<ResourceManager>     instance;

    /*-------------------------------------------------------------------------------------*\
    | I2C/SMBus Interfaces                                                                  |
    \*-------------------------------------------------------------------------------------*/
    std::vector<i2c_smbus_interface*>           busses;

    /*-------------------------------------------------------------------------------------*\
    | RGBControllers                                                                        |
    \*-------------------------------------------------------------------------------------*/
    //std::vector<RGBController*>                 rgb_controllers_hw;
    std::vector<RGBController*>                 rgb_controllers;

    /*-------------------------------------------------------------------------------------*\
    | Network Server                                                                        |
    \*-------------------------------------------------------------------------------------*/
    NetworkServer*                              server;

    /*-------------------------------------------------------------------------------------*\
    | Network Clients                                                                       |
    \*-------------------------------------------------------------------------------------*/
    //std::vector<NetworkClient*>                 clients;

    /*-------------------------------------------------------------------------------------*\
    | Detectors                                                                             |
    \*-------------------------------------------------------------------------------------*/
    std::vector<DeviceDetectorFunction>         device_detectors;
    std::vector<std::string>                    device_detector_strings;
    std::vector<I2CBusDetectorFunction>         i2c_bus_detectors;
    std::vector<I2CDeviceDetectorFunction>      i2c_device_detectors;
    std::vector<std::string>                    i2c_device_detector_strings;

    /*-------------------------------------------------------------------------------------*\
    | Detection Thread and Detection State                                                  |
    \*-------------------------------------------------------------------------------------*/
    std::thread *                               DetectDevicesThread;
    std::mutex                                  DetectDeviceMutex;

    std::atomic<bool>                           detection_is_required;
    std::atomic<unsigned int>                   detection_percent;
    const char*                                 detection_string;
    
    /*-------------------------------------------------------------------------------------*\
    | Device List Changed Callback                                                          |
    \*-------------------------------------------------------------------------------------*/
    std::mutex                                  DeviceListChangeMutex;
    std::vector<ResourceManagerCallback>        DeviceListChangeCallbacks;
    std::vector<void *>                         DeviceListChangeCallbackArgs;
};
