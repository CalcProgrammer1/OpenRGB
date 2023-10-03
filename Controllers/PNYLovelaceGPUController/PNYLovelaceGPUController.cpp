/*-----------------------------------------*\
|  PNYLovelaceGPUController.cpp             |
|                                           |
|  Driver for PNY GPU RGB (Lovelace)        |
|  lighting controller                      |
|                                           |
|  yufan   10/1/2023                        |
\*-----------------------------------------*/

#include "PNYLovelaceGPUController.h"

PNYLovelaceGPUController::PNYLovelaceGPUController(i2c_smbus_interface* bus, pny_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

PNYLovelaceGPUController::~PNYLovelaceGPUController()
{

}

std::string PNYLovelaceGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

void PNYLovelaceGPUController::SetOff()
{
    unsigned char data[7] = {};
    bus->i2c_smbus_write_i2c_block_data(dev, PNY_GPU_REG_LIGHTING, sizeof(data), data);
}

void PNYLovelaceGPUController::SetDirect(unsigned char led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char data[7] = {PNY_GPU_MODE_STATIC, 0xFF, led, 0x00, red, green, blue};
    bus->i2c_smbus_write_i2c_block_data(dev, PNY_GPU_REG_LIGHTING, sizeof(data), data);
}

void PNYLovelaceGPUController::SetCycle(unsigned char speed, unsigned char brightness)
{
    speed = 0xB2 - speed;
    unsigned char data[7] = {PNY_GPU_MODE_CYCLE, brightness, speed, 0x00, 0xAA, 0x00, 0x00};
    bus->i2c_smbus_write_i2c_block_data(dev, PNY_GPU_REG_LIGHTING, sizeof(data), data);
}

void PNYLovelaceGPUController::SetBreath(unsigned char speed, unsigned char red, unsigned char green, unsigned char blue)
{
    speed = 0x19 - speed;
    unsigned char data[7] = {PNY_GPU_MODE_BREATH, 0xFF, speed, 0x01, red, green, blue};
    bus->i2c_smbus_write_i2c_block_data(dev, PNY_GPU_REG_LIGHTING, sizeof(data), data);
}

void PNYLovelaceGPUController::SetWave(unsigned char speed, unsigned char brightness)
{
    speed = 0xBF - speed;
    unsigned char data[7] = {PNY_GPU_MODE_WAVE, brightness, speed, 0x00, 0xAA, 0x00, 0x00};
    bus->i2c_smbus_write_i2c_block_data(dev, PNY_GPU_REG_LIGHTING, sizeof(data), data);
}

void PNYLovelaceGPUController::SetFlash(unsigned char speed, unsigned char brightness, unsigned char red, unsigned char green, unsigned char blue)
{
    speed = 0x4D - speed;
    unsigned char data[7] = {PNY_GPU_MODE_FLASH, brightness, speed, 0x00, red, green, blue};
    bus->i2c_smbus_write_i2c_block_data(dev, PNY_GPU_REG_LIGHTING, sizeof(data), data);
}
