/*---------------------------------------------------------*\
| DetectionManager.cpp                                      |
|                                                           |
|   OpenRGB Detection Manager handles detecting supported   |
|   devices and initializing their RGBControllers           |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      08 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <chrono>
#include "DetectionManager.h"
#include "LogManager.h"
#include "pci_ids.h"
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "StringUtils.h"

#ifdef __linux__
#include <unistd.h>
#endif

using namespace std::chrono_literals;

/*---------------------------------------------------------*\
| DetectionManager name for log entries                     |
\*---------------------------------------------------------*/
const char* DETECTIONMANAGER = "DetectionManager";

/*---------------------------------------------------------*\
| Define a macro for QT lupdate to parse                    |
\*---------------------------------------------------------*/
#define QT_TRANSLATE_NOOP(scope, x) x

/*---------------------------------------------------------*\
| Warning Strings                                           |
\*---------------------------------------------------------*/
const char* I2C_ERR_WIN =   QT_TRANSLATE_NOOP("DetectionManager",
                                              "<h2>Some internal devices may not be detected:</h2>"
                                              "<p>One or more I2C or SMBus interfaces failed to initialize.</p>"
                                              "<p><b>RGB DRAM modules, some motherboards' onboard RGB lighting, and RGB Graphics Cards, will not be available in OpenRGB</b> without I2C or SMBus.</p>"
                                              "<h4>How to fix this:</h4>"
                                              "<p>On Windows, this is usually caused by a failure to load the PawnIO driver.</p>"
                                              "<p>You must first install <a href='https://pawnio.eu/'>PawnIO</a>, then you must OpenRGB as administrator in order to access these devices.</p>"
                                              "<p>See <a href='https://help.openrgb.org/'>help.openrgb.org</a> for additional troubleshooting steps if you keep seeing this message.<br></p>"
                                              "<h3>If you are not using internal RGB on a desktop this message is not important to you.</h3>");
const char* I2C_ERR_LINUX = QT_TRANSLATE_NOOP("DetectionManager",
                                              "<h2>Some internal devices may not be detected:</h2>"
                                              "<p>One or more I2C or SMBus interfaces failed to initialize.</p>"
                                              "<p><b>RGB DRAM modules, some motherboards' onboard RGB lighting, and RGB Graphics Cards, will not be available in OpenRGB</b> without I2C or SMBus.</p>"
                                              "<h4>How to fix this:</h4>"
                                              "<p>On Linux, this is usually because the i2c-dev module is not loaded.</p>"
                                              "<p>You must load the i2c-dev module along with the correct i2c driver for your motherboard. "
                                              "This is usually i2c-piix4 for AMD systems and i2c-i801 for Intel systems.</p>"
                                              "<p>See <a href='https://help.openrgb.org/'>help.openrgb.org</a> for additional troubleshooting steps if you keep seeing this message.<br></p>"
                                              "<h3>If you are not using internal RGB on a desktop this message is not important to you.</h3>");

const char* UDEV_MISSING =  QT_TRANSLATE_NOOP("DetectionManager",
                                              "<h2>WARNING:</h2>"
                                              "<p>The OpenRGB udev rules are not installed.</p>"
                                              "<p>Most devices will not be available unless running OpenRGB as root.</p>"
                                              "<p>If using AppImage, Flatpak, or self-compiled versions of OpenRGB you must install the udev rules manually</p>"
                                              "<p>See <a href='https://openrgb.org/udev'>https://openrgb.org/udev</a> to install the udev rules manually</p>");
const char* UDEV_MUTLI =    QT_TRANSLATE_NOOP("DetectionManager",
                                              "<h2>WARNING:</h2>"
                                              "<p>Multiple OpenRGB udev rules are installed.</p>"
                                              "<p>The udev rules file 60-openrgb.rules is installed in both /etc/udev/rules.d and /usr/lib/udev/rules.d.</p>"
                                              "<p>Multiple udev rules files can conflict, it is recommended to remove one of them.</p>");

/*---------------------------------------------------------*\
| Default hidapi wrappter that just uses default hidapi     |
\*---------------------------------------------------------*/
const hidapi_wrapper default_hidapi_wrapper =
{
    NULL,
    (hidapi_wrapper_send_feature_report)        hid_send_feature_report,
    (hidapi_wrapper_get_feature_report)         hid_get_feature_report,
    (hidapi_wrapper_get_serial_number_string)   hid_get_serial_number_string,
    (hidapi_wrapper_open_path)                  hid_open_path,
    (hidapi_wrapper_enumerate)                  hid_enumerate,
    (hidapi_wrapper_free_enumeration)           hid_free_enumeration,
    (hidapi_wrapper_close)                      hid_close,
    (hidapi_wrapper_error)                      hid_error
};

/*---------------------------------------------------------*\
| BasicHIDBlock Implementation                              |
\*---------------------------------------------------------*/
bool BasicHIDBlock::compare(hid_device_info* info)
{
    return((vid == info->vendor_id)
        && (pid == info->product_id)
#ifdef USE_HID_USAGE
        && ((usage_page == HID_USAGE_PAGE_ANY)
         || (usage_page == info->usage_page))
        && ((usage      == HID_USAGE_ANY)
         || (usage      == info->usage))
        && ((interface  == HID_INTERFACE_ANY)
         || (interface  == info->interface_number))
#else
        && ((interface  == HID_INTERFACE_ANY)
         || (interface  == info->interface_number))
#endif
        );
}

/*---------------------------------------------------------*\
| DetectionManager Global Instance Pointer                  |
\*---------------------------------------------------------*/
DetectionManager* DetectionManager::instance;

DetectionManager::DetectionManager()
{
    /*-----------------------------------------------------*\
    | Initialize global instance pointer the when created   |
    | There should only ever be one instance of             |
    | DetectionManager                                      |
    \*-----------------------------------------------------*/
    if(!instance)
    {
        instance = this;
    }
    /*-----------------------------------------------------*\
    | If, for whatever reason, DetectionManager already     |
    | exists, delete this instance as only one should exist |
    \*-----------------------------------------------------*/
    else
    {
        delete this;
        return;
    }

    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    detection_in_progress           = false;
    detection_percent               = 100;
    detection_percent_denominator   = 0;
    detection_string                = "";
    dynamic_detectors_processed     = false;
    initial_detection               = true;

    /*-----------------------------------------------------*\
    | Start the background thread                           |
    \*-----------------------------------------------------*/
    background_thread_running       = true;
    BackgroundThread                = new std::thread(&DetectionManager::BackgroundThreadFunction, this);
}

DetectionManager::~DetectionManager()
{
    /*-----------------------------------------------------*\
    | Exit HID in background thread                         |
    \*-----------------------------------------------------*/
    RunInBackgroundThread(std::bind(&DetectionManager::BackgroundHidExit, this));

    /*-----------------------------------------------------*\
    | Mark the background detection thread as not running   |
    | and then wake it up so it knows that it has to stop   |
    \*-----------------------------------------------------*/
    background_thread_running = false;
    BackgroundThreadScheduledFunctionStart.notify_one();

    /*-----------------------------------------------------*\
    | Stop the background thread                            |
    \*-----------------------------------------------------*/
    if(BackgroundThread)
    {
        BackgroundThread->join();
        delete BackgroundThread;
        BackgroundThread = nullptr;
    }
}

