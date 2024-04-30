/*-------------------------------------------------------------------*\
|  Lenovo4ZoneUSBController.h                                         |
|                                                                     |
|  interface for Lenovo 4-Zones Devices                               |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogManager.h"
#include "LenovoDevices4Zone.h"

#include <string>
#include <array>
#include <vector>
#include <utility>
#include <hidapi/hidapi.h>

#ifndef HID_MAX_STR
#define HID_MAX_STR                255
#endif

#define LENOVO_4_ZONE_HID_PACKET_SIZE     33

class Lenovo4ZoneUSBController
{
    public:
    /*--------------*\
    |ctor(s) and dtor|
    \*--------------*/
    Lenovo4ZoneUSBController(hid_device* dev_handle, const char* path, uint16_t in_pid);
    ~Lenovo4ZoneUSBController();

    void setMode(const KeyboardState &in_mode);

    /*--------------*\
    |device functions|
    \*--------------*/
    uint16_t getPid();
    std::string getName();
    std::string getLocation();

    private:
    /*--------------*\
    |data members    |
    \*--------------*/
    std::string name;
    hid_device *dev;
    std::string location;
    uint16_t pid;
    KeyboardState mode;

    /*--------------*\
    |device functions|
    \*--------------*/
    void sendBasicInstruction(uint8_t instruction);
};
