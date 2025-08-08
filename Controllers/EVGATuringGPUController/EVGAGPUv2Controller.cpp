/*---------------------------------------------------------*\
| EVGAGPUv2Controller.cpp                                   |
|                                                           |
|   Driver for EVGA V2 (Turing) GPU                         |
|                                                           |
|   TheRogueZeta                                15 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "EVGAGPUv2Controller.h"

EVGAGPUv2Controller::EVGAGPUv2Controller(i2c_smbus_interface* bus, evga_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

EVGAGPUv2Controller::~EVGAGPUv2Controller()
{

}

std::string EVGAGPUv2Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char        addr[5];

    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string EVGAGPUv2Controller::GetDeviceName()
{
    return(name);
}

unsigned char EVGAGPUv2Controller::GetBrightnessA()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_BRIGHTNESS));
}

RGBColor EVGAGPUv2Controller::GetColorA()
{
    int red     = bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_RED);
    int green   = bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_GREEN);
    int blue    = bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_BLUE);
    return(ToRGBColor(red, green, blue));
}

RGBColor EVGAGPUv2Controller::GetColorB()
{
    int red     = bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_COLOR_B_RED);
    int green   = bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_COLOR_B_GREEN);
    int blue    = bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_COLOR_B_BLUE);
    return(ToRGBColor(red, green, blue));
}

unsigned char EVGAGPUv2Controller::GetMode()
{
    unsigned char return_mode = 0;
    unsigned char mode = bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_MODE);

    if(mode == 0xFF)
    {
        //Registers may not ready after saving config. Read again if 0xFF.
        mode = bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_MODE);
    }

    switch (mode)
    {
        case EVGA_GPU_V2_MODE_OFF:
            {
                return_mode = EVGA_GPU_V2_RGB_MODE_OFF;
            }
            break;

        case EVGA_GPU_V2_MODE_STATIC:
            {
                return_mode = EVGA_GPU_V2_RGB_MODE_STATIC;
            }
            break;

        case EVGA_GPU_V2_MODE_RAINBOW:
            {
                return_mode = EVGA_GPU_V2_RGB_MODE_RAINBOW;
            }
            break;

        case EVGA_GPU_V2_MODE_BREATHING:
            {
                u16_to_u8 speed_16 = { (uint16_t) 0 };

                speed_16.lsb = bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_B_TO_A_SPEED_LSB);
                speed_16.msb = bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_B_TO_A_SPEED_MSB);

                if (speed_16.u16 == 0)
                {
                    return_mode = EVGA_GPU_V2_RGB_MODE_PULSE;
                }
                else
                {
                    return_mode = EVGA_GPU_V2_RGB_MODE_BREATHING;
                }
            }
            break;

        default:
            break;
    }

    return(return_mode);
}

unsigned char EVGAGPUv2Controller::GetSpeed()
{
    u16_to_u8 speed_16 = { (uint16_t) 0 };

    speed_16.lsb = bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_ONTIME_LSB);
    speed_16.msb = bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_ONTIME_MSB);

    speed_16.u16 /= SPEED_MULTIPLIER;

    return (unsigned char) speed_16.u16;
}

void EVGAGPUv2Controller::SetMode(uint8_t mode, RGBColor color1, RGBColor color2, uint16_t speed, uint8_t brightness)
{

    EnableWrite(true);
    switch (mode)
    {
    case EVGA_GPU_V2_RGB_MODE_OFF:
        {
            SendMode(EVGA_GPU_V2_MODE_OFF);
        }
        break;

    case EVGA_GPU_V2_RGB_MODE_STATIC:
        {
            SendMode(EVGA_GPU_V2_MODE_STATIC);
            SendColor(EVGA_GPU_V2_REG_COLOR_A_RED, RGBGetRValue(color1), RGBGetGValue(color1), RGBGetBValue(color1), brightness);
        }
        break;

    case EVGA_GPU_V2_RGB_MODE_RAINBOW:
        {
            SendMode(EVGA_GPU_V2_MODE_RAINBOW);
            SendBrightness(brightness); //Default = 0x64
            // Set Rainbow speed? No control in the GUI but this register is only set in Ranbow mode.
            bus->i2c_smbus_write_byte_data(dev, 0x19, 0x11);;
        }
        break;

    case EVGA_GPU_V2_RGB_MODE_BREATHING:
    case EVGA_GPU_V2_RGB_MODE_PULSE:
        {
            SendMode(EVGA_GPU_V2_MODE_BREATHING);

            /*---------------------------------------------------------*\
            | It is expected that color2 will be 0x000000 (black) for   |
            | 1 color mode otherwise set correctly therfore no further  |
            | inspection is required.                                   |
            \*---------------------------------------------------------*/

            SendColor(EVGA_GPU_V2_REG_COLOR_A_RED, RGBGetRValue(color1), RGBGetGValue(color1), RGBGetBValue(color1), brightness);
            SendColor(EVGA_GPU_V2_REG_COLOR_B_RED, RGBGetRValue(color2), RGBGetGValue(color2), RGBGetBValue(color2), brightness);

            /*-----------------------------------------------------------------*\
            | Breathing mode speeds are consistent for B_TO_A and A_TO_B        |
            | Pulse (Blink) mode is on/off ergo B_TO_A and A_TO_B = 0 (instant) |
            \*-----------------------------------------------------------------*/
            u16_to_u8 speed_16 = { (uint16_t) (speed * SPEED_MULTIPLIER) };
            u16_to_u8 rise_fall_un_16  = { (mode == EVGA_GPU_V2_RGB_MODE_PULSE) ? (uint16_t) 0 : speed_16.u16 };
            SendSpeed(speed_16, speed_16, rise_fall_un_16, rise_fall_un_16, rise_fall_un_16);

            // 0x61 = 0x01
            bus->i2c_smbus_write_byte_data(dev, 0x61,   0x01);
            // 0x6A and 0x6B = 0x00
            bus->i2c_smbus_write_byte_data(dev, 0x6A,   0x00);
            bus->i2c_smbus_write_byte_data(dev, 0x6B,   0x00);
        }
        break;

    default:
        break;
    }

    //Disable writes
    EnableWrite(false);
}

