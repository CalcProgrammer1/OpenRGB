/*-------------------------------------------------------------------*\
|  LenovoUSBController.h                                              |
|                                                                     |
|  Driver for various Lenovo Devices                                  |
|                                                                     |
|  Cooper Hall (geobot19)          17 Apr 2022                        |
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

#ifndef LENOVOUSBCONTROLLER_H
#define LENOVOUSBCONTROLLER_H

#define LENOVO_INSTRUCTION_START   0x07
#define LENOVO_ZONE_ID_0           0xA0

#define LENOVO_HID_PACKET_SIZE     192
#define LENOVO_MAX_LEDS_PER_PACKET 0x2F

class LenovoUSBController
{
    public:
    /*--------------*\
    |ctor(s) and dtor|
    \*--------------*/
    LenovoUSBController(hid_device* dev_handle, const char* path, uint16_t in_pid);
    ~LenovoUSBController();

    /*--------------*\
    |device functions|
    \*--------------*/
    void setZoneLeds(uint8_t zone_num, std::vector<std::pair<uint8_t, RGBColor>> &led_colors);
    void setSingleLED(uint8_t zone_num, uint8_t led_num, RGBColor color);
    uint16_t getPid();
    std::string getName();
    std::string getLocation();
    std::vector<uint8_t> getInformation(uint8_t information_id);
    void setDeviceSoftwareMode();
    void setDeviceHardwareMode();

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
    void sendBasicInstruction(uint8_t instruction);
};

#endif
