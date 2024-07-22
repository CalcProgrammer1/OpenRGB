/*
 * pcm.c
 *
 * Copyright (c) 2014 Jeremy Garff <jer @ jers.net>
 * PCM version Copyright (c) 2016 Ton van Overbeek <tvoverbeek @ gmail.com>
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
#include "pcm.h"


// Mapping of Pin to alternate function for PCM_CLK
const pcm_pin_table_t pcm_pin_clk[] =
{
    {
        .pinnum = 18,
        .altnum = 0,
    },
    {
        .pinnum = 28,
        .altnum = 2,
    },
};

// Mapping of Pin to alternate function for PCM_FS
const pcm_pin_table_t pcm_pin_fs[] =
{
    {
        .pinnum = 19,
        .altnum = 0,
    },
    {
        .pinnum = 29,
        .altnum = 2,
    },
};

// Mapping of Pin to alternate function for PCM_DIN
const pcm_pin_table_t pcm_pin_din[] =
{
    {
        .pinnum = 20,
        .altnum = 0,
    },
    {
        .pinnum = 30,
        .altnum = 2,
    },
};

// Mapping of Pin to alternate function for PCM_DOUT
const pcm_pin_table_t pcm_pin_dout[] =
{
    {
        .pinnum = 21,
        .altnum = 0,
    },
    {
        .pinnum = 31,
        .altnum = 2,
    },
};

const pcm_pin_tables_t pcm_pin_tables[NUM_PCMFUNS] =
{
    {
        .pins = pcm_pin_clk,
        .count = sizeof(pcm_pin_clk) / sizeof(pcm_pin_clk[0]),
    },
    {
        .pins = pcm_pin_fs,
        .count = sizeof(pcm_pin_fs) / sizeof(pcm_pin_fs[0]),
    },
    {
        .pins = pcm_pin_din,
        .count = sizeof(pcm_pin_din) / sizeof(pcm_pin_din[0]),
    },
    {
        .pins = pcm_pin_dout,
        .count = sizeof(pcm_pin_dout) / sizeof(pcm_pin_dout[0]),
    },
};


int pcm_pin_alt(int pcmfun, int pinnum)
{
    if (pcmfun < 0 || pcmfun > 3) {
        return -1;
    }
    const pcm_pin_tables_t *pintable = &pcm_pin_tables[pcmfun];
    int i;

    for (i = 0; i < pintable->count; i++)
    {
        if (pintable->pins[i].pinnum == pinnum)
        {
            return pintable->pins[i].altnum;
        }
    }

    return -1;
}