void EVGAGPUv2Controller::EnableWrite(bool boolEnable)
{
    if(boolEnable)
    {
        bus->i2c_smbus_write_byte_data(dev, 0x0E,   0xE5);
        bus->i2c_smbus_write_byte_data(dev, 0x0E,   0xE9);
        bus->i2c_smbus_write_byte_data(dev, 0x0E,   0xF5);
        bus->i2c_smbus_write_byte_data(dev, 0x0E,   0xF9);
    }
    else
    {
        bus->i2c_smbus_write_byte_data(dev, 0x08,   0x01);
        //Dissable commands
        bus->i2c_smbus_write_byte_data(dev, 0x0E,   0xF0);
        bus->i2c_smbus_write_byte_data(dev, 0x0E,   0xE0);
    }
}

void EVGAGPUv2Controller::SaveSettings()
{
    bus->i2c_smbus_write_byte_data(dev, 0x0E,   0xE5);
    bus->i2c_smbus_write_byte_data(dev, 0x0E,   0xE9);
    bus->i2c_smbus_write_byte_data(dev, 0x0E,   0xF0);
    bus->i2c_smbus_write_byte_data(dev, 0x1F,   0xE5);
    bus->i2c_smbus_write_byte_data(dev, 0x23,   0xE5);
    bus->i2c_smbus_write_byte_data(dev, 0x0E,   0xE0);
    bus->i2c_smbus_write_byte_data(dev, 0x0E,   0xE0);
}

void EVGAGPUv2Controller::SendBrightness(uint8_t brightness)
{
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_BRIGHTNESS, brightness);
}

void EVGAGPUv2Controller::SendColor(uint8_t start_register, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness)
{
    bus->i2c_smbus_write_byte_data(dev, start_register,         red);
    bus->i2c_smbus_write_byte_data(dev, (start_register + 1),   green);
    bus->i2c_smbus_write_byte_data(dev, (start_register + 2),   blue);
    bus->i2c_smbus_write_byte_data(dev, (start_register + 3),   brightness);
}

void EVGAGPUv2Controller::SendMode(uint8_t mode)
{
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_MODE, mode);
}

void EVGAGPUv2Controller::SendSpeed(u16_to_u8 aOnTime, u16_to_u8 bOnTime, u16_to_u8 b2a, u16_to_u8 a2b, u16_to_u8 speed_un)
{
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_COLOR_B_UN_LSB,     (unsigned char) speed_un.lsb );
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_COLOR_B_UN_MSB,     (unsigned char) speed_un.msb );
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_B_TO_A_SPEED_LSB,   (unsigned char) b2a.lsb );
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_B_TO_A_SPEED_MSB,   (unsigned char) b2a.msb );
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_COLOR_B_ONTIME_LSB, (unsigned char) bOnTime.lsb );
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_COLOR_B_ONTIME_MSB, (unsigned char) bOnTime.msb );
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_A_TO_B_SPEED_LSB,   (unsigned char) a2b.lsb );
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_A_TO_B_SPEED_MSB,   (unsigned char) a2b.msb );
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_ONTIME_LSB, (unsigned char) aOnTime.lsb );
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_ONTIME_MSB, (unsigned char) aOnTime.msb );
}

void EVGAGPUv2Controller::SetColor(RGBColor colorA, RGBColor colorB, uint8_t brightness)
{
    EnableWrite(true);
    SendColor(EVGA_GPU_V2_REG_COLOR_A_RED, RGBGetRValue(colorA), RGBGetGValue(colorA), RGBGetBValue(colorA), brightness);
    SendColor(EVGA_GPU_V2_REG_COLOR_B_RED, RGBGetRValue(colorB), RGBGetGValue(colorB), RGBGetBValue(colorB), brightness);
    EnableWrite(false);
}
