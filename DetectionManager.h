/*---------------------------------------------------------*\
| DetectionManager.h                                        |
|                                                           |
|   OpenRGB Detection Manager handles detecting supported   |
|   devices and initializing their RGBControllers           |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      08 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <nlohmann/json.hpp>
#include "hidapi_wrapper.h"
#include "i2c_smbus.h"
#include "RGBController.h"
#include "SPDWrapper.h"

using json = nlohmann::json;

/*---------------------------------------------------------*\
| Windows defines "interface" in its headers, stop it       |
\*---------------------------------------------------------*/
#undef interface

/*---------------------------------------------------------*\
| HID Default Values                                        |
\*---------------------------------------------------------*/
#define HID_INTERFACE_ANY               -1
#define HID_USAGE_ANY                   -1
#define HID_USAGE_PAGE_ANY              -1

/*---------------------------------------------------------*\
| Detector Function Types                                   |
\*---------------------------------------------------------*/
typedef std::vector<RGBController*> DetectedControllers;

typedef std::function<bool()>                                                                                       I2CBusDetectorFunction;
typedef std::function<DetectedControllers()>                                                                        DeviceDetectorFunction;
typedef std::function<DetectedControllers(std::vector<i2c_smbus_interface*>&)>                                      I2CDeviceDetectorFunction;
typedef std::function<DetectedControllers(i2c_smbus_interface*, std::vector<SPDWrapper*>&, const std::string&)>     I2CDRAMDeviceDetectorFunction;
typedef std::function<DetectedControllers(i2c_smbus_interface*, uint8_t, const std::string&)>                       I2CPCIDeviceDetectorFunction;
typedef std::function<DetectedControllers(hid_device_info*, const std::string&)>                                    HIDDeviceDetectorFunction;
typedef std::function<DetectedControllers(hidapi_wrapper wrapper, hid_device_info*, const std::string&)>            HIDWrappedDeviceDetectorFunction;
typedef std::function<void()>                                                                                       DynamicDetectorFunction;
typedef std::function<void()>                                                                                       PreDetectionHookFunction;

/*---------------------------------------------------------*\
| Detector Block Types                                      |
\*---------------------------------------------------------*/
class BasicHIDBlock
{
public:
    std::string                         name;
    uint16_t                            vid;
    uint16_t                            pid;
    int                                 interface;
    int                                 usage_page;
    int                                 usage;

    bool compare(hid_device_info* info);
};

class HIDDeviceDetectorBlock : public BasicHIDBlock
{
public:
    HIDDeviceDetectorFunction           function;
};

class HIDWrappedDeviceDetectorBlock : public BasicHIDBlock
{
public:
    HIDWrappedDeviceDetectorFunction    function;
};

typedef struct
{
    std::string                         name;
    I2CPCIDeviceDetectorFunction        function;
    uint16_t                            ven_id;
    uint16_t                            dev_id;
    uint16_t                            subven_id;
    uint16_t                            subdev_id;
    uint8_t                             i2c_addr;
} I2CPCIDeviceDetectorBlock;

typedef struct
{
    std::string                         name;
    I2CDRAMDeviceDetectorFunction       function;
    uint16_t                            jedec_id;
    uint8_t                             dram_type;
} I2CDRAMDeviceDetectorBlock;

/*---------------------------------------------------------*\
| Detection Callback Type                                   |
\*---------------------------------------------------------*/
typedef void (*DetectionCallback)(void *, unsigned int);

/*---------------------------------------------------------*\
| Update Reason Codes                                       |
\*---------------------------------------------------------*/
enum
{
    DETECTIONMANAGER_UPDATE_REASON_I2C_BUS_REGISTERED,          /* RegisterI2CBus() called          */
    DETECTIONMANAGER_UPDATE_REASON_RGBCONTROLLER_REGISTERED,    /* RegisterRGBController() called   */
    DETECTIONMANAGER_UPDATE_REASON_RGBCONTROLLER_UNREGISTERED,  /* UnregisterRGBController() called */
    DETECTIONMANAGER_UPDATE_REASON_RGBCONTROLLER_LIST_CLEARED,  /* RGBController list cleared       */
    DETECTIONMANAGER_UPDATE_REASON_DETECTION_STARTED,           /* Detection started                */
    DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED,  /* Detection progress changed       */
    DETECTIONMANAGER_UPDATE_REASON_DETECTION_COMPLETE,          /* Detection completed              */
};

/*---------------------------------------------------------*\
| DetectionManager class                                    |
\*---------------------------------------------------------*/
class DetectionManager
{
public:
    DetectionManager();
    ~DetectionManager();

