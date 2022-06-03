/*-------------------------------------------------------------------*\
|  LenovoUSBController.cpp                                            |
|                                                                     |
|  Driver for various Lenovo Devices                                  |
|                                                                     |
|  Cooper Hall (geobot19)          17 Apr 2022                        |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "LenovoUSBController.h"
#include "LogManager.h"

using namespace std;

LenovoUSBController::LenovoUSBController(hid_device* dev_handle, const char* path, uint16_t in_pid)
{
    const uint8_t   sz      = HID_MAX_STR;
    wchar_t         tmp[sz];

    dev                     = dev_handle;
    location                = path;
    pid                     = in_pid;

    hid_get_manufacturer_string(dev, tmp, sz);
    std::wstring w_tmp      = wstring(tmp);
    name                    = string(w_tmp.begin(), w_tmp.end());

    hid_get_product_string(dev, tmp, sz);
    w_tmp                   = wstring(tmp);
    name.append(" ").append(string(w_tmp.begin(), w_tmp.end()));

    setDeviceSoftwareMode();
}

LenovoUSBController::~LenovoUSBController()
{
    hid_close(dev);
}

uint16_t LenovoUSBController::getPid()
{
    return pid;
}

string LenovoUSBController::getName()
{
    return name;
}

string LenovoUSBController::getLocation()
{
    return location;
}

void LenovoUSBController::setZoneLeds(uint8_t zone_num, vector<pair<uint8_t, RGBColor>> &led_colors)
{
    for(size_t curr = 0; curr < led_colors.size();)
    {
        uint8_t buffer[LENOVO_HID_PACKET_SIZE] = {LENOVO_INSTRUCTION_START, LENOVO_ZONE_ID_0+zone_num, 0, 0};

        /*---------------------------------------------------------*\
        | Set the colour bytes in the packet                        |
        | buffer[2] is required to know the amount of leds in packet|
        | so it is set here as well                                 |
        \*---------------------------------------------------------*/
        for(; buffer[2] < LENOVO_MAX_LEDS_PER_PACKET && curr < led_colors.size(); ++buffer[2])
        {
            uint8_t offset     = (buffer[2] * 4) + 4;

            /*------------------*\
            |write the led number|
            \*------------------*/
            buffer[offset]     = led_colors[curr].first;
            /*--------------*\
            |write the colors|
            \*--------------*/
            buffer[offset + 1] = RGBGetRValue(led_colors[curr].second);
            buffer[offset + 2] = RGBGetGValue(led_colors[curr].second);
            buffer[offset + 3] = RGBGetBValue(led_colors[curr].second);

            ++curr;
        }

        hid_send_feature_report(dev, buffer, LENOVO_HID_PACKET_SIZE);
    }
}

void LenovoUSBController::setSingleLED(uint8_t zone_num, uint8_t led_num, RGBColor color)
{
    uint8_t buffer[LENOVO_HID_PACKET_SIZE] = {LENOVO_INSTRUCTION_START, LENOVO_ZONE_ID_0+zone_num, 1, 0, led_num, RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color)};
    hid_send_feature_report(dev, buffer, LENOVO_HID_PACKET_SIZE);
}

void LenovoUSBController::sendBasicInstruction(uint8_t instruction)
{
    uint8_t buffer[LENOVO_HID_PACKET_SIZE] = {LENOVO_INSTRUCTION_START, instruction};
    hid_send_feature_report(dev, buffer, LENOVO_HID_PACKET_SIZE);
}

void LenovoUSBController::setDeviceSoftwareMode()
{
    /*---------------------------------------*\
    | this is required for the device listen  |
    | to the software protocol                |
    \*---------------------------------------*/
    sendBasicInstruction(0xB2);
}

void LenovoUSBController::setDeviceHardwareMode()
{
    /*---------------------------------------*\
    |releases the device from sofware mode so |
    |that onboard controlls can be used       |
    |this has not been shown to happen between|
    |reboots                                  |
    \*---------------------------------------*/
    sendBasicInstruction(0xB1);
}