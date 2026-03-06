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

#ifndef _SCSI_H_
#define _SCSI_H_

#define RTK_READ_CMD  0xE2
#define RTK_WRITE_CMD 0xE3
#define RTK_NVME_READ_CMD   0xE4
#define RTK_NVME_WRITE_CMD  0xE5

#define SCSI_REQUEST_SENSE              0x03
#define SCSI_STD_INQUIRY                0x12
#define SCSI_REPORT_LUNS                0xa0

#define SCSI_SENSE_LEN 18

#define RTK_RCMD                        0x80
#define RTK_FWU_START                   0x01
#define RTK_FWU_WADDR                   0x10
#define RTK_FWU_WDATA                   0x11
#define RTK_FWU_RDATA                   (0x12 | RTK_RCMD)
#define RTK_RD_EFUSE                    (0x16 | RTK_RCMD)
#define RTK_RD_MEM                      (0x22 | RTK_RCMD)
#define RTK_WT_MEM                      0x23
#define RTK_GET_FW_VER                  (0x25 | RTK_RCMD)
#define FWU_GET_FLASHBOOT               (0x26 | RTK_RCMD)
#define RTK_OPAL_SET                    0x27
#define RTK_OPAL_GET                    (0x28 | RTK_RCMD)
#define RTK_FW_RESTART                  0x30
#define RTK_WR_PCIE_CONF                0x33
#define RTK_RD_PCIE_CONF                (0x33 | RTK_RCMD)
#define RTK_WR_PCIE_MEM                 0x34
#define RTK_RD_PCIE_MEM                 (0x34 | RTK_RCMD)
#define RTK_DUMP_FW_VARIABLES           (0x35 | RTK_RCMD)
#define RTK_READ_UART_STR               (0x38 | RTK_RCMD)
#define RTK_WRITE_UART_STR              (0x38)
#define RTK_CLEAR_UART_BUFFER           (0x3a)
#define RTK_GET_UART_RX_COUNT           (0x3a | RTK_RCMD)
#define RTK_GET_DISK_STATUS             (0x3b | RTK_RCMD)
#define RTK_I2C_READ                    (0x3c | RTK_RCMD)
#define RTK_I2C_WRITE                   (0x3d)
#define RTK_I2C_SET_ADDR                (0x3e)
#define RTK_I2C_INIT                    (0x3f)
#define RTK_GPIO_MUX_SEL                (0x41)
#define RTK_GET_IC_UUID                 (0x43 | RTK_RCMD)
#define RTK_SET_CUST_PAGE               (0x44)
#define RTK_GET_CUST_PAGE               (0x44 | RTK_RCMD)
#define RTK_GPIO_SET_VAL                (0x45)
#define RTK_GPIO_GET_VAL                (0x45 | RTK_RCMD)
#define RTK_GPIO_SET_DIR                (0x46)
#define RTK_GET_BUF_SIZE                (0x4a | RTK_RCMD)
#define RTK_SET_HW_DMA_MODE             (0x4b)
#define RTK_GET_HW_DMA_MODE             (0x4b | RTK_RCMD)
#define RTK_LED_SET_INFO                (0x4c)
#define RTK_LED_GET_INFO                (0x4c | RTK_RCMD)
#define RTK_SET_DISK_SPEED              (0x4d)
#define RTK_GET_DISK_SPEED              (0x4d | RTK_RCMD)
#define RTK_UART_INIT                   (0x4e)
#define RTK_SET_BRIDGE_RW_MD            (0x4f)
#define RTK_GET_BRIDGE_RW_MD            (0x4f | RTK_RCMD)
#define RTK_GET_LUN                     (0x50 | RTK_RCMD)

#define RTK_WR_RAID_MODE                (0x52)
#define RTK_RD_RAID_MODE                (0x52 | RTK_RCMD)

#define RTK_GET_HID_CH                  (0x5A | RTK_RCMD)

#define INQ_LEN 512
#define INQ_VENDOR_OFFSET 8
#define INQ_VENDOR_LEN 8
#define INQ_PRODUCT_OFFSET 16
#define INQ_PRODUCT_LEN 16

#define SCSI_TIMEOUT_DEFAULT 10

#endif
