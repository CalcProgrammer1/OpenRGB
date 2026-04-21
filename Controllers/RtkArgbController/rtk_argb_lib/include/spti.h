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

#ifndef _SPTI_H_
#define _SPTI_H_

#include "scsi.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "scsi.h"

#define MAX_SPTI_RETRY 5

#define SPT_CDB_LENGTH 32
#define SPT_SENSE_LENGTH 32
#define SPTWB_DATA_LENGTH 4096


#ifdef _WIN32

#include <windows.h>
#include <ntddscsi.h>

typedef struct _SCSI_PASS_THROUGH_WITH_BUFFERS {
	SCSI_PASS_THROUGH spt;
	ULONG             Filler;      // realign buffers to double word boundary
	UCHAR             ucSenseBuf[SPT_SENSE_LENGTH];
	UCHAR             ucDataBuf[SPTWB_DATA_LENGTH];
} SCSI_PASS_THROUGH_WITH_BUFFERS, *PSCSI_PASS_THROUGH_WITH_BUFFERS;

typedef struct _SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER {
	SCSI_PASS_THROUGH_DIRECT sptd;
	ULONG             Filler;      // realign buffer to double word boundary
	UCHAR             ucSenseBuf[SPT_SENSE_LENGTH];
} SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, *PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER;

#else

#define IOCTL_SCSI_PASS_THROUGH _IOWR('F', 1, SCSI_PASS_THROUGH_WITH_BUFFERS)
#define SCSI_IOCTL_DATA_OUT          0
#define SCSI_IOCTL_DATA_IN           1

typedef struct _SCSI_PASS_THROUGH
{
	unsigned short Length;
	unsigned char ScsiStatus;
	unsigned char PathId;
	unsigned char TargetId;
	unsigned char Lun;
	unsigned char CdbLength;
	unsigned char SenseInfoLength;
	unsigned char DataIn;
	int DataTransferLength;
	int TimeOutValue;
	int DataBufferOffset;
	int SenseInfoOffset;
	unsigned char Cdb[16];
} SCSI_PASS_THROUGH, *PSCSI_PASS_THROUGH;

typedef struct _SCSI_PASS_THROUGH_DIRECT {
	unsigned short Length;
	unsigned char ScsiStatus;
	unsigned char PathId;
	unsigned char TargetId;
	unsigned char Lun;
	unsigned char CdbLength;
	unsigned char SenseInfoLength;
	unsigned char DataIn;
	unsigned long DataTransferLength;
	unsigned long TimeOutValue;
	void *DataBuffer;
	unsigned long SenseInfoOffset;
	unsigned char Cdb[16];
} SCSI_PASS_THROUGH_DIRECT, *PSCSI_PASS_THROUGH_DIRECT;

typedef struct _S_SCSI_PASS_THROUGH_WITH_BUFFERS
{
	SCSI_PASS_THROUGH   spt;
	unsigned int        Filler; // realign buffers to double word boundary
	unsigned char       ucSenseBuf[SPT_SENSE_LENGTH];
	unsigned char       ucDataBuf[SPTWB_DATA_LENGTH];
} SCSI_PASS_THROUGH_WITH_BUFFERS, *PSCSI_PASS_THROUGH_WITH_BUFFERS;

typedef struct _SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER {
	SCSI_PASS_THROUGH_DIRECT sptd;
	unsigned long             Filler;      // realign buffer to double word boundary
	unsigned char             ucSenseBuf[SPT_SENSE_LENGTH];
} SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, *PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER;

#endif

int scsi_pass_thru_intf(dHandle fd,
			uint8_t *cdb,
			uint8_t *buf,
			int buflen,
			uint8_t is_in,
			int timeout,
			uint8_t *sense);

#ifdef __cplusplus
}
#endif

#endif /* _SPTI_H_ */

