#include "OpenRGBDialog2.h"
#include "OpenRGBDevicePage.h"
#include "OpenRGBZoneResizeDialog.h"
#include "ResourceManager.h"
#include "hsv.h"

using namespace Ui;

static void UpdateCallback(void * this_ptr)
{
    OpenRGBDevicePage * this_obj = (OpenRGBDevicePage *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "UpdateInterface", Qt::QueuedConnection);
}

static QString ModeDescription(const mode& m)
{
    /*-----------------------------------------------------------------*\
    | List of common mode names can be found on the OpenRGB Wiki:       |
    | https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/Common-Modes   |
    \*-----------------------------------------------------------------*/
    static const std::unordered_map<std::string, QString> descriptions =
    {
        {"Direct",          "Set individual LEDs to static colors.  Safe for use with software-driven effects."                     },
        {"Custom",          "Set individual LEDs to static colors.  Not safe for use with software-driven effects."                 },
        {"Static",          "Sets the entire device or a zone to a single color."                                                   },
        {"Breathing",       "Gradually fades between fully off and fully on."                                                       },
        {"Flashing",        "Abruptly changes between fully off and fully on."                                                      },
        {"Spectrum Cycle",  "Gradually cycles through the entire color spectrum.  All lights on the device are the same color."     },
        {"Rainbow Wave",    "Gradually cycles through the entire color spectrum.  Produces a rainbow pattern that moves."           },
        {"Reactive",        "Flashes lights when keys or buttons are pressed."                                                      },
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
    ui(new Ui::OpenRGBDevicePageUi)
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
    | numerical labels setting                              |
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

    ui->DeviceViewBox->setController(device);
    ui->DeviceViewBox->hide();
    
    /*-----------------------------------------------------*\
    | Set up the color palette buttons                      |
    \*-----------------------------------------------------*/
    ui->ButtonBlack->setStyleSheet("QPushButton {background-color: rgb(0,0,0); color: rgb(0,0,0); border: 1px solid rgb(128, 128, 128); padding-top: 1px; padding-bottom: 1px;}");
    ui->ButtonBlack->setFlat(true);
    ui->ButtonBlack->setMinimumWidth(20);
    ui->ButtonBlack->update();

    ui->ButtonRed->setStyleSheet("QPushButton {background-color: rgb(255,0,0); color: rgb(255,0,0); border: 1px solid rgb(128, 128, 128); padding-top: 1px; padding-bottom: 1px;}");
    ui->ButtonRed->setFlat(true);
    ui->ButtonRed->setMinimumWidth(20);
    ui->ButtonRed->update();

    ui->ButtonYellow->setStyleSheet("QPushButton {background-color: rgb(255,255,0); color: rgb(255,255,0); border: 1px solid rgb(128, 128, 128); padding-top: 1px; padding-bottom: 1px;}");
    ui->ButtonYellow->setFlat(true);
    ui->ButtonYellow->setMinimumWidth(20);
    ui->ButtonYellow->update();

    ui->ButtonGreen->setStyleSheet("QPushButton {background-color: rgb(0,255,0); color: rgb(0,255,0); border: 1px solid rgb(128, 128, 128); padding-top: 1px; padding-bottom: 1px;}");
    ui->ButtonGreen->setFlat(true);
    ui->ButtonGreen->setMinimumWidth(20);
    ui->ButtonGreen->update();

    ui->ButtonCyan->setStyleSheet("QPushButton {background-color: rgb(0,255,255); color: rgb(0,255,255); border: 1px solid rgb(128, 128, 128); padding-top: 1px; padding-bottom: 1px;}");
    ui->ButtonCyan->setFlat(true);
    ui->ButtonCyan->setMinimumWidth(20);
    ui->ButtonCyan->update();

    ui->ButtonBlue->setStyleSheet("QPushButton {background-color: rgb(0,0,255); color: rgb(0,0,255); border: 1px solid rgb(128, 128, 128); padding-top: 1px; padding-bottom: 1px;}");
    ui->ButtonBlue->setFlat(true);
    ui->ButtonBlue->setMinimumWidth(20);
    ui->ButtonBlue->update();

    ui->ButtonMagenta->setStyleSheet("QPushButton {background-color: rgb(255,0,255); color: rgb(255,0,255); border: 1px solid rgb(128, 128, 128); padding-top: 1px; padding-bottom: 1px;}");
    ui->ButtonMagenta->setFlat(true);
    ui->ButtonMagenta->setMinimumWidth(20);
    ui->ButtonMagenta->update();

    ui->ButtonWhite->setStyleSheet("QPushButton {background-color: rgb(255,255,255); color: rgb(255,255,255); border: 1px solid rgb(128, 128, 128); padding-top: 1px; padding-bottom: 1px;}");
    ui->ButtonWhite->setFlat(true);
    ui->ButtonWhite->setMinimumWidth(20);
    ui->ButtonWhite->update();

    /*-----------------------------------------------------*\
    | Fill in the mode selection box                        |
    \*-----------------------------------------------------*/
    ui->ModeBox->blockSignals(true);
    ui->ModeBox->clear();

    for (std::size_t i = 0; i < device->modes.size(); i++)
    {
        ui->ModeBox->addItem(device->modes[i].name.c_str());
        ui->ModeBox->setItemData(i, ModeDescription(device->modes[i]), Qt::ToolTipRole);
    }

    ui->ModeBox->setCurrentIndex(device->GetMode());
    ui->ModeBox->blockSignals(false);

    /*-----------------------------------------------------*\
    | Update mode user interface elements                   |
    \*-----------------------------------------------------*/
    UpdateModeUi();
    ui->RedSpinBox->setValue(ui->ColorWheelBox->color().red());
    ui->GreenSpinBox->setValue(ui->ColorWheelBox->color().green());
    ui->BlueSpinBox->setValue(ui->ColorWheelBox->color().blue());
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

RGBController* Ui::OpenRGBDevicePage::GetController()
{
    return device;
}

void Ui::OpenRGBDevicePage::on_ZoneBox_currentIndexChanged(int /*index*/)
{
    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();

    switch(device->modes[selected_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
            {
                int selected_zone = ui->ZoneBox->currentIndex();

                QString msLine;
                if(MultipleSelected)
                {
                    msLine = ui->LEDBox->itemText(ui->LEDBox->count() - 1);
                }
                ui->LEDBox->blockSignals(true);
                ui->LEDBox->clear();

                if(device->zones.size() > 1)
                {
                    if(selected_zone == 0)
                    {
                        if(device->leds.size() > 1)
                        {
                            ui->LEDBox->addItem("Entire Device");
                            ui->LEDBox->setEnabled(1);
                        }
                        else
                        {
                            ui->LEDBox->setDisabled(1);
                        }
                        for (std::size_t i = 0; i < device->leds.size(); i++)
                        {
                            ui->LEDBox->addItem(device->leds[i].name.c_str());
                        }
                        if(MultipleSelected) // Preserve the multiple() option
                        {
                            ui->LEDBox->addItem(msLine);
                            ui->LEDBox->setCurrentIndex(device->leds.size());
                        }

                        ui->ResizeButton->setEnabled(false);
                        if(!ui->ZoneBox->signalsBlocked())
                        {
                            ui->DeviceViewBox->blockSignals(true);
                            ui->DeviceViewBox->clearSelection();
                            ui->DeviceViewBox->blockSignals(false);
                        }
                    }
                    selected_zone = selected_zone - 1;
                }
                if(device->zones.size() == 1 || selected_zone != -1)
                {
                    // Disable led box if there's only one LED anyway
                    if(device->zones[selected_zone].leds_count > 1)
                    {
                        ui->LEDBox->addItem("Entire Zone");
                        ui->LEDBox->setEnabled(1);
                    }
                    else
                    {
                        ui->LEDBox->setDisabled(1);
                    }
                    for (std::size_t led_idx = 0; led_idx < device->zones[selected_zone].leds_count; led_idx++)
                    {
                        ui->LEDBox->addItem(device->zones[selected_zone].leds[led_idx].name.c_str());
                    }

                    if(device->zones[selected_zone].leds_min == device->zones[selected_zone].leds_max)
                    {
                        ui->ResizeButton->setEnabled(false);
                    }
                    else
                    {
                        ui->ResizeButton->setEnabled(true);
                    }
                    if(!ui->ZoneBox->signalsBlocked())
                    {
                        ui->DeviceViewBox->blockSignals(true);
                        ui->DeviceViewBox->selectZone(selected_zone);
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

void Ui::OpenRGBDevicePage::on_LEDBox_currentIndexChanged(int index)
{
    if(index < 0)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();

    switch(device->modes[selected_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
            {
                int selected_zone   = ui->ZoneBox->currentIndex();
                bool multiple       = (std::size_t(index) == (device->leds.size() + 1));

                RGBColor color      = 0x00000000;
                bool updateColor    = 0;

                if(device->zones.size() > 1)
                {
                    if(selected_zone == 0) // All zones
                    {
                        if(device->leds.size() > 1)
                        {
                            if(index == 0) // All LEDs on the entire device
                            {
                                if(!ui->LEDBox->signalsBlocked())
                                {
                                    ui->DeviceViewBox->blockSignals(true);
                                    ui->DeviceViewBox->clearSelection();
                                    ui->DeviceViewBox->blockSignals(false);
                                }
                            }
                            index = index - 1;
                        }
                        if((device->leds.size() == 1 || index != -1) && !multiple)
                        {
                            if(MultipleSelected)
                            {
                                ui->LEDBox->removeItem(device->leds.size() + 1);
                            }
                            MultipleSelected = 0;
                            color = device->GetLED(index); // One LED, proceed
                            updateColor = 1;
                            if(!ui->LEDBox->signalsBlocked())
                            {
                                ui->DeviceViewBox->blockSignals(true);
                                ui->DeviceViewBox->selectLed(index);
                                ui->DeviceViewBox->blockSignals(false);
                            }
                        }
                    }
                    selected_zone = selected_zone - 1;
                }
                if(device->zones.size() == 1 || selected_zone != -1) // A specific zone is selected
                {
                    if(device->zones[selected_zone].leds_count > 1)
                    {
                        if(index == 0) // Entire zone
                        {
                            if(!ui->LEDBox->signalsBlocked())
                            {
                                ui->DeviceViewBox->blockSignals(true);
                                ui->DeviceViewBox->selectZone(selected_zone);
                                ui->DeviceViewBox->blockSignals(false);
                            }
                        }
                        index = index - 1;
                    }
                    if(device->zones[selected_zone].leds_count == 1 || index != -1)
                    {
                        if((unsigned int)index < device->zones[selected_zone].leds_count)
                        {
                            color = device->zones[selected_zone].colors[index];
                            updateColor = 1;
                            int globalIndex = device->zones[selected_zone].leds - &(device->leds[0]) + index;
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
                    UpdatingColor = true;
                    ui->RedSpinBox->setValue(RGBGetRValue(color));
                    ui->GreenSpinBox->setValue(RGBGetGValue(color));
                    ui->BlueSpinBox->setValue(RGBGetBValue(color));
                    UpdatingColor = false;
                    updateHSV();
                    updateWheel();
                }
            }
            break;

        case MODE_COLORS_MODE_SPECIFIC:
            {
                /*-----------------------------------------------------*\
                | Update color picker with color of selected mode       |
                \*-----------------------------------------------------*/
                RGBColor color = device->modes[selected_mode].colors[index];
                UpdatingColor = true;
                ui->RedSpinBox->setValue(RGBGetRValue(color));
                ui->GreenSpinBox->setValue(RGBGetGValue(color));
                ui->BlueSpinBox->setValue(RGBGetBValue(color));
                UpdatingColor = false;
                updateHSV();
                updateWheel();
            }
            break;
    }

}

void Ui::OpenRGBDevicePage::on_ModeBox_currentIndexChanged(int /*index*/)
{
    /*-----------------------------------------------------*\
    | Update mode user interface elements                   |
    \*-----------------------------------------------------*/
    UpdateModeUi();

    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();
}

void Ui::OpenRGBDevicePage::on_PerLEDCheck_clicked()
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

void Ui::OpenRGBDevicePage::on_ModeSpecificCheck_clicked()
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

void Ui::OpenRGBDevicePage::on_RandomCheck_clicked()
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

void Ui::OpenRGBDevicePage::on_SpeedSlider_valueChanged(int /*value*/)
{
    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();
}

void Ui::OpenRGBDevicePage::on_DirectionBox_currentIndexChanged(int /*index*/)
{
    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    UpdateMode();
}

void Ui::OpenRGBDevicePage::UpdateInterface()
{
    //UpdateModeUi();
    ui->DeviceViewBox->repaint();
}

void Ui::OpenRGBDevicePage::UpdateModeUi()
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
        bool supports_dir_lr        = ( device->modes[selected_mode].flags & MODE_FLAG_HAS_DIRECTION_LR );
        bool supports_dir_ud        = ( device->modes[selected_mode].flags & MODE_FLAG_HAS_DIRECTION_UD );
        bool supports_dir_hv        = ( device->modes[selected_mode].flags & MODE_FLAG_HAS_DIRECTION_HV );
        bool per_led                = device->modes[selected_mode].color_mode == MODE_COLORS_PER_LED;
        bool mode_specific          = device->modes[selected_mode].color_mode == MODE_COLORS_MODE_SPECIFIC;
        bool random                 = device->modes[selected_mode].color_mode == MODE_COLORS_RANDOM;
        unsigned int dir            = device->modes[selected_mode].direction;

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

        ui->DirectionBox->blockSignals(true);
        ui->DirectionBox->clear();
        
        if(supports_dir_lr)
        {
            ui->DirectionBox->addItem("Left");
            ui->DirectionBox->addItem("Right");
        }

        if(supports_dir_ud)
        {
            ui->DirectionBox->addItem("Up");
            ui->DirectionBox->addItem("Down");
        }

        if(supports_dir_hv)
        {
            ui->DirectionBox->addItem("Horizontal");
            ui->DirectionBox->addItem("Vertical");
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
        }
        else
        {
            ui->PerLEDCheck->setEnabled(false);
            ui->PerLEDCheck->setAutoExclusive(false);
            ui->PerLEDCheck->setChecked(false);
            ui->PerLEDCheck->setAutoExclusive(true);
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

                ui->ResizeButton->setEnabled(false);
                ui->ApplyColorsButton->setEnabled(false);
                //ui->AutoFillCheck->setEnabled(false);
                break;

            case MODE_COLORS_PER_LED:
                ui->ZoneBox->blockSignals(true);
                ui->ZoneBox->clear();

                if(device->zones.size() > 1)
                {
                    ui->ZoneBox->setEnabled(1);
                    ui->ZoneBox->addItem("All Zones");
                }
                else
                {
                    ui->ZoneBox->setDisabled(1);
                }

                for (std::size_t i = 0; i < device->zones.size(); i++)
                {
                    ui->ZoneBox->addItem(device->zones[i].name.c_str());
                }

                ui->ZoneBox->setCurrentIndex(0);
                ui->ZoneBox->blockSignals(false);
                ui->ApplyColorsButton->setEnabled(true);
                //ui->AutoFillCheck->setEnabled(true);

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
                ui->ZoneBox->addItem("Mode Specific");
                ui->ZoneBox->blockSignals(false);

                ui->LEDBox->blockSignals(true);
                ui->LEDBox->clear();

                if(device->modes[selected_mode].colors_min == device->modes[selected_mode].colors_max)
                {
                    ui->ResizeButton->setEnabled(false);
                }
                else
                {
                    ui->ResizeButton->setEnabled(true);
                }

                for(unsigned int i = 0; i < device->modes[selected_mode].colors.size(); i++)
                {
                    char id_buf[32];
                    snprintf(id_buf, 16, "Mode Color %u", i);
                    ui->LEDBox->addItem(id_buf);
                }

                ui->LEDBox->setCurrentIndex(0);
                on_LEDBox_currentIndexChanged(0);
                ui->LEDBox->setEnabled(true);
                ui->LEDBox->blockSignals(false);
                ui->ApplyColorsButton->setEnabled(true);
                //ui->AutoFillCheck->setEnabled(true);
                break;
        }
    }
}

void Ui::OpenRGBDevicePage::UpdateMode()
{
    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    int current_mode = ui->ModeBox->currentIndex();

    if(current_mode >= 0)
    {
        int  current_speed     = 0;
        bool current_per_led   = ui->PerLEDCheck->isChecked();
        bool current_mode_specific = ui->ModeSpecificCheck->isChecked();
        bool current_random    = ui->RandomCheck->isChecked();
        int  current_dir_idx   = ui->DirectionBox->currentIndex();
        int  current_direction = 0;
        bool supports_dir_lr = ( device->modes[(unsigned int)current_mode].flags & MODE_FLAG_HAS_DIRECTION_LR );
        bool supports_dir_ud = ( device->modes[(unsigned int)current_mode].flags & MODE_FLAG_HAS_DIRECTION_UD );
        bool supports_dir_hv = ( device->modes[(unsigned int)current_mode].flags & MODE_FLAG_HAS_DIRECTION_HV );

        /*-----------------------------------------------------*\
        | Set the direction value                               |
        \*-----------------------------------------------------*/
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
        | Don't set the mode if the current mode is invalid     |
        \*-----------------------------------------------------*/
        if((unsigned int)current_mode < device->modes.size())
        {
            /*-----------------------------------------------------*\
            | Update mode parameters                                |
            \*-----------------------------------------------------*/
            device->modes[(unsigned int)current_mode].speed  = current_speed;

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

void Ui::OpenRGBDevicePage::SetDevice(unsigned char red, unsigned char green, unsigned char blue)
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(red);
    ui->GreenSpinBox->setValue(green);
    ui->BlueSpinBox->setValue(blue);
    UpdatingColor = false;
    updateHSV();
    updateWheel();
    updateDeviceView();
}

void Ui::OpenRGBDevicePage::UpdateDevice()
{
    ui->ModeBox->blockSignals(true);
    ui->ModeBox->setCurrentIndex(device->active_mode);
    ui->ModeBox->blockSignals(false);
    UpdateModeUi();
    UpdateMode();
}

void Ui::OpenRGBDevicePage::SetCustomMode(unsigned char red, unsigned char green, unsigned char blue)
{
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
    UpdatingColor = true;
    ui->RedSpinBox->setValue(red);
    ui->GreenSpinBox->setValue(green);
    ui->BlueSpinBox->setValue(blue);
    UpdatingColor = false;
    updateHSV();
    updateWheel();

    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();

    switch(device->modes[selected_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
            {
                /*-----------------------------------------------------*\
                | Set all device LEDs to the current color              |
                \*-----------------------------------------------------*/
                RGBColor color = ToRGBColor(
                    ui->RedSpinBox->text().toInt(),
                    ui->GreenSpinBox->text().toInt(),
                    ui->BlueSpinBox->text().toInt()
                );

                device->SetAllLEDs(color);
            }
            break;

        case MODE_COLORS_MODE_SPECIFIC:
            {
                /*-----------------------------------------------------*\
                | Set all device LEDs to the current color              |
                \*-----------------------------------------------------*/
                RGBColor color = ToRGBColor(
                    ui->RedSpinBox->text().toInt(),
                    ui->GreenSpinBox->text().toInt(),
                    ui->BlueSpinBox->text().toInt()
                );

                for(std::size_t i = 0; i < device->modes[selected_mode].colors.size(); i++)
                {
                    device->modes[selected_mode].colors[i] = color;
                }
            }
            break;
    }

    /*-----------------------------------------------------*\
    | Apply mode and colors                                 |
    \*-----------------------------------------------------*/
    UpdateMode();
}

void Ui::OpenRGBDevicePage::on_ButtonBlack_clicked()
{
    SetDevice(0, 0, 0);
}

void Ui::OpenRGBDevicePage::on_ButtonRed_clicked()
{
    SetDevice(255, 0, 0);
}

void Ui::OpenRGBDevicePage::on_ButtonYellow_clicked()
{
    SetDevice(255, 255, 0);
}

void Ui::OpenRGBDevicePage::on_ButtonGreen_clicked()
{
    SetDevice(0, 255, 0);
}

void Ui::OpenRGBDevicePage::on_ButtonCyan_clicked()
{
    SetDevice(0, 255, 255);
}

void Ui::OpenRGBDevicePage::on_ButtonBlue_clicked()
{
    SetDevice(0, 0, 255);
}

void Ui::OpenRGBDevicePage::on_ButtonMagenta_clicked()
{
    SetDevice(255, 0, 255);
}

void Ui::OpenRGBDevicePage::on_ButtonWhite_clicked()
{
    SetDevice(255, 255, 255);
}

void Ui::OpenRGBDevicePage::on_ColorWheelBox_colorChanged(const QColor color)
{
    if(UpdatingColor)
    {
        return;
    }

    UpdatingColor = true;
    ui->RedSpinBox->setValue(color.red());
    ui->GreenSpinBox->setValue(color.green());
    ui->BlueSpinBox->setValue(color.blue());
    UpdatingColor = false;

    updateHSV();
    updateDeviceView();
}

void Ui::OpenRGBDevicePage::updateRGB()
{
    if(UpdatingColor)
    {
        return;
    }

    UpdatingColor = true;

    hsv_t hsv;
    hsv.hue = ui->HueSpinBox->value();
    hsv.saturation = ui->SatSpinBox->value();
    hsv.value = ui->ValSpinBox->value();

    RGBColor rgb = hsv2rgb(&hsv);

    ui->RedSpinBox->setValue(RGBGetRValue(rgb));
    ui->GreenSpinBox->setValue(RGBGetGValue(rgb));
    ui->BlueSpinBox->setValue(RGBGetBValue(rgb));

    UpdatingColor = false;
}

void Ui::OpenRGBDevicePage::updateHSV()
{
    if(UpdatingColor)
    {
        return;
    }

    UpdatingColor = true;

    RGBColor rgb = ToRGBColor(ui->RedSpinBox->value(), ui->GreenSpinBox->value(), ui->BlueSpinBox->value());

    hsv_t hsv;

    rgb2hsv(rgb, &hsv);

    ui->HueSpinBox->setValue(hsv.hue);
    ui->SatSpinBox->setValue(hsv.saturation);
    ui->ValSpinBox->setValue(hsv.value);

    UpdatingColor = false;
}

void Ui::OpenRGBDevicePage::updateWheel()
{
    if(UpdatingColor)
    {
        return;
    }

    UpdatingColor = true;

    RGBColor qrgb = ToRGBColor
                        (
                        ui->BlueSpinBox->value(),
                        ui->GreenSpinBox->value(),
                        ui->RedSpinBox->value());

    ui->ColorWheelBox->setColor(QColor::fromRgb(qrgb));

    UpdatingColor = false;
}

void Ui::OpenRGBDevicePage::updateDeviceView()
{
    if(false)//ui->AutoFillCheck->isChecked())
    {
        /*-----------------------------------------------------*\
        | Read selected mode                                    |
        \*-----------------------------------------------------*/
        unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();

        switch(device->modes[selected_mode].color_mode)
        {
            case MODE_COLORS_PER_LED:
                {
                    RGBColor qrgb = ToRGBColor
                                        (
                                        ui->RedSpinBox->value(),
                                        ui->GreenSpinBox->value(),
                                        ui->BlueSpinBox->value());
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
                        ui->RedSpinBox->text().toInt(),
                        ui->GreenSpinBox->text().toInt(),
                        ui->BlueSpinBox->text().toInt()
                    );

                    device->modes[selected_mode].colors[index] = color;

                    device->UpdateMode();
                }
                break;
        }
    }
}

void Ui::OpenRGBDevicePage::on_RedSpinBox_valueChanged(int /*arg1*/)
{
    updateHSV();
    updateWheel();
    updateDeviceView();
}

void Ui::OpenRGBDevicePage::on_HueSpinBox_valueChanged(int /*arg1*/)
{
    updateRGB();
    updateWheel();
    updateDeviceView();
}

void Ui::OpenRGBDevicePage::on_GreenSpinBox_valueChanged(int /*arg1*/)
{
    updateHSV();
    updateWheel();
    updateDeviceView();
}

void Ui::OpenRGBDevicePage::on_SatSpinBox_valueChanged(int /*arg1*/)
{
    updateRGB();
    updateWheel();
    updateDeviceView();
}

void Ui::OpenRGBDevicePage::on_BlueSpinBox_valueChanged(int /*arg1*/)
{
    updateHSV();
    updateWheel();
    updateDeviceView();
}

void Ui::OpenRGBDevicePage::on_ValSpinBox_valueChanged(int /*arg1*/)
{
    updateRGB();
    updateWheel();
    updateDeviceView();
}

void Ui::OpenRGBDevicePage::on_DeviceViewBox_selectionChanged(QVector<int> indices)
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
                ui->LEDBox->removeItem(device->leds.size() + 1);
            }
            ui->LEDBox->addItem("Multiple (" + QVariant(indices.size()).toString() + ")");
            ui->LEDBox->setCurrentIndex(device->leds.size() + 1);
            MultipleSelected = 1;
        }
    }
    else
    {
        ui->LEDBox->setCurrentIndex(0);
    }
    ui->LEDBox->blockSignals(false);
}

void Ui::OpenRGBDevicePage::on_SetAllButton_clicked()
{
    unsigned char red   = ui->RedSpinBox->value();
    unsigned char green = ui->GreenSpinBox->value();
    unsigned char blue  = ui->BlueSpinBox->value();

    emit SetAllDevices(red, green, blue);
}

void Ui::OpenRGBDevicePage::on_ResizeButton_clicked()
{
    switch(device->modes[device->active_mode].color_mode)
    {
    case MODE_COLORS_PER_LED:
        {
            int selected_zone = ui->ZoneBox->currentIndex();

            if (device->zones.size() > 1)
            {
                selected_zone -= 1;
            }

            if(device->zones[selected_zone].type == ZONE_TYPE_LINEAR)
            {
                OpenRGBZoneResizeDialog dlg(device->zones[selected_zone].leds_min,
                                            device->zones[selected_zone].leds_max,
                                            device->zones[selected_zone].leds_count);

                int new_size = dlg.show();

                if(new_size >= 0)
                {
                    device->ResizeZone(selected_zone, new_size);

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
                                        device->modes[device->active_mode].colors.size());

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

void Ui::OpenRGBDevicePage::ShowDeviceView()
{
    ui->DeviceViewBox->show();
}

void Ui::OpenRGBDevicePage::HideDeviceView()
{
    ui->DeviceViewBox->hide();
}

void Ui::OpenRGBDevicePage::on_ApplyColorsButton_clicked()
{
    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();

    switch(device->modes[selected_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
            {
                RGBColor qrgb = ToRGBColor
                                    (
                                    ui->RedSpinBox->value(),
                                    ui->GreenSpinBox->value(),
                                    ui->BlueSpinBox->value());
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
                    ui->RedSpinBox->text().toInt(),
                    ui->GreenSpinBox->text().toInt(),
                    ui->BlueSpinBox->text().toInt()
                );

                device->modes[selected_mode].colors[index] = color;

                device->UpdateMode();
            }
            break;
    }
}

void Ui::OpenRGBDevicePage::on_SelectAllLEDsButton_clicked()
{
    if(device->modes[device->active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        ui->LEDBox->setCurrentIndex(0);
        on_LEDBox_currentIndexChanged(0);
        ui->DeviceViewBox->repaint();
    }
}
