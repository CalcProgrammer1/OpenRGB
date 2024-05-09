/*---------------------------------------------------------*\
| ASRockPolychromeV2SMBusController.h                       |
|                                                           |
|   Driver for SMBus ASRock Polychrome V2 motherboards      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                13 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef uint8_t polychrome_dev_id;

#define ASROCK_V2_CONTROLLER_NAME  "ASRock Polychrome v2 SMBus Controller"

enum
{
    /*------------------------------------------------------------------------------------------*\
    | ASRock Polychrome v2 Registers                                                             |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V2_REG_FIRMWARE_VER          = 0x00,     /* Firmware version Major.Minor         */
    POLYCHROME_V2_REG_MODE                  = 0x30,     /* Mode selection register              */
    POLYCHROME_V2_REG_LED_SELECT            = 0x31,     /* LED selection register               */
    POLYCHROME_V2_REG_LED_COUNT             = 0x32,     /* Additional LED count register        */
    POLYCHROME_V2_REG_LED_CONFIG            = 0x33,     /* LED configuration register           */
    POLYCHROME_V2_REG_COLOR                 = 0x34,     /* Color register: Red, Green, Blue     */
    POLYCHROME_V2_REG_ARGB_GRB              = 0x35,     /* ARGB bistream reversing register     */
};

enum
{
    POLYCHROME_V2_ZONE_1                    = 0x00,     /* RGB LED 1 Header                     */
    POLYCHROME_V2_ZONE_2                    = 0x01,     /* RGB LED 2 Header                     */
    POLYCHROME_V2_ZONE_3                    = 0x02,     /* Audio/PCH Zone LEDs                  */
    POLYCHROME_V2_ZONE_4                    = 0x03,     /* Audio/PCH Zone LEDs                  */
    POLYCHROME_V2_ZONE_5                    = 0x04,     /* IO Cover Zone LEDs                   */
    POLYCHROME_V2_ZONE_ADDRESSABLE          = 0x05,     /* Addressable LED header               */
    POLYCHROME_V2_ZONE_COUNT                = 0x06,     /* Total number of zones                */
    POLYCHROME_V2_ZONE_ADDRESSABLE_MAX      = 0x64,     /* Maxinum number of ARGB LEDs          */
};

/*----------------------------------------------------------------------------------------------*\
| Definitions for Polychrome V2                                                                  |
\*----------------------------------------------------------------------------------------------*/
#define POLYCHROME_V2_NUM_MODES             14          /* Number of Polychrome V2 modes        */

enum
{
    POLYCHROME_V2_MODE_OFF                  = 0x10,     /* OFF mode                             */
    POLYCHROME_V2_MODE_STATIC               = 0x11,     /* Static color mode                    */
    POLYCHROME_V2_MODE_BREATHING            = 0x12,     /* Breating effect mode                 */
    POLYCHROME_V2_MODE_STROBE               = 0x13,     /* Strobe effect mode                   */
    POLYCHROME_V2_MODE_SPECTRUM_CYCLE       = 0x14,     /* Spectrum Cycle effect mode           */
    POLYCHROME_V2_MODE_RANDOM               = 0x15,     /* Random effect mode                   */
    POLYCHROME_V2_MODE_WAVE                 = 0x17,     /* Wave effect mode                     */
    POLYCHROME_V2_MODE_SPRING               = 0x18,     /* Spring effect mode                   */
    POLYCHROME_V2_MODE_STACK                = 0x19,     /* Stack effect mode                    */
    POLYCHROME_V2_MODE_CRAM                 = 0x1A,     /* Cram effect mode                     */
    POLYCHROME_V2_MODE_SCAN                 = 0x1B,     /* Scan effect mode                     */
    POLYCHROME_V2_MODE_NEON                 = 0x1C,     /* Neon effect mode                     */
    POLYCHROME_V2_MODE_WATER                = 0x1D,     /* Water effect mode                    */
    POLYCHROME_V2_MODE_RAINBOW              = 0x1E,     /* Rainbow effect mode                  */
};

enum
{
    POLYCHROME_V2_BREATHING_SPEED_MIN               = 0x0A,     /* Slowest speed                        */
    POLYCHROME_V2_BREATHING_SPEED_DEFAULT           = 0x06,     /* Default speed                        */
    POLYCHROME_V2_BREATHING_SPEED_MAX               = 0x02,     /* Fastest speed                        */

    POLYCHROME_V2_STROBE_SPEED_MIN                  = 0xA0,     /* Slowest speed                        */
    POLYCHROME_V2_STROBE_SPEED_DEFAULT              = 0x4D,     /* Default speed                        */
    POLYCHROME_V2_STROBE_SPEED_MAX                  = 0x05,     /* Fastest speed                        */

