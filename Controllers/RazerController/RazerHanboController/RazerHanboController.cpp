/*---------------------------------------------------------*\
| RazerHanboController.cpp                                  |
|                                                           |
|   Driver for Razer Hanbo devices                          |
|                                                           |
|   Joseph East (dripsnek)                      12 Apr 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "RazerHanboController.h"
#include "RazerDevices.h"

using namespace std::chrono_literals;

RazerHanboController::RazerHanboController(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name)
{
    dev             = dev_handle;
    dev_pid         = pid;
    location        = path;
    name            = dev_name;
    device_index    = 0;

    for(unsigned int i = 0; i < RAZER_NUM_DEVICES; i++)
    {
        if(device_list[i]->pid == dev_pid)
        {
            device_index = i;
        }
    }

    GetFirmware();
    SetDirectMode();
}

RazerHanboController::~RazerHanboController()
{
    hid_close(dev);
}

unsigned int RazerHanboController::GetDeviceIndex()
{
    return(device_index);
}

device_type RazerHanboController::GetDeviceType()
{
    return(device_list[device_index]->type);
}

std::string RazerHanboController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RazerHanboController::GetFirmwareString()
{
    return firmware_version;
}

std::string RazerHanboController::GetSerialString()
{
    return serial_string;
}

std::string RazerHanboController::GetName()
{
    return(name);
}

void RazerHanboController::SetDirectMode()
{
    razer_hanbo_report request_report  = razer_hanbo_create_report(0x82);
    razer_hanbo_report response_report = razer_hanbo_create_report(0x00);

    /*---------------------------------------*\
    | Take the one request and transform as   |
    | appropriate for the sequence.           |
    \*---------------------------------------*/
    request_report.arguments[0] = 0x01;
    UsbSend(&request_report);
    UsbReceive(&response_report);

    request_report.header[1]    = 0x80;
    request_report.arguments[0] = 0x01;
    request_report.arguments[1] = 0x00;
    request_report.arguments[2] = 0x01;
    UsbSend(&request_report);
    UsbReceive(&response_report);

    request_report.header[1]    = 0x80;
    request_report.arguments[0] = 0x01;
    request_report.arguments[1] = 0x01;
    request_report.arguments[2] = 0x01;
    UsbSend(&request_report);
    UsbReceive(&response_report);

    memset(&request_report, 0, sizeof(razer_hanbo_report));
    request_report.header[1]    = 0x82;
    request_report.arguments[0] = 0x01;
    UsbSend(&request_report);
    UsbReceive(&response_report);
}

void RazerHanboController::SetModeOff()
{
    SetDirectMode();
}

void RazerHanboController::SetModeSpectrumCycle()
{
    razer_hanbo_report request_report  = razer_hanbo_create_report(0x82);
    razer_hanbo_report response_report = razer_hanbo_create_report(0x00);

    request_report.arguments[0] = 0x01;
    UsbSend(&request_report);
    UsbReceive(&response_report);

    request_report.header[1]    = 0x80;
    request_report.arguments[0] = 0x01;
    UsbSend(&request_report);
    UsbReceive(&response_report);

    request_report.header[1]    = 0x80;
    request_report.arguments[0] = 0x01;
    request_report.arguments[1] = 0x01;
    UsbSend(&request_report);
    UsbReceive(&response_report);
}

