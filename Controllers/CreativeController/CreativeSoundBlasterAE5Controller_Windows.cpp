/*---------------------------------------------------------*\
| CreativeSoundBlasterAE5Controller_Windows.cpp             |
|                                                           |
|   Driver for Creative SoundBlaster AE-5 (Windows)         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <windows.h>
#include <setupapi.h>
#include "CreativeSoundBlasterAE5Controller_Windows.h"
#include "LogManager.h"
#include <algorithm>

/*---------------------------------------------------------*\
| Lifecycle                                                 |
\*---------------------------------------------------------*/

CreativeSoundBlasterAE5Controller_Windows::CreativeSoundBlasterAE5Controller_Windows()
{
    name         = "Creative SoundBlaster AE-5";
    location     = "";
    device_found = false;
    device_handle = INVALID_HANDLE_VALUE;
    device_opened = false;
    external_led_count = 0;
    led_mutex = CreateMutexA(NULL, FALSE, "OpenRGB_AE5_LED_Mutex");
}

CreativeSoundBlasterAE5Controller_Windows::~CreativeSoundBlasterAE5Controller_Windows()
{
    CloseDevice();
    if(led_mutex != NULL)
    {
        CloseHandle(led_mutex);
        led_mutex = NULL;
    }
}

bool CreativeSoundBlasterAE5Controller_Windows::Initialize()
{
    if(!FindDevice())
    {
        return false;
    }

    hdaudio_device_path = FindHDAudioDevicePath();
    if(hdaudio_device_path.empty())
    {
        LOG_WARNING("[%s] Failed to find HDAudio device path", name.c_str());
        return false;
    }

    return OpenDevice();
}

/*---------------------------------------------------------*\
| Device Discovery                                          |
\*---------------------------------------------------------*/

bool CreativeSoundBlasterAE5Controller_Windows::FindDevice()
{
    LOG_DEBUG("[%s] Looking for vendor 0x%04X, device 0x%04X", name.c_str(), AE5_VENDOR_ID, AE5_DEVICE_ID);

    HDEVINFO device_info_set = SetupDiGetClassDevs(
        NULL,                    // No class GUID
        TEXT("PCI"),             // Enumerator
        NULL,                    // No parent window
        DIGCF_PRESENT | DIGCF_ALLCLASSES  // Only present devices
    );

    if(device_info_set == INVALID_HANDLE_VALUE)
    {
        LOG_ERROR("[%s] SetupDiGetClassDevs failed: %lu", name.c_str(), GetLastError());
        return false;
    }

    SP_DEVINFO_DATA device_info_data;
    device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);

    for(DWORD device_index = 0; SetupDiEnumDeviceInfo(device_info_set, device_index, &device_info_data); device_index++)
    {
        TCHAR hardware_id[256];
        if(!SetupDiGetDeviceRegistryProperty(device_info_set, &device_info_data, SPDRP_HARDWAREID,
                                              NULL, (PBYTE)hardware_id, sizeof(hardware_id), NULL))
        {
            continue;
        }

        std::string hw_id_str(hardware_id);
        std::transform(hw_id_str.begin(), hw_id_str.end(), hw_id_str.begin(), ::toupper);

        if(hw_id_str.find("PCI\\VEN_1102&DEV_0012") != std::string::npos)
        {
            // Check subsystem ID to determine AE-5 variant
            // Format is SUBSYS_{DEVICE_ID}{VENDOR_ID}
            if(hw_id_str.find("SUBSYS_01911102") != std::string::npos)
            {
                name = "Creative SoundBlaster AE-5 Plus";
            }
            else if(hw_id_str.find("SUBSYS_00511102") != std::string::npos)
            {
                name = "Creative SoundBlaster AE-5";
            }
            else
            {
                name = "Creative SoundBlaster AE-5";
                LOG_WARNING("[%s] Unknown subsystem variant found in hardware ID: %s", name.c_str(), hardware_id);
                LOG_WARNING("[%s] Please report this to @eclipse_sol84 in the OpenRGB Discord!", name.c_str());
            }

            LOG_INFO("[%s] Found matching device: %s", name.c_str(), hardware_id);

            TCHAR location_info[256];
            if(SetupDiGetDeviceRegistryProperty(device_info_set, &device_info_data, SPDRP_LOCATION_INFORMATION,
                                                 NULL, (PBYTE)location_info, sizeof(location_info), NULL))
            {
                location = "PCI: " + std::string(location_info);
            }
            else
            {
                location = "PCI: " + hw_id_str;
            }

            device_found = true;
            SetupDiDestroyDeviceInfoList(device_info_set);

            LOG_INFO("[%s] Device successfully detected at %s", name.c_str(), location.c_str());
            return true;
        }
    }

    SetupDiDestroyDeviceInfoList(device_info_set);
    LOG_WARNING("[%s] No matching device found", name.c_str());
    return false;
}

