/*---------------------------------------------------------*\
| ENESMBusInterface_SpectrixS40G_Linux.cpp                  |
|                                                           |
|   ENE SMBus interface for XPG Spectrix S40G (Linux)       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstdint>
#include <cstring>
#include <sys/ioctl.h>
#include "ENESMBusInterface_SpectrixS40G_Linux.h"

/*---------------------------------------------------------------------*\
| Functions for submitting NVME admin passthrough command taken from    |
| libnvme: https://github.com/linux-nvme/libnvme                        |
\*---------------------------------------------------------------------*/

#define NVME_IOCTL_ADMIN_CMD	_IOWR('N', 0x41, struct nvme_passthru_cmd)

struct nvme_passthru_cmd
{
    uint8_t     opcode;
    uint8_t     flags;
	uint16_t    rsvd1;
	uint32_t    nsid;
	uint32_t    cdw2;
	uint32_t    cdw3;
	uint64_t    metadata;
	uint64_t    addr;
	uint32_t    metadata_len;
	uint32_t    data_len;
	uint32_t    cdw10;
	uint32_t    cdw11;
	uint32_t    cdw12;
	uint32_t    cdw13;
	uint32_t    cdw14;
	uint32_t    cdw15;
	uint32_t    timeout_ms;
	uint32_t    result;
};

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
        .result     = 0,
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

/*---------------------------------------------------------------------*\
| ENESMBusInterface_SpectrixS40G implementation                         |
\*---------------------------------------------------------------------*/

ENESMBusInterface_SpectrixS40G::ENESMBusInterface_SpectrixS40G(int fd, char* path)
{
    this->nvme_fd = fd;
	this->path    = path;
}

ENESMBusInterface_SpectrixS40G::~ENESMBusInterface_SpectrixS40G()
{

}

ene_interface_type ENESMBusInterface_SpectrixS40G::GetInterfaceType()
{
    return(ENE_INTERFACE_TYPE_SPECTRIX_S40G);
}

std::string ENESMBusInterface_SpectrixS40G::GetLocation()
{
    return("NVMe: " + path);
}

int ENESMBusInterface_SpectrixS40G::GetMaxBlock()
{
    return(24);
}

unsigned char ENESMBusInterface_SpectrixS40G::ENERegisterRead(ene_dev_id dev, ene_register reg)
{
  	struct nvme_passthru_cmd cfg;

    memset(&cfg, 0, sizeof(nvme_passthru_cmd));

    unsigned short corrected_reg    = ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF);

    cfg.opcode                      = 0xFA;
    cfg.cdw12                       = (corrected_reg << 16) | (dev << 1);
    cfg.cdw13                       = 0x81100001;
    cfg.data_len                    = 1;

    unsigned char   data[1];
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

    return(data[0]);
}

void ENESMBusInterface_SpectrixS40G::ENERegisterWrite(ene_dev_id dev, ene_register reg, unsigned char val)
{
  	struct nvme_passthru_cmd cfg;

    memset(&cfg, 0, sizeof(nvme_passthru_cmd));

    unsigned short corrected_reg    = ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF);

    cfg.opcode                      = 0xFB;
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

void ENESMBusInterface_SpectrixS40G::ENERegisterWriteBlock(ene_dev_id dev, ene_register reg, unsigned char * data, unsigned char sz)
{
  	struct nvme_passthru_cmd cfg;

    memset(&cfg, 0, sizeof(nvme_passthru_cmd));

    unsigned short corrected_reg    = ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF);

    cfg.opcode                      = 0xFB;
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