/*---------------------------------------------------------*\
| DetectionManager Global Instance Accessor                 |
\*---------------------------------------------------------*/
DetectionManager* DetectionManager::get()
{
    /*-----------------------------------------------------*\
    | If DetectionManager does not exist yet, create it     |
    \*-----------------------------------------------------*/
    if(!instance)
    {
        instance = new DetectionManager();
    }

    return instance;
}

/*---------------------------------------------------------*\
| Functions to access device lists                          |
\*---------------------------------------------------------*/
std::vector<i2c_smbus_interface*>& DetectionManager::GetI2CBuses()
{
    return(i2c_buses);
}

std::vector<RGBController*>& DetectionManager::GetRGBControllers()
{
    return(rgb_controllers);
}

/*---------------------------------------------------------*\
| I2C Bus Detector Registration Function                    |
\*---------------------------------------------------------*/
void DetectionManager::RegisterI2CBusDetector(I2CBusDetectorFunction detector)
{
    i2c_bus_detectors.push_back(detector);
}

/*---------------------------------------------------------*\
| RGBController Detector Registration Functions             |
\*---------------------------------------------------------*/
void DetectionManager::RegisterDeviceDetector(std::string name, DeviceDetectorFunction detector)
{
    device_detector_strings.push_back(name);
    device_detectors.push_back(detector);
}

void DetectionManager::RegisterDynamicDetector(std::string name, DynamicDetectorFunction detector)
{
    dynamic_detector_strings.push_back(name);
    dynamic_detectors.push_back(detector);
}

void DetectionManager::RegisterHIDDeviceDetector(std::string name, HIDDeviceDetectorFunction  detector, uint16_t vid, uint16_t pid, int interface, int usage_page, int usage)
{
    HIDDeviceDetectorBlock block;

    block.name          = name;
    block.vid           = vid;
    block.pid           = pid;
    block.function      = detector;
    block.interface     = interface;
    block.usage_page    = usage_page;
    block.usage         = usage;

    hid_device_detectors.push_back(block);
}

void DetectionManager::RegisterHIDWrappedDeviceDetector(std::string name, HIDWrappedDeviceDetectorFunction  detector, uint16_t vid, uint16_t pid, int interface, int usage_page, int usage)
{
    HIDWrappedDeviceDetectorBlock block;

    block.name          = name;
    block.vid           = vid;
    block.pid           = pid;
    block.function      = detector;
    block.interface     = interface;
    block.usage_page    = usage_page;
    block.usage         = usage;

    hid_wrapped_device_detectors.push_back(block);
}

void DetectionManager::RegisterI2CDeviceDetector(std::string name, I2CDeviceDetectorFunction detector)
{
    i2c_device_detector_strings.push_back(name);
    i2c_device_detectors.push_back(detector);
}

void DetectionManager::RegisterI2CDRAMDeviceDetector(std::string name, I2CDRAMDeviceDetectorFunction detector, uint16_t jedec_id, uint8_t dram_type)
{
    I2CDRAMDeviceDetectorBlock block;

    block.name          = name;
    block.function      = detector;
    block.jedec_id      = jedec_id;
    block.dram_type     = dram_type;

    i2c_dram_device_detectors.push_back(block);
}

void DetectionManager::RegisterI2CPCIDeviceDetector(std::string name, I2CPCIDeviceDetectorFunction detector, uint16_t ven_id, uint16_t dev_id, uint16_t subven_id, uint16_t subdev_id, uint8_t i2c_addr)
{
    I2CPCIDeviceDetectorBlock block;

    block.name          = name;
    block.function      = detector;
    block.ven_id        = ven_id;
    block.dev_id        = dev_id;
    block.subven_id     = subven_id;
    block.subdev_id     = subdev_id;
    block.i2c_addr      = i2c_addr;

    i2c_pci_device_detectors.push_back(block);
}

/*---------------------------------------------------------*\
| Pre-Detection Hook Function Registration Function         |
\*---------------------------------------------------------*/
void DetectionManager::RegisterPreDetectionHook(PreDetectionHookFunction hook)
{
    pre_detection_hooks.push_back(hook);
}

/*---------------------------------------------------------*\
| Detection Callback Registration Functions                 |
\*---------------------------------------------------------*/
void DetectionManager::RegisterDetectionCallback(DetectionCallback new_callback, void * new_callback_arg)
{
    DetectionCallbackMutex.lock();

    for(std::size_t idx = 0; idx < DetectionCallbacks.size(); idx++)
    {
        if(DetectionCallbacks[idx] == new_callback && DetectionCallbackArgs[idx] == new_callback_arg)
        {
            DetectionCallbackMutex.unlock();

            LOG_TRACE("[%s] Tried to register an already registered detection callback, skipping.  Total callbacks registered: %d", DETECTIONMANAGER, DetectionCallbacks.size());

            return;
        }
    }

    DetectionCallbacks.push_back(new_callback);
    DetectionCallbackArgs.push_back(new_callback_arg);

    DetectionCallbackMutex.unlock();

    LOG_TRACE("[%s] Registered detection callback.  Total callbacks registered: %d", DETECTIONMANAGER, DetectionCallbacks.size());
}

void DetectionManager::UnregisterDetectionCallback(DetectionCallback callback, void *callback_arg)
{
    DetectionCallbackMutex.lock();

    for(std::size_t idx = 0; idx < DetectionCallbacks.size(); idx++)
    {
        if(DetectionCallbacks[idx] == callback && DetectionCallbackArgs[idx] == callback_arg)
        {
            DetectionCallbacks.erase(DetectionCallbacks.begin() + idx);
            DetectionCallbackArgs.erase(DetectionCallbackArgs.begin() + idx);
        }
    }

    DetectionCallbackMutex.unlock();

    LOG_TRACE("[%s] Unregistered detection callback.  Total callbacks registered: %d", DETECTIONMANAGER, DetectionCallbacks.size());
}

/*---------------------------------------------------------*\
| Functions for registering I2C buses from within detectors |
\*---------------------------------------------------------*/
void DetectionManager::RegisterI2CBus(i2c_smbus_interface *bus)
{
    LOG_INFO("[%s] Registering I2C interface: %s Device %04X:%04X Subsystem: %04X:%04X", DETECTIONMANAGER, bus->device_name, bus->pci_vendor, bus->pci_device, bus->pci_subsystem_vendor, bus->pci_subsystem_device);

    /*-----------------------------------------------------*\
    | Add the new bus to the list                           |
    \*-----------------------------------------------------*/
    i2c_buses.push_back(bus);

    /*-----------------------------------------------------*\
    | Signal I2C Bus List Update                            |
    \*-----------------------------------------------------*/
    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_I2C_BUS_REGISTERED);
}

