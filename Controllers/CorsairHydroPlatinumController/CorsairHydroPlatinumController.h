/*-------------------------------------------------------------------*\
|  CorsairHydroPlatinumController.h                                   |
|                                                                     |
|  Driver for Corsair Hydro Platinum AIO Coolers                      |
|                                                                     |
|  Kasper         28th March 2021                                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <vector>
#include <string>
#include <hidapi/hidapi.h>

#define CORSAIR_HYDRO_PLATINUM_PACKET_SIZE 65
#define CORSAIR_HYDRO_PLATINUM_PACKET_DELAY 5

enum
{
    CORSAIR_HYDRO_PLATINUM_MAGIC_1 = 0b001,
    CORSAIR_HYDRO_PLATINUM_MAGIC_2 = 0b010,
    CORSAIR_HYDRO_PLATINUM_MAGIC_3 = 0b011,

    CORSAIR_HYDRO_PLATINUM_SET_LIGHTING_1 = 0b100,
    CORSAIR_HYDRO_PLATINUM_SET_LIGHTING_2 = 0b101,
    CORSAIR_HYDRO_PLATINUM_SET_LIGHTING_3 = 0b110,
};

class CorsairHydroPlatinumController
{
public:
    CorsairHydroPlatinumController(hid_device* dev_handle, const char* path);
    ~CorsairHydroPlatinumController();

    std::string GetLocation();
    std::string GetFirmwareString();
    void SetupColors(std::vector<RGBColor> colors);

private:
    hid_device* dev;
    std::string location;
    std::string firmware_version;

    std::atomic<unsigned int> sequence_number;

    void SendMagic(const uint8_t* magic, unsigned int command);
    void SendColors(std::vector<RGBColor> colors, unsigned int start, unsigned int end, unsigned int command);
    unsigned int GetSequenceNumber();
    uint8_t ComputePEC(const void * data, size_t size);
};
