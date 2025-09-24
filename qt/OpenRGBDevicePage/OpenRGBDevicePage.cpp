/*---------------------------------------------------------*\
| OpenRGBDevicePage.cpp                                     |
|                                                           |
|   User interface for OpenRGB device page                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "OpenRGBDevicePage.h"
#include "OpenRGBZoneResizeDialog.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "ui_OpenRGBDevicePage.h"

static void UpdateCallback(void * this_ptr, unsigned int update_reason)
{
    OpenRGBDevicePage * this_obj = (OpenRGBDevicePage *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "UpdateInterface", Qt::QueuedConnection, Q_ARG(unsigned int, update_reason));
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

    /*-----------------------------------------------------*\
    | Initialize Device View                                |
    \*-----------------------------------------------------*/
    ui->DeviceViewBox->setController(device);
    ui->DeviceViewBoxFrame->hide();

    /*-----------------------------------------------------*\
    | Update zone list                                      |
    \*-----------------------------------------------------*/
    UpdateZoneList();

    /*-----------------------------------------------------*\
    | Update mode list                                      |
    \*-----------------------------------------------------*/
    UpdateModeList();

    /*-----------------------------------------------------*\
    | Update mode user interface elements                   |
    \*-----------------------------------------------------*/
    UpdateModeUi();

    /*-----------------------------------------------------*\
    | Update LED list                                       |
    \*-----------------------------------------------------*/
    UpdateLEDList();

    /*-----------------------------------------------------*\
    | Update LED UI                                         |
    \*-----------------------------------------------------*/
    UpdateLEDUi();
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

/*---------------------------------------------------------*\
| Device Functions                                          |
\*---------------------------------------------------------*/
RGBController* OpenRGBDevicePage::GetController()
{
    return(device);
}

void OpenRGBDevicePage::SetCustomMode(unsigned char red, unsigned char green, unsigned char blue)
{
    /*-----------------------------------------------------*\
    | Set the selected mode to the custom mode and update UI|
    \*-----------------------------------------------------*/
    device->SetCustomMode();
    UpdateModeList();
    UpdateModeUi();

    /*-----------------------------------------------------*\
    | Set the color boxes                                   |
    \*-----------------------------------------------------*/
    RGBColor color = ToRGBColor(red, green, blue);
    current_color.setRgb(red, green, blue);
    UpdateColorUi();

    /*-----------------------------------------------------*\
    | Set colors for the active mode                        |
    \*-----------------------------------------------------*/
    switch(device->GetModeColorMode(device->GetActiveMode()))
    {
        case MODE_COLORS_PER_LED:
        {
            device->SetAllColors(color);
        }
        break;

        case MODE_COLORS_MODE_SPECIFIC:
        {
            for(std::size_t i = 0; i < device->GetModeColorsCount(device->GetActiveMode()); i++)
            {
                device->SetModeColor(device->GetActiveMode(), i, color);
            }
            break;
        }
    }

    /*-----------------------------------------------------*\
    | Apply mode and colors                                 |
    \*-----------------------------------------------------*/
    UpdateMode();
}

/*---------------------------------------------------------*\
| Device View Functions                                     |
\*---------------------------------------------------------*/
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

void OpenRGBDevicePage::ShowDeviceView()
{
    /*-----------------------------------------------------*\
    | Determine the selected mode                           |
    \*-----------------------------------------------------*/
    bool            selected_zone_mode;
    int             selected_mode;

    GetSelectedMode(&selected_zone_mode, &selected_mode);

    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    bool            selected_all_zones;
    int             selected_zone;
    int             selected_segment;

    GetSelectedZone(&selected_all_zones, &selected_zone, &selected_segment);

    /*-----------------------------------------------------*\
    | Set device view showing flag to True                  |
    \*-----------------------------------------------------*/
    DeviceViewShowing = true;

    /*-----------------------------------------------------*\
    | Only show device view if active mode is Per-LED and   |
    | device contains at least one LED                      |
    \*-----------------------------------------------------*/
    if(device->GetLEDCount() > 0)
    {
        if(selected_zone_mode && (selected_mode >= 0))
        {
            if(device->GetZoneModeFlags(selected_zone, selected_mode) & MODE_FLAG_HAS_PER_LED_COLOR)
            {
                ui->DeviceViewBoxFrame->show();
            }
        }
        else if(selected_zone_mode)
        {
            if(device->GetModeFlags(device->GetActiveMode()) & MODE_FLAG_HAS_PER_LED_COLOR)
            {
                ui->DeviceViewBoxFrame->show();
            }
        }
        else
        {
            if(device->GetModeFlags(selected_mode) & MODE_FLAG_HAS_PER_LED_COLOR)
            {
                ui->DeviceViewBoxFrame->show();
            }
        }
    }
}

/*---------------------------------------------------------*\
| UI Update Functions                                       |
\*---------------------------------------------------------*/
void OpenRGBDevicePage::UpdateColor()
{
    /*-----------------------------------------------------*\
    | Determine the selected mode                           |
    \*-----------------------------------------------------*/
    bool            selected_zone_mode;
    int             selected_mode;

    GetSelectedMode(&selected_zone_mode, &selected_mode);

    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    bool            selected_all_zones;
    int             selected_zone;
    int             selected_segment;

    GetSelectedZone(&selected_all_zones, &selected_zone, &selected_segment);

    /*-----------------------------------------------------*\
    | Get color mode of selected mode                       |
    \*-----------------------------------------------------*/
    unsigned int    color_mode;

    if(selected_zone_mode && (selected_mode >= 0))
    {
        color_mode = device->GetZoneModeColorMode(selected_zone, selected_mode);
    }
    else if(selected_zone_mode)
    {
        color_mode = device->GetModeColorMode(device->GetActiveMode());
    }
    else
    {
        color_mode = device->GetModeColorMode(selected_mode);
    }

    /*-----------------------------------------------------*\
    | OpenRGB's RGBColor is stored differently than Qt's    |
    | qrgb type, so casting between them doesn't work       |
    \*-----------------------------------------------------*/
    RGBColor rgb_color = ToRGBColor(current_color.red(), current_color.green(), current_color.blue());

    switch(color_mode)
    {
        case MODE_COLORS_PER_LED:
            ui->DeviceViewBox->setSelectionColor(rgb_color);
            break;

        case MODE_COLORS_MODE_SPECIFIC:
            {
                unsigned int index = ui->LEDBox->currentIndex();

                if(selected_zone_mode && (selected_mode >= 0))
                {
                    device->SetZoneModeColor(selected_zone, selected_mode, index, rgb_color);
                    device->UpdateZoneMode(selected_zone);
                }
                else if(selected_zone_mode)
                {
                    device->SetModeColor(device->GetActiveMode(), index, rgb_color);
                    device->UpdateMode();
                }
                else
                {
                    device->SetModeColor(selected_mode, index, rgb_color);
                    device->UpdateMode();
                }
            }
            break;
    }
}

void OpenRGBDevicePage::UpdateColorUi()
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

void OpenRGBDevicePage::UpdateDevice()
{
    ui->ModeBox->blockSignals(true);
    ui->ModeBox->setCurrentIndex(device->GetActiveMode());
    ui->ModeBox->blockSignals(false);
    UpdateModeUi();
    UpdateMode();
}

