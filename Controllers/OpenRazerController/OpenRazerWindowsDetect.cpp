#include "Detector.h"
#include "RGBController.h"
#include "RGBController_OpenRazerWindows.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hid.h>

#ifdef _WIN64
#define OPENRAZERDLL            "OpenRazer64.dll"
#elif WIN32
#define OPENRAZERDLL            "OpenRazer.dll"
#endif

typedef struct
{
    struct device_attribute* dev_attr_list[44];
} device_fn_list_type;

/*---------------------------------------------------------*\
| This is a table of device attribute names.  It should     |
| always match the order of the entries in the structure    |
\*---------------------------------------------------------*/
static const char* device_fn_names[] =
{
    "device_type",
    "device_serial",
    "firmware_version",

    "matrix_custom_frame",
    "matrix_brightness",

    "matrix_effect_custom",
    "matrix_effect_none",
    "matrix_effect_static",
    "matrix_effect_breath",
    "matrix_effect_spectrum",
    "matrix_effect_reactive",
    "matrix_effect_wave",

    "logo_led_brightness",
    "logo_matrix_effect_none",
    "logo_matrix_effect_static",
    "logo_matrix_effect_breath",
    "logo_matrix_effect_spectrum",
    "logo_matrix_effect_reactive",

    "scroll_led_brightness",
    "scroll_matrix_effect_none",
    "scroll_matrix_effect_static",
    "scroll_matrix_effect_breath",
    "scroll_matrix_effect_spectrum",
    "scroll_matrix_effect_reactive",

    "left_led_brightness",
    "left_matrix_effect_none",
    "left_matrix_effect_static",
    "left_matrix_effect_breath",
    "left_matrix_effect_spectrum",
    "left_matrix_effect_reactive",
    "left_matrix_effect_wave",

    "right_led_brightness",
    "right_matrix_effect_none",
    "right_matrix_effect_static",
    "right_matrix_effect_breath",
    "right_matrix_effect_spectrum",
    "right_matrix_effect_reactive",
    "right_matrix_effect_wave",

    "logo_led_effect",
    "logo_led_rgb",
    "logo_led_state",

    "scroll_led_effect",
    "scroll_led_rgb",
    "scroll_led_state"
};

/*---------------------------------------------------------*\
| This function searches the device attribute list of a     |
| given device to fill in a device_fn_type structure        |
\*---------------------------------------------------------*/
static void load_device_fn(device_fn_type* device_fn, device* dev)
{
    memset(device_fn, 0, sizeof(device_fn_type));

    for (int table_idx = 0; table_idx < 44; table_idx++)
    {
        for (int list_idx = 0; list_idx < dev->attr_count; list_idx++)
        {
            if (strcmp(device_fn_names[table_idx], dev->attr_list[list_idx]->name) == 0)
            {
                ((device_fn_list_type*)device_fn)->dev_attr_list[table_idx] = dev->attr_list[list_idx];
            }
        }
    }
}

/******************************************************************************************\
*                                                                                          *
*   DetectOpenRazerControllers                                                             *
*                                                                                          *
*       Detect devices supported by the OpenRazer kernel drivers                           *
*                                                                                          *
\******************************************************************************************/

