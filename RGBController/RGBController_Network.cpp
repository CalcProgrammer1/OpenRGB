/*-----------------------------------------*\
|  RGBController_Network.cpp                |
|                                           |
|  Generic RGB Interface Network Class      |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/11/2020   |
\*-----------------------------------------*/

#include <cstring>

#include "RGBController_Network.h"

RGBController_Network::RGBController_Network(NetworkClient * client_ptr, unsigned int dev_idx_val)
{
    client  = client_ptr;
    dev_idx = dev_idx_val;
}

void RGBController_Network::SetupZones()
{
    //Don't send anything, this function should only process on host
}

void RGBController_Network::ResizeZone(int zone, int new_size)
{
    client->SendRequest_RGBController_ResizeZone(dev_idx, zone, new_size);
}

void RGBController_Network::DeviceUpdateLEDs()
{
    unsigned char * data = GetColorDescription();
    unsigned int size;

    memcpy(&size, &data[0], sizeof(unsigned int));

    client->SendRequest_RGBController_UpdateLEDs(dev_idx, data, size);

    delete[] data;
}

void RGBController_Network::UpdateZoneLEDs(int zone)
{
    unsigned char * data = GetZoneColorDescription(zone);
    unsigned int size;

    memcpy(&size, &data[0], sizeof(unsigned int));

    client->SendRequest_RGBController_UpdateZoneLEDs(dev_idx, data, size);

    delete[] data;
}

void RGBController_Network::UpdateSingleLED(int led)
{
    unsigned char * data = GetSingleLEDColorDescription(led);

    client->SendRequest_RGBController_UpdateSingleLED(dev_idx, data, sizeof(int) + sizeof(RGBColor));

    delete[] data;
}

void RGBController_Network::SetCustomMode()
{
    client->SendRequest_RGBController_SetCustomMode(dev_idx);

    client->SendRequest_ControllerData(dev_idx);
    client->WaitOnControllerData();
}

void RGBController_Network::DeviceUpdateMode()
{
    unsigned char * data = GetModeDescription(active_mode);
    unsigned int size;

    memcpy(&size, &data[0], sizeof(unsigned int));

    client->SendRequest_RGBController_UpdateMode(dev_idx, data, size);

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
    DeviceUpdateLEDs();
}
