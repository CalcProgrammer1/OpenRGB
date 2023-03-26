/*-------------------------------------------------------------------*\
|  Lenovo4ZoneUSBController.cpp                                       |
|                                                                     |
|  interface for Lenovo 4-Zones Devices                               |
\*-------------------------------------------------------------------*/

#include <iostream>
#include "Lenovo4ZoneUSBController.h"
#include "LogManager.h"

Lenovo4ZoneUSBController::Lenovo4ZoneUSBController(hid_device* dev_handle, const char* path, uint16_t in_pid)
{
    const uint8_t   sz      = HID_MAX_STR;
    wchar_t         tmp[sz];

    dev                     = dev_handle;
    location                = path;
    pid                     = in_pid;

    hid_get_manufacturer_string(dev, tmp, sz);
    std::wstring w_tmp      = std::wstring(tmp);
    name                    = std::string(w_tmp.begin(), w_tmp.end());

    hid_get_product_string(dev, tmp, sz);
    w_tmp                   = std::wstring(tmp);
    name.append(" ").append(std::string(w_tmp.begin(), w_tmp.end()));

    setDeviceSoftwareMode();
}

Lenovo4ZoneUSBController::~Lenovo4ZoneUSBController()
{
    hid_close(dev);
}

void Lenovo4ZoneUSBController::setMode(const KeyboardState &in_mode)
{
    uint8_t buffer[LENOVO_4_ZONE_HID_PACKET_SIZE];
    memcpy(buffer, &in_mode, LENOVO_4_ZONE_HID_PACKET_SIZE);
    hid_send_feature_report(dev, buffer, LENOVO_4_ZONE_HID_PACKET_SIZE);
}

uint16_t Lenovo4ZoneUSBController::getPid()
{
    return pid;
}

std::string Lenovo4ZoneUSBController::getName()
{
    return name;
}

std::string Lenovo4ZoneUSBController::getLocation()
{
    return location;
}


void Lenovo4ZoneUSBController::sendBasicInstruction(uint8_t )
{
}

void Lenovo4ZoneUSBController::setDeviceSoftwareMode()
{
    /*---------------------------------------*\
    | this is required for the device listen  |
    | to the software protocol                |
    \*---------------------------------------*/
    sendBasicInstruction(0xB2);
}

void Lenovo4ZoneUSBController::setDeviceHardwareMode()
{
    /*---------------------------------------*\
    |releases the device from sofware mode so |
    |that onboard controlls can be used       |
    |this has not been shown to happen between|
    |reboots                                  |
    \*---------------------------------------*/
    sendBasicInstruction(0xB1);
}
