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

#ifndef _LED_EFFECT_H_
#define _LED_EFFECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(_WIN32)
typedef struct tagRGBQUAD {
	unsigned char    rgbBlue;
	unsigned char    rgbGreen;
	unsigned char    rgbRed;
	unsigned char    rgbReserved;
} RGBQUAD;
#endif

typedef enum LED_MODE
{
	NONE,
	COLOR_ALWAYS,   //single always
	COLOR_CYCLE,    //row take turns
	COLOR_BLINK,    //each row represents a blink color
	COLOR_BREATHE,
	FORCE_SHUTDOWN,
	PANEL_DISPLAY,
	PANEL_TEMP_MONITOR,
	NUMBER_OF_MODE,
} LED_MODE;

enum LED_EFF
{
	LED_EFF_NONE,
	LED_EFF_ALWAYS,
	LED_EFF_BLINK,
	LED_EFF_BREATHE,
	LED_EFF_SPECTRUM,
	LED_EFF_SCROLL,
	LED_EFF_RAINBOW_SCROLL,
	LED_EFF_RUNNING_WATER,
	LED_EFF_SLIDING,
	LED_EFF_NEWTON_CRADLE,
	LED_EFF_METEOR,
	NUMBER_OF_LED_EFF_MODE,
};

int HSV2RGB(float hue, float sat, float value);

int led_eff_set_always_on(dHandle fd,
			  struct led_mem *led_mem_dev,
			  RGBQUAD rgb);

int led_eff_set_blink(dHandle fd,
		      struct led_mem *led_mem_dev,
		      RGBQUAD rgb,
		      uint16_t cycle);

int led_eff_set_breathe(dHandle fd,
			struct led_mem *led_mem_dev,
			RGBQUAD rgb,
			uint16_t cycle);

int led_eff_set_spectrum(dHandle fd,
			 struct led_mem *led_mem_dev,
			 uint16_t cycle);

int led_eff_set_scroll(dHandle fd,
		       struct led_mem *led_mem_dev,
		       RGBQUAD rgb,
		       uint16_t cycle);

int led_eff_set_rainbow_scroll(dHandle fd,
			       struct led_mem *led_mem_dev,
			       uint16_t cycle);

int led_eff_set_running_water(dHandle fd,
			      struct led_mem *led_mem_dev,
			      RGBQUAD rgb,
			      uint16_t cycle);

int led_eff_set_sliding(dHandle fd,
			struct led_mem *led_mem_dev,
			uint16_t cycle);

int led_eff_set_newton_cradle(dHandle fd,
			      struct led_mem *led_mem_dev,
			      uint16_t cycle);

int led_eff_set_meteor(dHandle fd,
		       struct led_mem *led_mem_dev,
		       RGBQUAD rgb,
		       uint16_t cycle);

#ifdef __cplusplus
}
#endif


#endif /* _LED_EFFECT_H_ */