void DetectOpenRazerControllers(std::vector<RGBController*> &rgb_controllers)
{
    static HMODULE module = LoadLibrary(OPENRAZERDLL);

	if(module == nullptr)
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Map DLL functions                                         |
    \*---------------------------------------------------------*/
	typedef unsigned int(*INITRAZERDRIVER)(struct hid_device** hdev);
	
    INITRAZERDRIVER init_razer_kbd_driver = reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(module, "init_razer_kbd_driver"));
    INITRAZERDRIVER init_razer_mousemat_driver = reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(module, "init_razer_mousemat_driver"));
	INITRAZERDRIVER init_razer_mouse_driver = reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(module, "init_razer_mouse_driver"));
	INITRAZERDRIVER init_razer_accessory_driver = reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(module, "init_razer_accessory_driver"));
    INITRAZERDRIVER init_razer_kraken_driver = reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(module, "init_razer_kraken_driver"));
    INITRAZERDRIVER init_razer_core_driver = reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(module, "init_razer_core_driver"));

    /*---------------------------------------------------------*\
    | Initialize all OpenRazer driver modules and store devices |
    \*---------------------------------------------------------*/
    struct hid_device* hdev;
	unsigned int num;

    if(init_razer_kbd_driver != NULL)
    {
        hdev = NULL;
        num = init_razer_kbd_driver(&hdev);
        for (unsigned int i = 0; i < num; i++)
        {
            if (hdev[i].dev.attr_count < 1) continue;

            device_fn_type* device_fn = new device_fn_type;
            load_device_fn(device_fn, &hdev[i].dev);

            RGBController_OpenRazer * razer_rgb = new RGBController_OpenRazer(&hdev[i].dev, device_fn);

            if(razer_rgb->device_index != -1)
            {
                rgb_controllers.push_back(razer_rgb);
            }
            else
            {
                delete razer_rgb;
            }
        }
    }

    if(init_razer_mouse_driver != NULL)
    {
        hdev = NULL;
        num = init_razer_mouse_driver(&hdev);
        for (unsigned int i = 0; i < num; i++)
        {
            if (hdev[i].dev.attr_count < 1) continue;

            device_fn_type* device_fn = new device_fn_type;
            load_device_fn(device_fn, &hdev[i].dev);

            RGBController_OpenRazer * razer_rgb = new RGBController_OpenRazer(&hdev[i].dev, device_fn);

            if(razer_rgb->device_index != -1)
            {
                rgb_controllers.push_back(razer_rgb);
            }
            else
            {
                delete razer_rgb;
            }
        }
    }

    if(init_razer_mousemat_driver != NULL)
    {
        hdev = NULL;
        num = init_razer_mousemat_driver(&hdev);
        for (unsigned int i = 0; i < num; i++)
        {
            if (hdev[i].dev.attr_count < 1) continue;

            device_fn_type* device_fn = new device_fn_type;
            load_device_fn(device_fn, &hdev[i].dev);

            RGBController_OpenRazer * razer_rgb = new RGBController_OpenRazer(&hdev[i].dev, device_fn);

            if(razer_rgb->device_index != -1)
            {
                rgb_controllers.push_back(razer_rgb);
            }
            else
            {
                delete razer_rgb;
            }
        }
    }

    if(init_razer_accessory_driver != NULL)
    {
        hdev = NULL;
        num = init_razer_accessory_driver(&hdev);
        for (unsigned int i = 0; i < num; i++)
        {
            if (hdev[i].dev.attr_count < 1) continue;

            device_fn_type* device_fn = new device_fn_type;
            load_device_fn(device_fn, &hdev[i].dev);

            RGBController_OpenRazer * razer_rgb = new RGBController_OpenRazer(&hdev[i].dev, device_fn);

            if(razer_rgb->device_index != -1)
            {
                rgb_controllers.push_back(razer_rgb);
            }
            else
            {
                delete razer_rgb;
            }
        }
    }

    if(init_razer_kraken_driver != NULL)
    {
        hdev = NULL;
        num = init_razer_kraken_driver(&hdev);
        for (unsigned int i = 0; i < num; i++)
        {
            if (hdev[i].dev.attr_count < 1) continue;

            device_fn_type* device_fn = new device_fn_type;
            load_device_fn(device_fn, &hdev[i].dev);

            RGBController_OpenRazer * razer_rgb = new RGBController_OpenRazer(&hdev[i].dev, device_fn);

            if(razer_rgb->device_index != -1)
            {
                rgb_controllers.push_back(razer_rgb);
            }
            else
            {
                delete razer_rgb;
            }
        }
    }

    if(init_razer_core_driver != NULL)
    {
        hdev = NULL;
        num = init_razer_core_driver(&hdev);
        for (unsigned int i = 0; i < num; i++)
        {
            if (hdev[i].dev.attr_count < 1) continue;

            device_fn_type* device_fn = new device_fn_type;
            load_device_fn(device_fn, &hdev[i].dev);

            RGBController_OpenRazer * razer_rgb = new RGBController_OpenRazer(&hdev[i].dev, device_fn);

            if(razer_rgb->device_index != -1)
            {
                rgb_controllers.push_back(razer_rgb);
            }
            else
            {
                delete razer_rgb;
            }
        }
    }

}   /* DetectOpenRazerControllers() */

REGISTER_DETECTOR("OpenRazer-Win32", DetectOpenRazerControllers);
