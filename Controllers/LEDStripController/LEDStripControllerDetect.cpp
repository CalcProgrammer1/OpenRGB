/*---------------------------------------------------------*\
| LEDStripControllerDetect.cpp                              |
|                                                           |
|   Detector for serial LED strips                          |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      11 Dec 2016 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "LEDStripController.h"
#include "RGBController_LEDStrip.h"
#include "SettingsManager.h"
#include "LogManager.h"

/******************************************************************************************\
*                                                                                          *
*   DetectLEDStripControllers                                                              *
*                                                                                          *
*       Detect devices supported by the LEDStrip driver                                    *
*                                                                                          *
\******************************************************************************************/

void DetectLEDStripControllers()
{
    json                    ledstrip_settings;
    LEDStripDevice          dev;

    /*-------------------------------------------------*\
    | Get LED Strip settings from settings manager      |
    \*-------------------------------------------------*/
    ledstrip_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("LEDStripDevices");

    /*-------------------------------------------------*\
    | If the LEDStrip settings contains devices, process|
    \*-------------------------------------------------*/
    if(ledstrip_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < ledstrip_settings["devices"].size(); device_idx++)
        {
            if(ledstrip_settings["devices"][device_idx].contains("name"))
            {
                dev.name = ledstrip_settings["devices"][device_idx]["name"];
            }
            else
            {
                /*-------------------------------------------------*\
                | Default name                                      |
                \*-------------------------------------------------*/
                dev.name = "LED Strip";
            }

            if(ledstrip_settings["devices"][device_idx].contains("port"))
            {
                dev.port = ledstrip_settings["devices"][device_idx]["port"];
            }

            if(ledstrip_settings["devices"][device_idx].contains("baud"))
            {
                dev.baud = ledstrip_settings["devices"][device_idx]["baud"];
            }

            if(ledstrip_settings["devices"][device_idx].contains("num_leds"))
            {
                dev.num_leds = ledstrip_settings["devices"][device_idx]["num_leds"];
            }

            if(ledstrip_settings["devices"][device_idx].contains("protocol"))
            {
                std::string protocol_string = ledstrip_settings["devices"][device_idx]["protocol"];

                if(protocol_string == "keyboard_visualizer")
                {
                    dev.protocol = LED_PROTOCOL_KEYBOARD_VISUALIZER;
                }
                else if(protocol_string == "adalight")
                {
                    dev.protocol = LED_PROTOCOL_ADALIGHT;
                }
                else if(protocol_string == "tpm2")
                {
                    dev.protocol = LED_PROTOCOL_TPM2;
                }
                else if(protocol_string == "basic_i2c")
                {
                    dev.protocol = LED_PROTOCOL_BASIC_I2C;
                }
                else
                {
                    LOG_WARNING("[LEDStripController] '%s' is not a valid value for protocol", protocol_string.c_str());
                    return;
                }
            }
            else
            {
                /*-------------------------------------------------*\
                | Default to the Keyboard Visualizer protocol       |
                \*-------------------------------------------------*/
                dev.protocol = LED_PROTOCOL_KEYBOARD_VISUALIZER;
            }

            if(dev.port.empty())
            {
                LOG_WARNING("[LEDStripController] port value cannot be left empty.");
                return;
            }

            if(dev.baud <= 0)
            {
                LOG_WARNING("[LEDStripController] baud value cannot be left empty.");
                return;
            }

            if(dev.num_leds <= 0)
            {
                LOG_WARNING("[LEDStripController] num_leds value cannot be left empty.");
                return;
            }

            std::string value = dev.port + "," + std::to_string(dev.baud) + "," + std::to_string(dev.num_leds);

            LEDStripController*     controller     = new LEDStripController(dev.name);
            controller->Initialize((char *)value.c_str(), dev.protocol);

            RGBController_LEDStrip* rgb_controller = new RGBController_LEDStrip(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }

}   /* DetectLEDStripControllers() */

REGISTER_DETECTOR("LED Strip", DetectLEDStripControllers);
