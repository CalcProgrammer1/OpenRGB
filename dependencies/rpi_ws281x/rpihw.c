/*
 * rpihw.c
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


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <byteswap.h>

#include "rpihw.h"


#define LINE_WIDTH_MAX                           80
#define HW_VER_STRING                            "Revision"

#define PERIPH_BASE_RPI                          0x20000000
#define PERIPH_BASE_RPI2                         0x3f000000
#define PERIPH_BASE_RPI4                         0xfe000000

#define VIDEOCORE_BASE_RPI                       0x40000000
#define VIDEOCORE_BASE_RPI2                      0xc0000000

#define RPI_MANUFACTURER_MASK                    (0xf << 16)
#define RPI_WARRANTY_MASK                        (0x3 << 24)

static const rpi_hw_t rpi_hw_info[] = {
    //
    // Raspberry Pi 400
    //
    {
        .hwver = 0xc03130,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 400 - 4GB v1.0"
    },
    {
        .hwver = 0xc03131,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 400 - 4GB v1.1"
    },

    //
    // Raspberry Pi 4
    //
    {
        .hwver = 0xA03111,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 1GB v1.1"
    },
    {
        .hwver = 0xB03111,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 2GB v.1.1"
    },
    {
        .hwver = 0xC03111,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 4GB v1.1"
    },
    {
        .hwver = 0xA03112,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 1GB v1.2"
    },
    {
        .hwver = 0xB03112,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 2GB v.1.2"
    },
    {
        .hwver = 0xC03112,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 4GB v1.2"
    },
    {
        .hwver = 0xb03114,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 2GB v1.4"
    },
    {
        .hwver = 0xD03114,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 8GB v1.4"
    },
    {
        .hwver = 0xc03114,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 4GB v1.4"
    },
    {
        .hwver = 0xa03115,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 1GB v1.5"
    },
    {
        .hwver = 0xb03115,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 2GB v1.5"
    },
    {
        .hwver = 0xc03115,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 4GB v1.5"
    },
    {
        .hwver = 0xd03115,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 4 Model B - 8GB v1.5"
    },

    //
    // Compute Module 4
    //
    {
        .hwver = 0xa03140,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Compute Module 4 v1.0 eMMC"
    },
    {
        .hwver = 0xb03140,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Compute Module 4 v1.0 Lite"
    },
    {
        .hwver = 0xc03140,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Compute Module 4 v1.0 WiFi"
    },
    {
        .hwver = 0xd03140,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Compute Module 4 v1.0 WiFi 8GB"
    },
    {
        .hwver = 0xa03141,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Compute Module 4 Rev 1.1"
    },
    {
        .hwver = 0xb03141,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Compute Module 4 Rev 1.1"
    },
    {
        .hwver = 0xc03141,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Compute Module 4 Rev 1.1"
    },
    {
        .hwver = 0xd03141,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Compute Module 4 Rev 1.1"
    },

    //
    // Model B Rev 1.0
    //
    {
        .hwver  = 0x02,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model B",
    },
    {
        .hwver  = 0x03,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model B",
    },

    //
    // Model B Rev 2.0
    //
    {
        .hwver  = 0x04,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model B",
    },
    {
        .hwver  = 0x05,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model B",
    },
    {
        .hwver  = 0x06,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model B",
    },

    //
    // Model A
    //
    {
        .hwver  = 0x07,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model A",
    },
    {
        .hwver  = 0x08,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model A",
    },
    {
        .hwver  = 0x09,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model A",
    },

    //
    // Model B
    //
    {
        .hwver  = 0x0d,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model B",
    },
    {
        .hwver  = 0x0e,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model B",
    },
    {
        .hwver  = 0x0f,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model B",
    },

    //
    // Model B+
    //
    {
        .hwver  = 0x10,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model B+",
    },
    {
        .hwver  = 0x13,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model B+",
    },
    {
        .hwver  = 0x900032,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model B+",
    },

    //
    // Compute Module
    //
    {
        .hwver  = 0x11,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Compute Module 1",
    },
    {
        .hwver  = 0x14,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Compute Module 1",
    },
    {
        .hwver  = 0xa020a0,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Compute Module 3/L3",
    },
    {
        .hwver  = 0xa02100,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Compute Module 3+",
    },
    {
            .hwver  = 0xa220a0,
            .type = RPI_HWVER_TYPE_PI2,
            .periph_base = PERIPH_BASE_RPI2,
            .videocore_base = VIDEOCORE_BASE_RPI2,
            .desc = "Compute Module 3 Rev 1.0",
    },

    //
    // Pi Zero
    //
    {
        .hwver  = 0x900092,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Pi Zero v1.2",
    },
    {
        .hwver  = 0x900093,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Pi Zero v1.3",
    },
    {
        .hwver  = 0x920093,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Pi Zero v1.3",
    },
    {
        .hwver  = 0x9200c1,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Pi Zero W v1.1",
    },
    {
        .hwver  = 0x9000c1,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Pi Zero W v1.1",
    },

    //
    // Model Zero 2 W
    //
    {
        .hwver  = 0x902120,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi Zero 2 W v1.0",
    },

    //
    // Model A+
    //
    {
        .hwver  = 0x12,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model A+",
    },
    {
        .hwver  = 0x15,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model A+",
    },
    {
        .hwver  = 0x900021,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Model A+",
    },

    //
    // Pi 2 Model B
    //
    {
        .hwver  = 0xa01041,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 2",
    },
    {
        .hwver  = 0xa01040,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 2",
    },
    {
        .hwver  = 0xa21041,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 2",
    },
    //
    // Pi 2 with BCM2837
    //
    {
        .hwver  = 0xa22042,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 2",
    },
    //
    // Pi 3 Model B
    //
    {
        .hwver  = 0xa020d4,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 3 B+",
    },
    {
        .hwver  = 0xa020d3,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 3 B+",
    },
    {
        .hwver  = 0xa32082,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 3 B",
    },
    {
        .hwver  = 0xa02082,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 3",
    },
    {
        .hwver  = 0xa02083,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 3",
    },
    {
        .hwver  = 0xa22082,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 3",
    },
    {
        .hwver  = 0xa22083,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Pi 3",
    },
    {
        .hwver  = 0x9020e0,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Model 3 A+",
    }
};


const rpi_hw_t *rpi_hw_detect(void)
{
    const rpi_hw_t *result = NULL;
    uint32_t rev;
    unsigned i;

#ifdef __aarch64__
    // On ARM64, read revision from /proc/device-tree as it is not shown in
    // /proc/cpuinfo
    FILE *f = fopen("/proc/device-tree/system/linux,revision", "r");
    if (!f)
    {
        return NULL;
    }
    size_t read = fread(&rev, 1, sizeof(uint32_t), f);
    if (read != sizeof(uint32_t))
        goto done;
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        rev = bswap_32(rev);  // linux,revision appears to be in big endian
    #endif

    for (i = 0; i < (sizeof(rpi_hw_info) / sizeof(rpi_hw_info[0])); i++)
    {
        uint32_t hwver = rpi_hw_info[i].hwver;
        if (rev == hwver)
        {
            result = &rpi_hw_info[i];

            goto done;
        }
    }
#else
    FILE *f = fopen("/proc/cpuinfo", "r");
    char line[LINE_WIDTH_MAX];

    if (!f)
    {
        return NULL;
    }

    while (fgets(line, LINE_WIDTH_MAX - 1, f))
    {
        if (strstr(line, HW_VER_STRING))
        {
            char *substr;

            substr = strstr(line, ": ");
            if (!substr)
            {
                continue;
            }

            errno = 0;
            rev = strtoul(&substr[1], NULL, 16);  // Base 16
            if (errno)
            {
                continue;
            }

            for (i = 0; i < (sizeof(rpi_hw_info) / sizeof(rpi_hw_info[0])); i++)
            {
                uint32_t hwver = rpi_hw_info[i].hwver;

                // Take out warranty and manufacturer bits
                hwver &= ~(RPI_WARRANTY_MASK | RPI_MANUFACTURER_MASK);
                rev &= ~(RPI_WARRANTY_MASK | RPI_MANUFACTURER_MASK);

                if (rev == hwver)
                {
                    result = &rpi_hw_info[i];

                    goto done;
                }
            }
        }
    }
#endif
done:
    fclose(f);

    return result;
}
