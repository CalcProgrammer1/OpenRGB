/*-----------------------------------------*\
|  ASRockPolychromeSMBusController.cpp      |
|                                           |
|  Driver for for ASRock ASR LED and        |
|  Polychrome RGB lighting controller       |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/14/2019  |
\*-----------------------------------------*/

#include "ASRockPolychromeSMBusController.h"
#include <cstring>
#include "dependencies/dmiinfo.h"

using namespace std::chrono_literals;

PolychromeController::PolychromeController(i2c_smbus_interface* bus, polychrome_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    DMIInfo dmi;

    unsigned short fw_version    = ReadFirmwareVersion();
    unsigned char  major_version = fw_version >> 8;

    /*-----------------------------------------------------*\
    | Determine whether the device uses ASR LED or          |
    | Polychrome protocol by checking firmware version.     |
    | Versions 1.xx and 2.xx use ASR LED, 3.xx uses         |
    | Polychrome                                            |
    \*-----------------------------------------------------*/
    switch(major_version)
    {
        case ASROCK_TYPE_ASRLED:
            device_name = "ASRock " + dmi.getMainboard();
            asrock_type = ASROCK_TYPE_ASRLED;
            memset(zone_led_count, 0, sizeof(zone_led_count));
            break;

        case ASROCK_TYPE_POLYCHROME_V1:
            device_name = "ASRock " + dmi.getMainboard();
            asrock_type = ASROCK_TYPE_POLYCHROME_V1;
            ReadLEDConfiguration();
            break;

        case ASROCK_TYPE_POLYCHROME_V2:
            device_name = "ASRock " + dmi.getMainboard();
            asrock_type = ASROCK_TYPE_POLYCHROME_V2;
            ReadLEDConfiguration();
            break;

        default:
            asrock_type = ASROCK_TYPE_UNKNOWN;
            break;
    }
}

PolychromeController::~PolychromeController()
{

}

unsigned int PolychromeController::GetASRockType()
{
    return(asrock_type);
}

std::string PolychromeController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string PolychromeController::GetDeviceName()
{
    return(device_name);
}

std::string PolychromeController::GetFirmwareVersion()
{
    unsigned short fw_version    = ReadFirmwareVersion();
    unsigned char  major_version = fw_version >> 8;
    unsigned char  minor_version = fw_version & 0xFF;

    return(std::to_string(major_version) + "." + std::to_string(minor_version));
}

unsigned short PolychromeController::ReadFirmwareVersion()
{
    // The firmware register holds two bytes, so the first read should return 2
    // If not, report invalid firmware revision FFFF
    unsigned char asrock_version[2] = { 0x00, 0x00};
    if (bus->i2c_smbus_read_block_data(dev, ASROCK_REG_FIRMWARE_VER, asrock_version) == 0x02)
    {
        unsigned char major = asrock_version[0];
        unsigned char minor = asrock_version[1];

        return((major << 8) | minor);
    }
    else
    {
        return(0xFFFF);
    }
}

void PolychromeController::ReadLEDConfiguration()
{
    /*---------------------------------------------------------------------------------*\
    | The LED configuration register holds 6 bytes, so the first read should return 6   |
    | If not, set all zone sizes to zero                                                |
    \*---------------------------------------------------------------------------------*/
    unsigned char asrock_zone_count[6] = { 0x0 };
    if (bus->i2c_smbus_read_block_data(dev, POLYCHROME_REG_LED_CONFIG, asrock_zone_count) == 0x06)
    {
        zone_led_count[POLYCHROME_ZONE_1]           = asrock_zone_count[0];
        zone_led_count[POLYCHROME_ZONE_2]           = asrock_zone_count[1];
        zone_led_count[POLYCHROME_ZONE_3]           = asrock_zone_count[2];
        zone_led_count[POLYCHROME_ZONE_4]           = asrock_zone_count[3];
        zone_led_count[POLYCHROME_ZONE_5]           = asrock_zone_count[4];
        zone_led_count[POLYCHROME_ZONE_ADDRESSABLE] = asrock_zone_count[5];
    }
    else
    {
        memset(zone_led_count, 0, sizeof(zone_led_count));
    }
}

unsigned int PolychromeController::GetMode()
{
    return(active_mode);
}