    /*-----------------------------------------------------*\
    | DetectionManager Global Instance Accessor             |
    \*-----------------------------------------------------*/
    static DetectionManager*            get();

    /*-----------------------------------------------------*\
    | Functions to access device lists                      |
    \*-----------------------------------------------------*/
    std::vector<i2c_smbus_interface*>&  GetI2CBuses();
    std::vector<RGBController*>&        GetRGBControllers();

    /*-----------------------------------------------------*\
    | I2C Bus Detector Registration Function                |
    \*-----------------------------------------------------*/
    void                                RegisterI2CBusDetector(I2CBusDetectorFunction detector);

    /*-----------------------------------------------------*\
    | RGBController Detector Registration Functions         |
    \*-----------------------------------------------------*/
    void                                RegisterDeviceDetector(std::string name, DeviceDetectorFunction detector);
    void                                RegisterDynamicDetector(std::string name, DynamicDetectorFunction detector);
    void                                RegisterHIDDeviceDetector(std::string name, HIDDeviceDetectorFunction  detector, uint16_t vid, uint16_t pid, int interface = HID_INTERFACE_ANY, int usage_page = HID_USAGE_PAGE_ANY, int usage = HID_USAGE_ANY);
    void                                RegisterHIDWrappedDeviceDetector(std::string name, HIDWrappedDeviceDetectorFunction  detector, uint16_t vid, uint16_t pid, int interface = HID_INTERFACE_ANY, int usage_page = HID_USAGE_PAGE_ANY, int usage = HID_USAGE_ANY);
    void                                RegisterI2CDeviceDetector(std::string name, I2CDeviceDetectorFunction  detector);
    void                                RegisterI2CDRAMDeviceDetector(std::string name, I2CDRAMDeviceDetectorFunction detector, uint16_t jedec_id, uint8_t dram_type);
    void                                RegisterI2CPCIDeviceDetector(std::string name, I2CPCIDeviceDetectorFunction detector, uint16_t ven_id, uint16_t dev_id, uint16_t subven_id, uint16_t subdev_id, uint8_t i2c_addr);

    /*-----------------------------------------------------*\
    | Pre-Detection Hook Function Registration Function     |
    \*-----------------------------------------------------*/
    void                                RegisterPreDetectionHook(PreDetectionHookFunction hook);

    /*-----------------------------------------------------*\
    | Detection Callback Registration Functions             |
    \*-----------------------------------------------------*/
    void                                RegisterDetectionCallback(DetectionCallback new_callback, void * new_callback_arg);
    void                                UnregisterDetectionCallback(DetectionCallback callback, void *callback_arg);

    /*-----------------------------------------------------*\
    | Functions for registering I2C buses from within       |
    | detectors                                             |
    \*-----------------------------------------------------*/
    void                                RegisterI2CBus(i2c_smbus_interface *bus);

    /*-----------------------------------------------------*\
    | Functions for registering RGBControllers from within  |
    | detectors                                             |
    \*-----------------------------------------------------*/
    void                                RegisterRGBController(RGBController *rgb_controller);
    void                                UnregisterRGBController(RGBController *rgb_controller);

    /*-----------------------------------------------------*\
    | Detection state functions                             |
    \*-----------------------------------------------------*/
    void                                AbortDetection();
    void                                BeginDetection();
    unsigned int                        GetDetectionPercent();
    std::string                         GetDetectionString();
    void                                WaitForDetection();

private:
    /*-----------------------------------------------------*\
    | Static pointer to shared instance of DetectionManager |
    \*-----------------------------------------------------*/
    static DetectionManager*                    instance;

    /*-----------------------------------------------------*\
    | List of I2C buses managed by DetectorManager          |
    \*-----------------------------------------------------*/
    std::vector<i2c_smbus_interface*>           i2c_buses;

    /*-----------------------------------------------------*\
    | List of RGBControllers managed by DetectorManager     |
    \*-----------------------------------------------------*/
    std::vector<RGBController*>                 rgb_controllers;

    /*-----------------------------------------------------*\
    | List of manually configured RGBController information |
    | from stored sizes profile                             |
    \*-----------------------------------------------------*/
    std::vector<RGBController*>                 rgb_controllers_sizes;