void OpenRGBDevicePage::UpdateLEDList()
{
    /*-----------------------------------------------------*\
    | Determine the selected mode                           |
    \*-----------------------------------------------------*/
    bool            selected_zone_mode;
    int             selected_mode;

    GetSelectedMode(&selected_zone_mode, &selected_mode);

    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    bool            selected_all_zones;
    int             selected_zone;
    int             selected_segment;

    GetSelectedZone(&selected_all_zones, &selected_zone, &selected_segment);

    /*-----------------------------------------------------*\
    | Update LED list based on active mode's color mode     |
    \*-----------------------------------------------------*/
    unsigned int    color_mode;

    if(selected_zone_mode)
    {
        if(selected_mode >= 0)
        {
            color_mode = device->GetZoneModeColorMode(selected_zone, selected_mode);
        }
        else
        {
            color_mode = device->GetModeColorMode(device->GetActiveMode());
        }
    }
    else
    {
        color_mode = device->GetModeColorMode(selected_mode);
    }

    switch(color_mode)
    {
        case MODE_COLORS_NONE:
        case MODE_COLORS_RANDOM:
        default:
            /*---------------------------------------------*\
            | Update label                                  |
            \*---------------------------------------------*/
            ui->LEDLabel->setText("LED:");

            /*---------------------------------------------*\
            | Disable Select All button                     |
            \*---------------------------------------------*/
            ui->SpinBoxModeColors->setHidden(true);
            ui->SpinBoxModeColors->setEnabled(false);
            ui->SelectAllLEDsButton->setHidden(false);
            ui->SelectAllLEDsButton->setEnabled(false);

            /*---------------------------------------------*\
            | Clear LED box                                 |
            \*---------------------------------------------*/
            ui->LEDBox->blockSignals(true);
            ui->LEDBox->clear();

            /*---------------------------------------------*\
            | Disable LED box                               |
            \*---------------------------------------------*/
            ui->LEDBox->setEnabled(false);
            ui->LEDBox->blockSignals(false);
            break;

        case MODE_COLORS_PER_LED:
            /*---------------------------------------------*\
            | Update label                                  |
            \*---------------------------------------------*/
            ui->LEDLabel->setText("LED:");

            /*---------------------------------------------*\
            | Enable Select All button                      |
            \*---------------------------------------------*/
            ui->SpinBoxModeColors->setHidden(true);
            ui->SpinBoxModeColors->setEnabled(false);
            ui->SelectAllLEDsButton->setHidden(false);
            ui->SelectAllLEDsButton->setEnabled(true);

            /*---------------------------------------------*\
            | Clear LED box                                 |
            \*---------------------------------------------*/
            ui->LEDBox->blockSignals(true);
            ui->LEDBox->clear();

            /*---------------------------------------------*\
            | Clear multiple selected flag                  |
            \*---------------------------------------------*/
            MultipleSelected = false;

            /*---------------------------------------------*\
            | Handle "All Zones" selected condition or      |
            | multiple selection condition                  |
            \*---------------------------------------------*/
            if(selected_all_zones)
            {
                /*-----------------------------------------*\
                | If there are multiple LEDs, add the       |
                | "Entire Device" option to the LED box and |
                | enable it, otherwise there is only one    |
                | LED so disable it                         |
                \*-----------------------------------------*/
                if(device->GetLEDCount() > 1)
                {
                    ui->LEDBox->addItem(tr("Entire Device"));
                    ui->LEDBox->setEnabled(1);
                }
                else
                {
                    ui->LEDBox->setDisabled(1);
                }

                /*-----------------------------------------*\
                | Fill in the LED list with all LEDs in the |
                | device                                    |
                \*-----------------------------------------*/
                for(std::size_t i = 0; i < device->GetLEDCount(); i++)
                {
                    ui->LEDBox->addItem(device->GetLEDDisplayName((unsigned int)i).c_str());
                }

                /*-----------------------------------------*\
                | Editing is not allowed when all zones are |
                | selected at once                          |
                \*-----------------------------------------*/
                ui->EditZoneButton->setEnabled(false);

                if(!ui->ZoneBox->signalsBlocked())
                {
                    ui->DeviceViewBox->blockSignals(true);
                    ui->DeviceViewBox->clearSelection();
                    ui->DeviceViewBox->blockSignals(false);
                }
            }

            /*---------------------------------------------*\
            | Handle a zone selected condition              |
            \*---------------------------------------------*/
            else if(selected_zone != -1 && selected_segment == -1)
            {
                unsigned int leds_in_zone = device->GetLEDsInZone(selected_zone);

                /*-----------------------------------------*\
                | If there are multiple LEDs, add the       |
                | "Entire Zone" option to the LED box and   |
                | enable it, otherwise there is only one    |
                | LED so disable it                         |
                \*-----------------------------------------*/
                if(leds_in_zone > 1)
                {
                    ui->LEDBox->addItem(tr("Entire Zone"));
                    ui->LEDBox->setEnabled(1);
                }
                else
                {
                    ui->LEDBox->setDisabled(1);
                }

                /*-----------------------------------------*\
                | Fill in the LED list with all LEDs in the |
                | zone                                      |
                \*-----------------------------------------*/
                for(std::size_t led_idx = 0; led_idx < leds_in_zone; led_idx++)
                {
                    ui->LEDBox->addItem(device->GetLEDName(device->GetZoneStartIndex(selected_zone) + led_idx).c_str());
                }

                /*-----------------------------------------*\
                | Enable editing if:                        |
                |   Zone has variable size                  |
                | OR                                        |
                |   Zone is LINEAR and device type is       |
                |   LEDSTRIP                                |
                \*-----------------------------------------*/
                bool zone_is_editable = false;

                if(device->GetZoneLEDsMin(selected_zone) != device->GetZoneLEDsMax(selected_zone))
                {
                    zone_is_editable = true;
                }

                if((device->GetZoneType(selected_zone) == ZONE_TYPE_LINEAR) && (device->GetDeviceType() == DEVICE_TYPE_LEDSTRIP))
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

            /*---------------------------------------------*\
            | Handle a segment selected condition           |
            \*---------------------------------------------*/
            else if(selected_zone != -1 && selected_segment != -1)
            {
                /*-----------------------------------------*\
                | If there are multiple LEDs, add the       |
                | "Entire Segment" option to the LED box    |
                | and enable it, otherwise there is only    |
                | one LED so disable it                     |
                \*-----------------------------------------*/
                if(device->GetZoneSegmentLEDsCount(selected_zone, selected_segment) > 1)
                {
                    ui->LEDBox->addItem(tr("Entire Segment"));
                    ui->LEDBox->setEnabled(1);
                }
                else
                {
                    ui->LEDBox->setDisabled(1);
                }

                /*-----------------------------------------*\
                | Fill in the LED list with all LEDs in the |
                | segment                                   |
                \*-----------------------------------------*/
                for(std::size_t led_idx = 0; led_idx < device->GetZoneSegmentLEDsCount(selected_zone, selected_segment); led_idx++)
                {
                    ui->LEDBox->addItem(device->GetLEDName(device->GetZoneStartIndex(selected_zone) + device->GetZoneSegmentStartIndex(selected_zone, selected_segment) + led_idx).c_str());
                }

                /*-----------------------------------------*\
                | Editing is not allowed when a segment is  |
                | selected                                  |
                \*-----------------------------------------*/
                ui->EditZoneButton->setEnabled(false);

                if(!ui->ZoneBox->signalsBlocked())
                {
                    ui->DeviceViewBox->blockSignals(true);
                    ui->DeviceViewBox->selectSegment(selected_zone, selected_segment);
                    ui->DeviceViewBox->blockSignals(false);
                }
            }

            /*---------------------------------------------*\
            | Enable LED box                                |
            \*---------------------------------------------*/
            ui->LEDBox->setCurrentIndex(0);
            ui->LEDBox->blockSignals(false);
            break;

        case MODE_COLORS_MODE_SPECIFIC:
            {
                /*-----------------------------------------*\
                | Update label                              |
                \*-----------------------------------------*/
                ui->LEDLabel->setText("Color:");

                /*-----------------------------------------*\
                | Get mode colors information               |
                \*-----------------------------------------*/
                unsigned int mode_colors_min;
                unsigned int mode_colors_max;
                unsigned int mode_colors;

                if(selected_zone_mode && (selected_mode >= 0))
                {
                    mode_colors_min = device->GetZoneModeColorsMin(selected_zone, selected_mode);
                    mode_colors_max = device->GetZoneModeColorsMax(selected_zone, selected_mode);
                    mode_colors     = device->GetZoneModeColorsCount(selected_zone, selected_mode);
                }
                else if(selected_zone_mode)
                {
                    mode_colors_min = device->GetModeColorsMin(device->GetActiveMode());
                    mode_colors_max = device->GetModeColorsMax(device->GetActiveMode());
                    mode_colors     = device->GetModeColorsCount(device->GetActiveMode());
                }
                else
                {
                    mode_colors_min = device->GetModeColorsMin(selected_mode);
                    mode_colors_max = device->GetModeColorsMax(selected_mode);
                    mode_colors     = device->GetModeColorsCount(selected_mode);
                }

                ui->SpinBoxModeColors->setMinimum(mode_colors_min);
                ui->SpinBoxModeColors->setMaximum(mode_colors_max);
                ui->SpinBoxModeColors->setValue(mode_colors);

                /*-----------------------------------------*\
                | Swap Select ALl button for Mode Colors    |
                \*-----------------------------------------*/
                ui->SelectAllLEDsButton->setHidden(true);
                ui->SelectAllLEDsButton->setEnabled(false);
                ui->SpinBoxModeColors->setHidden(false);
                ui->SpinBoxModeColors->setEnabled(true);

                /*-----------------------------------------*\
                | Clear LED box                             |
                \*-----------------------------------------*/
                ui->LEDBox->blockSignals(true);
                ui->LEDBox->clear();

                /*-----------------------------------------*\
                | Add entries for each mode-specific color  |
                \*-----------------------------------------*/
                for(unsigned int i = 0; i < mode_colors; i++)
                {
                    char id_buf[64];

                    snprintf(id_buf, 32, "Mode-Specific Color %u", i);
                    ui->LEDBox->addItem(id_buf);
                }

                /*-----------------------------------------*\
                | Enable LED box                            |
                \*-----------------------------------------*/
                ui->LEDBox->setCurrentIndex(0);
                ui->LEDBox->setEnabled(true);
                ui->LEDBox->blockSignals(false);
            }
            break;
    }
}

void OpenRGBDevicePage::UpdateLEDUi()
{
    /*-----------------------------------------------------*\
    | Determine the selected mode                           |
    \*-----------------------------------------------------*/
    bool            selected_zone_mode;
    int             selected_mode;

    GetSelectedMode(&selected_zone_mode, &selected_mode);

    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    bool            selected_all_zones;
    int             selected_zone;
    int             selected_segment;

    GetSelectedZone(&selected_all_zones, &selected_zone, &selected_segment);

    /*-----------------------------------------------------*\
    | Read selected LED                                     |
    \*-----------------------------------------------------*/
    int             index = ui->LEDBox->currentIndex();

    /*-----------------------------------------------------*\
    | Process LED UI based on color mode                    |
    \*-----------------------------------------------------*/
    unsigned int    color_mode;

    if(selected_zone_mode)
    {
        if(selected_mode >= 0)
        {
            color_mode = device->GetZoneModeColorMode(selected_zone, selected_mode);
        }
        else
        {
            color_mode = device->GetModeColorMode(device->GetActiveMode());
        }
    }
    else
    {
        color_mode = device->GetModeColorMode(selected_mode);
    }

    switch(color_mode)
    {
        case MODE_COLORS_PER_LED:
            {
                /*-----------------------------------------*\
                | Initialize both selected zone and segment |
                | to -1 to indicate there is no selection   |
                \*-----------------------------------------*/
                bool            selected_all_leds   = false;
                int             selected_led        = -1;

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
                bool        multiple    = (std::size_t(selected_led) == (device->GetLEDCount() + 1));
                RGBColor    color       = 0x00000000;
                bool        updateColor = false;

                /*-----------------------------------------*\
                | Remove multiple selection                 |
                \*-----------------------------------------*/
                if(MultipleSelected)
                {
                    ui->LEDBox->removeItem((int)(device->GetLEDCount() + 1));
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
                    if((device->GetLEDCount() == 1 || selected_led != -1) && !multiple)
                    {
                        /*---------------------------------*\
                        | Get selected LED's current color  |
                        \*---------------------------------*/
                        color = device->GetColor(selected_led);

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
                            color = device->GetZoneColor(selected_zone, selected_led);

                            /*-----------------------------*\
                            | Set update color flag         |
                            \*-----------------------------*/
                            updateColor = true;

                            /*-----------------------------*\
                            | Set global index              |
                            \*-----------------------------*/
                            int globalIndex = device->GetZoneStartIndex(selected_zone) + selected_led;

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
                    if(device->GetZoneSegmentLEDsCount(selected_zone, selected_segment) == 1 || selected_led != -1)
                    {
                        if((unsigned int)selected_led < device->GetZoneSegmentLEDsCount(selected_zone, selected_segment))
                        {
                            /*-----------------------------*\
                            | Get selected LED's current    |
                            | color                         |
                            \*-----------------------------*/
                            color = device->GetZoneColor(selected_zone, (selected_led + device->GetZoneSegmentStartIndex(selected_zone, selected_segment)));

                            /*-----------------------------*\
                            | Set update color flag         |
                            \*-----------------------------*/
                            updateColor = true;

                            /*-----------------------------*\
                            | Set global index              |
                            \*-----------------------------*/
                            int globalIndex = device->GetZoneStartIndex(selected_zone) + selected_led + device->GetZoneSegmentStartIndex(selected_zone, selected_segment);

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
                | Update color picker with color of         |
                | selected LED                              |
                \*-----------------------------------------*/
                if(updateColor)
                {
                    current_color.setRgb(RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));

                    UpdateColorUi();
                }

                /*-----------------------------------------*\
                | Update Device View                        |
                \*-----------------------------------------*/
                ui->DeviceViewBox->repaint();
            }
            break;

        case MODE_COLORS_MODE_SPECIFIC:
            {
                /*-----------------------------------------*\
                | Update color picker with color of         |
                | selected                                  |
                \*-----------------------------------------*/
                RGBColor color;

                if(selected_zone_mode && (selected_mode >= 0))
                {
                    color = device->GetZoneModeColor(selected_zone, selected_mode, index);
                }
                else if(selected_zone_mode)
                {
                    color = device->GetModeColor(device->GetActiveMode(), index);
                }
                else
                {
                    color = device->GetModeColor(selected_mode, index);
                }

                current_color.setRgb(RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));

                UpdateColorUi();
            }
            break;
        default:
            current_color.setRgb(0, 0, 0);
            UpdateColorUi();
            break;

    }
}

void OpenRGBDevicePage::UpdateMode()
{
    /*-----------------------------------------------------*\
    | Determine the selected mode                           |
    \*-----------------------------------------------------*/
    bool            selected_zone_mode;
    int             selected_mode;

    GetSelectedMode(&selected_zone_mode, &selected_mode);

    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    bool            selected_all_zones;
    int             selected_zone;
    int             selected_segment;

    GetSelectedZone(&selected_all_zones, &selected_zone, &selected_segment);

    if(selected_zone_mode && selected_mode < 0)
    {
        device->SetZoneActiveMode(selected_zone, -1);
    }
    else if(selected_mode >= 0)
    {
        int  current_speed          = 0;
        int  current_brightness     = 0;
        bool current_per_led        = ui->PerLEDCheck->isChecked();
        bool current_mode_specific  = ui->ModeSpecificCheck->isChecked();
        bool current_random         = ui->RandomCheck->isChecked();
        int  current_dir_idx        = ui->DirectionBox->currentIndex();
        int  current_direction      = 0;
        bool supports_dir_lr        = 0;
        bool supports_dir_ud        = 0;
        bool supports_dir_hv        = 0;

        if(selected_zone_mode)
        {
            supports_dir_lr         = ( device->GetZoneModeFlags(selected_zone, (unsigned int)selected_mode) & MODE_FLAG_HAS_DIRECTION_LR );
            supports_dir_ud         = ( device->GetZoneModeFlags(selected_zone, (unsigned int)selected_mode) & MODE_FLAG_HAS_DIRECTION_UD );
            supports_dir_hv         = ( device->GetZoneModeFlags(selected_zone, (unsigned int)selected_mode) & MODE_FLAG_HAS_DIRECTION_HV );
        }
        else
        {
            supports_dir_lr         = ( device->GetModeFlags((unsigned int)selected_mode) & MODE_FLAG_HAS_DIRECTION_LR );
            supports_dir_ud         = ( device->GetModeFlags((unsigned int)selected_mode) & MODE_FLAG_HAS_DIRECTION_UD );
            supports_dir_hv         = ( device->GetModeFlags((unsigned int)selected_mode) & MODE_FLAG_HAS_DIRECTION_HV );
        }

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

            if(selected_zone_mode)
            {
                device->SetZoneModeDirection(selected_zone, (unsigned int)selected_mode, current_direction);
            }
            else
            {
                device->SetModeDirection((unsigned int)selected_mode, current_direction);
            }
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
                if(selected_zone_mode)
                {
                    current_speed = device->GetZoneModeSpeedMin(selected_zone, (unsigned int)selected_mode) - ui->SpeedSlider->value() + device->GetZoneModeSpeedMax(selected_zone, (unsigned int)selected_mode);
                }
                else
                {
                    current_speed = device->GetModeSpeedMin((unsigned int)selected_mode) - ui->SpeedSlider->value() + device->GetModeSpeedMax((unsigned int)selected_mode);
                }
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
                if(selected_zone_mode)
                {
                    current_brightness = device->GetZoneModeBrightnessMin(selected_zone, (unsigned int)selected_mode) - ui->BrightnessSlider->value() + device->GetZoneModeBrightnessMax(selected_zone, (unsigned int)selected_mode);
                }
                else
                {
                    current_brightness = device->GetModeBrightnessMin((unsigned int)selected_mode) - ui->BrightnessSlider->value() + device->GetModeBrightnessMax((unsigned int)selected_mode);
                }
            }
            else
            {
                current_brightness = ui->BrightnessSlider->value();
            }
        }

        /*-----------------------------------------------------*\
        | Don't set the mode if the current mode is invalid     |
        \*-----------------------------------------------------*/
        std::size_t mode_count = 0;

        if(selected_zone_mode)
        {
            mode_count = device->GetZoneModeCount(selected_zone);
        }
        else
        {
            mode_count = device->GetModeCount();
        }

        if((std::size_t)selected_mode < mode_count)
        {
            /*-----------------------------------------------------*\
            | Update mode parameters                                |
            \*-----------------------------------------------------*/
            if(selected_zone_mode)
            {
                device->SetZoneModeSpeed(selected_zone, (unsigned int)selected_mode, current_speed);
                device->SetZoneModeBrightness(selected_zone, (unsigned int)selected_mode, current_brightness);

                if(current_per_led)
                {
                    device->SetZoneModeColorMode(selected_zone, (unsigned int)selected_mode, MODE_COLORS_PER_LED);
                }
                else if(current_mode_specific)
                {
                    device->SetZoneModeColorMode(selected_zone, (unsigned int)selected_mode, MODE_COLORS_MODE_SPECIFIC);
                }
                else if(current_random)
                {
                    device->SetZoneModeColorMode(selected_zone, (unsigned int)selected_mode, MODE_COLORS_RANDOM);
                }
                else
                {
                    device->SetZoneModeColorMode(selected_zone, (unsigned int)selected_mode, MODE_COLORS_NONE);
                }

                /*-----------------------------------------------------*\
                | Change device mode                                    |
                \*-----------------------------------------------------*/
                device->SetZoneActiveMode(selected_zone, (unsigned int)selected_mode);

                if(device->GetZoneModeColorMode(selected_zone, (unsigned int)selected_mode) == MODE_COLORS_PER_LED)
                {
                    device->UpdateZoneLEDs(selected_zone);
                }
            }
            else
            {
                device->SetModeSpeed((unsigned int)selected_mode, current_speed);
                device->SetModeBrightness((unsigned int)selected_mode, current_brightness);

                if(current_per_led)
                {
                    device->SetModeColorMode((unsigned int)selected_mode, MODE_COLORS_PER_LED);
                }
                else if(current_mode_specific)
                {
                    device->SetModeColorMode((unsigned int)selected_mode, MODE_COLORS_MODE_SPECIFIC);
                }
                else if(current_random)
                {
                    device->SetModeColorMode((unsigned int)selected_mode, MODE_COLORS_RANDOM);
                }
                else
                {
                    device->SetModeColorMode((unsigned int)selected_mode, MODE_COLORS_NONE);
                }

                /*-----------------------------------------------------*\
                | Change device mode                                    |
                \*-----------------------------------------------------*/
                device->SetActiveMode((unsigned int)selected_mode);

                if(device->GetModeColorMode((unsigned int)selected_mode) == MODE_COLORS_PER_LED)
                {
                    device->UpdateLEDs();
                }
            }
        }
    }
}

void OpenRGBDevicePage::UpdateModeList()
{
    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    bool            selected_all_zones;
    int             selected_zone;
    int             selected_segment;

    GetSelectedZone(&selected_all_zones, &selected_zone, &selected_segment);

    /*-----------------------------------------------------*\
    | Fill in the mode list                                 |
    | If the device supports per-zone modes and a zone is   |
    | selected, add a default option                        |
    \*-----------------------------------------------------*/
    ui->ModeBox->blockSignals(true);
    ui->ModeBox->clear();

    std::size_t entry_count = 0;

    if(!selected_all_zones && !(device->GetModeFlags(device->GetActiveMode()) & MODE_FLAG_REQUIRES_ENTIRE_DEVICE) && (selected_zone >= 0) && device->SupportsPerZoneModes())
    {
        ui->ModeBox->addItem("Follow Device Mode");
        ui->ModeBox->setItemData((int)entry_count, "Follow the device's global mode", Qt::ToolTipRole);

        entry_count++;

        for(std::size_t i = 0; i < device->GetZoneModeCount(selected_zone); i++)
        {
            ui->ModeBox->addItem(device->GetZoneModeName(selected_zone, (unsigned int)i).c_str());
            ui->ModeBox->setItemData((int)entry_count, ModeDescription(device->GetZoneModeName(selected_zone, i)), Qt::ToolTipRole);

            entry_count++;
        }

        ui->ModeBox->setCurrentIndex(device->GetZoneActiveMode(selected_zone) + 1);
        ui->ModeBox->setEnabled(true);
    }
    else
    {
        for(std::size_t i = 0; i < device->GetModeCount(); i++)
        {
            ui->ModeBox->addItem(device->GetModeName((unsigned int)i).c_str());
            ui->ModeBox->setItemData((int)entry_count, ModeDescription(device->GetModeName(i)), Qt::ToolTipRole);

            entry_count++;
        }

        ui->ModeBox->setCurrentIndex(device->GetActiveMode());
        ui->ModeBox->setEnabled(selected_all_zones);
    }

    ui->ModeBox->blockSignals(false);
}

void OpenRGBDevicePage::UpdateModeUi()
{
    /*-----------------------------------------------------*\
    | Determine the selected mode                           |
    \*-----------------------------------------------------*/
    bool            selected_zone_mode;
    int             selected_mode;

    GetSelectedMode(&selected_zone_mode, &selected_mode);

    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    bool            selected_all_zones;
    int             selected_zone;
    int             selected_segment;

    GetSelectedZone(&selected_all_zones, &selected_zone, &selected_segment);

    /*-----------------------------------------------------*\
    | Read mode variables                                   |
    \*-----------------------------------------------------*/
    bool            enable_controls;
    unsigned int    brightness;
    unsigned int    brightness_min;
    unsigned int    brightness_max;
    unsigned int    color_mode;
    unsigned int    direction;
    unsigned int    flags;
    unsigned int    speed;
    unsigned int    speed_min;
    unsigned int    speed_max;

    if(selected_zone_mode)
    {
        if((selected_mode >= 0) && (selected_mode < (int)device->GetZoneModeCount(selected_zone)))
        {
            enable_controls     = true;
            brightness          = device->GetZoneModeBrightness(selected_zone, selected_mode);
            brightness_min      = device->GetZoneModeBrightnessMin(selected_zone, selected_mode);
            brightness_max      = device->GetZoneModeBrightnessMax(selected_zone, selected_mode);
            color_mode          = device->GetZoneModeColorMode(selected_zone, selected_mode);
            direction           = device->GetZoneModeDirection(selected_zone, selected_mode);
            flags               = device->GetZoneModeFlags(selected_zone, selected_mode);
            speed               = device->GetZoneModeSpeed(selected_zone, selected_mode);
            speed_min           = device->GetZoneModeSpeedMin(selected_zone, selected_mode);
            speed_max           = device->GetZoneModeSpeedMax(selected_zone, selected_mode);
        }
        else
        {
            enable_controls     = false;
            brightness          = 0;
            brightness_min      = 0;
            brightness_max      = 0;
            color_mode          = 0;
            direction           = 0;
            flags               = 0;
            speed               = 0;
            speed_min           = 0;
            speed_max           = 0;
        }
    }
    else if(selected_mode < (int)device->GetModeCount())
    {
        enable_controls         = selected_all_zones;
        brightness              = device->GetModeBrightness(selected_mode);
        brightness_min          = device->GetModeBrightnessMin(selected_mode);
        brightness_max          = device->GetModeBrightnessMax(selected_mode);
        color_mode              = device->GetModeColorMode(selected_mode);
        direction               = device->GetModeDirection(selected_mode);
        flags                   = device->GetModeFlags(selected_mode);
        speed                   = device->GetModeSpeed(selected_mode);
        speed_min               = device->GetModeSpeedMin(selected_mode);
        speed_max               = device->GetModeSpeedMax(selected_mode);
    }
    else
    {
        /*-------------------------------------------------*\
        | Don't update the UI if the current mode is invalid|
        \*-------------------------------------------------*/
        return;
    }

    /*-----------------------------------------------------*\
    | Read mode flags                                       |
    \*-----------------------------------------------------*/
    bool    supports_per_led        = flags & MODE_FLAG_HAS_PER_LED_COLOR;
    bool    supports_mode_specific  = flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    bool    supports_random         = flags & MODE_FLAG_HAS_RANDOM_COLOR;
    bool    supports_speed          = flags & MODE_FLAG_HAS_SPEED;
    bool    supports_brightness     = flags & MODE_FLAG_HAS_BRIGHTNESS;
    bool    supports_dir_lr         = flags & MODE_FLAG_HAS_DIRECTION_LR;
    bool    supports_dir_ud         = flags & MODE_FLAG_HAS_DIRECTION_UD;
    bool    supports_dir_hv         = flags & MODE_FLAG_HAS_DIRECTION_HV;
    bool    per_led                 = color_mode == MODE_COLORS_PER_LED;
    bool    mode_specific           = color_mode == MODE_COLORS_MODE_SPECIFIC;
    bool    random                  = color_mode == MODE_COLORS_RANDOM;
    bool    manual_save             = flags & MODE_FLAG_MANUAL_SAVE;
    bool    automatic_save          = flags & MODE_FLAG_AUTOMATIC_SAVE;

    if(supports_speed)
    {
        ui->SpeedSlider->blockSignals(true);
        int  current_speed;
        InvertedSpeed = speed_min > speed_max;

        if(InvertedSpeed)
        {
            /*-----------------------------------------------------*\
            | If Speed Slider is inverted, invert value             |
            \*-----------------------------------------------------*/
            ui->SpeedSlider->setMinimum(speed_max);
            ui->SpeedSlider->setMaximum(speed_min);
            current_speed = speed_min - speed + speed_max;
        }
        else
        {
            ui->SpeedSlider->setMinimum(speed_min);
            ui->SpeedSlider->setMaximum(speed_max);
            current_speed = speed;
        }

        ui->SpeedSlider->setValue(current_speed);
        ui->SpeedSlider->setEnabled(enable_controls);
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
        InvertedBrightness = brightness_min > brightness_max;

        if(InvertedBrightness)
        {
            /*-----------------------------------------------------*\
            | If Brightness Slider is inverted, invert value        |
            \*-----------------------------------------------------*/
            ui->BrightnessSlider->setMinimum(brightness_max);
            ui->BrightnessSlider->setMaximum(brightness_min);
            current_brightness = brightness_min - brightness + brightness_max;
        }
        else
        {
            ui->BrightnessSlider->setMinimum(brightness_min);
            ui->BrightnessSlider->setMaximum(brightness_max);
            current_brightness = brightness;
        }

        ui->BrightnessSlider->setValue(current_brightness);
        ui->BrightnessSlider->setEnabled(enable_controls);
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
         &&((direction == MODE_DIRECTION_LEFT)
          ||(direction == MODE_DIRECTION_RIGHT)))
        {
            ui->DirectionBox->setCurrentIndex(direction);
        }

        if((supports_dir_ud)
         &&((direction == MODE_DIRECTION_UP)
          ||(direction == MODE_DIRECTION_DOWN)))
        {
            if(supports_dir_lr)
            {
                ui->DirectionBox->setCurrentIndex(direction);
            }
            else
            {
                ui->DirectionBox->setCurrentIndex(direction - 2);
            }
        }

        if((supports_dir_hv)
         &&((direction == MODE_DIRECTION_HORIZONTAL)
          ||(direction == MODE_DIRECTION_VERTICAL)))
        {
            if(supports_dir_lr && supports_dir_ud)
            {
                ui->DirectionBox->setCurrentIndex(direction);
            }
            else if(supports_dir_lr || supports_dir_ud)
            {
                ui->DirectionBox->setCurrentIndex(direction - 2);
            }
            else
            {
                ui->DirectionBox->setCurrentIndex(direction - 4);
            }
        }

        ui->DirectionBox->setEnabled(enable_controls);
    }
    else
    {
        ui->DirectionBox->setEnabled(false);
    }

    ui->DirectionBox->blockSignals(false);

    if(supports_per_led)
    {
        ui->PerLEDCheck->setEnabled(enable_controls);
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
        ui->ModeSpecificCheck->setEnabled(enable_controls);
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
        ui->RandomCheck->setEnabled(enable_controls);
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
        ui->DeviceSaveButton->setEnabled(enable_controls);
    }
    else
    {
        ui->DeviceSaveButton->setText(tr("Saving Not Supported"));
        ui->DeviceSaveButton->setEnabled(false);
    }

    /*-----------------------------------------------------*\
    | Enable the Apply Colors button if a mode with colors  |
    | is selected and automatic updates are not enabled     |
    \*-----------------------------------------------------*/
    if(supports_mode_specific || supports_per_led)
    {
        ui->ApplyColorsButton->setEnabled(enable_controls && (!AutoUpdateEnabled()));
    }
    else
    {
        ui->ApplyColorsButton->setEnabled(false);
    }

    /*-----------------------------------------------------*\
    | Enable the Set All Colors button if the selected mode |
    | is "Direct", "Custom", or "Static"                    |
    \*-----------------------------------------------------*/
    std::string selected_mode_string;

    if(!selected_zone_mode)
    {
        selected_mode_string = device->GetModeName(selected_mode);
    }
    else if(selected_zone_mode && (selected_mode >= 0))
    {
        selected_mode_string = device->GetZoneModeName(selected_zone, selected_mode);
    }

    ui->SetAllButton->setDisabled((selected_mode_string != "Direct") && (selected_mode_string != "Custom") && (selected_mode_string != "Static"));

#if 0
        /*-----------------------------------------------------*\
        | Fill in the zone box based on color mode              |
        \*-----------------------------------------------------*/
        switch(color_mode)
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

                if(device->GetZoneCount() > 1)
                {
                    ui->ZoneBox->setEnabled(1);
                    ui->ZoneBox->addItem(tr("All Zones"));
                }
                else if(device->GetZoneCount() == 1 && device->GetZoneSegmentCount(0) > 1)
                {
                    ui->ZoneBox->setEnabled(1);
                }
                else
                {
                    ui->ZoneBox->setDisabled(1);
                    ui->EditZoneButton->setEnabled(false);
                }

                for(std::size_t zone_idx = 0; zone_idx < device->GetZoneCount(); zone_idx++)
                {
                    ui->ZoneBox->addItem(device->GetZoneName((unsigned int)zone_idx).c_str());

                    for(std::size_t segment_idx = 0; segment_idx < device->GetZoneSegmentCount(zone_idx); segment_idx++)
                    {
                        ui->ZoneBox->addItem(("    " + device->GetZoneSegmentName(zone_idx, segment_idx)).c_str());
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

                int led_index = ui->LEDBox->currentIndex();

                ui->LEDBox->blockSignals(true);
                ui->LEDBox->clear();

                if(device->GetModeColorsMin(selected_mode) == device->GetModeColorsMax(selected_mode))
                {
                    ui->EditZoneButton->setEnabled(false);
                }
                else
                {
                    ui->EditZoneButton->setEnabled(true);
                }

                for(unsigned int i = 0; i < device->GetModeColorsCount(selected_mode); i++)
                {
                    char id_buf[32];
                    // TODO: translate
                    snprintf(id_buf, 32, "Mode Color %u", i);
                    ui->LEDBox->addItem(id_buf);
                }

                if(led_index >= ui->LEDBox->count())
                {
                    led_index = 0;
                }

                ui->LEDBox->setCurrentIndex(led_index);
                on_LEDBox_currentIndexChanged(led_index);
                ui->LEDBox->setEnabled(true);
                ui->LEDBox->blockSignals(false);
                ui->ApplyColorsButton->setEnabled(true);
                break;
        }
#endif
}

void OpenRGBDevicePage::UpdateZoneList()
{
    ui->ZoneBox->blockSignals(true);
    ui->ZoneBox->clear();

    if(device->GetZoneCount() > 1)
    {
        ui->ZoneBox->setEnabled(1);
        ui->ZoneBox->addItem(tr("Entire Device"));
    }
    else if(device->GetZoneCount() == 1 && device->GetZoneSegmentCount(0) > 1)
    {
        ui->ZoneBox->setEnabled(1);
    }
    else
    {
        ui->ZoneBox->setDisabled(1);
        ui->EditZoneButton->setEnabled(false);
    }

    for(std::size_t zone_idx = 0; zone_idx < device->GetZoneCount(); zone_idx++)
    {
        ui->ZoneBox->addItem(device->GetZoneName((unsigned int)zone_idx).c_str());

        for(std::size_t segment_idx = 0; segment_idx < device->GetZoneSegmentCount(zone_idx); segment_idx++)
        {
            ui->ZoneBox->addItem(("    " + device->GetZoneSegmentName(zone_idx, segment_idx)).c_str());
        }
    }

    ui->ZoneBox->setCurrentIndex(0);
    ui->ZoneBox->blockSignals(false);
    ui->ApplyColorsButton->setEnabled(true);

    /*-----------------------------------------------------*\
    | Update color picker with color of first LED           |
    \*-----------------------------------------------------*/
    //on_LEDBox_currentIndexChanged(0);
}

/*---------------------------------------------------------*\
| Helper functions                                          |
\*---------------------------------------------------------*/
bool OpenRGBDevicePage::AutoUpdateEnabled()
{
    /*-----------------------------------------------------*\
    | Determine the selected mode                           |
    \*-----------------------------------------------------*/
    bool            selected_zone_mode;
    int             selected_mode;

    GetSelectedMode(&selected_zone_mode, &selected_mode);

    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    bool            selected_all_zones;
    int             selected_zone;
    int             selected_segment;

    GetSelectedZone(&selected_all_zones, &selected_zone, &selected_segment);

    if(selected_zone_mode && (selected_mode >= 0))
    {
        return(!(device->GetZoneModeFlags(selected_zone, selected_mode) & MODE_FLAG_AUTOMATIC_SAVE));
    }
    else if(selected_zone_mode)
    {
        return(!(device->GetModeFlags(device->GetActiveMode()) & MODE_FLAG_AUTOMATIC_SAVE));
    }
    else
    {
        return(!(device->GetModeFlags(selected_mode) & MODE_FLAG_AUTOMATIC_SAVE));
    }
}

void OpenRGBDevicePage::ColorChanged()
{
    UpdateColorUi();

    if(AutoUpdateEnabled())
    {
        UpdateColor();
    }
}

void OpenRGBDevicePage::GetSelectedMode(bool * selected_zone_mode, int * selected_mode)
{
    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    bool            selected_all_zones;
    int             selected_zone;
    int             selected_segment;

    GetSelectedZone(&selected_all_zones, &selected_zone, &selected_segment);

    if(selected_all_zones || !device->SupportsPerZoneModes() || (device->GetModeFlags(device->GetActiveMode()) & MODE_FLAG_REQUIRES_ENTIRE_DEVICE))
    {
        *selected_zone_mode = false;
        *selected_mode      = ui->ModeBox->currentIndex();
    }
    else
    {
        *selected_zone_mode = true;
        *selected_mode      = ui->ModeBox->currentIndex() - 1;
    }
}

void OpenRGBDevicePage::GetSelectedZone(bool * selected_all_zones, int * selected_zone, int * selected_segment)
{
    /*-----------------------------------------------------*\
    | Get the current index of the zone box                 |
    \*-----------------------------------------------------*/
    int current_index       = 0;
    int index               = ui->ZoneBox->currentIndex();

    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    *selected_all_zones     = false;
    *selected_zone          = -1;
    *selected_segment       = -1;

    /*-----------------------------------------------------*\
    | Handle condition where device has more than one zone, |
    | which adds an "All Zones" entry to the Zone menu in   |
    | the first index                                       |
    \*-----------------------------------------------------*/
    if(device->GetZoneCount() > 1)
    {
        if(index == current_index)
        {
            *selected_all_zones = true;
        }

        current_index++;
    }

    if(!(*selected_all_zones))
    {
        for(std::size_t zone_idx = 0; zone_idx < device->GetZoneCount(); zone_idx++)
        {
            if(index == (int)current_index)
            {
                *selected_zone = (int)zone_idx;
                break;
            }

            current_index++;

            for(std::size_t segment_idx = 0; segment_idx < device->GetZoneSegmentCount(zone_idx); segment_idx++)
            {
                if(index == (int)current_index)
                {
                    *selected_zone    = (int)zone_idx;
                    *selected_segment = (int)segment_idx;
                    break;
                }

                current_index++;
            }

            if(*selected_segment != -1)
            {
                break;
            }
        }

        if((*selected_segment == -1) && (device->GetZoneCount() <= 1))
        {
            *selected_all_zones = true;
        }
    }
}

QString OpenRGBDevicePage::ModeDescription(const std::string mode_name)
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
    std::unordered_map<std::string, QString>::const_iterator it = descriptions.find(mode_name);

    if(it != descriptions.end())
    {
        return it->second;
    }

    return "";
}