void RazerHanboController::SetZoneLeds(int zone_idx, const zone& input_zone)
{
    razer_hanbo_report request_report = razer_hanbo_create_report(0x32);
    std::string payload;

    unsigned int j = 0;

    if(zone_idx > PUMP)
        request_report.header[1] = 0x40;

    payload = "0107000000000" + std::to_string(zone_idx);

    for(unsigned int i = 0; i < payload.length(); i += 2)
    {
        std::string byteString      = payload.substr(i, 2);
        char byte                   = (char)strtol(byteString.c_str(), NULL, 16);
        request_report.arguments[j] = byte;
        j++;
    }

    /*--------------------------------------------*\
    | The color command format is G/R/B            |
    \*--------------------------------------------*/

    for(unsigned int i = 0; i < input_zone.leds_count; i++)
    {
        request_report.arguments[j] = RGBGetGValue(input_zone.colors[i]);
        j++;
        request_report.arguments[j] = RGBGetRValue(input_zone.colors[i]);
        j++;
        request_report.arguments[j] = RGBGetBValue(input_zone.colors[i]);
        j++;
    }

    /*--------------------------------------------*\
    | Writing RGB values does not generate ack     |
    | reports from the cooler. Add a gap between   |
    | transactions to not overwhelm it.            |
    \*--------------------------------------------*/
    UsbSend(&request_report);
    std::this_thread::sleep_for(2ms);
}

/*--------------------------------------------*\
| The Hanbo allows for individual brightness   |
| of the pump cap and fans. OpenRGB only has   |
| a single brightness slider. Whilst this      |
| function can support individual settings,    |
| most of the time they will be invoked with   |
| identical values                             |
\*--------------------------------------------*/
void RazerHanboController::SetBrightness(int zone, unsigned int brightness)
{
    razer_hanbo_report request_report  = razer_hanbo_create_report(0x70);
    razer_hanbo_report response_report = razer_hanbo_create_report(0x00);

    request_report.arguments[0] = 0x01;
    request_report.arguments[1] = 0x00;

    if(zone > PUMP)
        request_report.arguments[1] = 0x01;

    request_report.arguments[2] = brightness & 0xFF;
    UsbSend(&request_report);
    UsbReceive(&response_report);
}

/*---------------------------------------------------------------------------------*\
| Basic report and response creation function                                       |
\*---------------------------------------------------------------------------------*/

razer_hanbo_report RazerHanboController::razer_hanbo_create_report(unsigned char header)
{
    /*---------------------------------------------------------*\
    | One type supports both requests and responses.            |
    | Requests start at header[1] to provide a dummy byte.      |
    | Responses provide 0 to this function for consistency      |
    \*---------------------------------------------------------*/
    razer_hanbo_report new_report;

    memset(&new_report, 0, sizeof(razer_hanbo_report));
    new_report.header[1] = header;
    return new_report;
}

/*---------------------------------------------------------------------------------*\
| Get functions (request information from device)                                   |
\*---------------------------------------------------------------------------------*/

void RazerHanboController::GetFirmware()
{
    razer_hanbo_report request_report  = razer_hanbo_create_report(0x01);
    razer_hanbo_report response_report = razer_hanbo_create_report(0x00);

    request_report.arguments[0] = 0x01;
    UsbSend(&request_report);
    /*---------------------------------------*\
    | The Hanbo sends firmware reports twice  |
    \*---------------------------------------*/
    UsbReceive(&response_report);
    UsbReceive(&response_report);

    if(response_report.header[0] == 0x02)
    {
        std::string ret_serial(response_report.arguments, response_report.arguments+15);
        std::vector<unsigned char> firmware_ret(response_report.arguments+27, response_report.arguments+29);
        char major = firmware_ret[0];
        char minor = firmware_ret[1] >> 4 & 0x0F;
        char patch = firmware_ret[1] & 0x0F;
        char ver[12];

        snprintf(ver, sizeof(ver), "%hhu.%hhu.%hhu", major, minor, patch);
        serial_string    = ret_serial;
        firmware_version = std::string(ver);
    }
}

/*---------------------------------------------------------------------------------*\
| USB transfer functions                                                            |
\*---------------------------------------------------------------------------------*/

int RazerHanboController::UsbReceive(razer_hanbo_report* report)
{
    return hid_read_timeout(dev, (unsigned char*)report, sizeof(*report),2);
}

int RazerHanboController::UsbSend(razer_hanbo_report* report)
{
    return hid_write(dev, (unsigned char*)report, sizeof(*report));
}
