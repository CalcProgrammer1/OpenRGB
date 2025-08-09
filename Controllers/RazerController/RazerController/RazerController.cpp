/*---------------------------------------------------------*\
| RazerController.cpp                                       |
|                                                           |
|   Driver for Razer devices                                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                22 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "RazerController.h"
#include "RazerDevices.h"
#include "LogManager.h"
#include "RazerDeviceGuard.h"

using namespace std::chrono_literals;

RazerController::RazerController(hid_device* dev_handle, hid_device* dev_argb_handle, const char* path, unsigned short pid, std::string dev_name)
{
    dev               = dev_handle;
    dev_argb          = dev_argb_handle;
    dev_pid           = pid;
    location          = path;
    name              = dev_name;
    device_index      = 0;
    guard_manager_ptr = new DeviceGuardManager(new RazerDeviceGuard());

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
    | Set report index                                                  |
    \*-----------------------------------------------------------------*/
    switch(dev_pid)
    {
        case RAZER_LEVIATHAN_V2_PID:
        case RAZER_LEVIATHAN_V2X_PID:
            report_index    = 0x07;
            response_index  = 0x07;
            break;

        default:
            report_index    = 0;
            response_index  = 0;
    }

    /*-----------------------------------------------------------------*\
    | Determine transaction ID for device                               |
    \*-----------------------------------------------------------------*/
    dev_transaction_id = device_list[device_index]->transaction_id;

    switch(dev_pid)
    {
        case RAZER_CHARGING_PAD_CHROMA_PID:
        case RAZER_CHROMA_MUG_PID:
        case RAZER_FIREFLY_HYPERFLUX_PID:
            {
            razer_set_device_mode(RAZER_DEVICE_MODE_SOFTWARE);
            }
            break;
    }

    /*-----------------------------------------------------------------*\
    | Determine LED ID for device                                       |
    \*-----------------------------------------------------------------*/
    switch(dev_pid)
    {
        case RAZER_BASILISK_ULTIMATE_WIRED_PID:
        case RAZER_BASILISK_ULTIMATE_WIRELESS_PID:
        case RAZER_BASILISK_V3_PID:
        case RAZER_BASILISK_V3_35K_PID:
        case RAZER_BASILISK_V3_X_HYPERSPEED_PID:
        case RAZER_BASILISK_V3_PRO_WIRED_PID:
        case RAZER_BASILISK_V3_PRO_WIRELESS_PID:
        case RAZER_BASILISK_V3_PRO_35K_WIRED_PID:
        case RAZER_BASILISK_V3_PRO_35K_WIRELESS_PID:
        case RAZER_BASILISK_V3_PRO_BLUETOOTH_PID:
        case RAZER_BASE_STATION_CHROMA_PID:
        case RAZER_BASE_STATION_V2_CHROMA_PID:
        case RAZER_BLADE_14_2022_PID:
        case RAZER_BLADE_14_2023_PID:
        case RAZER_BLADE_15_2022_PID:
        case RAZER_CHARGING_PAD_CHROMA_PID:
        case RAZER_CHROMA_HDK_PID:
        case RAZER_COBRA_PRO_WIRED_PID:
        case RAZER_COBRA_PRO_WIRELESS_PID:
        case RAZER_CORE_X_PID:
        case RAZER_DEATHADDER_ELITE_PID:
        case RAZER_DEATHADDER_V2_PID:
        case RAZER_DEATHADDER_V2_MINI_PID:
        case RAZER_DEATHADDER_ESSENTIAL_V2_PID:
        case RAZER_DEATHSTALKER_V2_PRO_TKL_WIRED_PID:
        case RAZER_DEATHSTALKER_V2_PRO_TKL_WIRELESS_PID:
        case RAZER_DEATHSTALKER_V2_PRO_WIRED_PID:
        case RAZER_DEATHSTALKER_V2_PRO_WIRELESS_PID:
        case RAZER_FIREFLY_V2_PID:
        case RAZER_FIREFLY_V2_PRO_PID:
        case RAZER_FIREFLY_HYPERFLUX_PID:
        case RAZER_GOLIATHUS_CHROMA_EXTENDED_PID:
        case RAZER_GOLIATHUS_CHROMA_PID:
        case RAZER_GOLIATHUS_CHROMA_3XL_PID:
        case RAZER_LAPTOP_STAND_CHROMA_PID:
        case RAZER_LAPTOP_STAND_CHROMA_V2_PID:
        case RAZER_LEVIATHAN_V2_PID:
        case RAZER_LEVIATHAN_V2X_PID:
        case RAZER_MAMBA_ELITE_PID:
        case RAZER_MAMBA_HYPERFLUX_PID:
        case RAZER_MOUSE_BUNGEE_V3_CHROMA_PID:
        case RAZER_MOUSE_DOCK_PRO_PID:
        case RAZER_NAGA_CLASSIC_PID:
        case RAZER_NAGA_LEFT_HANDED_PID:
        case RAZER_NAGA_PRO_V2_WIRED_PID:
        case RAZER_NAGA_PRO_V2_WIRELESS_PID:
        case RAZER_O11_DYNAMIC_PID:
        case RAZER_STRIDER_CHROMA_PID:
        case RAZER_TARTARUS_PRO_PID:
        case RAZER_TARTARUS_V2_PID:
            dev_led_id = RAZER_LED_ID_ZERO;
            break;

        case RAZER_BLACKWIDOW_2019_PID:
        case RAZER_BLACKWIDOW_ELITE_PID:
        case RAZER_BLACKWIDOW_ESSENTIAL_PID:
        case RAZER_BLACKWIDOW_LITE_PID:
        case RAZER_BLACKWIDOW_V3_PID:
        case RAZER_BLACKWIDOW_V3_PRO_WIRED_PID:
        case RAZER_BLACKWIDOW_V3_PRO_BLUETOOTH_PID:
        case RAZER_BLACKWIDOW_V3_PRO_WIRELESS_PID:
        case RAZER_BLACKWIDOW_V3_TKL_PID:
        case RAZER_BLACKWIDOW_V3_MINI_WIRED_PID:
        case RAZER_BLACKWIDOW_V3_MINI_WIRELESS_PID:
        case RAZER_CYNOSA_CHROMA_PID:
        case RAZER_CYNOSA_LITE_PID:
        case RAZER_CYNOSA_V2_PID:
        case RAZER_DEATHSTALKER_V2_PID:
        case RAZER_HUNTSMAN_ELITE_PID:
        case RAZER_HUNTSMAN_PID:
        case RAZER_HUNTSMAN_MINI_PID:
        case RAZER_HUNTSMAN_MINI_ANALOG_PID:
        case RAZER_HUNTSMAN_TE_PID:
        case RAZER_HUNTSMAN_V2_ANALOG_PID:
        case RAZER_HUNTSMAN_V2_TKL_PID:
        case RAZER_HUNTSMAN_V2_PID:
        case RAZER_HUNTSMAN_V3_PRO_PID:
        case RAZER_HUNTSMAN_V3_PRO_TKL_WHITE_PID:
        case RAZER_ORNATA_CHROMA_PID:
        case RAZER_ORNATA_CHROMA_V2_PID:
        case RAZER_ORNATA_V3_PID:
        case RAZER_ORNATA_V3_REV2_PID:
        case RAZER_ORNATA_V3_TKL_PID:
        case RAZER_ORNATA_V3_X_PID:
        case RAZER_CORE_PID:
        case RAZER_FIREFLY_PID:
        default:
            dev_led_id = RAZER_LED_ID_BACKLIGHT;
            break;
    }

    /*-----------------------------------------------------------------*\
    | Determine matrix type for device                                  |
    \*-----------------------------------------------------------------*/
    matrix_type = device_list[device_index]->matrix_type;

    /*-----------------------------------------------------------------*\
    | Start keepalive thread for devices that need it to prevent RGB    |
    | from timing out                                                   |
    \*-----------------------------------------------------------------*/
    switch(dev_pid)
    {
        case RAZER_BLADE_14_2021_PID:
        case RAZER_BLADE_14_2022_PID:
            keepalive_thread_run = true;
            keepalive_thread     = new std::thread(&RazerController::KeepaliveThreadFunction, this);
            break;

        default:
            keepalive_thread_run = false;
            keepalive_thread     = NULL;
            break;
    }
}