/*---------------------------------------------------------*\
| Functions for registering RGBControllers from within      |
| detectors                                                 |
\*---------------------------------------------------------*/
void DetectionManager::RegisterRGBController(RGBController *rgb_controller)
{
    LOG_INFO("[%s] Registering RGB controller %s", DETECTIONMANAGER, rgb_controller->GetName().c_str());

    /*-----------------------------------------------------*\
    | Mark this controller as locally owned                 |
    \*-----------------------------------------------------*/
    rgb_controller->flags &= ~CONTROLLER_FLAG_REMOTE;
    rgb_controller->flags |= CONTROLLER_FLAG_LOCAL;

    /*-----------------------------------------------------*\
    | Load sizes for the new controller                     |
    \*-----------------------------------------------------*/
    ResourceManager::get()->GetProfileManager()->LoadControllerFromListWithOptions(rgb_controllers_sizes, detection_size_entry_used, rgb_controller, true, false);

    /*-----------------------------------------------------*\
    | Add the new controller to the list                    |
    \*-----------------------------------------------------*/
    rgb_controllers.push_back(rgb_controller);

    /*-----------------------------------------------------*\
    | Signal Device List Update                             |
    \*-----------------------------------------------------*/
    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_RGBCONTROLLER_REGISTERED);
}

void DetectionManager::UnregisterRGBController(RGBController* rgb_controller)
{
    LOG_INFO("[%s] Unregistering RGB controller %s", DETECTIONMANAGER, rgb_controller->GetName().c_str());

    /*-----------------------------------------------------*\
    | Clear callbacks from the controller before removal    |
    \*-----------------------------------------------------*/
    rgb_controller->ClearCallbacks();

    /*-----------------------------------------------------*\
    | Find the controller to remove and remove it from the  |
    | master list                                           |
    \*-----------------------------------------------------*/
    std::vector<RGBController*>::iterator rgb_it = std::find(rgb_controllers.begin(), rgb_controllers.end(), rgb_controller);

    if(rgb_it != rgb_controllers.end())
    {
        rgb_controllers.erase(rgb_it);
    }

    /*-----------------------------------------------------*\
    | Signal Device List Update                             |
    \*-----------------------------------------------------*/
    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_RGBCONTROLLER_UNREGISTERED);
}

/*---------------------------------------------------------*\
| Detection state functions                                 |
\*---------------------------------------------------------*/
void DetectionManager::AbortDetection()
{
    LOG_INFO("[%s] Detection abort requested", DETECTIONMANAGER);

    detection_in_progress   = false;
    detection_percent       = 100;
    detection_string        = "Stopping";
}

void DetectionManager::BeginDetection()
{
    /*-----------------------------------------------------*\
    | Perform pre-detection setup                           |
    \*-----------------------------------------------------*/
    bool detection_ready = ProcessPreDetection();

    /*-----------------------------------------------------*\
    | Run DetectDevices function in background thread if    |
    | ready to run detection                                |
    \*-----------------------------------------------------*/
    if(detection_ready)
    {
        RunInBackgroundThread(std::bind(&DetectionManager::BackgroundDetectDevices, this));
    }
}

unsigned int DetectionManager::GetDetectionPercent()
{
    return(detection_percent.load());
}

std::string DetectionManager::GetDetectionString()
{
    return(detection_string);
}

void DetectionManager::WaitForDetection()
{
    DetectDevicesMutex.lock();
    DetectDevicesMutex.unlock();
}

/*---------------------------------------------------------*\
| DetectionManager Background Thread Functions              |
\*---------------------------------------------------------*/
void DetectionManager::BackgroundThreadFunction()
{
    /*-----------------------------------------------------*\
    | The background thread that runs scheduled coroutines  |
    | when applicable                                       |
    | Stays asleep if nothing is scheduled                  |
    | NOTE: this thread owns the HIDAPI library internal    |
    | objects on MacOS                                      |
    | hid_init and hid_exit may not be called outside of    |
    | this thread.  Calling hid_exit outside of this thread |
    | WILL cause an immediate CRASH on MacOS.               |
    | BackgroundThreadStateMutex will be UNLOCKED as long   |
    | as the thread is suspended.  It locks automatically   |
    | when any coroutine is running.  However, it seems to  |
    | be necessary to be separate from the                  |
    | DeviceDetectionMutex, even though their states are    |
    | nearly identical.                                     |
    \------------------------------------------------------*/
    std::unique_lock lock(BackgroundThreadStateMutex);

    while(background_thread_running)
    {
        if(BackgroundThreadScheduledFunction)
        {
            std::function<void()> coroutine = nullptr;
            std::swap(BackgroundThreadScheduledFunction, coroutine);

            try
            {
                coroutine();
            }
            catch(std::exception& e)
            {
                LOG_ERROR("[%s] Unhandled exception in coroutine; e.what(): %s", DETECTIONMANAGER, e.what());
            }
            catch(...)
            {
                LOG_ERROR("[%s] Unhandled exception in coroutine", DETECTIONMANAGER);
            }
        }

        /*-------------------------------------------------*\
        | This line will cause the thread to suspend until  |
        | the condition variable is triggered               |
        | NOTE: it may be subject to "spurious wakeups"     |
        \*-------------------------------------------------*/
        BackgroundThreadScheduledFunctionStart.wait(lock);
    }
}

void DetectionManager::RunInBackgroundThread(std::function<void()> coroutine)
{
    if(std::this_thread::get_id() == BackgroundThread->get_id())
    {
        /*-------------------------------------------------*\
        | We are already in the background thread - don't   |
        | schedule the call, run it immediately             |
        \*-------------------------------------------------*/
        coroutine();
    }
    else
    {
        /*-------------------------------------------------*\
        | Lock the background thread state mutex and assign |
        | the new function to the background thread, then   |
        | trigger the background thread to start it         |
        \*-------------------------------------------------*/
        BackgroundThreadStateMutex.lock();

        if(BackgroundThreadScheduledFunction != nullptr)
        {
            LOG_WARNING("[%s] Detection coroutine: assigned a new coroutine when one was already scheduled - probably two rescan events sent at once", DETECTIONMANAGER);
        }

        BackgroundThreadScheduledFunction = coroutine;
        BackgroundThreadStateMutex.unlock();
        BackgroundThreadScheduledFunctionStart.notify_one();
    }
}

