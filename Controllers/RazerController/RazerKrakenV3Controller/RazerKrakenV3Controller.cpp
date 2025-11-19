/*---------------------------------------------------------*\
| RazerKrakenV3Controller.cpp                               |
|                                                           |
|   Driver for Razer devices with 13-byte reports           |
|                                                           |
|   Greg Sandstrom (superstrom)                  1 Nov 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "RazerKrakenController.h"
#include "RazerKrakenV3Controller.h"
#include "RazerDevices.h"

using namespace std::chrono_literals;

RazerKrakenV3Controller::RazerKrakenV3Controller(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name)
{
    dev               = dev_handle;
    dev_pid           = pid;
    location          = path;
    name              = dev_name;

    /*-----------------------------------------------------------------*\
    | Loop through all known devices to look for a name match           |
    \*-----------------------------------------------------------------*/
    for(unsigned int i = 0; i < RAZER_NUM_DEVICES; i++)
    {
        if(device_list[i]->pid == dev_pid)
        {
            /*---------------------------------------------------------*\
            | Set device ID                                             |
            \*---------------------------------------------------------*/
            device_index = i;
        }
    }

    /*-----------------------------------------------------------------*\
    | Determine matrix type for device                                  |
    \*-----------------------------------------------------------------*/
    matrix_type = device_list[device_index]->matrix_type;
}

RazerKrakenV3Controller::~RazerKrakenV3Controller()
{
    hid_close(dev);
}

std::string RazerKrakenV3Controller::GetName()
{
    return(name);
}

unsigned int RazerKrakenV3Controller::GetDeviceIndex()
{
    return(device_index);
}

device_type RazerKrakenV3Controller::GetDeviceType()
{
    return(device_list[device_index]->type);
}

std::string RazerKrakenV3Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RazerKrakenV3Controller::GetFirmwareString()
{
    return(razer_get_firmware());
}

std::string RazerKrakenV3Controller::GetSerialString()
{
    return(razer_get_serial());
}

unsigned char RazerKrakenV3Controller::GetMaxBrightness()
{
    /*-----------------------------------------------------*\
    | Max brightness for most devices is 0xFF (255)         |
    |   Add PIDs only for devices that use 0x64 (100)       |
    |   or any another arbitrary value                      |
    \*-----------------------------------------------------*/
    unsigned char max_brightness = 255;

    return(max_brightness);
}

void RazerKrakenV3Controller::SetModeDirect()
{
    razer_kraken_v3_request_report report = razer_kraken_create_v3_report();

    //  0 1 2 3 4 5 6 7 8
    // 4001000f0800000000
    // 40010000080000000000000000
    report.report_id = RAZER_KRAKEN_V3_REPORT_ID;
    report.command_id = RAZER_KRAKEN_V3_CMD_LIGHTING_SET_MODE;
    report.arguments[1] = 0x0F;
    report.arguments[2] = RAZER_KRAKEN_V3_MODE_ID_DIRECT;

    hid_write(dev, (unsigned char*)&report, sizeof(report));
}

void RazerKrakenV3Controller::SetDirect(RGBColor* colors)
{
    razer_kraken_v3_request_report report = razer_kraken_create_v3_report();

    // how to get the number of leds?
    unsigned int led_count = device_list[device_index]->cols;

    //  0 1 2 3 4 5 6 7 8
    // 400300ffffff000000
    // 400300ffffff00000000000000
    report.report_id = RAZER_KRAKEN_V3_REPORT_ID;
    report.command_id = RAZER_KRAKEN_V3_CMD_LIGHTING_SET_COLOR;

    for(unsigned int led_idx = 0; led_idx < led_count; led_idx++)
    {
        report.arguments[1 + (led_idx * 3)] = RGBGetRValue(colors[led_idx]);
        report.arguments[2 + (led_idx * 3)] = RGBGetGValue(colors[led_idx]);
        report.arguments[3 + (led_idx * 3)] = RGBGetBValue(colors[led_idx]);
    }

    hid_write(dev, (unsigned char*)&report, sizeof(report));
}

void RazerKrakenV3Controller::SetBrightness(unsigned char brightness)
{
    razer_kraken_v3_request_report report = razer_kraken_create_v3_report();

    //  0 1 2 3 4 5 6 7 8
    // 40020000ff00000000
    report.report_id = RAZER_KRAKEN_V3_REPORT_ID;
    report.command_id = RAZER_KRAKEN_V3_CMD_LIGHTING_SET_BRIGHTNESS;

    report.arguments[2] = brightness;

    hid_write(dev, (unsigned char*)&report, sizeof(report));
}

