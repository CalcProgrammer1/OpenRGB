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

#pragma once

typedef unsigned char	polychrome_dev_id;

enum
{
    FIRMWARE_VER_1_PT_10            = 0x010A,   /* Firmware nu51_1.10                   */
    FIRMWARE_VER_2_PT_00            = 0x0200,   /* Firmware nu51_2.00                   */
    FIRMWARE_VER_2_PT_10            = 0x020A,   /* Firmware nu51_2.10                   */
    FIRMWARE_VER_3_PT_00            = 0x0300,   /* Firmware nu51_3.00                   */
};

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
    POLYCHROME_REG_FIRMWARE_VER     = 0x00,     /* Firmware version Major.Minor         */
    POLYCHROME_REG_MODE             = 0x30,     /* Mode selection register              */
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

class PolychromeController
{
public:
    PolychromeController(i2c_smbus_interface* bus, polychrome_dev_id dev);
    ~PolychromeController();

    char*           GetDeviceName();
    unsigned int    GetLEDCount();
    unsigned int    GetMode();
    bool            IsAsrLed();
    void            SetColor(unsigned char red, unsigned char green, unsigned char blue);
    void            SetMode(unsigned char mode);
    unsigned short  GetFirmwareVersion();

private:
    bool                    asr_led;
    char                    device_name[32];
    unsigned int            led_count;
    unsigned char           active_mode;
    i2c_smbus_interface*    bus;
    polychrome_dev_id       dev;

};
