/*
 * BSD-3-Clause
 *
 * Copyright (c) 2022, Realtek Semiconductor Corp. All rights reserved.
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

#ifndef _ATA_H_
#define _ATA_H_

#ifdef __cplusplus
extern "C" {
#endif

enum {
	/* SCSI command */
	ATA_12 = 0xA1, /*!< 0xA1 */
	ATA_16 = 0x85, /*!< 0x85 */

	/* ATA protocol */
	ATA_PROTO_NON_DATA = 3 << 1, /*!< 3 << 1 */
	ATA_PROTO_PIO_IN = 4 << 1,   /*!< 4 << 1 */
	ATA_PROTO_PIO_OUT = 5 << 1,  /*!< 5 << 1 */

	/* ATA command */
	ATA_CMD_IDENTIFY = 0xEC,          /*!< 0xEC */
	ATA_CMD_SET_FEATURES = 0xEF,      /*!< 0xEF */
	ATA_CMD_SEC_SET_PWD = 0xF1,       /*!< 0xF1 */
	ATA_CMD_SEC_UNLOCK = 0xF2,        /*!< 0xF2 */
	ATA_CMD_SEC_ERASE_PREPARE = 0xF3, /*!< 0xF3 */
	ATA_CMD_SEC_ERASE_UNIT = 0xF4,    /*!< 0xF4 */
	ATA_CMD_SEC_FREEZE_LOCK = 0xF5,   /*!< 0xF5 */
	ATA_CMD_SEC_DISABLE_PWD = 0xF6,   /*!< 0xF6 */
	ATA_CMD_SEC_LOCK = 0xF7,          /*!< 0xF7 */

	/* ATA identify offset */
	ATA_ID_SERNO                    = 10,  /*!< 10 */
	ATA_ID_FW_REV                   = 23,  /*!< 23 */
	ATA_ID_PROD                     = 27,  /*!< 27 */
	ATA_ID_NORMAL_SEC_ERASE_TIME    = 89,  /*!< 89 */
	ATA_ID_ENHANCED_SEC_ERASE_TIME  = 90,  /*!< 90 */
	ATA_ID_MASTER_PWD_ID            = 92,  /*!< 92 */
	ATA_ID_DLF                      = 128, /*!< 128 */

	/* bits in identify security status */
	ATA_SEC_SUPPORTED       = (1 << 0), /*!< 1 << 0 */
	ATA_SEC_ENABLED         = (1 << 1), /*!< 1 << 1 */
	ATA_SEC_LOCKED          = (1 << 2), /*!< 1 << 2 */
	ATA_SEC_FROZEN          = (1 << 3), /*!< 1 << 3 */
	ATA_SEC_CNT_EXPIRED     = (1 << 4), /*!< 1 << 4 */
	ATA_SEC_ENH_ERASE_SUPP  = (1 << 5), /*!< 1 << 5 */
	ATA_SEC_MASTER_PWD_CAP  = (1 << 8), /*!< 1 << 8 */

	ATA_SEC_ERASE_TIME_FORMAT = (1 << 15), /*!< 1 << 15 */
};

PACK (
struct ata_smart_entry {
	uint8_t id;
	uint8_t flag[2];
	uint8_t current_value;
	uint8_t worst_value;
	uint8_t data[4];
	uint8_t attr_specific[2];
	uint8_t threshold;
});

struct ata_smart_log {
	uint8_t version[2];
	struct ata_smart_entry entry[30];
};

int ata_identify(dHandle fd, uint8_t *id, int len);
int ata_get_disk_info(dHandle fd, struct identify_info *disk);
int ata_get_smart_log(dHandle fd, uint8_t *log, uint32_t len);
int ata_parse_smart_log(uint8_t *buf);
int ata_set_feature(dHandle fd, uint8_t subcmd_lba, uint8_t subcmd_cnt);

#ifdef __cplusplus
}
#endif

#endif /* _ATA_H_ */
