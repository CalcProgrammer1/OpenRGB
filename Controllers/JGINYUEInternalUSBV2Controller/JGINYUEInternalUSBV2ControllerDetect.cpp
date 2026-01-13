/*---------------------------------------------------------*\
| JGINYUEInternalUSBV2ControllerDetect.cpp                  |
|                                                           |
|   Detector for JGINYUE USB motherboard V2                 |
|                                                           |
|   Tong R (tcr020)                             06 Aug 2024 |
|   Liu ShiMeng(Moon dream stars)               09 Aug 2023 |
|   Dongguan Yonghang Electronic Technology Co., Ltd        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <hidapi.h>
#include "DetectionManager.h"
#include "dmiinfo.h"
#include "find_usb_serial_port.h"
#include "JGINYUEInternalUSBV2Controller.h"
#include "LogManager.h"
#include "RGBController_JGINYUEInternalUSBV2.h"
#include "RGBController.h"
#include "serial_port.h"

/*---------------------------------------------------------*\
| JGINYUE vendor ID                                         |
\*---------------------------------------------------------*/
#define JGINYUE_VID_V2                                 0x1A86

/*---------------------------------------------------------*\
| JGINYUE product ID                                        |
\*---------------------------------------------------------*/
#define JGINYUE_MOTHERBOARD_PID_V2                     0xE30B

DetectedControllers DetectJGINYUEInternalUSBV2Controller(hid_device_info* info,const std::string& /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;
    DMIInfo             dmi_info;
    std::string         manufacturer = dmi_info.getManufacturer();

    std::transform(manufacturer.begin(), manufacturer.end(), manufacturer.begin(), ::toupper);

    if(manufacturer.find("JGINYUE") != std::string::npos)
    {
        LOG_INFO("[JGINYUEInternalUSBV2ControllerDetect] Pass manufacture name check.Start to init HID and CDC interface");

        if(dev)
        {
            serial_port *port = nullptr;
            std::vector<std::string*> serial_ports = find_usb_serial_port(JGINYUE_VID_V2, JGINYUE_MOTHERBOARD_PID_V2);

            if(serial_ports.size() == 0)
            {
                LOG_WARNING("[JGINYUEInternalUSBV2ControllerDetect] JGINYUE device found but no serial port detected - Direct mode will be unavailable");
            }
            else if(serial_ports.size() > 1)
            {
                LOG_WARNING("[JGINYUEInternalUSBV2ControllerDetect] Multiple serial ports found for JGINYUE device, using first one");
            }

            if(serial_ports.size() >= 1)
            {
                port = new serial_port();
                if(!port->serial_open(serial_ports[0]->c_str(), 115200))
                {
                    LOG_WARNING("[JGINYUEInternalUSBV2ControllerDetect] Failed to open serial port %s - Direct mode will be unavailable. HID modes will still work.", serial_ports[0]->c_str());
                    delete port;
                    port = nullptr;
                }
            }

            /*---------------------------------------------*\
            | Clean up serial port string vector            |
            \*---------------------------------------------*/
            for(std::string* str_ptr : serial_ports)
            {
                delete str_ptr;
            }

            JGINYUEInternalUSBV2Controller *     controller     = new JGINYUEInternalUSBV2Controller(dev, info->path, port);
            RGBController_JGINYUEInternalUSBV2 * rgb_controller = new RGBController_JGINYUEInternalUSBV2(controller);

            detected_controllers.push_back(rgb_controller);
        }
    }
    else
    {
        LOG_INFO("[JGINYUEInternalUSBV2ControllerDetect] JGINYUE Internal USB ControllerV2 not found,error manufacturer name:%s",manufacturer.c_str());
        hid_close(dev);
    }

    return(detected_controllers);
}

#ifdef _WIN32
REGISTER_HID_DETECTOR("JGINYUE Internal USB ControllerV2", DetectJGINYUEInternalUSBV2Controller, JGINYUE_VID_V2, JGINYUE_MOTHERBOARD_PID_V2);
#else
REGISTER_HID_DETECTOR_IPU("JGINYUE Internal USB ControllerV2", DetectJGINYUEInternalUSBV2Controller, JGINYUE_VID_V2, JGINYUE_MOTHERBOARD_PID_V2, 0, 0xFF00, 1);
#endif
