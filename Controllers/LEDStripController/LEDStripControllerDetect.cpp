#include "Detector.h"
#include "LEDStripController.h"
#include "RGBController.h"
#include "RGBController_LEDStrip.h"
#include "SettingsManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

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
            /*-------------------------------------------------*\
            | Default to the Keyboard Visualizer protocol       |
            \*-------------------------------------------------*/
            dev.name     = "LED Strip";
            dev.protocol = LED_PROTOCOL_KEYBOARD_VISUALIZER;

            if(ledstrip_settings["devices"][device_idx].contains("name"))
            {
                dev.name = ledstrip_settings["devices"][device_idx]["name"];
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
            }

            std::string value = dev.port + "," + std::to_string(dev.baud) + "," + std::to_string(dev.num_leds);

            LEDStripController*     controller     = new LEDStripController();
            controller->Initialize((char *)value.c_str(), dev.protocol);

            RGBController_LEDStrip* rgb_controller = new RGBController_LEDStrip(controller);
            rgb_controller->name                   = dev.name;

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }

}   /* DetectLEDStripControllers() */

REGISTER_DETECTOR("LED Strip", DetectLEDStripControllers);
