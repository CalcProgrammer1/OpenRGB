/*-----------------------------------------*\
|  ASRockPolychromeSMBusController.h        |
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

#define ASROCK_CONTROLLER_NAME  "ASRock Polychrome SMBus Controller"
#define ASROCK_DETECTOR_NAME    "ASRock Polychrome SMBus Detect"
#define VENDOR_NAME             "ASRock"
#define SMBUS_ADDRESS           0x6A

enum
{
    ASROCK_TYPE_UNKNOWN                     = 0x00,     /* Unknown Type or Not ASRock Device    */
    ASROCK_TYPE_ASRLED                      = 0x01,     /* ASRock Firmware 1.x - ASR LED        */
    ASROCK_TYPE_POLYCHROME_V1               = 0x02,     /* ASRock Firmware 2.x - Polychrome V1  */
    ASROCK_TYPE_POLYCHROME_V2               = 0x03,     /* ASRock Firmware 3.x - Polychrome V2  */
};

enum
{
    /*------------------------------------------------------------------------------------------*\
    | ASRock Common Registers                                                                    |
    \*------------------------------------------------------------------------------------------*/
    ASROCK_REG_FIRMWARE_VER                 = 0x00,     /* Firmware version Major.Minor         */
    ASROCK_REG_MODE                         = 0x30,     /* Mode selection register              */
    ASROCK_REG_LED_SELECT                   = 0x31,     /* LED selection register               */
    /*------------------------------------------------------------------------------------------*\
    | ASRock Polychrome V1/V2 Common Registers                                                   |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_REG_LED_CONFIG               = 0x33,     /* LED configuration register           */
    /*------------------------------------------------------------------------------------------*\
    | ASRock Polychrome V1 (Firmware 2.x) Registers                                              |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_REG_SET_ALL               = 0x32,     /* Set All register 0x1 = set all       */
    POLYCHROME_V1_REG_ARGB_GRB              = 0x35,     /* ARGB bistream reversing register     */
    /*------------------------------------------------------------------------------------------*\
    | ASRock Polychrome V2 (Firmware 3.x) Registers                                              |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V2_REG_LED_COUNT             = 0x32,     /* Additional LED count register        */
    POLYCHROME_V2_REG_COLOR                 = 0x34,     /* Color register: Red, Green, Blue     */
    POLYCHROME_V2_REG_ARGB_GRB              = 0x35,     /* ARGB bistream reversing register     */
};

