/*-----------------------------------------*\
|  HyperXQuadcastSController.h              |
|                                           |
|  Implementation for the HyperX            |
|  Quadcast S RGB microphone                |
|                                           |
|  Matt Silva (thesilvanator) 2022          |
\*-----------------------------------------*/
#pragma once

#include <string>
#include "hidapi_wrapper.h"
#include "LogManager.h"
#include "RGBController.h"

#define HYPERX_QUADCAST_S_PACKET_SIZE 64 + 1
#define HYPERX_QUADCAST_S_FRAME_SIZE  8

class HyperXQuadcastSController
{
public:
    HyperXQuadcastSController(hidapi_wrapper hid_wrapper, hid_device* dev, std::string path);
    ~HyperXQuadcastSController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendDirect(std::vector<RGBColor> color_data);
    void SaveColors(std::vector<RGBColor> colors, unsigned int num_frames);

private:
    hidapi_wrapper      wrapper;
    hid_device*         dev;
    std::string         location;
    std::string         serial_number;
    std::mutex          lock;

    void                SendEOT(uint8_t frame_count);
    void                SendToRegister(uint8_t reg, uint8_t param1, uint8_t param2);
};
