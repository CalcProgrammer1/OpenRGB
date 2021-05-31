/*-----------------------------------------*\
|  ResourceManager.h                        |
|                                           |
|  OpenRGB Resource Manager controls access |
|  to application components including      |
|  RGBControllers, I2C interfaces, and      |
|  network SDK components                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/27/2020   |
\*-----------------------------------------*/

#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <thread>
#include <string>

#include "i2c_smbus.h"
#include "NetworkClient.h"
#include "NetworkServer.h"
#include "ProfileManager.h"
#include "RGBController.h"
#include "SettingsManager.h"

#define HID_INTERFACE_ANY   -1
#define HID_USAGE_ANY       -1
#define HID_USAGE_PAGE_ANY  -1L

#define CONTROLLER_LIST_HID 0

struct hid_device_info;

typedef std::function<void()>                                       I2CBusDetectorFunction;
typedef std::function<void(std::vector<RGBController*>&)>           DeviceDetectorFunction;
typedef std::function<void(std::vector<i2c_smbus_interface*>&)>     I2CDeviceDetectorFunction;
typedef std::function<void(hid_device_info*, const std::string&)>   HIDDeviceDetectorFunction;
typedef std::function<void()>                                       DynamicDetectorFunction;
typedef struct
{
    std::string                 name;
    HIDDeviceDetectorFunction   function;
    unsigned int                address;
    int                         interface;
    int                         usage_page;
    int                         usage;
} HIDDeviceDetectorBlock;

typedef void (*DeviceListChangeCallback)(void *);
typedef void (*DetectionProgressCallback)(void *);
typedef void (*DetectionStartCallback)(void *);
typedef void (*DetectionEndCallback)(void *);
typedef void (*I2CBusListChangeCallback)(void *);

class ResourceManagerInterface
{
public:
    virtual std::vector<i2c_smbus_interface*> & GetI2CBusses()                                                                                      = 0;

    virtual void                                RegisterRGBController(RGBController *rgb_controller)                                                = 0;
    virtual void                                UnregisterRGBController(RGBController *rgb_controller)                                              = 0;

    virtual void                                RegisterDeviceListChangeCallback(DeviceListChangeCallback new_callback, void * new_callback_arg)    = 0;
    virtual void                                RegisterDetectionProgressCallback(DetectionProgressCallback new_callback, void * new_callback_arg)  = 0;
    virtual void                                RegisterDetectionStartCallback(DetectionStartCallback new_callback, void * new_callback_arg)        = 0;
    virtual void                                RegisterDetectionEndCallback(DetectionEndCallback new_callback, void * new_callback_arg)            = 0;
    virtual void                                RegisterI2CBusListChangeCallback(I2CBusListChangeCallback new_callback, void * new_callback_arg)    = 0;

    virtual std::vector<RGBController*> &       GetRGBControllers()                                                                                 = 0;

    virtual unsigned int                        GetDetectionPercent()                                                                               = 0;

    virtual std::string                         GetConfigurationDirectory()                                                                         = 0;

    virtual std::vector<NetworkClient*>&        GetClients()                                                                                        = 0;
    virtual NetworkServer*                      GetServer()                                                                                         = 0;

    virtual ProfileManager*                     GetProfileManager()                                                                                 = 0;
    virtual SettingsManager*                    GetSettingsManager()                                                                                = 0;

    virtual void                                UpdateDeviceList()                                                                                  = 0;
    virtual void                                WaitForDeviceDetection()                                                                            = 0;

protected:
    virtual                                    ~ResourceManagerInterface() {};
};

class ResourceManager: public ResourceManagerInterface
{
public:
    static ResourceManager *get();
    
    ResourceManager();
    ~ResourceManager();

    void RegisterI2CBus(i2c_smbus_interface *);
    std::vector<i2c_smbus_interface*> & GetI2CBusses();
    
    void RegisterRGBController(RGBController *rgb_controller);
    void UnregisterRGBController(RGBController *rgb_controller);

    std::vector<RGBController*> & GetRGBControllers();
    
    void RegisterI2CBusDetector         (I2CBusDetectorFunction     detector);
    void RegisterDeviceDetector         (std::string name, DeviceDetectorFunction     detector);
    void RegisterI2CDeviceDetector      (std::string name, I2CDeviceDetectorFunction  detector);
    void RegisterHIDDeviceDetector      (std::string name,
                                         HIDDeviceDetectorFunction  detector,
                                         uint16_t vid,
                                         uint16_t pid,
                                         int interface  = HID_INTERFACE_ANY,
                                         int usage_page = HID_USAGE_PAGE_ANY,
                                         int usage      = HID_USAGE_ANY);
    void RegisterDynamicDetector        (std::string name, DynamicDetectorFunction detector);
    
