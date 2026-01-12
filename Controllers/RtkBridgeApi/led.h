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

#ifndef _LED_H_
#define _LED_H_
#pragma pack(1)

#define LED_SUBCMD_APPCTL		0x01
#define LED_SUBCMD_RWMEM		0x02
#define LED_SUBCMD_RGB_DRT		0x03
#define LED_SUBCMD_SUP_SYNC		0x04
#define LED_SUBCMD_SUP_ARGB		0x05
#define LED_SUBCMD_SUP_TP		0x06
#define LED_SUBCMD_SUP_DP		0x07

#define MAX_ARGB_NUM			20
#define MAX_APPCTL_ROW			43
#define LED_MEM_NUM_OF_STATE		5
#define LED_MEM_BUF_SIZE		16384
#define LED_INFO_SIZE			32

/* Throughput */
#define MAX_THROUGHPUT_SPEED		2000		// 2000 MB/s
#define MAX_THROUGHPUT_CYCLE_PCT	100		// +100% of each speed decline
#define MAX_THROUGHPUT_EQ_PARTS		100		// 2000 MB/s divide to n parts

#define BREATH_DELAY			100

#ifdef __cplusplus
extern "C" {
#endif

struct led_ex_cmd {
	uint8_t mode;
	uint8_t row_num;
	uint16_t cycle;
	uint8_t hi_ramp;
	uint8_t low_ramp;
	uint8_t hi_stable;
	uint8_t low_stable;
	uint8_t effect_id;
	uint16_t latency;
	uint8_t *color;
	uint16_t len;
};

struct led_mem_info {
	uint8_t mode;
	uint8_t row_num;
	uint16_t cycle;
	uint8_t hi_ramp;	// 1 unit = 100ms
	uint8_t lo_ramp;	// 1 unit = 100ms
	uint8_t hi_stable;	// 1 unit = 100ms
	uint8_t lo_stable;	// 1 unit = 100ms

	/* effect_id
	 * for tool usage
	 * bit7: single / multi color
	 * bit[0:6]: id of effect combobox
	 */
	uint8_t effect_id;
	uint16_t latency;

	uint8_t reserved[21];
	uint8_t ctbl[MAX_APPCTL_ROW * 3];
};

struct led_mem_rgbn {
	struct led_mem_info *rgb[MAX_ARGB_NUM];
};

struct led_mem_header {
	uint32_t signature;
	uint32_t version;
	uint32_t header_addr;
	uint32_t header_size;
	uint32_t data_addr;
	uint32_t data_size;
	uint32_t rgb_num;
	uint16_t bright;
	uint32_t argb_port_sel[2];

	/* Thoughput settings */
	uint16_t max_speed;		// throughput max speed.
	uint16_t cycle_pct;		// n% cycle percentages for throughput variety.
	uint8_t equal_parts;		// divide max_speed to n parts.

	uint8_t reserved[21];
};

/**
 * @brief led parameters stored in memory
 */
struct led_mem {
	struct led_mem_header *header;
	struct led_mem_rgbn *data[LED_MEM_NUM_OF_STATE];
	uint8_t info_size;
	uint8_t max_row;
	uint8_t sync_method;
	uint32_t buf_size;
};

typedef enum LED_MEM_STATE
{
	LED_MEM_IDLE_STATE,
	LED_MEM_RW_STATE,
	LED_MEM_IPS_STATE,
	LED_MEM_SLEEP_STATE,
	LED_MEM_ERROR_STATE,
} LED_MEM_STATE;

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

int led_set_appctl(dHandle fd,
		   bool en);

int led_get_appctl(dHandle fd,
		   uint8_t *sts);

int led_mem_write(dHandle fd,
		  struct led_mem *led_mem_dev);

int led_mem_read(dHandle fd,
		 struct led_mem *led_mem_dev);

int led_mem_write_rgb_direct(dHandle fd,
			     struct led_mem *led_mem_dev,
			     uint8_t *colors,
			     uint16_t numberOfColors);

int led_get_ic_support_sync_light(dHandle fd, uint8_t sync_method);
int led_get_ic_support_argb(dHandle fd);
int led_get_ic_support_throughput(dHandle fd);
int led_get_ic_support_dualport(dHandle fd);

int led_mem_init(dHandle fd,
		 struct led_mem *led_mem_dev);

void led_mem_deinit(struct led_mem *led_mem_dev);

bool led_mem_check_alive(struct led_mem *led_mem_dev);

int led_mem_write_params(dHandle fd,
			 struct led_mem *led_mem_dev);

int led_mem_set_mode(dHandle fd,
		     struct led_mem *led_mem_dev,
		     uint16_t led_state,
		     uint8_t idx,
		     uint8_t mode,
		     bool submit);

int led_mem_get_mode(dHandle fd,
		     struct led_mem *led_mem_dev,
		     uint16_t led_state,
		     uint8_t idx,
		     uint8_t *mode);

int led_mem_set_row(dHandle fd,
		    struct led_mem *led_mem_dev,
		    uint16_t led_state,
		    uint8_t idx,
		    uint8_t row_num,
		    bool submit);

int led_mem_get_row(dHandle fd,
		    struct led_mem *led_mem_dev,
		    uint16_t led_state,
		    uint8_t idx,
		    uint8_t *row_num);

int led_mem_set_cycle(dHandle fd,
		      struct led_mem *led_mem_dev,
		      uint16_t led_state,
		      uint8_t idx,
		      uint16_t cycle,
		      bool submit);

int led_mem_get_cycle(dHandle fd,
		      struct led_mem *led_mem_dev,
		      uint16_t led_state,
		      uint8_t idx,
		      uint16_t *cycle);

int led_mem_set_ramp(dHandle fd,
		     struct led_mem *led_mem_dev,
		     uint16_t led_state,
		     uint8_t idx,
		     uint16_t ramp_factor,
		     bool submit);

int led_mem_get_ramp(dHandle fd,
		     struct led_mem *led_mem_dev,
		     uint16_t led_state,
		     uint8_t idx,
		     uint16_t *ramp_factor);

int led_mem_set_stable(dHandle fd,
		       struct led_mem *led_mem_dev,
		       uint16_t led_state,
		       uint8_t idx,
		       uint16_t stable_factor,
		       bool submit);

int led_mem_get_stable(dHandle fd,
		       struct led_mem *led_mem_dev,
		       uint16_t led_state,
		       uint8_t idx,
		       uint16_t *stable_factor);

int led_mem_set_latency(dHandle fd,
			struct led_mem *led_mem_dev,
			uint16_t led_state,
			uint8_t idx,
			uint16_t latency,
			bool submit);

int led_mem_get_latency(dHandle fd,
			struct led_mem *led_mem_dev,
			uint16_t led_state,
			uint8_t idx,
			uint16_t *latency);

int led_mem_set_eff_id(dHandle fd,
		       struct led_mem *led_mem_dev,
		       uint16_t led_state,
		       uint8_t idx,
		       uint8_t eff_id,
		       bool submit);

int led_mem_get_eff_id(dHandle fd,
		       struct led_mem *led_mem_dev,
		       uint16_t led_state,
		       uint8_t idx,
		       uint8_t *eff_id);

int led_mem_set_bright(dHandle fd,
		       struct led_mem *led_mem_dev,
		       uint16_t bright,
		       bool submit);

int led_mem_get_bright(dHandle fd,
		       struct led_mem *led_mem_dev,
		       uint16_t *bright);

int led_mem_set_color(dHandle fd,
		      struct led_mem *led_mem_dev,
		      uint16_t led_state,
		      uint8_t idx,
		      uint8_t row,
		      uint8_t *val,
		      uint8_t len,
		      bool submit);

int led_mem_get_color(dHandle fd,
		      struct led_mem *led_mem_dev,
		      uint16_t led_state,
		      uint8_t idx,
		      uint8_t row,
		      uint8_t *val,
		      uint8_t len);

int led_mem_set_rgb_num(dHandle fd,
			struct led_mem *led_mem_dev,
			int rgb_num,
			bool submit);

int led_mem_get_rgb_num(struct led_mem *led_mem_dev,
			int *rgb_num);

int led_mem_set_ex(dHandle fd,
		   struct led_mem *led_mem_dev,
		   struct led_ex_cmd *cmd,
		   uint8_t led_state,
		   uint32_t argb_sel,
		   bool is_dual_port,
		   bool submit);

int led_mem_set_tp_mspd(dHandle fd,
			struct led_mem *led_mem_dev,
			int mspd,
			bool submit);

int led_mem_set_tp_cycle_pct(dHandle fd,
			     struct led_mem *led_mem_dev,
			     int cycle_pct,
			     bool submit);

int led_mem_set_tp_eq_parts(dHandle fd,
			    struct led_mem *led_mem_dev,
			    int eq_parts,
			    bool submit);

int led_mem_set_argb_sel(dHandle fd,
			 struct led_mem *led_mem_dev,
			 uint32_t *argb_sel,
			 bool submit);

#ifdef __cplusplus
}
#endif

#pragma pack()

#endif /* _LED_H_ */
