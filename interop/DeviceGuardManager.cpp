/*---------------------------------------------------------*\
|  DeviceGuardManager.cpp                                   |
|                                                           |
|  Responsible for managing a DeviceGuard implementation,   |
|  allowing clients to wait for exclusive access to a       |
|  device using the DeviceGuardLock it provides.            |
|                                                           |
|  Evan Mulawski, 2023-09-05                                |
|                                                           |
\*---------------------------------------------------------*/

#include "DeviceGuardManager.h"

DeviceGuardManager::DeviceGuardManager(DeviceGuard* guard_ptr) : guard(guard_ptr)
{

}

DeviceGuardManager::~DeviceGuardManager()
{
    delete guard;
}

DeviceGuardLock DeviceGuardManager::AwaitExclusiveAccess()
{
    return DeviceGuardLock(*guard);
}
