#include "OpenRGBDevicePage.h"
#include "OpenRGBZoneResizeDialog.h"
#include "hsv.h"

using namespace Ui;

static void UpdateCallback(void * this_ptr)
{
    OpenRGBDevicePage * this_obj = (OpenRGBDevicePage *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "UpdateInterface", Qt::QueuedConnection);
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
    | Set up the color palette buttons                      |
    \*-----------------------------------------------------*/
    QPalette pal;

    ui->widget->setController(device);

    device->RegisterUpdateCallback(UpdateCallback, this);

    pal = ui->ButtonRed->palette();
    pal.setColor(QPalette::Button, QColor(255, 0, 0));
    ui->ButtonRed->setAutoFillBackground(true);
    ui->ButtonRed->setPalette(pal);
    ui->ButtonRed->setFlat(true);
    ui->ButtonRed->update();

    pal = ui->ButtonYellow->palette();
    pal.setColor(QPalette::Button, QColor(255, 255, 0));
    ui->ButtonYellow->setAutoFillBackground(true);
    ui->ButtonYellow->setPalette(pal);
    ui->ButtonYellow->setFlat(true);
    ui->ButtonYellow->update();

    pal = ui->ButtonGreen->palette();
    pal.setColor(QPalette::Button, QColor(0, 255, 0));
    ui->ButtonGreen->setAutoFillBackground(true);
    ui->ButtonGreen->setPalette(pal);
    ui->ButtonGreen->setFlat(true);
    ui->ButtonGreen->update();

    pal = ui->ButtonCyan->palette();
    pal.setColor(QPalette::Button, QColor(0, 255, 255));
    ui->ButtonCyan->setAutoFillBackground(true);
    ui->ButtonCyan->setPalette(pal);
    ui->ButtonCyan->setFlat(true);
    ui->ButtonCyan->update();

    pal = ui->ButtonBlue->palette();
    pal.setColor(QPalette::Button, QColor(0, 0, 255));
    ui->ButtonBlue->setAutoFillBackground(true);
    ui->ButtonBlue->setPalette(pal);
    ui->ButtonBlue->setFlat(true);
    ui->ButtonBlue->update();

    pal = ui->ButtonMagenta->palette();
    pal.setColor(QPalette::Button, QColor(255, 0, 255));
    ui->ButtonMagenta->setAutoFillBackground(true);
    ui->ButtonMagenta->setPalette(pal);
    ui->ButtonMagenta->setFlat(true);
    ui->ButtonMagenta->update();

    /*-----------------------------------------------------*\
    | Fill in the mode selection box                        |
    \*-----------------------------------------------------*/
    ui->ModeBox->blockSignals(true);
    ui->ModeBox->clear();

    for (std::size_t i = 0; i < device->modes.size(); i++)
    {
        ui->ModeBox->addItem(device->modes[i].name.c_str());
    }

    ui->ModeBox->setCurrentIndex(device->GetMode());
    ui->ModeBox->blockSignals(false);

    /*-----------------------------------------------------*\
    | Update mode user interface elements                   |
    \*-----------------------------------------------------*/
    UpdateModeUi();
}

