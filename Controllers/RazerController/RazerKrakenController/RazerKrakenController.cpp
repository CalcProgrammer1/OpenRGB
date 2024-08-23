/*---------------------------------------------------------*\
| RazerKrakenController.cpp                                 |
|                                                           |
|   Driver for Razer Kraken                                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                28 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "RazerKrakenController.h"
#include "RazerDevices.h"

using namespace std::chrono_literals;

RazerKrakenController::RazerKrakenController(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name)
{
    dev             = dev_handle;
    dev_pid         = pid;
    location        = path;
    name            = dev_name;
    device_index    = 0;

    /*-----------------------------------------------------------------*\
    | Loop through all known devices to look for a name match           |
    \*-----------------------------------------------------------------*/
    for (unsigned int i = 0; i < RAZER_NUM_DEVICES; i++)
    {
        if (device_list[i]->pid == dev_pid)
        {
            /*---------------------------------------------------------*\
            | Set device ID                                             |
            \*---------------------------------------------------------*/
            device_index = i;
        }
    }

    /*-----------------------------------------------------------------*\
    | Determine addresses for device                                    |
    \*-----------------------------------------------------------------*/
    switch(dev_pid)
    {
        case RAZER_KRAKEN_V2_PID:
        case RAZER_KRAKEN_KITTY_BLACK_EDITION_V2_PID:
        case RAZER_KRAKEN_ULTIMATE_PID:
            led_mode_address        = 0x172D;
            custom_address          = 0x1189;
            breathing_address[0]    = 0x1741;
            breathing_address[1]    = 0x1745;
            breathing_address[2]    = 0x174D;
            break;
        case RAZER_KRAKEN_CLASSIC_PID:
        case RAZER_KRAKEN_CLASSIC_ALT_PID:
        case RAZER_KRAKEN_PID:
            led_mode_address        = 0x1008;
            custom_address          = 0x1189;
            breathing_address[0]    = 0x15DE;
            breathing_address[1]    = 0x15DE;
            breathing_address[2]    = 0x15DE;
            break;
    }
}

RazerKrakenController::~RazerKrakenController()
{
    hid_close(dev);
}

std::string RazerKrakenController::GetName()
{
    return(name);
}

unsigned int RazerKrakenController::GetDeviceIndex()
{
    return(device_index);
}

device_type RazerKrakenController::GetDeviceType()
{
    return(device_list[device_index]->type);
}

std::string RazerKrakenController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RazerKrakenController::GetFirmwareString()
{
    return(razer_get_firmware());
}

std::string RazerKrakenController::GetSerialString()
{
    return(razer_get_serial());
}

void RazerKrakenController::SetModeBreathingOneColor(unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_set_mode_breathing_one_color(red, grn, blu);
}

void RazerKrakenController::SetModeBreathingTwoColors(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2)
{
    razer_set_mode_breathing_two_colors(r1, g1, b1, r2, g2, b2);
}

void RazerKrakenController::SetModeBreathingThreeColors(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2, unsigned char r3, unsigned char g3, unsigned char b3)
{
    razer_set_mode_breathing_three_colors(r1, g1, b1, r2, g2, b2, r3, g3, b3);
}

void RazerKrakenController::SetModeCustom(unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_set_mode_custom(red, grn, blu);
}

void RazerKrakenController::SetModeOff()
{
    razer_set_mode_none();
}

void RazerKrakenController::SetModeSpectrumCycle()
{
    razer_set_mode_spectrum_cycle();
}

void RazerKrakenController::SetModeStatic(unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_set_mode_static(red, grn, blu);
}

/*---------------------------------------------------------------------------------*\
| Basic report and response creation functions                                      |
\*---------------------------------------------------------------------------------*/

razer_kraken_request_report RazerKrakenController::razer_kraken_create_report(unsigned char report_id, unsigned char destination, unsigned char length, unsigned short address)
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

razer_kraken_effect_byte RazerKrakenController::razer_kraken_create_effect_byte()
{
    razer_kraken_effect_byte            effect_byte;

    memset(&effect_byte, 0, sizeof(razer_kraken_effect_byte));

    return effect_byte;
}

