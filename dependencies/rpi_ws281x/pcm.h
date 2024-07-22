/*
 * pcm.h
 *
 * Copyright (c) 2014 Jeremy Garff <jer @ jers.net>
 * PCM version Copyright (c) Ton van Overbeek <tvoverbeek @ gmail.com>
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

#ifndef __PCM_H__
#define __PCM_H__

#include <stdint.h>

/*
 *
 * Pin mapping of alternate pin configuration for PCM
 *
 * GPIO   ALT PCM_CLK   ALT PCM-FS   ALT PCM_DIN   ALT PCM_DOUT
 *
 *  18        0             
 *  19                      0
 *  20                                   0
 *  21                                                 0
 *  28        2 
 *  29                      2
 *  30                                   2
 *  31                                                 2
 *
 */


typedef struct
{
    uint32_t cs;
#define RPI_PCM_CS_STBY                         (1 << 25)
#define RPI_PCM_CS_SYNC                         (1 << 24)
#define RPI_PCM_CS_RXSEX                        (1 << 23)
#define RPI_PCM_CS_RXF                          (1 << 22)
#define RPI_PCM_CS_TXE                          (1 << 21)
#define RPI_PCM_CS_RXD                          (1 << 20)
#define RPI_PCM_CS_TXD                          (1 << 19)
#define RPI_PCM_CS_RXR                          (1 << 18)
#define RPI_PCM_CS_TXW                          (1 << 17)
#define RPI_PCM_CS_RXERR                        (1 << 16)
#define RPI_PCM_CS_TXERR                        (1 << 15)
#define RPI_PCM_CS_RXSYNC                       (1 << 14)
#define RPI_PCM_CS_TXSYNC                       (1 << 13)
#define RPI_PCM_CS_DMAEN                        (1 << 9)
#define RPI_PCM_CS_RXTHR(val)                   ((val & 0x03) << 7)
#define RPI_PCM_CS_TXTHR(val)                   ((val & 0x03) << 5)
#define RPI_PCM_CS_RXCLR                        (1 << 4)
#define RPI_PCM_CS_TXCLR                        (1 << 3)
#define RPI_PCM_CS_TXON                         (1 << 2)
#define RPI_PCM_CS_RXON                         (1 << 1)
#define RPI_PCM_CS_EN                           (1 << 0)
    uint32_t fifo;
    uint32_t mode;
#define RPI_PCM_MODE_CLK_DIS                    (1 << 28)
#define RPI_PCM_MODE_PDMN                       (1 << 27)
#define RPI_PCM_MODE_PDME                       (1 << 26)
#define RPI_PCM_MODE_FRXP                       (1 << 25)
#define RPI_PCM_MODE_FTXP                       (1 << 24)
#define RPI_PCM_MODE_CLKM                       (1 << 23)
#define RPI_PCM_MODE_CLKI                       (1 << 22)
#define RPI_PCM_MODE_FSM                        (1 << 21)
#define RPI_PCM_MODE_FSI                        (1 << 20)
#define RPI_PCM_MODE_FLEN(val)                  ((val & 0x3ff) << 10)
#define RPI_PCM_MODE_FSLEN(val)                 ((val & 0x3ff) << 0)
    uint32_t rxc;
#define RPI_PCM_RXC_CH1WEX                      (1 << 31)
#define RPI_PCM_RXC_CH1EN                       (1 << 30)
#define RPI_PCM_RXC_CH1POS(val)                 ((val & 0x3ff) << 20)
#define RPI_PCM_RXC_CH1WID(val)                 ((val & 0x0f) << 16)
#define RPI_PCM_RXC_CH2WEX                      (1 << 15)
#define RPI_PCM_RXC_CH2EN                       (1 << 14)
#define RPI_PCM_RXC_CH2POS(val)                 ((val & 0x3ff) << 4)
#define RPI_PCM_RXC_CH2WID(val)                 ((val & 0x0f) << 0)
    uint32_t txc;
#define RPI_PCM_TXC_CH1WEX                      (1 << 31)
#define RPI_PCM_TXC_CH1EN                       (1 << 30)
#define RPI_PCM_TXC_CH1POS(val)                 ((val & 0x3ff) << 20)
#define RPI_PCM_TXC_CH1WID(val)                 ((val & 0x0f) << 16)
#define RPI_PCM_TXC_CH2WEX                      (1 << 15)
#define RPI_PCM_TXC_CH2EN                       (1 << 14)
#define RPI_PCM_TXC_CH2POS(val)                 ((val & 0x3ff) << 4)
#define RPI_PCM_TXC_CH2WID(val)                 ((val & 0x0f) << 0)
    uint32_t dreq;
#define RPI_PCM_DREQ_TX_PANIC(val)              ((val & 0x7f) << 24)
#define RPI_PCM_DREQ_RX_PANIC(val)              ((val & 0x7f) << 16)
#define RPI_PCM_DREQ_TX(val)                    ((val & 0x7f) << 8)
#define RPI_PCM_DREQ_RX(val)                    ((val & 0x7f) << 0)
    uint32_t inten;
#define RPI_PCM_INTEN_RXERR                     (1 << 3)
#define RPI_PCM_INTEN_TXERR                     (1 << 2)
#define RPI_PCM_INTEN_RXR                       (1 << 1)
#define RPI_PCM_INTEN_TXW                       (1 << 0)
    uint32_t intstc;
#define RPI_PCM_INTSTC_RXERR                    (1 << 3)
#define RPI_PCM_INTSTC_TXERR                    (1 << 2)
#define RPI_PCM_INTSTC_RXR                      (1 << 1)
#define RPI_PCM_INTSTC_TXW                      (1 << 0)
    uint32_t gray;
#define RPI_PCM_GRAY_RXFIFOLEVEL(val)           ((val & 0x3f) << 16)
#define RPI_PCM_GRAY_FLUSHED(val)               ((val & 0x3f) << 10
#define RPI_PCM_GRAY_RXLEVEL(val)               ((val & 0x3f) << 4)
#define RPI_PCM_GRAY_FLUSH                      (1 << 2)
#define RPI_PCM_GRAY_CLR                        (1 << 1)
#define RPI_PCM_GRAY_EN                         (1 << 0)
} __attribute__((packed, aligned(4))) pcm_t;


#define PCM_OFFSET                               (0x00203000)
#define PCM_PERIPH_PHYS                          (0x7e203000)

#define NUM_PCMFUNS        4
#define PCMFUN_CLK         0
#define PCMFUN_FS          1
#define PCMFUN_DIN         2
#define PCMFUN_DOUT        3

typedef struct
{
    int pinnum;
    int altnum;
} pcm_pin_table_t;

typedef struct
{
    const int count;
    const pcm_pin_table_t *pins;
} pcm_pin_tables_t;


int pcm_pin_alt(int pcmfun, int pinnum);


#endif /* __PCM_H__ */
