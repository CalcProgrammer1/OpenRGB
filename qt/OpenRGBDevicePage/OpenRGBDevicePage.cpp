/*---------------------------------------------------------*\
| OpenRGBDevicePage.cpp                                     |
|                                                           |
|   User interface for OpenRGB device page                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBDevicePage.h"
#include "OpenRGBZoneResizeDialog.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "ui_OpenRGBDevicePage.h"

static void UpdateCallback(void * this_ptr)
{
    OpenRGBDevicePage * this_obj = (OpenRGBDevicePage *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "UpdateInterface", Qt::QueuedConnection);
}

QString OpenRGBDevicePage::ModeDescription(const mode& m)
{
    /*-----------------------------------------------------------------*\
    | List of common mode names can be found on the OpenRGB Wiki:       |
    | https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/Common-Modes   |
    \*-----------------------------------------------------------------*/
    static const std::unordered_map<std::string, QString> descriptions =
    {
        {"Direct",          tr("Set individual LEDs to static colors.  Safe for use with software-driven effects.")                     },
        {"Custom",          tr("Set individual LEDs to static colors.  Not safe for use with software-driven effects.")                 },
        {"Static",          tr("Sets the entire device or a zone to a single color.")                                                   },
        {"Breathing",       tr("Gradually fades between fully off and fully on.")                                                       },
        {"Flashing",        tr("Abruptly changes between fully off and fully on.")                                                      },
        {"Spectrum Cycle",  tr("Gradually cycles through the entire color spectrum.  All lights on the device are the same color.")     },
        {"Rainbow Wave",    tr("Gradually cycles through the entire color spectrum.  Produces a rainbow pattern that moves.")           },
        {"Reactive",        tr("Flashes lights when keys or buttons are pressed.")                                                      },
    };

    /*-----------------------------------------------------------------*\
    | Find the given mode name in the list and return the description   |
    | if it exists, otherwise return an empty string                    |
    \*-----------------------------------------------------------------*/
    std::unordered_map<std::string, QString>::const_iterator it = descriptions.find(m.name);

    if(it != descriptions.end())
    {
        return it->second;
    }

    return "";
}

OpenRGBDevicePage::OpenRGBDevicePage(RGBController *dev, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBDevicePage)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Store device pointer                                  |
    \*-----------------------------------------------------*/
    device = dev;

    /*-----------------------------------------------------*\
    | Register update callback with the device              |
    \*-----------------------------------------------------*/
    device->RegisterUpdateCallback(UpdateCallback, this);

    /*-----------------------------------------------------*\
    | Set up the device view                                |
    \*-----------------------------------------------------*/
    connect(ui->DeviceViewBox, &DeviceView::selectionChanged, this, &OpenRGBDevicePage::on_DeviceViewBox_selectionChanged);

    /*-----------------------------------------------------*\
    | Get the UserInterface settings and check the          |
    | numerical labels and hex format settings              |
    \*-----------------------------------------------------*/
    SettingsManager*    settings_manager    = ResourceManager::get()->GetSettingsManager();
    std::string         ui_string           = "UserInterface";
    json                ui_settings;

    ui_settings = settings_manager->GetSettings(ui_string);

    if(ui_settings.contains("numerical_labels"))
    {
        bool            numerical_labels    = ui_settings["numerical_labels"];

        ui->DeviceViewBox->setNumericalLabels(numerical_labels);
    }

    if(ui_settings.contains("hex_format"))
    {
        if(ui_settings["hex_format"] == "RGB")
        {
            HexFormatRGB = true;
        }
        else if(ui_settings["hex_format"] == "BGR")
        {
            HexFormatRGB = false;
        }
    }

    ui->DeviceViewBox->setController(device);
    ui->DeviceViewBoxFrame->hide();

    /*-----------------------------------------------------*\
    | Fill in the mode selection box                        |
    \*-----------------------------------------------------*/
    ui->ModeBox->blockSignals(true);
    ui->ModeBox->clear();

    for(std::size_t i = 0; i < device->modes.size(); i++)
    {
        ui->ModeBox->addItem(device->GetModeName((unsigned int)i).c_str());
        ui->ModeBox->setItemData((int)i, ModeDescription(device->modes[i]), Qt::ToolTipRole);
    }

    ui->ModeBox->setCurrentIndex(device->GetMode());
    ui->ModeBox->blockSignals(false);

    /*-----------------------------------------------------*\
    | Update mode user interface elements                   |
    \*-----------------------------------------------------*/
    UpdateModeUi();

    /*-----------------------------------------------------*\
    | Set initial color to black and update UI              |
    \*-----------------------------------------------------*/
    current_color.setRgb(0, 0, 0);
    updateColorUi();

    ui->ApplyColorsButton->setDisabled(autoUpdateEnabled());
    ui->SetAllButton->setDisabled(device->modes[0].name != "Direct" && device->modes[0].name != "Custom" && device->modes[0].name != "Static");
}

OpenRGBDevicePage::~OpenRGBDevicePage()
{
    /*-----------------------------------------------------*\
    | Unregister update callback from the controller if the |
    | controller still exists                               |
    \*-----------------------------------------------------*/
    for(unsigned int controller_idx = 0; controller_idx < ResourceManager::get()->GetRGBControllers().size(); controller_idx++)
    {
        if(ResourceManager::get()->GetRGBControllers()[controller_idx] == device)
        {
            device->UnregisterUpdateCallback(this);
            break;
        }
    }

    delete ui;
}

