/*---------------------------------------------------------*\
| PatriotViperController.cpp                                |
|                                                           |
|   Driver for Patriot Viper RAM                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                01 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "PatriotViperController.h"

PatriotViperController::PatriotViperController(i2c_smbus_interface* bus, viper_dev_id dev, unsigned char slots)
{
    this->bus   = bus;
    this->dev   = dev;
    slots_valid = slots;

    strcpy(device_name, "Patriot Viper RGB");

    led_count = 0;

    for(int i = 0; i < 8; i++)
    {
        if((slots_valid & (1 << i)) != 0)
        {
            led_count += 5;
        }
    }

    keepalive_thread = NULL;
    keepalive_thread_run = 1;
}

PatriotViperController::~PatriotViperController()
{
    StopKeepaliveThread();
}

std::string PatriotViperController::GetDeviceName()
{
    return(device_name);
}

std::string PatriotViperController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned int PatriotViperController::GetLEDCount()
{
    return(led_count);
}

unsigned int PatriotViperController::GetSlotCount()
{
    unsigned int slot_count = 0;

    for(int slot = 0; slot < 4; slot++)
    {
        if((slots_valid & (1 << slot)) != 0)
        {
            slot_count++;
        }
    }

    return(slot_count);
}

unsigned int PatriotViperController::GetMode()
{
    return(mode);
}

void PatriotViperController::SetEffectColor(unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);

    ViperRegisterWrite(VIPER_REG_LED0_EFFECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED1_EFFECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED2_EFFECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED3_EFFECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED4_EFFECT_COLOR, red, blue, green);

    ViperRegisterWrite(VIPER_REG_MODE, mode, 0x00, speed);
    ViperRegisterWrite(VIPER_REG_MODE, 0xAA, 0x00, 0x00);
    ViperRegisterWrite(VIPER_REG_MODE, 0xFA, 0x00, 0x00);
}

void PatriotViperController::SetAllColors(unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_REG_LED0_EFFECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED1_EFFECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED2_EFFECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED3_EFFECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED4_EFFECT_COLOR, red, blue, green);
}

void PatriotViperController::SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_REG_LED0_EFFECT_COLOR + led, red, blue, green);
}

void PatriotViperController::SetLEDEffectColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);

    ViperRegisterWrite(VIPER_REG_LED0_EFFECT_COLOR + led, red, blue, green);

    ViperRegisterWrite(VIPER_REG_MODE, mode, 0x00, speed);
    ViperRegisterWrite(VIPER_REG_MODE, 0xAA, 0x00, 0x00);
    ViperRegisterWrite(VIPER_REG_MODE, 0xFA, 0x00, 0x00);
}

void PatriotViperController::SetLEDColor(unsigned int /*slot*/, unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);

    ViperRegisterWrite(VIPER_REG_LED0_DIRECT_COLOR + led, red, blue, green);

    ViperRegisterWrite(VIPER_REG_APPLY, 0x01, 0x00, 0x00);
}

void PatriotViperController::SetLEDEffectColor(unsigned int /*slot*/, unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);

    ViperRegisterWrite(VIPER_REG_LED0_EFFECT_COLOR + led, red, blue, green);

    ViperRegisterWrite(VIPER_REG_MODE, mode, 0x00, speed);
    ViperRegisterWrite(VIPER_REG_MODE, 0xAA, 0x00, 0x00);
    ViperRegisterWrite(VIPER_REG_MODE, 0xFA, 0x00, 0x00);
}

