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

#ifndef _SCSI_DEV_H_
#define _SCSI_DEV_H_


#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_ARGB_DEV
#define M_SCSI_INIT		argb_scsi_init
#define M_SCSI_EXIT		argb_scsi_exit
#define M_SCSI_ENUM		argb_scsi_enum
#define M_SCSI_FREE_ENUM	argb_scsi_free_enum
#define M_SCSI_DEV_OPEN_PATH	argb_scsi_dev_open_path
#define M_SCSI_DEV_CLOSE	argb_scsi_dev_close
#define M_SCSI_GET_HANDLE	argb_scsi_get_handle
#define M_SCSI_DEV_SPTI		argb_scsi_dev_spti
#else
#define M_SCSI_INIT		scsi_init
#define M_SCSI_EXIT		scsi_exit
#define M_SCSI_ENUM		scsi_enum
#define M_SCSI_FREE_ENUM	scsi_free_enum
#define M_SCSI_DEV_OPEN_PATH	scsi_dev_open_path
#define M_SCSI_DEV_CLOSE	scsi_dev_close
#define M_SCSI_GET_HANDLE	scsi_get_handle
#define M_SCSI_DEV_SPTI		scsi_dev_spti
#endif

struct _scsi_device;
typedef struct _scsi_device scsi_device;
struct scsi_device_info {
	char *path;
	uint8_t label;
	unsigned short vendor_id;
	unsigned short product_id;
	struct scsi_device_info *next;
};

#ifdef CONFIG_ARGB_DEV
int argb_scsi_init(void);
void argb_scsi_exit(void);
struct scsi_device_info *argb_scsi_enum(uint16_t vid, uint16_t pid);
void argb_scsi_free_enum(struct scsi_device_info *devs);
scsi_device *argb_scsi_dev_open_path(const char *path);
int argb_scsi_dev_close(scsi_device *dev);
void *argb_scsi_get_handle(scsi_device *dev);

int argb_scsi_dev_spti(scsi_device *dev, uint8_t *cdb,
		       uint8_t *buf, int buflen,
		       uint8_t is_in, int timeout,
		       uint8_t *sense);
#else
int scsi_init(void);
void scsi_exit(void);
struct scsi_device_info *scsi_enum(uint16_t vid, uint16_t pid);
void scsi_free_enum(struct scsi_device_info *devs);
scsi_device *scsi_dev_open_path(const char *path);
int scsi_dev_close(scsi_device *dev);
void *scsi_get_handle(scsi_device *dev);

int scsi_dev_spti(scsi_device *dev, uint8_t *cdb,
		  uint8_t *buf, int buflen,
		  uint8_t is_in, int timeout,
		  uint8_t *sense);
#endif

#ifdef __cplusplus
}
#endif
#endif
