/*-----------------------------------------*\
|  CorsairPeripheralController.h            |
|                                           |
|  Definitions and types for Corsair RGB    |
|  keyboard, mouse, and mousemat lighting   |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/9/2020    |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    CORSAIR_COMMAND_WRITE       = 0x07,
    CORSAIR_COMMAND_READ        = 0x0E,
    CORSAIR_COMMAND_STREAM      = 0x7F
};

enum
{
    CORSAIR_PROPERTY_FIRMWARE_INFO              = 0x01,
    CORSAIR_PROPERTY_RESET                      = 0x02,
    CORSAIR_PROPERTY_SPECIAL_FUNCTION           = 0x04,
    CORSAIR_PROPERTY_LIGHTING_CONTROL           = 0x05,
    CORSAIR_PROPERTY_HARDWARE_PROFILE           = 0x13,
    CORSAIR_PROPERTY_SUBMIT_MOUSE_COLOR         = 0x22,
    CORSAIR_PROPERTY_SUBMIT_KEYBOARD_COLOR_9    = 0x27,
    CORSAIR_PROPERTY_SUBMIT_KEYBOARD_COLOR_24   = 0x28,
};

enum
{
    CORSAIR_LIGHTING_CONTROL_HARDWARE           = 0x01,
    CORSAIR_LIGHTING_CONTROL_SOFTWARE           = 0x02
};

enum
{
    CORSAIR_COLOR_CHANNEL_RED       = 0x01,
    CORSAIR_COLOR_CHANNEL_GREEN     = 0x02,
    CORSAIR_COLOR_CHANNEL_BLUE      = 0x03
};

class CorsairPeripheralController
{
public:
    CorsairPeripheralController(hid_device* dev_handle);
    ~CorsairPeripheralController();

    device_type     GetDeviceType();
    std::string     GetFirmwareString();

    void            SetLEDs(std::vector<RGBColor> colors);
    void            SetLEDsKeyboardFull(std::vector<RGBColor> colors);
    void            SetLEDsKeyboardLimited(std::vector<RGBColor> colors);
    void            SetLEDsMouse(std::vector<RGBColor> colors);
    void            SetLEDsMousemat(std::vector<RGBColor> colors);

private:
    hid_device*             dev;

    std::string             firmware_version;
    device_type             type;

    void    LightingControl();
    void    SpecialFunctionControl();

    void    ReadFirmwareInfo();
    
    void    StreamPacket
                (
                unsigned char   packet_id,
                unsigned char   data_sz,
                unsigned char*  data_ptr
                );

    void    SubmitKeyboardFullColors
                (
                unsigned char   color_channel,
                unsigned char   packet_count,
                unsigned char   finish_val
                );

    void    SubmitKeyboardLimitedColors
                (
                unsigned char   byte_count
                );

    void    SubmitMouseColors
                (
                unsigned char   num_zones,
                RGBColor *      color_data
                );

    void    SubmitMousematColors
            (
            unsigned char   num_zones,
            RGBColor *      color_data
            );
};
