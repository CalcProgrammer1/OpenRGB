/*---------------------------------------------------------*\
| RazerKrakenV4Controller.cpp                               |
|                                                           |
|   Driver for Razer devices with 64-byte report            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "RazerKrakenV4Controller.h"
#include "RazerDevices.h"

using namespace std::chrono_literals;

RazerKrakenV4Controller::RazerKrakenV4Controller(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name)
{
    dev               = dev_handle;
    dev_pid           = pid;
    location          = path;
    name              = dev_name;

    /*-----------------------------------------------------*\
    | Loop through all known devices to look for a name     |
    | match                                                 |
    \*-----------------------------------------------------*/
    for(unsigned int i = 0; i < RAZER_NUM_DEVICES; i++)
    {
        if(device_list[i]->pid == dev_pid)
        {
            /*---------------------------------------------*\
            | Set device ID                                 |
            \*---------------------------------------------*/
            device_index = i;
        }
    }

    /*-----------------------------------------------------*\
    | Determine matrix type for device                      |
    \*-----------------------------------------------------*/
    matrix_type = device_list[device_index]->matrix_type;

    /*-----------------------------------------------------*\
    | All Kraken V4 devices use 0x02 for report and         |
    | response index                                        |
    \*-----------------------------------------------------*/
    report_index    = 0x02;
    response_index  = 0x02;

    /*-----------------------------------------------------*\
    | Determine transaction ID for device                   |
    \*-----------------------------------------------------*/
    dev_transaction_id = device_list[device_index]->transaction_id;

    /*-----------------------------------------------------*\
    | Determine wireless flag for device                    |
    \*-----------------------------------------------------*/
    switch(dev_pid)
    {
        case RAZER_KRAKEN_V4_WIRELESS_PID:
        case RAZER_KRAKEN_KITTY_V3_PRO_WIRELESS_PID:
            dev_wireless_flag = 0x80;
            break;

        default:
            dev_wireless_flag = 0x00;
            break;
    }
}

RazerKrakenV4Controller::~RazerKrakenV4Controller()
{
    hid_close(dev);
}

std::string RazerKrakenV4Controller::GetName()
{
    return(name);
}

unsigned int RazerKrakenV4Controller::GetDeviceIndex()
{
    return(device_index);
}

device_type RazerKrakenV4Controller::GetDeviceType()
{
    return(device_list[device_index]->type);
}

std::string RazerKrakenV4Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RazerKrakenV4Controller::GetFirmwareString()
{
    return(razer_get_firmware());
}

std::string RazerKrakenV4Controller::GetSerialString()
{
    return(razer_get_serial());
}

unsigned char RazerKrakenV4Controller::GetMaxBrightness()
{
    /*-----------------------------------------------------*\
    | Max brightness for most devices is 0xFF (255)         |
    |   Add PIDs only for devices that use 0x64 (100)       |
    |   or any another arbitrary value                      |
    \*-----------------------------------------------------*/
    unsigned char max_brightness = 255;

    return(max_brightness);
}

void RazerKrakenV4Controller::SetDirect(RGBColor* colors)
{
    struct razer_kraken_v4_report   report          = razer_kraken_v4_create_report(0x0F, 0x03, (5 + (3 * device_list[device_index]->cols)));

    report.arguments[2]             = 0;
    report.arguments[3]             = 0;
    report.arguments[4]             = device_list[device_index]->cols - 1;

    for(unsigned int led_idx = 0; led_idx < device_list[device_index]->cols; led_idx++)
    {
        report.arguments[5 + (led_idx * 3)] = RGBGetRValue(colors[led_idx]);
        report.arguments[6 + (led_idx * 3)] = RGBGetGValue(colors[led_idx]);
        report.arguments[7 + (led_idx * 3)] = RGBGetBValue(colors[led_idx]);
    }

    razer_usb_send(&report);
}

void RazerKrakenV4Controller::SetBrightness(unsigned char brightness)
{
    struct razer_kraken_v4_report   report          = razer_kraken_v4_create_report(0x00, 0x00, 0x05);

    report.arguments[0]             = RAZER_KRAKEN_V4_CMD_LIGHTING_SET_BRIGHTNESS;
    report.arguments[2]             = 0x01;
    report.arguments[3]             = brightness;

    razer_usb_send(&report);
}

void RazerKrakenV4Controller::SetModeWave()
{
    struct razer_kraken_v4_report   report          = razer_kraken_v4_create_report(0x00, 0x00, 0x05);

    report.arguments[0]             = RAZER_KRAKEN_V4_CMD_LIGHTING_SET_MODE;
    report.arguments[2]             = 0x01;
    report.arguments[3]             = 0x04;

    razer_usb_send(&report);
}

