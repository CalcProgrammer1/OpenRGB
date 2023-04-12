/*-----------------------------------------*\
|  PalitGPUController.cpp                   |
|                                           |
|  Driver for Palit GPU RGB                 |
|  lighting controller                      |
|                                           |
|  Manatsawin Hanmongkolchai  04/11/2023    |
\*-----------------------------------------*/

#include "PalitGPUController.h"

PalitGPUController::PalitGPUController(i2c_smbus_interface* bus, palit_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

PalitGPUController::~PalitGPUController()
{

}

std::string PalitGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

void PalitGPUController::SetDirect(unsigned char red, unsigned char green, unsigned char blue)
{
    // NvAPI_I2CWriteEx:  Dev: 0x08 RegSize: 0x01 Reg: 0x03 Size: 0x04 Data: 0xFF 0x00 0x00 0xFF
    uint8_t values[] = {red, green, blue, 0xFF};
    bus->i2c_smbus_write_i2c_block_data(dev, PALIT_GPU_REG_LED, sizeof(values), values);
}
