/*---------------------------------------------------------*\
| AsusAuraTUFKeyboardLayouts.h                              |
|                                                           |
|   Layouts for ASUS Aura TUF keyboard                      |
|                                                           |
|   Mola19                                      02 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <map>
#include <vector>
#include "RGBControllerKeyNames.h"
#include "RGBController.h"

enum
{
    ASUS_TUF_K7_LAYOUT_CA    = 1,
    ASUS_TUF_K7_LAYOUT_AR    = 2,
    ASUS_TUF_K7_LAYOUT_DE    = 3,
    ASUS_TUF_K7_LAYOUT_UK    = 4,
    ASUS_TUF_K7_LAYOUT_FR    = 5,
    ASUS_TUF_K7_LAYOUT_CN    = 6,
    ASUS_TUF_K7_LAYOUT_HU    = 7,
    ASUS_TUF_K7_LAYOUT_IT    = 8,
    ASUS_TUF_K7_LAYOUT_TH    = 9,
    ASUS_TUF_K7_LAYOUT_UA    = 10,
    ASUS_TUF_K7_LAYOUT_NO    = 11,
    ASUS_TUF_K7_LAYOUT_PT    = 12,
    ASUS_TUF_K7_LAYOUT_HE    = 13,
    ASUS_TUF_K7_LAYOUT_RU    = 14,
    ASUS_TUF_K7_LAYOUT_ES    = 15,
    ASUS_TUF_K7_LAYOUT_TW    = 16,
    ASUS_TUF_K7_LAYOUT_US    = 17,
    ASUS_TUF_K7_LAYOUT_TR    = 18,
    ASUS_TUF_K7_LAYOUT_CZ    = 19,
    ASUS_TUF_K7_LAYOUT_BE    = 20,
    ASUS_TUF_K7_LAYOUT_JP    = 21,
    ASUS_TUF_K7_LAYOUT_KR    = 22,
    ASUS_TUF_K7_LAYOUT_IS    = 23,
    ASUS_TUF_K7_LAYOUT_WB    = 24,
    ASUS_TUF_K7_LAYOUT_SW_CH = 25
};

#define NA  0xFFFFFFFF

struct led_value
{
    const char*         name;
    unsigned char       id;
};

struct layout_info
{
    unsigned int*               matrix_map;
    int                         size;
    int                         rows;
    int                         cols;
    std::vector<led_value>      led_names;
};

static unsigned int ASUS_TUF_K7_LAYOUT_KEYS_ANSI[6][24] =
{
    {   0,  NA,   8,  14,  19,  24,  NA,  34,  39,  44,  49,  55,  61,  66,  70,  NA,  74,  78,  83,  NA,  NA,  NA,  NA,  NA },
    {   1,   6,   9,  15,  20,  25,  29,  35,  40,  45,  50,  56,  62,  67,  NA,  NA,  75,  79,  84,  NA,  87,  92,  96, 101 },
    {   2,  NA,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  71,  NA,  76,  80,  85,  NA,  88,  93,  97, 102 },
    {   3,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  NA,  72,  NA,  NA,  NA,  NA,  NA,  89,  94,  98,  NA },
    {   4,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  NA,  69,  NA,  NA,  NA,  81,  NA,  NA,  90,  95,  99, 103 },
    {   5,   7,  13,  NA,  NA,  NA,  33,  NA,  NA,  NA,  54,  60,  65,  NA,  73,  NA,  77,  82,  86,  NA,  91,  NA, 100,  NA }
};

static unsigned int ASUS_TUF_K7_LAYOUT_KEYS_ISO[6][24] =
{
    {   0,  NA,   9,  15,  20,  25,  NA,  35,  40,  45,  50,  56,  62,  67,  72,  NA,  75,  79,  84,  NA,  NA,  NA,  NA,  NA },
    {   1,   6,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  NA,  NA,  76,  80,  85,  NA,  88,  93,  97, 102 },
    {   2,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  69,  NA,  NA,  77,  81,  86,  NA,  89,  94,  98, 103 },
    {   3,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  65,  70,  73,  NA,  NA,  NA,  NA,  NA,  90,  95,  99,  NA },
    {   4,   7,  13,  19,  24,  29,  33,  39,  44,  49,  54,  60,  NA,  71,  NA,  NA,  NA,  82,  NA,  NA,  91,  96, 100, 104 },
    {   5,   8,  14,  NA,  NA,  NA,  34,  NA,  NA,  NA,  55,  61,  66,  NA,  74,  NA,  78,  83,  87,  NA,  92,  NA, 101,  NA }
};

static unsigned int ASUS_ROG_AZOTH_LAYOUT_KEYS_US[6][16] = {
    {   0,   6,   9,  15,  20,  25,  30,  36,  41,  46,  51,  57,  63,  NA,  NA,  NA },
    {   1,   7,  10,  16,  21,  26,  31,  37,  42,  47,  52,  58,  64,  69,  NA,  76 },
    {   2,  NA,  11,  17,  22,  27,  32,  38,  43,  48,  53,  59,  65,  70,  72,  77 },
    {   3,  NA,  12,  18,  23,  28,  33,  39,  44,  49,  54,  60,  66,  NA,  73,  78 },
    {   4,  NA,  13,  19,  24,  29,  34,  40,  45,  50,  55,  61,  67,  NA,  74,  79 },
    {   5,   8,  14,  NA,  NA,  NA,  35,  NA,  NA,  NA,  56,  62,  68,  71,  75,  80 }
};

static unsigned int ASUS_ROG_AZOTH_LAYOUT_KEYS_UK[6][16] = {
    {   0,   6,  10,  16,  21,  26,  31,  37,  42,  47,  52,  58,  64,  NA,  NA,  NA },
    {   1,   7,  11,  17,  22,  27,  32,  38,  43,  48,  53,  59,  65,  70,  NA,  77 },
    {   2,  NA,  12,  18,  23,  28,  33,  39,  44,  49,  54,  60,  66,  71,  NA,  78 },
    {   3,  NA,  13,  19,  24,  29,  34,  40,  45,  50,  55,  61,  67,  72,  74,  79 },
    {   4,   8,  14,  20,  25,  30,  35,  41,  46,  51,  56,  62,  68,  NA,  75,  80 },
    {   5,   9,  15,  NA,  NA,  NA,  36,  NA,  NA,  NA,  57,  63,  69,  73,  76,  81 }
};

static unsigned int ASUS_ROG_STRIX_SCOPE_LAYOUT_KEYS_ANSI[6][24] =
{
    {   0,  NA,   8,  14,  19,  24,  NA,  34,  39,  44,  49,  55,  61,  66,  70,  NA,  74,  78,  83,  NA,  NA,  NA, 104, 105 },
    {   1,   6,   9,  15,  20,  25,  29,  35,  40,  45,  50,  56,  62,  67,  NA,  NA,  75,  79,  84,  NA,  87,  92,  96, 101 },
    {   2,  NA,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  71,  NA,  76,  80,  85,  NA,  88,  93,  97, 102 },
    {   3,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  NA,  72,  NA,  NA,  NA,  NA,  NA,  89,  94,  98,  NA },
    {   4,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  NA,  69,  NA,  NA,  NA,  81,  NA,  NA,  90,  95,  99, 103 },
    {   5,   7,  13,  NA,  NA,  NA,  33,  NA,  NA,  NA,  54,  60,  65,  NA,  73,  NA,  77,  82,  86,  NA,  91,  NA, 100,  NA }
};


static unsigned int ASUS_ROG_STRIX_SCOPE_LAYOUT_KEYS_ISO[6][24] =
{
    {   0,  NA,   9,  15,  20,  25,  NA,  35,  40,  45,  50,  56,  62,  67,  72,  NA,  75,  79,  84,  NA,  NA,  NA, 105, 106 },
    {   1,   6,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  NA,  NA,  76,  80,  85,  NA,  88,  93,  97, 102 },
    {   2,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  69,  NA,  NA,  77,  81,  86,  NA,  89,  94,  98, 103 },
    {   3,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  65,  70,  73,  NA,  NA,  NA,  NA,  NA,  90,  95,  99,  NA },
    {   4,   7,  13,  19,  24,  29,  33,  39,  44,  49,  54,  60,  NA,  71,  NA,  NA,  NA,  82,  NA,  NA,  91,  96, 100, 104 },
    {   5,   8,  14,  NA,  NA,  NA,  34,  NA,  NA,  NA,  55,  61,  66,  NA,  74,  NA,  78,  83,  87,  NA,  92,  NA, 101,  NA }
};

static unsigned int ASUS_ROG_STRIX_SCOPE_II_LAYOUT_KEYS_ANSI[6][24] =
{
    {   0,  NA,   8,  14,  19,  24,  NA,  35,  41,  46,  51,  57,  63,  68,  72,  NA,  76,  80,  85,  NA,  NA,  94,  NA,  NA },
    {   1,   6,   9,  15,  20,  25,  30,  36,  42,  47,  52,  58,  64,  69,  NA,  NA,  77,  81,  86,  NA,  89,  95,  99, 104 },
    {   2,  NA,  10,  16,  21,  26,  31,  37,  43,  48,  53,  59,  65,  70,  73,  NA,  78,  82,  87,  NA,  90,  96, 100, 105 },
    {   3,  NA,  11,  17,  22,  27,  32,  38,  44,  49,  54,  60,  66,  NA,  74,  NA,  NA,  NA,  NA,  NA,  91,  97, 101,  NA },
    {   4,  NA,  12,  18,  23,  28,  33,  39,  45,  50,  55,  61,  NA,  71,  NA,  NA,  NA,  83,  NA,  NA,  92,  98, 102, 106 },
    {   5,   7,  13,  NA,  NA,  29,  34,  40,  NA,  NA,  56,  62,  67,  NA,  75,  NA,  79,  84,  88,  NA,  93,  NA, 103,  NA }
};


static unsigned int ASUS_ROG_STRIX_SCOPE_II_LAYOUT_KEYS_ISO[6][24] =
{
    {   0,  NA,   9,  15,  20,  25,  NA,  36,  42,  47,  52,  58,  64,  69,  74,  NA,  77,  81,  86,  NA,  NA,  95,  NA,  NA },
    {   1,   6,  10,  16,  21,  26,  31,  37,  43,  48,  53,  59,  65,  70,  NA,  NA,  78,  82,  87,  NA,  90,  96, 100, 105 },
    {   2,  NA,  11,  17,  22,  27,  32,  38,  44,  49,  54,  60,  66,  71,  NA,  NA,  79,  83,  88,  NA,  91,  97, 101, 106 },
    {   3,  NA,  12,  18,  23,  28,  33,  39,  45,  50,  55,  61,  67,  72,  75,  NA,  NA,  NA,  NA,  NA,  92,  98, 102,  NA },
    {   4,   7,  13,  19,  24,  29,  34,  40,  46,  51,  56,  62,  NA,  73,  NA,  NA,  NA,  84,  NA,  NA,  93,  99, 103, 107 },
    {   5,   8,  14,  NA,  NA,  30,  35,  41,  NA,  NA,  57,  63,  68,  NA,  76,  NA,  80,  85,  89,  NA,  94,  NA, 104,  NA }
};

static unsigned int ASUS_ROG_STRIX_SCOPE_II_96_WIRELESS_LAYOUT_KEYS_ANSI[6][19] =
{
    {   0,   6,  11,  17,  22,  27,  33,  39,  45,  50,  55,  61,  67,  72,  77,  80,  86,  NA,  97 },
    {   1,   7,  12,  18,  23,  28,  34,  40,  46,  51,  56,  62,  68,  73,  NA,  81,  87,  92,  98 },
    {   2,   8,  13,  19,  24,  29,  35,  41,  47,  52,  57,  63,  69,  74,  NA,  82,  88,  93,  99 },
    {   3,   9,  14,  20,  25,  30,  36,  42,  48,  53,  58,  64,  NA,  75,  NA,  83,  89,  94,  NA },
    {   4,  NA,  15,  21,  26,  31,  37,  43,  49,  54,  59,  65,  70,  NA,  78,  84,  90,  95, 100 },
    {   5,  10,  16,  NA,  NA,  32,  38,  44,  NA,  NA,  60,  66,  71,  76,  79,  85,  91,  96,  NA }
};

static unsigned int ASUS_ROG_STRIX_SCOPE_II_96_WIRELESS_LAYOUT_KEYS_ISO[6][19] =
{
    {   0,   6,  12,  18,  23,  28,  34,  40,  46,  51,  56,  62,  68,  74,  78,  81,  87,  NA,  98 },
    {   1,   7,  13,  19,  24,  29,  35,  41,  47,  52,  57,  63,  69,  75,  NA,  82,  88,  93,  99 },
    {   2,   8,  14,  20,  25,  30,  36,  42,  48,  53,  58,  64,  70,  NA,  NA,  83,  89,  94, 100 },
    {   3,   9,  15,  21,  26,  31,  37,  43,  49,  54,  59,  65,  71,  76,  NA,  84,  90,  95,  NA },
    {   4,  10,  16,  22,  27,  32,  38,  44,  50,  55,  60,  66,  72,  NA,  79,  85,  91,  96, 101 },
    {   5,  11,  17,  NA,  NA,  33,  39,  45,  NA,  NA,  61,  67,  73,  77,  80,  86,  92,  97,  NA }
};

static unsigned int ASUS_ROG_STRIX_FLARE_LAYOUT_KEYS_ANSI[6][26] =
{
    {   NA,  0,  NA,   8,  14,  19,  24,  NA,  34,  39,  44,  49,  55,  61,  66,  70,  NA,  74,  78,  83,  NA, 104,  NA,  NA,  NA,  NA },
    {   NA,  1,   6,   9,  15,  20,  25,  29,  35,  40,  45,  50,  56,  62,  67,  NA,  NA,  75,  79,  84,  NA,  87,  92,  96, 101,  NA },
    {   NA,  2,  NA,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  71,  NA,  76,  80,  85,  NA,  88,  93,  97, 102,  NA },
    {  105,  3,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  NA,  72,  NA,  NA,  NA,  NA,  NA,  89,  94,  98,  NA, 106 },
    {   NA,  4,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  NA,  69,  NA,  NA,  NA,  81,  NA,  NA,  90,  95,  99, 103,  NA },
    {   NA,  5,   7,  13,  NA,  NA,  NA,  33,  NA,  NA,  NA,  54,  60,  65,  NA,  73,  NA,  77,  82,  86,  NA,  91,  NA, 100,  NA,  NA }
};

static unsigned int ASUS_ROG_STRIX_FLARE_LAYOUT_KEYS_ISO[6][26] =
{
    {    NA,  0,  NA,   9,  15,  20,  25,  NA,  35,  40,  45,  50,  56,  62,  67,  72,  NA,  75,  79,  84,  NA, 105,  NA,  NA,  NA,  NA },
    {    NA,  1,   6,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  NA,  NA,  76,  80,  85,  NA,  88,  93,  97, 102,  NA },
    {    NA,  2,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  69,  NA,  NA,  77,  81,  86,  NA,  89,  94,  98, 103,  NA },
    {   106,  3,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  65,  70,  73,  NA,  NA,  NA,  NA,  NA,  90,  95,  99,  NA, 107 },
    {    NA,  4,   7,  13,  19,  24,  29,  33,  39,  44,  49,  54,  60,  NA,  71,  NA,  NA,  NA,  82,  NA,  NA,  91,  96, 100, 104,  NA },
    {    NA,  5,   8,  14,  NA,  NA,  NA,  34,  NA,  NA,  NA,  55,  61,  66,  NA,  74,  NA,  78,  83,  87,  NA,  92,  NA, 101,  NA,  NA }
};

static unsigned int ASUS_ROG_STRIX_FLARE_II_LAYOUT_KEYS_ANSI[7][30] =
{
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 104, 105,  NA,  NA },
    {   0,  NA,   8,  14,  19,  24,  NA,  34,  39,  44,  49,  55,  61,  66,  70,  NA,  NA,  NA,  NA,  74,  78,  83,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA },
    {   1,   6,   9,  15,  20,  25,  29,  35,  40,  45,  50,  56,  62,  67,  NA,  NA,  NA,  NA,  NA,  75,  79,  84,  NA,  NA,  NA,  NA,  87,  92,  96, 101 },
    {   2,  NA,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  71,  NA,  NA,  NA,  NA,  76,  80,  85,  NA,  NA,  NA,  NA,  88,  93,  97, 102 },
    {   3,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  NA,  72,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  89,  94,  98,  NA },
    {   4,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  NA,  69,  NA,  NA,  NA,  NA,  NA,  NA,  81,  NA,  NA,  NA,  NA,  NA,  90,  95,  99, 103 },
    {   5,   7,  13,  NA,  NA,  NA,  33,  NA,  NA,  NA,  54,  60,  65,  NA,  73,  NA,  NA,  NA,  NA,  77,  82,  86,  NA,  NA,  NA,  NA,  91,  NA, 100,  NA },
};

static unsigned int ASUS_ROG_STRIX_FLARE_II_LAYOUT_KEYS_ISO[7][30] =
{
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 105, 106,  NA,  NA },
    {   0,  NA,   9,  15,  20,  25,  NA,  35,  40,  45,  50,  56,  62,  67,  72,  NA,  NA,  NA,  NA,  75,  79,  84,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA },
    {   1,   6,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  NA,  NA,  NA,  NA,  NA,  76,  80,  85,  NA,  NA,  NA,  NA,  88,  93,  97, 102 },
    {   2,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  69,  NA,  NA,  NA,  NA,  NA,  77,  81,  86,  NA,  NA,  NA,  NA,  89,  94,  98, 103 },
    {   3,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  65,  70,  73,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  90,  95,  99,  NA },
    {   4,   7,  13,  19,  24,  29,  33,  39,  44,  49,  54,  60,  NA,  71,  NA,  NA,  NA,  NA,  NA,  NA,  82,  NA,  NA,  NA,  NA,  NA,  91,  96, 100, 104 },
    {   5,   8,  14,  NA,  NA,  NA,  34,  NA,  NA,  NA,  55,  61,  66,  NA,  74,  NA,  NA,  NA,  NA,  78,  83,  87,  NA,  NA,  NA,  NA,  92,  NA, 101,  NA },
};

static unsigned int ASUS_ROG_STRIX_FLARE_II_ANIMATE_LAYOUT_KEYS_ANSI[7][30] =
{
    {   0,  NA,   8,  14,  19,  24,  NA,  34,  39,  44,  49,  55,  61,  66,  70,  NA,  NA,  NA,  NA,  74,  78,  83,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA },
    {   1,   6,   9,  15,  20,  25,  29,  35,  40,  45,  50,  56,  62,  67,  NA,  NA,  NA,  NA,  NA,  75,  79,  84,  NA,  NA,  NA,  NA,  87,  92,  96, 101 },
    {   2,  NA,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  71,  NA,  NA,  NA,  NA,  76,  80,  85,  NA,  NA,  NA,  NA,  88,  93,  97, 102 },
    {   3,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  NA,  72,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  89,  94,  98,  NA },
    {   4,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  NA,  69,  NA,  NA,  NA,  NA,  NA,  NA,  81,  NA,  NA,  NA,  NA,  NA,  90,  95,  99, 103 },
    {   5,   7,  13,  NA,  NA,  NA,  33,  NA,  NA,  NA,  54,  60,  65,  NA,  73,  NA,  NA,  NA,  NA,  77,  82,  86,  NA,  NA,  NA,  NA,  91,  NA, 100,  NA },
    { 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133 }
};

static unsigned int ASUS_ROG_STRIX_FLARE_II_ANIMATE_LAYOUT_KEYS_ISO[7][30] =
{
    {   0,  NA,   9,  15,  20,  25,  NA,  35,  40,  45,  50,  56,  62,  67,  72,  NA,  NA,  NA,  NA,  75,  79,  84,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA },
    {   1,   6,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  NA,  NA,  NA,  NA,  NA,  76,  80,  85,  NA,  NA,  NA,  NA,  88,  93,  97, 102 },
    {   2,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  69,  NA,  NA,  NA,  NA,  NA,  77,  81,  86,  NA,  NA,  NA,  NA,  89,  94,  98, 103 },
    {   3,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  65,  70,  73,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  90,  95,  99,  NA },
    {   4,   7,  13,  19,  24,  29,  33,  39,  44,  49,  54,  60,  NA,  71,  NA,  NA,  NA,  NA,  NA,  NA,  82,  NA,  NA,  NA,  NA,  NA,  91,  96, 100, 104 },
    {   5,   8,  14,  NA,  NA,  NA,  34,  NA,  NA,  NA,  55,  61,  66,  NA,  74,  NA,  NA,  NA,  NA,  78,  83,  87,  NA,  NA,  NA,  NA,  92,  NA, 101,  NA },
    { 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134 }
};

static unsigned int ASUS_FALCHION_LAYOUT_KEYS_ANSI[5][16] =
{
    {   0,   5,   7,  12,  16,  20,  24,  29,  33,  37,  41,  46,  51,  56,  NA,  63 },
    {   1,  NA,   8,  13,  17,  21,  25,  30,  34,  38,  42,  47,  52,  57,  59,  64 },
    {   2,  NA,   9,  14,  18,  22,  26,  31,  35,  39,  43,  48,  53,  NA,  60,  65 },
    {   3,  NA,  10,  15,  19,  23,  27,  32,  36,  40,  44,  49,  54,  NA,  61,  66 },
    {   4,   6,  11,  NA,  NA,  NA,  28,  NA,  NA,  NA,  45,  50,  55,  58,  62,  67 }
};

static unsigned int ASUS_FALCHION_LAYOUT_KEYS_ISO[5][16] =
{
    {   0,   5,   8,  13,  17,  21,  25,  30,  34,  38,  42,  47,  52,  57,  NA,  64 },
    {   1,  NA,   9,  14,  18,  22,  26,  31,  35,  39,  43,  48,  53,  58,  NA,  65 },
    {   2,  NA,  10,  15,  19,  23,  27,  32,  36,  40,  44,  49,  54,  59,  61,  66 },
    {   3,   6,  11,  16,  20,  24,  28,  33,  37,  41,  45,  50,  55,  NA,  62,  67 },
    {   4,   7,  12,  NA,  NA,  NA,  29,  NA,  NA,  NA,  46,  51,  56,  60,  63,  68 }
};

static unsigned int ASUS_CLAYMORE_NO_NUMPAD_LAYOUT_KEYS_ANSI[7][19] =
{
    {   0,  NA,   8,  14,  19,  24,  NA,  34,  39,  45,  50,  56,  62,  67,  71,  NA,  75,  79,  84 },
    {   1,   6,   9,  15,  20,  25,  29,  35,  40,  46,  51,  57,  63,  68,  NA,  NA,  76,  80,  85 },
    {   2,  NA,  10,  16,  21,  26,  30,  36,  41,  47,  52,  58,  64,  69,  72,  NA,  77,  81,  86 },
    {   3,  NA,  11,  17,  22,  27,  31,  37,  42,  48,  53,  59,  65,  NA,  73,  NA,  NA,  NA,  NA },
    {   4,  NA,  12,  18,  23,  28,  32,  38,  43,  49,  54,  60,  NA,  70,  NA,  NA,  NA,  82,  NA },
    {   5,   7,  13,  NA,  NA,  NA,  33,  NA,  NA,  NA,  55,  61,  66,  NA,  74,  NA,  78,  83,  87 },
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  44,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA }
};

static unsigned int ASUS_CLAYMORE_NO_NUMPAD_LAYOUT_KEYS_ISO[7][19] =
{
    {   0,  NA,   9,  15,  20,  25,  NA,  35,  40,  46,  51,  57,  63,  68,  73,  NA,  76,  80,  85 },
    {   1,   6,  10,  16,  21,  26,  30,  36,  41,  47,  52,  58,  64,  69,  NA,  NA,  77,  81,  86 },
    {   2,  NA,  11,  17,  22,  27,  31,  37,  42,  48,  53,  59,  65,  70,  NA,  NA,  78,  82,  87 },
    {   3,  NA,  12,  18,  23,  28,  32,  38,  43,  49,  54,  60,  66,  71,  74,  NA,  NA,  NA,  NA },
    {   4,   7,  13,  19,  24,  29,  33,  39,  44,  50,  55,  61,  NA,  72,  NA,  NA,  NA,  83,  NA },
    {   5,   8,  14,  NA,  NA,  NA,  34,  NA,  NA,  NA,  56,  62,  67,  NA,  75,  NA,  79,  84,  88 },
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  45,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA }
};

static unsigned int ASUS_CLAYMORE_NUMPAD_RIGHT_LAYOUT_KEYS_ANSI[7][24] =
{
    {   0,  NA,   8,  14,  19,  24,  NA,  34,  39,  45,  50,  56,  62,  67,  71,  NA,  75,  79,  84,  NA,  NA,  NA,  NA,  NA },
    {   1,   6,   9,  15,  20,  25,  29,  35,  40,  46,  51,  57,  63,  68,  NA,  NA,  76,  80,  85,  NA,  88,  93,  97, 102 },
    {   2,  NA,  10,  16,  21,  26,  30,  36,  41,  47,  52,  58,  64,  69,  72,  NA,  77,  81,  86,  NA,  89,  94,  98, 103 },
    {   3,  NA,  11,  17,  22,  27,  31,  37,  42,  48,  53,  59,  65,  NA,  73,  NA,  NA,  NA,  NA,  NA,  90,  95,  99,  NA },
    {   4,  NA,  12,  18,  23,  28,  32,  38,  43,  49,  54,  60,  NA,  70,  NA,  NA,  NA,  82,  NA,  NA,  91,  96, 100, 104 },
    {   5,   7,  13,  NA,  NA,  NA,  33,  NA,  NA,  NA,  55,  61,  66,  NA,  74,  NA,  78,  83,  87,  NA,  92,  NA, 101,  NA },
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  44,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA }
};

static unsigned int ASUS_CLAYMORE_NUMPAD_RIGHT_LAYOUT_KEYS_ISO[7][24] =
{
    {   0,  NA,   9,  15,  20,  25,  NA,  35,  40,  46,  51,  57,  63,  68,  73,  NA,  76,  80,  85,  NA,  NA,  NA,  NA,  NA },
    {   1,   6,  10,  16,  21,  26,  30,  36,  41,  47,  52,  58,  64,  69,  NA,  NA,  77,  81,  86,  NA,  89,  94,  98, 103 },
    {   2,  NA,  11,  17,  22,  27,  31,  37,  42,  48,  53,  59,  65,  70,  NA,  NA,  78,  82,  87,  NA,  90,  95,  99, 104 },
    {   3,  NA,  12,  18,  23,  28,  32,  38,  43,  49,  54,  60,  66,  71,  74,  NA,  NA,  NA,  NA,  NA,  91,  96, 100,  NA },
    {   4,   7,  13,  19,  24,  29,  33,  39,  44,  50,  55,  61,  NA,  72,  NA,  NA,  NA,  83,  NA,  NA,  92,  97, 101, 105 },
    {   5,   8,  14,  NA,  NA,  NA,  34,  NA,  NA,  NA,  56,  62,  67,  NA,  75,  NA,  79,  84,  88,  NA,  93,  NA, 102,  NA },
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  45,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA }
};

static unsigned int ASUS_CLAYMORE_NUMPAD_LEFT_LAYOUT_KEYS_ANSI[7][24] =
{
    {  NA,  NA,  NA,  NA,  NA,  17,  NA,  25,  31,  36,  41,  NA,  51,  56,  62,  67,  73,  79,  84,  88,  NA,  92,  96, 101 },
    {   0,   5,   9,  14,  NA,  18,  23,  26,  32,  37,  42,  46,  52,  57,  63,  68,  74,  80,  85,  NA,  NA,  93,  97, 102 },
    {   1,   6,  10,  15,  NA,  19,  NA,  27,  33,  38,  43,  47,  53,  58,  64,  69,  75,  81,  86,  89,  NA,  94,  98, 103 },
    {   2,   7,  11,  NA,  NA,  20,  NA,  28,  34,  39,  44,  48,  54,  59,  65,  70,  76,  82,  NA,  90,  NA,  NA,  NA,  NA },
    {   3,   8,  12,  16,  NA,  21,  NA,  29,  35,  40,  45,  49,  55,  60,  66,  71,  77,  NA,  87,  NA,  NA,  NA,  99,  NA },
    {   4,  NA,  13,  NA,  NA,  22,  24,  30,  NA,  NA,  NA,  50,  NA,  NA,  NA,  72,  78,  83,  NA,  91,  NA,  95, 100, 104 },
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  61,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA }
};

static unsigned int ASUS_CLAYMORE_NUMPAD_LEFT_LAYOUT_KEYS_ISO[7][24] =
{
    {  NA,  NA,  NA,  NA,  NA,  17,  NA,  26,  32,  37,  42,  NA,  52,  57,  63,  68,  74,  80,  85,  90,  NA,  93,  97, 102 },
    {   0,   5,   9,  14,  NA,  18,  23,  27,  33,  38,  43,  47,  53,  58,  64,  69,  75,  81,  86,  NA,  NA,  94,  98, 103 },
    {   1,   6,  10,  15,  NA,  19,  NA,  28,  34,  39,  44,  48,  54,  59,  65,  70,  76,  82,  87,  NA,  NA,  95,  99, 104 },
    {   2,   7,  11,  NA,  NA,  20,  NA,  29,  35,  40,  45,  49,  55,  60,  66,  71,  77,  83,  88,  91,  NA,  NA,  NA,  NA },
    {   3,   8,  12,  16,  NA,  21,  24,  30,  36,  41,  46,  50,  56,  61,  67,  72,  78,  NA,  89,  NA,  NA,  NA, 100,  NA },
    {   4,  NA,  13,  NA,  NA,  22,  25,  31,  NA,  NA,  NA,  51,  NA,  NA,  NA,  73,  79,  84,  NA,  92,  NA,  96, 101, 105 },
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  62,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA }
};

static unsigned int ASUS_TUF_K1_LAYOUT_KEYS[1][5] =
{
    {  0,  1,  2,  3,  4 },
};

static std::map<int,layout_info> AsusTUFK7Layouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_TUF_K7_LAYOUT_KEYS_ISO,
            105,
            6,
            24,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x11 },
                { KEY_EN_ISO_BACK_SLASH,      0x0C },
                { KEY_EN_LEFT_WINDOWS,        0x0D },

                { KEY_EN_F1,                  0x18 },
                { KEY_EN_2,                   0x19 },
                { KEY_EN_Q,                   0x12 },
                { KEY_EN_A,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },

                { KEY_EN_F2,                  0x20 },
                { KEY_EN_3,                   0x21 },
                { KEY_EN_W,                   0x1A },
                { KEY_EN_S,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F3,                  0x28 },
                { KEY_EN_4,                   0x29 },
                { KEY_EN_E,                   0x22 },
                { KEY_EN_D,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F4,                  0x30 },
                { KEY_EN_5,                   0x31 },
                { KEY_EN_R,                   0x2A },
                { KEY_EN_F,                   0x2B },
                { KEY_EN_V,                   0x2C },

                { KEY_EN_6,                   0x39 },
                { KEY_EN_T,                   0x32 },
                { KEY_EN_G,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F5,                  0x40 },
                { KEY_EN_7,                   0x41 },
                { KEY_EN_Y,                   0x3A },
                { KEY_EN_H,                   0x3B },
                { KEY_EN_N,                   0x3C },

                { KEY_EN_F6,                  0x48 },
                { KEY_EN_8,                   0x49 },
                { KEY_EN_U,                   0x42 },
                { KEY_EN_J,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F7,                  0x50 },
                { KEY_EN_9,                   0x51 },
                { KEY_EN_I,                   0x4A },
                { KEY_EN_K,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F8,                  0x58 },
                { KEY_EN_0,                   0x59 },
                { KEY_EN_O,                   0x52 },
                { KEY_EN_L,                   0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },

                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x61 },
                { KEY_EN_P,                   0x5A },
                { KEY_EN_SEMICOLON,           0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x69 },
                { KEY_EN_LEFT_BRACKET,        0x62 },
                { KEY_EN_QUOTE,               0x63 },
                { KEY_EN_MENU,                0x65 },

                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x79 },
                { KEY_EN_RIGHT_BRACKET,       0x6A },
                { KEY_EN_POUND,               0x6B },
                { KEY_EN_RIGHT_SHIFT,         0x7C },

                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ISO_ENTER,           0x7B },
                { KEY_EN_RIGHT_CONTROL,       0x7D },

                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },

                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },

                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },

                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },

                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },

                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },

                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 }
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_TUF_K7_LAYOUT_KEYS_ANSI,
            104,
            6,
            24,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x11 },
                { KEY_EN_LEFT_WINDOWS,        0x0D },

                { KEY_EN_F1,                  0x18 },
                { KEY_EN_2,                   0x19 },
                { KEY_EN_Q,                   0x12 },
                { KEY_EN_A,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },

                { KEY_EN_F2,                  0x20 },
                { KEY_EN_3,                   0x21 },
                { KEY_EN_W,                   0x1A },
                { KEY_EN_S,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F3,                  0x28 },
                { KEY_EN_4,                   0x29 },
                { KEY_EN_E,                   0x22 },
                { KEY_EN_D,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F4,                  0x30 },
                { KEY_EN_5,                   0x31 },
                { KEY_EN_R,                   0x2A },
                { KEY_EN_F,                   0x2B },
                { KEY_EN_V,                   0x2C },

                { KEY_EN_6,                   0x39 },
                { KEY_EN_T,                   0x32 },
                { KEY_EN_G,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F5,                  0x40 },
                { KEY_EN_7,                   0x41 },
                { KEY_EN_Y,                   0x3A },
                { KEY_EN_H,                   0x3B },
                { KEY_EN_N,                   0x3C },

                { KEY_EN_F6,                  0x48 },
                { KEY_EN_8,                   0x49 },
                { KEY_EN_U,                   0x42 },
                { KEY_EN_J,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F7,                  0x50 },
                { KEY_EN_9,                   0x51 },
                { KEY_EN_I,                   0x4A },
                { KEY_EN_K,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F8,                  0x58 },
                { KEY_EN_0,                   0x59 },
                { KEY_EN_O,                   0x52 },
                { KEY_EN_L,                   0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },

                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x61 },
                { KEY_EN_P,                   0x5A },
                { KEY_EN_SEMICOLON,           0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x69 },
                { KEY_EN_LEFT_BRACKET,        0x62 },
                { KEY_EN_QUOTE,               0x63 },
                { KEY_EN_MENU,                0x65 },

                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x79 },
                { KEY_EN_RIGHT_BRACKET,       0x6A },
                { KEY_EN_RIGHT_SHIFT,         0x7C },

                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ANSI_BACK_SLASH,     0x7A },
                { KEY_EN_ANSI_ENTER,          0x7B },
                { KEY_EN_RIGHT_CONTROL,       0x7D },

                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },

                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },

                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },

                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },

                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },

                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },

                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 }
            }
        }
    },
};

static std::map<int,layout_info> AsusROGAzothLayouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_ROG_AZOTH_LAYOUT_KEYS_UK,
            82,
            6,
            16,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_F1,                  0x08 },
                { KEY_EN_1,                   0x09 },
                { KEY_EN_ISO_BACK_SLASH,      0x0C },
                { KEY_EN_LEFT_WINDOWS,        0x0D },

                { KEY_EN_F2,                  0x10 },
                { KEY_EN_2,                   0x11 },
                { KEY_EN_Q,                   0x0A },
                { KEY_EN_A,                   0x0B },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },

                { KEY_EN_F3,                  0x18 },
                { KEY_EN_3,                   0x19 },
                { KEY_EN_W,                   0x12 },
                { KEY_EN_S,                   0x13 },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F4,                  0x20 },
                { KEY_EN_4,                   0x21 },
                { KEY_EN_E,                   0x1A },
                { KEY_EN_D,                   0x1B },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F5,                  0x28 },
                { KEY_EN_5,                   0x29 },
                { KEY_EN_R,                   0x22 },
                { KEY_EN_F,                   0x23 },
                { KEY_EN_V,                   0x2C },

                { KEY_EN_F6,                  0x30 },
                { KEY_EN_6,                   0x31 },
                { KEY_EN_T,                   0x2A },
                { KEY_EN_G,                   0x2B },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F7,                  0x38 },
                { KEY_EN_7,                   0x39 },
                { KEY_EN_Y,                   0x32 },
                { KEY_EN_H,                   0x33 },
                { KEY_EN_N,                   0x3C },

                { KEY_EN_F8,                  0x40 },
                { KEY_EN_8,                   0x41 },
                { KEY_EN_U,                   0x3A },
                { KEY_EN_J,                   0x3B },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F9,                  0x48 },
                { KEY_EN_9,                   0x49 },
                { KEY_EN_I,                   0x42 },
                { KEY_EN_K,                   0x43 },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F10,                 0x50 },
                { KEY_EN_0,                   0x51 },
                { KEY_EN_O,                   0x4A },
                { KEY_EN_L,                   0x4B },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x55 },

                { KEY_EN_F11,                 0x58 },
                { KEY_EN_MINUS,               0x59 },
                { KEY_EN_P,                   0x52 },
                { KEY_EN_SEMICOLON,           0x53 },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F12,                 0x60 },
                { KEY_EN_EQUALS,              0x61 },
                { KEY_EN_LEFT_BRACKET,        0x5A },
                { KEY_EN_QUOTE,               0x5B },
                { KEY_EN_RIGHT_SHIFT,         0x64 },
                { KEY_EN_RIGHT_CONTROL,       0x65 },

                { KEY_EN_BACKSPACE,           0x69 },
                { KEY_EN_RIGHT_BRACKET,       0x62 },
                { KEY_EN_POUND,               0x6B },
                { KEY_EN_LEFT_ARROW,          0x6D },

                { KEY_EN_ISO_ENTER,           0x6A },
                { KEY_EN_UP_ARROW,            0x74 },
                { KEY_EN_DOWN_ARROW,          0x75 },

                { KEY_EN_INSERT,              0x79 },
                { KEY_EN_DELETE,              0x7A },
                { KEY_EN_PAGE_UP,             0x7B },
                { KEY_EN_PAGE_DOWN,           0x7C },
                { KEY_EN_RIGHT_ARROW,         0x7D },
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_ROG_AZOTH_LAYOUT_KEYS_US,
            81,
            6,
            16,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_F1,                  0x08 },
                { KEY_EN_1,                   0x09 },
                { KEY_EN_LEFT_WINDOWS,        0x0D },

                { KEY_EN_F2,                  0x10 },
                { KEY_EN_2,                   0x11 },
                { KEY_EN_Q,                   0x0A },
                { KEY_EN_A,                   0x0B },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },

                { KEY_EN_F3,                  0x18 },
                { KEY_EN_3,                   0x19 },
                { KEY_EN_W,                   0x12 },
                { KEY_EN_S,                   0x13 },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F4,                  0x20 },
                { KEY_EN_4,                   0x21 },
                { KEY_EN_E,                   0x1A },
                { KEY_EN_D,                   0x1B },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F5,                  0x28 },
                { KEY_EN_5,                   0x29 },
                { KEY_EN_R,                   0x22 },
                { KEY_EN_F,                   0x23 },
                { KEY_EN_V,                   0x2C },

                { KEY_EN_F6,                  0x30 },
                { KEY_EN_6,                   0x31 },
                { KEY_EN_T,                   0x2A },
                { KEY_EN_G,                   0x2B },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F7,                  0x38 },
                { KEY_EN_7,                   0x39 },
                { KEY_EN_Y,                   0x32 },
                { KEY_EN_H,                   0x33 },
                { KEY_EN_N,                   0x3C },

                { KEY_EN_F8,                  0x40 },
                { KEY_EN_8,                   0x41 },
                { KEY_EN_U,                   0x3A },
                { KEY_EN_J,                   0x3B },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F9,                  0x48 },
                { KEY_EN_9,                   0x49 },
                { KEY_EN_I,                   0x42 },
                { KEY_EN_K,                   0x43 },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F10,                 0x50 },
                { KEY_EN_0,                   0x51 },
                { KEY_EN_O,                   0x4A },
                { KEY_EN_L,                   0x4B },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x55 },

                { KEY_EN_F11,                 0x58 },
                { KEY_EN_MINUS,               0x59 },
                { KEY_EN_P,                   0x52 },
                { KEY_EN_SEMICOLON,           0x53 },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F12,                 0x60 },
                { KEY_EN_EQUALS,              0x61 },
                { KEY_EN_LEFT_BRACKET,        0x5A },
                { KEY_EN_QUOTE,               0x5B },
                { KEY_EN_RIGHT_SHIFT,         0x64 },
                { KEY_EN_RIGHT_CONTROL,       0x65 },

                { KEY_EN_BACKSPACE,           0x69 },
                { KEY_EN_RIGHT_BRACKET,       0x62 },
                { KEY_EN_LEFT_ARROW,          0x6D },

                { KEY_EN_ANSI_BACK_SLASH,     0x6A },
                { KEY_EN_ANSI_ENTER,          0x6B },
                { KEY_EN_UP_ARROW,            0x74 },
                { KEY_EN_DOWN_ARROW,          0x75 },

                { KEY_EN_INSERT,              0x79 },
                { KEY_EN_DELETE,              0x7A },
                { KEY_EN_PAGE_UP,             0x7B },
                { KEY_EN_PAGE_DOWN,           0x7C },
                { KEY_EN_RIGHT_ARROW,         0x7D },
            }
        }
    },
};

static std::map<int,layout_info> AsusROGStrixScopeLayouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_ROG_STRIX_SCOPE_LAYOUT_KEYS_ISO,
            107,
            6,
            24,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x11 },
                { KEY_EN_ISO_BACK_SLASH,      0x0C },
                { KEY_EN_LEFT_WINDOWS,        0x15 },

                { KEY_EN_F1,                  0x18 },
                { KEY_EN_2,                   0x19 },
                { KEY_EN_Q,                   0x12 },
                { KEY_EN_A,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x1D },

                { KEY_EN_F2,                  0x20 },
                { KEY_EN_3,                   0x21 },
                { KEY_EN_W,                   0x1A },
                { KEY_EN_S,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F3,                  0x28 },
                { KEY_EN_4,                   0x29 },
                { KEY_EN_E,                   0x22 },
                { KEY_EN_D,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F4,                  0x30 },
                { KEY_EN_5,                   0x31 },
                { KEY_EN_R,                   0x2A },
                { KEY_EN_F,                   0x2B },
                { KEY_EN_V,                   0x2C },

                { KEY_EN_6,                   0x39 },
                { KEY_EN_T,                   0x32 },
                { KEY_EN_G,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F5,                  0x40 },
                { KEY_EN_7,                   0x41 },
                { KEY_EN_Y,                   0x3A },
                { KEY_EN_H,                   0x3B },
                { KEY_EN_N,                   0x3C },

                { KEY_EN_F6,                  0x48 },
                { KEY_EN_8,                   0x49 },
                { KEY_EN_U,                   0x42 },
                { KEY_EN_J,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F7,                  0x50 },
                { KEY_EN_9,                   0x51 },
                { KEY_EN_I,                   0x4A },
                { KEY_EN_K,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F8,                  0x58 },
                { KEY_EN_0,                   0x59 },
                { KEY_EN_O,                   0x52 },
                { KEY_EN_L,                   0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },

                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x61 },
                { KEY_EN_P,                   0x5A },
                { KEY_EN_SEMICOLON,           0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x69 },
                { KEY_EN_LEFT_BRACKET,        0x62 },
                { KEY_EN_QUOTE,               0x63 },
                { KEY_EN_MENU,                0x65 },

                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x79 },
                { KEY_EN_RIGHT_BRACKET,       0x6A },
                { KEY_EN_POUND,               0x6B },
                { KEY_EN_RIGHT_SHIFT,         0x7C },

                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ISO_ENTER,           0x7B },
                { KEY_EN_RIGHT_CONTROL,       0x7D },

                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },

                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },

                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },

                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },

                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },

                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },

                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 },

                { "Logo 1",                   0xA8 },
                { "Logo 2",                   0xB0 },
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_ROG_STRIX_SCOPE_LAYOUT_KEYS_ANSI,
            106,
            6,
            24,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x11 },
                { KEY_EN_LEFT_WINDOWS,        0x15 },

                { KEY_EN_F1,                  0x18 },
                { KEY_EN_2,                   0x19 },
                { KEY_EN_Q,                   0x12 },
                { KEY_EN_A,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x1D },

                { KEY_EN_F2,                  0x20 },
                { KEY_EN_3,                   0x21 },
                { KEY_EN_W,                   0x1A },
                { KEY_EN_S,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F3,                  0x28 },
                { KEY_EN_4,                   0x29 },
                { KEY_EN_E,                   0x22 },
                { KEY_EN_D,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F4,                  0x30 },
                { KEY_EN_5,                   0x31 },
                { KEY_EN_R,                   0x2A },
                { KEY_EN_F,                   0x2B },
                { KEY_EN_V,                   0x2C },

                { KEY_EN_6,                   0x39 },
                { KEY_EN_T,                   0x32 },
                { KEY_EN_G,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F5,                  0x40 },
                { KEY_EN_7,                   0x41 },
                { KEY_EN_Y,                   0x3A },
                { KEY_EN_H,                   0x3B },
                { KEY_EN_N,                   0x3C },

                { KEY_EN_F6,                  0x48 },
                { KEY_EN_8,                   0x49 },
                { KEY_EN_U,                   0x42 },
                { KEY_EN_J,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F7,                  0x50 },
                { KEY_EN_9,                   0x51 },
                { KEY_EN_I,                   0x4A },
                { KEY_EN_K,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F8,                  0x58 },
                { KEY_EN_0,                   0x59 },
                { KEY_EN_O,                   0x52 },
                { KEY_EN_L,                   0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },

                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x61 },
                { KEY_EN_P,                   0x5A },
                { KEY_EN_SEMICOLON,           0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x69 },
                { KEY_EN_LEFT_BRACKET,        0x62 },
                { KEY_EN_QUOTE,               0x63 },
                { KEY_EN_MENU,                0x65 },

                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x79 },
                { KEY_EN_RIGHT_BRACKET,       0x6A },
                { KEY_EN_RIGHT_SHIFT,         0x7C },

                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ANSI_BACK_SLASH,     0x7A },
                { KEY_EN_ANSI_ENTER,          0x7B },
                { KEY_EN_RIGHT_CONTROL,       0x7D },

                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },

                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },

                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },

                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },

                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },

                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },

                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 },

                { "Logo 1",                   0xA8 },
                { "Logo 2",                   0xB0 },
            }
        }
    },
};

static std::map<int,layout_info> AsusROGStrixScopeIILayouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_ROG_STRIX_SCOPE_II_LAYOUT_KEYS_ISO,
            108,
            6,
            24,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x11 },
                { KEY_EN_ISO_BACK_SLASH,      0x0C },
                { KEY_EN_LEFT_WINDOWS,        0x0D },

                { KEY_EN_F1,                  0x18 },
                { KEY_EN_2,                   0x19 },
                { KEY_EN_Q,                   0x12 },
                { KEY_EN_A,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },

                { KEY_EN_F2,                  0x20 },
                { KEY_EN_3,                   0x21 },
                { KEY_EN_W,                   0x1A },
                { KEY_EN_S,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F3,                  0x28 },
                { KEY_EN_4,                   0x29 },
                { KEY_EN_E,                   0x22 },
                { KEY_EN_D,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F4,                  0x30 },
                { KEY_EN_5,                   0x31 },
                { KEY_EN_R,                   0x2A },
                { KEY_EN_F,                   0x2B },
                { KEY_EN_V,                   0x2C },
                { KEY_EN_SPACE,               0x2D },

                { KEY_EN_6,                   0x39 },
                { KEY_EN_T,                   0x32 },
                { KEY_EN_G,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F5,                  0x40 },
                { KEY_EN_7,                   0x41 },
                { KEY_EN_Y,                   0x3A },
                { KEY_EN_H,                   0x3B },
                { KEY_EN_N,                   0x3C },
                { KEY_EN_SPACE,               0x3D },

                { KEY_EN_F6,                  0x48 },
                { KEY_EN_8,                   0x49 },
                { KEY_EN_U,                   0x42 },
                { KEY_EN_J,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F7,                  0x50 },
                { KEY_EN_9,                   0x51 },
                { KEY_EN_I,                   0x4A },
                { KEY_EN_K,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F8,                  0x58 },
                { KEY_EN_0,                   0x59 },
                { KEY_EN_O,                   0x52 },
                { KEY_EN_L,                   0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },

                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x61 },
                { KEY_EN_P,                   0x5A },
                { KEY_EN_SEMICOLON,           0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x69 },
                { KEY_EN_LEFT_BRACKET,        0x62 },
                { KEY_EN_QUOTE,               0x63 },
                { KEY_EN_MENU,                0x65 },

                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x79 },
                { KEY_EN_RIGHT_BRACKET,       0x6A },
                { KEY_EN_POUND,               0x6B },
                { KEY_EN_RIGHT_SHIFT,         0x7C },

                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ISO_ENTER,           0x7B },
                { KEY_EN_RIGHT_CONTROL,       0x7D },

                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },

                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },

                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },

                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },

                { "Logo",                     0xA0 },
                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },

                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },

                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 },

            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_ROG_STRIX_SCOPE_II_LAYOUT_KEYS_ANSI,
            107,
            6,
            24,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x11 },
                { KEY_EN_LEFT_WINDOWS,        0x0D },

                { KEY_EN_F1,                  0x18 },
                { KEY_EN_2,                   0x19 },
                { KEY_EN_Q,                   0x12 },
                { KEY_EN_A,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },

                { KEY_EN_F2,                  0x20 },
                { KEY_EN_3,                   0x21 },
                { KEY_EN_W,                   0x1A },
                { KEY_EN_S,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F3,                  0x28 },
                { KEY_EN_4,                   0x29 },
                { KEY_EN_E,                   0x22 },
                { KEY_EN_D,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F4,                  0x30 },
                { KEY_EN_5,                   0x31 },
                { KEY_EN_R,                   0x2A },
                { KEY_EN_F,                   0x2B },
                { KEY_EN_V,                   0x2C },
                { KEY_EN_SPACE,               0x2D },

                { KEY_EN_6,                   0x39 },
                { KEY_EN_T,                   0x32 },
                { KEY_EN_G,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F5,                  0x40 },
                { KEY_EN_7,                   0x41 },
                { KEY_EN_Y,                   0x3A },
                { KEY_EN_H,                   0x3B },
                { KEY_EN_N,                   0x3C },
                { KEY_EN_SPACE,               0x3D },

                { KEY_EN_F6,                  0x48 },
                { KEY_EN_8,                   0x49 },
                { KEY_EN_U,                   0x42 },
                { KEY_EN_J,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F7,                  0x50 },
                { KEY_EN_9,                   0x51 },
                { KEY_EN_I,                   0x4A },
                { KEY_EN_K,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F8,                  0x58 },
                { KEY_EN_0,                   0x59 },
                { KEY_EN_O,                   0x52 },
                { KEY_EN_L,                   0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },

                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x61 },
                { KEY_EN_P,                   0x5A },
                { KEY_EN_SEMICOLON,           0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x69 },
                { KEY_EN_LEFT_BRACKET,        0x62 },
                { KEY_EN_QUOTE,               0x63 },
                { KEY_EN_MENU,                0x65 },

                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x79 },
                { KEY_EN_RIGHT_BRACKET,       0x6A },
                { KEY_EN_RIGHT_SHIFT,         0x7C },

                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ANSI_BACK_SLASH,     0x7A },
                { KEY_EN_ANSI_ENTER,          0x7B },
                { KEY_EN_RIGHT_CONTROL,       0x7D },

                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },

                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },

                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },

                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },

                { "Logo",                     0xA0 },
                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },

                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },

                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 },
            }
        }
    },
};

static std::map<int,layout_info> AsusROGStrixScopeII96WirelessLayouts =
{
    {
        ASUS_TUF_K7_LAYOUT_NO,
        {
            *ASUS_ROG_STRIX_SCOPE_II_96_WIRELESS_LAYOUT_KEYS_ISO,
            102,
            6,
            19,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_NORD_HALF,              0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_F1,                  0x08 },
                { KEY_EN_1,                   0x09 },
                { KEY_EN_Q,                   0x0A },
                { KEY_EN_A,                   0x0B },
                { KEY_NORD_ANGLE_BRACKET,     0x0C },
                { KEY_EN_LEFT_WINDOWS,        0x0D },

                { KEY_EN_F2,                  0x10 },
                { KEY_EN_2,                   0x11 },
                { KEY_EN_W,                   0x12 },
                { KEY_EN_S,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },

                { KEY_EN_F3,                  0x18 },
                { KEY_EN_3,                   0x19 },
                { KEY_EN_E,                   0x1A },
                { KEY_EN_D,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F4,                  0x20 },
                { KEY_EN_4,                   0x21 },
                { KEY_EN_R,                   0x22 },
                { KEY_EN_F,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F5,                  0x28 },
                { KEY_EN_5,                   0x29 },
                { KEY_EN_T,                   0x2A },
                { KEY_EN_G,                   0x2B },
                { KEY_EN_V,                   0x2C },
                { KEY_EN_SPACE,               0x2D },

                { KEY_EN_F6,                  0x30 },
                { KEY_EN_6,                   0x31 },
                { KEY_EN_Y,                   0x32 },
                { KEY_EN_H,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F7,                  0x38 },
                { KEY_EN_7,                   0x39 },
                { KEY_EN_U,                   0x3A },
                { KEY_EN_J,                   0x3B },
                { KEY_EN_N,                   0x3C },
                { KEY_EN_SPACE,               0x3D },

                { KEY_EN_F8,                  0x40 },
                { KEY_EN_8,                   0x41 },
                { KEY_EN_I,                   0x42 },
                { KEY_EN_K,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F9,                  0x48 },
                { KEY_EN_9,                   0x49 },
                { KEY_EN_O,                   0x4A },
                { KEY_EN_L,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F10,                 0x50 },
                { KEY_EN_0,                   0x51 },
                { KEY_EN_P,                   0x52 },
                { KEY_NORD_O_AE,              0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x55 },

                { KEY_EN_F11,                 0x58 },
                { KEY_NORD_PLUS_QUESTION,     0x59 },
                { KEY_NORD_AAL,               0x5A },
                { KEY_NORD_A_OE,              0x5B },
                { KEY_NORD_HYPHEN,            0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F12,                 0x60 },
                { KEY_NORD_ACUTE_GRAVE,       0x61 },
                { KEY_NORD_DOTS_CARET,        0x62 },
                { KEY_NORD_QUOTE,             0x63 },
                { KEY_EN_RIGHT_SHIFT,         0x64 },
                { KEY_EN_RIGHT_CONTROL,       0x65 },

                { KEY_EN_INSERT,              0x68 },
                { KEY_EN_BACKSPACE,           0x69 },
                { KEY_EN_ISO_ENTER,           0x6B },
                { KEY_EN_LEFT_ARROW,          0x6D },

                { KEY_EN_DELETE,              0x70 },
                { KEY_EN_UP_ARROW,            0x74 },
                { KEY_EN_DOWN_ARROW,          0x75 },

                { KEY_EN_PAGE_UP,             0x78 },
                { KEY_EN_NUMPAD_LOCK,         0x79 },
                { KEY_EN_NUMPAD_7,            0x7A },
                { KEY_EN_NUMPAD_4,            0x7B },
                { KEY_EN_NUMPAD_1,            0x7C },
                { KEY_EN_RIGHT_ARROW,         0x7D },

                { KEY_EN_PAGE_DOWN,           0x80 },
                { KEY_EN_NUMPAD_DIVIDE,       0x81 },
                { KEY_EN_NUMPAD_8,            0x82 },
                { KEY_EN_NUMPAD_5,            0x83 },
                { KEY_EN_NUMPAD_2,            0x84 },
                { KEY_EN_NUMPAD_0,            0x85 },

                { KEY_EN_NUMPAD_TIMES,        0x89 },
                { KEY_EN_NUMPAD_9,            0x8A },
                { KEY_EN_NUMPAD_6,            0x8B },
                { KEY_EN_NUMPAD_3,            0x8C },
                { KEY_EN_NUMPAD_PERIOD,       0x8D },

                { "Logo",                     0x88 },
                { KEY_EN_NUMPAD_MINUS,        0x91 },
                { KEY_EN_NUMPAD_PLUS,         0x92 },
                { KEY_EN_NUMPAD_ENTER,        0x94 }
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_ROG_STRIX_SCOPE_II_96_WIRELESS_LAYOUT_KEYS_ISO,
            102,
            6,
            19,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_F1,                  0x08 },
                { KEY_EN_1,                   0x09 },
                { KEY_EN_Q,                   0x0A },
                { KEY_EN_A,                   0x0B },
                { KEY_EN_ISO_BACK_SLASH,      0x0C },
                { KEY_EN_LEFT_WINDOWS,        0x0D },

                { KEY_EN_F2,                  0x10 },
                { KEY_EN_2,                   0x11 },
                { KEY_EN_W,                   0x12 },
                { KEY_EN_S,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },

                { KEY_EN_F3,                  0x18 },
                { KEY_EN_3,                   0x19 },
                { KEY_EN_E,                   0x1A },
                { KEY_EN_D,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F4,                  0x20 },
                { KEY_EN_4,                   0x21 },
                { KEY_EN_R,                   0x22 },
                { KEY_EN_F,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F5,                  0x28 },
                { KEY_EN_5,                   0x29 },
                { KEY_EN_T,                   0x2A },
                { KEY_EN_G,                   0x2B },
                { KEY_EN_V,                   0x2C },
                { KEY_EN_SPACE,               0x2D },

                { KEY_EN_F6,                  0x30 },
                { KEY_EN_6,                   0x31 },
                { KEY_EN_Y,                   0x32 },
                { KEY_EN_H,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F7,                  0x38 },
                { KEY_EN_7,                   0x39 },
                { KEY_EN_U,                   0x3A },
                { KEY_EN_J,                   0x3B },
                { KEY_EN_N,                   0x3C },
                { KEY_EN_SPACE,               0x3D },

                { KEY_EN_F8,                  0x40 },
                { KEY_EN_8,                   0x41 },
                { KEY_EN_I,                   0x42 },
                { KEY_EN_K,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F9,                  0x48 },
                { KEY_EN_9,                   0x49 },
                { KEY_EN_O,                   0x4A },
                { KEY_EN_L,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F10,                 0x50 },
                { KEY_EN_0,                   0x51 },
                { KEY_EN_P,                   0x52 },
                { KEY_EN_SEMICOLON,           0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x55 },

                { KEY_EN_F11,                 0x58 },
                { KEY_EN_MINUS,               0x59 },
                { KEY_EN_LEFT_BRACKET,        0x5A },
                { KEY_EN_QUOTE,               0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F12,                 0x60 },
                { KEY_EN_EQUALS,              0x61 },
                { KEY_EN_RIGHT_BRACKET,       0x62 },
                { KEY_EN_POUND,               0x63 },
                { KEY_EN_RIGHT_SHIFT,         0x64 },
                { KEY_EN_RIGHT_CONTROL,       0x65 },

                { KEY_EN_INSERT,              0x68 },
                { KEY_EN_BACKSPACE,           0x69 },
                { KEY_EN_ISO_ENTER,           0x6B },
                { KEY_EN_LEFT_ARROW,          0x6D },

                { KEY_EN_DELETE,              0x70 },
                { KEY_EN_UP_ARROW,            0x74 },
                { KEY_EN_DOWN_ARROW,          0x75 },

                { KEY_EN_PAGE_UP,             0x78 },
                { KEY_EN_NUMPAD_LOCK,         0x79 },
                { KEY_EN_NUMPAD_7,            0x7A },
                { KEY_EN_NUMPAD_4,            0x7B },
                { KEY_EN_NUMPAD_1,            0x7C },
                { KEY_EN_RIGHT_ARROW,         0x7D },

                { KEY_EN_PAGE_DOWN,           0x80 },
                { KEY_EN_NUMPAD_DIVIDE,       0x81 },
                { KEY_EN_NUMPAD_8,            0x82 },
                { KEY_EN_NUMPAD_5,            0x83 },
                { KEY_EN_NUMPAD_2,            0x84 },
                { KEY_EN_NUMPAD_0,            0x85 },

                { KEY_EN_NUMPAD_TIMES,        0x89 },
                { KEY_EN_NUMPAD_9,            0x8A },
                { KEY_EN_NUMPAD_6,            0x8B },
                { KEY_EN_NUMPAD_3,            0x8C },
                { KEY_EN_NUMPAD_PERIOD,       0x8D },

                { "Logo",                     0x88 },
                { KEY_EN_NUMPAD_MINUS,        0x91 },
                { KEY_EN_NUMPAD_PLUS,         0x92 },
                { KEY_EN_NUMPAD_ENTER,        0x94 }
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_ROG_STRIX_SCOPE_II_96_WIRELESS_LAYOUT_KEYS_ANSI,
            101,
            6,
            19,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_F1,                  0x08 },
                { KEY_EN_1,                   0x09 },
                { KEY_EN_Q,                   0x0A },
                { KEY_EN_A,                   0x0B },
                { KEY_EN_LEFT_WINDOWS,        0x0D },

                { KEY_EN_F2,                  0x10 },
                { KEY_EN_2,                   0x11 },
                { KEY_EN_W,                   0x12 },
                { KEY_EN_S,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },

                { KEY_EN_F3,                  0x18 },
                { KEY_EN_3,                   0x19 },
                { KEY_EN_E,                   0x1A },
                { KEY_EN_D,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F4,                  0x20 },
                { KEY_EN_4,                   0x21 },
                { KEY_EN_R,                   0x22 },
                { KEY_EN_F,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F5,                  0x28 },
                { KEY_EN_5,                   0x29 },
                { KEY_EN_T,                   0x2A },
                { KEY_EN_G,                   0x2B },
                { KEY_EN_V,                   0x2C },
                { KEY_EN_SPACE,               0x2D },

                { KEY_EN_F6,                  0x30 },
                { KEY_EN_6,                   0x31 },
                { KEY_EN_Y,                   0x32 },
                { KEY_EN_H,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F7,                  0x38 },
                { KEY_EN_7,                   0x39 },
                { KEY_EN_U,                   0x3A },
                { KEY_EN_J,                   0x3B },
                { KEY_EN_N,                   0x3C },
                { KEY_EN_SPACE,               0x3D },

                { KEY_EN_F8,                  0x40 },
                { KEY_EN_8,                   0x41 },
                { KEY_EN_I,                   0x42 },
                { KEY_EN_K,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F9,                  0x48 },
                { KEY_EN_9,                   0x49 },
                { KEY_EN_O,                   0x4A },
                { KEY_EN_L,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F10,                 0x50 },
                { KEY_EN_0,                   0x51 },
                { KEY_EN_P,                   0x52 },
                { KEY_EN_SEMICOLON,           0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x55 },

                { KEY_EN_F11,                 0x58 },
                { KEY_EN_MINUS,               0x59 },
                { KEY_EN_LEFT_BRACKET,        0x5A },
                { KEY_EN_QUOTE,               0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F12,                 0x60 },
                { KEY_EN_EQUALS,              0x61 },
                { KEY_EN_RIGHT_BRACKET,       0x62 },
                { KEY_EN_RIGHT_SHIFT,         0x64 },
                { KEY_EN_RIGHT_CONTROL,       0x65 },

                { KEY_EN_INSERT,              0x68 },
                { KEY_EN_BACKSPACE,           0x69 },
                { KEY_EN_ANSI_BACK_SLASH,     0x6A },
                { KEY_EN_ANSI_ENTER,          0x6B },
                { KEY_EN_LEFT_ARROW,          0x6D },

                { KEY_EN_DELETE,              0x70 },
                { KEY_EN_UP_ARROW,            0x74 },
                { KEY_EN_DOWN_ARROW,          0x75 },

                { KEY_EN_PAGE_UP,             0x78 },
                { KEY_EN_NUMPAD_LOCK,         0x79 },
                { KEY_EN_NUMPAD_7,            0x7A },
                { KEY_EN_NUMPAD_4,            0x7B },
                { KEY_EN_NUMPAD_1,            0x7C },
                { KEY_EN_RIGHT_ARROW,         0x7D },

                { KEY_EN_PAGE_DOWN,           0x80 },
                { KEY_EN_NUMPAD_DIVIDE,       0x81 },
                { KEY_EN_NUMPAD_8,            0x82 },
                { KEY_EN_NUMPAD_5,            0x83 },
                { KEY_EN_NUMPAD_2,            0x84 },
                { KEY_EN_NUMPAD_0,            0x85 },

                { KEY_EN_NUMPAD_TIMES,        0x89 },
                { KEY_EN_NUMPAD_9,            0x8A },
                { KEY_EN_NUMPAD_6,            0x8B },
                { KEY_EN_NUMPAD_3,            0x8C },
                { KEY_EN_NUMPAD_PERIOD,       0x8D },

                { "Logo",                     0x88 },
                { KEY_EN_NUMPAD_MINUS,        0x91 },
                { KEY_EN_NUMPAD_PLUS,         0x92 },
                { KEY_EN_NUMPAD_ENTER,        0x94 }
            }
        }
    },
};

static std::map<int,layout_info> AsusROGStrixFlareLayouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_ROG_STRIX_FLARE_LAYOUT_KEYS_ISO,
            108,
            6,
            26,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x11 },
                { KEY_EN_ISO_BACK_SLASH,      0x0C },
                { KEY_EN_LEFT_WINDOWS,        0x0D },

                { KEY_EN_F1,                  0x18 },
                { KEY_EN_2,                   0x19 },
                { KEY_EN_Q,                   0x12 },
                { KEY_EN_A,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },

                { KEY_EN_F2,                  0x20 },
                { KEY_EN_3,                   0x21 },
                { KEY_EN_W,                   0x1A },
                { KEY_EN_S,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F3,                  0x28 },
                { KEY_EN_4,                   0x29 },
                { KEY_EN_E,                   0x22 },
                { KEY_EN_D,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F4,                  0x30 },
                { KEY_EN_5,                   0x31 },
                { KEY_EN_R,                   0x2A },
                { KEY_EN_F,                   0x2B },
                { KEY_EN_V,                   0x2C },

                { KEY_EN_6,                   0x39 },
                { KEY_EN_T,                   0x32 },
                { KEY_EN_G,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F5,                  0x40 },
                { KEY_EN_7,                   0x41 },
                { KEY_EN_Y,                   0x3A },
                { KEY_EN_H,                   0x3B },
                { KEY_EN_N,                   0x3C },

                { KEY_EN_F6,                  0x48 },
                { KEY_EN_8,                   0x49 },
                { KEY_EN_U,                   0x42 },
                { KEY_EN_J,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F7,                  0x50 },
                { KEY_EN_9,                   0x51 },
                { KEY_EN_I,                   0x4A },
                { KEY_EN_K,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F8,                  0x58 },
                { KEY_EN_0,                   0x59 },
                { KEY_EN_O,                   0x52 },
                { KEY_EN_L,                   0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },

                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x61 },
                { KEY_EN_P,                   0x5A },
                { KEY_EN_SEMICOLON,           0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x69 },
                { KEY_EN_LEFT_BRACKET,        0x62 },
                { KEY_EN_QUOTE,               0x63 },
                { KEY_EN_MENU,                0x65 },

                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x79 },
                { KEY_EN_RIGHT_BRACKET,       0x6A },
                { KEY_EN_POUND,               0x6B },
                { KEY_EN_RIGHT_SHIFT,         0x7C },

                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ISO_ENTER,           0x7B },
                { KEY_EN_RIGHT_CONTROL,       0x7D },

                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },

                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },

                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },

                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },

                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },

                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },

                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 },

                { "Logo",                     0xB8 },
                { "Underglow left",           0xB9 },
                { "Underglow right",          0xBA }
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_ROG_STRIX_FLARE_LAYOUT_KEYS_ANSI,
            107,
            6,
            26,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x11 },
                { KEY_EN_LEFT_WINDOWS,        0x0D },

                { KEY_EN_F1,                  0x18 },
                { KEY_EN_2,                   0x19 },
                { KEY_EN_Q,                   0x12 },
                { KEY_EN_A,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },

                { KEY_EN_F2,                  0x20 },
                { KEY_EN_3,                   0x21 },
                { KEY_EN_W,                   0x1A },
                { KEY_EN_S,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F3,                  0x28 },
                { KEY_EN_4,                   0x29 },
                { KEY_EN_E,                   0x22 },
                { KEY_EN_D,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F4,                  0x30 },
                { KEY_EN_5,                   0x31 },
                { KEY_EN_R,                   0x2A },
                { KEY_EN_F,                   0x2B },
                { KEY_EN_V,                   0x2C },

                { KEY_EN_6,                   0x39 },
                { KEY_EN_T,                   0x32 },
                { KEY_EN_G,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F5,                  0x40 },
                { KEY_EN_7,                   0x41 },
                { KEY_EN_Y,                   0x3A },
                { KEY_EN_H,                   0x3B },
                { KEY_EN_N,                   0x3C },

                { KEY_EN_F6,                  0x48 },
                { KEY_EN_8,                   0x49 },
                { KEY_EN_U,                   0x42 },
                { KEY_EN_J,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F7,                  0x50 },
                { KEY_EN_9,                   0x51 },
                { KEY_EN_I,                   0x4A },
                { KEY_EN_K,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F8,                  0x58 },
                { KEY_EN_0,                   0x59 },
                { KEY_EN_O,                   0x52 },
                { KEY_EN_L,                   0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },

                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x61 },
                { KEY_EN_P,                   0x5A },
                { KEY_EN_SEMICOLON,           0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x69 },
                { KEY_EN_LEFT_BRACKET,        0x62 },
                { KEY_EN_QUOTE,               0x63 },
                { KEY_EN_MENU,                0x65 },

                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x79 },
                { KEY_EN_RIGHT_BRACKET,       0x6A },
                { KEY_EN_RIGHT_SHIFT,         0x7C },

                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ANSI_BACK_SLASH,     0x7A },
                { KEY_EN_ANSI_ENTER,          0x7B },
                { KEY_EN_RIGHT_CONTROL,       0x7D },

                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },

                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },

                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },

                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },

                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },

                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },

                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 },

                { "Logo",                     0xB8 },
                { "Underglow left",           0xB9 },
                { "Underglow right",          0xBA }
            }
        }
    },
};

static std::map<int,layout_info> AsusROGStrixFlareIILayouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_ROG_STRIX_FLARE_II_LAYOUT_KEYS_ISO,
            107,
            7,
            30,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x11 },
                { KEY_EN_ISO_BACK_SLASH,      0x0C },
                { KEY_EN_LEFT_WINDOWS,        0x15 },

                { KEY_EN_F1,                  0x18 },
                { KEY_EN_2,                   0x19 },
                { KEY_EN_Q,                   0x12 },
                { KEY_EN_A,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x1D },

                { KEY_EN_F2,                  0x20 },
                { KEY_EN_3,                   0x21 },
                { KEY_EN_W,                   0x1A },
                { KEY_EN_S,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F3,                  0x28 },
                { KEY_EN_4,                   0x29 },
                { KEY_EN_E,                   0x22 },
                { KEY_EN_D,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F4,                  0x30 },
                { KEY_EN_5,                   0x31 },
                { KEY_EN_R,                   0x2A },
                { KEY_EN_F,                   0x2B },
                { KEY_EN_V,                   0x2C },

                { KEY_EN_6,                   0x39 },
                { KEY_EN_T,                   0x32 },
                { KEY_EN_G,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F5,                  0x40 },
                { KEY_EN_7,                   0x41 },
                { KEY_EN_Y,                   0x3A },
                { KEY_EN_H,                   0x3B },
                { KEY_EN_N,                   0x3C },

                { KEY_EN_F6,                  0x48 },
                { KEY_EN_8,                   0x49 },
                { KEY_EN_U,                   0x42 },
                { KEY_EN_J,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F7,                  0x50 },
                { KEY_EN_9,                   0x51 },
                { KEY_EN_I,                   0x4A },
                { KEY_EN_K,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F8,                  0x58 },
                { KEY_EN_0,                   0x59 },
                { KEY_EN_O,                   0x52 },
                { KEY_EN_L,                   0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },

                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x61 },
                { KEY_EN_P,                   0x5A },
                { KEY_EN_SEMICOLON,           0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x69 },
                { KEY_EN_LEFT_BRACKET,        0x62 },
                { KEY_EN_QUOTE,               0x63 },
                { KEY_EN_MENU,                0x65 },

                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x79 },
                { KEY_EN_RIGHT_BRACKET,       0x6A },
                { KEY_EN_POUND,               0x6B },
                { KEY_EN_RIGHT_SHIFT,         0x7C },

                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ISO_ENTER,           0x7B },
                { KEY_EN_RIGHT_CONTROL,       0x7D },

                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },

                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },

                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },

                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },

                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },

                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },

                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 },

                { "Logo 1",                    0xB0 },
                { "Logo 2",                    0xA8 },
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_ROG_STRIX_FLARE_II_LAYOUT_KEYS_ANSI,
            106,
            7,
            30,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x11 },
                { KEY_EN_LEFT_WINDOWS,        0x15 },

                { KEY_EN_F1,                  0x18 },
                { KEY_EN_2,                   0x19 },
                { KEY_EN_Q,                   0x12 },
                { KEY_EN_A,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x1D },

                { KEY_EN_F2,                  0x20 },
                { KEY_EN_3,                   0x21 },
                { KEY_EN_W,                   0x1A },
                { KEY_EN_S,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F3,                  0x28 },
                { KEY_EN_4,                   0x29 },
                { KEY_EN_E,                   0x22 },
                { KEY_EN_D,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F4,                  0x30 },
                { KEY_EN_5,                   0x31 },
                { KEY_EN_R,                   0x2A },
                { KEY_EN_F,                   0x2B },
                { KEY_EN_V,                   0x2C },

                { KEY_EN_6,                   0x39 },
                { KEY_EN_T,                   0x32 },
                { KEY_EN_G,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F5,                  0x40 },
                { KEY_EN_7,                   0x41 },
                { KEY_EN_Y,                   0x3A },
                { KEY_EN_H,                   0x3B },
                { KEY_EN_N,                   0x3C },

                { KEY_EN_F6,                  0x48 },
                { KEY_EN_8,                   0x49 },
                { KEY_EN_U,                   0x42 },
                { KEY_EN_J,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F7,                  0x50 },
                { KEY_EN_9,                   0x51 },
                { KEY_EN_I,                   0x4A },
                { KEY_EN_K,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F8,                  0x58 },
                { KEY_EN_0,                   0x59 },
                { KEY_EN_O,                   0x52 },
                { KEY_EN_L,                   0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },

                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x61 },
                { KEY_EN_P,                   0x5A },
                { KEY_EN_SEMICOLON,           0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x69 },
                { KEY_EN_LEFT_BRACKET,        0x62 },
                { KEY_EN_QUOTE,               0x63 },
                { KEY_EN_MENU,                0x65 },

                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x79 },
                { KEY_EN_RIGHT_BRACKET,       0x6A },
                { KEY_EN_RIGHT_SHIFT,         0x7C },

                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ANSI_BACK_SLASH,     0x7A },
                { KEY_EN_ANSI_ENTER,          0x7B },
                { KEY_EN_RIGHT_CONTROL,       0x7D },

                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },

                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },

                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },

                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },

                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },

                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },

                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 },

                { "Logo 1",                    0xB0 },
                { "Logo 2",                    0xA8 },
            }
        }
    },
};

static std::map<int,layout_info> AsusROGStrixFlareIIAnimateLayouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_ROG_STRIX_FLARE_II_ANIMATE_LAYOUT_KEYS_ISO,
            135,
            7,
            30,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x11 },
                { KEY_EN_ISO_BACK_SLASH,      0x0C },
                { KEY_EN_LEFT_WINDOWS,        0x15 },

                { KEY_EN_F1,                  0x18 },
                { KEY_EN_2,                   0x19 },
                { KEY_EN_Q,                   0x12 },
                { KEY_EN_A,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x1D },

                { KEY_EN_F2,                  0x20 },
                { KEY_EN_3,                   0x21 },
                { KEY_EN_W,                   0x1A },
                { KEY_EN_S,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F3,                  0x28 },
                { KEY_EN_4,                   0x29 },
                { KEY_EN_E,                   0x22 },
                { KEY_EN_D,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F4,                  0x30 },
                { KEY_EN_5,                   0x31 },
                { KEY_EN_R,                   0x2A },
                { KEY_EN_F,                   0x2B },
                { KEY_EN_V,                   0x2C },

                { KEY_EN_6,                   0x39 },
                { KEY_EN_T,                   0x32 },
                { KEY_EN_G,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F5,                  0x40 },
                { KEY_EN_7,                   0x41 },
                { KEY_EN_Y,                   0x3A },
                { KEY_EN_H,                   0x3B },
                { KEY_EN_N,                   0x3C },

                { KEY_EN_F6,                  0x48 },
                { KEY_EN_8,                   0x49 },
                { KEY_EN_U,                   0x42 },
                { KEY_EN_J,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F7,                  0x50 },
                { KEY_EN_9,                   0x51 },
                { KEY_EN_I,                   0x4A },
                { KEY_EN_K,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F8,                  0x58 },
                { KEY_EN_0,                   0x59 },
                { KEY_EN_O,                   0x52 },
                { KEY_EN_L,                   0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },

                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x61 },
                { KEY_EN_P,                   0x5A },
                { KEY_EN_SEMICOLON,           0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x69 },
                { KEY_EN_LEFT_BRACKET,        0x62 },
                { KEY_EN_QUOTE,               0x63 },
                { KEY_EN_MENU,                0x65 },

                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x79 },
                { KEY_EN_RIGHT_BRACKET,       0x6A },
                { KEY_EN_POUND,               0x6B },
                { KEY_EN_RIGHT_SHIFT,         0x7C },

                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ISO_ENTER,           0x7B },
                { KEY_EN_RIGHT_CONTROL,       0x7D },

                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },

                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },

                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },

                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },

                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },

                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },

                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 },

                { "Underglow LED 1",          0x06 },
                { "Underglow LED 2",          0x0E },
                { "Underglow LED 3",          0x16 },
                { "Underglow LED 4",          0x1E },
                { "Underglow LED 5",          0x26 },
                { "Underglow LED 6",          0x2E },
                { "Underglow LED 7",          0x36 },
                { "Underglow LED 8",          0x3E },
                { "Underglow LED 9",          0x46 },
                { "Underglow LED 10",         0x4E },
                { "Underglow LED 11",         0x56 },
                { "Underglow LED 12",         0x5E },
                { "Underglow LED 13",         0x66 },
                { "Underglow LED 14",         0x6E },
                { "Underglow LED 15",         0x76 },
                { "Underglow LED 16",         0x7E },
                { "Underglow LED 17",         0x86 },
                { "Underglow LED 18",         0x8E },
                { "Underglow LED 19",         0x96 },
                { "Underglow LED 20",         0x9E },
                { "Underglow LED 21",         0xA6 },
                { "Underglow LED 22",         0xAE },
                { "Underglow LED 23",         0xB6 },
                { "Underglow LED 24",         0xBE },
                { "Underglow LED 25",         0xC6 },
                { "Underglow LED 26",         0xCE },
                { "Underglow LED 27",         0xD6 },
                { "Underglow LED 28",         0xDE },
                { "Underglow LED 29",         0xE6 },
                { "Underglow LED 30",         0xEE },
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_ROG_STRIX_FLARE_II_ANIMATE_LAYOUT_KEYS_ANSI,
            134,
            7,
            30,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x11 },
                { KEY_EN_LEFT_WINDOWS,        0x15 },

                { KEY_EN_F1,                  0x18 },
                { KEY_EN_2,                   0x19 },
                { KEY_EN_Q,                   0x12 },
                { KEY_EN_A,                   0x13 },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x1D },

                { KEY_EN_F2,                  0x20 },
                { KEY_EN_3,                   0x21 },
                { KEY_EN_W,                   0x1A },
                { KEY_EN_S,                   0x1B },
                { KEY_EN_X,                   0x1C },

                { KEY_EN_F3,                  0x28 },
                { KEY_EN_4,                   0x29 },
                { KEY_EN_E,                   0x22 },
                { KEY_EN_D,                   0x23 },
                { KEY_EN_C,                   0x24 },

                { KEY_EN_F4,                  0x30 },
                { KEY_EN_5,                   0x31 },
                { KEY_EN_R,                   0x2A },
                { KEY_EN_F,                   0x2B },
                { KEY_EN_V,                   0x2C },

                { KEY_EN_6,                   0x39 },
                { KEY_EN_T,                   0x32 },
                { KEY_EN_G,                   0x33 },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x35 },

                { KEY_EN_F5,                  0x40 },
                { KEY_EN_7,                   0x41 },
                { KEY_EN_Y,                   0x3A },
                { KEY_EN_H,                   0x3B },
                { KEY_EN_N,                   0x3C },

                { KEY_EN_F6,                  0x48 },
                { KEY_EN_8,                   0x49 },
                { KEY_EN_U,                   0x42 },
                { KEY_EN_J,                   0x43 },
                { KEY_EN_M,                   0x44 },

                { KEY_EN_F7,                  0x50 },
                { KEY_EN_9,                   0x51 },
                { KEY_EN_I,                   0x4A },
                { KEY_EN_K,                   0x4B },
                { KEY_EN_COMMA,               0x4C },

                { KEY_EN_F8,                  0x58 },
                { KEY_EN_0,                   0x59 },
                { KEY_EN_O,                   0x52 },
                { KEY_EN_L,                   0x53 },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },

                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x61 },
                { KEY_EN_P,                   0x5A },
                { KEY_EN_SEMICOLON,           0x5B },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x5D },

                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x69 },
                { KEY_EN_LEFT_BRACKET,        0x62 },
                { KEY_EN_QUOTE,               0x63 },
                { KEY_EN_MENU,                0x65 },

                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x79 },
                { KEY_EN_RIGHT_BRACKET,       0x6A },
                { KEY_EN_RIGHT_SHIFT,         0x7C },

                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ANSI_BACK_SLASH,     0x7A },
                { KEY_EN_ANSI_ENTER,          0x7B },
                { KEY_EN_RIGHT_CONTROL,       0x7D },

                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },

                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },

                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },

                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },

                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },

                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },

                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 },

                { "Underglow LED 1",          0x06 },
                { "Underglow LED 2",          0x0E },
                { "Underglow LED 3",          0x16 },
                { "Underglow LED 4",          0x1E },
                { "Underglow LED 5",          0x26 },
                { "Underglow LED 6",          0x2E },
                { "Underglow LED 7",          0x36 },
                { "Underglow LED 8",          0x3E },
                { "Underglow LED 9",          0x46 },
                { "Underglow LED 10",         0x4E },
                { "Underglow LED 11",         0x56 },
                { "Underglow LED 12",         0x5E },
                { "Underglow LED 13",         0x66 },
                { "Underglow LED 14",         0x6E },
                { "Underglow LED 15",         0x76 },
                { "Underglow LED 16",         0x7E },
                { "Underglow LED 17",         0x86 },
                { "Underglow LED 18",         0x8E },
                { "Underglow LED 19",         0x96 },
                { "Underglow LED 20",         0x9E },
                { "Underglow LED 21",         0xA6 },
                { "Underglow LED 22",         0xAE },
                { "Underglow LED 23",         0xB6 },
                { "Underglow LED 24",         0xBE },
                { "Underglow LED 25",         0xC6 },
                { "Underglow LED 26",         0xCE },
                { "Underglow LED 27",         0xD6 },
                { "Underglow LED 28",         0xDE },
                { "Underglow LED 29",         0xE6 },
                { "Underglow LED 30",         0xEE },
            }
        }
    },
};

static std::map<int,layout_info> AsusFalchionLayouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_FALCHION_LAYOUT_KEYS_ISO,
            69,
            5,
            16,
            {
                { KEY_EN_ESCAPE,            0x00    },
                { KEY_EN_TAB,               0x01    },
                { KEY_EN_CAPS_LOCK,         0x02    },
                { KEY_EN_LEFT_SHIFT,        0x03    },
                { KEY_EN_LEFT_CONTROL,      0x04    },

                { KEY_EN_1,                 0x08    },
                { KEY_EN_ISO_BACK_SLASH,    0x0B    },
                { KEY_EN_LEFT_WINDOWS,      0x0C    },

                { KEY_EN_2,                 0x10    },
                { KEY_EN_Q,                 0x09    },
                { KEY_EN_A,                 0x0A    },
                { KEY_EN_Z,                 0x13    },
                { KEY_EN_LEFT_ALT,          0x14    },

                { KEY_EN_3,                 0x18    },
                { KEY_EN_W,                 0x11    },
                { KEY_EN_S,                 0x12    },
                { KEY_EN_X,                 0x1B    },

                { KEY_EN_4,                 0x20    },
                { KEY_EN_E,                 0x19    },
                { KEY_EN_D,                 0x1A    },
                { KEY_EN_C,                 0x23    },

                { KEY_EN_5,                 0x28    },
                { KEY_EN_R,                 0x21    },
                { KEY_EN_F,                 0x22    },
                { KEY_EN_V,                 0x2B    },

                { KEY_EN_6,                 0x30    },
                { KEY_EN_T,                 0x29    },
                { KEY_EN_G,                 0x2A    },
                { KEY_EN_B,                 0x33    },
                { KEY_EN_SPACE,             0x34    },

                { KEY_EN_7,                 0x38    },
                { KEY_EN_Y,                 0x31    },
                { KEY_EN_H,                 0x32    },
                { KEY_EN_N,                 0x3B    },

                { KEY_EN_8,                 0x40    },
                { KEY_EN_U,                 0x39    },
                { KEY_EN_J,                 0x3A    },
                { KEY_EN_M,                 0x43    },

                { KEY_EN_9,                 0x48    },
                { KEY_EN_I,                 0x41    },
                { KEY_EN_K,                 0x42    },
                { KEY_EN_COMMA,             0x4B    },

                { KEY_EN_0,                 0x50    },
                { KEY_EN_O,                 0x49    },
                { KEY_EN_L,                 0x4A    },
                { KEY_EN_PERIOD,            0x53    },
                { KEY_EN_RIGHT_ALT,         0x4C    },

                { KEY_EN_MINUS,             0x58    },
                { KEY_EN_P,                 0x51    },
                { KEY_EN_SEMICOLON,         0x52    },
                { KEY_EN_FORWARD_SLASH,     0x5B    },
                { KEY_EN_RIGHT_FUNCTION,    0x54    },

                { KEY_EN_EQUALS,            0x60    },
                { KEY_EN_LEFT_BRACKET,      0x59    },
                { KEY_EN_QUOTE,             0x5A    },
                { KEY_EN_RIGHT_SHIFT,       0x63    },
                { KEY_EN_RIGHT_CONTROL,     0x5C    },

                { KEY_EN_BACKSPACE,         0x68    },
                { KEY_EN_RIGHT_BRACKET,     0x61    },
                { KEY_EN_POUND,             0x62    },
                { KEY_EN_LEFT_ARROW,        0x64    },

                { KEY_EN_ISO_ENTER,         0x6A    },
                { KEY_EN_UP_ARROW,          0x6B    },
                { KEY_EN_DOWN_ARROW,        0x6C    },

                { KEY_EN_INSERT,            0x70    },
                { KEY_EN_DELETE,            0x71    },
                { KEY_EN_PAGE_UP,           0x72    },
                { KEY_EN_PAGE_DOWN,         0x73    },
                { KEY_EN_RIGHT_ARROW,       0x74    },
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_FALCHION_LAYOUT_KEYS_ANSI,
            68,
            5,
            16,
            {
                { KEY_EN_ESCAPE,            0x00    },
                { KEY_EN_TAB,               0x01    },
                { KEY_EN_CAPS_LOCK,         0x02    },
                { KEY_EN_LEFT_SHIFT,        0x03    },
                { KEY_EN_LEFT_CONTROL,      0x04    },

                { KEY_EN_1,                 0x08    },
                { KEY_EN_LEFT_WINDOWS,      0x0C    },

                { KEY_EN_2,                 0x10    },
                { KEY_EN_Q,                 0x09    },
                { KEY_EN_A,                 0x0A    },
                { KEY_EN_Z,                 0x13    },
                { KEY_EN_LEFT_ALT,          0x14    },

                { KEY_EN_3,                 0x18    },
                { KEY_EN_W,                 0x11    },
                { KEY_EN_S,                 0x12    },
                { KEY_EN_X,                 0x1B    },

                { KEY_EN_4,                 0x20    },
                { KEY_EN_E,                 0x19    },
                { KEY_EN_D,                 0x1A    },
                { KEY_EN_C,                 0x23    },

                { KEY_EN_5,                 0x28    },
                { KEY_EN_R,                 0x21    },
                { KEY_EN_F,                 0x22    },
                { KEY_EN_V,                 0x2B    },

                { KEY_EN_6,                 0x30    },
                { KEY_EN_T,                 0x29    },
                { KEY_EN_G,                 0x2A    },
                { KEY_EN_B,                 0x33    },
                { KEY_EN_SPACE,             0x34    },

                { KEY_EN_7,                 0x38    },
                { KEY_EN_Y,                 0x31    },
                { KEY_EN_H,                 0x32    },
                { KEY_EN_N,                 0x3B    },

                { KEY_EN_8,                 0x40    },
                { KEY_EN_U,                 0x39    },
                { KEY_EN_J,                 0x3A    },
                { KEY_EN_M,                 0x43    },

                { KEY_EN_9,                 0x48    },
                { KEY_EN_I,                 0x41    },
                { KEY_EN_K,                 0x42    },
                { KEY_EN_COMMA,             0x4B    },

                { KEY_EN_0,                 0x50    },
                { KEY_EN_O,                 0x49    },
                { KEY_EN_L,                 0x4A    },
                { KEY_EN_PERIOD,            0x53    },
                { KEY_EN_RIGHT_ALT,         0x4C    },

                { KEY_EN_MINUS,             0x58    },
                { KEY_EN_P,                 0x51    },
                { KEY_EN_SEMICOLON,         0x52    },
                { KEY_EN_FORWARD_SLASH,     0x5B    },
                { KEY_EN_RIGHT_FUNCTION,    0x54    },

                { KEY_EN_EQUALS,            0x60    },
                { KEY_EN_LEFT_BRACKET,      0x59    },
                { KEY_EN_QUOTE,             0x5A    },
                { KEY_EN_RIGHT_SHIFT,       0x63    },
                { KEY_EN_RIGHT_CONTROL,     0x5C    },

                { KEY_EN_BACKSPACE,         0x68    },
                { KEY_EN_RIGHT_BRACKET,     0x61    },
                { KEY_EN_LEFT_ARROW,        0x64    },

                { KEY_EN_ANSI_BACK_SLASH,   0x69    },
                { KEY_EN_ANSI_ENTER,        0x6A    },
                { KEY_EN_UP_ARROW,          0x6B    },
                { KEY_EN_DOWN_ARROW,        0x6C    },

                { KEY_EN_INSERT,            0x70    },
                { KEY_EN_DELETE,            0x71    },
                { KEY_EN_PAGE_UP,           0x72    },
                { KEY_EN_PAGE_DOWN,         0x73    },
                { KEY_EN_RIGHT_ARROW,       0x74    },
            }
        }
    },
};


static std::map<int,layout_info> AsusClaymoreNoNumpadLayouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_CLAYMORE_NO_NUMPAD_LAYOUT_KEYS_ISO,
            89,
            7,
            19,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },
                { KEY_EN_1,                   0x09 },
                { KEY_EN_ISO_BACK_SLASH,      0x0C },
                { KEY_EN_LEFT_WINDOWS,        0x0D },
                { KEY_EN_F1,                  0x10 },
                { KEY_EN_2,                   0x11 },
                { KEY_EN_Q,                   0x0A },
                { KEY_EN_A,                   0x0B },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },
                { KEY_EN_F2,                  0x18 },
                { KEY_EN_3,                   0x19 },
                { KEY_EN_W,                   0x12 },
                { KEY_EN_S,                   0x13 },
                { KEY_EN_X,                   0x1C },
                { KEY_EN_F3,                  0x20 },
                { KEY_EN_4,                   0x21 },
                { KEY_EN_E,                   0x1A },
                { KEY_EN_D,                   0x1B },
                { KEY_EN_C,                   0x24 },
                { KEY_EN_F4,                  0x28 },
                { KEY_EN_5,                   0x29 },
                { KEY_EN_R,                   0x22 },
                { KEY_EN_F,                   0x23 },
                { KEY_EN_V,                   0x2C },
                { KEY_EN_6,                   0x31 },
                { KEY_EN_T,                   0x2A },
                { KEY_EN_G,                   0x2B },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x25 },
                { KEY_EN_F5,                  0x38 },
                { KEY_EN_7,                   0x39 },
                { KEY_EN_Y,                   0x32 },
                { KEY_EN_H,                   0x33 },
                { KEY_EN_N,                   0x3C },
                { KEY_EN_F6,                  0x40 },
                { KEY_EN_8,                   0x41 },
                { KEY_EN_U,                   0x3A },
                { KEY_EN_J,                   0x3B },
                { KEY_EN_M,                   0x44 },
                { "Logo",                     0x45 },
                { KEY_EN_F7,                  0x48 },
                { KEY_EN_9,                   0x49 },
                { KEY_EN_I,                   0x42 },
                { KEY_EN_K,                   0x43 },
                { KEY_EN_COMMA,               0x4C },
                { KEY_EN_F8,                  0x50 },
                { KEY_EN_0,                   0x51 },
                { KEY_EN_O,                   0x4A },
                { KEY_EN_L,                   0x4B },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },
                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x59 },
                { KEY_EN_P,                   0x52 },
                { KEY_EN_SEMICOLON,           0x53 },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x55 },
                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x61 },
                { KEY_EN_LEFT_BRACKET,        0x5A },
                { KEY_EN_QUOTE,               0x5B },
                { KEY_EN_MENU,                0x5D },
                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x69 },
                { KEY_EN_RIGHT_BRACKET,       0x62 },
                { KEY_EN_POUND,               0x63 },
                { KEY_EN_RIGHT_SHIFT,         0x6C },
                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ISO_ENTER,           0x6B },
                { KEY_EN_RIGHT_CONTROL,       0x6D },
                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },
                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },
                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 }
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_CLAYMORE_NO_NUMPAD_LAYOUT_KEYS_ANSI,
            88,
            7,
            19,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },
                { KEY_EN_1,                   0x09 },
                { KEY_EN_LEFT_WINDOWS,        0x0D },
                { KEY_EN_F1,                  0x10 },
                { KEY_EN_2,                   0x11 },
                { KEY_EN_Q,                   0x0A },
                { KEY_EN_A,                   0x0B },
                { KEY_EN_Z,                   0x0C },
                { KEY_EN_LEFT_ALT,            0x15 },
                { KEY_EN_F2,                  0x18 },
                { KEY_EN_3,                   0x19 },
                { KEY_EN_W,                   0x12 },
                { KEY_EN_S,                   0x13 },
                { KEY_EN_X,                   0x14 },
                { KEY_EN_F3,                  0x20 },
                { KEY_EN_4,                   0x21 },
                { KEY_EN_E,                   0x1A },
                { KEY_EN_D,                   0x1B },
                { KEY_EN_C,                   0x1C },
                { KEY_EN_F4,                  0x28 },
                { KEY_EN_5,                   0x29 },
                { KEY_EN_R,                   0x22 },
                { KEY_EN_F,                   0x23 },
                { KEY_EN_V,                   0x24 },
                { KEY_EN_6,                   0x31 },
                { KEY_EN_T,                   0x2A },
                { KEY_EN_G,                   0x2B },
                { KEY_EN_B,                   0x2C },
                { KEY_EN_SPACE,               0x25 },
                { KEY_EN_F5,                  0x38 },
                { KEY_EN_7,                   0x39 },
                { KEY_EN_Y,                   0x32 },
                { KEY_EN_H,                   0x33 },
                { KEY_EN_N,                   0x34 },
                { KEY_EN_F6,                  0x40 },
                { KEY_EN_8,                   0x41 },
                { KEY_EN_U,                   0x3A },
                { KEY_EN_J,                   0x3B },
                { KEY_EN_M,                   0x3C },
                { "Logo",                     0x3D },
                { KEY_EN_F7,                  0x48 },
                { KEY_EN_9,                   0x49 },
                { KEY_EN_I,                   0x42 },
                { KEY_EN_K,                   0x43 },
                { KEY_EN_COMMA,               0x44 },
                { KEY_EN_F8,                  0x50 },
                { KEY_EN_0,                   0x51 },
                { KEY_EN_O,                   0x4A },
                { KEY_EN_L,                   0x4B },
                { KEY_EN_PERIOD,              0x4C },
                { KEY_EN_RIGHT_ALT,           0x4D },
                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x59 },
                { KEY_EN_P,                   0x52 },
                { KEY_EN_SEMICOLON,           0x53 },
                { KEY_EN_FORWARD_SLASH,       0x54 },
                { KEY_EN_RIGHT_FUNCTION,      0x55 },
                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x61 },
                { KEY_EN_LEFT_BRACKET,        0x5A },
                { KEY_EN_QUOTE,               0x5B },
                { KEY_EN_MENU,                0x5D },
                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x69 },
                { KEY_EN_RIGHT_BRACKET,       0x62 },
                { KEY_EN_RIGHT_SHIFT,         0x6C },
                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ANSI_BACK_SLASH,     0x6A },
                { KEY_EN_ANSI_ENTER,          0x6B },
                { KEY_EN_RIGHT_CONTROL,       0x6D },
                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },
                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },
                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 }
            }
        }
    },
};

static std::map<int,layout_info> AsusClaymoreNumpadRightLayouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_CLAYMORE_NUMPAD_RIGHT_LAYOUT_KEYS_ISO,
            106,
            7,
            24,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },
                { KEY_EN_1,                   0x09 },
                { KEY_EN_ISO_BACK_SLASH,      0x0C },
                { KEY_EN_LEFT_WINDOWS,        0x0D },
                { KEY_EN_F1,                  0x10 },
                { KEY_EN_2,                   0x11 },
                { KEY_EN_Q,                   0x0A },
                { KEY_EN_A,                   0x0B },
                { KEY_EN_Z,                   0x14 },
                { KEY_EN_LEFT_ALT,            0x15 },
                { KEY_EN_F2,                  0x18 },
                { KEY_EN_3,                   0x19 },
                { KEY_EN_W,                   0x12 },
                { KEY_EN_S,                   0x13 },
                { KEY_EN_X,                   0x1C },
                { KEY_EN_F3,                  0x20 },
                { KEY_EN_4,                   0x21 },
                { KEY_EN_E,                   0x1A },
                { KEY_EN_D,                   0x1B },
                { KEY_EN_C,                   0x24 },
                { KEY_EN_F4,                  0x28 },
                { KEY_EN_5,                   0x29 },
                { KEY_EN_R,                   0x22 },
                { KEY_EN_F,                   0x23 },
                { KEY_EN_V,                   0x2C },
                { KEY_EN_6,                   0x31 },
                { KEY_EN_T,                   0x2A },
                { KEY_EN_G,                   0x2B },
                { KEY_EN_B,                   0x34 },
                { KEY_EN_SPACE,               0x25 },
                { KEY_EN_F5,                  0x38 },
                { KEY_EN_7,                   0x39 },
                { KEY_EN_Y,                   0x32 },
                { KEY_EN_H,                   0x33 },
                { KEY_EN_N,                   0x3C },
                { KEY_EN_F6,                  0x40 },
                { KEY_EN_8,                   0x41 },
                { KEY_EN_U,                   0x3A },
                { KEY_EN_J,                   0x3B },
                { KEY_EN_M,                   0x44 },
                { "Logo",                     0x45 },
                { KEY_EN_F7,                  0x48 },
                { KEY_EN_9,                   0x49 },
                { KEY_EN_I,                   0x42 },
                { KEY_EN_K,                   0x43 },
                { KEY_EN_COMMA,               0x4C },
                { KEY_EN_F8,                  0x50 },
                { KEY_EN_0,                   0x51 },
                { KEY_EN_O,                   0x4A },
                { KEY_EN_L,                   0x4B },
                { KEY_EN_PERIOD,              0x54 },
                { KEY_EN_RIGHT_ALT,           0x4D },
                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x59 },
                { KEY_EN_P,                   0x52 },
                { KEY_EN_SEMICOLON,           0x53 },
                { KEY_EN_FORWARD_SLASH,       0x5C },
                { KEY_EN_RIGHT_FUNCTION,      0x55 },
                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x61 },
                { KEY_EN_LEFT_BRACKET,        0x5A },
                { KEY_EN_QUOTE,               0x5B },
                { KEY_EN_MENU,                0x5D },
                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x69 },
                { KEY_EN_RIGHT_BRACKET,       0x62 },
                { KEY_EN_POUND,               0x63 },
                { KEY_EN_RIGHT_SHIFT,         0x6C },
                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ISO_ENTER,           0x6B },
                { KEY_EN_RIGHT_CONTROL,       0x6D },
                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },
                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },
                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },
                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },
                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },
                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },
                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 }
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_CLAYMORE_NUMPAD_RIGHT_LAYOUT_KEYS_ANSI,
            105,
            7,
            24,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },
                { KEY_EN_1,                   0x09 },
                { KEY_EN_LEFT_WINDOWS,        0x0D },
                { KEY_EN_F1,                  0x10 },
                { KEY_EN_2,                   0x11 },
                { KEY_EN_Q,                   0x0A },
                { KEY_EN_A,                   0x0B },
                { KEY_EN_Z,                   0x0C },
                { KEY_EN_LEFT_ALT,            0x15 },
                { KEY_EN_F2,                  0x18 },
                { KEY_EN_3,                   0x19 },
                { KEY_EN_W,                   0x12 },
                { KEY_EN_S,                   0x13 },
                { KEY_EN_X,                   0x14 },
                { KEY_EN_F3,                  0x20 },
                { KEY_EN_4,                   0x21 },
                { KEY_EN_E,                   0x1A },
                { KEY_EN_D,                   0x1B },
                { KEY_EN_C,                   0x1C },
                { KEY_EN_F4,                  0x28 },
                { KEY_EN_5,                   0x29 },
                { KEY_EN_R,                   0x22 },
                { KEY_EN_F,                   0x23 },
                { KEY_EN_V,                   0x24 },
                { KEY_EN_6,                   0x31 },
                { KEY_EN_T,                   0x2A },
                { KEY_EN_G,                   0x2B },
                { KEY_EN_B,                   0x2C },
                { KEY_EN_SPACE,               0x25 },
                { KEY_EN_F5,                  0x38 },
                { KEY_EN_7,                   0x39 },
                { KEY_EN_Y,                   0x32 },
                { KEY_EN_H,                   0x33 },
                { KEY_EN_N,                   0x34 },
                { KEY_EN_F6,                  0x40 },
                { KEY_EN_8,                   0x41 },
                { KEY_EN_U,                   0x3A },
                { KEY_EN_J,                   0x3B },
                { KEY_EN_M,                   0x3C },
                { "Logo",                     0x3D },
                { KEY_EN_F7,                  0x48 },
                { KEY_EN_9,                   0x49 },
                { KEY_EN_I,                   0x42 },
                { KEY_EN_K,                   0x43 },
                { KEY_EN_COMMA,               0x44 },
                { KEY_EN_F8,                  0x50 },
                { KEY_EN_0,                   0x51 },
                { KEY_EN_O,                   0x4A },
                { KEY_EN_L,                   0x4B },
                { KEY_EN_PERIOD,              0x4C },
                { KEY_EN_RIGHT_ALT,           0x4D },
                { KEY_EN_F9,                  0x60 },
                { KEY_EN_MINUS,               0x59 },
                { KEY_EN_P,                   0x52 },
                { KEY_EN_SEMICOLON,           0x53 },
                { KEY_EN_FORWARD_SLASH,       0x54 },
                { KEY_EN_RIGHT_FUNCTION,      0x55 },
                { KEY_EN_F10,                 0x68 },
                { KEY_EN_EQUALS,              0x61 },
                { KEY_EN_LEFT_BRACKET,        0x5A },
                { KEY_EN_QUOTE,               0x5B },
                { KEY_EN_MENU,                0x5D },
                { KEY_EN_F11,                 0x70 },
                { KEY_EN_BACKSPACE,           0x69 },
                { KEY_EN_RIGHT_BRACKET,       0x62 },
                { KEY_EN_RIGHT_SHIFT,         0x6C },
                { KEY_EN_F12,                 0x78 },
                { KEY_EN_ANSI_BACK_SLASH,     0x6A },
                { KEY_EN_ANSI_ENTER,          0x6B },
                { KEY_EN_RIGHT_CONTROL,       0x6D },
                { KEY_EN_PRINT_SCREEN,        0x80 },
                { KEY_EN_INSERT,              0x81 },
                { KEY_EN_DELETE,              0x82 },
                { KEY_EN_LEFT_ARROW,          0x85 },
                { KEY_EN_SCROLL_LOCK,         0x88 },
                { KEY_EN_HOME,                0x89 },
                { KEY_EN_END,                 0x8A },
                { KEY_EN_UP_ARROW,            0x8C },
                { KEY_EN_DOWN_ARROW,          0x8D },
                { KEY_EN_PAUSE_BREAK,         0x90 },
                { KEY_EN_PAGE_UP,             0x91 },
                { KEY_EN_PAGE_DOWN,           0x92 },
                { KEY_EN_RIGHT_ARROW,         0x95 },
                { KEY_EN_NUMPAD_LOCK,         0x99 },
                { KEY_EN_NUMPAD_7,            0x9A },
                { KEY_EN_NUMPAD_4,            0x9B },
                { KEY_EN_NUMPAD_1,            0x9C },
                { KEY_EN_NUMPAD_0,            0x9D },
                { KEY_EN_NUMPAD_DIVIDE,       0xA1 },
                { KEY_EN_NUMPAD_8,            0xA2 },
                { KEY_EN_NUMPAD_5,            0xA3 },
                { KEY_EN_NUMPAD_2,            0xA4 },
                { KEY_EN_NUMPAD_TIMES,        0xA9 },
                { KEY_EN_NUMPAD_9,            0xAA },
                { KEY_EN_NUMPAD_6,            0xAB },
                { KEY_EN_NUMPAD_3,            0xAC },
                { KEY_EN_NUMPAD_PERIOD,       0xAD },
                { KEY_EN_NUMPAD_MINUS,        0xB1 },
                { KEY_EN_NUMPAD_PLUS,         0xB2 },
                { KEY_EN_NUMPAD_ENTER,        0xB4 }
            }
        }
    },
};

static std::map<int,layout_info> AsusClaymoreNumpadLeftLayouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_CLAYMORE_NUMPAD_LEFT_LAYOUT_KEYS_ISO,
            106,
            7,
            24,
            {
                { KEY_EN_NUMPAD_LOCK,         0x01 },
                { KEY_EN_NUMPAD_7,            0x02 },
                { KEY_EN_NUMPAD_4,            0x03 },
                { KEY_EN_NUMPAD_1,            0x04 },
                { KEY_EN_NUMPAD_0,            0x05 },
                { KEY_EN_NUMPAD_DIVIDE,       0x09 },
                { KEY_EN_NUMPAD_8,            0x0A },
                { KEY_EN_NUMPAD_5,            0x0B },
                { KEY_EN_NUMPAD_2,            0x0C },
                { KEY_EN_NUMPAD_TIMES,        0x11 },
                { KEY_EN_NUMPAD_9,            0x12 },
                { KEY_EN_NUMPAD_6,            0x13 },
                { KEY_EN_NUMPAD_3,            0x14 },
                { KEY_EN_NUMPAD_PERIOD,       0x15 },
                { KEY_EN_NUMPAD_MINUS,        0x19 },
                { KEY_EN_NUMPAD_PLUS,         0x1A },
                { KEY_EN_NUMPAD_ENTER,        0x1C },
                { KEY_EN_ESCAPE,              0x20 },
                { KEY_EN_BACK_TICK,           0x21 },
                { KEY_EN_TAB,                 0x22 },
                { KEY_EN_CAPS_LOCK,           0x23 },
                { KEY_EN_LEFT_SHIFT,          0x24 },
                { KEY_EN_LEFT_CONTROL,        0x25 },
                { KEY_EN_1,                   0x29 },
                { KEY_EN_ISO_BACK_SLASH,      0x2C },
                { KEY_EN_LEFT_WINDOWS,        0x2D },
                { KEY_EN_F1,                  0x30 },
                { KEY_EN_2,                   0x31 },
                { KEY_EN_Q,                   0x2A },
                { KEY_EN_A,                   0x2B },
                { KEY_EN_Z,                   0x34 },
                { KEY_EN_LEFT_ALT,            0x35 },
                { KEY_EN_F2,                  0x38 },
                { KEY_EN_3,                   0x39 },
                { KEY_EN_W,                   0x32 },
                { KEY_EN_S,                   0x33 },
                { KEY_EN_X,                   0x3C },
                { KEY_EN_F3,                  0x40 },
                { KEY_EN_4,                   0x41 },
                { KEY_EN_E,                   0x3A },
                { KEY_EN_D,                   0x3B },
                { KEY_EN_C,                   0x44 },
                { KEY_EN_F4,                  0x48 },
                { KEY_EN_5,                   0x49 },
                { KEY_EN_R,                   0x42 },
                { KEY_EN_F,                   0x43 },
                { KEY_EN_V,                   0x4C },
                { KEY_EN_6,                   0x51 },
                { KEY_EN_T,                   0x4A },
                { KEY_EN_G,                   0x4B },
                { KEY_EN_B,                   0x54 },
                { KEY_EN_SPACE,               0x45 },
                { KEY_EN_F5,                  0x58 },
                { KEY_EN_7,                   0x59 },
                { KEY_EN_Y,                   0x52 },
                { KEY_EN_H,                   0x53 },
                { KEY_EN_N,                   0x5C },
                { KEY_EN_F6,                  0x60 },
                { KEY_EN_8,                   0x61 },
                { KEY_EN_U,                   0x5A },
                { KEY_EN_J,                   0x5B },
                { KEY_EN_M,                   0x64 },
                { "Logo",                     0x65 },
                { KEY_EN_F7,                  0x68 },
                { KEY_EN_9,                   0x69 },
                { KEY_EN_I,                   0x62 },
                { KEY_EN_K,                   0x63 },
                { KEY_EN_COMMA,               0x6C },
                { KEY_EN_F8,                  0x70 },
                { KEY_EN_0,                   0x71 },
                { KEY_EN_O,                   0x6A },
                { KEY_EN_L,                   0x6B },
                { KEY_EN_PERIOD,              0x74 },
                { KEY_EN_RIGHT_ALT,           0x6D },
                { KEY_EN_F9,                  0x80 },
                { KEY_EN_MINUS,               0x79 },
                { KEY_EN_P,                   0x72 },
                { KEY_EN_SEMICOLON,           0x73 },
                { KEY_EN_FORWARD_SLASH,       0x7C },
                { KEY_EN_RIGHT_FUNCTION,      0x75 },
                { KEY_EN_F10,                 0x88 },
                { KEY_EN_EQUALS,              0x81 },
                { KEY_EN_LEFT_BRACKET,        0x7A },
                { KEY_EN_QUOTE,               0x7B },
                { KEY_EN_MENU,                0x7D },
                { KEY_EN_F11,                 0x90 },
                { KEY_EN_BACKSPACE,           0x89 },
                { KEY_EN_RIGHT_BRACKET,       0x82 },
                { KEY_EN_POUND,               0x83 },
                { KEY_EN_RIGHT_SHIFT,         0x8C },
                { KEY_EN_F12,                 0x98 },
                { KEY_EN_ISO_ENTER,           0x8B },
                { KEY_EN_RIGHT_CONTROL,       0x8D },
                { KEY_EN_PRINT_SCREEN,        0xA0 },
                { KEY_EN_INSERT,              0xA1 },
                { KEY_EN_DELETE,              0xA2 },
                { KEY_EN_LEFT_ARROW,          0xA5 },
                { KEY_EN_SCROLL_LOCK,         0xA8 },
                { KEY_EN_HOME,                0xA9 },
                { KEY_EN_END,                 0xAA },
                { KEY_EN_UP_ARROW,            0xAC },
                { KEY_EN_DOWN_ARROW,          0xAD },
                { KEY_EN_PAUSE_BREAK,         0xB0 },
                { KEY_EN_PAGE_UP,             0xB1 },
                { KEY_EN_PAGE_DOWN,           0xB2 },
                { KEY_EN_RIGHT_ARROW,         0xB5 }
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_CLAYMORE_NUMPAD_LEFT_LAYOUT_KEYS_ANSI,
            105,
            7,
            24,
            {
                { KEY_EN_NUMPAD_LOCK,         0x01 },
                { KEY_EN_NUMPAD_7,            0x02 },
                { KEY_EN_NUMPAD_4,            0x03 },
                { KEY_EN_NUMPAD_1,            0x04 },
                { KEY_EN_NUMPAD_0,            0x05 },
                { KEY_EN_NUMPAD_DIVIDE,       0x09 },
                { KEY_EN_NUMPAD_8,            0x0A },
                { KEY_EN_NUMPAD_5,            0x0B },
                { KEY_EN_NUMPAD_2,            0x0C },
                { KEY_EN_NUMPAD_TIMES,        0x11 },
                { KEY_EN_NUMPAD_9,            0x12 },
                { KEY_EN_NUMPAD_6,            0x13 },
                { KEY_EN_NUMPAD_3,            0x14 },
                { KEY_EN_NUMPAD_PERIOD,       0x15 },
                { KEY_EN_NUMPAD_MINUS,        0x19 },
                { KEY_EN_NUMPAD_PLUS,         0x1A },
                { KEY_EN_NUMPAD_ENTER,        0x1C },
                { KEY_EN_ESCAPE,              0x20 },
                { KEY_EN_BACK_TICK,           0x21 },
                { KEY_EN_TAB,                 0x22 },
                { KEY_EN_CAPS_LOCK,           0x23 },
                { KEY_EN_LEFT_SHIFT,          0x24 },
                { KEY_EN_LEFT_CONTROL,        0x25 },
                { KEY_EN_1,                   0x29 },
                { KEY_EN_LEFT_WINDOWS,        0x2D },
                { KEY_EN_F1,                  0x30 },
                { KEY_EN_2,                   0x31 },
                { KEY_EN_Q,                   0x2A },
                { KEY_EN_A,                   0x2B },
                { KEY_EN_Z,                   0x2C },
                { KEY_EN_LEFT_ALT,            0x35 },
                { KEY_EN_F2,                  0x38 },
                { KEY_EN_3,                   0x39 },
                { KEY_EN_W,                   0x32 },
                { KEY_EN_S,                   0x33 },
                { KEY_EN_X,                   0x34 },
                { KEY_EN_F3,                  0x40 },
                { KEY_EN_4,                   0x41 },
                { KEY_EN_E,                   0x3A },
                { KEY_EN_D,                   0x3B },
                { KEY_EN_C,                   0x3C },
                { KEY_EN_F4,                  0x48 },
                { KEY_EN_5,                   0x49 },
                { KEY_EN_R,                   0x42 },
                { KEY_EN_F,                   0x43 },
                { KEY_EN_V,                   0x44 },
                { KEY_EN_6,                   0x51 },
                { KEY_EN_T,                   0x4A },
                { KEY_EN_G,                   0x4B },
                { KEY_EN_B,                   0x4C },
                { KEY_EN_SPACE,               0x45 },
                { KEY_EN_F5,                  0x58 },
                { KEY_EN_7,                   0x59 },
                { KEY_EN_Y,                   0x52 },
                { KEY_EN_H,                   0x53 },
                { KEY_EN_N,                   0x54 },
                { KEY_EN_F6,                  0x60 },
                { KEY_EN_8,                   0x61 },
                { KEY_EN_U,                   0x5A },
                { KEY_EN_J,                   0x5B },
                { KEY_EN_M,                   0x5C },
                { "Logo",                     0x5D },
                { KEY_EN_F7,                  0x68 },
                { KEY_EN_9,                   0x69 },
                { KEY_EN_I,                   0x62 },
                { KEY_EN_K,                   0x63 },
                { KEY_EN_COMMA,               0x64 },
                { KEY_EN_F8,                  0x70 },
                { KEY_EN_0,                   0x71 },
                { KEY_EN_O,                   0x6A },
                { KEY_EN_L,                   0x6B },
                { KEY_EN_PERIOD,              0x6C },
                { KEY_EN_RIGHT_ALT,           0x6D },
                { KEY_EN_F9,                  0x80 },
                { KEY_EN_MINUS,               0x79 },
                { KEY_EN_P,                   0x72 },
                { KEY_EN_SEMICOLON,           0x73 },
                { KEY_EN_FORWARD_SLASH,       0x74 },
                { KEY_EN_RIGHT_FUNCTION,      0x75 },
                { KEY_EN_F10,                 0x88 },
                { KEY_EN_EQUALS,              0x81 },
                { KEY_EN_LEFT_BRACKET,        0x7A },
                { KEY_EN_QUOTE,               0x7B },
                { KEY_EN_MENU,                0x7D },
                { KEY_EN_F11,                 0x90 },
                { KEY_EN_BACKSPACE,           0x89 },
                { KEY_EN_RIGHT_BRACKET,       0x82 },
                { KEY_EN_RIGHT_SHIFT,         0x8C },
                { KEY_EN_F12,                 0x98 },
                { KEY_EN_ANSI_BACK_SLASH,     0x8A },
                { KEY_EN_ANSI_ENTER,          0x8B },
                { KEY_EN_RIGHT_CONTROL,       0x8D },
                { KEY_EN_PRINT_SCREEN,        0xA0 },
                { KEY_EN_INSERT,              0xA1 },
                { KEY_EN_DELETE,              0xA2 },
                { KEY_EN_LEFT_ARROW,          0xA5 },
                { KEY_EN_SCROLL_LOCK,         0xA8 },
                { KEY_EN_HOME,                0xA9 },
                { KEY_EN_END,                 0xAA },
                { KEY_EN_UP_ARROW,            0xAC },
                { KEY_EN_DOWN_ARROW,          0xAD },
                { KEY_EN_PAUSE_BREAK,         0xB0 },
                { KEY_EN_PAGE_UP,             0xB1 },
                { KEY_EN_PAGE_DOWN,           0xB2 },
                { KEY_EN_RIGHT_ARROW,         0xB5 }
            }
        }
    },
};

static std::map<int,layout_info> AsusTufK1Layouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            *ASUS_TUF_K1_LAYOUT_KEYS,
            5,
            1,
            5,
            {
                { "Keyboard LED 1",             0x00 },
                { "Keyboard LED 2",             0x01 },
                { "Keyboard LED 3",             0x02 },
                { "Keyboard LED 4",             0x03 },
                { "Keyboard LED 5",             0x04 },
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            *ASUS_TUF_K1_LAYOUT_KEYS,
            5,
            1,
            5,
            {
                { "Keyboard LED 1",             0x00 },
                { "Keyboard LED 2",             0x01 },
                { "Keyboard LED 3",             0x02 },
                { "Keyboard LED 4",             0x03 },
                { "Keyboard LED 5",             0x04 },
            }
        }
    },
};
