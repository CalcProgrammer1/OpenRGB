/*---------------------------------------------------------*\
| RGBController_CMKeyboardController.cpp                    |
|                                                           |
|   RGBController for Cooler Master keyboards               |
|                                                           |
|   Tam D (too.manyhobbies)                     30 Nov 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_CMKeyboardController.h"
#include "CMKeyboardDevices.h"

/**------------------------------------------------------------------*\
    @name Coolermaster Masterkeys Keyboards
    @category Keyboard
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterV1Keyboards,DetectCoolerMasterV2Keyboards
    @comment
    In CMKeyboardV1Controller brightness control not supported.
    Supported effects differ between CMKeyboardV1Controller and
    CMKeyboardV2Controller.
\*-------------------------------------------------------------------*/
RGBController_CMKeyboardController::RGBController_CMKeyboardController(CMKeyboardAbstractController* pController)
{
    m_pController       = pController;
    vendor              = m_pController->GetDeviceVendor();
    type                = DEVICE_TYPE_KEYBOARD;
    description         = "Cooler Master Keyboard Device";
    version             = m_pController->GetFirmwareVersion();

    /*----------------------------------------------------------------*\
    | Coolermaster uses the name field to store the serial number in   |
    | many of their keyboards.                                         |
    \*----------------------------------------------------------------*/
    serial              = m_pController->GetDeviceSerial();
    location            = m_pController->GetLocation();
    m_keyboardLayout    = m_pController->GetKeyboardLayout();
    name                = m_pController->GetDeviceName();

    m_pController->InitializeModes(modes);

    SetupZones();
}

RGBController_CMKeyboardController::~RGBController_CMKeyboardController()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            if(zones[zone_index].matrix_map->map != NULL)
            {
                delete zones[zone_index].matrix_map->map;
            }

            delete zones[zone_index].matrix_map;
        }
    }

    if(m_pController)
    {
        delete m_pController;
    }
}

#define COOLERMASTER_ZONES_MAX 1
void RGBController_CMKeyboardController::SetupZones()
{
    std::string physical_size;
    unsigned int max_led_value              = 0;
    const cm_kb_device* coolermaster        = m_pController->GetDeviceData();

    /*---------------------------------------------------------*\
    | Fill in zones from the device data                        |
    \*---------------------------------------------------------*/
    for(size_t i = 0; i < COOLERMASTER_ZONES_MAX; i++)
    {
        if(coolermaster->zones[i] == NULL)
        {
            break;
        }
        else
        {
            zone new_zone;

            new_zone.name                   = coolermaster->zones[i]->name;
            new_zone.type                   = coolermaster->zones[i]->type;

            if(new_zone.type == ZONE_TYPE_MATRIX)
            {
                KeyboardLayoutManager new_kb(m_keyboardLayout, coolermaster->layout_new->base_size, coolermaster->layout_new->key_values);

                matrix_map_type * new_map   = new matrix_map_type;
                new_zone.matrix_map         = new_map;

                if(coolermaster->layout_new->base_size != KEYBOARD_SIZE_EMPTY)
                {
                    /*---------------------------------------------------------*\
                    | Minor adjustments to keyboard layout                      |
                    \*---------------------------------------------------------*/
                    keyboard_keymap_overlay_values* temp    = coolermaster->layout_new;
                    new_kb.ChangeKeys(*temp);

                    new_map->height             = new_kb.GetRowCount();
                    new_map->width              = new_kb.GetColumnCount();
                    new_map->map                = new unsigned int[new_map->height * new_map->width];

                    /*---------------------------------------------------------*\
                    | Matrix map still uses declared zone rows and columns      |
                    |   as the packet structure depends on the matrix map       |
                    \*---------------------------------------------------------*/
                    new_kb.GetKeyMap(new_map->map, KEYBOARD_MAP_FILL_TYPE_COUNT, new_map->height, new_map->width);

                    /*---------------------------------------------------------*\
                    | Create LEDs for the Matrix zone                           |
                    |   Place keys in the layout to populate the matrix         |
                    \*---------------------------------------------------------*/
                    new_zone.leds_count                     = new_kb.GetKeyCount();
                    LOG_DEBUG("[%s] Created KB matrix with %d rows and %d columns containing %d keys",
                              m_pController->GetDeviceName().c_str(), new_kb.GetRowCount(), new_kb.GetColumnCount(), new_zone.leds_count);

                    for(unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
                    {
                        led new_led;

                        new_led.name                = new_kb.GetKeyNameAt(led_idx);
                        new_led.value               = new_kb.GetKeyValueAt(led_idx);
                        max_led_value               = std::max(max_led_value, new_led.value);
                        leds.push_back(new_led);
                    }
                }

                /*---------------------------------------------------------*\
                | Add 1 the max_led_value to account for the 0th index      |
                \*---------------------------------------------------------*/
                max_led_value++;
            }

            /*---------------------------------------------------------*\
            | name is not set yet so description is used instead        |
            \*---------------------------------------------------------*/
            LOG_DEBUG("[%s] Creating a %s zone: %s with %d LEDs", description.c_str(),
                      ((new_zone.type == ZONE_TYPE_MATRIX) ? "matrix": "linear"),
                      new_zone.name.c_str(), new_zone.leds_count);
            new_zone.leds_min                   = new_zone.leds_count;
            new_zone.leds_max                   = new_zone.leds_count;
            zones.push_back(new_zone);

        }
    }


    SetupColors();
}

void RGBController_CMKeyboardController::ResizeZone(int /*zone*/, int /*new_size*/)
{
}

void RGBController_CMKeyboardController::DeviceUpdateLEDs()
{
    m_pController->SetLeds(leds, colors);
}

void RGBController_CMKeyboardController::UpdateSingleLED(int led, RGBColor color)
{
    uint8_t key_value = m_pLayoutManager->GetKeyValueAt(led);
    m_pController->SetSingleLED(key_value, color);
}

void RGBController_CMKeyboardController::UpdateSingleLED(int led)
{
    m_pController->SetSingleLED(led, colors[led]);
}

void RGBController_CMKeyboardController::UpdateZoneLEDs(int /*zone_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMKeyboardController::DeviceUpdateMode()
{
    m_pController->SetMode(modes[active_mode]);
}

void RGBController_CMKeyboardController::SetCustomMode()
{

}
