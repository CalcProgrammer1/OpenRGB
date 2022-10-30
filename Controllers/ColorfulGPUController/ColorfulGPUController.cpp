#include "ColorfulGPUController.h"
#include <cstring>

ColorfulGPUController::ColorfulGPUController(i2c_smbus_interface* bus, colorful_gpu_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

ColorfulGPUController::~ColorfulGPUController()
{

}

std::string ColorfulGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

void ColorfulGPUController::SetDirect(RGBColor color)
{
    uint8_t r = RGBGetRValue(color);
    uint8_t g = RGBGetGValue(color);
    uint8_t b = RGBGetBValue(color);

    uint8_t data_pkt[COLORFUL_PACKET_LENGTH] = { 0xAA, 0xEF, 0x12, 0x03, 0x01, 0xFF, r, g, b};

    int crc = 0;

    for (int i = 0; i < COLORFUL_PACKET_LENGTH - 2; ++i)
    {
        crc += data_pkt[i];
    }

    data_pkt[COLORFUL_PACKET_LENGTH - 2] = crc & 0xFF;
    data_pkt[COLORFUL_PACKET_LENGTH - 1] = crc >> 8;

    bus->i2c_write_block(dev, COLORFUL_PACKET_LENGTH, data_pkt);
}
