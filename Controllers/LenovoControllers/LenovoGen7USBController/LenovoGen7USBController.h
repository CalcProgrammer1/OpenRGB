/*-------------------------------------------------------------------*\
|  LenovoGen7USBController.h                                          |
|                                                                     |
|  Driver for Lenovo Legion 7 Gen 7                                   |
|                                                                     |
|  Peter Vazny                     15 Nov 2022                        |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController.h"
#include "LogManager.h"

#include <string>
#include <array>
#include <vector>
#include <utility>
#include <hidapi/hidapi.h>

#ifndef HID_MAX_STR
#define HID_MAX_STR                255
#endif

#ifndef LENOVOGEN7USBCONTROLLER_H
#define LENOVOGEN7USBCONTROLLER_H

#define PACKET_SIZE                960
#define REPORT_ID                 0x07
#define DIRECT_MODE               0xA1
#define SWITCH_PROFILE            0xC8
#define GET_ACTIVE_PROFILE        0xCA
#define SAVE_PROFILE              0xCB
#define GET_PROFILE               0xCC
#define GET_BRIGHTNESS            0xCD
#define SET_BRIGHTNESS            0xCE
#define SET_DIRECT_MODE           0xD0
#define GET_DIRECT_MODE_PROFILE   0xD1

struct led_group
{
    uint8_t               mode;
    uint8_t               speed;
    uint8_t               spin;
    uint8_t               direction;
    uint8_t               color_mode;
    std::vector<RGBColor> colors;
    std::vector<uint16_t> leds;
};

class LenovoGen7USBController
{

    public:
    /*--------------*\
    |ctor(s) and dtor|
    \*--------------*/
    LenovoGen7USBController(hid_device* dev_handle, const char* path, uint16_t in_pid);
    LenovoGen7USBController();
    ~LenovoGen7USBController();

    /*--------------*\
    |device functions|
    \*--------------*/
    void setLedsByGroup(uint8_t profile_id, std::vector<led_group> led_groups);
    void setLedsDirect(std::vector<led> &leds, std::vector<RGBColor> &colors);
    void setLedsAllOff(uint8_t profile_id);
    void setLedsDirectOn(uint8_t profile_id);
    void setLedsDirectOff(uint8_t profile_id);
    uint16_t getPid();
    std::string getName();
    std::string getLocation();
    uint8_t getCurrentProfileId();
    uint8_t getCurrentBrightness();
    void setBrightness(uint8_t brightness);
    uint8_t getKeyboardId();
    void switchProfileTo(uint8_t profile_id);
    std::vector<led_group> getProfileSettings(uint8_t profile_id);


    private:
    /*--------------*\
    |data members    |
    \*--------------*/
    std::string name;
    hid_device *dev;
    std::string location;
    uint16_t pid;

    /*--------------*\
    |device functions|
    \*--------------*/
    void sendFeatureReport(uint8_t packet[], size_t packet_size);
    std::vector<uint8_t> getFeatureReport(uint8_t packet[], size_t packet_size);
    std::string ConvertBytesToHex(uint8_t packet[], size_t packet_size);
    std::string ConvertBytesToHex(const std::vector<uint8_t> &input);
};

#endif // LENOVOGEN7USBCONTROLLER_H