RazerController::~RazerController()
{
    if(keepalive_thread != NULL)
    {
        keepalive_thread_run = false;
        keepalive_thread->join();
    }

    hid_close(dev);
    delete guard_manager_ptr;
}

std::string RazerController::GetName()
{
    return(name);
}

unsigned int RazerController::GetDeviceIndex()
{
    return(device_index);
}

device_type RazerController::GetDeviceType()
{
    return(device_list[device_index]->type);
}

std::string RazerController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RazerController::GetFirmwareString()
{
    return(razer_get_firmware());
}

std::string RazerController::GetSerialString()
{
    return(razer_get_serial());
}

void RazerController::KeepaliveThreadFunction()
{
    /*-----------------------------------------------------------------*\
    | Performing a get device mode request seems to be enough to keep   |
    | the lighting active on devices with the lighting timeout, so      |
    | periodically send a device mode request every 2.5s.               |
    \*-----------------------------------------------------------------*/
    while(keepalive_thread_run.load())
    {
        if((std::chrono::steady_clock::now() - last_update_time) > 2500ms)
        {
            razer_get_device_mode();
        }
        std::this_thread::sleep_for(1s);
    }
}

void RazerController::SetAddressableZoneSizes(unsigned char zone_1_size, unsigned char zone_2_size, unsigned char zone_3_size, unsigned char zone_4_size, unsigned char zone_5_size, unsigned char zone_6_size)
{
    razer_report report     = razer_create_addressable_size_report(zone_1_size, zone_2_size, zone_3_size, zone_4_size, zone_5_size, zone_6_size);

    razer_usb_send(&report);
}

unsigned char RazerController::GetMaxBrightness()
{
    /*-----------------------------------------------------*\
    | Max brightness for most devices is 0xFF (255)         |
    |   Add PIDs only for devices that use 0x64 (100)       |
    |   or any another arbitrary value                      |
    \*-----------------------------------------------------*/
    unsigned char max_brightness = 255;

    switch(dev_pid)
    {
        /*-----------------------------------------------------*\
        | Mice                                                  |
        \*-----------------------------------------------------*/
        case RAZER_DEATHADDER_ESSENTIAL_V2_PID:

            max_brightness = 100;
            break;
    }

    return(max_brightness);
}

void RazerController::SetBrightness(unsigned char brightness)
{
    razer_set_brightness(brightness);
}

void RazerController::SetLEDs(RGBColor* colors)
{
    /*---------------------------------------------------------*\
    | Get the matrix layout information from the device list    |
    \*---------------------------------------------------------*/
    unsigned int matrix_cols = device_list[device_index]->cols;
    unsigned int matrix_rows = device_list[device_index]->rows;

    /*---------------------------------------------------------*\
    | Create an output array large enough to hold RGB data for  |
    | a single row                                              |
    \*---------------------------------------------------------*/
    unsigned char* output_array = new unsigned char[matrix_cols * 3];

    /*---------------------------------------------------------*\
    | Send one row of the custom frame at a time                |
    \*---------------------------------------------------------*/
    for (unsigned int row = 0; row < matrix_rows; row++)
    {
        unsigned int row_offset = (row * matrix_cols);

        /*-----------------------------------------------------*\
        | Fill the output array with RGB data                   |
        \*-----------------------------------------------------*/
        for(unsigned int col = 0; col < matrix_cols; col++)
        {
            unsigned int color_idx = col + row_offset;
            output_array[(col * 3) + 0] = (char)RGBGetRValue(colors[color_idx]);
            output_array[(col * 3) + 1] = (char)RGBGetGValue(colors[color_idx]);
            output_array[(col * 3) + 2] = (char)RGBGetBValue(colors[color_idx]);
        }

        /*-----------------------------------------------------*\
        | Send the output array to the device                   |
        \*-----------------------------------------------------*/
        std::this_thread::sleep_for(1ms);

        razer_set_custom_frame(row, 0, matrix_cols - 1, output_array);
    }

    std::this_thread::sleep_for(1ms);

    /*---------------------------------------------------------*\
    | Set custom mode to apply frame                            |
    \*---------------------------------------------------------*/
    razer_set_mode_custom();

    /*---------------------------------------------------------*\
    | Delete the output array                                   |
    \*---------------------------------------------------------*/
    delete[] output_array;
}

void RazerController::SetModeBreathingOneColor(unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_set_mode_breathing_one_color(red, grn, blu);
}

void RazerController::SetModeBreathingRandom()
{
    razer_set_mode_breathing_random();
}

void RazerController::SetModeBreathingTwoColors(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2)
{
    razer_set_mode_breathing_two_colors(r1, g1, b1, r2, g2, b2);
}

void RazerController::SetModeOff()
{
    razer_set_mode_none();
}

void RazerController::SetModeSpectrumCycle()
{
    razer_set_mode_spectrum_cycle();
}

void RazerController::SetModeStatic(unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_set_mode_static(red, grn, blu);
}

void RazerController::SetModeWave(unsigned char direction)
{
    razer_set_mode_wave(direction);
}

bool RazerController::SupportsBreathing()
{
    /*-----------------------------------------------------*\
    | Breathing Mode is assumed as supported in hardware    |
    |   Add PIDs only for devices that DO NOT support the   |
    |   hardware breathing mode i.e. Packet captures show   |
    |   software driving the basic `Breathing` mode         |
    \*-----------------------------------------------------*/
    bool supports_breathing = true;

    switch(dev_pid)
    {
        /*-----------------------------------------------------*\
        | Mice                                                  |
        \*-----------------------------------------------------*/
        case RAZER_BASILISK_V3_PID:
        case RAZER_BASILISK_V3_35K_PID:
        case RAZER_BASILISK_V3_PRO_WIRED_PID:
        case RAZER_BASILISK_V3_PRO_WIRELESS_PID:
        case RAZER_BASILISK_V3_PRO_35K_WIRED_PID:
        case RAZER_BASILISK_V3_PRO_35K_WIRELESS_PID:
        case RAZER_BASILISK_V3_PRO_BLUETOOTH_PID:

            supports_breathing = false;
            break;
    }

    return(supports_breathing);
}

bool RazerController::SupportsReactive()
{
    return(false);
}

