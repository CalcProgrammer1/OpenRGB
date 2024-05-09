/*---------------------------------------------------------*\
| ASRockASRRGBSMBusController.h                             |
|                                                           |
|   Driver for SMBus ASRock ASR RGB motherboards            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                13 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef uint8_t	polychrome_dev_id;

#define ASROCK_ASR_CONTROLLER_NAME  "ASRock ASR RGB SMBus Controller"

enum
{
    /*------------------------------------------------------------------------------------------*\
    | ASRock Common Registers                                                                    |
    \*------------------------------------------------------------------------------------------*/
    ASROCK_ASR_REG_FIRMWARE_VER             = 0x00,     /* Firmware version Major.Minor         */
    ASROCK_ASR_REG_MODE                     = 0x30,     /* Mode selection register              */
    ASROCK_ASR_REG_LED_SELECT               = 0x31,     /* LED selection register               */
};

/*----------------------------------------------------------------------------------------------*\
| Definitions for ASR LED                                                                        |
\*----------------------------------------------------------------------------------------------*/
#define ASRLED_NUM_MODES                    8           /* Number of ASR LED modes              */

enum
{
    ASRLED_MODE_OFF                         = 0x10,     /* OFF mode                             */
    ASRLED_MODE_STATIC                      = 0x11,     /* Static color mode                    */
    ASRLED_MODE_BREATHING                   = 0x12,     /* Breathing effect mode                */
    ASRLED_MODE_STROBE                      = 0x13,     /* Strobe effect mode                   */
    ASRLED_MODE_SPECTRUM_CYCLE              = 0x14,     /* Spectrum Cycle effect mode           */
    ASRLED_MODE_RANDOM                      = 0x15,     /* Random effect mode                   */
    ASRLED_MODE_MUSIC                       = 0x17,     /* Music effect mode                    */
    ASRLED_MODE_WAVE                        = 0x18,     /* Wave effect mode                     */
};

enum
{
    ASRLED_SPEED_MIN                        = 0x05,     /* Slowest speed                        */
    ASRLED_SPEED_DEFAULT                    = 0x03,     /* Default speed                        */
    ASRLED_SPEED_MAX                        = 0x00,     /* Fastest speed                        */
};

class ASRockASRRGBSMBusController
{
public:
    ASRockASRRGBSMBusController(i2c_smbus_interface* bus, polychrome_dev_id dev);
    ~ASRockASRRGBSMBusController();

    std::string             GetDeviceLocation();
    std::string             GetDeviceName();
    std::string             GetFirmwareVersion();
    uint8_t                 GetMode();
    void                    SetColorsAndSpeed(uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
    void                    SetMode(uint8_t zone, uint8_t mode, uint8_t speed);

    uint16_t                fw_version;

private:
    std::string             device_name;
    uint8_t                 active_zone;
    uint8_t                 active_mode;
    uint8_t                 active_speed;
    i2c_smbus_interface*    bus;
    polychrome_dev_id       dev;
};