OpenRGBDevicePage::~OpenRGBDevicePage()
{
    delete ui;
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
                unsigned int selected_zone = ui->ZoneBox->currentIndex();

                ui->LEDBox->blockSignals(true);
                ui->LEDBox->clear();

                if(selected_zone == 0)
                {
                    for (std::size_t i = 0; i < device->leds.size(); i++)
                    {
                        ui->LEDBox->addItem(device->leds[i].name.c_str());
                    }

                    ui->ResizeButton->setEnabled(false);
                }
                else
                {
                    selected_zone = selected_zone - 1;

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
    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();

    switch(device->modes[selected_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
            {
                unsigned int selected_zone = ui->ZoneBox->currentIndex();

                RGBColor color = 0x00000000;

                if(selected_zone == 0)
                {
                    color = device->GetLED(index);
                }
                else
                {
                    selected_zone = selected_zone - 1;

                    if((unsigned int)index < device->zones[selected_zone].leds_count)
                    {
                        color = device->zones[selected_zone].colors[index];
                    }
                }

                /*-----------------------------------------------------*\
                | Update color picker with color of selected LED        |
                \*-----------------------------------------------------*/
                UpdatingColor = true;
                ui->RedSpinBox->setValue(RGBGetRValue(color));
                ui->GreenSpinBox->setValue(RGBGetGValue(color));
                ui->BlueSpinBox->setValue(RGBGetBValue(color));
                UpdatingColor = false;
                updateHSV();
                updateWheel();
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
    UpdateModeUi();
    ui->widget->repaint();
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

            if(device->modes[selected_mode].speed_min > device->modes[selected_mode].speed_max)
            {
                InvertedSpeed = true;
                ui->SpeedSlider->setMinimum(device->modes[selected_mode].speed_max);
                ui->SpeedSlider->setMaximum(device->modes[selected_mode].speed_min);
            }
            else
            {
                InvertedSpeed = false;
                ui->SpeedSlider->setMinimum(device->modes[selected_mode].speed_min);
                ui->SpeedSlider->setMaximum(device->modes[selected_mode].speed_max);
            }
            
            ui->SpeedSlider->setValue(device->modes[selected_mode].speed);
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
                break;

            case MODE_COLORS_PER_LED:
                ui->ZoneBox->blockSignals(true);
                ui->ZoneBox->clear();

                ui->ZoneBox->addItem("All Zones");

                for (std::size_t i = 0; i < device->zones.size(); i++)
                {
                    ui->ZoneBox->addItem(device->zones[i].name.c_str());
                }

                ui->ZoneBox->setCurrentIndex(0);
                ui->ZoneBox->blockSignals(false);

                /*-----------------------------------------------------*\
                | Update LED box                                        |
                \*-----------------------------------------------------*/
                on_ZoneBox_currentIndexChanged(0);

                /*-----------------------------------------------------*\
                | Update color picker with color of first LED           |
                \*-----------------------------------------------------*/
                on_LEDBox_currentIndexChanged(0);
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

                for (std::size_t i = 0; i < device->modes[selected_mode].colors.size(); i++)
                {
                    char id_buf[32];
                    snprintf(id_buf, 16, "Mode Color %lu", i);
                    ui->LEDBox->addItem(id_buf);
                }

                ui->LEDBox->setCurrentIndex(0);
                on_LEDBox_currentIndexChanged(0);
                ui->LEDBox->blockSignals(false);
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
            current_speed = ui->SpeedSlider->value();

            /*-----------------------------------------------------*\
            | If Speed Slider is inverted, invert value             |
            \*-----------------------------------------------------*/
            if(InvertedSpeed)
            {
                current_speed = device->modes[(unsigned int)current_mode].speed_min - current_speed + device->modes[current_mode].speed_max;
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
    
    on_SetDeviceButton_clicked();
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

void Ui::OpenRGBDevicePage::on_SetDeviceButton_clicked()
{
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

                device->UpdateLEDs();
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

                device->UpdateMode();
            }
            break;
    }
}

void Ui::OpenRGBDevicePage::on_SetZoneButton_clicked()
{
    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();

    switch(device->modes[selected_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
            {
                unsigned int index = ui->ZoneBox->currentIndex();

                if(index == 0)
                {
                    on_SetDeviceButton_clicked();
                }
                else
                {
                    index = index - 1;

                    /*-----------------------------------------------------*\
                    | Set all LEDs in the selected zone to the current color|
                    \*-----------------------------------------------------*/
                    RGBColor color = ToRGBColor(
                        ui->RedSpinBox->text().toInt(),
                        ui->GreenSpinBox->text().toInt(),
                        ui->BlueSpinBox->text().toInt()
                    );

                    device->SetAllZoneLEDs(index, color);
                    device->UpdateZoneLEDs(index);
                }
            }
            break;
    }
}

void Ui::OpenRGBDevicePage::on_SetLEDButton_clicked()
{
    /*-----------------------------------------------------*\
    | Read selected mode                                    |
    \*-----------------------------------------------------*/
    unsigned int selected_mode   = (unsigned int)ui->ModeBox->currentIndex();
    unsigned int index = ui->LEDBox->currentIndex();

    switch(device->modes[selected_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
            {
                unsigned int selected_zone = ui->ZoneBox->currentIndex();

                /*-----------------------------------------------------*\
                | Set the selected LED to the current color             |
                \*-----------------------------------------------------*/
                RGBColor color = ToRGBColor(
                    ui->RedSpinBox->text().toInt(),
                    ui->GreenSpinBox->text().toInt(),
                    ui->BlueSpinBox->text().toInt()
                );

                if(selected_zone == 0)
                {
                    device->SetLED(index, color);
                    device->UpdateSingleLED(index);
                }
                else
                {
                    selected_zone = selected_zone - 1;

                    device->SetLED(device->zones[selected_zone].start_idx + index, color);
                    device->UpdateSingleLED(device->zones[selected_zone].start_idx + index);
                }
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

                device->modes[selected_mode].colors[index] = color;

                device->UpdateMode();
            }
            break;
    }
}

void Ui::OpenRGBDevicePage::on_ButtonRed_clicked()
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(255);
    ui->GreenSpinBox->setValue(0);
    ui->BlueSpinBox->setValue(0);
    UpdatingColor = false;
    updateHSV();
    updateWheel();
}

void Ui::OpenRGBDevicePage::on_ButtonYellow_clicked()
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(255);
    ui->GreenSpinBox->setValue(255);
    ui->BlueSpinBox->setValue(0);
    UpdatingColor = false;
    updateHSV();
    updateWheel();
}

void Ui::OpenRGBDevicePage::on_ButtonGreen_clicked()
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(0);
    ui->GreenSpinBox->setValue(255);
    ui->BlueSpinBox->setValue(0);
    UpdatingColor = false;
    updateHSV();
    updateWheel();
}

void Ui::OpenRGBDevicePage::on_ButtonCyan_clicked()
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(0);
    ui->GreenSpinBox->setValue(255);
    ui->BlueSpinBox->setValue(255);
    UpdatingColor = false;
    updateHSV();
    updateWheel();
}

void Ui::OpenRGBDevicePage::on_ButtonBlue_clicked()
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(0);
    ui->GreenSpinBox->setValue(0);
    ui->BlueSpinBox->setValue(255);
    UpdatingColor = false;
    updateHSV();
    updateWheel();
}

void Ui::OpenRGBDevicePage::on_ButtonMagenta_clicked()
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(255);
    ui->GreenSpinBox->setValue(0);
    ui->BlueSpinBox->setValue(255);
    UpdatingColor = false;
    updateHSV();
    updateWheel();
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

void Ui::OpenRGBDevicePage::on_RedSpinBox_valueChanged(int /*arg1*/)
{
    updateHSV();
    updateWheel();
}

void Ui::OpenRGBDevicePage::on_HueSpinBox_valueChanged(int /*arg1*/)
{
    updateRGB();
    updateWheel();
}

void Ui::OpenRGBDevicePage::on_GreenSpinBox_valueChanged(int /*arg1*/)
{
    updateHSV();
    updateWheel();
}

void Ui::OpenRGBDevicePage::on_SatSpinBox_valueChanged(int /*arg1*/)
{
    updateRGB();
    updateWheel();
}

void Ui::OpenRGBDevicePage::on_BlueSpinBox_valueChanged(int /*arg1*/)
{
    updateHSV();
    updateWheel();
}

void Ui::OpenRGBDevicePage::on_ValSpinBox_valueChanged(int /*arg1*/)
{
    updateRGB();
    updateWheel();
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

            selected_zone -= 1;

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
