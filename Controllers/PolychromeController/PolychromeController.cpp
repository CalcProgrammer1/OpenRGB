/*-----------------------------------------*\
|  PolychromeController.cpp                 |
|                                           |
|  Driver for for ASRock ASR LED and        |
|  Polychrome RGB lighting controller       |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/14/2019  |
\*-----------------------------------------*/

#include "PolychromeController.h"
#include <cstring>

PolychromeController::PolychromeController(i2c_smbus_interface* bus, polychrome_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
    
    switch (GetFirmwareVersion())
    {
    case FIRMWARE_VER_1_PT_10:
        led_count = 1;
        strcpy(device_name, "ASRock ASR LED FW 1.10");
        break;

    case FIRMWARE_VER_2_PT_00:
        led_count = 1;
        strcpy(device_name, "ASRock Polychrome FW 2.00");
        break;

    case FIRMWARE_VER_3_PT_00:
        led_count = 1;
        strcpy(device_name, "ASRock Polychrome FW 3.00");
        break;
    }
}

PolychromeController::~PolychromeController()
{

}

char* PolychromeController::GetDeviceName()
{
    return(device_name);
}

unsigned short PolychromeController::GetFirmwareVersion()
{
    // The firmware register holds two bytes, so the first read should return 2
    // If not, report invalid firmware revision FFFF
    if (bus->i2c_smbus_read_byte_data(dev, POLYCHROME_REG_FIRMWARE_VER) == 0x02)
    {
        unsigned char major = bus->i2c_smbus_read_byte(dev);
        unsigned char minor = bus->i2c_smbus_read_byte(dev);

        return((major << 8) | minor);
    }
    else
    {
        return(0xFFFF);
    }
}

unsigned int PolychromeController::GetLEDCount()
{
    return(led_count);
}

void PolychromeController::SetAllColors(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char* colors = new unsigned char[led_count * 3];

    for (int i = 0; i < (led_count * 3); i += 3)
    {
        colors[i + 0] = red;
        colors[i + 1] = blue;
        colors[i + 2] = green;
    }

    delete colors;
}

void PolychromeController::SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };
}

void PolychromeController::SetMode(unsigned char mode)
{
    // For ASR LED protocol
    //placeholder//

    // For Polychrome protocol
    bus->i2c_smbus_write_byte_data(dev, POLYCHROME_REG_MODE, mode);
}