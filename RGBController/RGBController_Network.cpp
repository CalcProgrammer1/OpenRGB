/*---------------------------------------------------------*\
| RGBController_Network.cpp                                 |
|                                                           |
|   RGBController implementation that represents a remote   |
|   RGBController instance from a connected OpenRGB server  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>

#include "RGBController_Network.h"

RGBController_Network::RGBController_Network(NetworkClient * client_ptr, unsigned int dev_id_val)
{
    client  = client_ptr;
    dev_id  = dev_id_val;
}

unsigned int RGBController_Network::GetID()
{
    return(dev_id);
}

void RGBController_Network::ClearSegments(int zone)
{
    client->SendRequest_RGBController_ClearSegments(dev_id, zone);

    client->SendRequest_ControllerData(dev_id);
    client->WaitOnControllerData();
}

void RGBController_Network::AddSegment(int zone, segment new_segment)
{
    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    AccessMutex.lock_shared();

    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned int data_size                  = 0;

    /*-----------------------------------------------------*\
    | Calculate data size                                   |
    \*-----------------------------------------------------*/
    data_size                              += sizeof(data_size);
    data_size                              += sizeof(zone);
    data_size                              += GetSegmentDescriptionSize(new_segment, client->GetProtocolVersion());

    /*-----------------------------------------------------*\
    | Create data buffer                                    |
    \*-----------------------------------------------------*/
    unsigned char * data_buf                = new unsigned char[data_size];
    unsigned char * data_ptr                = data_buf;

    /*-----------------------------------------------------*\
    | Copy in data size                                     |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*-----------------------------------------------------*\
    | Copy in zone index                                    |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &zone, sizeof(zone));
    data_ptr += sizeof(zone);

    /*-----------------------------------------------------*\
    | Copy in segment description                           |
    \*-----------------------------------------------------*/
    data_ptr                                = GetSegmentDescriptionData(data_ptr, new_segment, client->GetProtocolVersion());

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    AccessMutex.unlock_shared();

    client->SendRequest_RGBController_AddSegment(dev_id, data_buf, data_size);

    delete[] data_buf;

    client->SendRequest_ControllerData(dev_id);
    client->WaitOnControllerData();
}

void RGBController_Network::ResizeZone(int zone, int new_size)
{
    client->SendRequest_RGBController_ResizeZone(dev_id, zone, new_size);

    client->SendRequest_ControllerData(dev_id);
    client->WaitOnControllerData();
}

void RGBController_Network::DeviceUpdateLEDs()
{
    unsigned char * data = CreateUpdateLEDsPacket(client->GetProtocolVersion());
    unsigned int size;

    memcpy(&size, &data[0], sizeof(unsigned int));

    client->SendRequest_RGBController_UpdateLEDs(dev_id, data, size);

    delete[] data;
}

void RGBController_Network::DeviceUpdateZoneLEDs(int zone)
{
    unsigned char * data = CreateUpdateZoneLEDsPacket(zone);
    unsigned int size;

    memcpy(&size, &data[0], sizeof(unsigned int));

    client->SendRequest_RGBController_UpdateZoneLEDs(dev_id, data, size);

    delete[] data;
}

void RGBController_Network::DeviceUpdateSingleLED(int led)
{
    unsigned char * data = CreateUpdateSingleLEDPacket(led);

    client->SendRequest_RGBController_UpdateSingleLED(dev_id, data, sizeof(int) + sizeof(RGBColor));

    delete[] data;
}

void RGBController_Network::SetCustomMode()
{
    client->SendRequest_RGBController_SetCustomMode(dev_id);

    client->SendRequest_ControllerData(dev_id);
    client->WaitOnControllerData();
}

void RGBController_Network::DeviceUpdateMode()
{
    unsigned char * data;
    unsigned int    size;

    data = CreateUpdateModePacket(active_mode, &size, client->GetProtocolVersion());

    client->SendRequest_RGBController_UpdateMode(dev_id, data, size);

    delete[] data;
}

void RGBController_Network::DeviceUpdateZoneMode(int zone)
{
    unsigned char * data;
    unsigned int    size;

    data = CreateUpdateZoneModePacket(zone, zones[zone].active_mode, &size, client->GetProtocolVersion());

    client->SendRequest_RGBController_UpdateZoneMode(dev_id, data, size);

    delete[] data;
}

void RGBController_Network::DeviceSaveMode()
{
    unsigned char * data;
    unsigned int    size;

    data = CreateUpdateModePacket(active_mode, &size, client->GetProtocolVersion());

    client->SendRequest_RGBController_SaveMode(dev_id, data, size);

    delete[] data;
}

/*-----------------------------------------------------*\
| This function overrides RGBController::UpdateLEDs()!  |
| Normally, UpdateLEDs() sets a flag for the updater    |
| thread to update the device asynchronously, which     |
| prevents delays updating local devices.  This causes  |
| instability and flickering with network devices though|
| so for the network implementation, process all updates|
| synchronously.                                        |
\*-----------------------------------------------------*/
void RGBController_Network::UpdateLEDs()
{
    AccessMutex.lock_shared();
    DeviceUpdateLEDs();
    AccessMutex.unlock_shared();
}

void RGBController_Network::UpdateMode()
{
    AccessMutex.lock_shared();
    DeviceUpdateMode();
    AccessMutex.unlock_shared();
}