/*---------------------------------------------------------*\
| Functions that must be run in the background thread       |
\*---------------------------------------------------------*/
void DetectionManager::BackgroundDetectDevices()
{
    /*-----------------------------------------------------*\
    | Lock detection mutex                                  |
    \*-----------------------------------------------------*/
    DetectDevicesMutex.lock();

    /*-----------------------------------------------------*\
    | Set detection in progress flag                        |
    \*-----------------------------------------------------*/
    detection_in_progress = true;

    /*-----------------------------------------------------*\
    | Initialize local variables                            |
    \*-----------------------------------------------------*/
    hid_device_info*    current_hid_device;
    json                detector_settings;
    hid_device_info*    hid_devices                 = NULL;
    bool                hid_safe_mode               = false;
    unsigned int        initial_detection_delay_ms  = 0;

    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|               Start device detection               |");
    LOG_INFO("------------------------------------------------------");

    /*-----------------------------------------------------*\
    | Start at 0% detection progress                        |
    \*-----------------------------------------------------*/
    detection_percent   = 0;
    detection_string    = "";

    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

    /*-----------------------------------------------------*\
    | Open device disable list and read in disabled         |
    | device strings                                        |
    \*-----------------------------------------------------*/
    detector_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Detectors");

    /*-----------------------------------------------------*\
    | Check HID safe mode setting                           |
    \*-----------------------------------------------------*/
    if(detector_settings.contains("hid_safe_mode"))
    {
        hid_safe_mode = detector_settings["hid_safe_mode"];
    }

    /*-----------------------------------------------------*\
    | Check initial detection delay setting                 |
    \*-----------------------------------------------------*/
    if(detector_settings.contains("initial_detection_delay_ms"))
    {
        initial_detection_delay_ms = detector_settings["initial_detection_delay_ms"];
    }

    /*-----------------------------------------------------*\
    | If configured, delay detection for the configured     |
    | time only on first detection                          |
    \*-----------------------------------------------------*/
    if(initial_detection)
    {
        if(initial_detection_delay_ms != 0)
        {
            detection_string = "Waiting for detection delay";
            SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

            LOG_INFO("[%s] Delaying detection for %d ms", DETECTIONMANAGER, initial_detection_delay_ms);
            std::this_thread::sleep_for(initial_detection_delay_ms * 1ms);
        }

        /*-------------------------------------------------*\
        | Also initialize HID on first detection, as this   |
        | must be called from the background thread         |
        \*-------------------------------------------------*/
        BackgroundHidInit();

        initial_detection = false;
    }

    /*-----------------------------------------------------*\
    | Reset the size entry used flags vector                |
    \*-----------------------------------------------------*/
    detection_size_entry_used.resize(rgb_controllers_sizes.size());

    for(std::size_t size_idx = 0; size_idx < detection_size_entry_used.size(); size_idx++)
    {
        detection_size_entry_used[size_idx] = false;
    }

    /*-----------------------------------------------------*\
    | Enumerate HID devices unless using HID safe mode      |
    \*-----------------------------------------------------*/
    if(!hid_safe_mode)
    {
        hid_devices = hid_enumerate(0, 0);
    }

    /*-----------------------------------------------------*\
    | Calculate the percentage denominator by adding the    |
    | number of I2C and miscellaneous detectors and the     |
    | number of enumerated HID devices                      |
    |                                                       |
    | Start by iterating through all HID devices in list to |
    | get a total count                                     |
    \*-----------------------------------------------------*/
    detection_percent_i2c_count         = (unsigned int)i2c_device_detectors.size();
    detection_percent_i2c_dram_count    = (unsigned int)i2c_dram_device_detectors.size();
    detection_percent_i2c_pci_count     = (unsigned int)i2c_pci_device_detectors.size();

    if(hid_safe_mode)
    {
        detection_percent_hid_count     = (unsigned int)hid_device_detectors.size();
    }
    else
    {
        current_hid_device              = hid_devices;
        detection_percent_hid_count     = 0;

        while(current_hid_device)
        {
            detection_percent_hid_count++;

            current_hid_device          = current_hid_device->next;
        }
    }

    detection_percent_other_count       = (unsigned int)device_detectors.size();

    detection_percent_denominator       = detection_percent_i2c_count
                                        + detection_percent_i2c_dram_count
                                        + detection_percent_i2c_pci_count
                                        + detection_percent_other_count
                                        + detection_percent_hid_count;

    /*-----------------------------------------------------*\
    | Detect I2C interfaces                                 |
    \*-----------------------------------------------------*/
    BackgroundDetectI2CBuses();

    /*-----------------------------------------------------*\
    | Detect I2C devices                                    |
    \*-----------------------------------------------------*/
    BackgroundDetectI2CDevices(detector_settings);

    /*-----------------------------------------------------*\
    | Detect I2C DRAM devices                               |
    \*-----------------------------------------------------*/
    BackgroundDetectI2CDRAMDevices(detector_settings);

    /*-----------------------------------------------------*\
    | Detect I2C PCI devices                                |
    \*-----------------------------------------------------*/
    BackgroundDetectI2CPCIDevices(detector_settings);

    /*-----------------------------------------------------*\
    | Detect HID devices                                    |
    \*-----------------------------------------------------*/
    if(hid_safe_mode)
    {
        BackgroundDetectHIDDevicesSafe(detector_settings);
    }
    else
    {
        BackgroundDetectHIDDevices(hid_devices, detector_settings);
    }

    /*-----------------------------------------------------*\
    | Detect Wrapped HID devices (GLIBC Linux only)         |
    \*-----------------------------------------------------*/
#ifdef __linux__
#ifdef __GLIBC__
    BackgroundDetectHIDDevicesWrapped(hid_devices, detector_settings);
#endif
#endif

    /*-----------------------------------------------------*\
    | Detect other devices                                  |
    \*-----------------------------------------------------*/
    BackgroundDetectOtherDevices(detector_settings);

    /*-----------------------------------------------------*\
    | Signal that detection progress reached 100%           |
    \*-----------------------------------------------------*/
    detection_percent     = 100;
    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

    /*-----------------------------------------------------*\
    | Signal that detection is complete                     |
    \*-----------------------------------------------------*/
    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_COMPLETE);

    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|                Detection completed                 |");
    LOG_INFO("------------------------------------------------------");

    /*-----------------------------------------------------*\
    | Clear detection in progress flag                      |
    \*-----------------------------------------------------*/
    detection_in_progress = false;

    /*-----------------------------------------------------*\
    | Unlock detection mutex                                |
    \*-----------------------------------------------------*/
    DetectDevicesMutex.unlock();

#ifdef __linux__
    /*-----------------------------------------------------*\
    | Check if the udev rules exist                         |
    \*-----------------------------------------------------*/
    bool udev_not_exist     = false;
    bool udev_multiple      = false;

    if(access("/etc/udev/rules.d/60-openrgb.rules", F_OK) != 0)
    {
        if(access("/usr/lib/udev/rules.d/60-openrgb.rules", F_OK) != 0)
        {
            udev_not_exist  = true;
        }
    }
    else
    {
        if(access("/usr/lib/udev/rules.d/60-openrgb.rules", F_OK) == 0)
        {
            udev_multiple   = true;
        }
    }

    /*-----------------------------------------------------*\
    | If the udev rules file is not installed, show a dialog|
    \*-----------------------------------------------------*/
    if(udev_not_exist)
    {
        LOG_DIALOG("%s", UDEV_MISSING);

        udev_multiple       = false;
        i2c_interface_fail  = false;
    }

    /*-----------------------------------------------------*\
    | If multiple udev rules files are installed, show a    |
    | dialog                                                |
    \*-----------------------------------------------------*/
    if(udev_multiple)
    {
        LOG_DIALOG("%s", UDEV_MUTLI);

        i2c_interface_fail  = false;
    }

#endif

    /*-----------------------------------------------------*\
    | If any i2c interfaces failed to detect due to an      |
    | error condition, show a dialog                        |
    \*-----------------------------------------------------*/
    if(i2c_interface_fail)
    {
#ifdef _WIN32
        LOG_DIALOG("%s", I2C_ERR_WIN);
#endif
#ifdef __linux__
        LOG_DIALOG("%s", I2C_ERR_LINUX);
#endif
    }
}

