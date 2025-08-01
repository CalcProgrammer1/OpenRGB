/*---------------------------------------------------------*\
| CorsairICueLinkProtocol.cpp                               |
|                                                           |
|   Driver for Corsair iCue Link System Hub                 |
|                                                           |
|   Aiden Vigue (acvigue)                       2 Mar 2025  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "CorsairICueLinkProtocol.h"

const CorsairICueLinkDevice* FindCorsairICueLinkDevice(unsigned char type, unsigned char model)
{
    for(size_t i = 0; i < sizeof(known_devices) / sizeof(known_devices[0]); i++)
    {
        if(known_devices[i].type == type && known_devices[i].model == model)
        {
            return(&known_devices[i]);
        }
    }

    return nullptr;
}