std::string CreativeSoundBlasterAE5Controller_Windows::FindHDAudioDevicePath()
{
    LOG_INFO("[%s] Searching for HDAudio device path...", name.c_str());

    HKEY device_classes_key;
    LONG result = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                                "SYSTEM\\CurrentControlSet\\Control\\DeviceClasses",
                                0, KEY_READ, &device_classes_key);

    if(result != ERROR_SUCCESS)
    {
        LOG_ERROR("[%s] Failed to open DeviceClasses registry key: %lu", name.c_str(), result);
        return "";
    }

    DWORD guid_index = 0;
    CHAR guid_name[256];
    DWORD guid_name_size;

    // Enumerate all GUID folders in DeviceClasses
    while(true)
    {
        guid_name_size = sizeof(guid_name);
        result = RegEnumKeyExA(device_classes_key, guid_index, guid_name, &guid_name_size,
                              NULL, NULL, NULL, NULL);

        if(result != ERROR_SUCCESS) break;

        HKEY guid_key;
        result = RegOpenKeyExA(device_classes_key, guid_name, 0, KEY_READ, &guid_key);
        if(result != ERROR_SUCCESS)
        {
            guid_index++;
            continue;
        }

        // Enumerate device instances under this GUID
        DWORD device_index = 0;
        CHAR device_name[512];
        DWORD device_name_size;

        while(true)
        {
            device_name_size = sizeof(device_name);
            result = RegEnumKeyExA(guid_key, device_index, device_name, &device_name_size,
                                  NULL, NULL, NULL, NULL);

            if(result != ERROR_SUCCESS) break;

            std::string device_str(device_name);
            std::transform(device_str.begin(), device_str.end(), device_str.begin(), ::toupper);

            if(device_str.find("VEN_1102&DEV_0011") != std::string::npos)
            {

                // Check if this device has a #GPDHDA subkey
                HKEY device_key;
                result = RegOpenKeyExA(guid_key, device_name, 0, KEY_READ, &device_key);
                if(result == ERROR_SUCCESS)
                {
                    HKEY gpdhda_key;
                    result = RegOpenKeyExA(device_key, "#GPDHDA", 0, KEY_READ, &gpdhda_key);
                    if(result == ERROR_SUCCESS)
                    {
                        LOG_INFO("[%s] Found HDAudio device interface", name.c_str());
                        RegCloseKey(gpdhda_key);
                        RegCloseKey(device_key);

                        // Convert registry path format to device path format
                        std::string device_path(device_name);
                        std::transform(device_path.begin(), device_path.end(), device_path.begin(), ::tolower);

                        if(device_path.substr(0, 4) == "##?#")
                        {
                            device_path = "\\\\?\\" + device_path.substr(4);
                        }

                        device_path += "\\gpdhda";

                        RegCloseKey(guid_key);
                        RegCloseKey(device_classes_key);
                        return device_path;
                    }
                    RegCloseKey(device_key);
                }
            }

            device_index++;
        }

        RegCloseKey(guid_key);
        guid_index++;
    }

    RegCloseKey(device_classes_key);
    LOG_WARNING("[%s] No Creative HDAudio device found in registry", name.c_str());
    return "";
}