void DetectionManager::BackgroundDetectI2CBuses()
{
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|             Detecting I2C interfaces               |");
    LOG_INFO("------------------------------------------------------");

    detection_percent   = 0;
    detection_string    = "Detecting I2C interfaces";

    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

    i2c_interface_fail = false;

    for(std::size_t i2c_bus_detector_idx = 0; i2c_bus_detector_idx < i2c_bus_detectors.size(); i2c_bus_detector_idx++)
    {
        if(i2c_bus_detectors[i2c_bus_detector_idx]() == false)
        {
            i2c_interface_fail = true;
        }
    }
}

void DetectionManager::BackgroundDetectI2CDevices(json detector_settings)
{
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|               Detecting I2C devices                |");
    LOG_INFO("------------------------------------------------------");

    for(std::size_t i2c_detector_idx = 0; i2c_detector_idx < i2c_device_detectors.size(); i2c_detector_idx++)
    {
        /*-------------------------------------------------*\
        | Update detection string                           |
        \*-------------------------------------------------*/
        detection_string = i2c_device_detector_strings[i2c_detector_idx].c_str();

        /*-------------------------------------------------*\
        | Check if this detector is enabled                 |
        \*-------------------------------------------------*/
        bool this_device_enabled = true;

        if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
        {
            this_device_enabled = detector_settings["detectors"][detection_string];
        }

        LOG_DEBUG("[%s] %s is %s", DETECTIONMANAGER, detection_string.c_str(), ((this_device_enabled == true) ? "enabled" : "disabled"));

        /*-------------------------------------------------*\
        | If enabled, signal progress changed and call the  |
        | detector                                          |
        \*-------------------------------------------------*/
        if(this_device_enabled)
        {
            SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

            i2c_device_detectors[i2c_detector_idx](i2c_buses);
        }

        LOG_TRACE("[%s] %s detection end", DETECTIONMANAGER, detection_string.c_str());

        /*-------------------------------------------------*\
        | Update detection percent                          |
        \*-------------------------------------------------*/
        unsigned int detection_percent_numerator = i2c_detector_idx;

        detection_percent = (unsigned int)(100.0f * (detection_percent_numerator / detection_percent_denominator));
    }
}

void DetectionManager::BackgroundDetectI2CDRAMDevices(json detector_settings)
{
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|            Detecting I2C DRAM modules              |");
    LOG_INFO("------------------------------------------------------");

    detection_string = "Reading DRAM SPD Information";

    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

    for(std::size_t bus = 0; bus < i2c_buses.size() && IsAnyDimmDetectorEnabled(detector_settings); bus++)
    {
        IF_DRAM_SMBUS(i2c_buses[bus]->pci_vendor, i2c_buses[bus]->pci_device)
        {
            std::vector<SPDWrapper> slots;
            SPDMemoryType dram_type = SPD_RESERVED;

            for(uint8_t spd_addr = 0x50; spd_addr < 0x58; spd_addr++)
            {
                SPDDetector spd(i2c_buses[bus], spd_addr, dram_type);
                if(spd.is_valid())
                {
                    SPDWrapper accessor(spd);
                    dram_type = spd.memory_type();
                    LOG_INFO("[%s] Detected occupied slot %d, bus %d, type %s", DETECTIONMANAGER, spd_addr - 0x50 + 1, bus, spd_memory_type_name[dram_type]);
                    LOG_DEBUG("[%s] JEDEC ID: 0x%04x", DETECTIONMANAGER, accessor.jedec_id());
                    slots.push_back(accessor);
                }
            }

            for(std::size_t i2c_detector_idx = 0; i2c_detector_idx < i2c_dram_device_detectors.size(); i2c_detector_idx++)
            {
                if((i2c_dram_device_detectors[i2c_detector_idx].dram_type == dram_type) && is_jedec_in_slots(slots, i2c_dram_device_detectors[i2c_detector_idx].jedec_id))
                {
                    detection_string = i2c_dram_device_detectors[i2c_detector_idx].name.c_str();

                    /*-------------------------------------*\
                    | Check if this detector is enabled     |
                    \*-------------------------------------*/
                    bool this_device_enabled = true;

                    if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
                    {
                        this_device_enabled = detector_settings["detectors"][detection_string];
                    }

                    LOG_DEBUG("[%s] %s is %s", DETECTIONMANAGER, detection_string.c_str(), ((this_device_enabled == true) ? "enabled" : "disabled"));

                    if(this_device_enabled)
                    {
                        SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

                        std::vector<SPDWrapper*> matching_slots = slots_with_jedec(slots, i2c_dram_device_detectors[i2c_detector_idx].jedec_id);
                        i2c_dram_device_detectors[i2c_detector_idx].function(i2c_buses[bus], matching_slots, i2c_dram_device_detectors[i2c_detector_idx].name);
                    }

                    LOG_TRACE("[%s] %s detection end", DETECTIONMANAGER, detection_string.c_str());
                }

                /*-----------------------------------------*\
                | Update detection percent                  |
                \*-----------------------------------------*/
                unsigned int detection_percent_numerator = detection_percent_i2c_count
                                                         + i2c_detector_idx;

                detection_percent = (unsigned int)(100.0f * (detection_percent_numerator / detection_percent_denominator));
            }
        }
    }
}

void DetectionManager::BackgroundDetectI2CPCIDevices(json detector_settings)
{
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|               Detecting I2C PCI devices            |");
    LOG_INFO("------------------------------------------------------");

    for(std::size_t i2c_detector_idx = 0; i2c_detector_idx < i2c_pci_device_detectors.size(); i2c_detector_idx++)
    {
        detection_string = i2c_pci_device_detectors[i2c_detector_idx].name.c_str();

        /*-------------------------------------------------*\
        | Check if this detector is enabled                 |
        \*-------------------------------------------------*/
        bool this_device_enabled = true;

        if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
        {
            this_device_enabled = detector_settings["detectors"][detection_string];
        }

        LOG_DEBUG("[%s] %s is %s", DETECTIONMANAGER, detection_string.c_str(), ((this_device_enabled == true) ? "enabled" : "disabled"));

        if(this_device_enabled)
        {
            SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

            for(std::size_t bus = 0; bus < i2c_buses.size(); bus++)
            {
                if(i2c_buses[bus]->pci_vendor           == i2c_pci_device_detectors[i2c_detector_idx].ven_id    &&
                   i2c_buses[bus]->pci_device           == i2c_pci_device_detectors[i2c_detector_idx].dev_id    &&
                   i2c_buses[bus]->pci_subsystem_vendor == i2c_pci_device_detectors[i2c_detector_idx].subven_id &&
                   i2c_buses[bus]->pci_subsystem_device == i2c_pci_device_detectors[i2c_detector_idx].subdev_id)
                {
                    i2c_pci_device_detectors[i2c_detector_idx].function(i2c_buses[bus], i2c_pci_device_detectors[i2c_detector_idx].i2c_addr, i2c_pci_device_detectors[i2c_detector_idx].name);
                }
            }
        }

        LOG_TRACE("[%s] %s detection end", DETECTIONMANAGER, detection_string.c_str());

        /*-------------------------------------------------*\
        | Update detection percent                          |
        \*-------------------------------------------------*/
        unsigned int detection_percent_numerator = detection_percent_i2c_count
                                                 + detection_percent_i2c_dram_count
                                                 + i2c_detector_idx;

        detection_percent = (unsigned int)(100.0f * (detection_percent_numerator / detection_percent_denominator));
    }
}

