/*
 * BSD-3-Clause
 *
 * Copyright (c) 2022 - 2024, Realtek Semiconductor Corp. All rights reserved.
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

#ifndef _BRIDGE_DEV_H_
#define _BRIDGE_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

#define NAME_STR_LEN 512
#define MAX_BLK_SIZE 4096

enum BRDEV_TYPE {
	BRDEV_TYPE_BRIDGE,
	BRDEV_TYPE_ARGB,
	NUM_BRDEV_TYPE,
};

enum BRINTF_TYPE {
	BRINTF_TYPE_SCSI,
	BRINTF_TYPE_HID,
	BRINTF_TYPE_MS,
	NUM_BRINTF_TYPE,
};

struct identify_info {
	// IDENTIFY information
	uint8_t serial_num[20];
	uint8_t null0;  // make sn a cstring
	uint8_t fw_rev[8];
	uint8_t null1;  // make firmware rev a cstring
	uint8_t model_num[40];
	uint8_t null2;  // make model number a cstring
};

struct discovery_info {
	uint8_t tper : 1;
	uint8_t locking : 1;
	uint8_t geometry : 1;
	uint8_t single_user_mode : 1;
	uint8_t opalv10 : 1;
	uint8_t opalv20 : 1;
	uint8_t pyritev100 : 1;
	uint8_t pyritev200 : 1;
	uint8_t blocksid : 1;
	uint8_t tper_ack_nack : 1;
	uint8_t tper_async : 1;
	uint8_t tper_buffer_mgt : 1;
	uint8_t tper_comid_mgt : 1;
	uint8_t tper_streaming : 1;
	uint8_t tper_sync : 1;
	uint8_t locking_locked : 1;
	uint8_t locking_locking_enabled : 1;
	uint8_t locking_locking_supported : 1;
	uint8_t locking_mbr_not_supported : 1;
	uint8_t locking_mbr_done : 1;
	uint8_t locking_mbr_enabled : 1;
	uint8_t locking_media_encrypt : 1;
	uint8_t geometry_align : 1;
	uint8_t blocksid_sid_value : 1;
	uint8_t blocksid_sid_block : 1;
	uint8_t blocksid_hw_reset : 1;
	uint8_t single_user_mode_any : 1;
	uint8_t single_user_mode_all : 1;
	uint8_t single_user_mode_policy : 1;
	uint8_t ruby100 : 1;
	uint8_t reserved0 : 2;
	uint32_t reserved1;
	uint64_t geometry_alignment_granularity;
	uint32_t geometry_logical_blocksize;
	uint16_t enc_mode;
	uint16_t reserved2;
	uint64_t geometry_lowest_aligned_lba;
	uint32_t single_user_mode_locking_objects;
	uint16_t opalv10_base_comid;
	uint16_t opalv10_num_comids;
	uint16_t base_comid;
	uint16_t num_comids;
	uint8_t initial_pin;
	uint8_t reverted_pin;
	uint16_t num_admins;
	uint16_t num_users;
	uint8_t range_crossing : 1;
	uint8_t reserved3 : 7;
	uint8_t reserved4;
	uint16_t pyritev100_base_comid;
	uint16_t pyritev100_num_comids;
	uint16_t pyritev100_initial_pin;
	uint16_t pyritev100_reverted_pin;
	uint16_t pyritev200_base_comid;
	uint16_t pyritev200_num_comids;
	uint16_t pyritev200_initial_pin;
	uint16_t pyritev200_reverted_pin;
	uint32_t reserved5;
};

/* ata security info */
struct ata_sec_info {
	uint16_t supported : 1;
	uint16_t enabled : 1;
	uint16_t locked : 1;
	uint16_t frozen : 1;
	uint16_t expired : 1;
	uint16_t enh_sec_erase_supported : 1;
	uint16_t reserved00 : 2;
	uint16_t master_pwd_cap : 1;
	uint16_t reserved01 : 7;
	uint16_t master_pwd_id;
	uint16_t normal_sec_erase_time;
	uint16_t enhanced_sec_erase_time;
};

/** structure to store Disk information. */
struct ssd_info {
	struct identify_info identify;
	struct discovery_info discovery;
	struct ata_sec_info ata_sec;
};

struct scan_result {
	char name[NAME_STR_LEN];
	char br_name[NAME_STR_LEN];
	struct ssd_info ssdinfo;
#if defined(__APPLE__)
	char bsd_name[NAME_STR_LEN];
#endif
	uint8_t dev_type;
	struct scan_result *next;
};

struct br_priv;

struct bridge_device {
	void *hdl;
	uint32_t blk_size;
	uint8_t lun;
	uint8_t dev_type;
	uint8_t intf_type;
	struct bridge_hid_ch *chs;
	struct br_priv *priv;
};

struct bridge_device *bridge_open_handle(char *dev_path);
int bridge_close_handle(struct bridge_device *fd);
void bridge_exit();
struct scan_result *bridge_disk_scan(struct usb_id *id, int id_num);
struct scan_result *bridge_dev_scan(struct usb_id *id, int id_num);
void bridge_free_scan(struct scan_result *result);
void* bridge_dev_get_handle(struct bridge_device *dev);
void bridge_dev_set_disk_blk_size(struct bridge_device *dev, uint32_t blk_size);

#ifdef __cplusplus
}
#endif

#endif /* _BRIDGE_DEV_H_ */