bool RazerController::SupportsWave()
{
    bool supports_wave = false;

    switch(dev_pid)
    {
        /*-----------------------------------------------------*\
        | Keyboards, Keypads, and Laptops                       |
        \*-----------------------------------------------------*/
        case RAZER_BLACKWIDOW_CHROMA_PID:
        case RAZER_BLACKWIDOW_CHROMA_TE_PID:
        case RAZER_BLACKWIDOW_CHROMA_V2_PID:
        case RAZER_BLACKWIDOW_OVERWATCH_PID:
        case RAZER_BLACKWIDOW_V3_PID:
        case RAZER_BLACKWIDOW_V3_PRO_WIRED_PID:
        case RAZER_BLACKWIDOW_V3_PRO_BLUETOOTH_PID:
        case RAZER_BLACKWIDOW_V3_PRO_WIRELESS_PID:
        case RAZER_BLACKWIDOW_V3_TKL_PID:
        case RAZER_BLACKWIDOW_V3_MINI_WIRED_PID:
        case RAZER_BLACKWIDOW_V3_MINI_WIRELESS_PID:
        case RAZER_BLACKWIDOW_V4_PID:
        case RAZER_BLACKWIDOW_V4_PRO_PID:
        case RAZER_BLACKWIDOW_V4_PRO_75_WIRED_PID:
        case RAZER_BLACKWIDOW_V4_X_PID:
        case RAZER_BLACKWIDOW_X_CHROMA_PID:
        case RAZER_BLACKWIDOW_X_CHROMA_TE_PID:
        case RAZER_BLADE_2016_PID:
        case RAZER_BLADE_LATE_2016_PID:
        case RAZER_BLADE_2018_ADVANCED_PID:
        case RAZER_BLADE_2018_MERCURY_PID:
        case RAZER_BLADE_2019_ADVANCED_PID:
        case RAZER_BLADE_2019_BASE_PID:
        case RAZER_BLADE_2019_MERCURY_PID:
        case RAZER_BLADE_2019_STUDIO_PID:
        case RAZER_BLADE_2020_ADVANCED_PID:
        case RAZER_BLADE_LATE_2020_PID:
        case RAZER_BLADE_2020_BASE_PID:
        case RAZER_BLADE_2021_ADVANCED_PID:
        case RAZER_BLADE_2021_BASE_PID:
        case RAZER_BLADE_2021_BASE_V2_PID:
        case RAZER_BLADE_LATE_2021_ADVANCED_PID:
        case RAZER_BLADE_14_2021_PID:
        case RAZER_BLADE_14_2022_PID:
        case RAZER_BLADE_14_2023_PID:
        case RAZER_BLADE_15_2022_PID:
        case RAZER_BLADE_PRO_2016_PID:
        case RAZER_BLADE_PRO_2017_PID:
        case RAZER_BLADE_PRO_2017_FULLHD_PID:
        case RAZER_BLADE_PRO_2019_PID:
        case RAZER_BLADE_PRO_LATE_2019_PID:
        case RAZER_BLADE_PRO_17_2020_PID:
        case RAZER_BLADE_PRO_17_2021_PID:
        case RAZER_BLADE_STEALTH_2016_PID:
        case RAZER_BLADE_STEALTH_LATE_2016_PID:
        case RAZER_BLADE_STEALTH_2017_PID:
        case RAZER_BLADE_STEALTH_LATE_2017_PID:
        case RAZER_BOOK_13_2020_PID:
        case RAZER_CYNOSA_CHROMA_PID:
        case RAZER_CYNOSA_V2_PID:
        case RAZER_DEATHSTALKER_CHROMA_PID:
        case RAZER_DEATHSTALKER_V2_PID:
        case RAZER_DEATHSTALKER_V2_PRO_TKL_WIRED_PID:
        case RAZER_DEATHSTALKER_V2_PRO_TKL_WIRELESS_PID:
        case RAZER_DEATHSTALKER_V2_PRO_WIRED_PID:
        case RAZER_DEATHSTALKER_V2_PRO_WIRELESS_PID:
        case RAZER_ORNATA_CHROMA_PID:
        case RAZER_ORNATA_CHROMA_V2_PID:
        case RAZER_ORNATA_V3_PID:
        case RAZER_ORNATA_V3_REV2_PID:
        case RAZER_ORNATA_V3_TKL_PID:
        case RAZER_ORNATA_V3_X_PID:
        case RAZER_HUNTSMAN_PID:
        case RAZER_HUNTSMAN_ELITE_PID:
        case RAZER_HUNTSMAN_MINI_PID:
        case RAZER_HUNTSMAN_MINI_ANALOG_PID:
        case RAZER_HUNTSMAN_TE_PID:
        case RAZER_HUNTSMAN_V2_ANALOG_PID:
        case RAZER_HUNTSMAN_V2_TKL_PID:
        case RAZER_HUNTSMAN_V2_PID:
        case RAZER_HUNTSMAN_V3_PRO_PID:
        case RAZER_HUNTSMAN_V3_PRO_TKL_WHITE_PID:
        case RAZER_ORBWEAVER_CHROMA_PID:
        case RAZER_TARTARUS_PRO_PID:
        case RAZER_TARTARUS_V2_PID:

        /*-----------------------------------------------------*\
        | Mice                                                  |
        \*-----------------------------------------------------*/
        case RAZER_BASILISK_ULTIMATE_WIRED_PID:
        case RAZER_BASILISK_ULTIMATE_WIRELESS_PID:
        case RAZER_BASILISK_V3_PID:
        case RAZER_BASILISK_V3_35K_PID:
        case RAZER_BASILISK_V3_PRO_WIRED_PID:
        case RAZER_BASILISK_V3_PRO_WIRELESS_PID:
        case RAZER_BASILISK_V3_PRO_35K_WIRED_PID:
        case RAZER_BASILISK_V3_PRO_35K_WIRELESS_PID:
        case RAZER_BASILISK_V3_PRO_BLUETOOTH_PID:
        case RAZER_COBRA_PRO_WIRED_PID:
        case RAZER_COBRA_PRO_WIRELESS_PID:
        case RAZER_DIAMONDBACK_CHROMA_PID:
        case RAZER_MAMBA_2015_WIRED_PID:
        case RAZER_MAMBA_2015_WIRELESS_PID:
        case RAZER_MAMBA_ELITE_PID:
        case RAZER_MAMBA_TE_PID:
        case RAZER_NAGA_LEFT_HANDED_PID:

        /*-----------------------------------------------------*\
        | Headsets                                              |
        \*-----------------------------------------------------*/
        case RAZER_TIAMAT_71_V2_PID:

        /*-----------------------------------------------------*\
        | Accessories                                           |
        \*-----------------------------------------------------*/
        case RAZER_BASE_STATION_CHROMA_PID:
        case RAZER_BASE_STATION_V2_CHROMA_PID:
        case RAZER_CHARGING_PAD_CHROMA_PID:
        case RAZER_CHROMA_ADDRESSABLE_RGB_CONTROLLER_PID:
        case RAZER_CHROMA_MUG_PID:
        case RAZER_CHROMA_HDK_PID:
        case RAZER_CHROMA_PC_CASE_LIGHTING_KIT_PID:
        case RAZER_CORE_PID:
        case RAZER_CORE_X_PID:
        case RAZER_FIREFLY_PID:
        case RAZER_FIREFLY_V2_PID:
        case RAZER_FIREFLY_V2_PRO_PID:
        case RAZER_FIREFLY_HYPERFLUX_PID:
        case RAZER_LAPTOP_STAND_CHROMA_PID:
        case RAZER_LAPTOP_STAND_CHROMA_V2_PID:
        case RAZER_LEVIATHAN_V2_PID:
        case RAZER_LEVIATHAN_V2X_PID:
        case RAZER_MOUSE_BUNGEE_V3_CHROMA_PID:
        case RAZER_MOUSE_DOCK_PRO_PID:
        case RAZER_NOMMO_CHROMA_PID:
        case RAZER_NOMMO_PRO_PID:
        case RAZER_O11_DYNAMIC_PID:
        case RAZER_STRIDER_CHROMA_PID:
        case RAZER_THUNDERBOLT_4_DOCK_CHROMA_PID:

            supports_wave = true;
            break;
    }

    return(supports_wave);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

unsigned char RazerController::razer_calculate_crc(razer_report* report)
{
    /*---------------------------------------------------------*\
    | The second to last byte of report is a simple checksum    |
    | Just xor all bytes up with overflow and you are done      |
    \*---------------------------------------------------------*/
    unsigned char   crc         = 0;
    unsigned char*  report_ptr  = (unsigned char*)report;

    /*---------------------------------------------------------*\
    | The start and end checks here have been modified compared |
    | to the original OpenRazer version.  This is due to adding |
    | the report ID field to the razer_report structure for     |
    | compatibility with HIDAPI.                                |
    \*---------------------------------------------------------*/
    for(unsigned int i = 3; i < 89; i++)
    {
        crc ^= report_ptr[i];
    }

    return crc;
}

/*---------------------------------------------------------------------------------*\
| Basic report and response creation functions                                      |
\*---------------------------------------------------------------------------------*/

razer_report RazerController::razer_create_report(unsigned char command_class, unsigned char command_id, unsigned char data_size)
{
    razer_report new_report;

    /*---------------------------------------------------------*\
    | Zero out the new report                                   |
    \*---------------------------------------------------------*/
    memset(&new_report, 0, sizeof(razer_report));

    /*---------------------------------------------------------*\
    | Fill in the new report with the given parameters          |
    \*---------------------------------------------------------*/
    new_report.report_id            = report_index;
    new_report.status               = 0x00;
    new_report.transaction_id.id    = dev_transaction_id;
    new_report.remaining_packets    = 0x00;
    new_report.protocol_type        = 0x00;
    new_report.data_size            = data_size;
    new_report.command_class        = command_class;
    new_report.command_id.id        = command_id;

    return new_report;
}

razer_report RazerController::razer_create_response()
{
    razer_report new_report;

    /*---------------------------------------------------------*\
    | Zero out the new report                                   |
    \*---------------------------------------------------------*/
    memset(&new_report, 0, sizeof(razer_report));

    /*---------------------------------------------------------*\
    | Fill in the new report with the given parameters          |
    \*---------------------------------------------------------*/
    new_report.report_id            = response_index;
    new_report.status               = 0x00;
    new_report.transaction_id.id    = dev_transaction_id;
    new_report.remaining_packets    = 0x00;
    new_report.protocol_type        = 0x00;
    new_report.command_class        = 0x00;
    new_report.command_id.id        = 0x00;
    new_report.data_size            = 0x00;

    return new_report;
}

/*---------------------------------------------------------------------------------*\
| Command report creation functions                                                 |
\*---------------------------------------------------------------------------------*/

razer_report RazerController::razer_create_addressable_size_report
    (
    unsigned char zone_1_size,
    unsigned char zone_2_size,
    unsigned char zone_3_size,
    unsigned char zone_4_size,
    unsigned char zone_5_size,
    unsigned char zone_6_size
    )
{
    razer_report report         = razer_create_report(0x0F, 0x08, 0x0D);

    report.arguments[0]         = 0x06;
    report.arguments[1]         = (zone_1_size == 0) ? 0x01 : 0x19;
    report.arguments[2]         = zone_1_size;
    report.arguments[3]         = (zone_2_size == 0) ? 0x02 : 0x19;
    report.arguments[4]         = zone_2_size;
    report.arguments[5]         = (zone_3_size == 0) ? 0x03 : 0x19;
    report.arguments[6]         = zone_3_size;
    report.arguments[7]         = (zone_4_size == 0) ? 0x04 : 0x19;
    report.arguments[8]         = zone_4_size;
    report.arguments[9]         = (zone_5_size == 0) ? 0x05 : 0x19;
    report.arguments[10]        = zone_5_size;
    report.arguments[11]        = (zone_6_size == 0) ? 0x06 : 0x19;
    report.arguments[12]        = zone_6_size;

    return report;
}

razer_report RazerController::razer_create_addressable_startup_detect_report(bool enable)
{
    razer_report report         = razer_create_report(0x00, 0x44, 0x01);

    report.arguments[0]         = enable;

    return report;
}

razer_report RazerController::razer_create_brightness_extended_matrix_report(unsigned char variable_storage, unsigned char led_id, unsigned char brightness)
{
    razer_report report         = razer_create_report(0x0F, 0x04, 0x03);

    report.arguments[0]         = variable_storage;
    report.arguments[1]         = led_id;
    report.arguments[2]         = brightness;

    return report;
}

razer_report RazerController::razer_create_brightness_standard_report(unsigned char variable_storage, unsigned char led_id, unsigned char brightness)
{
    razer_report report         = razer_create_report(0x03, 0x03, 0x03);

    report.arguments[0]         = variable_storage;
    report.arguments[1]         = led_id;
    report.arguments[2]         = brightness;

    return report;
}

razer_argb_report RazerController::razer_create_custom_frame_argb_report(unsigned char row_index, unsigned char stop_col, unsigned char* rgb_data)
{
    razer_argb_report report;

    /*---------------------------------------------------------*\
    | Zero out the new report                                   |
    \*---------------------------------------------------------*/
    memset(&report, 0, sizeof(razer_argb_report));

    /*---------------------------------------------------------*\
    | Fill in report header                                     |
    \*---------------------------------------------------------*/
    report.hid_id               = 0;

    if(row_index < 5)
    {
        report.report_id        = 0x04;
    }
    else
    {
        report.report_id        = 0x84;
    }

    report.channel_1            = row_index;
    report.channel_2            = row_index;
    report.pad                  = 0;
    report.last_idx             = stop_col;

    /*---------------------------------------------------------*\
    | Copy in the RGB data                                      |
    \*---------------------------------------------------------*/
    memcpy(&report.color_data, rgb_data, (stop_col + 1) * 3);

    return report;
}

razer_report RazerController::razer_create_custom_frame_linear_report(unsigned char start_col, unsigned char stop_col, unsigned char* rgb_data)
{
    razer_report report         = razer_create_report(0x03, 0x0C, 0x32);
    size_t       row_length     = (size_t) (((stop_col + 1) - start_col) * 3);

    report.arguments[0]         = start_col;
    report.arguments[1]         = stop_col;

    /*---------------------------------------------------------*\
    | Copy in the RGB data                                      |
    \*---------------------------------------------------------*/
    memcpy(&report.arguments[2], rgb_data, row_length);

    return report;
}

razer_report RazerController::razer_create_custom_frame_extended_matrix_report(unsigned char row_index, unsigned char start_col, unsigned char stop_col, unsigned char* rgb_data)
{
    const size_t row_length     = (size_t)(((stop_col + 1) - start_col) * 3);
    const size_t packet_length  = row_length + 5;

    razer_report report         = razer_create_report(0x0F, 0x03, (unsigned char)packet_length);

    report.arguments[2]         = row_index;
    report.arguments[3]         = start_col;
    report.arguments[4]         = stop_col;

    /*---------------------------------------------------------*\
    | Copy in the RGB data                                      |
    \*---------------------------------------------------------*/
    memcpy(&report.arguments[5], rgb_data, row_length);

    return report;
}

razer_report RazerController::razer_create_custom_frame_standard_matrix_report(unsigned char row_index, unsigned char start_col, unsigned char stop_col, unsigned char* rgb_data)
{
    const size_t row_length     = (size_t)(((stop_col + 1) - start_col) * 3);
    const size_t packet_length  = row_length + 4;

    razer_report report         = razer_create_report(0x03, 0x0B, (unsigned char)packet_length);

    report.arguments[0]         = 0xFF;
    report.arguments[1]         = row_index;
    report.arguments[2]         = start_col;
    report.arguments[3]         = stop_col;

    /*---------------------------------------------------------*\
    | Copy in the RGB data                                      |
    \*---------------------------------------------------------*/
    memcpy(&report.arguments[4], rgb_data, row_length);

    return report;
}

razer_report RazerController::razer_create_device_mode_report(unsigned char mode, unsigned char param)
{
    razer_report report         = razer_create_report(0x00, 0x04, 0x02);

    report.arguments[0]         = mode;
    report.arguments[1]         = param;

    return report;
}

razer_report RazerController::razer_create_mode_breathing_one_color_extended_matrix_report(unsigned char variable_storage, unsigned char led_id, unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_report report         = razer_create_report(0x0F, 0x02, 0x09);

    report.arguments[0]         = variable_storage;
    report.arguments[1]         = led_id;
    report.arguments[2]         = 0x02;

    report.arguments[3]         = 0x01;
    report.arguments[5]         = 0x01;

    report.arguments[6]         = red;
    report.arguments[7]         = grn;
    report.arguments[8]         = blu;

    return report;
}

razer_report RazerController::razer_create_mode_breathing_one_color_standard_matrix_report(unsigned char /*variable_storage*/, unsigned char /*led_id*/, unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_report report         = razer_create_report(0x03, 0x0A, 0x08);

    report.arguments[0]         = 0x03;
    report.arguments[1]         = 0x01;
    report.arguments[2]         = red;
    report.arguments[3]         = grn;
    report.arguments[4]         = blu;

    return report;
}

razer_report RazerController::razer_create_mode_breathing_random_extended_matrix_report(unsigned char variable_storage, unsigned char led_id)
{
    razer_report report         = razer_create_report(0x0F, 0x02, 0x06);

    report.arguments[0]         = variable_storage;
    report.arguments[1]         = led_id;
    report.arguments[2]         = 0x02;

    return report;
}

razer_report RazerController::razer_create_mode_breathing_random_standard_matrix_report(unsigned char /*variable_storage*/, unsigned char /*led_id*/)
{
    razer_report report         = razer_create_report(0x03, 0x0A, 0x08);

    report.arguments[0]         = 0x03;
    report.arguments[1]         = 0x03;

    return report;
}

razer_report RazerController::razer_create_mode_breathing_two_colors_extended_matrix_report(unsigned char variable_storage, unsigned char led_id, unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2)
{
    razer_report report         = razer_create_report(0x0F, 0x02, 0x0C);

    report.arguments[0]         = variable_storage;
    report.arguments[1]         = led_id;
    report.arguments[2]         = 0x02;

    report.arguments[3]         = 0x02;
    report.arguments[5]         = 0x02;

    report.arguments[6]         = r1;
    report.arguments[7]         = g1;
    report.arguments[8]         = b1;
    report.arguments[9]         = r2;
    report.arguments[10]        = g2;
    report.arguments[11]        = b2;

    return report;
}

razer_report RazerController::razer_create_mode_breathing_two_colors_standard_matrix_report(unsigned char /*variable_storage*/, unsigned char /*led_id*/, unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2)
{
    razer_report report         = razer_create_report(0x03, 0x0A, 0x08);

    report.arguments[0]         = 0x03;
    report.arguments[1]         = 0x02;
    report.arguments[2]         = r1;
    report.arguments[3]         = g1;
    report.arguments[4]         = b1;
    report.arguments[5]         = r2;
    report.arguments[6]         = g2;
    report.arguments[7]         = b2;

    return report;
}

razer_report RazerController::razer_create_mode_custom_extended_matrix_report()
{
    struct razer_report report  = razer_create_report(0x0F, 0x02, 0x0C);

    report.arguments[0]         = 0x00;
    report.arguments[1]         = 0x00;
    report.arguments[2]         = 0x08;

    return report;
}

razer_report RazerController::razer_create_mode_custom_standard_matrix_report(unsigned char variable_storage)
{
    razer_report report         = razer_create_report(0x03, 0x0A, 0x02);

    report.arguments[0]         = 0x05;
    report.arguments[1]         = variable_storage;

    return report;
}

razer_report RazerController::razer_create_mode_none_extended_matrix_report(unsigned char variable_storage, unsigned char led_id)
{
    struct razer_report report  = razer_create_report(0x0F, 0x02, 06);

    report.arguments[0]         = variable_storage;
    report.arguments[1]         = led_id;
    report.arguments[2]         = 0x00;

    return report;
}

razer_report RazerController::razer_create_mode_none_standard_matrix_report(unsigned char /*variable_storage*/, unsigned char /*led_id*/)
{
    struct razer_report report  = razer_create_report(0x03, 0x0A, 0x01);

    report.arguments[0]         = 0x00;

    return report;
}

razer_report RazerController::razer_create_mode_spectrum_cycle_extended_matrix_report(unsigned char variable_storage, unsigned char led_id)
{
    razer_report report         = razer_create_report(0x0F, 0x02, 0x06);

    report.arguments[0]         = variable_storage;
    report.arguments[1]         = led_id;
    report.arguments[2]         = 0x03;

    return report;
}

razer_report RazerController::razer_create_mode_spectrum_cycle_standard_matrix_report(unsigned char /*variable_storage*/, unsigned char /*led_id*/)
{
    razer_report report         = razer_create_report(0x03, 0x0A, 0x01);

    report.arguments[0]         = 0x04;

    return report;
}

razer_report RazerController::razer_create_mode_static_extended_matrix_report(unsigned char variable_storage, unsigned char led_id, unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_report report         = razer_create_report(0x0F, 0x02, 0x09);

    report.arguments[0]         = variable_storage;
    report.arguments[1]         = led_id;
    report.arguments[2]         = 0x01;

    report.arguments[5]         = 0x01;
    report.arguments[6]         = red;
    report.arguments[7]         = grn;
    report.arguments[8]         = blu;

    return report;
}

razer_report RazerController::razer_create_mode_static_standard_matrix_report(unsigned char /*variable_storage*/, unsigned char /*led_id*/, unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_report report         = razer_create_report(0x03, 0x0A, 0x04);

    report.arguments[0]         = 0x06;
    report.arguments[1]         = red;
    report.arguments[2]         = grn;
    report.arguments[3]         = blu;

    return report;
}

razer_report RazerController::razer_create_mode_wave_extended_matrix_report(unsigned char variable_storage, unsigned char led_id, unsigned char direction)
{
    razer_report report         = razer_create_report(0x0F, 0x02, 0x06);

    report.arguments[0]         = variable_storage;
    report.arguments[1]         = led_id;
    report.arguments[2]         = 0x04;

    report.arguments[3]         = direction;
    report.arguments[4]         = 0x28;

    return report;
}

razer_report RazerController::razer_create_mode_wave_standard_matrix_report(unsigned char /*variable_storage*/, unsigned char /*led_id*/, unsigned char direction)
{
    razer_report report         = razer_create_report(0x03, 0x0A, 0x02);

    report.arguments[0]         = 0x01;
    report.arguments[1]         = direction;

    return report;
}

razer_report RazerController::razer_create_set_led_rgb_report(unsigned char variable_storage, unsigned char led_id, unsigned char* rgb_data)
{
    razer_report report         = razer_create_report(0x03, 0x01, 0x05);

    report.arguments[0]         = variable_storage;
    report.arguments[1]         = led_id;
    report.arguments[2]         = rgb_data[0];
    report.arguments[3]         = rgb_data[1];
    report.arguments[4]         = rgb_data[2];

    return report;
}

razer_report RazerController::razer_create_set_led_effect_report(unsigned char variable_storage, unsigned char led_id, unsigned char effect)
{
    razer_report report         = razer_create_report(0x03, 0x02, 0x03);

    report.arguments[0]         = variable_storage;
    report.arguments[1]         = led_id;

    if(effect > 5)
    {
        report.arguments[2]     = 5;
    }
    else
    {
        report.arguments[2]     = effect;
    }

    return report;
}

/*---------------------------------------------------------------------------------*\
| Get functions (request information from device)                                   |
\*---------------------------------------------------------------------------------*/

unsigned char RazerController::razer_get_device_mode()
{
    std::string         firmware_string         = "";
    struct razer_report report                  = razer_create_report(0x00, RAZER_COMMAND_ID_GET_DEVICE_MODE, 0x02);
    struct razer_report response_report         = razer_create_response();

    std::this_thread::sleep_for(2ms);
    razer_usb_send(&report);
    std::this_thread::sleep_for(5ms);
    razer_usb_receive(&response_report);

    return(response_report.arguments[0]);
}

std::string RazerController::razer_get_firmware()
{
    std::string         firmware_string         = "";
    struct razer_report report                  = razer_create_report(0x00, RAZER_COMMAND_ID_GET_FIRMWARE_VERSION, 0x02);
    struct razer_report response_report         = razer_create_response();

    std::this_thread::sleep_for(2ms);
    razer_usb_send(&report);
    std::this_thread::sleep_for(5ms);
    razer_usb_receive(&response_report);

    firmware_string = "v" + std::to_string(response_report.arguments[0]) + "." + std::to_string(response_report.arguments[1]);

    return firmware_string;
}

std::string RazerController::razer_get_serial()
{
    char                serial_string[64]   = "";
    struct razer_report report              = razer_create_report(0x00, RAZER_COMMAND_ID_GET_SERIAL_STRING, 0x16);
    struct razer_report response_report     = razer_create_response();

    std::this_thread::sleep_for(2ms);
    razer_usb_send(&report);
    std::this_thread::sleep_for(5ms);
    razer_usb_receive(&response_report);

    memcpy(&serial_string[0], &response_report.arguments[0], 22);
    serial_string[22] = '\0';

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

void RazerController::razer_get_keyboard_info(unsigned char* layout, unsigned char* variant)
{
    struct razer_report report              = razer_create_report(0x00, RAZER_COMMAND_ID_GET_KEYBOARD_INFO, 0x00);
    struct razer_report response_report     = razer_create_response();

    std::this_thread::sleep_for(1ms);
    razer_usb_send(&report);
    std::this_thread::sleep_for(1ms);
    razer_usb_receive(&response_report);

    *layout = response_report.arguments[0];
    *variant = response_report.arguments[1];
}

unsigned char RazerController::GetKeyboardLayoutType()
{
    unsigned char layout;
    unsigned char variant;

    RazerController::razer_get_keyboard_info(&layout, &variant);

    switch(layout)
    {
        case RAZER_KEYBOARD_LAYOUT_US:
        case RAZER_KEYBOARD_LAYOUT_RUSSIAN:                // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_CHT:                    // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_TURKISH:                // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_THAILAND:               // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_ARABIC:                 // Unconfirmed
            return RAZER_LAYOUT_TYPE_ANSI;

        case RAZER_KEYBOARD_LAYOUT_GREEK:                  // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_UK:
        case RAZER_KEYBOARD_LAYOUT_NORDIC:
        case RAZER_KEYBOARD_LAYOUT_KOREAN:                 // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_PORTUGESE_BRAZIL:       // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_SPANISH_LATIN_AMERICAN: // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_SWISS:                  // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_SPANISH_EUR:            // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_ITALIAN:                // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_PORTUGESE_PORTUGA:      // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_HEBREW:                 // Unconfirmed
            return RAZER_LAYOUT_TYPE_ISO;

        case RAZER_KEYBOARD_LAYOUT_FRENCH:
            return RAZER_LAYOUT_TYPE_AZERTY;

        case RAZER_KEYBOARD_LAYOUT_JAPAN:                  // Unconfirmed
            return RAZER_LAYOUT_TYPE_JIS;

        case RAZER_KEYBOARD_LAYOUT_GERMAN:
            return RAZER_LAYOUT_TYPE_QWERTZ;

        default:
            return RAZER_LAYOUT_TYPE_ALL;
    }
}

std::string RazerController::GetKeyboardLayoutString()
{
    unsigned char layout;
    unsigned char variant;

    RazerController::razer_get_keyboard_info(&layout, &variant);

    switch(layout)
    {
        case RAZER_KEYBOARD_LAYOUT_US:                     return "US (ANSI)";
        case RAZER_KEYBOARD_LAYOUT_GERMAN:                 return "German (QWERTZ)";
        case RAZER_KEYBOARD_LAYOUT_GREEK:                  return "Greek (ISO)";                   // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_FRENCH:                 return "French (ISO)";
        case RAZER_KEYBOARD_LAYOUT_RUSSIAN:                return "Russian (ANSI)";                // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_UK:                     return "UK (ISO)";
        case RAZER_KEYBOARD_LAYOUT_NORDIC:                 return "Nordic (ISO)";
        case RAZER_KEYBOARD_LAYOUT_CHT:                    return "Chinese Traditional (ANSI)";    // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_KOREAN:                 return "Korean (ISO)";                  // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_TURKISH:                return "Turkish (ANSI)";                // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_THAILAND:               return "Thai (ANSI)";                   // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_JAPAN:                  return "Japanese (JIS)";                // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_PORTUGESE_BRAZIL:       return "Portugese (Brazil) (ISO)";      // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_SPANISH_LATIN_AMERICAN: return "Spanish (Latin america) (ISO)"; // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_SWISS:                  return "Swiss (ISO)";                   // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_SPANISH_EUR:            return "Spanish (Europe) (ISO)";        // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_ITALIAN:                return "Italian (ISO)";                 // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_PORTUGESE_PORTUGA:      return "Portugese (Portugal) (ISO)";    // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_HEBREW:                 return "Hebrew (ISO)";                  // Unconfirmed
        case RAZER_KEYBOARD_LAYOUT_ARABIC:                 return "Arabic (ANSI)";                 // Unconfirmed
        default:
            std::string tmp = "Unknown: ";
            tmp.append(std::to_string(layout));
            return tmp;
    }
}

std::string RazerController::GetVariantName()
{
    unsigned char layout;
    unsigned char variant;

    RazerController::razer_get_keyboard_info(&layout, &variant);

    switch(variant)
    {
        case RAZER_KEYBOARD_VARIANT_BLACK:   return "Black";
        case RAZER_KEYBOARD_VARIANT_MERCURY: return "Mercury";
        default:                             return "Unkown Variant";
    }
}

/*---------------------------------------------------------------------------------*\
| Set functions (send information to device)                                        |
\*---------------------------------------------------------------------------------*/

void RazerController::razer_set_brightness(unsigned char brightness)
{
    razer_report report;

    switch(matrix_type)
    {
        case RAZER_MATRIX_TYPE_STANDARD:
        case RAZER_MATRIX_TYPE_LINEAR:
            report                          = razer_create_brightness_standard_report(RAZER_STORAGE_NO_SAVE, dev_led_id, brightness);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED:
            report                          = razer_create_brightness_extended_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, brightness);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED_ARGB:
            report                          = razer_create_brightness_extended_matrix_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_ARGB_CH_1, brightness);
            razer_usb_send(&report);

            report                          = razer_create_brightness_extended_matrix_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_ARGB_CH_2, brightness);
            razer_usb_send(&report);

            report                          = razer_create_brightness_extended_matrix_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_ARGB_CH_3, brightness);
            razer_usb_send(&report);

            report                          = razer_create_brightness_extended_matrix_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_ARGB_CH_4, brightness);
            razer_usb_send(&report);

            report                          = razer_create_brightness_extended_matrix_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_ARGB_CH_5, brightness);
            razer_usb_send(&report);

            report                          = razer_create_brightness_extended_matrix_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_ARGB_CH_6, brightness);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_CUSTOM:
            switch(dev_pid)
            {
                /*-------------------------------------------------*\
                | These devices use standard matrix reports         |
                \*-------------------------------------------------*/
                case RAZER_ORBWEAVER_CHROMA_PID:
                    report                  = razer_create_brightness_standard_report(RAZER_STORAGE_NO_SAVE, dev_led_id, brightness);
                    razer_usb_send(&report);
                    break;
            }
            break;
    }
}

