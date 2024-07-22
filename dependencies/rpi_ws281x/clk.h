/*
 * clk.h
 *
 * Copyright (c) 2014 Jeremy Garff <jer @ jers.net>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *     1.  Redistributions of source code must retain the above copyright notice, this list of
 *         conditions and the following disclaimer.
 *     2.  Redistributions in binary form must reproduce the above copyright notice, this list
 *         of conditions and the following disclaimer in the documentation and/or other materials
 *         provided with the distribution.
 *     3.  Neither the name of the owner nor the names of its contributors may be used to endorse
 *         or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __CLK_H__
#define __CLK_H__

#include <stdint.h>

typedef struct {
    uint32_t ctl;
#define CM_CLK_CTL_PASSWD                        (0x5a << 24)
#define CM_CLK_CTL_MASH(val)                     ((val & 0x3) << 9)
#define CM_CLK_CTL_FLIP                          (1 << 8)
#define CM_CLK_CTL_BUSY                          (1 << 7)
#define CM_CLK_CTL_KILL                          (1 << 5)
#define CM_CLK_CTL_ENAB                          (1 << 4)
#define CM_CLK_CTL_SRC_GND                       (0 << 0)
#define CM_CLK_CTL_SRC_OSC                       (1 << 0)
#define CM_CLK_CTL_SRC_TSTDBG0                   (2 << 0)
#define CM_CLK_CTL_SRC_TSTDBG1                   (3 << 0)
#define CM_CLK_CTL_SRC_PLLA                      (4 << 0)
#define CM_CLK_CTL_SRC_PLLC                      (5 << 0)
#define CM_CLK_CTL_SRC_PLLD                      (6 << 0)
#define CM_CLK_CTL_SRC_HDMIAUX                   (7 << 0)
    uint32_t div;
#define CM_CLK_DIV_PASSWD                        (0x5a << 24)
#define CM_CLK_DIV_DIVI(val)                     ((val & 0xfff) << 12)
#define CM_CLK_DIV_DIVF(val)                     ((val & 0xfff) << 0)
} __attribute__((packed, aligned(4))) cm_clk_t;


/*
 * PWM and PCM clock offsets from https://www.scribd.com/doc/127599939/BCM2835-Audio-clocks
 *
 */
#define CM_PCM_OFFSET                            (0x00101098)
#define CM_PWM_OFFSET                            (0x001010a0)


#endif /* __CLK_H__ */