void DetectionManager::BackgroundDetectHIDDevices(hid_device_info* hid_devices, json detector_settings)
{
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|               Detecting HID devices                |");
    LOG_INFO("------------------------------------------------------");

    hid_device_info* current_hid_device = hid_devices;

    /*-----------------------------------------------------*\
    | Iterate through all devices in list and run detectors |
    \*-----------------------------------------------------*/
    unsigned int hid_device_count = 0;

    while(current_hid_device)
    {
        if(LogManager::get()->getLoglevel() >= LL_DEBUG)
        {
            const char* manu_name = StringUtils::wchar_to_char(current_hid_device->manufacturer_string);
            const char* prod_name = StringUtils::wchar_to_char(current_hid_device->product_string);

            LOG_DEBUG("[%s] %04X:%04X U=%04X P=0x%04X I=%d - %-25s - %s", DETECTIONMANAGER, current_hid_device->vendor_id, current_hid_device->product_id, current_hid_device->usage, current_hid_device->usage_page, current_hid_device->interface_number, manu_name, prod_name);
        }

        detection_string = "";

        SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

        /*-------------------------------------------------*\
        | Loop through all available detectors.  If all     |
        | required information matches, run the detector    |
        \*-------------------------------------------------*/
        for(std::size_t hid_detector_idx = 0; hid_detector_idx < hid_device_detectors.size(); hid_detector_idx++)
        {
            HIDDeviceDetectorBlock & detector = hid_device_detectors[hid_detector_idx];

            if(detector.compare(current_hid_device))
            {
                detection_string = detector.name.c_str();

                /*-----------------------------------------*\
                | Check if this detector is enabled or      |
                | needs to be added to the settings list    |
                \*-----------------------------------------*/
                bool this_device_enabled = true;

                if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
                {
                    this_device_enabled = detector_settings["detectors"][detection_string];
                }

                LOG_DEBUG("[%s] %s is %s", DETECTIONMANAGER, detection_string.c_str(), ((this_device_enabled == true) ? "enabled" : "disabled"));

                if(this_device_enabled)
                {
                    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

                    detector.function(current_hid_device, hid_device_detectors[hid_detector_idx].name);
                }
            }
        }

        /*-------------------------------------------------*\
        | Loop through all available wrapped HID detectors. |
        | If all required information matches, run the      |
        | detector                                          |
        \*-------------------------------------------------*/
        for(std::size_t hid_detector_idx = 0; hid_detector_idx < hid_wrapped_device_detectors.size(); hid_detector_idx++)
        {
            HIDWrappedDeviceDetectorBlock & detector = hid_wrapped_device_detectors[hid_detector_idx];

            if(detector.compare(current_hid_device))
            {
                detection_string = detector.name.c_str();

                /*-----------------------------------------*\
                | Check if this detector is enabled or      |
                | needs to be added to the settings list    |
                \*-----------------------------------------*/
                bool this_device_enabled = true;

                if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
                {
                    this_device_enabled = detector_settings["detectors"][detection_string];
                }

                LOG_DEBUG("[%s] %s is %s", DETECTIONMANAGER, detection_string.c_str(), ((this_device_enabled == true) ? "enabled" : "disabled"));

                if(this_device_enabled)
                {
                    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

                    detector.function(default_hidapi_wrapper, current_hid_device, hid_wrapped_device_detectors[hid_detector_idx].name);
                }
            }
        }

        /*-------------------------------------------------*\
        | Update detection percent                          |
        \*-------------------------------------------------*/
        hid_device_count++;

        unsigned int detection_percent_numerator = detection_percent_i2c_count
                                                 + detection_percent_i2c_dram_count
                                                 + detection_percent_i2c_pci_count
                                                 + hid_device_count;

        detection_percent = (unsigned int)(100.0f * (detection_percent_numerator / detection_percent_denominator));

        /*-------------------------------------------------*\
        | Move on to the next HID device                    |
        \*-------------------------------------------------*/
        current_hid_device = current_hid_device->next;
    }

    /*-----------------------------------------------------*\
    | Done using the device list, free it                   |
    \*-----------------------------------------------------*/
    hid_free_enumeration(hid_devices);
}

void DetectionManager::BackgroundDetectHIDDevicesSafe(json detector_settings)
{
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|               Detecting HID devices                |");
    LOG_INFO("|                  with safe mode                    |");
    LOG_INFO("------------------------------------------------------");

    /*-----------------------------------------------------*\
    | Loop through all available detectors.  If all         |
    | required information matches, run the detector        |
    \*-----------------------------------------------------*/
    for(std::size_t hid_detector_idx = 0; hid_detector_idx < hid_device_detectors.size(); hid_detector_idx++)
    {
        HIDDeviceDetectorBlock & detector = hid_device_detectors[hid_detector_idx];

        LOG_VERBOSE("[%s] Trying to run detector for [%s] (for %04x:%04x)", DETECTIONMANAGER, detector.name.c_str(), detector.vid, detector.pid);

        hid_device_info* current_hid_device = hid_enumerate(detector.vid, detector.pid);

        while(current_hid_device)
        {
            if(detector.compare(current_hid_device))
            {
                detection_string = detector.name.c_str();

                /*-----------------------------------------*\
                | Check if this detector is enabled or      |
                | needs to be added to the settings list    |
                \*-----------------------------------------*/
                bool this_device_enabled = true;

                if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
                {
                    this_device_enabled = detector_settings["detectors"][detection_string];
                }

                LOG_DEBUG("[%s] %s is %s", DETECTIONMANAGER, detection_string.c_str(), ((this_device_enabled == true) ? "enabled" : "disabled"));

                if(this_device_enabled)
                {
                    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

                    detector.function(current_hid_device, hid_device_detectors[hid_detector_idx].name);

                    LOG_TRACE("[%s] %s detection end", DETECTIONMANAGER, detection_string.c_str());
                }
            }

            current_hid_device = current_hid_device->next;

            /*---------------------------------------------*\
            | Update detection percent                      |
            \*---------------------------------------------*/
            unsigned int detection_percent_numerator = detection_percent_i2c_count
                                                    + detection_percent_i2c_dram_count
                                                    + detection_percent_i2c_pci_count
                                                    + hid_detector_idx;

            detection_percent = (unsigned int)(100.0f * (detection_percent_numerator / detection_percent_denominator));
        }

        hid_free_enumeration(current_hid_device);
    }
}

