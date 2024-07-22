/*
 * pwm.c
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

#include "ws2811.h"

#include "pwm.h"


// Mapping of Pin to alternate function for PWM channel 0
const pwm_pin_table_t pwm_pin_chan0[] =
{
    {
        .pinnum = 12,
        .altnum = 0,
    },
    {
        .pinnum = 18,
        .altnum = 5,
    },
    {
        .pinnum = 40,
        .altnum = 0,
    },
};

// Mapping of Pin to alternate function for PWM channel 1
const pwm_pin_table_t pwm_pin_chan1[] =
{
    {
        .pinnum = 13,
        .altnum = 0,
    },
    {
        .pinnum = 19,
        .altnum = 5,
    },
    {
        .pinnum = 41,
        .altnum = 0,
    },
    {
        .pinnum = 45,
        .altnum = 0,
    },
};

const pwm_pin_tables_t pwm_pin_tables[RPI_PWM_CHANNELS] =
{
    {
        .pins = pwm_pin_chan0,
        .count = sizeof(pwm_pin_chan0) / sizeof(pwm_pin_chan0[0]),
    },
    {
        .pins = pwm_pin_chan1,
        .count = sizeof(pwm_pin_chan1) / sizeof(pwm_pin_chan1[0]),
    },
};


int pwm_pin_alt(int chan, int pinnum)
{
    const pwm_pin_tables_t *pintable = &pwm_pin_tables[chan];
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