unsigned char * RGBController_Network::CreateUpdateLEDsPacket(unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    AccessMutex.lock_shared();

    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned int data_size                  = 0;

    /*-----------------------------------------------------*\
    | Calculate data size                                   |
    \*-----------------------------------------------------*/
    data_size                              += sizeof(data_size);
    data_size                              += GetColorDescriptionSize(protocol_version);

    /*-----------------------------------------------------*\
    | Create data buffer                                    |
    \*-----------------------------------------------------*/
    unsigned char * data_buf                = new unsigned char[data_size];
    unsigned char * data_ptr                = data_buf;

    /*-----------------------------------------------------*\
    | Copy in data size                                     |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*-----------------------------------------------------*\
    | Copy in color data                                    |
    \*-----------------------------------------------------*/
    data_ptr                                = GetColorDescriptionData(data_ptr, protocol_version);

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    AccessMutex.unlock_shared();

    return(data_buf);
}

unsigned char * RGBController_Network::CreateUpdateModePacket(int mode_idx, unsigned int* size, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    AccessMutex.lock_shared();

    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned int data_size                  = 0;

    /*-----------------------------------------------------*\
    | Calculate data size                                   |
    \*-----------------------------------------------------*/
    data_size                              += sizeof(data_size);
    data_size                              += sizeof(mode_idx);
    data_size                              += GetModeDescriptionSize(modes[mode_idx], protocol_version);

    /*-----------------------------------------------------*\
    | Create data buffer                                    |
    \*-----------------------------------------------------*/
    unsigned char* data_buf                 = new unsigned char[data_size];
    unsigned char* data_ptr                 = data_buf;

    /*-----------------------------------------------------*\
    | Copy in data size                                     |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*-----------------------------------------------------*\
    | Copy in mode index                                    |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode_idx, sizeof(mode_idx));
    data_ptr += sizeof(mode_idx);

    /*-----------------------------------------------------*\
    | Copy in mode description                              |
    \*-----------------------------------------------------*/
    data_ptr = GetModeDescriptionData(data_ptr, modes[mode_idx], protocol_version);

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    AccessMutex.unlock_shared();

    *size = data_size;

    return(data_buf);
}

unsigned char * RGBController_Network::CreateUpdateSingleLEDPacket(int led)
{
    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    AccessMutex.lock_shared();

    /*-----------------------------------------------------*\
    | Fixed size descrption:                                |
    |       int:      LED index                             |
    |       RGBColor: LED color                             |
    \*-----------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[sizeof(int) + sizeof(RGBColor)];

    /*-----------------------------------------------------*\
    | Copy in LED index                                     |
    \*-----------------------------------------------------*/
    memcpy(&data_buf[0], &led, sizeof(int));

    /*-----------------------------------------------------*\
    | Copy in LED color                                     |
    \*-----------------------------------------------------*/
    memcpy(&data_buf[sizeof(led)], &colors[led], sizeof(RGBColor));

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    AccessMutex.unlock_shared();

    return(data_buf);
}

unsigned char * RGBController_Network::CreateUpdateZoneLEDsPacket(int zone)
{
    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    AccessMutex.lock_shared();

    unsigned int data_ptr   = 0;
    unsigned int data_size  = 0;

    unsigned short num_colors = zones[zone].leds_count;

    /*-----------------------------------------------------*\
    | Calculate data size                                   |
    \*-----------------------------------------------------*/
    data_size += sizeof(data_size);
    data_size += sizeof(zone);
    data_size += sizeof(num_colors);
    data_size += num_colors * sizeof(RGBColor);

    /*-----------------------------------------------------*\
    | Create data buffer                                    |
    \*-----------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*-----------------------------------------------------*\
    | Copy in data size                                     |
    \*-----------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*-----------------------------------------------------*\
    | Copy in zone index                                    |
    \*-----------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &zone, sizeof(zone));
    data_ptr += sizeof(zone);

    /*-----------------------------------------------------*\
    | Copy in number of colors (data)                       |
    \*-----------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_colors, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*-----------------------------------------------------*\
    | Copy in colors                                        |
    \*-----------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        /*-------------------------------------------------*\
        | Copy in color (data)                              |
        \*-------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &zones[zone].colors[color_index], sizeof(zones[zone].colors[color_index]));
        data_ptr += sizeof(zones[zone].colors[color_index]);
    }

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    AccessMutex.unlock_shared();

    return(data_buf);
}

unsigned char * RGBController_Network::CreateUpdateZoneModePacket(int zone_idx, int mode_idx, unsigned int* size, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    AccessMutex.lock_shared();

    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned int data_size                  = 0;

    /*-----------------------------------------------------*\
    | Calculate data size                                   |
    \*-----------------------------------------------------*/
    data_size                              += sizeof(data_size);
    data_size                              += sizeof(zone_idx);
    data_size                              += sizeof(mode_idx);

    if(mode_idx >= 0)
    {
        data_size                          += GetModeDescriptionSize(zones[zone_idx].modes[mode_idx], protocol_version);
    }

    /*-----------------------------------------------------*\
    | Create data buffer                                    |
    \*-----------------------------------------------------*/
    unsigned char* data_buf                 = new unsigned char[data_size];
    unsigned char* data_ptr                 = data_buf;

    /*-----------------------------------------------------*\
    | Copy in data size                                     |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*-----------------------------------------------------*\
    | Copy in zone index                                    |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &zone_idx, sizeof(zone_idx));
    data_ptr += sizeof(zone_idx);

    /*-----------------------------------------------------*\
    | Copy in mode index                                    |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode_idx, sizeof(mode_idx));
    data_ptr += sizeof(mode_idx);

    /*-----------------------------------------------------*\
    | Copy in mode description                              |
    \*-----------------------------------------------------*/
    if(mode_idx >= 0)
    {
        data_ptr = GetModeDescriptionData(data_ptr, zones[zone_idx].modes[mode_idx], protocol_version);
    }

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    AccessMutex.unlock_shared();

    *size = data_size;

    return(data_buf);
}
