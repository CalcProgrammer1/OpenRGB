/*-------------------------------------------------------------------*\
|  XPGSpectrixS40GController.cpp	                                  |
|                                                                     |
|  Driver for XPG's Spectrix S40G NVMe                                |
|                                                                     |
|  NicolasNewman          25th Mar 2021                               |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "XPGSpectrixS40GController.h"
#include <malloc.h>
#include <cstring>
#include "LogManager.h"

#ifdef _WIN32
    #include <windows.h>
    #include <fileapi.h>
    #include <winioctl.h>
    #include <nvme.h>
#else

#endif

#ifdef _WIN32
    int XPGSpectrixS40GController::SetHandle(wchar_t buff[MAX_PATH]) {
        wchar_t path[MAX_PATH];
        wcscpy(path, L"\\\\?\\");
        wcsncat(path, buff, MAX_PATH - 4);
		for (size_t i = 0; i < MAX_PATH && path[i] != '\0'; i++) {
			path[i] = tolower(path[i]);
		}
        wprintf(L"%s\n", path);
        hDevice = CreateFileW(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)0x0, OPEN_EXISTING, 0x0, (HANDLE)0x0);
        if (hDevice == INVALID_HANDLE_VALUE) {
                return 0;
        }
        return 1;
    }
#else

#endif

/*-----------------------------------------*\
|  AsusAuraSMBusController.cpp              |
|                                           |
|  Driver for ASUS Aura RGB lighting        |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/19/2018   |
\*-----------------------------------------*/
#include <cstring>

static const char* aura_channels[] =                /* Aura channel strings                 */
{
    "Audio",
    "Backplate",
    "Back I/O",
    "Center",
    "Center",
    "DRAM",
    "PCIe",
    "RGB Header",
    "RGB Header 2",
    "RGB Header",
    "Unknown",
};

XPGSpectrixS40GController::XPGSpectrixS40GController(aura_dev_id dev)
{
    this->dev = dev;
    direct_reg = AURA_REG_COLORS_DIRECT_V2;
    effect_reg = AURA_REG_COLORS_EFFECT_V2;
    channel_cfg = AURA_CONFIG_CHANNEL_V2;
    led_count = 8;
}

XPGSpectrixS40GController::~XPGSpectrixS40GController()
{

}

std::string XPGSpectrixS40GController::GetDeviceName()
{
    return("SSD");
}

std::string XPGSpectrixS40GController::GetDeviceLocation()
{
    return("I2C: SSD");
}

unsigned char XPGSpectrixS40GController::GetChannel(unsigned int led)
{
    return(0);
}