    /*-----------------------------------------------------*\
    | Detectors                                             |
    \*-----------------------------------------------------*/
    std::vector<DeviceDetectorFunction>         device_detectors;
    std::vector<std::string>                    device_detector_strings;
    std::vector<I2CBusDetectorFunction>         i2c_bus_detectors;
    std::vector<I2CDeviceDetectorFunction>      i2c_device_detectors;
    std::vector<std::string>                    i2c_device_detector_strings;
    std::vector<I2CDRAMDeviceDetectorBlock>     i2c_dram_device_detectors;
    std::vector<I2CPCIDeviceDetectorBlock>      i2c_pci_device_detectors;
    std::vector<HIDDeviceDetectorBlock>         hid_device_detectors;
    std::vector<HIDWrappedDeviceDetectorBlock>  hid_wrapped_device_detectors;
    std::vector<DynamicDetectorFunction>        dynamic_detectors;
    std::vector<std::string>                    dynamic_detector_strings;
    std::vector<PreDetectionHookFunction>       pre_detection_hooks;

    /*-----------------------------------------------------*\
    | Detection Callbacks                                   |
    \*-----------------------------------------------------*/
    std::vector<DetectionCallback>              DetectionCallbacks;
    std::vector<void *>                         DetectionCallbackArgs;
    std::mutex                                  DetectionCallbackMutex;

    /*-----------------------------------------------------*\
    | Detection State Variables                             |
    \*-----------------------------------------------------*/
    std::mutex                                  DetectDevicesMutex;
    std::atomic<unsigned int>                   detection_percent;
    unsigned int                                detection_percent_hid_count;
    unsigned int                                detection_percent_i2c_count;
    unsigned int                                detection_percent_i2c_dram_count;
    unsigned int                                detection_percent_i2c_pci_count;
    unsigned int                                detection_percent_other_count;
    unsigned int                                detection_percent_denominator;
    std::vector<bool>                           detection_size_entry_used;
    std::string                                 detection_string;
    bool                                        dynamic_detectors_processed;
    bool                                        initial_detection;
    bool                                        i2c_interface_fail;

    /*-----------------------------------------------------*\
    | Background Thread Variables                           |
    \*-----------------------------------------------------*/
    std::thread *                               BackgroundThread;
    std::function<void()>                       BackgroundThreadScheduledFunction;
    std::condition_variable                     BackgroundThreadScheduledFunctionStart;
    std::mutex                                  BackgroundThreadStateMutex;

    std::atomic<bool>                           background_thread_running;
    std::atomic<bool>                           detection_in_progress;

    /*-----------------------------------------------------*\
    | Background Thread Functions                           |
    \*-----------------------------------------------------*/
    void BackgroundThreadFunction();
    void RunInBackgroundThread(std::function<void()>);

    /*-----------------------------------------------------*\
    | Functions that must be run in the background thread   |
    \*-----------------------------------------------------*/
    void BackgroundDetectDevices();
    void BackgroundDetectHIDDevices(hid_device_info* hid_devices, json detector_settings);
    void BackgroundDetectHIDDevicesSafe(json detector_settings);
#ifdef __linux__
#ifdef __GLIBC__
    void BackgroundDetectHIDDevicesWrapped(hid_device_info* hid_devices, json detector_settings);
#endif
#endif
    void BackgroundDetectI2CBuses();
    void BackgroundDetectI2CDevices(json detector_settings);
    void BackgroundDetectI2CDRAMDevices(json detector_settings);
    void BackgroundDetectI2CPCIDevices(json detector_settings);
    void BackgroundDetectOtherDevices(json detector_settings);
    void BackgroundHIDExit();
    void BackgroundHIDInit();

    /*-----------------------------------------------------*\
    | Functions to run detectors                            |
    \*-----------------------------------------------------*/
    void RunHIDDetector(hid_device_info* current_hid_device, json detector_settings);
    void RunHIDWrappedDetector(const hidapi_wrapper* wrapper, hid_device_info* current_hid_device, json detector_settings);

    /*-----------------------------------------------------*\
    | Detection processing functions                        |
    \*-----------------------------------------------------*/
    void ProcessCleanup();
    void ProcessDynamicDetectors();
    bool ProcessPreDetection();
    void ProcessPreDetectionHooks();
    void UpdateDetectorSettings();

    /*-----------------------------------------------------*\
    | Function for signalling DetectionManager updates to   |
    | registered callbacks                                  |
    \*-----------------------------------------------------*/
    void SignalUpdate(unsigned int update_reason);

    bool IsAnyDimmDetectorEnabled(json &detector_settings);
};

/*---------------------------------------------------------*\
| Detector Types                                            |
\*---------------------------------------------------------*/
class DeviceDetector
{
public:
    DeviceDetector(std::string name, DeviceDetectorFunction detector)
	{
        DetectionManager::get()->RegisterDeviceDetector(name, detector);
	}
};