/*---------------------------------------------------------*\
| Callback handler                                          |
\*---------------------------------------------------------*/
void OpenRGBDevicePage::UpdateInterface(unsigned int update_reason)
{
    switch(update_reason)
    {
    case RGBCONTROLLER_UPDATE_REASON_HIDDEN:
    case RGBCONTROLLER_UPDATE_REASON_UNHIDDEN:
        emit RefreshList();
        break;

    case RGBCONTROLLER_UPDATE_REASON_UPDATELEDS:
        /*-------------------------------------------------*\
        | Update LED UI                                     |
        \*-------------------------------------------------*/
        UpdateLEDUi();
        break;

    case RGBCONTROLLER_UPDATE_REASON_UPDATEMODE:
    case RGBCONTROLLER_UPDATE_REASON_SAVEMODE:
        /*-------------------------------------------------*\
        | Update mode list to update selected mode          |
        \*-------------------------------------------------*/
        UpdateModeList();

        /*-------------------------------------------------*\
        | Update mode user interface elements               |
        \*-------------------------------------------------*/
        UpdateModeUi();

        /*-------------------------------------------------*\
        | Update LED list                                   |
        \*-------------------------------------------------*/
        UpdateLEDList();

        /*-------------------------------------------------*\
        | Update LED UI                                     |
        \*-------------------------------------------------*/
        UpdateLEDUi();
        break;

    case RGBCONTROLLER_UPDATE_REASON_ADDSEGMENT:
    case RGBCONTROLLER_UPDATE_REASON_CLEARSEGMENTS:
    case RGBCONTROLLER_UPDATE_REASON_RESIZEZONE:
        UpdateModeUi();
        ui->DeviceViewBox->repaint();
        break;
    }
}

