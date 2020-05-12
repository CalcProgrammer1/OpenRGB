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
    
    unsigned short fw_version = GetFirmwareVersion();

    /*-----------------------------------------------------*\
    | Determine whether the device uses ASR LED or          |
    | Polychrome protocol by checking firmware version.     |
    | Versions 1.xx and 2.xx use ASR LED, 3.xx uses         |
    | Polychrome                                            |
    \*-----------------------------------------------------*/
    if((fw_version >> 8) < 0x03)
    {
        snprintf(device_name, 32, "ASRock ASR LED FW %d.%02d", (fw_version >> 8), (fw_version & 0xFF));
        led_count   = 1;
        asr_led     = true;
    }
    else
    {
        snprintf(device_name, 32, "ASRock Polychrome FW %d.%02d", (fw_version >> 8), (fw_version & 0xFF));
        led_count   = 1;
        asr_led     = false;
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

unsigned int PolychromeController::GetMode()
{
    return(active_mode);
}

bool PolychromeController::IsAsrLed()
{
    return(asr_led);
}

void PolychromeController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, green, blue };

    if (asr_led)
    {
        bus->i2c_smbus_write_block_data(dev, active_mode, 3, colors);
    }
    else
    {
        bus->i2c_smbus_write_block_data(dev, POLYCHROME_REG_COLOR, 3, colors);
    }
}

void PolychromeController::SetMode(unsigned char mode)
{
    active_mode = mode;

    if (asr_led)
    {
        bus->i2c_smbus_write_block_data(dev, ASRLED_REG_MODE, 1, &active_mode);
    }
    else
    {
        bus->i2c_smbus_write_block_data(dev, POLYCHROME_REG_MODE, 1, &active_mode);
    }    
}
