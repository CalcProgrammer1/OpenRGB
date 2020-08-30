/*-----------------------------------------*\
|  PolychromeController.h                   |
|                                           |
|  Definitions and types for ASRock         |
|  ASR LED and Polychrome RGB lighting      |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/13/2019  |
\*-----------------------------------------*/

#include "i2c_smbus.h"
#include <string>

#pragma once

typedef unsigned char	polychrome_dev_id;

enum
{
    ASRLED_REG_FIRMWARE_VER         = 0x00,     /* Firmware version Major.Minor         */
    ASRLED_REG_MODE                 = 0x30,     /* Mode selection register              */
};

#define ASRLED_NUM_MODES             8          /* Number of ASR LED modes              */

enum
{
    ASRLED_MODE_OFF                 = 0x10,     /* OFF mode                             */
    ASRLED_MODE_STATIC              = 0x11,     /* Static color mode                    */
    ASRLED_MODE_BREATHING           = 0x12,     /* Breathing effect mode                */
    ASRLED_MODE_FLASHING            = 0x13,     /* Flashing effect mode                 */
    ASRLED_MODE_SPECTRUM_CYCLE      = 0x14,     /* Spectrum Cycle effect mode           */
    ASRLED_MODE_RANDOM              = 0x15,     /* Random effect mode                   */
    ASRLED_MODE_MUSIC               = 0x17,     /* Music effect mode                    */
    ASRLED_MODE_WAVE                = 0x18,     /* Wave effect mode                     */
};

enum
{
    ASRLED_SPEED_MIN                = 0x05,     /* Slowest speed                        */
    ASRLED_SPEED_DEFAULT            = 0x03,     /* Default speed                        */
    ASRLED_SPEED_MAX                = 0x00,     /* Fastest speed                        */
};

enum
{
    POLYCHROME_REG_FIRMWARE_VER     = 0x00,     /* Firmware version Major.Minor         */
    POLYCHROME_REG_MODE             = 0x30,     /* Mode selection register              */
    POLYCHROME_REG_LED_SELECT       = 0x31,     /* LED selection register               */
    POLYCHROME_REG_LED_COUNT        = 0x32,     /* Additional LED count register        */
    POLYCHROME_REG_LED_CONFIG       = 0x33,     /* LED configuration register           */
    POLYCHROME_REG_COLOR            = 0x34,     /* Color register: Red, Green, Blue     */
};

#define POLYCHROME_NUM_MODES          14        /* Number of Polychrome modes           */

enum
{
    POLYCHROME_MODE_OFF             = 0x10,     /* OFF mode                             */
    POLYCHROME_MODE_STATIC          = 0x11,     /* Static color mode                    */
    POLYCHROME_MODE_BREATHING       = 0x12,     /* Breating effect mode                 */
    POLYCHROME_MODE_FLASHING        = 0x13,     /* Flashing effect mode                 */
    POLYCHROME_MODE_SPECTRUM_CYCLE  = 0x14,     /* Spectrum Cycle effect mode           */
    POLYCHROME_MODE_RANDOM          = 0x15,     /* Random effect mode                   */
    POLYCHROME_MODE_WAVE            = 0x17,     /* Wave effect mode                     */
    POLYCHROME_MODE_SPRING          = 0x18,     /* Spring effect mode                   */
    POLYCHROME_MODE_STACK           = 0x19,     /* Stack effect mode                    */
    POLYCHROME_MODE_CRAM            = 0x1A,     /* Cram effect mode                     */
    POLYCHROME_MODE_SCAN            = 0x1B,     /* Scan effect mode                     */
    POLYCHROME_MODE_NEON            = 0x1C,     /* Neon effect mode                     */
    POLYCHROME_MODE_WATER           = 0x1D,     /* Water effect mode                    */
    POLYCHROME_MODE_RAINBOW         = 0x1E,     /* Rainbow effect mode                  */
};

enum
{
    POLYCHROME_SPEED_MIN            = 0x05,     /* Slowest speed                        */
    POLYCHROME_SPEED_DEFAULT        = 0x03,     /* Default speed                        */
    POLYCHROME_SPEED_MAX            = 0x00,     /* Fastest speed                        */
};

enum
{
    POLYCHROME_ZONE_RGB_FAN_HDR     = 0x00,     /* RGB Fan Header                       */
    POLYCHROME_ZONE_RGB_LED_HDR     = 0x01,     /* RGB LED Header                       */
    POLYCHROME_ZONE_AUDIO           = 0x02,     /* Audio Zone LEDs                      */
    POLYCHROME_ZONE_PCH             = 0x03,     /* PCH Zone LEDs                        */
    POLYCHROME_ZONE_IO_COVER        = 0x04,     /* IO Cover Zone LEDs                   */
    POLYCHROME_ZONE_ADDRESSABLE     = 0x05,     /* Addressable LED header               */
};

class PolychromeController
{
public:
    PolychromeController(i2c_smbus_interface* bus, polychrome_dev_id dev);
    ~PolychromeController();

    std::string     GetDeviceName();
    std::string     GetFirmwareVersion();
    unsigned int    GetMode();
    bool            IsAsrLed();
    void            SetColorsAndSpeed(unsigned char led, unsigned char red, unsigned char green, unsigned char blue);
    void            SetMode(unsigned char mode, unsigned char speed);

private:
    bool                    asr_led;
    std::string             device_name;
    unsigned int            zone_led_count[6];
    unsigned char           active_mode;
    unsigned char           active_speed;
    i2c_smbus_interface*    bus;
    polychrome_dev_id       dev;

    unsigned short  ReadFirmwareVersion();
    void            ReadLEDConfiguration();
};