#ifdef __linux__
#ifdef __GLIBC__
void DetectionManager::BackgroundDetectHIDDevicesWrapped(hid_device_info* hid_devices, json detector_settings)
{
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|            Detecting libusb HID devices            |");
    LOG_INFO("------------------------------------------------------");

    void *         dyn_handle = NULL;
    hidapi_wrapper wrapper;

    /*-----------------------------------------------------*\
    | Load the libhidapi-libusb library                     |
    \*-----------------------------------------------------*/
    if((dyn_handle = dlopen("libhidapi-libusb.so", RTLD_NOW | RTLD_NODELETE | RTLD_DEEPBIND)))
    {
        /*-------------------------------------------------*\
        | Create a wrapper with the libusb functions        |
        \*-------------------------------------------------*/
        wrapper =
        {
            .dyn_handle                     = dyn_handle,
            .hid_send_feature_report        = (hidapi_wrapper_send_feature_report)          dlsym(dyn_handle,"hid_send_feature_report"),
            .hid_get_feature_report         = (hidapi_wrapper_get_feature_report)           dlsym(dyn_handle,"hid_get_feature_report"),
            .hid_get_serial_number_string   = (hidapi_wrapper_get_serial_number_string)     dlsym(dyn_handle,"hid_get_serial_number_string"),
            .hid_open_path                  = (hidapi_wrapper_open_path)                    dlsym(dyn_handle,"hid_open_path"),
            .hid_enumerate                  = (hidapi_wrapper_enumerate)                    dlsym(dyn_handle,"hid_enumerate"),
            .hid_free_enumeration           = (hidapi_wrapper_free_enumeration)             dlsym(dyn_handle,"hid_free_enumeration"),
            .hid_close                      = (hidapi_wrapper_close)                        dlsym(dyn_handle,"hid_close"),
            .hid_error                      = (hidapi_wrapper_error)                        dlsym(dyn_handle,"hid_free_enumeration")
        };

        hid_devices                         = wrapper.hid_enumerate(0, 0);

        hid_device_info* current_hid_device = hid_devices;

        /*-------------------------------------------------*\
        | Iterate through all devices in list and run       |
        | detectors                                         |
        \*-------------------------------------------------*/
        unsigned int hid_device_count       = 0;

        while(current_hid_device)
        {
            if(LogManager::get()->getLoglevel() >= LL_DEBUG)
            {
                const char* manu_name = StringUtils::wchar_to_char(current_hid_device->manufacturer_string);
                const char* prod_name = StringUtils::wchar_to_char(current_hid_device->product_string);

                LOG_DEBUG("[%s] %04X:%04X U=%04X P=0x%04X I=%d - %-25s - %s", DETECTIONMANAGER, current_hid_device->vendor_id, current_hid_device->product_id, current_hid_device->usage, current_hid_device->usage_page, current_hid_device->interface_number, manu_name, prod_name);
            }

            detection_string = "";

            SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

            /*---------------------------------------------*\
            | Loop through all available wrapped HID        |
            | detectors.  If all required information       |
            | matches, run the detector                     |
            \*---------------------------------------------*/
            for(std::size_t hid_detector_idx = 0; hid_detector_idx < hid_wrapped_device_detectors.size(); hid_detector_idx++)
            {
                HIDWrappedDeviceDetectorBlock & detector = hid_wrapped_device_detectors[hid_detector_idx];

                if(detector.compare(current_hid_device))
                {
                    detection_string = detector.name.c_str();

                    /*-------------------------------------*\
                    | Check if this detector is enabled or  |
                    | needs to be added to the settings list|
                    \*-------------------------------------*/
                    bool this_device_enabled = true;

                    if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
                    {
                        this_device_enabled = detector_settings["detectors"][detection_string];
                    }

                    LOG_DEBUG("[%s] %s is %s", DETECTIONMANAGER, detection_string.c_str(), ((this_device_enabled == true) ? "enabled" : "disabled"));

                    if(this_device_enabled)
                    {
                        SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

                        detector.function(wrapper, current_hid_device, detector.name);
                    }
                }
            }

            /*---------------------------------------------*\
            | Update detection percent                      |
            \*---------------------------------------------*/
            hid_device_count++;

            detection_percent = (unsigned int)(100.0f * ((i2c_device_detectors.size() + i2c_dram_device_detectors.size() + i2c_pci_device_detectors.size() + hid_device_count) / detection_percent_denominator));

            /*---------------------------------------------*\
            | Move on to the next HID device                |
            \*---------------------------------------------*/
            current_hid_device = current_hid_device->next;
        }

        /*-------------------------------------------------*\
        | Done using the device list, free it               |
        \*-------------------------------------------------*/
        wrapper.hid_free_enumeration(hid_devices);
    }
}
#endif
#endif

void DetectionManager::BackgroundDetectOtherDevices(json detector_settings)
{
    LOG_INFO("------------------------------------------------------");
    LOG_INFO("|              Detecting other devices               |");
    LOG_INFO("------------------------------------------------------");

    for(std::size_t detector_idx = 0; detector_idx < device_detectors.size(); detector_idx++)
    {
        detection_string = device_detector_strings[detector_idx].c_str();

        /*-------------------------------------------------*\
        | Check if this detector is enabled                 |
        \*-------------------------------------------------*/
        bool this_device_enabled = true;

        if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string))
        {
            this_device_enabled = detector_settings["detectors"][detection_string];
        }

        LOG_DEBUG("[%s] %s is %s", DETECTIONMANAGER, detection_string.c_str(), ((this_device_enabled == true) ? "enabled" : "disabled"));

        if(this_device_enabled)
        {
            SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

            device_detectors[detector_idx]();
        }

        LOG_TRACE("[%s] %s detection end", DETECTIONMANAGER, detection_string.c_str());

        /*-------------------------------------------------*\
        | Update detection percent                          |
        \*-------------------------------------------------*/
        unsigned int detection_percent_numerator = detection_percent_i2c_count
                                                 + detection_percent_i2c_dram_count
                                                 + detection_percent_i2c_pci_count
                                                 + detection_percent_hid_count
                                                 + detector_idx;

        detection_percent = (unsigned int)(100.0f * (detection_percent_numerator / detection_percent_denominator));
    }
}

void DetectionManager::BackgroundHidExit()
{
    /*-----------------------------------------------------*\
    | Exit HID interface                                    |
    \*-----------------------------------------------------*/
    int hid_status = hid_exit();

    LOG_DEBUG("[%s] Exiting HID interface: %s", DETECTIONMANAGER, ((hid_status == 0) ? "Success" : "Failed"));
}

void DetectionManager::BackgroundHidInit()
{
    /*-----------------------------------------------------*\
    | Initialize HID interface                              |
    \*-----------------------------------------------------*/
    int hid_status = hid_init();

    LOG_DEBUG("[%s] Initializing HID interfaces: %s", DETECTIONMANAGER, ((hid_status == 0) ? "Success" : "Failed"));
}

/*---------------------------------------------------------*\
| Detection processing functions                            |
\*---------------------------------------------------------*/
void DetectionManager::ProcessCleanup()
{
    WaitForDetection();

    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_RGBCONTROLLER_LIST_CLEARED);

    /*-----------------------------------------------------*\
    | Make a copy of the list so that the controllers can   |
    | be deleted after the list is cleared                  |
    \*-----------------------------------------------------*/
    std::vector<RGBController *> rgb_controllers_copy = rgb_controllers;

    /*-----------------------------------------------------*\
    | Clear the controllers list                            |
    \*-----------------------------------------------------*/
    rgb_controllers.clear();

    /*-----------------------------------------------------*\
    | Delete the controllers                                |
    \*-----------------------------------------------------*/
    for(RGBController* rgb_controller : rgb_controllers_copy)
    {
        delete rgb_controller;
    }

    /*-----------------------------------------------------*\
    | Make a copy of the list so that the I2C buses can be  |
    | deleted after the list is cleared                     |
    \*-----------------------------------------------------*/
    std::vector<i2c_smbus_interface *> i2c_buses_copy = i2c_buses;

    /*-----------------------------------------------------*\
    | Clear the I2C buses list                              |
    \*-----------------------------------------------------*/
    i2c_buses.clear();

    /*-----------------------------------------------------*\
    | Delete the I2C buses                                  |
    \*-----------------------------------------------------*/
    for(i2c_smbus_interface* bus : i2c_buses_copy)
    {
        delete bus;
    }
}