void OpenRGBDevicePage::changeEvent(QEvent *event)
{
    /*-----------------------------------------------------*\
    | Retranslate the UI when a language change event occurs|
    \*-----------------------------------------------------*/
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

RGBController* OpenRGBDevicePage::GetController()
{
    return device;
}

void OpenRGBDevicePage::on_ZoneBox_currentIndexChanged(int index)
{
    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();

    /*-----------------------------------------------------*\
    | Process zone box change based on color mode           |
    \*-----------------------------------------------------*/
    switch(device->modes[selected_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
            {
                /*-----------------------------------------*\
                | Initialize both selected zone and segment |
                | to -1 to indicate there is no selection   |
                \*-----------------------------------------*/
                unsigned int    current_index       = 0;
                bool            selected_all_zones  = false;
                int             selected_zone       = -1;
                int             selected_segment    = -1;

                /*-----------------------------------------*\
                | Handle condition where device has more    |
                | than one zone, which adds an "All Zones"  |
                | entry to the Zone menu in the first index |
                \*-----------------------------------------*/
                if(device->zones.size() > 1)
                {
                    if(index == (int)current_index)
                    {
                        selected_all_zones = true;
                    }

                    current_index++;
                }

                /*-----------------------------------------*\
                | Determine selected zone and optionally    |
                | selected segment based on index if "All   |
                | Zones" is not the selected index          |
                \*-----------------------------------------*/
                if(!selected_all_zones)
                {
                    for(std::size_t zone_idx = 0; zone_idx < device->zones.size(); zone_idx++)
                    {
                        if(index == (int)current_index)
                        {
                            selected_zone = (int)zone_idx;
                            break;
                        }

                        current_index++;

                        for(std::size_t segment_idx = 0; segment_idx < device->zones[zone_idx].segments.size(); segment_idx++)
                        {
                            if(index == (int)current_index)
                            {
                                selected_zone    = (int)zone_idx;
                                selected_segment = (int)segment_idx;
                                break;
                            }

                            current_index++;
                        }

                        if(selected_segment != -1)
                        {
                            break;
                        }
                    }
                }

                /*-----------------------------------------*\
                | Clear LED box                             |
                \*-----------------------------------------*/
                ui->LEDBox->blockSignals(true);
                ui->LEDBox->clear();

                /*-----------------------------------------*\
                | Clear multiple selected flag              |
                \*-----------------------------------------*/
                MultipleSelected = false;

                /*-----------------------------------------*\
                | Handle "All Zones" selected condition or  |
                | multiple selection condition              |
                \*-----------------------------------------*/
                if(selected_all_zones)
                {
                    /*-------------------------------------*\
                    | If there are multiple LEDs, add the   |
                    | "Entire Device" option to the LED box |
                    | and enable it, otherwise there is     |
                    | only one LED so disable it            |
                    \*-------------------------------------*/
                    if(device->leds.size() > 1)
                    {
                        ui->LEDBox->addItem(tr("Entire Device"));
                        ui->LEDBox->setEnabled(1);
                    }
                    else
                    {
                        ui->LEDBox->setDisabled(1);
                    }

                    /*-------------------------------------*\
                    | Fill in the LED list with all LEDs in |
                    | the device                            |
                    \*-------------------------------------*/
                    for(std::size_t i = 0; i < device->leds.size(); i++)
                    {
                        ui->LEDBox->addItem(device->GetLEDName((unsigned int)i).c_str());
                    }

                    /*-------------------------------------*\
                    | Editing is not allowed when all       |
                    | zones are selected at once            |
                    \*-------------------------------------*/
                    ui->EditZoneButton->setEnabled(false);

                    if(!ui->ZoneBox->signalsBlocked())
                    {
                        ui->DeviceViewBox->blockSignals(true);
                        ui->DeviceViewBox->clearSelection();
                        ui->DeviceViewBox->blockSignals(false);
                    }
                }

                /*-----------------------------------------*\
                | Handle a zone selected condition          |
                \*-----------------------------------------*/
                else if(selected_zone != -1 && selected_segment == -1)
                {
                    unsigned int leds_in_zone = device->GetLEDsInZone(selected_zone);

                    /*-------------------------------------*\
                    | If there are multiple LEDs, add the   |
                    | "Entire Zone" option to the LED box   |
                    | and enable it, otherwise there is     |
                    | only one LED so disable it            |
                    \*-------------------------------------*/
                    if(leds_in_zone > 1)
                    {
                        ui->LEDBox->addItem(tr("Entire Zone"));
                        ui->LEDBox->setEnabled(1);
                    }
                    else
                    {
                        ui->LEDBox->setDisabled(1);
                    }

                    /*-------------------------------------*\
                    | Fill in the LED list with all LEDs in |
                    | the zone                              |
                    \*-------------------------------------*/
                    for(std::size_t led_idx = 0; led_idx < leds_in_zone; led_idx++)
                    {
                        ui->LEDBox->addItem(device->zones[selected_zone].leds[led_idx].name.c_str());
                    }

                    /*-------------------------------------*\
                    | Enable editing if:                    |
                    |   Zone has variable size              |
                    | OR                                    |
                    |   Zone is LINEAR and device type is   |
                    |   LEDSTRIP                            |
                    \*-------------------------------------*/
                    bool zone_is_editable = false;

                    if(device->zones[selected_zone].leds_min != device->zones[selected_zone].leds_max)
                    {
                        zone_is_editable = true;
                    }

                    if((device->zones[selected_zone].type == ZONE_TYPE_LINEAR) && (device->type == DEVICE_TYPE_LEDSTRIP))
                    {
                        zone_is_editable = true;
                    }

                    ui->EditZoneButton->setEnabled(zone_is_editable);

                    if(!ui->ZoneBox->signalsBlocked())
                    {
                        ui->DeviceViewBox->blockSignals(true);
                        ui->DeviceViewBox->selectZone(selected_zone);
                        ui->DeviceViewBox->blockSignals(false);
                    }
                }

                /*-----------------------------------------*\
                | Handle a segment selected condition       |
                \*-----------------------------------------*/
                else if(selected_zone != -1 && selected_segment != -1)
                {
                    /*-------------------------------------*\
                    | If there are multiple LEDs, add the   |
                    | "Entire Segment" option to the LED    |
                    | box and enable it, otherwise there is |
                    | only one LED so disable it            |
                    \*-------------------------------------*/
                    if(device->zones[selected_zone].segments[selected_segment].leds_count > 1)
                    {
                        ui->LEDBox->addItem(tr("Entire Segment"));
                        ui->LEDBox->setEnabled(1);
                    }
                    else
                    {
                        ui->LEDBox->setDisabled(1);
                    }

                    /*-------------------------------------*\
                    | Fill in the LED list with all LEDs in |
                    | the segment                           |
                    \*-------------------------------------*/
                    for(std::size_t led_idx = 0; led_idx < device->zones[selected_zone].segments[selected_segment].leds_count; led_idx++)
                    {
                        ui->LEDBox->addItem(device->zones[selected_zone].leds[led_idx + device->zones[selected_zone].segments[selected_segment].start_idx].name.c_str());
                    }

                    /*-------------------------------------*\
                    | Editing is not allowed when a         |
                    | segment is selected                   |
                    \*-------------------------------------*/
                    ui->EditZoneButton->setEnabled(false);

                    if(!ui->ZoneBox->signalsBlocked())
                    {
                        ui->DeviceViewBox->blockSignals(true);
                        ui->DeviceViewBox->selectSegment(selected_zone, selected_segment);
                        ui->DeviceViewBox->blockSignals(false);
                    }
                }

                ui->LEDBox->setCurrentIndex(0);
                on_LEDBox_currentIndexChanged(0);
                ui->LEDBox->blockSignals(false);
            }
            break;
    }
}

void OpenRGBDevicePage::on_LEDBox_currentIndexChanged(int index)
{
    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();

    /*-----------------------------------------------------*\
    | Process zone box change based on color mode           |
    \*-----------------------------------------------------*/
    switch(device->modes[selected_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
            {
                /*-----------------------------------------*\
                | Initialize both selected zone and segment |
                | to -1 to indicate there is no selection   |
                \*-----------------------------------------*/
                unsigned int    current_index       = 0;
                bool            selected_all_zones  = false;
                bool            selected_all_leds   = false;
                int             selected_led        = -1;
                int             selected_zone       = -1;
                int             selected_segment    = -1;

                /*-----------------------------------------*\
                | Handle condition where device has more    |
                | than one zone, which adds an "All Zones"  |
                | entry to the Zone menu in the first index |
                \*-----------------------------------------*/
                if(device->zones.size() > 1)
                {
                    if(ui->ZoneBox->currentIndex() == (int)current_index)
                    {
                        selected_all_zones = true;
                    }

                    current_index++;
                }

                /*-----------------------------------------*\
                | Determine selected zone and optionally    |
                | selected segment based on index if "All   |
                | Zones" is not the selected index          |
                \*-----------------------------------------*/
                if(!selected_all_zones)
                {
                    for(std::size_t zone_idx = 0; zone_idx < device->zones.size(); zone_idx++)
                    {
                        if(ui->ZoneBox->currentIndex() == (int)current_index)
                        {
                            selected_zone = (int)zone_idx;
                            break;
                        }

                        current_index++;

                        for(std::size_t segment_idx = 0; segment_idx < device->zones[zone_idx].segments.size(); segment_idx++)
                        {
                            if(ui->ZoneBox->currentIndex() == (int)current_index)
                            {
                                selected_zone    = (int)zone_idx;
                                selected_segment = (int)segment_idx;
                                break;
                            }

                            current_index++;
                        }

                        if(selected_segment != -1)
                        {
                            break;
                        }
                    }
                }

                /*-----------------------------------------*\
                | Handle selection of "Entire Device/Zone/  |
                | Segment" index is selected                |
                |                                           |
                | There should always be an Entire index as |
                | long as the LED box is enabled            |
                \*-----------------------------------------*/
                if(index == 0)
                {
                    selected_all_leds = true;
                }
                /*-----------------------------------------*\
                | Determine selected LED if "Entire Device/ |
                | Zone/Segment" is not selected             |
                \*-----------------------------------------*/
                else
                {
                    selected_led = index - 1;
                }

                /*-----------------------------------------*\
                | Initialize variables                      |
                \*-----------------------------------------*/
                bool        multiple    = (std::size_t(selected_led) == (device->leds.size() + 1));
                RGBColor    color       = 0x00000000;
                bool        updateColor = false;

                /*-----------------------------------------*\
                | Remove multiple selection                 |
                \*-----------------------------------------*/
                if(MultipleSelected)
                {
                    ui->LEDBox->removeItem((int)(device->leds.size() + 1));
                }

                MultipleSelected = false;

                /*-----------------------------------------*\
                | Handle "All Zones" selected condition     |
                \*-----------------------------------------*/
                if(selected_all_zones)
                {
                    /*-------------------------------------*\
                    | Handle Entire Device selection        |
                    \*-------------------------------------*/
                    if(selected_all_leds)
                    {
                        if(!ui->LEDBox->signalsBlocked())
                        {
                            ui->DeviceViewBox->blockSignals(true);
                            ui->DeviceViewBox->clearSelection();
                            ui->DeviceViewBox->blockSignals(false);
                        }
                    }

                    /*-------------------------------------*\
                    | Handle single selected LED            |
                    \*-------------------------------------*/
                    if((device->leds.size() == 1 || selected_led != -1) && !multiple)
                    {
                        /*---------------------------------*\
                        | Get selected LED's current color  |
                        \*---------------------------------*/
                        color = device->GetLED(selected_led);

                        /*---------------------------------*\
                        | Set update color flag             |
                        \*---------------------------------*/
                        updateColor = true;

                        /*---------------------------------*\
                        | Select LED in device view         |
                        \*---------------------------------*/
                        if(!ui->LEDBox->signalsBlocked())
                        {
                            ui->DeviceViewBox->blockSignals(true);
                            ui->DeviceViewBox->selectLed(selected_led);
                            ui->DeviceViewBox->blockSignals(false);
                        }
                    }
                }
                /*-----------------------------------------*\
                | Handle a zone selected condition          |
                \*-----------------------------------------*/
                else if(selected_zone != -1 && selected_segment == -1)
                {
                    /*-------------------------------------*\
                    | Handle Entire Zone selection          |
                    \*-------------------------------------*/
                    if(selected_all_leds)
                    {
                        if(!ui->LEDBox->signalsBlocked())
                        {
                            ui->DeviceViewBox->blockSignals(true);
                            ui->DeviceViewBox->selectZone(selected_zone);
                            ui->DeviceViewBox->blockSignals(false);
                        }
                    }

                    /*-------------------------------------*\
                    | Handle single selected LED            |
                    \*-------------------------------------*/
                    if(device->GetLEDsInZone(selected_zone) == 1 || selected_led != -1)
                    {
                        if((unsigned int)selected_led < device->GetLEDsInZone(selected_zone))
                        {
                            /*-----------------------------*\
                            | Get selected LED's current    |
                            | color                         |
                            \*-----------------------------*/
                            color = device->zones[selected_zone].colors[selected_led];

                            /*-----------------------------*\
                            | Set update color flag         |
                            \*-----------------------------*/
                            updateColor = 1;

                            /*-----------------------------*\
                            | Set global index              |
                            \*-----------------------------*/
                            int globalIndex = device->zones[selected_zone].leds - &(device->leds[0]) + selected_led;

                            /*-----------------------------*\
                            | Select LED in device view     |
                            \*-----------------------------*/
                            if(!ui->LEDBox->signalsBlocked())
                            {
                                ui->DeviceViewBox->blockSignals(true);
                                ui->DeviceViewBox->selectLed(globalIndex);
                                ui->DeviceViewBox->blockSignals(false);
                            }
                        }
                    }
                }
                /*-----------------------------------------*\
                | Handle a segment selected condition       |
                \*-----------------------------------------*/
                else if(selected_zone != -1 && selected_segment != -1)
                {
                    /*-------------------------------------*\
                    | Handle Entire Zone selection          |
                    \*-------------------------------------*/
                    if(selected_all_leds)
                    {
                        if(!ui->LEDBox->signalsBlocked())
                        {
                            ui->DeviceViewBox->blockSignals(true);
                            ui->DeviceViewBox->selectSegment(selected_zone, selected_segment);
                            ui->DeviceViewBox->blockSignals(false);
                        }
                    }

                    /*-------------------------------------*\
                    | Handle single selected LED            |
                    \*-------------------------------------*/
                    if(device->zones[selected_zone].segments[selected_segment].leds_count == 1 || selected_led != -1)
                    {
                        if((unsigned int)selected_led < device->zones[selected_zone].segments[selected_segment].leds_count)
                        {
                            /*-----------------------------*\
                            | Get selected LED's current    |
                            | color                         |
                            \*-----------------------------*/
                            color = device->zones[selected_zone].colors[selected_led + device->zones[selected_zone].segments[selected_segment].start_idx];

                            /*-----------------------------*\
                            | Set update color flag         |
                            \*-----------------------------*/
                            updateColor = 1;

                            /*-----------------------------*\
                            | Set global index              |
                            \*-----------------------------*/
                            int globalIndex = device->zones[selected_zone].leds - &(device->leds[0]) + selected_led + device->zones[selected_zone].segments[selected_segment].start_idx;

                            /*-----------------------------*\
                            | Select LED in device view     |
                            \*-----------------------------*/
                            if(!ui->LEDBox->signalsBlocked())
                            {
                                ui->DeviceViewBox->blockSignals(true);
                                ui->DeviceViewBox->selectLed(globalIndex);
                                ui->DeviceViewBox->blockSignals(false);
                            }
                        }
                    }
                }

                /*-----------------------------------------------------*\
                | Update color picker with color of selected LED        |
                \*-----------------------------------------------------*/
                if(updateColor)
                {
                    current_color.setRgb(RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));

                    updateColorUi();
                }
            }
            break;

        case MODE_COLORS_MODE_SPECIFIC:
            {
                /*-----------------------------------------------------*\
                | Update color picker with color of selected mode       |
                \*-----------------------------------------------------*/
                RGBColor color = device->modes[selected_mode].colors[index];

                current_color.setRgb(RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));

                updateColorUi();
            }
            break;
    }

}

void OpenRGBDevicePage::on_ModeBox_currentIndexChanged(int index)
{
    /*-----------------------------------------------------*\
    | Update mode user interface elements                   |
    \*-----------------------------------------------------*/
    UpdateModeUi();

    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();

    /*-----------------------------------------------------*\
    | Disable the button if we can safely auto apply colors |
    \*-----------------------------------------------------*/
    ui->ApplyColorsButton->setDisabled(autoUpdateEnabled());
    ui->SetAllButton->setDisabled(device->modes[index].name != "Direct" && device->modes[index].name != "Custom" && device->modes[index].name != "Static");

}

void OpenRGBDevicePage::on_PerLEDCheck_clicked()
{
    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();

    /*-----------------------------------------------------*\
    | Update mode user interface elements                   |
    \*-----------------------------------------------------*/
    UpdateModeUi();
}

void OpenRGBDevicePage::on_ModeSpecificCheck_clicked()
{
    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();

    /*-----------------------------------------------------*\
    | Update mode user interface elements                   |
    \*-----------------------------------------------------*/
    UpdateModeUi();
}

void OpenRGBDevicePage::on_RandomCheck_clicked()
{
    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();

    /*-----------------------------------------------------*\
    | Update mode user interface elements                   |
    \*-----------------------------------------------------*/
    UpdateModeUi();
}

void OpenRGBDevicePage::on_BrightnessSlider_valueChanged(int /*value*/)
{
    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();
}

void OpenRGBDevicePage::on_SpeedSlider_valueChanged(int /*value*/)
{
    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();
}

void OpenRGBDevicePage::on_DirectionBox_currentIndexChanged(int /*index*/)
{
    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();
}

void OpenRGBDevicePage::UpdateInterface()
{
    //UpdateModeUi();
    ui->DeviceViewBox->repaint();
}

void OpenRGBDevicePage::UpdateModeUi()
{
    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();

    /*-----------------------------------------------------*\
    | Don't update the UI if the current mode is invalid    |
    \*-----------------------------------------------------*/
    if(selected_mode < device->modes.size())
    {
        bool supports_per_led       = ( device->modes[selected_mode].flags & MODE_FLAG_HAS_PER_LED_COLOR );
        bool supports_mode_specific = ( device->modes[selected_mode].flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR );
        bool supports_random        = ( device->modes[selected_mode].flags & MODE_FLAG_HAS_RANDOM_COLOR );
        bool supports_speed         = ( device->modes[selected_mode].flags & MODE_FLAG_HAS_SPEED );
        bool supports_brightness    = ( device->modes[selected_mode].flags & MODE_FLAG_HAS_BRIGHTNESS);
        bool supports_dir_lr        = ( device->modes[selected_mode].flags & MODE_FLAG_HAS_DIRECTION_LR );
        bool supports_dir_ud        = ( device->modes[selected_mode].flags & MODE_FLAG_HAS_DIRECTION_UD );
        bool supports_dir_hv        = ( device->modes[selected_mode].flags & MODE_FLAG_HAS_DIRECTION_HV );
        bool per_led                = device->modes[selected_mode].color_mode == MODE_COLORS_PER_LED;
        bool mode_specific          = device->modes[selected_mode].color_mode == MODE_COLORS_MODE_SPECIFIC;
        bool random                 = device->modes[selected_mode].color_mode == MODE_COLORS_RANDOM;
        unsigned int dir            = device->modes[selected_mode].direction;
        bool manual_save            = ( device->modes[selected_mode].flags & MODE_FLAG_MANUAL_SAVE );
        bool automatic_save         = ( device->modes[selected_mode].flags & MODE_FLAG_AUTOMATIC_SAVE );

        if(supports_speed)
        {
            ui->SpeedSlider->blockSignals(true);
            int  current_speed;
            InvertedSpeed = device->modes[selected_mode].speed_min > device->modes[selected_mode].speed_max;

            if(InvertedSpeed)
            {
                /*-----------------------------------------------------*\
                | If Speed Slider is inverted, invert value             |
                \*-----------------------------------------------------*/
                ui->SpeedSlider->setMinimum(device->modes[selected_mode].speed_max);
                ui->SpeedSlider->setMaximum(device->modes[selected_mode].speed_min);
                current_speed = device->modes[selected_mode].speed_min - device->modes[selected_mode].speed + device->modes[selected_mode].speed_max;
            }
            else
            {
                ui->SpeedSlider->setMinimum(device->modes[selected_mode].speed_min);
                ui->SpeedSlider->setMaximum(device->modes[selected_mode].speed_max);
                current_speed = device->modes[selected_mode].speed;
            }

            ui->SpeedSlider->setValue(current_speed);
            ui->SpeedSlider->setEnabled(true);
            ui->SpeedSlider->blockSignals(false);
        }
        else
        {
            ui->SpeedSlider->blockSignals(true);
            ui->SpeedSlider->setEnabled(false);
            ui->SpeedSlider->blockSignals(false);
        }

        if(supports_brightness)
        {
            ui->BrightnessSlider->blockSignals(true);
            int current_brightness;
            InvertedBrightness = device->modes[selected_mode].brightness_min > device->modes[selected_mode].brightness_max;

            if(InvertedBrightness)
            {
                /*-----------------------------------------------------*\
                | If Brightness Slider is inverted, invert value        |
                \*-----------------------------------------------------*/
                ui->BrightnessSlider->setMinimum(device->modes[selected_mode].brightness_max);
                ui->BrightnessSlider->setMaximum(device->modes[selected_mode].brightness_min);
                current_brightness = device->modes[selected_mode].brightness_min - device->modes[selected_mode].brightness + device->modes[selected_mode].brightness_max;
            }
            else
            {
                ui->BrightnessSlider->setMinimum(device->modes[selected_mode].brightness_min);
                ui->BrightnessSlider->setMaximum(device->modes[selected_mode].brightness_max);
                current_brightness = device->modes[selected_mode].brightness;
            }

            ui->BrightnessSlider->setValue(current_brightness);
            ui->BrightnessSlider->setEnabled(true);
            ui->BrightnessSlider->blockSignals(false);
        }
        else
        {
            ui->BrightnessSlider->blockSignals(true);
            ui->BrightnessSlider->setEnabled(false);
            ui->BrightnessSlider->blockSignals(false);
        }

        ui->DirectionBox->blockSignals(true);
        ui->DirectionBox->clear();

        if(supports_dir_lr)
        {
            ui->DirectionBox->addItem(tr("Left"));
            ui->DirectionBox->addItem(tr("Right"));
        }

        if(supports_dir_ud)
        {
            ui->DirectionBox->addItem(tr("Up"));
            ui->DirectionBox->addItem(tr("Down"));
        }

        if(supports_dir_hv)
        {
            ui->DirectionBox->addItem(tr("Horizontal"));
            ui->DirectionBox->addItem(tr("Vertical"));
        }

        if(supports_dir_lr || supports_dir_ud || supports_dir_hv)
        {
            if((supports_dir_lr)
             &&((dir == MODE_DIRECTION_LEFT)
              ||(dir == MODE_DIRECTION_RIGHT)))
            {
                ui->DirectionBox->setCurrentIndex(dir);
            }

            if((supports_dir_ud)
             &&((dir == MODE_DIRECTION_UP)
              ||(dir == MODE_DIRECTION_DOWN)))
            {
                if(supports_dir_lr)
                {
                    ui->DirectionBox->setCurrentIndex(dir);
                }
                else
                {
                    ui->DirectionBox->setCurrentIndex(dir - 2);
                }
            }

            if((supports_dir_hv)
             &&((dir == MODE_DIRECTION_HORIZONTAL)
              ||(dir == MODE_DIRECTION_VERTICAL)))
            {
                if(supports_dir_lr && supports_dir_ud)
                {
                    ui->DirectionBox->setCurrentIndex(dir);
                }
                else if(supports_dir_lr || supports_dir_ud)
                {
                    ui->DirectionBox->setCurrentIndex(dir - 2);
                }
                else
                {
                    ui->DirectionBox->setCurrentIndex(dir - 4);
                }
            }

            ui->DirectionBox->setEnabled(true);
        }
        else
        {
            ui->DirectionBox->setEnabled(false);
        }

        ui->DirectionBox->blockSignals(false);

        if(supports_per_led)
        {
            ui->PerLEDCheck->setEnabled(true);
            ui->PerLEDCheck->setChecked(per_led);

            if(DeviceViewShowing)
            {
                ui->DeviceViewBoxFrame->show();
            }
        }
        else
        {
            ui->PerLEDCheck->setEnabled(false);
            ui->PerLEDCheck->setAutoExclusive(false);
            ui->PerLEDCheck->setChecked(false);
            ui->PerLEDCheck->setAutoExclusive(true);
            ui->DeviceViewBoxFrame->hide();
        }

        if(supports_mode_specific)
        {
            ui->ModeSpecificCheck->setEnabled(true);
            ui->ModeSpecificCheck->setChecked(mode_specific);
        }
        else
        {
            ui->ModeSpecificCheck->setEnabled(false);
            ui->ModeSpecificCheck->setAutoExclusive(false);
            ui->ModeSpecificCheck->setChecked(false);
            ui->ModeSpecificCheck->setAutoExclusive(true);
        }

        if(supports_random)
        {
            ui->RandomCheck->setEnabled(true);
            ui->RandomCheck->setChecked(random);
        }
        else
        {
            ui->RandomCheck->setEnabled(false);
            ui->RandomCheck->setAutoExclusive(false);
            ui->RandomCheck->setChecked(false);
            ui->RandomCheck->setAutoExclusive(true);
        }

        if(automatic_save)
        {
            ui->DeviceSaveButton->setText(tr("Saved To Device"));
            ui->DeviceSaveButton->setEnabled(false);
        }
        else if(manual_save)
        {
            ui->DeviceSaveButton->setText(tr("Save To Device"));
            ui->DeviceSaveButton->setEnabled(true);
        }
        else
        {
            ui->DeviceSaveButton->setText(tr("Saving Not Supported"));
            ui->DeviceSaveButton->setEnabled(false);
        }

        /*-----------------------------------------------------*\
        | Fill in the zone box based on color mode              |
        \*-----------------------------------------------------*/
        switch(device->modes[selected_mode].color_mode)
        {
            case MODE_COLORS_NONE:
            case MODE_COLORS_RANDOM:
                ui->ZoneBox->blockSignals(true);
                ui->ZoneBox->clear();
                ui->ZoneBox->blockSignals(false);

                ui->LEDBox->blockSignals(true);
                ui->LEDBox->clear();
                ui->LEDBox->blockSignals(false);

                ui->EditZoneButton->setEnabled(false);
                ui->ApplyColorsButton->setEnabled(false);
                break;

            case MODE_COLORS_PER_LED:
                ui->ZoneBox->blockSignals(true);
                ui->ZoneBox->clear();

                if(device->zones.size() > 1)
                {
                    ui->ZoneBox->setEnabled(1);
                    ui->ZoneBox->addItem(tr("All Zones"));
                }
                else if(device->zones.size() == 1 && device->zones[0].segments.size() > 1)
                {
                    ui->ZoneBox->setEnabled(1);
                }
                else
                {
                    ui->ZoneBox->setDisabled(1);
                    ui->EditZoneButton->setEnabled(false);
                }

                for(std::size_t zone_idx = 0; zone_idx < device->zones.size(); zone_idx++)
                {
                    ui->ZoneBox->addItem(device->GetZoneName((unsigned int)zone_idx).c_str());

                    for(std::size_t segment_idx = 0; segment_idx < device->zones[zone_idx].segments.size(); segment_idx++)
                    {
                        ui->ZoneBox->addItem(("    " + device->zones[zone_idx].segments[segment_idx].name).c_str());
                    }
                }

                ui->ZoneBox->setCurrentIndex(0);
                ui->ZoneBox->blockSignals(false);
                ui->ApplyColorsButton->setEnabled(true);

                /*-----------------------------------------------------*\
                | Update LED box                                        |
                \*-----------------------------------------------------*/
                on_ZoneBox_currentIndexChanged(0);

                /*-----------------------------------------------------*\
                | Update color picker with color of first LED           |
                \*-----------------------------------------------------*/
                //on_LEDBox_currentIndexChanged(0);
                break;

            case MODE_COLORS_MODE_SPECIFIC:
                ui->ZoneBox->blockSignals(true);
                ui->ZoneBox->clear();
                ui->ZoneBox->addItem(tr("Mode Specific"));
                ui->ZoneBox->blockSignals(false);

                ui->LEDBox->blockSignals(true);
                ui->LEDBox->clear();

                if(device->modes[selected_mode].colors_min == device->modes[selected_mode].colors_max)
                {
                    ui->EditZoneButton->setEnabled(false);
                }
                else
                {
                    ui->EditZoneButton->setEnabled(true);
                }

                for(unsigned int i = 0; i < device->modes[selected_mode].colors.size(); i++)
                {
                    char id_buf[32];
                    // TODO: translate
                    snprintf(id_buf, 32, "Mode Color %u", i);
                    ui->LEDBox->addItem(id_buf);
                }

                ui->LEDBox->setCurrentIndex(0);
                on_LEDBox_currentIndexChanged(0);
                ui->LEDBox->setEnabled(true);
                ui->LEDBox->blockSignals(false);
                ui->ApplyColorsButton->setEnabled(true);
                break;
        }
    }
}

void OpenRGBDevicePage::UpdateMode()
{
    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    int current_mode = ui->ModeBox->currentIndex();

    if(current_mode >= 0)
    {
        int  current_speed          = 0;
        int  current_brightness     = 0;
        bool current_per_led        = ui->PerLEDCheck->isChecked();
        bool current_mode_specific  = ui->ModeSpecificCheck->isChecked();
        bool current_random         = ui->RandomCheck->isChecked();
        int  current_dir_idx        = ui->DirectionBox->currentIndex();
        int  current_direction      = 0;
        bool supports_dir_lr        = ( device->modes[(unsigned int)current_mode].flags & MODE_FLAG_HAS_DIRECTION_LR );
        bool supports_dir_ud        = ( device->modes[(unsigned int)current_mode].flags & MODE_FLAG_HAS_DIRECTION_UD );
        bool supports_dir_hv        = ( device->modes[(unsigned int)current_mode].flags & MODE_FLAG_HAS_DIRECTION_HV );

        /*-----------------------------------------------------*\
        | If DirectionBox is enabled, set the direction values  |
        \*-----------------------------------------------------*/
        if(ui->DirectionBox->isEnabled())
        {
            if(supports_dir_hv)
            {
                if(supports_dir_lr && supports_dir_ud)
                {
                    current_direction = current_dir_idx;
                }
                else if(supports_dir_lr || supports_dir_ud)
                {
                    current_direction = current_dir_idx + 2;
                }
                else
                {
                    current_direction = current_dir_idx + 4;
                }
            }

            if(supports_dir_ud)
            {
                if(supports_dir_lr)
                {
                    current_direction = current_dir_idx;
                }
                else
                {
                    current_direction = current_dir_idx + 2;
                }
            }

            if((supports_dir_lr)
             &&(current_dir_idx < 2))
            {
                current_direction = current_dir_idx;
            }

            device->modes[(unsigned int)current_mode].direction = current_direction;
        }

        /*-----------------------------------------------------*\
        | If Speed Slider is enabled, read the speed value      |
        \*-----------------------------------------------------*/
        if(ui->SpeedSlider->isEnabled())
        {
            /*-----------------------------------------------------*\
            | If Speed Slider is inverted, invert value             |
            \*-----------------------------------------------------*/
            if(InvertedSpeed)
            {
                current_speed = device->modes[(unsigned int)current_mode].speed_min - ui->SpeedSlider->value() + device->modes[current_mode].speed_max;
            }
            else
            {
                current_speed = ui->SpeedSlider->value();
            }
        }

        /*-----------------------------------------------------*\
        | If Brightness Slider is enabled, read the value       |
        \*-----------------------------------------------------*/
        if(ui->BrightnessSlider->isEnabled())
        {
            /*-----------------------------------------------------*\
            | If Brightness Slider is inverted, invert value        |
            \*-----------------------------------------------------*/
            if(InvertedBrightness)
            {
                current_brightness = device->modes[(unsigned int)current_mode].brightness_min - ui->BrightnessSlider->value() + device->modes[current_mode].brightness_max;
            }
            else
            {
                current_brightness = ui->BrightnessSlider->value();
            }
        }

        /*-----------------------------------------------------*\
        | Don't set the mode if the current mode is invalid     |
        \*-----------------------------------------------------*/
        if((unsigned int)current_mode < device->modes.size())
        {
            /*-----------------------------------------------------*\
            | Update mode parameters                                |
            \*-----------------------------------------------------*/
            device->modes[(unsigned int)current_mode].speed         = current_speed;
            device->modes[(unsigned int)current_mode].brightness    = current_brightness;

            if(current_per_led)
            {
                device->modes[(unsigned int)current_mode].color_mode = MODE_COLORS_PER_LED;
            }
            else if(current_mode_specific)
            {
                device->modes[(unsigned int)current_mode].color_mode = MODE_COLORS_MODE_SPECIFIC;
            }
            else if(current_random)
            {
                device->modes[(unsigned int)current_mode].color_mode = MODE_COLORS_RANDOM;
            }
            else
            {
                device->modes[(unsigned int)current_mode].color_mode = MODE_COLORS_NONE;
            }

            /*-----------------------------------------------------*\
            | Change device mode                                    |
            \*-----------------------------------------------------*/
            device->SetMode((unsigned int)current_mode);

            if(device->modes[(unsigned int)current_mode].color_mode == MODE_COLORS_PER_LED)
            {
                device->UpdateLEDs();
            }
        }
    }
}

void OpenRGBDevicePage::SetDevice(unsigned char red, unsigned char green, unsigned char blue)
{
    current_color.setRgb(red, green, blue);

    /*-----------------------------------------------------*\
    | Update the color UI                                   |
    \*-----------------------------------------------------*/
    colorChanged();
}

void OpenRGBDevicePage::UpdateDevice()
{
    ui->ModeBox->blockSignals(true);
    ui->ModeBox->setCurrentIndex(device->active_mode);
    ui->ModeBox->blockSignals(false);
    UpdateModeUi();
    UpdateMode();
}

void OpenRGBDevicePage::SetCustomMode(unsigned char red, unsigned char green, unsigned char blue)
{
    RGBColor color = ToRGBColor(red, green, blue);
    /*-----------------------------------------------------*\
    | Set the selected mode to the custom mode and update UI|
    \*-----------------------------------------------------*/
    device->SetCustomMode();
    ui->ModeBox->blockSignals(true);
    ui->ModeBox->setCurrentIndex(device->active_mode);
    ui->ModeBox->blockSignals(false);
    UpdateModeUi();

    /*-----------------------------------------------------*\
    | Set the color boxes                                   |
    \*-----------------------------------------------------*/
    current_color.setRgb(red, green, blue);
    updateColorUi();

    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();

    switch(device->modes[selected_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
        {
            device->SetAllLEDs(color);
        }
        break;

        case MODE_COLORS_MODE_SPECIFIC:
        {
            for(std::size_t i = 0; i < device->modes[selected_mode].colors.size(); i++)
            {
                device->modes[selected_mode].colors[i] = color;
            }
            break;
        }
    }

    /*-----------------------------------------------------*\
    | Apply mode and colors                                 |
    \*-----------------------------------------------------*/
    UpdateMode();
}

void OpenRGBDevicePage::on_SwatchBox_swatchChanged(const QColor color)
{
    /*-----------------------------------------------------*\
    | Store the swatch color to the current color QColor    |
    \*-----------------------------------------------------*/
    current_color = color;

    /*-----------------------------------------------------*\
    | Update the color UI                                   |
    \*-----------------------------------------------------*/
    colorChanged();
}

void OpenRGBDevicePage::on_ColorWheelBox_colorChanged(const QColor color)
{
    /*-----------------------------------------------------*\
    | Store the wheel color to the current color QColor     |
    \*-----------------------------------------------------*/
    current_color = color;

    /*-----------------------------------------------------*\
    | Update the color UI                                   |
    \*-----------------------------------------------------*/
    colorChanged();
}

bool OpenRGBDevicePage::autoUpdateEnabled()
{
    return !(device->modes[device->active_mode].flags & MODE_FLAG_AUTOMATIC_SAVE);
}

void OpenRGBDevicePage::on_RedSpinBox_valueChanged(int red)
{
    /*-----------------------------------------------------*\
    | Update the current color QColor red channel           |
    \*-----------------------------------------------------*/
    current_color.setRed(red);

    /*-----------------------------------------------------*\
    | Update the color UI                                   |
    \*-----------------------------------------------------*/
    colorChanged();
}

void OpenRGBDevicePage::on_HueSpinBox_valueChanged(int hue)
{
    /*-----------------------------------------------------*\
    | Read the saturation and value box values              |
    \*-----------------------------------------------------*/
    int sat = current_color.saturation();
    int val = current_color.value();

    /*-----------------------------------------------------*\
    | Update the current color QColor using HSV             |
    \*-----------------------------------------------------*/
    current_color.setHsv(hue, sat, val);

    /*-----------------------------------------------------*\
    | Update the color UI                                   |
    \*-----------------------------------------------------*/
    colorChanged();
}

void OpenRGBDevicePage::on_GreenSpinBox_valueChanged(int green)
{
    /*-----------------------------------------------------*\
    | Update the current color QColor green channel         |
    \*-----------------------------------------------------*/
    current_color.setGreen(green);

    /*-----------------------------------------------------*\
    | Update the color UI                                   |
    \*-----------------------------------------------------*/
    colorChanged();
}

void OpenRGBDevicePage::on_SatSpinBox_valueChanged(int sat)
{
    /*-----------------------------------------------------*\
    | Read the hue and value box values                     |
    \*-----------------------------------------------------*/
    int hue = current_color.hue();
    int val = current_color.value();

    /*-----------------------------------------------------*\
    | Update the current color QColor using HSV             |
    \*-----------------------------------------------------*/
    current_color.setHsv(hue, sat, val);

    /*-----------------------------------------------------*\
    | Update the color UI                                   |
    \*-----------------------------------------------------*/
    colorChanged();
}

void OpenRGBDevicePage::on_BlueSpinBox_valueChanged(int blue)
{
    /*-----------------------------------------------------*\
    | Update the current color QColor blue channel          |
    \*-----------------------------------------------------*/
    current_color.setBlue(blue);

    /*-----------------------------------------------------*\
    | Update the color UI                                   |
    \*-----------------------------------------------------*/
    colorChanged();
}

void OpenRGBDevicePage::on_ValSpinBox_valueChanged(int val)
{
    /*-----------------------------------------------------*\
    | Read the hue and saturation box values                |
    \*-----------------------------------------------------*/
    int hue = current_color.hue();
    int sat = current_color.saturation();

    /*-----------------------------------------------------*\
    | Update the current color QColor using HSV             |
    \*-----------------------------------------------------*/
    current_color.setHsv(hue, sat, val);

    /*-----------------------------------------------------*\
    | Update the color UI                                   |
    \*-----------------------------------------------------*/
    colorChanged();
}

void OpenRGBDevicePage::on_HexLineEdit_textChanged(const QString &arg1)
{
    /*-----------------------------------------------------*\
    | Make an editable copy of the string                   |
    \*-----------------------------------------------------*/
    QString temp = arg1;

    /*-----------------------------------------------------*\
    | Remove # character so that #XXXXXX color codes are    |
    | acceptable.  0xXXXXXX codes are already accepted by   |
    | toInt().  Convert into an RGBColor.  Mask off the     |
    | unused bits.                                          |
    \*-----------------------------------------------------*/
    RGBColor color = (RGBColor)(0x00FFFFFF & temp.replace("#", "").toInt(NULL, 16));

    /*-----------------------------------------------------*\
    | Store new color into the current color QColor         |
    | Because RGBColor stores color in BGR format, we have  |
    | to reverse the R and B channels if the hex format is  |
    | RGB.                                                  |
    \*-----------------------------------------------------*/
    if(HexFormatRGB)
    {
        current_color.setRed(RGBGetBValue(color));
        current_color.setGreen(RGBGetGValue(color));
        current_color.setBlue(RGBGetRValue(color));
    }
    else
    {
        current_color.setRed(RGBGetRValue(color));
        current_color.setGreen(RGBGetGValue(color));
        current_color.setBlue(RGBGetBValue(color));
    }

    /*-----------------------------------------------------*\
    | Update the color UI, but set the UpdateHex flag to    |
    | false so the hex edit box isn't updated while the user|
    | is in the middle of typing a value.                   |
    \*-----------------------------------------------------*/
    UpdateHex = false;
    colorChanged();
    UpdateHex = true;
}

void OpenRGBDevicePage::on_DeviceViewBox_selectionChanged(QVector<int> indices)
{
    if(device->modes[device->active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        ui->ZoneBox->blockSignals(true);
        ui->LEDBox->blockSignals(true);
        ui->ZoneBox->setCurrentIndex(0);
        on_ZoneBox_currentIndexChanged(0);
        //updateLeds(); // We want to update the LED box, but we don't want any of the side effects of that action
        ui->ZoneBox->blockSignals(false);
        if(indices.size() != 0 && size_t(indices.size()) != device->leds.size())
        {
            if(indices.size() == 1)
            {
                if(device->leds.size() == 1)
                {
                    ui->LEDBox->setCurrentIndex(0);
                }
                else
                {
                    ui->LEDBox->setCurrentIndex(indices[0] + 1);
                    // Set everything to it's color
                }
                MultipleSelected = 0;
            }
            else
            {
                if(MultipleSelected)
                {
                    ui->LEDBox->removeItem((int)(device->leds.size() + 1));
                }
                // TODO: translate
                ui->LEDBox->addItem("Multiple (" + QVariant(indices.size()).toString() + ")");
                ui->LEDBox->setCurrentIndex((int)(device->leds.size() + 1));
                MultipleSelected = 1;
            }
        }
        else
        {
            ui->LEDBox->setCurrentIndex(0);
        }
        ui->LEDBox->blockSignals(false);
    }
}

void OpenRGBDevicePage::on_SetAllButton_clicked()
{
    emit SetAllDevices(current_color.red(), current_color.green(), current_color.blue());
}

void OpenRGBDevicePage::on_EditZoneButton_clicked()
{
    switch(device->modes[device->active_mode].color_mode)
    {
    case MODE_COLORS_PER_LED:
        {
            /*-----------------------------------------*\
            | Initialize both selected zone and segment |
            | to -1 to indicate there is no selection   |
            \*-----------------------------------------*/
            unsigned int    current_index       = 0;
            bool            selected_all_zones  = false;
            int             selected_zone       = -1;
            int             selected_segment    = -1;

            /*-----------------------------------------*\
            | Handle condition where device has more    |
            | than one zone, which adds an "All Zones"  |
            | entry to the Zone menu in the first index |
            \*-----------------------------------------*/
            if(device->zones.size() > 1)
            {
                if(ui->ZoneBox->currentIndex() == (int)current_index)
                {
                    selected_all_zones = true;
                }

                current_index++;
            }

            /*-----------------------------------------*\
            | Determine selected zone and optionally    |
            | selected segment based on index if "All   |
            | Zones" is not the selected index          |
            \*-----------------------------------------*/
            if(!selected_all_zones)
            {
                for(std::size_t zone_idx = 0; zone_idx < device->zones.size(); zone_idx++)
                {
                    if(ui->ZoneBox->currentIndex() == (int)current_index)
                    {
                        selected_zone = (int)zone_idx;
                        break;
                    }

                    current_index++;

                    for(std::size_t segment_idx = 0; segment_idx < device->zones[zone_idx].segments.size(); segment_idx++)
                    {
                        if(ui->ZoneBox->currentIndex() == (int)current_index)
                        {
                            selected_zone    = (int)zone_idx;
                            selected_segment = (int)segment_idx;
                            break;
                        }

                        current_index++;
                    }

                    if(selected_segment != -1)
                    {
                        break;
                    }
                }
            }

            /*-----------------------------------------*\
            | If all zones or a segment are selected,   |
            | the edit button should not be clickable.  |
            | If somehow this did get clicked, ignore.  |
            \*-----------------------------------------*/
            if(selected_all_zones || selected_segment != -1)
            {
                return;
            }

            /*-----------------------------------------*\
            | Only allow resizing linear zones or       |
            | effects-only resizable zones              |
            \*-----------------------------------------*/
            if((device->zones[selected_zone].type == ZONE_TYPE_LINEAR) || (device->zones[selected_zone].flags & ZONE_FLAG_RESIZE_EFFECTS_ONLY))
            {
                OpenRGBZoneResizeDialog dlg(device, selected_zone);

                int new_size = dlg.show();

                if(new_size >= 0)
                {
                    /*-----------------------------------------------------*\
                    | Update mode UI to update Zone box                     |
                    \*-----------------------------------------------------*/
                    UpdateModeUi();

                    /*-----------------------------------------------------*\
                    | Update interface to update Device View                |
                    \*-----------------------------------------------------*/
                    UpdateInterface();

                    /*-----------------------------------------------------*\
                    | Update LED box                                        |
                    \*-----------------------------------------------------*/
                    on_ZoneBox_currentIndexChanged(selected_zone);

                    /*-----------------------------------------------------*\
                    | Update color picker with color of first LED           |
                    \*-----------------------------------------------------*/
                    on_LEDBox_currentIndexChanged(0);

                    /*-----------------------------------------------------*\
                    | Save the size profile                                 |
                    \*-----------------------------------------------------*/
                    SaveSizeProfile();
                }
            }
        }
        break;

    case MODE_COLORS_MODE_SPECIFIC:
        {
            OpenRGBZoneResizeDialog dlg(device->modes[device->active_mode].colors_min,
                                        device->modes[device->active_mode].colors_max,
                                        (int)device->modes[device->active_mode].colors.size());

            int new_size = dlg.show();

            if(new_size > 0)
            {
                device->modes[device->active_mode].colors.resize(new_size);
            }

            UpdateModeUi();
            UpdateMode();
        }
        break;
    }
}

void OpenRGBDevicePage::ShowDeviceView()
{
    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode = (unsigned int)ui->ModeBox->currentIndex();

    /*-----------------------------------------------------*\
    | Set device view showing flag to True                  |
    \*-----------------------------------------------------*/
    DeviceViewShowing = true;

    /*-----------------------------------------------------*\
    | Only show device view if active mode is Per-LED and   |
    | device contains at least one LED                      |
    \*-----------------------------------------------------*/
    if(device->modes[selected_mode].flags & MODE_FLAG_HAS_PER_LED_COLOR && device->leds.size() >= 1)
    {
        ui->DeviceViewBoxFrame->show();
    }
}

void OpenRGBDevicePage::HideDeviceView()
{
    /*-----------------------------------------------------*\
    | Set device view showing flag to False                 |
    \*-----------------------------------------------------*/
    DeviceViewShowing = false;

    /*-----------------------------------------------------*\
    | Hide device view                                      |
    \*-----------------------------------------------------*/
    ui->DeviceViewBoxFrame->hide();
}

void OpenRGBDevicePage::on_ApplyColorsButton_clicked()
{
    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode = (unsigned int)ui->ModeBox->currentIndex();

    switch(device->modes[selected_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
            {
                RGBColor qrgb = ToRGBColor(
                                    current_color.red(),
                                    current_color.green(),
                                    current_color.blue()
                                );

                ui->DeviceViewBox->setSelectionColor(qrgb);
            }
            break;

        case MODE_COLORS_MODE_SPECIFIC:
            {
                unsigned int index = ui->LEDBox->currentIndex();

                /*-----------------------------------------------------*\
                | Set all device LEDs to the current color              |
                \*-----------------------------------------------------*/
                RGBColor color = ToRGBColor(
                                    current_color.red(),
                                    current_color.green(),
                                    current_color.blue()
                                );

                device->modes[selected_mode].colors[index] = color;

                device->UpdateMode();
            }
            break;
    }
}

void OpenRGBDevicePage::on_SelectAllLEDsButton_clicked()
{
    if(device->modes[device->active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        ui->LEDBox->setCurrentIndex(0);
        on_LEDBox_currentIndexChanged(0);
        ui->DeviceViewBox->repaint();
    }
}

void OpenRGBDevicePage::on_DeviceSaveButton_clicked()
{
    if(device->modes[device->active_mode].flags & MODE_FLAG_MANUAL_SAVE)
    {
        device->SaveMode();
    }
}

void OpenRGBDevicePage::colorChanged()
{
    updateColorUi();

    if(autoUpdateEnabled())
    {
        unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();

        /*-----------------------------------------------------------------*\
        | OpenRGB's RGBColor is stored differently than Qt's qrgb type,     |
        | so casting between them doesn't work                              |
        \*-----------------------------------------------------------------*/
        RGBColor rgb_color = ToRGBColor(current_color.red(), current_color.green(), current_color.blue());

        switch(device->modes[selected_mode].color_mode)
        {
            case MODE_COLORS_PER_LED:
            {
                ui->DeviceViewBox->setSelectionColor(rgb_color);
                break;
            }

            case MODE_COLORS_MODE_SPECIFIC:
            {
                unsigned int index = ui->LEDBox->currentIndex();

                device->modes[selected_mode].colors[index] = rgb_color;
                device->UpdateMode();
                break;
            }
        }
    }
}

void OpenRGBDevicePage::updateColorUi()
{
    /*-----------------------------------------------------*\
    | Update colorwheel                                     |
    \*-----------------------------------------------------*/
    ui->ColorWheelBox->blockSignals(true);
    ui->ColorWheelBox->setColor(current_color);
    ui->ColorWheelBox->blockSignals(false);

    /*-----------------------------------------------------*\
    | Update RGB spinboxes                                  |
    \*-----------------------------------------------------*/
    ui->RedSpinBox->blockSignals(true);
    ui->RedSpinBox->setValue(current_color.red());
    ui->RedSpinBox->blockSignals(false);

    ui->GreenSpinBox->blockSignals(true);
    ui->GreenSpinBox->setValue(current_color.green());
    ui->GreenSpinBox->blockSignals(false);

    ui->BlueSpinBox->blockSignals(true);
    ui->BlueSpinBox->setValue(current_color.blue());
    ui->BlueSpinBox->blockSignals(false);

    /*-----------------------------------------------------*\
    | Update HSV spinboxes                                  |
    \*-----------------------------------------------------*/
    ui->HueSpinBox->blockSignals(true);
    ui->HueSpinBox->setValue(current_color.hue());
    ui->HueSpinBox->blockSignals(false);

    ui->SatSpinBox->blockSignals(true);
    ui->SatSpinBox->setValue(current_color.saturation());
    ui->SatSpinBox->blockSignals(false);

    ui->ValSpinBox->blockSignals(true);
    ui->ValSpinBox->setValue(current_color.value());
    ui->ValSpinBox->blockSignals(false);

    /*-----------------------------------------------------*\
    | Update Hex edit box                                   |
    \*-----------------------------------------------------*/
    if(UpdateHex)
    {
        RGBColor color = (0x00FFFFFF & current_color.rgb());

        /*-------------------------------------------------*\
        | If the hex format is BGR, swap R and B before     |
        | displaying as hex                                 |
        \*-------------------------------------------------*/
        if(!HexFormatRGB)
        {
            color = RGBGetRValue(color) << 16
                  | RGBGetGValue(color) << 8
                  | RGBGetBValue(color);
        }

        ui->HexLineEdit->blockSignals(true);
        ui->HexLineEdit->setText(QString().asprintf("%06X", color));
        ui->HexLineEdit->blockSignals(false);
    }
}