class I2CDeviceDetector
{
public:
    I2CDeviceDetector(std::string name, I2CDeviceDetectorFunction detector)
	{
        DetectionManager::get()->RegisterI2CDeviceDetector(name, detector);
	}
};

class I2CDRAMDeviceDetector
{
public:
    I2CDRAMDeviceDetector(std::string name, I2CDRAMDeviceDetectorFunction detector, uint16_t jedec_id, uint8_t dram_type)
	{
        DetectionManager::get()->RegisterI2CDRAMDeviceDetector(name, detector, jedec_id, dram_type);
	}
};

class I2CPCIDeviceDetector
{
public:
    I2CPCIDeviceDetector(std::string name, I2CPCIDeviceDetectorFunction detector, uint16_t ven_id, uint16_t dev_id, uint16_t subven_id, uint16_t subdev_id, uint8_t i2c_addr)
    {
        DetectionManager::get()->RegisterI2CPCIDeviceDetector(name, detector, ven_id, dev_id, subven_id, subdev_id, i2c_addr);
    }
};

class I2CBusDetector
{
public:
    I2CBusDetector(I2CBusDetectorFunction detector)
    {
        DetectionManager::get()->RegisterI2CBusDetector(detector);
    }
};

class HIDDeviceDetector
{
public:
    HIDDeviceDetector(std::string name, HIDDeviceDetectorFunction detector, uint16_t vid, uint16_t pid, int interface, int usage_page, int usage)
    {
        DetectionManager::get()->RegisterHIDDeviceDetector(name, detector, vid, pid, interface, usage_page, usage);
    }
};

class HIDWrappedDeviceDetector
{
public:
    HIDWrappedDeviceDetector(std::string name, HIDWrappedDeviceDetectorFunction detector, uint16_t vid, uint16_t pid, int interface, int usage_page, int usage)
    {
        DetectionManager::get()->RegisterHIDWrappedDeviceDetector(name, detector, vid, pid, interface, usage_page, usage);
    }
};

class DynamicDetector
{
public:
    DynamicDetector(std::string name, DynamicDetectorFunction detector)
    {
        DetectionManager::get()->RegisterDynamicDetector(name, detector);
    }
};

class PreDetectionHook
{
public:
    PreDetectionHook(PreDetectionHookFunction hook)
    {
        DetectionManager::get()->RegisterPreDetectionHook(hook);
    }
};