void RazerController::razer_set_custom_frame(unsigned char row_index, unsigned char start_col, unsigned char stop_col, unsigned char* rgb_data)
{
    razer_argb_report   argb_report;
    razer_report        report;

    switch(matrix_type)
    {
        case RAZER_MATRIX_TYPE_STANDARD:
            report                          = razer_create_custom_frame_standard_matrix_report(row_index, start_col, stop_col, rgb_data);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED:
            report                          = razer_create_custom_frame_extended_matrix_report(row_index, start_col, stop_col, rgb_data);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_LINEAR:
            report                          = razer_create_custom_frame_linear_report(start_col, stop_col, rgb_data);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED_ARGB:
            argb_report                     = razer_create_custom_frame_argb_report(row_index, stop_col, rgb_data);
            razer_usb_send_argb(&argb_report);
            break;

        case RAZER_MATRIX_TYPE_CUSTOM:
            switch(dev_pid)
            {
                /*-------------------------------------------------*\
                | These devices use individual LED effect reports   |
                \*-------------------------------------------------*/
                case RAZER_TARTARUS_CHROMA_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, rgb_data);
                    razer_usb_send(&report);
                    break;

                case RAZER_DEATHADDER_CHROMA_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, rgb_data);
                    razer_usb_send(&report);

                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_LOGO, &rgb_data[3]);
                    razer_usb_send(&report);
                    break;

                case RAZER_NAGA_EPIC_CHROMA_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, rgb_data);
                    razer_usb_send(&report);

                    std::this_thread::sleep_for(1ms);

                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, &rgb_data[3]);
                    razer_usb_send(&report);
                    break;

                case RAZER_MAMBA_2012_WIRED_PID:
                case RAZER_MAMBA_2012_WIRELESS_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, rgb_data);
                    razer_usb_send(&report);
                    break;

                /*-------------------------------------------------*\
                | The Orbweaver Chroma has an unusual matrix layout |
                | and the following code allows it to present as a  |
                | 5x5 matrix.  The hardware layout is:              |
                |                                                   |
                |   XX  XX  XX  XX  XX  XX  XX                      |
                |   XX  01  02  03  04  05  XX                      |
                |   XX  06  07  08  09  10  XX                      |
                |   XX  11  12  13  14  15  XX                      |
                |   XX  16  XX  17  18  19  20                      |
                |                                                   |
                | It uses a standard matrix report and transaction  |
                | ID 0x3F                                           |
                \*-------------------------------------------------*/
                case RAZER_ORBWEAVER_CHROMA_PID:
                    if(row_index != 3)
                    {
                        report              = razer_create_custom_frame_standard_matrix_report(row_index + 1, start_col + 1, stop_col + 1, rgb_data);
                        razer_usb_send(&report);
                    }
                    else
                    {
                        unsigned char rgb_data_adj[6*3];

                        memcpy(&rgb_data_adj[0], &rgb_data[0], 3);
                        memcpy(&rgb_data_adj[6], &rgb_data[3], 3*4);

                        report              = razer_create_custom_frame_standard_matrix_report(row_index + 1, start_col + 1, stop_col + 2, rgb_data_adj);
                        razer_usb_send(&report);
                    }
                    break;
            }
            break;
    }
}