/*---------------------------------------------------------------------------------*\
| Get functions (request information from device)                                   |
\*---------------------------------------------------------------------------------*/

std::string RazerKrakenController::razer_get_firmware()
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

std::string RazerKrakenController::razer_get_serial()
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
| Set functions (send information to device)                                        |
\*---------------------------------------------------------------------------------*/

void RazerKrakenController::razer_set_mode_breathing_one_color(unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_kraken_request_report rgb_report      = razer_kraken_create_report(0x04, 0x40, 0x03, breathing_address[0]);
    razer_kraken_request_report effect_report   = razer_kraken_create_report(0x04, 0x40, 0x01, led_mode_address);
    razer_kraken_effect_byte    effect_byte     = razer_kraken_create_effect_byte();

    rgb_report.arguments[0]                     = red;
    rgb_report.arguments[1]                     = grn;
    rgb_report.arguments[2]                     = blu;

    effect_byte.bits.on_off_static              = 1;
    effect_byte.bits.single_colour_breathing    = 1;
    effect_byte.bits.sync                       = 1;
    effect_report.arguments[0]                  = effect_byte.value;

    razer_usb_send(&rgb_report);
    razer_usb_send(&effect_report);
}

void RazerKrakenController::razer_set_mode_breathing_two_colors(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2)
{
    razer_kraken_request_report rgb_report_1    = razer_kraken_create_report(0x04, 0x40, 0x03, breathing_address[1]);
    razer_kraken_request_report rgb_report_2    = razer_kraken_create_report(0x04, 0x40, 0x03, breathing_address[1] + 4);
    razer_kraken_request_report effect_report   = razer_kraken_create_report(0x04, 0x40, 0x01, led_mode_address);
    razer_kraken_effect_byte    effect_byte     = razer_kraken_create_effect_byte();

    rgb_report_1.arguments[0]                   = r1;
    rgb_report_1.arguments[1]                   = g1;
    rgb_report_1.arguments[2]                   = b1;

    rgb_report_2.arguments[0]                   = r2;
    rgb_report_2.arguments[1]                   = g2;
    rgb_report_2.arguments[2]                   = b2;

    effect_byte.bits.on_off_static              = 1;
    effect_byte.bits.two_colour_breathing       = 1;
    effect_byte.bits.sync                       = 1;
    effect_report.arguments[0]                  = effect_byte.value;

    razer_usb_send(&rgb_report_1);
    razer_usb_send(&rgb_report_2);
    razer_usb_send(&effect_report);
}

void RazerKrakenController::razer_set_mode_breathing_three_colors(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2, unsigned char r3, unsigned char g3, unsigned char b3)
{
    razer_kraken_request_report rgb_report_1    = razer_kraken_create_report(0x04, 0x40, 0x03, breathing_address[1]);
    razer_kraken_request_report rgb_report_2    = razer_kraken_create_report(0x04, 0x40, 0x03, breathing_address[1] + 4);
    razer_kraken_request_report rgb_report_3    = razer_kraken_create_report(0x04, 0x40, 0x03, breathing_address[1] + 8);
    razer_kraken_request_report effect_report   = razer_kraken_create_report(0x04, 0x40, 0x01, led_mode_address);
    razer_kraken_effect_byte    effect_byte     = razer_kraken_create_effect_byte();

    rgb_report_1.arguments[0]                   = r1;
    rgb_report_1.arguments[1]                   = g1;
    rgb_report_1.arguments[2]                   = b1;

    rgb_report_2.arguments[0]                   = r2;
    rgb_report_2.arguments[1]                   = g2;
    rgb_report_2.arguments[2]                   = b2;

    rgb_report_3.arguments[0]                   = r3;
    rgb_report_3.arguments[1]                   = g3;
    rgb_report_3.arguments[2]                   = b3;

    effect_byte.bits.on_off_static              = 1;
    effect_byte.bits.three_colour_breathing     = 1;
    effect_byte.bits.sync                       = 1;
    effect_report.arguments[0]                  = effect_byte.value;

    razer_usb_send(&rgb_report_1);
    razer_usb_send(&rgb_report_2);
    razer_usb_send(&rgb_report_3);
    razer_usb_send(&effect_report);
}

