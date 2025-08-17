/*---------------------------------------------------------*\
| ResourceManager.h                                         |
|                                                           |
|   OpenRGB Resource Manager controls access to application |
|   components including RGBControllers, I2C interfaces,    |
|   and network SDK components                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                27 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <thread>
#include <string>
#include <vector>
#include "SPDWrapper.h"
#include "hidapi_wrapper.h"
#include "i2c_smbus.h"
#include "ResourceManagerInterface.h"
#include "filesystem.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define HID_INTERFACE_ANY   -1
#define HID_USAGE_ANY       -1
#define HID_USAGE_PAGE_ANY  -1

struct hid_device_info;
class NetworkClient;
class NetworkServer;
class ProfileManager;
class RGBController;
class SettingsManager;

typedef std::function<bool()>                                                                       I2CBusDetectorFunction;
typedef std::function<void()>                                                                       DeviceDetectorFunction;
typedef std::function<void(std::vector<i2c_smbus_interface*>&)>                                     I2CDeviceDetectorFunction;
typedef std::function<void(i2c_smbus_interface*, std::vector<SPDWrapper*>&, const std::string&)>    I2CDIMMDeviceDetectorFunction;
typedef std::function<void(i2c_smbus_interface*, uint8_t, const std::string&)>                      I2CPCIDeviceDetectorFunction;
typedef std::function<void(hid_device_info*, const std::string&)>                                   HIDDeviceDetectorFunction;
typedef std::function<void(hidapi_wrapper wrapper, hid_device_info*, const std::string&)>           HIDWrappedDeviceDetectorFunction;
typedef std::function<void()>                                                                       DynamicDetectorFunction;
typedef std::function<void()>                                                                       PreDetectionHookFunction;

class BasicHIDBlock
{
public:
    std::string  name;
    uint16_t     vid;
    uint16_t     pid;
    int          interface;
    int          usage_page;
    int          usage;

    bool compare(hid_device_info* info);
};

class HIDDeviceDetectorBlock : public BasicHIDBlock
{
public:
    HIDDeviceDetectorFunction   function;
};

class HIDWrappedDeviceDetectorBlock : public BasicHIDBlock
{
public:
    HIDWrappedDeviceDetectorFunction    function;
};

typedef struct
{
    std::string                     name;
    I2CPCIDeviceDetectorFunction    function;
    uint16_t                        ven_id;
    uint16_t                        dev_id;
    uint16_t                        subven_id;
    uint16_t                        subdev_id;
    uint8_t                         i2c_addr;
} I2CPCIDeviceDetectorBlock;

typedef struct
{
    std::string                     name;
    I2CDIMMDeviceDetectorFunction   function;
    uint16_t                        jedec_id;
    uint8_t                         dimm_type;
} I2CDIMMDeviceDetectorBlock;

/*---------------------------------------------------------*\
| Define a macro for QT lupdate to parse                    |
\*---------------------------------------------------------*/
#define QT_TRANSLATE_NOOP(scope, x) x