/*---------------------------------------------------------*\
| Retranslate Event Slot                                    |
\*---------------------------------------------------------*/
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

/*---------------------------------------------------------*\
| UI Control Slots                                          |
\*---------------------------------------------------------*/
void OpenRGBDevicePage::on_ApplyColorsButton_clicked()
{
    UpdateColor();
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
    ColorChanged();
}

void OpenRGBDevicePage::on_BrightnessSlider_valueChanged(int /*value*/)
{
    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();
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
    ColorChanged();
}

void OpenRGBDevicePage::on_DeviceSaveButton_clicked()
{
    if(device->GetModeFlags(device->GetActiveMode()) & MODE_FLAG_MANUAL_SAVE)
    {
        device->SaveMode();
    }
}

void OpenRGBDevicePage::on_DeviceViewBox_selectionChanged(QVector<int> indices)
{
    if(device->GetModeColorMode(device->GetActiveMode()) == MODE_COLORS_PER_LED)
    {
        ui->ZoneBox->blockSignals(true);
        ui->LEDBox->blockSignals(true);
        ui->ZoneBox->setCurrentIndex(0);
        on_ZoneBox_currentIndexChanged(0);
        //updateLeds(); // We want to update the LED box, but we don't want any of the side effects of that action
        ui->ZoneBox->blockSignals(false);
        if(indices.size() != 0 && size_t(indices.size()) != device->GetLEDCount())
        {
            if(indices.size() == 1)
            {
                if(device->GetLEDCount() == 1)
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
                    ui->LEDBox->removeItem((int)(device->GetLEDCount() + 1));
                }
                // TODO: translate
                ui->LEDBox->addItem("Multiple (" + QVariant(indices.size()).toString() + ")");
                ui->LEDBox->setCurrentIndex((int)(device->GetLEDCount() + 1));
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

void OpenRGBDevicePage::on_DirectionBox_currentIndexChanged(int /*index*/)
{
    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();
}

void OpenRGBDevicePage::on_EditZoneButton_clicked()
{
    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    bool            selected_all_zones;
    int             selected_zone;
    int             selected_segment;

    GetSelectedZone(&selected_all_zones, &selected_zone, &selected_segment);

    switch(device->GetModeColorMode(device->GetActiveMode()))
    {
        case MODE_COLORS_PER_LED:
            {
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
                if((device->GetZoneType(selected_zone) == ZONE_TYPE_LINEAR) || (device->GetZoneFlags(selected_zone) & ZONE_FLAG_RESIZE_EFFECTS_ONLY))
                {
                    OpenRGBZoneResizeDialog dlg(device, selected_zone);

                    int new_size = dlg.show();

                    if(new_size >= 0)
                    {
                        /*---------------------------------*\
                        | Update Zone box                   |
                        \*---------------------------------*/
                        on_ZoneBox_currentIndexChanged(selected_zone);

                        /*---------------------------------*\
                        | Update color picker with color of |
                        | first LED                         |
                        \*---------------------------------*/
                        on_LEDBox_currentIndexChanged(0);

                        /*---------------------------------*\
                        | Save the size profile             |
                        \*---------------------------------*/
                        SaveSizeProfile();
                    }
                }
            }
            break;
    }
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
    ColorChanged();
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
    ColorChanged();
    UpdateHex = true;
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
    ColorChanged();
}

void OpenRGBDevicePage::on_LEDBox_currentIndexChanged(int /*index*/)
{
    /*-----------------------------------------------------*\
    | Update LED UI                                         |
    \*-----------------------------------------------------*/
    UpdateLEDUi();
}

void OpenRGBDevicePage::on_ModeBox_currentIndexChanged(int /*index*/)
{
    /*-----------------------------------------------------*\
    | Update mode user interface elements                   |
    \*-----------------------------------------------------*/
    UpdateModeUi();

    /*-----------------------------------------------------*\
    | Update LED list                                       |
    \*-----------------------------------------------------*/
    UpdateLEDList();

    /*-----------------------------------------------------*\
    | Update LED UI                                         |
    \*-----------------------------------------------------*/
    UpdateLEDUi();

    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();
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

    /*-----------------------------------------------------*\
    | Update LED list                                       |
    \*-----------------------------------------------------*/
    UpdateLEDList();

    /*-----------------------------------------------------*\
    | Update LED UI                                         |
    \*-----------------------------------------------------*/
    UpdateLEDUi();
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

    /*-----------------------------------------------------*\
    | Update LED list                                       |
    \*-----------------------------------------------------*/
    UpdateLEDList();

    /*-----------------------------------------------------*\
    | Update LED UI                                         |
    \*-----------------------------------------------------*/
    UpdateLEDUi();
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

    /*-----------------------------------------------------*\
    | Update LED list                                       |
    \*-----------------------------------------------------*/
    UpdateLEDList();

    /*-----------------------------------------------------*\
    | Update LED UI                                         |
    \*-----------------------------------------------------*/
    UpdateLEDUi();
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
    ColorChanged();
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
    ColorChanged();
}

void OpenRGBDevicePage::on_SelectAllLEDsButton_clicked()
{
    /*-----------------------------------------------------*\
    | Determine the selected mode                           |
    \*-----------------------------------------------------*/
    bool            selected_zone_mode;
    int             selected_mode;

    GetSelectedMode(&selected_zone_mode, &selected_mode);

    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    bool            selected_all_zones;
    int             selected_zone;
    int             selected_segment;

    GetSelectedZone(&selected_all_zones, &selected_zone, &selected_segment);

    unsigned int    color_mode;

    if(selected_zone_mode && (selected_mode >= 0))
    {
        color_mode = device->GetZoneModeColorMode(selected_zone, selected_mode);
    }
    else if(selected_zone_mode)
    {
        color_mode = device->GetModeColorMode(device->GetActiveMode());
    }
    else
    {
        color_mode = device->GetModeColorMode(selected_mode);
    }

    switch(color_mode)
    {
        case MODE_COLORS_PER_LED:
            ui->LEDBox->setCurrentIndex(0);
            on_LEDBox_currentIndexChanged(0);
            ui->DeviceViewBox->repaint();
            break;
    }
}

void OpenRGBDevicePage::on_SetAllButton_clicked()
{
    emit SetAllDevices(current_color.red(), current_color.green(), current_color.blue());
}

void OpenRGBDevicePage::on_SpeedSlider_valueChanged(int /*value*/)
{
    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();
}

void OpenRGBDevicePage::on_SpinBoxModeColors_valueChanged(int count)
{
    /*-----------------------------------------------------*\
    | Determine the selected mode                           |
    \*-----------------------------------------------------*/
    bool            selected_zone_mode;
    int             selected_mode;

    GetSelectedMode(&selected_zone_mode, &selected_mode);

    /*-----------------------------------------------------*\
    | Determine what is selected, either all zones, a zone, |
    | or a segment                                          |
    \*-----------------------------------------------------*/
    bool            selected_all_zones;
    int             selected_zone;
    int             selected_segment;

    GetSelectedZone(&selected_all_zones, &selected_zone, &selected_segment);

    unsigned int    color_mode;

    if(selected_zone_mode && (selected_mode >= 0))
    {
        color_mode = device->GetZoneModeColorMode(selected_zone, selected_mode);
    }
    else if(selected_zone_mode)
    {
        color_mode = device->GetModeColorMode(device->GetActiveMode());
    }
    else
    {
        color_mode = device->GetModeColorMode(selected_mode);
    }

    switch(color_mode)
    {
        case MODE_COLORS_MODE_SPECIFIC:
            {
                /*-----------------------------------------*\
                | Get mode colors information               |
                \*-----------------------------------------*/
                unsigned int mode_colors_min;
                unsigned int mode_colors_max;
                unsigned int mode_colors;

                if(selected_zone_mode && (selected_mode >= 0))
                {
                    mode_colors_min = device->GetZoneModeColorsMin(selected_zone, selected_mode);
                    mode_colors_max = device->GetZoneModeColorsMax(selected_zone, selected_mode);
                    mode_colors     = device->GetZoneModeColorsCount(selected_zone, selected_mode);
                }
                else if(selected_zone_mode)
                {
                    mode_colors_min = device->GetModeColorsMin(device->GetActiveMode());
                    mode_colors_max = device->GetModeColorsMax(device->GetActiveMode());
                    mode_colors     = device->GetModeColorsCount(device->GetActiveMode());
                }
                else
                {
                    mode_colors_min = device->GetModeColorsMin(selected_mode);
                    mode_colors_max = device->GetModeColorsMax(selected_mode);
                    mode_colors     = device->GetModeColorsCount(selected_mode);
                }

                if((count >= mode_colors_min) && (count <= mode_colors_max))
                {
                    if(selected_zone_mode && (selected_mode >= 0))
                    {
                        device->SetZoneModeColorsCount(selected_zone, device->GetZoneActiveMode(selected_zone), count);
                    }
                    else
                    {
                        device->SetModeColorsCount(device->GetActiveMode(), count);
                    }
                }

                UpdateModeUi();
                UpdateLEDList();
                UpdateLEDUi();
                UpdateMode();
            }
            break;
    }
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
    ColorChanged();
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
    ColorChanged();
}

void OpenRGBDevicePage::on_ZoneBox_currentIndexChanged(int /*index*/)
{
    /*-----------------------------------------------------*\
    | Update mode list                                      |
    \*-----------------------------------------------------*/
    UpdateModeList();

    /*-----------------------------------------------------*\
    | Update mode user interface elements                   |
    \*-----------------------------------------------------*/
    UpdateModeUi();

    /*-----------------------------------------------------*\
    | Update LED list                                       |
    \*-----------------------------------------------------*/
    UpdateLEDList();

    /*-----------------------------------------------------*\
    | Update LED UI                                         |
    \*-----------------------------------------------------*/
    UpdateLEDUi();
}

