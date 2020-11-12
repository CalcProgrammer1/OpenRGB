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

typedef std::function<void(std::vector<i2c_smbus_interface*>&)>                                 I2CBusDetectorFunction;
typedef std::function<void(std::vector<RGBController*>&)>                                       DeviceDetectorFunction;
typedef std::function<void(std::vector<i2c_smbus_interface*>&, std::vector<RGBController*>&)>   I2CDeviceDetectorFunction;

enum ControllerList
{
    CONTROLLER_LIST_UI = 0, // Merges all lists
    CONTROLLER_LIST_HW,
    CONTROLLER_LIST_REMOTE
};

typedef void (*DeviceListChangeCallback)(void* receiver, int index, ControllerList list, bool removed);
typedef void (*DetectionProgressCallback)(void* receiver);

typedef struct
{
    DeviceListChangeCallback    callback;
    void*                       receiver;
} DeviceListChangeCallbackBlock;

typedef struct
{
    DetectionProgressCallback   callback;
    void*                       receiver;
 } DetectionProgressCallbackBlock;

class ResourceManager
{
public:
    static ResourceManager *get();
    
    ResourceManager();
    ~ResourceManager();
    
    void RegisterI2CBus(i2c_smbus_interface *);
    std::vector<i2c_smbus_interface*> & GetI2CBusses();
    
    void RegisterRGBController(RGBController*, ControllerList list = CONTROLLER_LIST_HW);
    bool RemoveRGBController(RGBController*, ControllerList list = CONTROLLER_LIST_UI);

    unsigned int GetControllerCount(ControllerList list = CONTROLLER_LIST_UI);
    RGBController* GetController(size_t, ControllerList list = CONTROLLER_LIST_UI);

    int GetUIListIndex(size_t, ControllerList list);
    
    void RegisterI2CBusDetector         (I2CBusDetectorFunction     detector);
    void RegisterDeviceDetector         (std::string name, DeviceDetectorFunction     detector);
    void RegisterI2CDeviceDetector      (std::string name, I2CDeviceDetectorFunction  detector);
    
    void RegisterDeviceListChangeCallback(DeviceListChangeCallback, void * receiver);
    void RegisterDetectionProgressCallback(DetectionProgressCallback callback, void* receiver);
    void UnregisterDeviceListChangeCallback(DeviceListChangeCallback, void* receiver);
    void UnregisterDetectionProgressCallback(DetectionProgressCallback callback, void* receiver);

    unsigned int GetDetectionPercent();
    const char*  GetDetectionString();

    std::string                     GetConfigurationDirectory();

    void                            RegisterClient(NetworkClient*);
    unsigned int                    GetClientCount();
    NetworkClient*                  GetClient(size_t id);
    std::vector<NetworkClient*>&    GetClients(); // TODO: hide the vector; do NOT push in here! Use RegisterClient instead
    NetworkServer*                  GetServer();

    ProfileManager*                 GetProfileManager();
    SettingsManager*                GetSettingsManager();

    void DeviceListChanged(int id, ControllerList list, bool removed);
    void DetectionProgressChanged();

    void Cleanup();

    void DetectDevices();

    void DetectDevicesThreadFunction();

    void StopDeviceDetection();

    void WaitForDeviceDetection();

private:
    static std::unique_ptr<ResourceManager>     instance;

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
    std::vector<RGBController*>                 rgb_controllers_hw_sizes;
    std::vector<bool>                           rgb_controllers_hw_sizes_used;
    std::vector<RGBController*>                 rgb_controllers_hw;
    std::vector<RGBController*>                 rgb_controllers_remote;

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
    std::vector<DeviceListChangeCallbackBlock>  DeviceListChangeCallbacks;

    /*-------------------------------------------------------------------------------------*\
    | Detection Progress Callback                                                           |
    \*-------------------------------------------------------------------------------------*/
    std::mutex                                  DetectionProgressMutex;
    std::vector<DetectionProgressCallbackBlock> DetectionProgressCallbacks;
};