/*---------------------------------------------------------*\
| Detector Registration Macros                              |
\*---------------------------------------------------------*/
#define REGISTER_DETECTOR(name, func)                                                   static DeviceDetector           device_detector_obj_##func(name, func)
#define REGISTER_I2C_DETECTOR(name, func)                                               static I2CDeviceDetector        device_detector_obj_##func(name, func)
#define REGISTER_I2C_DRAM_DETECTOR(name, func, jedec_id, dram_type)                     static I2CDRAMDeviceDetector    device_detector_obj_##func##jedec_id(name, func, jedec_id, dram_type)
#define REGISTER_I2C_PCI_DETECTOR(name, func, ven, dev, subven, subdev, addr)           static I2CPCIDeviceDetector     device_detector_obj_##ven##dev##subven##subdev##addr##func(name, func, ven, dev, subven, subdev, addr)
#define REGISTER_I2C_BUS_DETECTOR(func)                                                 static I2CBusDetector           device_detector_obj_##func(func)
#define REGISTER_HID_DETECTOR(name, func, vid, pid)                                     static HIDDeviceDetector        device_detector_obj_##vid##pid(name, func, vid, pid, HID_INTERFACE_ANY, HID_USAGE_PAGE_ANY, HID_USAGE_ANY)
#define REGISTER_HID_DETECTOR_I(name, func, vid, pid, interface)                        static HIDDeviceDetector        device_detector_obj_##vid##pid##_##interface(name, func, vid, pid, interface, HID_USAGE_PAGE_ANY, HID_USAGE_ANY)
#define REGISTER_HID_DETECTOR_IP(name, func, vid, pid, interface, page)                 static HIDDeviceDetector        device_detector_obj_##vid##pid##_##interface##_##page(name, func, vid, pid, interface, page, HID_USAGE_ANY)
#define REGISTER_HID_DETECTOR_IPU(name, func, vid, pid, interface, page, usage)         static HIDDeviceDetector        device_detector_obj_##vid##pid##_##interface##_##page##_##usage(name, func, vid, pid, interface, page, usage)
#define REGISTER_HID_DETECTOR_P(name, func, vid, pid, page)                             static HIDDeviceDetector        device_detector_obj_##vid##pid##__##page(name, func, vid, pid, HID_INTERFACE_ANY, page, HID_USAGE_ANY)
#define REGISTER_HID_DETECTOR_PU(name, func, vid, pid, page, usage)                     static HIDDeviceDetector        device_detector_obj_##vid##pid##__##page##_##usage(name, func, vid, pid, HID_INTERFACE_ANY, page, usage)
#define REGISTER_HID_WRAPPED_DETECTOR(name, func, vid, pid)                             static HIDWrappedDeviceDetector device_detector_obj_##vid##pid(name, func, vid, pid, HID_INTERFACE_ANY, HID_USAGE_PAGE_ANY, HID_USAGE_ANY)
#define REGISTER_HID_WRAPPED_DETECTOR_I(name, func, vid, pid, interface)                static HIDWrappedDeviceDetector device_detector_obj_##vid##pid##_##interface(name, func, vid, pid, interface, HID_USAGE_PAGE_ANY, HID_USAGE_ANY)
#define REGISTER_HID_WRAPPED_DETECTOR_IPU(name, func, vid, pid, interface, page, usage) static HIDWrappedDeviceDetector device_detector_obj_##vid##pid##_##interface##_##page##_##usage(name, func, vid, pid, interface, page, usage)
#define REGISTER_HID_WRAPPED_DETECTOR_PU(name, func, vid, pid, page, usage)             static HIDWrappedDeviceDetector device_detector_obj_##vid##pid##__##page##_##usage(name, func, vid, pid, HID_INTERFACE_ANY, page, usage)
#define REGISTER_DYNAMIC_DETECTOR(name, func)                                           static DynamicDetector          device_detector_obj_##func(name, func)
#define REGISTER_PRE_DETECTION_HOOK(func)                                               static PreDetectionHook         device_detector_obj_##func(func)

#define REGISTER_DYNAMIC_I2C_DETECTOR(name, func)                                       I2CDeviceDetector               device_detector_obj_##func(name, func)
#define REGISTER_DYNAMIC_I2C_DRAM_DETECTOR(name, func, jedec_id, dram_type)             I2CDRAMDeviceDetector           device_detector_obj_##func(name, func, jedec_id, dram_type)
#define REGISTER_DYNAMIC_I2C_PCI_DETECTOR(name, func, ven, dev, subven, subdev, addr)   I2CPCIDeviceDetector            device_detector_obj_##ven##dev##subven##subdev##addr##func(name, func, ven, dev, subven, subdev, addr)
#define REGISTER_DYNAMIC_I2C_BUS_DETECTOR(func)                                         I2CBusDetector                  device_detector_obj_##func(func)
#define REGISTER_DYNAMIC_HID_DETECTOR(name, func, vid, pid)                             HIDDeviceDetector               device_detector_obj_##vid##pid(name, func, vid, pid, HID_INTERFACE_ANY, HID_USAGE_PAGE_ANY, HID_USAGE_ANY)
#define REGISTER_DYNAMIC_HID_DETECTOR_I(name, func, vid, pid, interface)                HIDDeviceDetector               device_detector_obj_##vid##pid##_##interface(name, func, vid, pid, interface, HID_USAGE_PAGE_ANY, HID_USAGE_ANY)
#define REGISTER_DYNAMIC_HID_DETECTOR_IP(name, func, vid, pid, interface, page)         HIDDeviceDetector               device_detector_obj_##vid##pid##_##interface##_##page(name, func, vid, pid, interface, page, HID_USAGE_ANY)
#define REGISTER_DYNAMIC_HID_DETECTOR_IPU(name, func, vid, pid, interface, page, usage) HIDDeviceDetector               device_detector_obj_##vid##pid##_##interface##_##page##_##usage(name, func, vid, pid, interface, page, usage)
#define REGISTER_DYNAMIC_HID_DETECTOR_P(name, func, vid, pid, page)                     HIDDeviceDetector               device_detector_obj_##vid##pid##__##page(name, func, vid, pid, HID_INTERFACE_ANY, page, HID_USAGE_ANY)
#define REGISTER_DYNAMIC_HID_DETECTOR_PU(name, func, vid, pid, page, usage)             HIDDeviceDetector               device_detector_obj_##vid##pid##__##page##_##usage(name, func, vid, pid, HID_INTERFACE_ANY, page, usage)
