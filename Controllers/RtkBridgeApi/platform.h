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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef CONFIG_HID
#include "hidapi.h"
#endif

#ifdef CONFIG_MS
#include "mass_storage.h"
#endif

struct usb_id {
	uint16_t vid;
	uint16_t pid;
};

#ifdef CONFIG_ARGB_DEV
#include "argb_dev.h"
typedef struct argb_device *dHandle;
#else
#include "bridge_dev.h"
typedef bridge_device *dHandle;
#endif

#ifndef INVALID_DHANDLE
#if defined(CONFIG_MS)
#ifdef __cplusplus
#define INVALID_DHANDLE(fd) ((reinterpret_cast<unsigned long int *>(fd) <= 0) ? true : false)
#else
#define INVALID_DHANDLE(fd) (((unsigned long int *)(fd) <= 0) ? true : false)
#endif
#else
#ifdef _WIN32
#ifdef __cplusplus
#define INVALID_DHANDLE(fd) ((reinterpret_cast<ULONGLONG>(fd) <= 0) ? true : false)
#else
#define INVALID_DHANDLE(fd) (((ULONGLONG)(fd) <= 0) ? true : false)
#endif
#elif defined(__APPLE__) || defined(__linux__)
#define INVALID_DHANDLE(fd) ((fd <= 0) ? true : false)
#endif // _WIN32
#endif
#endif // !INVALID_DHANDLE

#ifndef NULL_DHANDLE
#ifdef _WIN32
#define NULL_DHANDLE NULL
#elif defined(__APPLE__) || defined(__linux__)
#define NULL_DHANDLE 0
#endif // _WIN32
#endif // !NULL_DHANDLE

#ifdef WIN32
#define _Align(n) __declspec(align(n))
#else
#define _Align(n) __attribute__((aligned(n)))
#endif // WIN32

#ifdef _WIN32
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#else
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif // WIN32

#ifndef _WIN32
#ifndef MIN
#define MIN
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#endif // WIN32

#endif
