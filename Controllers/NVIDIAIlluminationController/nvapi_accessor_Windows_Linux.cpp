/*---------------------------------------------------------*\
| nvapi_accessor_Windows_Linux.cpp                          |
|                                                           |
|   NVAPI accessor for NVIDIA NVAPI illumination API        |
|                                                           |
|   Carter Miller (GingerRunner)                20 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "nvapi_accessor_Windows_Linux.h"

nvapi_accessor::nvapi_accessor(NV_PHYSICAL_GPU_HANDLE handle)
{
    this->handle = handle;
}

NV_STATUS nvapi_accessor::nvapi_zone_control(char nvapi_call, NV_GPU_CLIENT_ILLUM_ZONE_CONTROL_PARAMS* zone_control_struct)
{
    NV_STATUS ret = -1;

    if(nvapi_call == NVAPI_ZONE_SET_CONTROL)
    {
        ret = NvAPI_GPU_ClientIllumZonesSetControl(handle, zone_control_struct);
    }
    else if(nvapi_call == NVAPI_ZONE_GET_CONTROL)
    {
        ret = NvAPI_GPU_ClientIllumZonesGetControl(handle, zone_control_struct);
    }
    /*----------------------------------------------------------------------------------*\
    | Based off experimentation, the NvAPI doesn't like to be spammed calls              |
    | or else it just ignores them, this applies to both get/set control (GingerRunner)  |
    \*----------------------------------------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(NVAPI_CONTROL_BUFFER_TIME_MS));

    return(ret);
}
