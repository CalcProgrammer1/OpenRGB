/*---------------------------------------------------------*\
|  HYTEMousematController.h                                 |
|                                                           |
|  Definitions for HYTE CNVS RGB mousemat controller        |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 7/18/2023        |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <vector>

#ifdef __FreeBSD__
#include <libusb.h>
#else
#include <libusb-1.0/libusb.h>
#endif

/*---------------------------------------------------------*\
| HYTE CNVS endpoint values                                 |
\*---------------------------------------------------------*/
#define HYTE_CNVS_EP_IN     0x81
#define HYTE_CNVS_EP_OUT    0x01

class HYTEMousematController
{
public:
    HYTEMousematController(libusb_device_handle* dev_handle);
    ~HYTEMousematController();

    std::string     GetLocation();

    void            FirmwareAnimationControl(bool enabled);
    void            StreamingCommand(RGBColor* colors);

private:
    libusb_device_handle*   dev;
    std::string             location;
};
