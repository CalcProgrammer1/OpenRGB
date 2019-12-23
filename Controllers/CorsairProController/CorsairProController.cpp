/*-----------------------------------------*\
|  CorsairProController.cpp                 |
|                                           |
|  Definitions and types for Corsair        |
|  Vengeance Pro RGB RAM lighting controller|
|                                           |
|  Adam Honse (CalcProgrammer1) 6/30/2019   |
\*-----------------------------------------*/

#include "CorsairProController.h"
#include <cstring>

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif

CorsairProController::CorsairProController(i2c_smbus_interface* bus, corsair_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    strcpy(device_name, "Corsair Vengeance Pro RGB");
    led_count = CORSAIR_PRO_LED_COUNT;

    for (int i = 0; i < led_count; i++)
    {
        led_red[i]      = 0;
        led_green[i]    = 0;
        led_blue[i]     = 0;
    }
}

CorsairProController::~CorsairProController()
{

}

std::string CorsairProController::GetDeviceName()
{
    return(device_name);
}

std::string CorsairProController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return(return_string);
}

unsigned int CorsairProController::GetLEDCount()
{
    return(led_count);
}

void CorsairProController::SetAllColors(unsigned char red, unsigned char green, unsigned char blue)
{
    for (int i = 0; i < led_count; i++)
    {
        led_red[i]      = red;
        led_green[i]    = green;
        led_blue[i]     = blue;
    }
}

void CorsairProController::SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    led_red[led]    = red;
    led_green[led]  = green;
    led_blue[led]   = blue;
}

void CorsairProController::ApplyColors()
{
    bus->i2c_smbus_write_byte_data(dev, 0x26, 0x02);
    Sleep(1);
    bus->i2c_smbus_write_byte_data(dev, 0x21, 0x00);
    Sleep(1);

    for (int i = 0; i < 10; i++)
    {
        bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, led_red[i]);
        bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, led_green[i]);
        bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, led_blue[i]);
        bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0xFF);
    }

    bus->i2c_smbus_write_byte_data(dev, 0x82, 0x02);
}

void CorsairProController::SetEffect(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(dev, 0x26, 0x01);
    Sleep(1);
    bus->i2c_smbus_write_byte_data(dev, 0x21, 0x00);
    Sleep(1);

    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, mode); //Mode
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, CORSAIR_PRO_SPEED_MEDIUM); //Speed
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, CORSAIR_PRO_EFFECT_RANDOM_COLORS); //Custom color
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, CORSAIR_PRO_DIRECTION_UP); //Direction
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00); // Custom color 1 red
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00); // Custom color 1 green
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00); // Custom color 1 blue
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0xFF);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00); // Custom color 2 red
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00); // Custom color 2 green
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00); // Custom color 2 blue
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0xFF);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_PRO_REG_COMMAND, 0x00);

    bus->i2c_smbus_write_byte_data(dev, 0x82, 0x01);
    WaitReady();
}

void CorsairProController::SetCustom()
{
    SetEffect(CORSAIR_PRO_MODE_STATIC);
}

bool CorsairProController::WaitReady()
{
    int i = 0;
    while (bus->i2c_smbus_read_byte_data(dev, 0x41) != 0x00)
    {
        i++;
        Sleep(1);
    }

    return false;
}