const char * XPGSpectrixS40GController::GetChannelName(unsigned int led)
{
    switch (config_table[channel_cfg + led])
    {
    case (unsigned char)AURA_LED_CHANNEL_AUDIO:
        return(aura_channels[0]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_BACKPLATE:
        return(aura_channels[1]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_BACK_IO:
        return(aura_channels[2]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_CENTER:
        return(aura_channels[3]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_CENTER_START:
        return(aura_channels[4]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_DRAM:
        return(aura_channels[5]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_PCIE:
        return(aura_channels[6]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_RGB_HEADER:
        return(aura_channels[7]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_RGB_HEADER_2:
        return(aura_channels[8]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_RGB_HEADER_3:
        return(aura_channels[9]);
        break;

    default:
        return(aura_channels[10]);
        break;
    }
}

unsigned int XPGSpectrixS40GController::GetLEDCount()
{
    return(led_count);
}

unsigned char XPGSpectrixS40GController::GetLEDRed(unsigned int led)
{
    return(AuraRegisterRead(direct_reg + ( 3 * led )));
}

unsigned char XPGSpectrixS40GController::GetLEDGreen(unsigned int led)
{
    return(AuraRegisterRead(direct_reg + ( 3 * led ) + 2));
}

unsigned char XPGSpectrixS40GController::GetLEDBlue(unsigned int led)
{
    return(AuraRegisterRead(direct_reg + ( 3 * led ) + 1));
}

void XPGSpectrixS40GController::SaveMode()
{
    AuraRegisterWrite(AURA_REG_APPLY, AURA_SAVE_VAL);
}

void XPGSpectrixS40GController::SetAllColorsDirect(RGBColor* colors)
{
    unsigned char* color_buf = new unsigned char[led_count * 3];

    for (unsigned int i = 0; i < (led_count * 3); i += 3)
    {
        color_buf[i + 0] = RGBGetRValue(colors[i / 3]);
        color_buf[i + 1] = RGBGetBValue(colors[i / 3]);
        color_buf[i + 2] = RGBGetGValue(colors[i / 3]);
    }

    AuraRegisterWriteBlock(direct_reg, color_buf, led_count * 3);

    delete color_buf;
}

void XPGSpectrixS40GController::SetAllColorsEffect(RGBColor* colors)
{
    unsigned char* color_buf = new unsigned char[led_count * 3];

    for (unsigned int i = 0; i < (led_count * 3); i += 3)
    {
        color_buf[i + 0] = RGBGetRValue(colors[i / 3]);
        color_buf[i + 1] = RGBGetBValue(colors[i / 3]);
        color_buf[i + 2] = RGBGetGValue(colors[i / 3]);
    }

    AuraRegisterWriteBlock(effect_reg, color_buf, led_count * 3);

    AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);

    delete[] color_buf;
}

void XPGSpectrixS40GController::SetDirect(unsigned char direct)
{
    AuraRegisterWrite(AURA_REG_DIRECT, direct);
    AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
}

void XPGSpectrixS40GController::SetLEDColorDirect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };

    AuraRegisterWriteBlock(direct_reg + ( 3 * led ), colors, 3);
}

void XPGSpectrixS40GController::SetLEDColorEffect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };

    AuraRegisterWriteBlock(effect_reg + (3 * led), colors, 3);

    AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
}

void XPGSpectrixS40GController::SetMode(unsigned char mode, unsigned char speed, unsigned char direction)
{
    AuraRegisterWrite(AURA_REG_MODE,      mode);
    AuraRegisterWrite(AURA_REG_SPEED,     speed);
    AuraRegisterWrite(AURA_REG_DIRECTION, direction);
    AuraRegisterWrite(AURA_REG_APPLY,     AURA_APPLY_VAL);
}

void XPGSpectrixS40GController::AuraUpdateDeviceName()
{
    for (int i = 0; i < 16; i++)
    {
        device_name[i] = AuraRegisterRead(AURA_REG_DEVICE_NAME + i);
    }
}

unsigned char XPGSpectrixS40GController::AuraRegisterRead(aura_register reg)
{
    return(0);
}

void XPGSpectrixS40GController::AuraRegisterWrite(aura_register reg, unsigned char val)
{
    if(hDevice != INVALID_HANDLE_VALUE)
    {
        /*-----------------------------------------------------------------------------*\
        | Create buffer to hold STORAGE_PROTOCOL_COMMAND                                |
        | Size must be enough for the STORAGE_PROTOCOL_COMMAND struct plus the command  |
        | data.  Subtract sizeof(DWORD) as the Command field in the structure overlaps  |
        | the actual command data.                                                      |
        \*-----------------------------------------------------------------------------*/
        unsigned char buffer[sizeof(STORAGE_PROTOCOL_COMMAND) + (sizeof(DWORD) * 34) - sizeof(DWORD)];

        /*-----------------------------------------------------------------------------*\
        | Create STORAGE_PROTOCOL_COMMAND pointer and point it to the buffer            |
        \*-----------------------------------------------------------------------------*/
        PSTORAGE_PROTOCOL_COMMAND command       = (PSTORAGE_PROTOCOL_COMMAND)buffer;

        /*-----------------------------------------------------------------------------*\
        | Fill in STORAGE_PROTOCOL_COMMAND structure                                    |
        \*-----------------------------------------------------------------------------*/
        command->Version                        = STORAGE_PROTOCOL_STRUCTURE_VERSION;
        command->Length                         = sizeof(STORAGE_PROTOCOL_COMMAND);
        command->ProtocolType                   = ProtocolTypeNvme;
        command->Flags                          = STORAGE_PROTOCOL_COMMAND_FLAG_ADAPTER_REQUEST;
        command->ReturnStatus                   = 0x00000000;
        command->ErrorCode                      = 0x00000000;
        command->CommandLength                  = STORAGE_PROTOCOL_COMMAND_LENGTH_NVME;
        command->ErrorInfoLength                = 0x00000040;
        command->DataToDeviceTransferLength     = 0x00000001;
        command->DataFromDeviceTransferLength   = 0x00000000;
        command->TimeOutValue                   = 0x00000001;
        command->ErrorInfoOffset                = 0x00000090;
        command->DataToDeviceBufferOffset       = 0x000000D0;
        command->DataFromDeviceBufferOffset     = 0x00000000;
        command->CommandSpecific                = STORAGE_PROTOCOL_SPECIFIC_NVME_ADMIN_COMMAND;
        command->Reserved0                      = 0x00000000;
        command->FixedProtocolReturnData        = 0x00000000;
        command->Reserved1[0]                   = 0x00000000;
        command->Reserved1[1]                   = 0x00000000;
        command->Reserved1[2]                   = 0x00000000;

        /*-----------------------------------------------------------------------------*\
        | Create ENE Register Write command, filling in the appropriate register and    |
        | value                                                                         |
        \*-----------------------------------------------------------------------------*/
        PNVME_COMMAND CommandValue              = (PNVME_COMMAND)command->Command;

        unsigned short corrected_reg            = ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF);

        CommandValue->CDW0.OPC                  = 0xFB;
        CommandValue->NSID                      = 0x00000031;
        CommandValue->u.GENERAL.CDW12           = (corrected_reg << 16) | (dev << 1);
        CommandValue->u.GENERAL.CDW13           = 0x01100001;

        DWORD ExtraValue[18] = { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                                 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000 };

        ExtraValue[16]                          = val;

        /*-----------------------------------------------------------------------------*\
        | Copy the ENE Register Write extra data into the STORAGE_PROTOCOL_COMMAND      |
        | buffer                                                                        |
        \*-----------------------------------------------------------------------------*/
        memcpy(&command->Command + sizeof(NVME_COMMAND), ExtraValue, sizeof(ExtraValue));

        /*-----------------------------------------------------------------------------*\
        | Send the STORAGE_PROTOCOL_COMMAND to the device                               |
        \*-----------------------------------------------------------------------------*/
        DeviceIoControl(hDevice, IOCTL_STORAGE_PROTOCOL_COMMAND, buffer, sizeof(buffer), buffer, sizeof(buffer), 0x0, (LPOVERLAPPED)0x0);
    }
}

void XPGSpectrixS40GController::AuraRegisterWriteBlock(aura_register reg, unsigned char * data, unsigned char sz)
{
    if(hDevice != INVALID_HANDLE_VALUE)
    {
        /*-----------------------------------------------------------------------------*\
        | Create buffer to hold STORAGE_PROTOCOL_COMMAND                                |
        | Size must be enough for the STORAGE_PROTOCOL_COMMAND struct plus the command  |
        | data.  Subtract sizeof(DWORD) as the Command field in the structure overlaps  |
        | the actual command data.                                                      |
        \*-----------------------------------------------------------------------------*/
        unsigned char buffer[sizeof(STORAGE_PROTOCOL_COMMAND) + (sizeof(DWORD) * 39) - sizeof(DWORD)];

        /*-----------------------------------------------------------------------------*\
        | Create STORAGE_PROTOCOL_COMMAND pointer and point it to the buffer            |
        \*-----------------------------------------------------------------------------*/
        PSTORAGE_PROTOCOL_COMMAND command       = (PSTORAGE_PROTOCOL_COMMAND)buffer;

        /*-----------------------------------------------------------------------------*\
        | Fill in STORAGE_PROTOCOL_COMMAND structure                                    |
        \*-----------------------------------------------------------------------------*/
        command->Version                        = STORAGE_PROTOCOL_STRUCTURE_VERSION;
        command->Length                         = sizeof(STORAGE_PROTOCOL_COMMAND);
        command->ProtocolType                   = ProtocolTypeNvme;
        command->Flags                          = STORAGE_PROTOCOL_COMMAND_FLAG_ADAPTER_REQUEST;
        command->ReturnStatus                   = 0x00000000;
        command->ErrorCode                      = 0x00000000;
        command->CommandLength                  = STORAGE_PROTOCOL_COMMAND_LENGTH_NVME;
        command->ErrorInfoLength                = 0x00000040;
        command->DataToDeviceTransferLength     = sz;
        command->DataFromDeviceTransferLength   = 0x00000000;
        command->TimeOutValue                   = 0x00000001;
        command->ErrorInfoOffset                = 0x00000090;
        command->DataToDeviceBufferOffset       = 0x000000D0;
        command->DataFromDeviceBufferOffset     = 0x00000000;
        command->CommandSpecific                = STORAGE_PROTOCOL_SPECIFIC_NVME_ADMIN_COMMAND;
        command->Reserved0                      = 0x00000000;
        command->FixedProtocolReturnData        = 0x00000000;
        command->Reserved1[0]                   = 0x00000000;
        command->Reserved1[1]                   = 0x00000000;
        command->Reserved1[2]                   = 0x00000000;

        /*-----------------------------------------------------------------------------*\
        | Create ENE Register Write Block command, filling in the appropriate register  |
        | and value                                                                     |
        \*-----------------------------------------------------------------------------*/
        PNVME_COMMAND CommandValue              = (PNVME_COMMAND)command->Command;

        unsigned short corrected_reg            = ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF);

        CommandValue->CDW0.OPC                  = 0xFB;
        CommandValue->NSID                      = 0x00000031;
        CommandValue->u.GENERAL.CDW12           = (corrected_reg << 16) | (dev << 1);
        CommandValue->u.GENERAL.CDW13           = 0x03100000 | sz;

        DWORD ExtraValue[23] = { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                                 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                                 0x00000000, 0x00000000, 0x00000000 };

        memcpy(&ExtraValue[16], data, sz);

        /*-----------------------------------------------------------------------------*\
        | Copy the ENE Register Write Block extra data into the                         |
        | STORAGE_PROTOCOL_COMMAND buffer                                               |
        \*-----------------------------------------------------------------------------*/
        memcpy(&command->Command + sizeof(NVME_COMMAND), ExtraValue, sizeof(ExtraValue));

        /*-----------------------------------------------------------------------------*\
        | Send the STORAGE_PROTOCOL_COMMAND to the device                               |
        \*-----------------------------------------------------------------------------*/
        DeviceIoControl(hDevice, IOCTL_STORAGE_PROTOCOL_COMMAND, buffer, sizeof(buffer), buffer, sizeof(buffer), 0x0, (LPOVERLAPPED)0x0);
    }
}

