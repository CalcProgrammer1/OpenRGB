/*---------------------------------------------------------*\
| ResourceManagerInterface.h                                |
|                                                           |
|   Provides a virtual interface to ResourceManager for     |
|   exposing ResourceManager to plugins.  Changes to this   |
|   class structure require a new plugin API version.       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "i2c_smbus.h"
#include "filesystem.h"

class NetworkClient;
class NetworkServer;
class ProfileManager;
class RGBController;
class SettingsManager;

typedef void (*ClientInfoChangeCallback)(void *);
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

    virtual void                                UnregisterDeviceListChangeCallback(DeviceListChangeCallback callback, void * callback_arg)          = 0;
    virtual void                                UnregisterDetectionProgressCallback(DetectionProgressCallback callback, void *callback_arg)         = 0;
    virtual void                                UnregisterDetectionStartCallback(DetectionStartCallback callback, void *callback_arg)               = 0;
    virtual void                                UnregisterDetectionEndCallback(DetectionEndCallback callback, void *callback_arg)                   = 0;
    virtual void                                UnregisterI2CBusListChangeCallback(I2CBusListChangeCallback callback, void * callback_arg)          = 0;

    virtual std::vector<RGBController*> &       GetRGBControllers()                                                                                 = 0;

    virtual unsigned int                        GetDetectionPercent()                                                                               = 0;

    virtual filesystem::path                    GetConfigurationDirectory()                                                                         = 0;

    virtual std::vector<NetworkClient*>&        GetClients()                                                                                        = 0;
    virtual NetworkServer*                      GetServer()                                                                                         = 0;

    virtual ProfileManager*                     GetProfileManager()                                                                                 = 0;
    virtual SettingsManager*                    GetSettingsManager()                                                                                = 0;

    virtual void                                UpdateDeviceList()                                                                                  = 0;
    virtual void                                WaitForDeviceDetection()                                                                            = 0;

protected:
    virtual                                    ~ResourceManagerInterface() {};
};
