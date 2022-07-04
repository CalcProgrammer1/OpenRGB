/*-----------------------------------------*\
|  ASRockPolychromeV1SMBusController.cpp    |
|                                           |
|  Driver for for ASRock ASR LED and        |
|  Polychrome RGB lighting controller       |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/14/2019  |
\*-----------------------------------------*/

#include "ASRockPolychromeV1SMBusController.h"
#include <cstring>
#include "dependencies/dmiinfo.h"
#include "LogManager.h"

#define ASROCK_ZONE_LED_COUNT_MESSAGE_EN    "[%s] Zone %i LED count: %02d"

using namespace std::chrono_literals;

ASRockPolychromeV1SMBusController::ASRockPolychromeV1SMBusController(i2c_smbus_interface* bus, polychrome_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    DMIInfo dmi;

    device_name =   "ASRock " + dmi.getMainboard();

    ReadLEDConfiguration();
}

ASRockPolychromeV1SMBusController::~ASRockPolychromeV1SMBusController()
{

}

std::string ASRockPolychromeV1SMBusController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string ASRockPolychromeV1SMBusController::GetDeviceName()
{
    return(device_name);
}

std::string ASRockPolychromeV1SMBusController::GetFirmwareVersion()
{
    uint8_t  major_version = fw_version >> 8;
    uint8_t  minor_version = fw_version & 0xFF;

    return(std::to_string(major_version) + "." + std::to_string(minor_version));
}

void ASRockPolychromeV1SMBusController::ReadLEDConfiguration()
{
    /*---------------------------------------------------------------------------------*\
    | The LED configuration register holds 6 bytes, so the first read should return 6   |
    | If not, set all zone sizes to zero                                                |
    \*---------------------------------------------------------------------------------*/
    LOG_DEBUG("[%s] Reading LED config from controller", device_name.c_str());
    uint8_t asrock_zone_count[I2C_SMBUS_BLOCK_MAX] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
    if (bus->i2c_smbus_read_block_data(dev, POLYCHROME_V1_REG_LED_CONFIG, asrock_zone_count) == 0x06)
    {
        zone_led_count[POLYCHROME_V1_ZONE_1]           = asrock_zone_count[0];
        zone_led_count[POLYCHROME_V1_ZONE_2]           = asrock_zone_count[1];
        zone_led_count[POLYCHROME_V1_ZONE_3]           = asrock_zone_count[2];
        zone_led_count[POLYCHROME_V1_ZONE_4]           = asrock_zone_count[3];
        zone_led_count[POLYCHROME_V1_ZONE_5]           = asrock_zone_count[4];
        zone_led_count[POLYCHROME_V1_ZONE_ADDRESSABLE] = asrock_zone_count[5];
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V1_ZONE_1, zone_led_count[POLYCHROME_V1_ZONE_1]);
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V1_ZONE_2, zone_led_count[POLYCHROME_V1_ZONE_2]);
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V1_ZONE_3, zone_led_count[POLYCHROME_V1_ZONE_3]);
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V1_ZONE_4, zone_led_count[POLYCHROME_V1_ZONE_4]);
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V1_ZONE_5, zone_led_count[POLYCHROME_V1_ZONE_5]);
        LOG_DEBUG("[%s] Addressable Zone LED count: %02d", device_name.c_str(), zone_led_count[POLYCHROME_V1_ZONE_ADDRESSABLE]);
    }
    else
    {
        LOG_WARNING("[%s] LED config read failed", device_name.c_str());
        memset(zone_led_count, 0, sizeof(zone_led_count));
    }
}

uint8_t ASRockPolychromeV1SMBusController::GetMode()
{
    return(active_mode);
}

void ASRockPolychromeV1SMBusController::SetColorsAndSpeed(uint8_t led, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t color_speed_pkt[4] = { red, green, blue, active_speed };
    uint8_t select_led_pkt[1]  = { led };
    

    /*-----------------------------------------------------*\
    | Select LED                                            |
    \*-----------------------------------------------------*/
    if(active_mode != POLYCHROME_V1_MODE_OFF)
    {
        bus->i2c_smbus_write_block_data(dev, POLYCHROME_V1_REG_LED_SELECT, 1, select_led_pkt);
        std::this_thread::sleep_for(1ms);
    }

    switch(active_mode)
    {
        /*-----------------------------------------------------*\
        | These modes take 4 bytes in R/G/B/S order             |
        \*-----------------------------------------------------*/
        case POLYCHROME_V1_MODE_BREATHING:
        case POLYCHROME_V1_MODE_STROBE:
        case POLYCHROME_V1_MODE_SPECTRUM_CYCLE:
        case POLYCHROME_V1_MODE_SPRING:
        case POLYCHROME_V1_MODE_METEOR:
        case POLYCHROME_V1_MODE_STACK:
        case POLYCHROME_V1_MODE_CRAM:
        case POLYCHROME_V1_MODE_SCAN:
        case POLYCHROME_V1_MODE_NEON:
        case POLYCHROME_V1_MODE_WATER:

            bus->i2c_smbus_write_block_data(dev, active_mode, 4, color_speed_pkt);
            break;

        /*-----------------------------------------------------*\
        | These modes take 3 bytes in R/G/B order               |
        \*-----------------------------------------------------*/
        default:
        case POLYCHROME_V1_MODE_STATIC:
        case POLYCHROME_V1_MODE_MUSIC:
            bus->i2c_smbus_write_block_data(dev, active_mode, 3, color_speed_pkt);
            break;

        /*-----------------------------------------------------*\
        | These modes take 1 byte - speed                       |
        \*-----------------------------------------------------*/
        case POLYCHROME_V1_MODE_RANDOM:
        case POLYCHROME_V1_MODE_WAVE:
        case POLYCHROME_V1_MODE_RAINBOW:
            bus->i2c_smbus_write_block_data(dev, active_mode, 1, &active_speed);
            break;

        /*-----------------------------------------------------*\
        | These modes take no bytes                             |
        \*-----------------------------------------------------*/
        case POLYCHROME_V1_MODE_OFF:
            break;
    }
    std::this_thread::sleep_for(1ms);

}

void ASRockPolychromeV1SMBusController::SetMode(uint8_t zone,uint8_t mode, uint8_t speed)
{
    uint8_t led_count_pkt[1]  = { 0x00 };
    active_zone                     = zone;
    active_mode                     = mode;
    active_speed                    = speed;

    /*-----------------------------------------------------*\
    | Make sure set all register is set to 0                |
    \*-----------------------------------------------------*/
    bus->i2c_smbus_write_block_data(dev, POLYCHROME_V1_REG_SET_ALL, 1, led_count_pkt);
    std::this_thread::sleep_for(1ms);

    /*-----------------------------------------------------*\
    | Set the zone we are working on                        |
    \*-----------------------------------------------------*/
    bus->i2c_smbus_write_block_data(dev, POLYCHROME_V1_REG_LED_SELECT, 1, &active_zone);
    std::this_thread::sleep_for(1ms);

    /*-----------------------------------------------------*\
    | Write the mode                                        |
    \*-----------------------------------------------------*/
    bus->i2c_smbus_write_block_data(dev, POLYCHROME_V1_REG_MODE, 1, &active_mode);
    std::this_thread::sleep_for(1ms);    
}