    POLYCHROME_V2_SPECTRUM_CYCLE_SPEED_MIN          = 0xA0,     /* Slowest speed                        */
    POLYCHROME_V2_SPECTRUM_CYCLE_SPEED_DEFAULT      = 0x50,     /* Default speed                        */
    POLYCHROME_V2_SPECTRUM_CYCLE_SPEED_MAX          = 0x0A,     /* Fastest speed                        */

    POLYCHROME_V2_RANDOM_SPEED_MIN                  = 0xA0,     /* Slowest speed                        */
    POLYCHROME_V2_RANDOM_SPEED_DEFAULT              = 0x4D,     /* Default speed                        */
    POLYCHROME_V2_RANDOM_SPEED_MAX                  = 0x05,     /* Fastest speed                        */

    POLYCHROME_V2_WAVE_SPEED_MIN                    = 0x06,     /* Slowest speed                        */
    POLYCHROME_V2_WAVE_SPEED_DEFAULT                = 0x03,     /* Default speed                        */
    POLYCHROME_V2_WAVE_SPEED_MAX                    = 0x01,     /* Fastest speed                        */

    POLYCHROME_V2_SPRING_SPEED_MIN                  = 0x20,     /* Slowest speed                        */
    POLYCHROME_V2_SPRING_SPEED_DEFAULT              = 0x11,     /* Default speed                        */
    POLYCHROME_V2_SPRING_SPEED_MAX                  = 0x02,     /* Fastest speed                        */

    POLYCHROME_V2_STACK_SPEED_MIN                   = 0x20,     /* Slowest speed                        */
    POLYCHROME_V2_STACK_SPEED_DEFAULT               = 0x11,     /* Default speed                        */
    POLYCHROME_V2_STACK_SPEED_MAX                   = 0x02,     /* Fastest speed                        */

    POLYCHROME_V2_CRAM_SPEED_MIN                    = 0x20,     /* Slowest speed                        */
    POLYCHROME_V2_CRAM_SPEED_DEFAULT                = 0x11,     /* Default speed                        */
    POLYCHROME_V2_CRAM_SPEED_MAX                    = 0x02,     /* Fastest speed                        */

    POLYCHROME_V2_SCAN_SPEED_MIN                    = 0x20,     /* Slowest speed                        */
    POLYCHROME_V2_SCAN_SPEED_DEFAULT                = 0x11,     /* Default speed                        */
    POLYCHROME_V2_SCAN_SPEED_MAX                    = 0x02,     /* Fastest speed                        */

    POLYCHROME_V2_NEON_SPEED_MIN                    = 0x20,     /* Slowest speed                        */
    POLYCHROME_V2_NEON_SPEED_DEFAULT                = 0x11,     /* Default speed                        */
    POLYCHROME_V2_NEON_SPEED_MAX                    = 0x02,     /* Fastest speed                        */

    POLYCHROME_V2_WATER_SPEED_MIN                   = 0x20,     /* Slowest speed                        */
    POLYCHROME_V2_WATER_SPEED_DEFAULT               = 0x11,     /* Default speed                        */
    POLYCHROME_V2_WATER_SPEED_MAX                   = 0x02,     /* Fastest speed                        */

    POLYCHROME_V2_RAINBOW_SPEED_MIN                 = 0x12,     /* Slowest speed                        */
    POLYCHROME_V2_RAINBOW_SPEED_DEFAULT             = 0x08,     /* Default speed                        */
    POLYCHROME_V2_RAINBOW_SPEED_MAX                 = 0x02,     /* Fastest speed                        */
};

class ASRockPolychromeV2SMBusController
{
public:
    ASRockPolychromeV2SMBusController(i2c_smbus_interface* bus, polychrome_dev_id dev);
    ~ASRockPolychromeV2SMBusController();

    std::string             GetDeviceLocation();
    std::string             GetDeviceName();
    std::string             GetFirmwareVersion();
    uint8_t                 GetMode();
    void                    SetColorsAndSpeed(uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
    void                    SetMode(uint8_t zone, uint8_t mode, uint8_t speed);

    uint8_t                 zone_led_count[6];
    uint16_t                fw_version;

private:
    std::string             device_name;
    uint8_t                 active_zone;
    uint8_t                 active_mode;
    uint8_t                 active_speed;
    i2c_smbus_interface*    bus;
    polychrome_dev_id       dev;

    void            ReadLEDConfiguration();
};