void RazerController::razer_set_device_mode(unsigned char device_mode)
{
    razer_report report                  = razer_create_device_mode_report(device_mode, 0x00);
    razer_usb_send(&report);
}

void RazerController::razer_set_mode_breathing_one_color(unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_report report;

    switch(matrix_type)
    {
        case RAZER_MATRIX_TYPE_STANDARD:
        case RAZER_MATRIX_TYPE_LINEAR:
            report                          = razer_create_mode_breathing_one_color_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, red, grn, blu);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED:
        case RAZER_MATRIX_TYPE_EXTENDED_ARGB:
            report                          = razer_create_mode_breathing_one_color_extended_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, red, grn, blu);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_CUSTOM:
            unsigned char rgb_data[6];
            rgb_data[0]             = red;
            rgb_data[1]             = grn;
            rgb_data[2]             = blu;
            rgb_data[3]             = red;
            rgb_data[4]             = grn;
            rgb_data[5]             = blu;

            switch(dev_pid)
            {
                /*-------------------------------------------------*\
                | These devices use individual LED effect reports   |
                \*-------------------------------------------------*/
                case RAZER_TARTARUS_CHROMA_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, rgb_data);
                    razer_usb_send(&report);
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, 2);
                    razer_usb_send(&report);
                    break;

                case RAZER_DEATHADDER_CHROMA_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, rgb_data);
                    razer_usb_send(&report);
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 2);
                    razer_usb_send(&report);

                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_LOGO, &rgb_data[3]);
                    razer_usb_send(&report);
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_LOGO, 2);
                    razer_usb_send(&report);
                break;

                case RAZER_NAGA_EPIC_CHROMA_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, rgb_data);
                    razer_usb_send(&report);

                    std::this_thread::sleep_for(1ms);

                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 2);
                    razer_usb_send(&report);

                    std::this_thread::sleep_for(1ms);

                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, &rgb_data[3]);
                    razer_usb_send(&report);

                    std::this_thread::sleep_for(1ms);

                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, 2);
                    razer_usb_send(&report);
                    break;

                case RAZER_MAMBA_2012_WIRED_PID:
                case RAZER_MAMBA_2012_WIRELESS_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, rgb_data);
                    razer_usb_send(&report);
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 2);
                    razer_usb_send(&report);
                    break;

                /*-------------------------------------------------*\
                | These devices use standard matrix reports         |
                \*-------------------------------------------------*/
                case RAZER_ORBWEAVER_CHROMA_PID:
                    report                  = razer_create_mode_breathing_one_color_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, red, grn, blu);
                    razer_usb_send(&report);
                    break;
            }
            break;
    }
}

