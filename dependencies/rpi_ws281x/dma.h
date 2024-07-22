/*
 * dma.h
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

#ifndef __DMA_H__
#define __DMA_H__

#include <stdint.h>

/*
 * DMA Control Block in Main Memory
 *
 * Note: Must start at a 256 byte aligned address.
 *       Use corresponding register field definitions.
 */
typedef struct
{
    uint32_t ti;
    uint32_t source_ad;
    uint32_t dest_ad;
    uint32_t txfr_len;
    uint32_t stride;
    uint32_t nextconbk;
    uint32_t resvd_0x18[2];
} __attribute__((packed, aligned(4))) dma_cb_t;

/*
 * DMA register set
 */
typedef struct
{
    uint32_t cs;
#define RPI_DMA_CS_RESET                         (1 << 31)
#define RPI_DMA_CS_ABORT                         (1 << 30)
#define RPI_DMA_CS_DISDEBUG                      (1 << 29)
#define RPI_DMA_CS_WAIT_OUTSTANDING_WRITES       (1 << 28)
#define RPI_DMA_CS_PANIC_PRIORITY(val)           ((val & 0xf) << 20)                
#define RPI_DMA_CS_PRIORITY(val)                 ((val & 0xf) << 16)
#define RPI_DMA_CS_ERROR                         (1 << 8)
#define RPI_DMA_CS_WAITING_OUTSTANDING_WRITES    (1 << 6)
#define RPI_DMA_CS_DREQ_STOPS_DMA                (1 << 5)
#define RPI_DMA_CS_PAUSED                        (1 << 4)
#define RPI_DMA_CS_DREQ                          (1 << 3)
#define RPI_DMA_CS_INT                           (1 << 2)
#define RPI_DMA_CS_END                           (1 << 1)
#define RPI_DMA_CS_ACTIVE                        (1 << 0)
    uint32_t conblk_ad;
    uint32_t ti;
#define RPI_DMA_TI_NO_WIDE_BURSTS                (1 << 26)
#define RPI_DMA_TI_WAITS(val)                    ((val & 0x1f) << 21)
#define RPI_DMA_TI_PERMAP(val)                   ((val & 0x1f) << 16)
#define RPI_DMA_TI_BURST_LENGTH(val)             ((val & 0xf) << 12)
#define RPI_DMA_TI_SRC_IGNORE                    (1 << 11)
#define RPI_DMA_TI_SRC_DREQ                      (1 << 10)
#define RPI_DMA_TI_SRC_WIDTH                     (1 << 9)
#define RPI_DMA_TI_SRC_INC                       (1 << 8)
#define RPI_DMA_TI_DEST_IGNORE                   (1 << 7)
#define RPI_DMA_TI_DEST_DREQ                     (1 << 6)
#define RPI_DMA_TI_DEST_WIDTH                    (1 << 5)
#define RPI_DMA_TI_DEST_INC                      (1 << 4)
#define RPI_DMA_TI_WAIT_RESP                     (1 << 3)
#define RPI_DMA_TI_TDMODE                        (1 << 1)
#define RPI_DMA_TI_INTEN                         (1 << 0)
    uint32_t source_ad;
    uint32_t dest_ad;
    uint32_t txfr_len;
#define RPI_DMA_TXFR_LEN_YLENGTH(val)            ((val & 0xffff) << 16)
#define RPI_DMA_TXFR_LEN_XLENGTH(val)            ((val & 0xffff) << 0)
    uint32_t stride;
#define RPI_DMA_STRIDE_D_STRIDE(val)             ((val & 0xffff) << 16)
#define RPI_DMA_STRIDE_S_STRIDE(val)             ((val & 0xffff) << 0)
    uint32_t nextconbk;
    uint32_t debug;
} __attribute__((packed, aligned(4))) dma_t;


#define DMA0_OFFSET                              (0x00007000)
#define DMA1_OFFSET                              (0x00007100)
#define DMA2_OFFSET                              (0x00007200)
#define DMA3_OFFSET                              (0x00007300)
#define DMA4_OFFSET                              (0x00007400)
#define DMA5_OFFSET                              (0x00007500)
#define DMA6_OFFSET                              (0x00007600)
#define DMA7_OFFSET                              (0x00007700)
#define DMA8_OFFSET                              (0x00007800)
#define DMA9_OFFSET                              (0x00007900)
#define DMA10_OFFSET                             (0x00007a00)
#define DMA11_OFFSET                             (0x00007b00)
#define DMA12_OFFSET                             (0x00007c00)
#define DMA13_OFFSET                             (0x00007d00)
#define DMA14_OFFSET                             (0x00007e00)
#define DMA15_OFFSET                             (0x00e05000)


#define PAGE_SIZE                                (1 << 12)
#define PAGE_MASK                                (~(PAGE_SIZE - 1))
#define PAGE_OFFSET(page)                        (page & (PAGE_SIZE - 1))


uint32_t dmanum_to_offset(int dmanum);

#endif /* __DMA_H__ */
