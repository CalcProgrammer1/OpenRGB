/*---------------------------------------------------------*\
| CreativeSoundBlasterAE5ControllerBase.h                   |
|                                                           |
|   Base interface for Creative SoundBlaster AE-5           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>

class CreativeSoundBlasterAE5ControllerBase
{
public:
    virtual ~CreativeSoundBlasterAE5ControllerBase() = default;

    virtual bool                Initialize() = 0;
    virtual std::string         GetDeviceLocation() = 0;
    virtual std::string         GetDeviceName() = 0;
    virtual unsigned int        GetLEDCount() = 0;
    virtual unsigned int        GetExternalLEDCount() = 0;
    virtual void                SetExternalLEDCount(unsigned int count) = 0;

    virtual void                SetLEDColors(unsigned char led_count, unsigned char* red_values,
                                            unsigned char* green_values, unsigned char* blue_values) = 0;
};