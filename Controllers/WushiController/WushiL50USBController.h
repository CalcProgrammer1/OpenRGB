/*-------------------------------------------------------------------*\
|  WushiL50USBController.h                                         |
|                                                                     |
|  interface for Wushi L50 Devices                               |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogManager.h"
#include "WushiDevicesL50.h"

#include <string>
#include <array>
#include <vector>
#include <utility>
#include <hidapi/hidapi.h>

#ifndef HID_MAX_STR
#define HID_MAX_STR                255
#endif

#define WUSHIL50_HID_PACKET_SIZE     33

class WushiL50USBController
{
    public:
    /*--------------*\
    |ctor(s) and dtor|
    \*--------------*/
    WushiL50USBController(hid_device* dev_handle, const char* path, uint16_t in_pid);
    ~WushiL50USBController();

    void setMode(const KeyboardState &in_mode);

    /*--------------*\
    |device functions|
    \*--------------*/
    uint16_t getPid();
    std::string getName();
    std::string getLocation();
    std::string GetSerialString();
    std::string GetFirmwareVersion();

    void setDeviceSoftwareMode();
    void setDeviceHardwareMode();

    private:
    /*--------------*\
    |data members    |
    \*--------------*/
    std::string name;
    hid_device *dev;
    std::string location;
    std::string serial_number;
    uint16_t pid;
    KeyboardState mode;
    std::string version;
    /*--------------*\
    |device functions|
    \*--------------*/
    void sendBasicInstruction(uint8_t instruction);
};