bool CreativeSoundBlasterAE5Controller_Windows::OpenDevice()
{
    if(hdaudio_device_path.empty())
    {
        LOG_ERROR("[%s] No HDAudio device path available", name.c_str());
        return false;
    }

    LOG_INFO("[%s] Opening device: %s", name.c_str(), hdaudio_device_path.c_str());

    std::wstring wide_path(hdaudio_device_path.begin(), hdaudio_device_path.end());

    device_handle = CreateFileW(
        wide_path.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if(device_handle == INVALID_HANDLE_VALUE)
    {
        DWORD error = GetLastError();
        LOG_ERROR("[%s] Failed to open device: Error %lu", name.c_str(), error);

        if(error == ERROR_PATH_NOT_FOUND) // Error 3
        {
            LOG_ERROR("[%s] This device requires Creative's official drivers to be installed for RGB control", name.c_str());
            LOG_ERROR("[%s] Please install Creative Sound Blaster Command software or drivers from Creative's website", name.c_str());
        }

        return false;
    }

    LOG_INFO("[%s] Device opened successfully", name.c_str());
    device_opened = true;
    return true;
}

void CreativeSoundBlasterAE5Controller_Windows::CloseDevice()
{
    if(device_handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(device_handle);
        device_handle = INVALID_HANDLE_VALUE;
        device_opened = false;
        LOG_INFO("[%s] Device closed", name.c_str());
    }
}

/*---------------------------------------------------------*\
| Getters                                                   |
\*---------------------------------------------------------*/

std::string CreativeSoundBlasterAE5Controller_Windows::GetDeviceLocation()
{
    return location;
}

std::string CreativeSoundBlasterAE5Controller_Windows::GetDeviceName()
{
    return name;
}

unsigned int CreativeSoundBlasterAE5Controller_Windows::GetLEDCount()
{
    return AE5_INTERNAL_LED_COUNT + external_led_count;
}

unsigned int CreativeSoundBlasterAE5Controller_Windows::GetExternalLEDCount()
{
    return external_led_count;
}

void CreativeSoundBlasterAE5Controller_Windows::SetExternalLEDCount(unsigned int count)
{
    external_led_count = count;
}

/*---------------------------------------------------------*\
| LED Control                                               |
\*---------------------------------------------------------*/

void CreativeSoundBlasterAE5Controller_Windows::SetLEDColors(unsigned char led_count, unsigned char* red_values,
                                                      unsigned char* green_values, unsigned char* blue_values)
{
    if(!device_opened || device_handle == INVALID_HANDLE_VALUE)
    {
        LOG_ERROR("[%s] Device not opened, cannot set LED colors", name.c_str());
        return;
    }

    if(led_count == 0)
    {
        return;
    }

    // Wait for mutex to prevent conflicts with Creative software
    if(led_mutex != NULL)
    {
        DWORD wait_result = WaitForSingleObject(led_mutex, 5000); // 5 second timeout
        if(wait_result != WAIT_OBJECT_0)
        {
            LOG_WARNING("[%s] Failed to acquire LED mutex, proceeding anyway", name.c_str());
        }
    }

    // Internal LEDs (first 5) - bug requires sending command twice?
    if(led_count > 0)
    {
        SendLEDCommand(0x03, AE5_INTERNAL_LED_COUNT, red_values, green_values, blue_values);
        SendLEDCommand(0x03, AE5_INTERNAL_LED_COUNT, red_values, green_values, blue_values);
    }

    // External LEDs
    if(led_count > AE5_INTERNAL_LED_COUNT && external_led_count > 0)
    {
        SendLEDCommand(0x02, external_led_count, red_values + AE5_INTERNAL_LED_COUNT, green_values + AE5_INTERNAL_LED_COUNT, blue_values + AE5_INTERNAL_LED_COUNT);
    }

    // Release mutex
    if(led_mutex != NULL)
    {
        ReleaseMutex(led_mutex);
    }
}

bool CreativeSoundBlasterAE5Controller_Windows::SendLEDCommand(BYTE command_byte, unsigned int led_count_to_set,
                                                              unsigned char* red_values, unsigned char* green_values, unsigned char* blue_values)
{
    AE5_LED_Command cmd;
    memset(&cmd, 0, sizeof(AE5_LED_Command));

    cmd.command = command_byte;
    cmd.packet_led_count = led_count_to_set;

    // Calculate data length
    unsigned int data_length = led_count_to_set * 4;
    cmd.data_length_low = data_length & 0xFF;
    cmd.data_length_high = (data_length >> 8) & 0xFF;

    // Fill LED data
    for(unsigned int i = 0; i < led_count_to_set; i++)
    {
        unsigned int offset = i * 4;

        if(command_byte == 0x02) // External WS2812B LED Strip
        {
            cmd.led_data[offset + 0] = blue_values[i];
            cmd.led_data[offset + 1] = red_values[i];
            cmd.led_data[offset + 2] = green_values[i];
            cmd.led_data[offset + 3] = 0x00;
        }
        else // Internal APA102 LED Strip
        {
            cmd.led_data[offset + 0] = red_values[i];
            cmd.led_data[offset + 1] = green_values[i];
            cmd.led_data[offset + 2] = blue_values[i];
            cmd.led_data[offset + 3] = 0xFF;
        }
    }

    AE5_LED_Command output_cmd;
    memset(&output_cmd, 0, sizeof(AE5_LED_Command));
    DWORD bytes_returned = 0;

    BOOL result = DeviceIoControl(
        device_handle,
        0x77772400, // Custom IOCTL command from Creative Lab's drivers
        &cmd,
        sizeof(AE5_LED_Command),
        &output_cmd,
        sizeof(AE5_LED_Command),
        &bytes_returned,
        NULL
    );

    if(!result)
    {
        DWORD error = GetLastError();
        LOG_ERROR("[%s] DeviceIoControl failed: Error %lu", name.c_str(), error);
        return false;
    }

    return true;
}
