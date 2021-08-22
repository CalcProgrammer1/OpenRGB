/*-----------------------------------------*\
|  CorsairK100Controller.h                  |
|                                           |
|  Driver for Corsair K100 Keyboard         |
\*-----------------------------------------*/

#ifndef CORSAIRK100CONTROLLER_H
#define CORSAIRK100CONTROLLER_H

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

typedef enum
{
    CORSAIR_TYPE_UNKNOWN            = 0,
    CORSAIR_TYPE_K100               = 1
} CorsairKeyboardType;

class CorsairK100Controller
{
public:
    CorsairK100Controller(hid_device* dev_handle, const char* path);
    ~CorsairK100Controller();
    CorsairKeyboardType             GetKeyboardType();
    std::string                     GetDeviceLocation();
    std::string                     GetFirmwareString();
    std::string                     GetName();
    std::string                     GetSerialString();

    void                            SetLEDs(std::vector<RGBColor> colors);
    void                            SetLEDsKeyboardLimited(std::vector<RGBColor> colors);
    void                            SetName(std::string device_name);

private:
    hid_device*             dev;

    std::string             firmware_version;
    std::string             location;
    std::string             name;
    device_type             type;
    CorsairKeyboardType     keyboard_type;

    void                    LightingControl();
    void                    SetLEDsKeyboardFull(std::vector<RGBColor> colors);
};

#endif // CORSAIRK100CONTROLLER_H
