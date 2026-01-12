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

#define MAX_DISKS 10

#ifdef CONFIG_ARGB_DEV
#define M_BRIDGE_CHECK_VID_PID			argb_bridge_check_vid_pid
#define M_BRIDGE_WRITE_UNLOCK			argb_bridge_write_unlock
#define M_BRIDGE_DISCONNECT			argb_bridge_disconnect
#define M_BRIDGE_CHECK_TARGET			argb_bridge_check_target
#define M_BRIDGE_GET_IC_GEN			argb_bridge_get_ic_gen
#define M_BRIDGE_GET_UUID			argb_bridge_get_uuid
#define M_BRIDGE_GET_CUST_PAGE			argb_bridge_get_cust_page
#define M_BRIDGE_SET_CUST_PAGE			argb_bridge_set_cust_page
#define M_BRIDGE_GET_FW_VER			argb_bridge_get_fw_ver
#define M_BRIDGE_GET_BUF_SIZE			argb_bridge_get_buf_size
#define M_BRIDGE_GET_USB_SPEED			argb_bridge_get_usb_speed
#define M_BRIDGE_GET_API_VER			argb_bridge_get_api_ver
#define M_BRIDGE_INQUIRY			argb_bridge_inquiry
#define M_BRIDGE_GET_NAME			argb_bridge_get_name
#define M_BRIDGE_GET_VENDOR_NAME		argb_bridge_get_vendor_name
#define M_BRIDGE_GET_PRODUCT_NAME		argb_bridge_get_product_name
#define M_BRIDGE_GET_DEBUG_LEVEL		argb_bridge_get_debug_level
#define M_BRIDGE_SET_DEBUG_LEVEL		argb_bridge_set_debug_level
#define M_BRIDGE_REPORT_LUNS			argb_bridge_report_luns
#else
#define M_BRIDGE_CHECK_VID_PID			bridge_check_vid_pid
#define M_BRIDGE_WRITE_UNLOCK			bridge_write_unlock
#define M_BRIDGE_DISCONNECT			bridge_disconnect
#define M_BRIDGE_CHECK_TARGET			bridge_check_target
#define M_BRIDGE_GET_IC_GEN			bridge_get_ic_gen
#define M_BRIDGE_GET_UUID			bridge_get_uuid
#define M_BRIDGE_GET_CUST_PAGE			bridge_get_cust_page
#define M_BRIDGE_SET_CUST_PAGE			bridge_set_cust_page
#define M_BRIDGE_GET_FW_VER			bridge_get_fw_ver
#define M_BRIDGE_GET_BUF_SIZE			bridge_get_buf_size
#define M_BRIDGE_GET_USB_SPEED			bridge_get_usb_speed
#define M_BRIDGE_GET_API_VER			bridge_get_api_ver
#define M_BRIDGE_INQUIRY			bridge_inquiry
#define M_BRIDGE_GET_NAME			bridge_get_name
#define M_BRIDGE_GET_VENDOR_NAME		bridge_get_vendor_name
#define M_BRIDGE_GET_PRODUCT_NAME		bridge_get_product_name
#define M_BRIDGE_GET_DEBUG_LEVEL		bridge_get_debug_level
#define M_BRIDGE_SET_DEBUG_LEVEL		bridge_set_debug_level
#define M_BRIDGE_REPORT_LUNS			bridge_report_luns
#endif

struct bridge_fw_version {
	uint32_t fw_major_ver;
	uint32_t fw_minor_ver;
	uint32_t fw_extra_ver;
	uint32_t fw_build_ver;
	uint32_t fw_build_date;
};

#ifdef CONFIG_ARGB_DEV
int argb_bridge_check_vid_pid(uint16_t vid,
			      uint16_t pid,
			      struct usb_id *id,
			      int id_num);
int argb_bridge_write_unlock(dHandle fd);
int argb_bridge_disconnect(dHandle fd, int type);
int argb_bridge_get_ic_gen(dHandle fd);
bool argb_bridge_check_target(dHandle fd);
int argb_bridge_get_uuid(dHandle fd, uint8_t *uuid);
int argb_bridge_get_cust_page(dHandle fd, uint8_t *cust_page);
int argb_bridge_set_cust_page(dHandle fd, uint8_t *cust_page);
int argb_bridge_get_fw_ver(dHandle fd, struct bridge_fw_version *fw_ver);
int argb_bridge_get_buf_size(dHandle fd, uint32_t *buf_size);
int argb_bridge_get_usb_speed(dHandle fd, uint32_t *speed);
void argb_bridge_get_api_ver(uint32_t *ver_main1,
			     uint32_t *ver_main2,
			     uint32_t *ver_sub1,
			     uint32_t *ver_sub2);
int argb_bridge_inquiry(dHandle fd, uint8_t *buf, uint32_t buflen);
int argb_bridge_get_name(dHandle fd, char *name, uint32_t len);
int argb_bridge_get_vendor_name(dHandle fd, char *name, uint32_t len);
int argb_bridge_get_product_name(dHandle fd, char *name, uint32_t len);

int argb_bridge_get_debug_level(const char *debug, int *lvl);
int argb_bridge_set_debug_level(int lvl);

int argb_bridge_report_luns(dHandle fd,
			    uint8_t *lun,
			    uint32_t *total_luns);
#else
int bridge_check_vid_pid(uint16_t vid,
			 uint16_t pid,
			 struct usb_id *id,
			 int id_num);
int bridge_write_unlock(dHandle fd);
int bridge_disconnect(dHandle fd, int type);
int bridge_get_ic_gen(dHandle fd);
bool bridge_check_target(dHandle fd);
int bridge_get_uuid(dHandle fd, uint8_t *uuid);
int bridge_get_cust_page(dHandle fd, uint8_t *cust_page);
int bridge_set_cust_page(dHandle fd, uint8_t *cust_page);
int bridge_get_fw_ver(dHandle fd, struct bridge_fw_version *fw_ver);
int bridge_get_buf_size(dHandle fd, uint32_t *buf_size);
int bridge_get_usb_speed(dHandle fd, uint32_t *speed);
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
#endif

#ifdef __cplusplus
}
#endif

#endif /* _BRIDGE_FUNC_H_ */