void DetectionManager::ProcessDynamicDetectors()
{
    for(std::size_t detector_idx = 0; detector_idx < dynamic_detectors.size(); detector_idx++)
    {
        dynamic_detectors[detector_idx]();
    }

    dynamic_detectors_processed = true;
}

bool DetectionManager::ProcessPreDetection()
{
    /*-----------------------------------------------------*\
    | Check if detection is already in progress before      |
    | continuing                                            |
    \*-----------------------------------------------------*/
    if(detection_in_progress.load())
    {
        return false;
    }

    /*-----------------------------------------------------*\
    | Process pre-detection hooks                           |
    \*-----------------------------------------------------*/
    ProcessPreDetectionHooks();

    /*-----------------------------------------------------*\
    | Process Dynamic Detectors                             |
    \*-----------------------------------------------------*/
    if(!dynamic_detectors_processed)
    {
        ProcessDynamicDetectors();
    }

    /*-----------------------------------------------------*\
    | Call detection start callbacks                        |
    \*-----------------------------------------------------*/
    LOG_TRACE("[%s] Signaling detection start", DETECTIONMANAGER);

    SignalUpdate(DETECTIONMANAGER_UPDATE_REASON_DETECTION_STARTED);

    /*-----------------------------------------------------*\
    | Update the detector settings                          |
    \*-----------------------------------------------------*/
    UpdateDetectorSettings();

    /*-----------------------------------------------------*\
    | Initialize sizes list                                 |
    \*-----------------------------------------------------*/
    rgb_controllers_sizes = ResourceManager::get()->GetProfileManager()->GetControllerListFromSizes();

    /*-----------------------------------------------------*\
    | Clean up any existing detected devices                |
    \*-----------------------------------------------------*/
    ProcessCleanup();

    return true;
}

void DetectionManager::ProcessPreDetectionHooks()
{
    for(std::size_t hook_idx = 0; hook_idx < pre_detection_hooks.size(); hook_idx++)
    {
        pre_detection_hooks[hook_idx]();
    }
}

void DetectionManager::UpdateDetectorSettings()
{
    json                detector_settings;
    bool                save_settings       = false;

    /*-----------------------------------------------------*\
    | Open device disable list and read in disabled device  |
    | strings                                               |
    \*-----------------------------------------------------*/
    detector_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Detectors");

    /*-----------------------------------------------------*\
    | Loop through all I2C detectors and see if any need to |
    | be saved to the settings                              |
    \*-----------------------------------------------------*/
    for(std::size_t i2c_detector_idx = 0; i2c_detector_idx < i2c_device_detectors.size(); i2c_detector_idx++)
    {
        detection_string = i2c_device_detector_strings[i2c_detector_idx].c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-----------------------------------------------------*\
    | Loop through all I2C DRAM detectors and see if any    |
    | need to be saved to the settings                      |
    \*-----------------------------------------------------*/
    for(std::size_t i2c_detector_idx = 0; i2c_detector_idx < i2c_dram_device_detectors.size(); i2c_detector_idx++)
    {
        detection_string = i2c_dram_device_detectors[i2c_detector_idx].name.c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-----------------------------------------------------*\
    | Loop through all I2C PCI detectors and see if any     |
    | need to be saved to the settings                      |
    \*-----------------------------------------------------*/
    for(std::size_t i2c_pci_detector_idx = 0; i2c_pci_detector_idx < i2c_pci_device_detectors.size(); i2c_pci_detector_idx++)
    {
        detection_string = i2c_pci_device_detectors[i2c_pci_detector_idx].name.c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-----------------------------------------------------*\
    | Loop through all HID detectors and see if any need to |
    | be saved to the settings                              |
    \*-----------------------------------------------------*/
    for(std::size_t hid_detector_idx = 0; hid_detector_idx < hid_device_detectors.size(); hid_detector_idx++)
    {
        detection_string = hid_device_detectors[hid_detector_idx].name.c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-----------------------------------------------------*\
    | Loop through all HID wrapped detectors and see if any |
    | need to be saved to the settings                      |
    \*-----------------------------------------------------*/
    for(std::size_t hid_wrapped_detector_idx = 0; hid_wrapped_detector_idx < hid_wrapped_device_detectors.size(); hid_wrapped_detector_idx++)
    {
        detection_string = hid_wrapped_device_detectors[hid_wrapped_detector_idx].name.c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-----------------------------------------------------*\
    | Loop through remaining detectors and see if any need  |
    | to be saved to the settings                           |
    \*-----------------------------------------------------*/
    for(std::size_t detector_idx = 0; detector_idx < device_detectors.size(); detector_idx++)
    {
        detection_string = device_detector_strings[detector_idx].c_str();

        if(!(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detection_string)))
        {
            detector_settings["detectors"][detection_string] = true;
            save_settings = true;
        }
    }

    /*-----------------------------------------------------*\
    | If there were any setting changes that need to be     |
    | saved, set the settings in the settings manager and   |
    | save them.                                            |
    \*-----------------------------------------------------*/
    if(save_settings)
    {
        LOG_INFO("[%s] Saving detector settings", DETECTIONMANAGER);

        ResourceManager::get()->GetSettingsManager()->SetSettings("Detectors", detector_settings);

        ResourceManager::get()->GetSettingsManager()->SaveSettings();
    }
}

/*---------------------------------------------------------*\
| Function for signalling DetectionManager updates to       |
| registered callbacks                                      |
\*---------------------------------------------------------*/
void DetectionManager::SignalUpdate(unsigned int update_reason)
{
    DetectionCallbackMutex.lock();

    for(std::size_t callback_idx = 0; callback_idx < DetectionCallbacks.size(); callback_idx++)
    {
        DetectionCallbacks[callback_idx](DetectionCallbackArgs[callback_idx], update_reason);
    }

    DetectionCallbackMutex.unlock();

    LOG_TRACE("[%s] Update signalled: %d.", DETECTIONMANAGER, update_reason);
}

bool DetectionManager::IsAnyDimmDetectorEnabled(json &detector_settings)
{
    for(std::size_t i2c_detector_idx = 0; i2c_detector_idx < i2c_dram_device_detectors.size(); i2c_detector_idx++)
    {
        std::string detector_name_string = i2c_dram_device_detectors[i2c_detector_idx].name.c_str();
        /*-------------------------------------------------*\
        | Check if this detector is enabled                 |
        \*-------------------------------------------------*/
        if(detector_settings.contains("detectors") && detector_settings["detectors"].contains(detector_name_string) &&
           detector_settings["detectors"][detector_name_string] == true)
        {
            return true;
        }
    }
    return false;
}
