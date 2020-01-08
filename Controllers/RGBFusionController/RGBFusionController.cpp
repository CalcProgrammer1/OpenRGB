/*-----------------------------------------*\
|  RGBFusionController.cpp                  |
|                                           |
|  Driver for Gigabyte Aorus RGB Fusion     |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/10/2019  |
\*-----------------------------------------*/

#include "RGBFusionController.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

RGBFusionController::RGBFusionController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    // Set Device name
    strcpy(device_name, "Gigabyte Motherboard");

    // Set LED count
    led_count = 2;

    // Enable control
    switch_bank(0);
    bus->i2c_smbus_write_byte_data(dev, 0x02, 0x09);
}

RGBFusionController::~RGBFusionController()
{

}

std::string RGBFusionController::GetDeviceName()
{
    return(device_name);
}

std::string RGBFusionController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return(return_string);
}

unsigned int RGBFusionController::GetLEDCount()
{
    return(led_count);
}

unsigned char RGBFusionController::GetMode()
{
    switch_bank(0);
    return(get_mode_ch_0());
}

void RGBFusionController::SetAllColors(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char mode_ch_0;
    unsigned char mode_ch_1;

    //Lock SMBus interface
    bus->i2c_smbus_wait_and_lock();
    
    switch_bank(1);
    set_color_ch_0(red, green, blue);
    set_color_ch_1(red, green, blue);

    switch_bank(0);
    mode_ch_0 = get_mode_ch_0();
    mode_ch_1 = get_mode_ch_1();
    set_mode_ch_0(mode_ch_0);
    set_mode_ch_1(mode_ch_1);

    //Unlock SMBus interface
    bus->i2c_smbus_unlock();
}

void RGBFusionController::SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char mode;

    switch (led)
    {
    case 0:
        //Lock SMBus interface
        bus->i2c_smbus_wait_and_lock();
        
        switch_bank(1);
        set_color_ch_0(red, green, blue);

        switch_bank(0);
        mode = get_mode_ch_0();
        set_mode_ch_0(mode);

        //Unlock SMBus interface
        bus->i2c_smbus_unlock();
        break;

    case 1:
        //Lock SMBus interface
        bus->i2c_smbus_wait_and_lock();

        switch_bank(1);
        set_color_ch_1(red, green, blue);

        switch_bank(0);
        mode = get_mode_ch_1();
        set_mode_ch_1(mode);

        //Unlock SMBus interface
        bus->i2c_smbus_unlock();
        break;
    }
}

void RGBFusionController::SetMode(unsigned char mode)
{
    //Lock SMBus interface
    bus->i2c_smbus_wait_and_lock();

    switch_bank(0);
    set_mode_ch_0(mode);
    set_mode_ch_1(mode);

    //Unlock SMBus interface
    bus->i2c_smbus_unlock();
}

void RGBFusionController::dump()
{
    int i, j;

    int start = 0x00;

    FILE* file = freopen("rgb_fusion_dump.txt", "a", stdout);

    printf("       0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");

    for (i = 0; i < 0xFF; i += 16)
    {
        printf("%04x: ", i + start);

        for (j = 0; j < 16; j++)
        {
            printf("%02x ", bus->i2c_smbus_read_byte_data(dev, (start + i + j)));
        }

        printf("\r\n");
    }

    fclose(file);
}

unsigned char RGBFusionController::get_mode_ch_0()
{
    return(bus->i2c_smbus_read_byte_data(dev, RGB_FUSION_BANK_0_REG_CH_0_MODE));
}

unsigned char RGBFusionController::get_mode_ch_1()
{
    return(bus->i2c_smbus_read_byte_data(dev, RGB_FUSION_BANK_0_REG_CH_1_MODE));
}

void RGBFusionController::set_color_ch_0(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, RGB_FUSION_BANK_1_REG_CH_0_R, red);
    bus->i2c_smbus_write_byte_data(dev, RGB_FUSION_BANK_1_REG_CH_0_G, green);
    bus->i2c_smbus_write_byte_data(dev, RGB_FUSION_BANK_1_REG_CH_0_B, blue);
    bus->i2c_smbus_write_byte_data(dev, 0x03, 0x01);
}

void RGBFusionController::set_color_ch_1(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, RGB_FUSION_BANK_1_REG_CH_1_R, red);
    bus->i2c_smbus_write_byte_data(dev, RGB_FUSION_BANK_1_REG_CH_1_G, green);
    bus->i2c_smbus_write_byte_data(dev, RGB_FUSION_BANK_1_REG_CH_1_B, blue);
    bus->i2c_smbus_write_byte_data(dev, 0x0B, 0x01);
}

void RGBFusionController::set_mode_ch_0(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(dev, RGB_FUSION_BANK_0_REG_CH_0_MODE, mode + RGB_FUSION_WRITE_MODE_OFST);
}

void RGBFusionController::set_mode_ch_1(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(dev, RGB_FUSION_BANK_0_REG_CH_1_MODE, mode + RGB_FUSION_WRITE_MODE_OFST);
}

void RGBFusionController::switch_bank(unsigned char bank)
{
    bus->i2c_smbus_write_byte_data(dev, RGB_FUSION_BANK_SWITCH_REG, bank);
}