void RazerController::razer_set_mode_breathing_random()
{
    razer_report report;

    switch(matrix_type)
    {
        case RAZER_MATRIX_TYPE_STANDARD:
        case RAZER_MATRIX_TYPE_LINEAR:
            report                          = razer_create_mode_breathing_random_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED:
        case RAZER_MATRIX_TYPE_EXTENDED_ARGB:
            report                          = razer_create_mode_breathing_random_extended_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_CUSTOM:
            switch(dev_pid)
            {
                /*-------------------------------------------------*\
                | These devices use standard matrix reports         |
                \*-------------------------------------------------*/
                case RAZER_ORBWEAVER_CHROMA_PID:
                    report                  = razer_create_mode_breathing_random_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id);
                    razer_usb_send(&report);
                    break;
            }
            break;
    }
}

void RazerController::razer_set_mode_breathing_two_colors(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2)
{
    razer_report report;

    switch(matrix_type)
    {
        case RAZER_MATRIX_TYPE_STANDARD:
        case RAZER_MATRIX_TYPE_LINEAR:
            report                          = razer_create_mode_breathing_two_colors_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, r1, g1, b1, r2, g2, b2);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED:
        case RAZER_MATRIX_TYPE_EXTENDED_ARGB:
            report                          = razer_create_mode_breathing_two_colors_extended_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, r1, g1, b1, r2, g2, b2);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_CUSTOM:
            switch(dev_pid)
            {
                /*-------------------------------------------------*\
                | These devices use standard matrix reports         |
                \*-------------------------------------------------*/
                case RAZER_ORBWEAVER_CHROMA_PID:
                    report                  = razer_create_mode_breathing_two_colors_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, r1, g1, b1, r2, g2, b2);
                    razer_usb_send(&report);
                    break;
            }
            break;
    }
}

