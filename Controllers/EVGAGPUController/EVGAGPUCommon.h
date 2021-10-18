/*-------------------------------------------------------------------*\
|  EVGAGPUCommon.h                                                    |
|                                                                     |
|  Common classes and functions for EVGA GPUs                         |
|                                                                     |
|  Chris M (Dr_No)         11th July 2021                             |
\*-------------------------------------------------------------------*/

#pragma once

union u16_to_u8
{
    uint16_t   u16;
    struct
    {
        uint8_t LSB;
        uint8_t MSB;
    };
};

#define EVGA_DETECT_MESSAGE     "[%s] Found a device match at Bus %02d for Device %04x and SubDevice %04x: %s"