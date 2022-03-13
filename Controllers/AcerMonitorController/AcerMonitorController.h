/*---------------------------------------------------------*\
|  AcerMonitorController.h                                  |
|                                                           |
|  Driver for 2020/2021 Acer monitors with rear RGB strips. |
|                                                           |
|  Shenghao Yang (me@shenghaoyang.info), 30/12/2021         |
|  Amended for code style by Adam Honse, 3/13/2022          |
\*---------------------------------------------------------*/

#pragma once

#include <RGBController.h>
#include <hidapi/hidapi.h>

enum
{
    ACER_MONITOR_OFFSET_REPORT_TYPE     = 0x00,     /* Offset of report type field in packet    */
    ACER_MONITOR_OFFSET_MAGIC_0         = 0x01,     /* Offset of magic 0 field in packet        */
    ACER_MONITOR_OFFSET_MAGIC_1         = 0x02,     /* Offset of magic 1 field in packet        */
    ACER_MONITOR_OFFSET_PAD_0           = 0x03,     /* Offset of pad 0 field in packet          */
    ACER_MONITOR_OFFSET_NUM_LEDS        = 0x04,     /* Offset of number of LEDs field in packet */
    ACER_MONITOR_OFFSET_STRIP_INDEX     = 0x05,     /* Offset of strip index field in packet    */
    ACER_MONITOR_OFFSET_PAD_1           = 0x06,     /* Offset of pad 1 field in packet          */
    ACER_MONITOR_OFFSET_RGB_DATA        = 0x0E,     /* Offset of RGB data field in packet       */
};

enum
{
    ACER_MONITOR_REPORT_TYPE            = 0x2A,     /* Value of report type field in packet     */
    ACER_MONITOR_MAGIC_0                = 0x0C,     /* Value of magic 0 field in packet         */
    ACER_MONITOR_MAGIC_1                = 0x02,     /* Value of magic 1 field in packet         */
};

class AcerMonitorController
{
public:
    AcerMonitorController(hid_device* dev_handle, const char* path);
    ~AcerMonitorController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SetColors(RGBColor *colors, std::size_t num_leds);

private:
    hid_device*     dev;
    std::string     location;
};
