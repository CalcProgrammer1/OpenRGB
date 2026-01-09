/*---------------------------------------------------------*\
| ResourceManagerInterface.h                                |
|                                                           |
|   Provides a virtual interface to ResourceManager for     |
|   exposing ResourceManager to plugins.  Changes to this   |
|   class structure require a new plugin API version.       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "i2c_smbus.h"
#include "filesystem.h"

class PluginManagerInterface;
class ProfileManager;
class RGBController;
class SettingsManager;

/*---------------------------------------------------------*\
| Callback Types                                            |
\*---------------------------------------------------------*/
typedef void (*ResourceManagerCallback)(void *, unsigned int);

/*---------------------------------------------------------*\
| ResourceManager Update Reason Codes                       |
\*---------------------------------------------------------*/
enum
{
    RESOURCEMANAGER_UPDATE_REASON_DETECTION_STARTED,            /* Detection started                */
    RESOURCEMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED,   /* Detection progress changed       */
    RESOURCEMANAGER_UPDATE_REASON_DETECTION_COMPLETE,           /* Detection completed              */
    RESOURCEMANAGER_UPDATE_REASON_CLIENT_INFO_UPDATED,          /* NetworkClient info updated       */
    RESOURCEMANAGER_UPDATE_REASON_I2C_BUS_LIST_UPDATED,         /* I2C bus list updated             */
    RESOURCEMANAGER_UPDATE_REASON_DEVICE_LIST_UPDATED,          /* Device list updated              */
};

class ResourceManagerInterface
{
public:
    /*-----------------------------------------------------*\
    | Resource Accessors                                    |
    \*-----------------------------------------------------*/
    virtual filesystem::path                    GetConfigurationDirectory()                                                                         = 0;
    virtual std::vector<i2c_smbus_interface*> & GetI2CBusses()                                                                                      = 0;
    virtual PluginManagerInterface*             GetPluginManager()                                                                                  = 0;
    virtual ProfileManager*                     GetProfileManager()                                                                                 = 0;
    virtual std::vector<RGBController*> &       GetRGBControllers()                                                                                 = 0;
    virtual SettingsManager*                    GetSettingsManager()                                                                                = 0;

    /*-----------------------------------------------------*\
    | Callback Registration Functions                       |
    \*-----------------------------------------------------*/
    virtual void                                RegisterResourceManagerCallback(ResourceManagerCallback new_callback, void * new_callback_arg)      = 0;
    virtual void                                UnregisterResourceManagerCallback(ResourceManagerCallback new_callback, void * new_callback_arg)    = 0;

    /*-----------------------------------------------------*\
    | Functions to manage detection                         |
    \*-----------------------------------------------------*/
    virtual bool                                GetDetectionEnabled()                                                                               = 0;
    virtual unsigned int                        GetDetectionPercent()                                                                               = 0;
    virtual std::string                         GetDetectionString()                                                                                = 0;
    virtual void                                WaitForDetection()                                                                            = 0;

protected:
    virtual                                    ~ResourceManagerInterface() {};
};