extern const char* I2C_ERR_WIN;
extern const char* I2C_ERR_LINUX;
extern const char* UDEV_MISSING;
extern const char* UDEV_MULTI;

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
    void RegisterI2CDIMMDeviceDetector  (std::string name, I2CDIMMDeviceDetectorFunction detector, uint16_t jedec_id, uint8_t dimm_type);
    void RegisterI2CPCIDeviceDetector   (std::string name, I2CPCIDeviceDetectorFunction detector, uint16_t ven_id, uint16_t dev_id, uint16_t subven_id, uint16_t subdev_id, uint8_t i2c_addr);
    void RegisterHIDDeviceDetector      (std::string name,
                                         HIDDeviceDetectorFunction  detector,
                                         uint16_t vid,
                                         uint16_t pid,
                                         int interface  = HID_INTERFACE_ANY,
                                         int usage_page = HID_USAGE_PAGE_ANY,
                                         int usage      = HID_USAGE_ANY);
    void RegisterHIDWrappedDeviceDetector   (std::string name,
                                            HIDWrappedDeviceDetectorFunction  detector,
                                            uint16_t vid,
                                            uint16_t pid,
                                            int interface  = HID_INTERFACE_ANY,
                                            int usage_page = HID_USAGE_PAGE_ANY,
                                            int usage      = HID_USAGE_ANY);
    void RegisterDynamicDetector        (std::string name, DynamicDetectorFunction detector);
    void RegisterPreDetectionHook       (PreDetectionHookFunction hook);

    void RegisterClientInfoChangeCallback(ClientInfoChangeCallback new_callback, void * new_callback_arg);
    void RegisterDeviceListChangeCallback(DeviceListChangeCallback new_callback, void * new_callback_arg);
    void RegisterDetectionProgressCallback(DetectionProgressCallback new_callback, void * new_callback_arg);
    void RegisterDetectionStartCallback(DetectionStartCallback new_callback, void * new_callback_arg);
    void RegisterDetectionEndCallback(DetectionEndCallback new_callback, void * new_callback_arg);
    void RegisterI2CBusListChangeCallback(I2CBusListChangeCallback new_callback, void * new_callback_arg);

    void UnregisterClientInfoChangeCallback(ClientInfoChangeCallback new_callback, void * new_callback_arg);
    void UnregisterDeviceListChangeCallback(DeviceListChangeCallback callback, void * callback_arg);
    void UnregisterDetectionProgressCallback(DetectionProgressCallback callback, void *callback_arg);
    void UnregisterDetectionStartCallback(DetectionStartCallback callback, void *callback_arg);
    void UnregisterDetectionEndCallback(DetectionEndCallback callback, void *callback_arg);
    void UnregisterI2CBusListChangeCallback(I2CBusListChangeCallback callback, void * callback_arg);

    bool         GetDetectionEnabled();
    unsigned int GetDetectionPercent();
    const char*  GetDetectionString();

    filesystem::path                GetConfigurationDirectory();

    void RegisterNetworkClient(NetworkClient* new_client);
    void UnregisterNetworkClient(NetworkClient* network_client);

    std::vector<NetworkClient*>&    GetClients();
    NetworkServer*                  GetServer();

    ProfileManager*                 GetProfileManager();
    SettingsManager*                GetSettingsManager();

    void                            SetConfigurationDirectory(const filesystem::path &directory);

    void ProcessPreDetectionHooks(); // Consider making private
    void ProcessDynamicDetectors();  // Consider making private
    void UpdateDeviceList();
    void ClientInfoChanged();
    void DeviceListChanged();
    void DetectionProgressChanged();
    void I2CBusListChanged();

    void Initialize(bool tryConnect, bool detectDevices, bool startServer, bool applyPostOptions);

    void Cleanup();

    void DetectDevices();

    void DisableDetection();

    void RescanDevices();

    void StopDeviceDetection();

    void WaitForInitialization();
    void WaitForDeviceDetection();