enum
{
    POLYCHROME_ZONE_1                       = 0x00,     /* RGB LED 1 Header                     */
    POLYCHROME_ZONE_2                       = 0x01,     /* RGB LED 2 Header                     */
    POLYCHROME_ZONE_3                       = 0x02,     /* Audio/PCH Zone LEDs                  */
    POLYCHROME_ZONE_4                       = 0x03,     /* Audio/PCH Zone LEDs                  */
    POLYCHROME_ZONE_5                       = 0x04,     /* IO Cover Zone LEDs                   */
    POLYCHROME_ZONE_ADDRESSABLE             = 0x05,     /* Addressable LED header               */
    POLYCHROME_ZONE_COUNT                   = 0x06,     /* Total number of zones                */
    POLYCHROME_ZONE_ADDRESSABLE_MAX         = 0x64,     /* Maxinum number of ARGB LEDs          */
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

/*----------------------------------------------------------------------------------------------*\
| Definitions for Polychrome V1                                                                  |
\*----------------------------------------------------------------------------------------------*/
#define POLYCHROME_V1_NUM_MODES             16          /* Number of Polychrome V1 modes        */

enum
{
    POLYCHROME_V1_MODE_OFF                  = 0x10,     /* OFF mode                             */
    POLYCHROME_V1_MODE_STATIC               = 0x11,     /* Static color mode                    */
    POLYCHROME_V1_MODE_BREATHING            = 0x12,     /* Breating effect mode                 */
    POLYCHROME_V1_MODE_STROBE               = 0x13,     /* Strobe effect mode                   */
    POLYCHROME_V1_MODE_SPECTRUM_CYCLE       = 0x14,     /* Spectrum Cycle effect mode           */
    POLYCHROME_V1_MODE_RANDOM               = 0x15,     /* Random effect mode                   */
    POLYCHROME_V1_MODE_MUSIC                = 0x17,     /* Music effect mode                    */
    POLYCHROME_V1_MODE_WAVE                 = 0x18,     /* Wave effect mode                     */
    /*------------------------------------------------------------------------------------------*\
    | Modes only available on ARGB headers                                                       |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_MODE_SPRING               = 0x19,     /* Spring effect mode                   */
    POLYCHROME_V1_MODE_METEOR               = 0x1A,     /* Meteor effect mode                   */
    POLYCHROME_V1_MODE_STACK                = 0x1B,     /* Stack effect mode                    */
    POLYCHROME_V1_MODE_CRAM                 = 0x1C,     /* Cram effect mode                     */
    POLYCHROME_V1_MODE_SCAN                 = 0x1D,     /* Scan effect mode                     */
    POLYCHROME_V1_MODE_NEON                 = 0x1E,     /* Neon effect mode                     */
    POLYCHROME_V1_MODE_WATER                = 0x1F,     /* Water effect mode                    */
    POLYCHROME_V1_MODE_RAINBOW              = 0x20,     /* Rainbow chase effect mode            */
};

enum
{
    /*------------------------------------------------------------------------------------------*\
    | POLYCHROME_V1_MODE_BREATHING                                                               |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_SPEED_MIN_BREATHING       = 0x0A,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_BREATHING   = 0x02,     /* Default speed                        */
    POLYCHROME_V1_SPEED_MAX_BREATHING       = 0x02,     /* Fastest speed                        */
    /*------------------------------------------------------------------------------------------*\
    | POLYCHROME_V1_MODE_STROBE                                                                  |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_SPEED_MIN_STROBE          = 0xA0,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_STROBE      = 0x14,     /* Default speed                        */
    POLYCHROME_V1_SPEED_MAX_STROBE          = 0x05,     /* Fastest speed                        */
    /*------------------------------------------------------------------------------------------*\
    | POLYCHROME_V1_MODE_SPECTRUM_CYCLE                                                          |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_SPEED_MIN_CYCLE           = 0xA0,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_CYCLE       = 0x14,     /* Default speed                        */
    POLYCHROME_V1_SPEED_MAX_CYCLE           = 0x0A,     /* Fastest speed                        */
    /*------------------------------------------------------------------------------------------*\
    | POLYCHROME_V1_MODE_RANDOM                                                                  |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_SPEED_MIN_RANDOM          = 0xA0,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_RANDOM      = 0x28,     /* Default speed                        */
    POLYCHROME_V1_SPEED_MAX_RANDOM          = 0x05,     /* Fastest speed                        */
    /*------------------------------------------------------------------------------------------*\
    | POLYCHROME_V1_MODE_WAVE                                                                    |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_SPEED_MIN_WAVE            = 0x06,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_WAVE        = 0x02,     /* Default speed                        */
    POLYCHROME_V1_SPEED_MAX_WAVE            = 0x01,     /* Fastest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_SPRING      = 0x04,     /* Default speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_METEOR      = 0x0A,     /* Default speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_STACK       = 0x04,     /* Default speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_CRAM        = 0x04,     /* Default speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_SCAN        = 0x0A,     /* Default speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_NEON        = 0x20,     /* Default speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_WATER       = 0x0A,     /* Default speed                        */
    /*------------------------------------------------------------------------------------------*\
    | POLYCHROME_V1_MODE_RAINBOW                                                                 |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_SPEED_MIN_RAINBOW         = 0x12,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_RAINBOW     = 0x0A,     /* Default speed                        */
    POLYCHROME_V1_SPEED_MAX_RAINBOW         = 0x01,     /* Fastest speed                        */
    POLYCHROME_V1_SPEED_MIN_ARGB            = 0x20,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_MAX_ARGB            = 0x02,     /* Fastest speed                        */
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
    POLYCHROME_V2_SPEED_MIN                 = 0x05,     /* Slowest speed                        */
    POLYCHROME_V2_SPEED_DEFAULT             = 0x03,     /* Default speed                        */
    POLYCHROME_V2_SPEED_MAX                 = 0x00,     /* Fastest speed                        */
};

class PolychromeController
{
public:
    PolychromeController(i2c_smbus_interface* bus, polychrome_dev_id dev);
    ~PolychromeController();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();
    std::string     GetFirmwareVersion();
    unsigned int    GetMode();
    unsigned int    GetASRockType();
    void            SetColorsAndSpeed(unsigned char led, unsigned char red, unsigned char green, unsigned char blue);
    void            SetMode(unsigned char zone, unsigned char mode, unsigned char speed);

    unsigned char           zone_led_count[6];
    
private:
    unsigned int            asrock_type;
    unsigned short          fw_version;
    std::string             device_name;
    unsigned char           active_zone;
    unsigned char           active_mode;
    unsigned char           active_speed;
    i2c_smbus_interface*    bus;
    polychrome_dev_id       dev;

    unsigned short  ReadFirmwareVersion();
    void            ReadLEDConfiguration();
};