void RazerController::razer_set_mode_custom()
{
    razer_report report;

    switch(matrix_type)
    {
        case RAZER_MATRIX_TYPE_STANDARD:
        case RAZER_MATRIX_TYPE_LINEAR:
            report                          = razer_create_mode_custom_standard_matrix_report(RAZER_STORAGE_NO_SAVE);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED:
            report                          = razer_create_mode_custom_extended_matrix_report();
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED_ARGB:
            break;

        case RAZER_MATRIX_TYPE_CUSTOM:
            switch(dev_pid)
            {
                /*-------------------------------------------------*\
                | These devices use individual LED effect reports   |
                \*-------------------------------------------------*/
                case RAZER_TARTARUS_CHROMA_PID:
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, 0);
                    razer_usb_send(&report);
                    break;

                case RAZER_DEATHADDER_CHROMA_PID:
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 0);
                    razer_usb_send(&report);

                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_LOGO, 0);
                    razer_usb_send(&report);
                    break;

                case RAZER_NAGA_EPIC_CHROMA_PID:
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 0);
                    razer_usb_send(&report);

                    std::this_thread::sleep_for(1ms);

                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, 0);
                    razer_usb_send(&report);
                    break;

                case RAZER_MAMBA_2012_WIRED_PID:
                case RAZER_MAMBA_2012_WIRELESS_PID:
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 0);
                    razer_usb_send(&report);
                    break;

                /*-------------------------------------------------*\
                | These devices use standard matrix reports         |
                \*-------------------------------------------------*/
                case RAZER_ORBWEAVER_CHROMA_PID:
                    report                  = razer_create_mode_custom_standard_matrix_report(RAZER_STORAGE_NO_SAVE);
                    razer_usb_send(&report);
                    break;
            }
            break;
    }
}