private:
    void UpdateDetectorSettings();
    void SetupConfigurationDirectory();
    bool AttemptLocalConnection();
    bool ProcessPreDetection();
    void ProcessPostDetection();
    bool IsAnyDimmDetectorEnabled(json &detector_settings);
    void RunInBackgroundThread(std::function<void()>);
    void BackgroundThreadFunction();

    /*-----------------------------------------------------*\
    | Functions that must be run in the background thread   |
    | These are not related to STL coroutines, yet this     |
    | name is the most convenient                           |
    \*-----------------------------------------------------*/
    void InitCoroutine();
    void DetectDevicesCoroutine();
    void HidExitCoroutine();

    /*-----------------------------------------------------*\
    | Static pointer to shared instance of ResourceManager  |
    \*-----------------------------------------------------*/
    static ResourceManager*                     instance;

    /*-----------------------------------------------------*\
    | Auto connection permitting flag                       |
    \*-----------------------------------------------------*/
    bool                                        tryAutoConnect;

    /*-----------------------------------------------------*\
    | Detection enabled flag                                |
    \*-----------------------------------------------------*/
    bool                                        detection_enabled;

    /*-----------------------------------------------------*\
    | Auto connection active flag                           |
    \*-----------------------------------------------------*/
    bool                                        auto_connection_active;

    /*-----------------------------------------------------*\
    | Auto connection client pointer                        |
    \*-----------------------------------------------------*/
    NetworkClient *                             auto_connection_client;

    /*-----------------------------------------------------*\
    | Auto connection permitting flag                       |
    \*-----------------------------------------------------*/
    bool                                        start_server;

    /*-----------------------------------------------------*\
    | Auto connection permitting flag                       |
    \*-----------------------------------------------------*/
    bool                                        apply_post_options;

    /*-----------------------------------------------------*\
    | Initialization completion flag                        |
    \*-----------------------------------------------------*/
    std::atomic<bool>                           init_finished;

    /*-----------------------------------------------------*\
    | Profile Manager                                       |
    \*-----------------------------------------------------*/
    ProfileManager*                             profile_manager;

    /*-----------------------------------------------------*\
    | Settings Manager                                      |
    \*-----------------------------------------------------*/
    SettingsManager*                            settings_manager;

    /*-----------------------------------------------------*\
    | I2C/SMBus Interfaces                                  |
    \*-----------------------------------------------------*/
    std::vector<i2c_smbus_interface*>           busses;

    /*-----------------------------------------------------*\
    | RGBControllers                                        |
    \*-----------------------------------------------------*/
    std::vector<RGBController*>                 rgb_controllers_sizes;
    std::vector<RGBController*>                 rgb_controllers_hw;
    std::vector<RGBController*>                 rgb_controllers;

    /*-----------------------------------------------------*\
    | Network Server                                        |
    \*-----------------------------------------------------*/
    NetworkServer*                              server;

    /*-----------------------------------------------------*\
    | Network Clients                                       |
    \*-----------------------------------------------------*/
    std::vector<NetworkClient*>                 clients;

    /*-----------------------------------------------------*\
    | Detectors                                             |
    \*-----------------------------------------------------*/
    std::vector<DeviceDetectorFunction>         device_detectors;
    std::vector<std::string>                    device_detector_strings;
    std::vector<I2CBusDetectorFunction>         i2c_bus_detectors;
    std::vector<I2CDeviceDetectorFunction>      i2c_device_detectors;
    std::vector<std::string>                    i2c_device_detector_strings;
    std::vector<I2CDIMMDeviceDetectorBlock>     i2c_dimm_device_detectors;
    std::vector<I2CPCIDeviceDetectorBlock>      i2c_pci_device_detectors;
    std::vector<HIDDeviceDetectorBlock>         hid_device_detectors;
    std::vector<HIDWrappedDeviceDetectorBlock>  hid_wrapped_device_detectors;
    std::vector<DynamicDetectorFunction>        dynamic_detectors;
    std::vector<std::string>                    dynamic_detector_strings;
    std::vector<PreDetectionHookFunction>       pre_detection_hooks;

    bool                                        dynamic_detectors_processed;

    /*-----------------------------------------------------*\
    | Detection Thread and Detection State                  |
    \*-----------------------------------------------------*/
    std::thread *                               DetectDevicesThread;
    std::mutex                                  DetectDeviceMutex;
    std::function<void()>                       ScheduledBackgroundFunction;
    std::mutex                                  BackgroundThreadStateMutex;

    /*-----------------------------------------------------*\
    | NOTE: wakes up the background detection thread        |
    \*-----------------------------------------------------*/
    std::condition_variable                     BackgroundFunctionStartTrigger;

    std::atomic<bool>                           background_thread_running;
    std::atomic<bool>                           detection_is_required;
    std::atomic<unsigned int>                   detection_percent;
    std::atomic<unsigned int>                   detection_prev_size;
    std::vector<bool>                           detection_size_entry_used;
    const char*                                 detection_string;

    /*-----------------------------------------------------*\
    | Client Info Changed Callback                          |
    \*-----------------------------------------------------*/
    std::vector<ClientInfoChangeCallback>       ClientInfoChangeCallbacks;
    std::vector<void *>                         ClientInfoChangeCallbackArgs;

    /*-----------------------------------------------------*\
    | Device List Changed Callback                          |
    \*-----------------------------------------------------*/
    std::mutex                                  DeviceListChangeMutex;
    std::vector<DeviceListChangeCallback>       DeviceListChangeCallbacks;
    std::vector<void *>                         DeviceListChangeCallbackArgs;

    /*-----------------------------------------------------*\
    | Detection Progress, Start, and End Callbacks          |
    \*-----------------------------------------------------*/
    std::mutex                                  DetectionProgressMutex;
    std::vector<DetectionProgressCallback>      DetectionProgressCallbacks;
    std::vector<void *>                         DetectionProgressCallbackArgs;

    std::vector<DetectionStartCallback>         DetectionStartCallbacks;
    std::vector<void *>                         DetectionStartCallbackArgs;

    std::vector<DetectionEndCallback>           DetectionEndCallbacks;
    std::vector<void *>                         DetectionEndCallbackArgs;

    /*-----------------------------------------------------*\
    | I2C/SMBus Adapter List Changed Callback               |
    \*-----------------------------------------------------*/
    std::mutex                                  I2CBusListChangeMutex;
    std::vector<I2CBusListChangeCallback>       I2CBusListChangeCallbacks;
    std::vector<void *>                         I2CBusListChangeCallbackArgs;

    /*-----------------------------------------------------*\
    | OpenRGB configuration directory path                  |
    \*-----------------------------------------------------*/
    filesystem::path                            config_dir;
};
