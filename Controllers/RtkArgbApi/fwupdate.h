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

#ifndef _PG_H_
#define _PG_H_

#ifdef __cplusplus
extern "C" {
#endif

enum {
	PG_ERR_DEV_PATH = 1,            /*!< 1  */
	PG_ERR_INTERNAL_FAILURE = 2,    /*!< 2  */
	PG_ERR_IO = 3,                  /*!< 3  */
	PG_ERR_REBOOT = 4,              /*!< 4  */
	PG_ERR_UUID = 5,                /*!< 5  */
	PG_ERR_WRONG_FW = 6,            /*!< 6  */
	PG_ERR_WRONG_RSQ_FW = 7,        /*!< 7  */
	PG_ERR_WRONG_LIGHT_BIN = 8,     /*!< 8  */
	PG_ERR_WRONG_CONF = 9,          /*!< 9  */
	PG_ERR_UPDATE_FW = 10,          /*!< 10 */
	PG_ERR_UPDATE_RSQ = 11,         /*!< 11 */
	PG_ERR_UPDATE_LIGHT = 12,       /*!< 12 */
	PG_ERR_UPDATE_CONF = 13,        /*!< 13 */
	PG_ERR_BOOT_RSQ_FW = 14,        /*!< 14 */
	PG_ERR_BOOT_NORMAL_FW = 15,     /*!< 15 */
	PG_ERR_NO_DEV = 16,             /*!< 16 */
};

/** \brief pg info structure. */
typedef struct _PGINFO
{
	uint16_t vid;
	uint16_t pid;

	char product[62 + 2];     //+2 for end '\0'
	char manufacture[30 + 2]; //+2 for end '\0'
	char serial[62 + 2];      //+2 for end '\0'

	char scsi_vendor[8 + 1];   //+1 for end '\0'
	char scsi_product[16 + 1]; //+1 for end '\0'

	uint8_t aspmdis;
	uint8_t dis_ips_thres;
	uint8_t led;
	uint32_t hw_led_cfg;
	uint8_t customized_led[15];
	uint8_t suspend_led_off;
	uint8_t dbg_pin;
	uint8_t disk_hotplug;
	uint8_t sd_mmc_type;
	uint8_t usb_self_pwr;
	uint64_t customized_feature;
	uint32_t support_hid;
	uint32_t raid_cfg;
	uint32_t force_usb_quirk;
	uint32_t rw_mode;
	uint16_t bcddevice;
	uint8_t scsi_wp_pin;
} PGINFO;

//// Fwupdate APIs ////
int fwupdate_all(const char *devpath,
		 const char *bin,
		 const char *rescuebin,
		 const char *conf,
		 const char *ledbin);
uint32_t fwupdate_get_progress();
void fwupdate_reset_progess();
int fwupdate_get_last_error();
int fwupdate_write_light_bin(dHandle fd, const char *bin);
int fwupdate_flash_dump(dHandle fd, char *file_name, uint16_t flash_size);

#ifdef CONFIG_ARGB_DEV
int fwupdate_write_light_arr(dHandle fd, void *ctl);
#endif

int pg_write(dHandle fd, const char *conf);
int pg_read(dHandle fd, PGINFO *info);
void pg_print_pginfo(PGINFO* info);
int pg_get_force_usb_speed(dHandle fd, uint8_t *speed);
int pg_set_force_usb_speed(dHandle fd, uint8_t speed);
int pg_get_serial_num(dHandle fd, char *sn, uint32_t len);
int pg_set_serial_num(dHandle fd, char *sn, uint32_t len);
int pg_get_manufacturer_name(dHandle fd, char *manu, uint32_t len);
int pg_get_product_name(dHandle fd, char *pdname, uint32_t len);
int pg_get_vid(dHandle fd, uint16_t *vid);
int pg_set_vid(dHandle fd, uint16_t vid);
int pg_get_pid(dHandle fd, uint16_t *pid);
int pg_set_pid(dHandle fd, uint16_t pid);
int pg_get_rw_mode(dHandle fd, uint32_t *mode);
int pg_set_rw_mode(dHandle fd, uint32_t mode);

#ifdef __cplusplus
}
#endif

#endif // _PG_H_
