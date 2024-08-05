/*---------------------------------------------------------*\
| LogitechProtocolCommon.h                                  |
|                                                           |
|   Common functionality for Logitech RAP and FAP protocols |
|                                                           |
|   Chris M (Dr_No)                             04 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <map>
#include <vector>
#include <hidapi.h>
#include "LogManager.h"

#define LOGITECH_PROTOCOL_TIMEOUT                       300     //Timeout in ms
#define LOGITECH_HEADER_SIZE                            3
#define LOGITECH_SHORT_MESSAGE                          0x10
#define LOGITECH_SHORT_MESSAGE_LEN                      7
#define LOGITECH_LONG_MESSAGE                           0x11
#define LOGITECH_LONG_MESSAGE_LEN                       20
#define LOGITECH_FAP_RESPONSE_LEN                       64      //Define a universal response buffer and allow the hidapi to determine the size

#define LOGITECH_DEFAULT_DEVICE_INDEX                   0xFF
#define LOGITECH_RECEIVER_DEVICE_INDEX                  0xFF    //The Unifying receiver uses RAP or register access protocol
#define LOGITECH_SET_REGISTER_REQUEST                   0x80
#define LOGITECH_GET_REGISTER_REQUEST                   0x81

#define LOGITECH_HIDPP_PAGE_ROOT_IDX                    0x00    //Used for querying the feature index
#define LOGITECH_CMD_ROOT_GET_FEATURE                   0x01
#define LOGITECH_CMD_ROOT_GET_PROTOCOL                  0x11

#define LOGITECH_HIDPP_PAGE_FEATURE_SET                 0x0001
#define LOGITECH_CMD_FEATURE_SET_GET_COUNT              0x01
#define LOGITECH_CMD_FEATURE_SET_GET_ID                 0x11

#define LOGITECH_HIDPP_PAGE_DEVICE_NAME_TYPE            0x0005
#define LOTITECH_CMD_DEVICE_NAME_TYPE_GET_COUNT         0x01
#define LOGITECH_CMD_DEVICE_NAME_TYPE_GET_DEVICE_NAME	0x11
#define LOGITECH_CMD_DEVICE_NAME_TYPE_GET_TYPE          0x21

#define LOGITECH_HIDPP_PAGE_RGB_EFFECTS1                0x8070
#define LOGITECH_HIDPP_PAGE_RGB_EFFECTS2                0x8071
#define LOGITECH_CMD_RGB_EFFECTS_GET_COUNT              0x00
#define LOGITECH_CMD_RGB_EFFECTS_GET_INFO               0x10
#define LOGITECH_CMD_RGB_EFFECTS_GET_CONTROL            0x20
#define LOGITECH_CMD_RGB_EFFECTS_SET_CONTROL            0x30
#define LOGITECH_CMD_RGB_EFFECTS_GET_STATE              0x40
#define LOGITECH_CMD_RGB_EFFECTS_SET_STATE              0x50
#define LOGITECH_CMD_RGB_EFFECTS_GET_CONFIG             0x60
#define LOGITECH_CMD_RGB_EFFECTS_SET_CONFIG             0x70
#define LOGITECH_CMD_RGB_EFFECTS_UNKNOWN                0x80    //Used to set "direct" mode

enum LOGITECH_DEVICE_TYPE
{
    LOGITECH_DEVICE_TYPE_KEYBOARD       = 0,
    LOGITECH_DEVICE_TYPE_REMOTECONTROL  = 1,
    LOGITECH_DEVICE_TYPE_NUMPAD         = 2,
    LOGITECH_DEVICE_TYPE_MOUSE          = 3,
    LOGITECH_DEVICE_TYPE_MOUSEPAD       = 4,
    LOGITECH_DEVICE_TYPE_TRACKBALL      = 5,
    LOGITECH_DEVICE_TYPE_PRESENTER      = 6,
    LOGITECH_DEVICE_TYPE_RECEIVER       = 7,
    LOGITECH_DEVICE_TYPE_HEADSET        = 8
};

enum LOGITECH_DEVICE_MODE
{
    LOGITECH_DEVICE_LED_OFF             = 0x0000,
    LOGITECH_DEVICE_LED_ON              = 0x0001,
    LOGITECH_DEVICE_LED_SPECTRUM        = 0x0003,
    LOGITECH_DEVICE_LED_WAVE            = 0x0004,
    LOGITECH_DEVICE_LED_STAR            = 0x0005,
    LOGITECH_DEVICE_LED_BREATHING       = 0x000A,
    LOGITECH_DEVICE_LED_RIPPLE          = 0x000B,
    LOGITECH_DEVICE_LED_CUSTOM          = 0x000C
};

extern const char* logitech_led_locations[];
extern const int NUM_LOGITECH_LED_LOCATIONS;

// Used for: {GET,SET}_REGISTER_{REQ,RSP}, SET_LONG_REGISTER_RSP, GET_LONG_REGISTER_REQ
struct message_short
{
        unsigned char               address;
        unsigned char               data[3];
};

// Used for: SET_LONG_REGISTER_REQ, GET_LONG_REGISTER_RSP
struct message_long
{
        unsigned char               address;
        unsigned char               data[16];
};

// Used for: ERROR_MSG
struct message_error
{
        unsigned char               sub_id;
        unsigned char               address;
        unsigned char               error_code;
        unsigned char               padding; /* set to 0 */
};

