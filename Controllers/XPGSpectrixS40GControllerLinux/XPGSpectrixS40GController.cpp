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
#include <sys/ioctl.h>

/*---------------------------------------------------------------------*\
| Functions for submitting NVME admin passthrough command taken from    |
| libnvme: https://github.com/linux-nvme/libnvme                        |
\*---------------------------------------------------------------------*/

#define NVME_IOCTL_ADMIN_CMD	_IOWR('N', 0x41, struct nvme_passthru_cmd)

static int nvme_submit_passthru(int fd, unsigned long ioctl_cmd,
				struct nvme_passthru_cmd *cmd, uint32_t *result)
{
	int err = ioctl(fd, ioctl_cmd, cmd);

	if (err >= 0 && result)
		*result = cmd->result;
	return err;
}

static int nvme_passthru(int fd, unsigned long ioctl_cmd, uint8_t opcode,
			 uint8_t flags, uint16_t rsvd, uint32_t nsid, uint32_t cdw2,
			 uint32_t cdw3, uint32_t cdw10, uint32_t cdw11, uint32_t cdw12,
			 uint32_t cdw13, uint32_t cdw14, uint32_t cdw15, uint32_t data_len,
			 void *data, uint32_t metadata_len, void *metadata,
			 uint32_t timeout_ms, uint32_t *result)
{
	struct nvme_passthru_cmd cmd = {
		.opcode		= opcode,
		.flags		= flags,
		.rsvd1		= rsvd,
		.nsid		= nsid,
		.cdw2		= cdw2,
		.cdw3		= cdw3,
		.metadata	= (uint64_t)(uintptr_t)metadata,
		.addr		= (uint64_t)(uintptr_t)data,
		.metadata_len	= metadata_len,
		.data_len	= data_len,
		.cdw10		= cdw10,
		.cdw11		= cdw11,
		.cdw12		= cdw12,
		.cdw13		= cdw13,
		.cdw14		= cdw14,
		.cdw15		= cdw15,
		.timeout_ms	= timeout_ms,
	};

	return nvme_submit_passthru(fd, ioctl_cmd, &cmd, result);
}

int nvme_admin_passthru(int fd, uint8_t opcode, uint8_t flags, uint16_t rsvd,
			uint32_t nsid, uint32_t cdw2, uint32_t cdw3, uint32_t cdw10,
			uint32_t cdw11, uint32_t cdw12, uint32_t cdw13, uint32_t cdw14,
			uint32_t cdw15, uint32_t data_len, void *data,
			uint32_t metadata_len, void *metadata, uint32_t timeout_ms,
			uint32_t *result)
{
	return nvme_passthru(fd, NVME_IOCTL_ADMIN_CMD, opcode, flags, rsvd,
			     nsid, cdw2, cdw3, cdw10, cdw11, cdw12, cdw13,
			     cdw14, cdw15, data_len, data, metadata_len,
			     metadata, timeout_ms, result);
}

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

XPGSpectrixS40GController::XPGSpectrixS40GController(int fd, aura_dev_id dev)
{
    nvme_fd = fd;
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
  	struct nvme_passthru_cmd cfg;

    memset(&cfg, 0, sizeof(nvme_passthru_cmd));

    unsigned short corrected_reg    = ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF);

    cfg.opcode                      = 0xFB;
    cfg.nsid                        = 0x00000031;
    cfg.cdw12                       = (corrected_reg << 16) | (dev << 1);
    cfg.cdw13                       = 0x01100001;
    cfg.data_len                    = 1;

    unsigned char data[1];

    data[0]                         = val;

    unsigned char   metadata[1];
    unsigned int    result;

    /*-----------------------------------------------------------------------------*\
    | Send the command to the device                                                |
    \*-----------------------------------------------------------------------------*/
    nvme_admin_passthru(nvme_fd, cfg.opcode, cfg.flags, cfg.rsvd1,
				cfg.nsid, cfg.cdw2, cfg.cdw3, cfg.cdw10,
				cfg.cdw11, cfg.cdw12, cfg.cdw13, cfg.cdw14,
				cfg.cdw15, cfg.data_len, data, cfg.metadata_len,
				metadata, cfg.timeout_ms, &result);
}

void XPGSpectrixS40GController::AuraRegisterWriteBlock(aura_register reg, unsigned char * data, unsigned char sz)
{
  	struct nvme_passthru_cmd cfg;

    memset(&cfg, 0, sizeof(nvme_passthru_cmd));

    unsigned short corrected_reg    = ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF);

    cfg.opcode                      = 0xFB;
    cfg.nsid                        = 0x00000031;
    cfg.cdw12                       = (corrected_reg << 16) | (dev << 1);
    cfg.cdw13                       = 0x03100000 | sz;
    cfg.data_len                    = sz;

    unsigned char   metadata[1];
    unsigned int    result;

    /*-----------------------------------------------------------------------------*\
    | Send the command to the device                                                |
    \*-----------------------------------------------------------------------------*/
    nvme_admin_passthru(nvme_fd, cfg.opcode, cfg.flags, cfg.rsvd1,
				cfg.nsid, cfg.cdw2, cfg.cdw3, cfg.cdw10,
				cfg.cdw11, cfg.cdw12, cfg.cdw13, cfg.cdw14,
				cfg.cdw15, cfg.data_len, data, cfg.metadata_len,
				metadata, cfg.timeout_ms, &result);
}

