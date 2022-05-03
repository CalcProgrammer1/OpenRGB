/*---------------------------------------------------------*\
|  Definitions for Corsair Hydro Series                     |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 8/17/2020        |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include <vector>
#ifdef __FreeBSD__
#include <libusb.h>
#else
#include <libusb-1.0/libusb.h>
#endif

#pragma once

enum
{
    CORSAIR_HYDRO_CMD_READ_PUMP_SPEED   = 0x31,
    CORSAIR_HYDRO_CMD_WRITE_PUMP_MODE   = 0x32,
    CORSAIR_HYDRO_CMD_READ_PUMP_MODE    = 0x33,
    CORSAIR_HYDRO_CMD_READ_FAN_SPEED    = 0x41,
    CORSAIR_HYDRO_CMD_READ_AIO_TEMP     = 0xA9,
    CORSAIR_HYDRO_CMD_READ_FIRMWARE     = 0xAA,
};

enum
{
    CORSAIR_HYDRO_PUMP_MODE_QUIET       = 0x00,
    CORSAIR_HYDRO_PUMP_MODE_BALANCED    = 0x01,
    CORSAIR_HYDRO_PUMP_MODE_PERFORMANCE = 0x02,
};

class CorsairHydroController
{
public:
    CorsairHydroController(libusb_device_handle* dev_handle);
    ~CorsairHydroController();

    unsigned char   GetFanPercent(unsigned char fan_channel);

    unsigned short  GetFanRPM(unsigned char fan_channel);

    std::string     GetFirmwareString();

    std::string     GetLocation();

    void            SetBlink
                        (
                            std::vector<RGBColor> & colors,
                            unsigned char           speed
                        );

    void            SetFixed
                        (
                            std::vector<RGBColor> & colors
                        );

    void            SetPulse
                        (
                            std::vector<RGBColor> & colors,
                            unsigned char           speed
                        );

    void            SetShift
                        (
                            std::vector<RGBColor> & colors,
                            unsigned char           speed
                        );    

private:
    libusb_device_handle*   dev;
    std::string             firmware_version;
    std::string             location;

    void            SendApplyBlink();
    void            SendApplyPulse();
    void            SendApplyShift();

    void            SendColors
                        (
                            std::vector<RGBColor> & colors
                        );

    void            SendFirmwareRequest();

    void            SendInit();

    void            SendSpeed
                        (
                            unsigned char           speed
                        );
};