void RazerController::razer_set_mode_none()
{
    razer_report report;

    switch(matrix_type)
    {
        case RAZER_MATRIX_TYPE_STANDARD:
        case RAZER_MATRIX_TYPE_LINEAR:
            report                          = razer_create_mode_none_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED:
        case RAZER_MATRIX_TYPE_EXTENDED_ARGB:
            report                          = razer_create_mode_none_extended_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_CUSTOM:
            unsigned char rgb_data[6];
            rgb_data[0]             = 0x00;
            rgb_data[1]             = 0x00;
            rgb_data[2]             = 0x00;
            rgb_data[3]             = 0x00;
            rgb_data[4]             = 0x00;
            rgb_data[5]             = 0x00;

            switch(dev_pid)
            {
                /*-------------------------------------------------*\
                | These devices use individual LED effect reports   |
                \*-------------------------------------------------*/
                case RAZER_TARTARUS_CHROMA_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, rgb_data);
                    razer_usb_send(&report);
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, 0);
                    razer_usb_send(&report);
                    break;

                case RAZER_DEATHADDER_CHROMA_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, rgb_data);
                    razer_usb_send(&report);
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 0);
                    razer_usb_send(&report);

                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_LOGO, &rgb_data[3]);
                    razer_usb_send(&report);
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_LOGO, 0);
                    razer_usb_send(&report);
                break;

                case RAZER_NAGA_EPIC_CHROMA_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, rgb_data);
                    razer_usb_send(&report);

                    std::this_thread::sleep_for(1ms);

                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 0);
                    razer_usb_send(&report);

                    std::this_thread::sleep_for(1ms);

                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, &rgb_data[3]);
                    razer_usb_send(&report);

                    std::this_thread::sleep_for(1ms);

                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, 0);
                    razer_usb_send(&report);
                    break;

                case RAZER_MAMBA_2012_WIRED_PID:
                case RAZER_MAMBA_2012_WIRELESS_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, rgb_data);
                    razer_usb_send(&report);
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 0);
                    razer_usb_send(&report);
                    break;

                /*-------------------------------------------------*\
                | These devices use standard matrix reports         |
                \*-------------------------------------------------*/
                case RAZER_ORBWEAVER_CHROMA_PID:
                    report                  = razer_create_mode_none_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id);
                    razer_usb_send(&report);
                    break;
            }
            break;
    }
}

void RazerController::razer_set_mode_spectrum_cycle()
{
    razer_report report;

    switch(matrix_type)
    {
        case RAZER_MATRIX_TYPE_STANDARD:
        case RAZER_MATRIX_TYPE_LINEAR:
            report                          = razer_create_mode_spectrum_cycle_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED:
        case RAZER_MATRIX_TYPE_EXTENDED_ARGB:
            report                          = razer_create_mode_spectrum_cycle_extended_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_CUSTOM:
            switch(dev_pid)
            {
                /*-------------------------------------------------*\
                | These devices use individual LED effect reports   |
                \*-------------------------------------------------*/
                case RAZER_TARTARUS_CHROMA_PID:
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, 4);
                    razer_usb_send(&report);
                    break;

                case RAZER_DEATHADDER_CHROMA_PID:
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 4);
                    razer_usb_send(&report);

                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_LOGO, 4);
                    razer_usb_send(&report);
                break;

                case RAZER_NAGA_EPIC_CHROMA_PID:
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 4);
                    razer_usb_send(&report);

                    std::this_thread::sleep_for(1ms);

                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, 4);
                    razer_usb_send(&report);
                    break;

                case RAZER_MAMBA_2012_WIRED_PID:
                case RAZER_MAMBA_2012_WIRELESS_PID:
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 4);
                    razer_usb_send(&report);
                    break;

                /*-------------------------------------------------*\
                | These devices use standard matrix reports         |
                \*-------------------------------------------------*/
                case RAZER_ORBWEAVER_CHROMA_PID:
                    report                  = razer_create_mode_spectrum_cycle_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id);
                    razer_usb_send(&report);
                    break;
            }
            break;
    }
}

void RazerController::razer_set_mode_static(unsigned char red, unsigned char grn, unsigned char blu)
{
    razer_report report;

    switch(matrix_type)
    {
        case RAZER_MATRIX_TYPE_STANDARD:
        case RAZER_MATRIX_TYPE_LINEAR:
            report                          = razer_create_mode_static_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, red, grn, blu);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED:
        case RAZER_MATRIX_TYPE_EXTENDED_ARGB:
            report                          = razer_create_mode_static_extended_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, red, grn, blu);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_CUSTOM:
            unsigned char rgb_data[6];
            rgb_data[0]             = red;
            rgb_data[1]             = grn;
            rgb_data[2]             = blu;
            rgb_data[3]             = red;
            rgb_data[4]             = grn;
            rgb_data[5]             = blu;

            switch(dev_pid)
            {
                /*-------------------------------------------------*\
                | These devices use individual LED effect reports   |
                \*-------------------------------------------------*/
                case RAZER_TARTARUS_CHROMA_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, rgb_data);
                    razer_usb_send(&report);
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, 0);
                    razer_usb_send(&report);
                    break;

                case RAZER_DEATHADDER_CHROMA_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, rgb_data);
                    razer_usb_send(&report);
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 0);
                    razer_usb_send(&report);

                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_LOGO, &rgb_data[3]);
                    razer_usb_send(&report);
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_LOGO, 0);
                    razer_usb_send(&report);
                break;

                case RAZER_NAGA_EPIC_CHROMA_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, rgb_data);
                    razer_usb_send(&report);

                    std::this_thread::sleep_for(1ms);

                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 0);
                    razer_usb_send(&report);

                    std::this_thread::sleep_for(1ms);

                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, &rgb_data[3]);
                    razer_usb_send(&report);

                    std::this_thread::sleep_for(1ms);

                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_BACKLIGHT, 0);
                    razer_usb_send(&report);
                    break;

                case RAZER_MAMBA_2012_WIRED_PID:
                case RAZER_MAMBA_2012_WIRELESS_PID:
                    report                  = razer_create_set_led_rgb_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, rgb_data);
                    razer_usb_send(&report);
                    report                  = razer_create_set_led_effect_report(RAZER_STORAGE_NO_SAVE, RAZER_LED_ID_SCROLL_WHEEL, 0);
                    razer_usb_send(&report);
                    break;

                /*-------------------------------------------------*\
                | These devices use standard matrix reports         |
                \*-------------------------------------------------*/
                case RAZER_ORBWEAVER_CHROMA_PID:
                    report                  = razer_create_mode_static_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, red, grn, blu);
                    razer_usb_send(&report);
                    break;
            }
            break;
    }
}

void RazerController::razer_set_mode_wave(unsigned char direction)
{
    razer_report report;

    switch(matrix_type)
    {
        case RAZER_MATRIX_TYPE_STANDARD:
        case RAZER_MATRIX_TYPE_LINEAR:
            report                          = razer_create_mode_wave_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, direction);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_EXTENDED:
        case RAZER_MATRIX_TYPE_EXTENDED_ARGB:
            report                          = razer_create_mode_wave_extended_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, direction);
            razer_usb_send(&report);
            break;

        case RAZER_MATRIX_TYPE_CUSTOM:
            switch(dev_pid)
            {
                /*-------------------------------------------------*\
                | These devices use standard matrix reports         |
                \*-------------------------------------------------*/
                case RAZER_ORBWEAVER_CHROMA_PID:
                    report                  = razer_create_mode_wave_standard_matrix_report(RAZER_STORAGE_NO_SAVE, dev_led_id, direction);
                    razer_usb_send(&report);
                    break;
            }
            break;
    }
}

/*---------------------------------------------------------------------------------*\
| USB transfer functions                                                            |
\*---------------------------------------------------------------------------------*/

int RazerController::razer_usb_receive(razer_report* report)
{
    return hid_get_feature_report(dev, (unsigned char*)report, sizeof(*report));
}

int RazerController::razer_usb_send(razer_report* report)
{
    report->crc = razer_calculate_crc(report);

    DeviceGuardLock _ = guard_manager_ptr->AwaitExclusiveAccess();
    return hid_send_feature_report(dev, (unsigned char*)report, sizeof(*report));
}

int RazerController::razer_usb_send_argb(razer_argb_report* report)
{
    DeviceGuardLock _ = guard_manager_ptr->AwaitExclusiveAccess();
    return hid_send_feature_report(dev_argb, (unsigned char*)report, sizeof(*report));
}