void RazerKrakenController::razer_set_mode_custom(unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_kraken_request_report rgb_report      = razer_kraken_create_report(0x04, 0x40, 3, custom_address);
    razer_kraken_request_report effect_report   = razer_kraken_create_report(0x04, 0x40, 0x01, led_mode_address);
    razer_kraken_effect_byte    effect_byte     = razer_kraken_create_effect_byte();

    effect_byte.value                       = 0;

    effect_byte.bits.on_off_static          = 1;
    effect_byte.bits.spectrum_cycling       = 0;

    rgb_report.arguments[0]                 = red;
    rgb_report.arguments[1]                 = grn;
    rgb_report.arguments[2]                 = blu;
    effect_report.arguments[0]              = effect_byte.value;

    switch(dev_pid)
    {
        case RAZER_KRAKEN_PID:
        case RAZER_KRAKEN_V2_PID:
        case RAZER_KRAKEN_KITTY_BLACK_EDITION_V2_PID:
        case RAZER_KRAKEN_ULTIMATE_PID:
            razer_usb_send(&rgb_report);
            break;
    }

    razer_usb_send(&effect_report);
}

void RazerKrakenController::razer_set_mode_none()
{
    razer_kraken_request_report report      = razer_kraken_create_report(0x04, 0x40, 0x01, led_mode_address);
    razer_kraken_effect_byte    effect_byte = razer_kraken_create_effect_byte();

    effect_byte.value                       = 0;

    effect_byte.bits.on_off_static          = 0;
    effect_byte.bits.spectrum_cycling       = 0;

    report.arguments[0]                     = effect_byte.value;

    razer_usb_send(&report);
}

void RazerKrakenController::razer_set_mode_spectrum_cycle()
{
    razer_kraken_request_report report      = razer_kraken_create_report(0x04, 0x40, 0x01, led_mode_address);
    razer_kraken_effect_byte    effect_byte = razer_kraken_create_effect_byte();

    effect_byte.value                       = 0;

    effect_byte.bits.on_off_static          = 1;
    effect_byte.bits.spectrum_cycling       = 1;

    report.arguments[0]                     = effect_byte.value;

    razer_usb_send(&report);
}

void RazerKrakenController::razer_set_mode_static(unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_kraken_request_report rgb_report      = razer_kraken_create_report(0x04, 0x40, 3, breathing_address[0]);
    razer_kraken_request_report effect_report   = razer_kraken_create_report(0x04, 0x40, 0x01, led_mode_address);
    razer_kraken_effect_byte    effect_byte     = razer_kraken_create_effect_byte();

    effect_byte.value                       = 0;

    effect_byte.bits.on_off_static          = 1;
    effect_byte.bits.spectrum_cycling       = 0;

    rgb_report.arguments[0]                 = red;
    rgb_report.arguments[1]                 = grn;
    rgb_report.arguments[2]                 = blu;
    effect_report.arguments[0]              = effect_byte.value;

    switch(dev_pid)
    {
        case RAZER_KRAKEN_PID:
        case RAZER_KRAKEN_V2_PID:
        case RAZER_KRAKEN_KITTY_BLACK_EDITION_V2_PID:
        case RAZER_KRAKEN_ULTIMATE_PID:
            razer_usb_send(&rgb_report);
            break;
    }

    razer_usb_send(&effect_report);
}

/*---------------------------------------------------------------------------------*\
| USB transfer functions                                                            |
\*---------------------------------------------------------------------------------*/

int RazerKrakenController::razer_usb_receive(razer_kraken_response_report* report)
{
    return hid_read(dev, (unsigned char*)report, sizeof(*report));
}

int RazerKrakenController::razer_usb_send(razer_kraken_request_report* report)
{
    return hid_write(dev, (unsigned char*)report, sizeof(*report));
}
