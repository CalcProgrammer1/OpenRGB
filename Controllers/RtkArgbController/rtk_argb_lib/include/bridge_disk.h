/*
 * BSD-3-Clause
 *
 * Copyright (c) 2022 - 2025, Realtek Semiconductor Corp. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   * Neither the name of the Realtek nor the names of its contributors may
 *     be used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _BRIDGE_DISK_H_
#define _BRIDGE_DISK_H_

enum bridge_disk_type {
	DISK_NVME = 0x0,
	DISK_SATA = 0x1,
};

enum bridge_port_type {
	PORT_PCIE = 0x1,
	PORT_SATA = 0x2,
};

enum bridge_disk_state {
	DISK_STATE_NO_LINK = 0x0,
	DISK_STATE_NORMAL = 0x1,
	DISK_STATE_UNCONFIG = 0x2,
	DISK_STATE_LOCK = 0x3,
};

/**
 * disk_type: NVMe or SATA
 * disk_state
 */
struct bridge_disk_status {
	uint32_t disk_type;
	uint32_t disk_state;
	uint32_t port_type;
	uint32_t reserved01;
};

int bridge_get_disk_status(dHandle fd, struct bridge_disk_status *disk_status);

bool bridge_check_cdrom(dHandle fd);

int bridge_get_hw_dma_mode(dHandle fd, bool *hw_dma_mode);

int bridge_set_hw_dma_mode(dHandle fd, bool hw_dma_mode);

int bridge_get_ips_threshold(dHandle fd, uint32_t *ips_threshold);

int bridge_set_disk_speed(dHandle fd, uint8_t speed);

int bridge_get_disk_speed(dHandle fd, uint8_t *speed);

int bridge_set_bridge_rw_md(dHandle fd, uint8_t mode);

int bridge_get_bridge_rw_md(dHandle fd, uint8_t *mode);

int bridge_disable_cdrom(dHandle fd);

int bridge_disable_disk_hotplug(dHandle fd);

int bridge_read_capacity(dHandle fd, uint64_t *sectors, uint32_t *blk_size);

#endif /* _BRIDGE_DISK_H_ */