unsigned char RazerKrakenV4Controller::razer_kraken_v4_calculate_crc(razer_kraken_v4_report* report)
{
    /*-----------------------------------------------------*\
    | The second to last byte of report is a simple         |
    | checksum.  Just xor all bytes up with overflow and    |
    | you are done                                          |
    \*-----------------------------------------------------*/
    unsigned char   crc         = 0;
    unsigned char*  report_ptr  = (unsigned char*)report;

    for(unsigned int i = 0; i < 61; i++)
    {
        crc ^= report_ptr[i];
    }

    return crc;
}

razer_kraken_v4_report RazerKrakenV4Controller::razer_kraken_v4_create_report(unsigned char command_class, unsigned char command_id, unsigned char data_size)
{
    razer_kraken_v4_report new_report;

    /*-----------------------------------------------------*\
    | Zero out the new report                               |
    \*-----------------------------------------------------*/
    memset(&new_report, 0, sizeof(razer_kraken_v4_report));

    /*-----------------------------------------------------*\
    | Fill in the new report with the given parameters      |
    \*-----------------------------------------------------*/
    new_report.report_id            = report_index;
    new_report.status               = 0x00;
    new_report.transaction_id       = dev_transaction_id;
    new_report.remaining_packets    = 0x00;
    new_report.protocol_type        = 0x00;
    new_report.data_size            = data_size;
    new_report.command_class        = command_class;
    new_report.command_id           = command_id;
    new_report.wireless_flag        = dev_wireless_flag;

    return new_report;
}

razer_kraken_v4_report RazerKrakenV4Controller::razer_kraken_v4_create_response()
{
    razer_kraken_v4_report new_report;

    /*-----------------------------------------------------*\
    | Zero out the new report                               |
    \*-----------------------------------------------------*/
    memset(&new_report, 0, sizeof(razer_kraken_v4_report));

    /*-----------------------------------------------------*\
    | Fill in the new report with the given parameters      |
    \*-----------------------------------------------------*/
    new_report.report_id            = response_index;
    new_report.status               = 0x00;
    new_report.transaction_id       = dev_transaction_id;
    new_report.remaining_packets    = 0x00;
    new_report.protocol_type        = 0x00;
    new_report.command_class        = 0x00;
    new_report.command_id           = 0x00;
    new_report.data_size            = 0x00;
    new_report.wireless_flag        = dev_wireless_flag;

    return new_report;
}

std::string RazerKrakenV4Controller::razer_get_firmware()
{
    std::string                     firmware_string = "";
    struct razer_kraken_v4_report   report          = razer_kraken_v4_create_report(0x00, 0x00, 0x04);
    struct razer_kraken_v4_report   response_report = razer_kraken_v4_create_response();

    report.arguments[0]             = RAZER_KRAKEN_V4_CMD_GET_FIRMWARE_INFO;

    std::this_thread::sleep_for(2ms);
    razer_usb_send(&report);
    std::this_thread::sleep_for(5ms);
    razer_usb_receive(&response_report);

    firmware_string = "v" + std::to_string(response_report.arguments[3]) + "." + std::to_string(response_report.arguments[4]) + "." + std::to_string(response_report.arguments[5]) + "." + std::to_string(response_report.arguments[6]);

    return firmware_string;
}

std::string RazerKrakenV4Controller::razer_get_serial()
{
    char                            serial_string[16];
    struct razer_kraken_v4_report   report          = razer_kraken_v4_create_report(0x00, 0x00, 0x04);
    struct razer_kraken_v4_report   response_report = razer_kraken_v4_create_response();

    report.arguments[0]             = RAZER_KRAKEN_V4_CMD_GET_SERIAL;

    std::this_thread::sleep_for(2ms);
    razer_usb_send(&report);
    std::this_thread::sleep_for(5ms);
    razer_usb_receive(&response_report);

    memcpy(&serial_string[0], &response_report.arguments[3], 16);
    serial_string[15] = '\0';

    for(size_t i = 0; i < 15; i++)
    {
        if(serial_string[i] < 30 || serial_string[i] > 126)
        {
            serial_string[i] = ' ';
        }
    }

    std::string ret_string = serial_string;
    return ret_string;
}

int RazerKrakenV4Controller::razer_usb_receive(razer_kraken_v4_report* report)
{
    report->crc = razer_kraken_v4_calculate_crc(report);

    return hid_read(dev, (unsigned char *)report, sizeof(*report));
}

int RazerKrakenV4Controller::razer_usb_send(razer_kraken_v4_report* report)
{
    return hid_write(dev, (unsigned char *)report, sizeof(*report));
}
