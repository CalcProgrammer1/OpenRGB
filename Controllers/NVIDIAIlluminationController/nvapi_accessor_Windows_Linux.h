/*---------------------------------------------------------*\
| nvapi_accessor_Windows_Linux.h                            |
|                                                           |
|   NVAPI accessor for NVIDIA NVAPI illumination API        |
|                                                           |
|   Carter Miller (GingerRunner)                20 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "nvapi.h"

// NVAPI Direct Calls
#define NVAPI_ZONE_GET_CONTROL 0
#define NVAPI_ZONE_SET_CONTROL 1
#define NVAPI_CONTROL_BUFFER_TIME_MS 30

class nvapi_accessor
{
public:
    nvapi_accessor(NV_PHYSICAL_GPU_HANDLE handle);
    NV_STATUS nvapi_zone_control(char nvapi_call, NV_GPU_CLIENT_ILLUM_ZONE_CONTROL_PARAMS* zone_control_struct);

private:
    NV_PHYSICAL_GPU_HANDLE handle;
};
