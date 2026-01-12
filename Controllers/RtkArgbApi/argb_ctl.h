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
#ifndef _ARGB_CTL_H_
#define _ARGB_CTL_H_

#pragma pack(1)

#define ARGBCTL_SIG			0x41524742
#define ARGBCTL_VER			0x3

#define ARGBCTL_EFF_MAX_NUM		5
#define ARGBCTL_ARGB_MAX		400

#define ARGB_COLOR_DEPTH		3

#define LED_SUBCMD_APPCTL		0x01
#define LED_SUBCMD_RWMEM		0x02
#define LED_SUBCMD_RGB_DRT		0x03
#define LED_SUBCMD_SUP_SYNC		0x04
#define LED_SUBCMD_SUP_ARGB		0x05
#define LED_SUBCMD_SUP_TP		0x06
#define LED_SUBCMD_SUP_DP		0x07
#define LED_SUBCMD_ARGB_CTRL_NUM	0x08
#define LED_SUBCMD_RGB_DRT_AIO		0x09

#ifdef __cplusplus
extern "C" {
#endif

enum ARGB_EFFECT_ID {
	EFF_NULL                              = 0x0000,
	EFF_ALWAYS_ON                         = 0x0001,
	EFF_BLINK                             = 0x0002,
	EFF_BREATH                            = 0x0003,
	EFF_SPECTRUM                          = 0x0004,
	EFF_SCROLL                            = 0x0005,
	EFF_RAINBOW_SCROLL                    = 0x0006,
	EFF_RUNNING_WATER                     = 0x0007,
	EFF_SLIDING                           = 0x0008,
	EFF_NEWTON_CRADLE                     = 0x0009,
	EFF_METEOR                            = 0x000A,
	EFF_RAINBOW_SLIDING                   = 0x000B,
};

enum ARGB_GROUP {
	ARGB_GRP_0,
	ARGB_GRP_1,
	ARGB_GRP_2,
	ARGB_GRP_3,
	ARGB_GRP_4,
	NUM_ARGB_GRP,
	ARGB_GRP_NULL,
};

enum SYNC_METHOD
{
	SYNC_METHOD_ALL,
	SYNC_METHOD_AURA,
	SYNC_METHOD_MYSTIC,
	SYNC_METHOD_RTKLED,
	SYNC_METHOD_GBT,
    SYNC_METHOD_ROG_AURA,
    SYNC_METHOD_OPENRGB,
	NUM_OF_SYNC_METHOD,

	SYNC_METHOD_LAMP = (0x80 | SYNC_METHOD_ALL),
};

/*****************************************************************
App ctl parameters
*****************************************************************/
enum LED_CTL
{
	LED_CTL_FW = 0,
	LED_CTL_APP,
	LED_CTL_LAMP,
};

/**
 * @brief argb ctl header
 */
typedef struct argbCtlHeader {
	uint32_t signature;
	uint32_t version;
	uint32_t header_addr;
	uint32_t header_size;
	uint32_t data_addr;
	uint16_t data_size;
	uint16_t single_data_size;
	uint32_t mdl_addr;
	uint32_t mdl_size;
	uint16_t rgb_num[NUM_ARGB_GRP];
	uint16_t bright[NUM_ARGB_GRP];
	uint8_t reserved[28];
} argbCtlHeader;

/**
 * @brief argb ctl effect data
 */
typedef struct argbCtlEffn {
	uint16_t eff_id;
	uint16_t cycle;
	uint8_t hi_ramp;
	uint8_t lo_ramp;
	uint8_t hi_stable;
	uint8_t lo_stable;
	uint32_t prime_color;
	uint32_t sec_color;
	uint32_t sub_cmd;
	uint8_t *reserved;
} argbCtlEffn;

/**
 * @brief argb ctl MDL attributes
 */
typedef struct __LampPosition
{
	uint32_t XInMicrometers;
	uint32_t YInMicrometers;
	uint32_t ZInMicrometers;
} LampPosition;
typedef struct __LampAttributes
{
	uint16_t LampId;
	LampPosition pos;
	uint32_t UpdateLatencyInMicroseconds;
	uint32_t LampPurposes;
	uint8_t RedLevelCount;
	uint8_t GreenLevelCount;
	uint8_t BlueLevelCount;
	uint8_t IntensityLevelCount;
	uint8_t IsProgrammable;
	uint8_t LampKey;
} LampAttributes;
struct LampArrayAttributesReport
{
	uint8_t ReportId;
	uint16_t LampCount;
	uint32_t BoundingBoxWidthInMicrometers;
	uint32_t BoundingBoxHeightInMicrometers;
	uint32_t BoundingBoxDepthInMicrometers;
	uint32_t LampArrayKind;
	uint32_t MinUpdateIntervalInMicroseconds;
};
struct LampAttributesResponseReport
{
	uint8_t ReportId;
	LampAttributes Attributes;
};

typedef struct argbCtlMDL {
	struct LampArrayAttributesReport *laar;
	struct LampAttributesResponseReport *larr;
	LampPosition *poses;
} argbCtlMDL;

/**
 * @brief argb parameters stored in memory
 */
typedef struct argbCtl {
	argbCtlHeader *header;
	argbCtlEffn *data[ARGBCTL_EFF_MAX_NUM];
	argbCtlMDL mdl[NUM_ARGB_GRP];
	uint8_t app_ctl[NUM_ARGB_GRP];
	uint8_t app_eff_done[NUM_ARGB_GRP];
	uint8_t *colors;
	uint8_t sync_method;
	uint32_t buf_size;
} argbCtl;

int led_argb_set_appctl(dHandle fd, uint8_t grp_num, uint8_t ctl);
int led_argb_get_appctl(dHandle fd, uint8_t grp_num, uint8_t *ctl);

int led_argb_write(dHandle fd,
		   argbCtl *argb_ctl);
int led_argb_read(dHandle fd,
		  argbCtl *argb_ctl);
int led_argb_write_rgb_direct(dHandle fd,
			      argbCtl *argb_ctl,
			      uint8_t *colors,
			      uint16_t numberOfColors,
			      uint8_t grp);
int led_argb_write_rgb_direct_aio(dHandle fd,
			      argbCtl *argb_ctl,
			      uint8_t group_mask,
			      uint8_t *colors,
			      uint16_t TotalRGBNum);

int led_argb_init(dHandle fd,
		  argbCtl *argb_ctl);
void led_argb_deinit(argbCtl *argb_ctl);
bool led_argb_check_alive(argbCtl *argb_ctl);

int led_argb_set_rgb_num(dHandle fd,
			 argbCtl *argb_ctl,
			 uint8_t grp,
			 uint16_t num_argb,
			 bool submit);
int led_argb_get_rgb_num(dHandle fd,
			 argbCtl *argb_ctl,
			 uint8_t grp,
			 uint16_t *num_argb);

int led_argb_get_ic_support_sync_light(dHandle fd, uint8_t sync_method);
int led_argb_get_ic_support_argb(dHandle fd);

int led_argb_set_bright(dHandle fd,
			argbCtl *argb_ctl,
			uint8_t grp,
			uint16_t bright,
			bool submit);
int led_argb_get_bright(dHandle fd,
			argbCtl *argb_ctl,
			uint8_t grp,
			uint16_t *bright);
int led_argb_set_eff_id(dHandle fd,
			argbCtl *argb_ctl,
			uint8_t eff_i,
			uint16_t id,
			bool submit);
int led_argb_get_eff_id(dHandle fd,
			argbCtl *argb_ctl,
			uint8_t eff_i,
			uint16_t *id);
int led_argb_set_cycle(dHandle fd,
		       argbCtl *argb_ctl,
		       uint8_t eff_i,
		       uint16_t cycle,
		       bool submit);
int led_argb_get_cycle(dHandle fd,
		       argbCtl *argb_ctl,
		       uint8_t eff_i,
		       uint16_t *cycle);
int led_argb_set_ramp(dHandle fd,
		      argbCtl *argb_ctl,
		      uint8_t eff_i,
		      uint16_t ramp,
		      bool submit);
int led_argb_get_ramp(dHandle fd,
		      argbCtl *argb_ctl,
		      uint8_t eff_i,
		      uint16_t *ramp);
int led_argb_set_stable(dHandle fd,
			argbCtl *argb_ctl,
			uint8_t eff_i,
			uint16_t stable,
			bool submit);
int led_argb_get_stable(dHandle fd,
			argbCtl *argb_ctl,
			uint8_t eff_i,
			uint16_t *stable);
int led_argb_set_p_color(dHandle fd,
			 argbCtl *argb_ctl,
			 uint8_t eff_i,
			 uint32_t p_color,
			 bool submit);
int led_argb_get_p_color(dHandle fd,
			 argbCtl *argb_ctl,
			 uint8_t eff_i,
			 uint32_t *p_color);
int led_argb_set_s_color(dHandle fd,
			 argbCtl *argb_ctl,
			 uint8_t eff_i,
			 uint32_t s_color,
			 bool submit);
int led_argb_get_s_color(dHandle fd,
			 argbCtl *argb_ctl,
			 uint8_t eff_i,
			 uint32_t *s_color);
int led_argb_set_subcmd(dHandle fd,
			argbCtl *argb_ctl,
			uint8_t eff_i,
			uint32_t subcmd,
			bool submit);
int led_argb_get_subcmd(dHandle fd,
			argbCtl *argb_ctl,
			uint8_t eff_i,
			uint32_t *subcmd);

int led_argb_set_ex(dHandle fd,
		    argbCtl *argb_ctl,
		    uint8_t eff_i,
		    argbCtlEffn *cmd,
		    bool submit);

int led_argb_set_id1(dHandle fd,
		    argbCtl *argb_ctl,
		    uint8_t grp,
		    struct LampArrayAttributesReport *laar,
		    bool submit);

int led_argb_set_id3(dHandle fd,
		    argbCtl *argb_ctl,
		    uint8_t grp,
		    struct LampAttributesResponseReport *larr,
		    bool submit);

int led_argb_set_pos(dHandle fd,
		     argbCtl *argb_ctl,
		     uint8_t grp,
		     void *pos,
		     bool submit);

#ifdef __cplusplus
}
#endif

#pragma pack()

#endif /* _ARGB_CTL_H_ */