void RazerKrakenV3Controller::SetModeWave()
{
    razer_kraken_v3_request_report report = razer_kraken_create_v3_report();

    //  0 1 2 3 4 5 6 7 8
    // 4001000f0300000000
    // 40010100030000000000000000
    report.report_id = RAZER_KRAKEN_V3_REPORT_ID;
    report.command_id = RAZER_KRAKEN_V3_CMD_LIGHTING_SET_MODE;
    report.arguments[1] = 0x0F;
    report.arguments[2] = RAZER_KRAKEN_V3_MODE_ID_WAVE;

    hid_write(dev, (unsigned char*)&report, sizeof(report));
}

void RazerKrakenV3Controller::SetModeBreathing(std::vector<RGBColor> colors)
{
    razer_kraken_v3_request_report report = razer_kraken_create_v3_report();

    unsigned char led_count = colors.size();

    //  0 1 2 3 4 5 6 7 8 9 0 1 2
    // 400101000101ff000000000000
    // 40010100010100ffff00000000
    // 40010100020200ff00ff000000
    report.report_id = RAZER_KRAKEN_V3_REPORT_ID;
    report.command_id = RAZER_KRAKEN_V3_CMD_LIGHTING_SET_MODE;
    report.arguments[0] = 0x01;
    report.arguments[2] = led_count; // normally this is where the MODE_ID goes....
    report.arguments[3] = led_count;
    for(unsigned int led_idx = 0; led_idx < led_count; led_idx++)
    {
        report.arguments[4 + (led_idx * 3)] = RGBGetRValue(colors[led_idx]);
        report.arguments[5 + (led_idx * 3)] = RGBGetGValue(colors[led_idx]);
        report.arguments[6 + (led_idx * 3)] = RGBGetBValue(colors[led_idx]);
    }

    hid_write(dev, (unsigned char*)&report, sizeof(report));
}

razer_kraken_v3_request_report RazerKrakenV3Controller::razer_kraken_create_v3_report()
{
    razer_kraken_v3_request_report new_report;

    /*---------------------------------------------------------*\
    | Zero out the new report                                   |
    \*---------------------------------------------------------*/
    memset(&new_report, 0, sizeof(razer_kraken_v3_request_report));

    new_report.report_id = RAZER_KRAKEN_V3_REPORT_ID;

    return new_report;
}

razer_kraken_request_report RazerKrakenV3Controller::razer_kraken_create_report(unsigned char report_id, unsigned char destination, unsigned char length, unsigned short address)
{
    razer_kraken_request_report new_report;

    /*---------------------------------------------------------*\
    | Zero out the new report                                   |
    \*---------------------------------------------------------*/
    memset(&new_report, 0, sizeof(razer_kraken_request_report));

    /*---------------------------------------------------------*\
    | Fill in the new report with the given parameters          |
    \*---------------------------------------------------------*/
    new_report.report_id    = report_id;
    new_report.destination  = destination;
    new_report.length       = length;
    new_report.addr_h       = (address >> 8);
    new_report.addr_l       = (address & 0xFF);

    return new_report;
}

std::string RazerKrakenV3Controller::razer_get_firmware()
{
    std::string                         firmware_string         = "";
    struct razer_kraken_request_report  report                  = razer_kraken_create_report(0x04, 0x20, 0x02, 0x0030);
    struct razer_kraken_response_report response_report;

    std::this_thread::sleep_for(1ms);
    razer_usb_send(&report);
    std::this_thread::sleep_for(1ms);
    razer_usb_receive(&response_report);

    if(response_report.report_id == 0x05)
    {
        firmware_string = "v" + std::to_string(response_report.arguments[1]) + "." + std::to_string(response_report.arguments[2]);
    }

    return firmware_string;
}

std::string RazerKrakenV3Controller::razer_get_serial()
{
    char                                serial_string[64]   = "";
    struct razer_kraken_request_report  report              = razer_kraken_create_report(0x04, 0x20, 0x16, 0x7f00);
    struct razer_kraken_response_report response_report;

    std::this_thread::sleep_for(1ms);
    razer_usb_send(&report);
    std::this_thread::sleep_for(1ms);
    razer_usb_receive(&response_report);

    if(response_report.report_id == 0x05)
    {
        strncpy(&serial_string[0], (const char*)&response_report.arguments[0], 22);
        serial_string[22] = '\0';
    }

    for(size_t i = 0; i < 22; i++)
    {
        if(serial_string[i] < 30 || serial_string[i] > 126)
        {
            serial_string[i] = ' ';
        }
    }

    std::string ret_string = serial_string;
    return ret_string;
}

/*---------------------------------------------------------------------------------*\
| USB transfer functions                                                            |
\*---------------------------------------------------------------------------------*/

int RazerKrakenV3Controller::razer_usb_receive(razer_kraken_response_report* report)
{
    return hid_read(dev, (unsigned char*)report, sizeof(*report));
}

int RazerKrakenV3Controller::razer_usb_send(razer_kraken_request_report* report)
{
    return hid_write(dev, (unsigned char*)report, sizeof(*report));
}
