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

typedef void (*ClientInfoChangeCallback)(void *);

class ResourceManagerInterface
{
public:
    virtual std::vector<i2c_smbus_interface*> & GetI2CBusses()                                                                                      = 0;

    virtual std::vector<RGBController*> &       GetRGBControllers()                                                                                 = 0;

    virtual unsigned int                        GetDetectionPercent()                                                                               = 0;

    virtual filesystem::path                    GetConfigurationDirectory()                                                                         = 0;

    virtual PluginManagerInterface*             GetPluginManager()                                                                                  = 0;
    virtual ProfileManager*                     GetProfileManager()                                                                                 = 0;
    virtual SettingsManager*                    GetSettingsManager()                                                                                = 0;

    virtual void                                UpdateDeviceList()                                                                                  = 0;
    virtual void                                WaitForDeviceDetection()                                                                            = 0;

protected:
    virtual                                    ~ResourceManagerInterface() {};
};