union shortFAPrequest
{
    uint8_t                         buffer[LOGITECH_SHORT_MESSAGE_LEN];
    struct
    {
        uint8_t                     report_id;
        uint8_t                     device_index;
        uint8_t                     feature_index;
        uint8_t                     feature_command;
        uint8_t                     data[LOGITECH_SHORT_MESSAGE_LEN - 4];
    };

    void init(uint8_t device_index, uint8_t feature_index)
    {
        this->report_id         = LOGITECH_SHORT_MESSAGE;
        this->device_index      = device_index;
        this->feature_index     = feature_index;
        this->feature_command   = 0;
        for(size_t i = 0; i < sizeof(data); i++)
        {
            this->data[i]   = 0;
        }
    };

    int size()
    {
        return LOGITECH_SHORT_MESSAGE_LEN;
    };
};

union longFAPrequest
{
    uint8_t                         buffer[LOGITECH_LONG_MESSAGE_LEN];
    struct
    {
        uint8_t                     report_id;
        uint8_t                     device_index;
        uint8_t                     feature_index;
        uint8_t                     feature_command;
        uint8_t                     data[LOGITECH_LONG_MESSAGE_LEN - 4];
    };

    void init(uint8_t device_index, uint8_t feature_index, uint8_t feature_command)
    {
        this->report_id         = LOGITECH_LONG_MESSAGE;
        this->device_index      = device_index;
        this->feature_index     = feature_index;
        this->feature_command   = feature_command;
        for(size_t i = 0; i < sizeof(data); i++)
        {
            this->data[i]   = 0;
        }
    };

    int size()
    {
        return LOGITECH_LONG_MESSAGE_LEN;
    };
};

union blankFAPmessage
{
    uint8_t                         buffer[LOGITECH_FAP_RESPONSE_LEN];
    struct
    {
        uint8_t                     report_id;
        uint8_t                     device_index;
        uint8_t                     feature_index;
        uint8_t                     feature_command;
        uint8_t                     data[LOGITECH_FAP_RESPONSE_LEN - 4];
    };

    //blank this buffer entirely
    void init()
    {
        for(size_t i = 0; i < sizeof(buffer); i++)
        {
            this->buffer[i]   = 0;
        }
    };

    int size()
    {
        return LOGITECH_FAP_RESPONSE_LEN;
    };
};

template<typename K, typename V>
static std::map<V, K> reverse_map(const std::map<K, V>& map)
{
    std::map<V, K> reversed_map;

    for(const std::pair<const K, V>& entry : map)
    {
        reversed_map[entry.second] = entry.first;
    }

    return reversed_map;
}

struct logitech_fx
{
        uint8_t                         index;
        uint16_t                        speed; //period
        LOGITECH_DEVICE_MODE            mode;
};

typedef std::map<uint8_t, hid_device*>  usages;
typedef std::map<uint16_t, uint8_t>     features;
typedef std::map<uint8_t, uint16_t>     rvrse_features;
typedef std::map<uint16_t, uint8_t>     wireless_map;

typedef std::vector<logitech_fx>        leds_fx;

struct logitech_led
{
        uint16_t                        location;
        uint8_t                         fx_count;
        leds_fx                         fx;
};

int getWirelessDevice(usages _usages, uint16_t pid, wireless_map *wireless_devices);    //Helper function needed outside of class

class logitech_device
{
public:
    logitech_device(char *path, usages _usages, uint8_t _device_index, bool _wireless);
    logitech_device(char *path, usages _usages, uint8_t _device_index, bool _wireless, std::shared_ptr<std::mutex> mutex_ptr);

    ~logitech_device();

    /*-----------------------------------------------------------------*\
    | usages is a std::map that stores all the devices HID usages       |
    |   This is to ensure that we can communicate to all windows usages |
    \*-----------------------------------------------------------------*/
    usages                      device_usages;
    features                    feature_list;
    uint8_t                     device_index;
    uint8_t                     RGB_feature_index;      //Stored for quick use
    uint8_t                     logitech_device_type;
    bool                        wireless;
    std::string                 device_name;
    std::string                 location;
    std::string                 protocol_version;

    bool                        connected();
    bool                        is_valid();
    void                        flushReadQueue();
    uint8_t                     getFeatureIndex(uint16_t feature_page);
    uint8_t                     getLED_count();
    logitech_led                getLED_info(uint8_t LED_num);
    uint8_t                     setDirectMode(bool direct);
    uint8_t                     setMode(uint8_t mode, uint16_t speed, uint8_t zone, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);
    int                         getDeviceName();
private:
    std::map<uint8_t, logitech_led> leds;
    std::shared_ptr<std::mutex> mutex;

    hid_device*                 getDevice(uint8_t usage_index);
    uint16_t                    getFeaturePage(uint8_t feature_index);
    int                         getDeviceFeatureList();
    void                        getRGBconfig();
    void                        initialiseDevice();
};