void PolychromeController::SetColorsAndSpeed(unsigned char led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char color_speed_pkt[4] = { red, green, blue, active_speed };
    unsigned char select_led_pkt[1]  = { led };
    
    switch(asrock_type)
    {
        case ASROCK_TYPE_ASRLED:
            /*-----------------------------------------------------*\
            | Select LED                                            |
            \*-----------------------------------------------------*/
            if(active_mode != ASRLED_MODE_OFF)
            {
                bus->i2c_smbus_write_block_data(dev, ASROCK_REG_LED_SELECT, 1, select_led_pkt);
                std::this_thread::sleep_for(1ms);
            }

            switch(active_mode)
            {
                /*-----------------------------------------------------*\
                | These modes take 4 bytes in R/G/B/S order             |
                \*-----------------------------------------------------*/
                case ASRLED_MODE_BREATHING:
                case ASRLED_MODE_STROBE:
                case ASRLED_MODE_SPECTRUM_CYCLE:
                    bus->i2c_smbus_write_block_data(dev, active_mode, 4, color_speed_pkt);
                    break;

                /*-----------------------------------------------------*\
                | These modes take 3 bytes in R/G/B order               |
                \*-----------------------------------------------------*/
                default:
                case ASRLED_MODE_STATIC:
                case ASRLED_MODE_MUSIC:
                    bus->i2c_smbus_write_block_data(dev, active_mode, 3, color_speed_pkt);
                    break;

                /*-----------------------------------------------------*\
                | These modes take 1 byte - speed                       |
                \*-----------------------------------------------------*/
                case ASRLED_MODE_RANDOM:
                case ASRLED_MODE_WAVE:
                    bus->i2c_smbus_write_block_data(dev, active_mode, 1, &active_speed);
                    break;

                /*-----------------------------------------------------*\
                | These modes take no bytes                             |
                \*-----------------------------------------------------*/
                case ASRLED_MODE_OFF:
                    break;
            }
            std::this_thread::sleep_for(1ms);
            break;

        case ASROCK_TYPE_POLYCHROME_V1:
            /*-----------------------------------------------------*\
            | Select LED                                            |
            \*-----------------------------------------------------*/
            if(active_mode != ASRLED_MODE_OFF)
            {
                bus->i2c_smbus_write_block_data(dev, ASROCK_REG_LED_SELECT, 1, select_led_pkt);
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
            break;

        case ASROCK_TYPE_POLYCHROME_V2:
            /*-----------------------------------------------------*\
            | Select LED                                            |
            \*-----------------------------------------------------*/
            switch(active_mode)
            {
                case POLYCHROME_V2_MODE_OFF:
                case POLYCHROME_V2_MODE_RAINBOW:
                case POLYCHROME_V2_MODE_SPECTRUM_CYCLE:
                    break;

                default:
                    bus->i2c_smbus_write_block_data(dev, ASROCK_REG_LED_SELECT, 1, select_led_pkt);
                    std::this_thread::sleep_for(1ms);

                    /*-----------------------------------------------------*\
                    | Polychrome firmware always writes color to fixed reg  |
                    \*-----------------------------------------------------*/
                    bus->i2c_smbus_write_block_data(dev, POLYCHROME_V2_REG_COLOR, 3, color_speed_pkt);
                    std::this_thread::sleep_for(1ms);
                    break;
            }
            break;
    }
}

void PolychromeController::SetMode(unsigned char zone,unsigned char mode, unsigned char speed)
{
    unsigned char led_count_pkt[1]  = { 0x00 };
    active_zone                     = zone;
    active_mode                     = mode;
    active_speed                    = speed;

    switch(asrock_type)
    {
        case ASROCK_TYPE_ASRLED:
            bus->i2c_smbus_write_block_data(dev, ASROCK_REG_MODE, 1, &active_mode);
            std::this_thread::sleep_for(1ms);
            break;

        case ASROCK_TYPE_POLYCHROME_V1:
            /*-----------------------------------------------------*\
            | Make sure set all register is set to 0                |
            \*-----------------------------------------------------*/
            bus->i2c_smbus_write_block_data(dev, POLYCHROME_V1_REG_SET_ALL, 1, led_count_pkt);
            std::this_thread::sleep_for(1ms);

            /*-----------------------------------------------------*\
            | Set the zone we are working on                        |
            \*-----------------------------------------------------*/
            bus->i2c_smbus_write_block_data(dev, ASROCK_REG_LED_SELECT, 1, &active_zone);
            std::this_thread::sleep_for(1ms);

            /*-----------------------------------------------------*\
            | Write the mode                                        |
            \*-----------------------------------------------------*/
            bus->i2c_smbus_write_block_data(dev, ASROCK_REG_MODE, 1, &active_mode);
            std::this_thread::sleep_for(1ms);
            break;

        case ASROCK_TYPE_POLYCHROME_V2:
            bus->i2c_smbus_write_block_data(dev, ASROCK_REG_MODE, 1, &active_mode);
            std::this_thread::sleep_for(1ms);

            /*-----------------------------------------------------*\
            | Select a single LED                                   |
            \*-----------------------------------------------------*/
            bus->i2c_smbus_write_block_data(dev, POLYCHROME_V2_REG_LED_COUNT, 0, led_count_pkt);
            std::this_thread::sleep_for(1ms);

            switch(active_mode)
            {
                /*-----------------------------------------------------*\
                | These modes don't take a speed                        |
                \*-----------------------------------------------------*/
                case POLYCHROME_V2_MODE_OFF:
                case POLYCHROME_V2_MODE_STATIC:
                    break;

                /*-----------------------------------------------------*\
                | All other modes, write speed to active mode register  |
                \*-----------------------------------------------------*/
                default:
                    bus->i2c_smbus_write_block_data(dev, active_mode, 1, &speed);
                    std::this_thread::sleep_for(1ms);
                    break;
            }
            break;
    }    
}
