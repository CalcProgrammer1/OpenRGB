/*---------------------------------------------------------*\
|  DeviceGuard.h                                            |
|                                                           |
|  An abstract interface for acquiring and releasing        |
|  a guard on a device, serving as a foundation for         |
|  implementing device-specific synchronization mechanisms. |
|                                                           |
|  Evan Mulawski, 2023-09-05                                |
|                                                           |
\*---------------------------------------------------------*/

#pragma once

class DeviceGuard
{
public:
    DeviceGuard();
    virtual ~DeviceGuard();

    virtual void Acquire() = 0;
    virtual void Release() = 0;
};
