#include "ColorfulTuringGPUController.h"
#include <cstring>

ColorfulTuringGPUController::ColorfulTuringGPUController(i2c_smbus_interface* bus, colorful_gpu_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

ColorfulTuringGPUController::~ColorfulTuringGPUController()
{

}

std::string ColorfulTuringGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

void ColorfulTuringGPUController::SetDirect(RGBColor color)
{
    uint8_t r = RGBGetRValue(color);
    uint8_t g = RGBGetGValue(color);
    uint8_t b = RGBGetBValue(color);
    uint8_t data_pkt[COLORFUL_PACKET_LENGTH] = { 0x88, 0x02, 0x32, 0x02, r, g, b};

    int crc = 1;

    for(int i = 0; i < COLORFUL_PACKET_LENGTH - 1; ++i)
    {
        crc += data_pkt[i];
    }
    crc &= 0xFF;
    crc = 256-crc;

    data_pkt[COLORFUL_PACKET_LENGTH - 1] = crc & 0xFF;

    bus->i2c_write_block(dev, COLORFUL_PACKET_LENGTH, data_pkt);
}
