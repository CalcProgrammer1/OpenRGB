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

#ifndef _BRIDGE_FUNC_H_
#define _BRIDGE_FUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof(arr[0]))
#endif

#define MAX_DISKS           10
#define MAX_NUM_HID_CHS     10

#define USBID_ANY           0xFFFF

struct bridge_fw_version {
	uint32_t fw_major_ver;
	uint32_t fw_minor_ver;
	uint32_t fw_extra_ver;
	uint32_t fw_build_ver;
	uint32_t fw_build_date;
};

struct bridge_hid_ch {
	uint8_t report_id;
	uint16_t ch_len;
};

int bridge_check_vid_pid(uint16_t vid,
			 uint16_t pid,
			 struct usb_id *id,
			 int id_num);
int bridge_write_unlock(dHandle fd);
int bridge_disconnect(dHandle fd, int type);
bool bridge_check_target(dHandle fd);
int bridge_get_uuid(dHandle fd, uint8_t *uuid);
int bridge_get_cust_page(dHandle fd, uint8_t *cust_page);
int bridge_set_cust_page(dHandle fd, uint8_t *cust_page);
int bridge_get_fw_ver(dHandle fd, struct bridge_fw_version *fw_ver);
int bridge_get_buf_size(dHandle fd, uint32_t *buf_size);
int bridge_get_usb_speed(dHandle fd, uint32_t *speed);
int bridge_get_hid_ch(dHandle fd, struct bridge_hid_ch *chs);
void bridge_get_api_ver(uint32_t *ver_main1,
			uint32_t *ver_main2,
			uint32_t *ver_sub1,
			uint32_t *ver_sub2);
int bridge_inquiry(dHandle fd, uint8_t *buf, uint32_t buflen);
int bridge_get_name(dHandle fd, char *name, uint32_t len);
int bridge_get_vendor_name(dHandle fd, char *name, uint32_t len);
int bridge_get_product_name(dHandle fd, char *name, uint32_t len);

int bridge_get_debug_level(const char *debug, int *lvl);
int bridge_set_debug_level(int lvl);

int bridge_report_luns(dHandle fd,
		       uint8_t *lun,
		       uint32_t *total_luns);

#ifdef __cplusplus
}
#endif

#endif /* _BRIDGE_FUNC_H_ */