    void RegisterDeviceListChangeCallback(DeviceListChangeCallback new_callback, void * new_callback_arg);
    void RegisterDetectionProgressCallback(DetectionProgressCallback new_callback, void * new_callback_arg);
    void RegisterDetectionStartCallback(DetectionStartCallback new_callback, void * new_callback_arg);
    void RegisterDetectionEndCallback(DetectionEndCallback new_callback, void * new_callback_arg);
    void RegisterI2CBusListChangeCallback(I2CBusListChangeCallback new_callback, void * new_callback_arg);

    bool         GetDetectionEnabled();
    unsigned int GetDetectionPercent();
    const char*  GetDetectionString();

    std::string                     GetConfigurationDirectory();

    void RegisterNetworkClient(NetworkClient* new_client);
    void UnregisterNetworkClient(NetworkClient* network_client);

    std::vector<NetworkClient*>&    GetClients();
    NetworkServer*                  GetServer();

    ProfileManager*                 GetProfileManager();
    SettingsManager*                GetSettingsManager();

    void                            SetConfigurationDirectory(std::string directory);

    void ProcessDynamicDetectors();
    void UpdateDeviceList();
    void DeviceListChanged();
    void DetectionProgressChanged();
    void I2CBusListChanged();

    void Cleanup();

    void DetectDevices();

    void DisableDetection();

    void StopDeviceDetection();

    void WaitForDeviceDetection();

private:
    void DetectDevicesThreadFunction();
    void UpdateDetectorSettings();
    void SetupConfigurationDirectory();

    /*-------------------------------------------------------------------------------------*\
    | Static pointer to shared instance of ResourceManager                                  |
    \*-------------------------------------------------------------------------------------*/
    static ResourceManager*                     instance;

    /*-------------------------------------------------------------------------------------*\
    | Detection enabled flag                                                                |
    \*-------------------------------------------------------------------------------------*/
    bool                                        detection_enabled;
    
    /*-------------------------------------------------------------------------------------*\
    | Profile Manager                                                                       |
    \*-------------------------------------------------------------------------------------*/
    ProfileManager*                             profile_manager;

    /*-------------------------------------------------------------------------------------*\
    | Settings Manager                                                                      |
    \*-------------------------------------------------------------------------------------*/
    SettingsManager*                            settings_manager;

    /*-------------------------------------------------------------------------------------*\
    | I2C/SMBus Interfaces                                                                  |
    \*-------------------------------------------------------------------------------------*/
    std::vector<i2c_smbus_interface*>           busses;

    /*-------------------------------------------------------------------------------------*\
    | RGBControllers                                                                        |
    \*-------------------------------------------------------------------------------------*/
    std::vector<RGBController*>                 rgb_controllers_sizes;
    std::vector<RGBController*>                 rgb_controllers_hw;
    std::vector<RGBController*>                 rgb_controllers;

    /*-------------------------------------------------------------------------------------*\
    | Network Server                                                                        |
    \*-------------------------------------------------------------------------------------*/
    NetworkServer*                              server;

    /*-------------------------------------------------------------------------------------*\
    | Network Clients                                                                       |
    \*-------------------------------------------------------------------------------------*/
    std::vector<NetworkClient*>                 clients;

    /*-------------------------------------------------------------------------------------*\
    | Detectors                                                                             |
    \*-------------------------------------------------------------------------------------*/
    std::vector<DeviceDetectorFunction>         device_detectors;
    std::vector<std::string>                    device_detector_strings;
    std::vector<I2CBusDetectorFunction>         i2c_bus_detectors;
    std::vector<I2CDeviceDetectorFunction>      i2c_device_detectors;
    std::vector<std::string>                    i2c_device_detector_strings;
    std::vector<HIDDeviceDetectorBlock>         hid_device_detectors;
    std::vector<std::string>                    hid_device_detector_strings;
    std::vector<DynamicDetectorFunction>        dynamic_detectors;
    std::vector<std::string>                    dynamic_detector_strings;

    bool                                        dynamic_detectors_processed;

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
    std::vector<DeviceListChangeCallback>       DeviceListChangeCallbacks;
    std::vector<void *>                         DeviceListChangeCallbackArgs;

    /*-------------------------------------------------------------------------------------*\
    | Detection Progress, Start, and End Callbacks                                          |
    \*-------------------------------------------------------------------------------------*/
    std::mutex                                  DetectionProgressMutex;
    std::vector<DetectionProgressCallback>      DetectionProgressCallbacks;
    std::vector<void *>                         DetectionProgressCallbackArgs;

    std::vector<DetectionStartCallback>         DetectionStartCallbacks;
    std::vector<void *>                         DetectionStartCallbackArgs;

    std::vector<DetectionEndCallback>           DetectionEndCallbacks;
    std::vector<void *>                         DetectionEndCallbackArgs;

    /*-------------------------------------------------------------------------------------*\
    | I2C/SMBus Adapter List Changed Callback                                               |
    \*-------------------------------------------------------------------------------------*/
    std::mutex                                  I2CBusListChangeMutex;
    std::vector<I2CBusListChangeCallback>       I2CBusListChangeCallbacks;
    std::vector<void *>                         I2CBusListChangeCallbackArgs;

    std::string config_dir;
};