void PatriotViperController::SetMode(unsigned char new_mode, unsigned char new_speed, unsigned int color_mode)
{
    StopKeepaliveThread();
    direct = false;
    mode   = new_mode;
    if(mode_speed[mode] == -1)
    {
        speed = new_speed;
    }
    else
    {
        speed = mode_speed[mode];
    }

    if(color_mode == 3)
    {
        /*--------------------------------------------------------------------------------------------------*\
        | Reset previously set mode color, because we want RAM sticks to fall-back to automatic viper colors |
        | These are not just rainbowey, but they are affected differently with modes.                        |
        \*--------------------------------------------------------------------------------------------------*/

        ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
        ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);
        ViperRegisterWrite(VIPER_REG_MODE, VIPER_MODE_DARK, 0x00, 0x00);
        ViperRegisterWrite(VIPER_REG_MODE, 0xAA, 0x00, 0x00);
        ViperRegisterWrite(VIPER_REG_MODE, 0xFA, 0x00, 0x00);
    }

    if(mode_steps[mode] == -1)
    {
        /*--------------------------------------------------------------------------------------------------*\
        | Based on a header file, if number of steps in mode is -1 it means mode is not synced,              |
        | doesn't have steps and the Keepalive thread is not needed. Set the mode directly and leave it.     |
        \*--------------------------------------------------------------------------------------------------*/

        ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
        ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);
        ViperRegisterWrite(VIPER_REG_MODE, mode, 0x00, speed);
        ViperRegisterWrite(VIPER_REG_MODE, 0xAA, 0x00, 0x00);
        ViperRegisterWrite(VIPER_REG_MODE, 0xFA, 0x00, 0x00);
    }
    else
    {
        /*----------------------------------------------------------------*\
        | Reset step counters and fire up Keepalive thread.                |
        | Thread will deal with changing the mode and everything else.     |
        \*----------------------------------------------------------------*/

        step = 0;
        sub_step = 0;

        keepalive_thread = new std::thread(&PatriotViperController::KeepaliveThread, this);
    }
}

void PatriotViperController::SetDirect()
{
    StopKeepaliveThread();
    direct = true;
    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);
    ViperRegisterWrite(VIPER_REG_MODE, VIPER_MODE_DIRECT, 0x00, 0x00);
}

void PatriotViperController::ViperRegisterWrite(viper_register reg, unsigned char val0, unsigned char val1, unsigned char val2)
{
    bus->i2c_smbus_write_byte_data(dev, reg, val0);
    bus->i2c_smbus_write_byte_data(dev, val2, val1);
}

void PatriotViperController::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        int cur_step = step.load();
        int cur_sub_step = sub_step.load();
        if(cur_sub_step == 0)
        {
            ViperRegisterWrite(VIPER_REG_MODE, mode, cur_step, speed);
            ViperRegisterWrite(VIPER_REG_MODE, 0xAA, 0x00, cur_sub_step);
        }
        else
        {
            ViperRegisterWrite(VIPER_REG_MODE, 0xAA, 0x00, cur_sub_step);
        }

        if(cur_sub_step == mode_sub_steps[mode])
        {
            sub_step.store(0);

            if(cur_step == mode_steps[mode])
            {
                step.store(0);
            }
            else
            {
                step.store(cur_step+1);
            }
        }
        else
        {
            sub_step.store(cur_sub_step+1);
        }

        /*---------------------------------------------------------------------------------------------------------*\
        | We have to use wait_for with condition_variable since some of the modes will keep the thread waiting for  |
        | long time and we don't want to make user wait for mode change when in middle of waiting period.           |
        \*---------------------------------------------------------------------------------------------------------*/
        int delay = GetDelay(mode, cur_step, cur_sub_step, cur_sub_step == mode_sub_steps[mode]);
        std::unique_lock<std::mutex> l(thread_ctrl_m);
        thread_ctrl.wait_for(l, std::chrono::milliseconds(delay));
    }
}

void PatriotViperController::StopKeepaliveThread()
{
    if(keepalive_thread != NULL)
    {
        keepalive_thread_run = 0;
        thread_ctrl.notify_one();
        keepalive_thread->join();
        keepalive_thread = NULL;
        keepalive_thread_run = 1;
    }
}

unsigned int PatriotViperController::GetDelay(unsigned char mode, unsigned int /*step*/, unsigned int sub_step, bool loop_end)
{
    if(loop_end)
    {
        if(mode == VIPER_MODE_VIPER)
        {
            return 7000;
        }
        else if(mode == VIPER_MODE_BREATHING)
        {
            return 3000;
        }
        else if(mode == VIPER_MODE_NEON)
        {
            return 600;
        }
        else if(mode == VIPER_MODE_AURORA)
        {
            return 300;
        }
        else
        {
            return 0;
        }
    }

    if(sub_step == 0)
    {
        if(mode == VIPER_MODE_VIPER)
        {
            return 3000;
        }
        else if(mode == VIPER_MODE_BREATHING)
        {
            return 3000;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(sub_step == 4 && mode == VIPER_MODE_VIPER)
        {
            return 1000;
        }
        else if(sub_step == 5 && mode == VIPER_MODE_VIPER)
        {
            return 590;
        }
        else if(mode == VIPER_MODE_NEON)
        {
            return 600;
        }
        else if(mode == VIPER_MODE_HEARTBEAT)
        {
            return 100;
        }
        else if(mode == VIPER_MODE_MARQUEE)
        {
            return 300;
        }
        else
        {
            return 0;
        }
    }
}
