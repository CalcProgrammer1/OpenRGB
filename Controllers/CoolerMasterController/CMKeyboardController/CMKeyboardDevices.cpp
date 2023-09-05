/*---------------------------------------------------------*\
| CMKeyboardDevices.cpp                                     |
|                                                           |
|   Device list for Cooler Master keyboards                 |
|                                                           |
|   Tam D (too.manyhobbies)                     30 Nov 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "CMKeyboardDevices.h"

/*-------------------------------------------------------------------------*\
|  Coolermaster Key Values                                                  |
\*-------------------------------------------------------------------------*/

const std::vector<unsigned int> mk_pro_s_keymap =
{
/*  ESC   F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK        */
	96,   97,   98,   99,   104,  105,  106,  112,  113,  114,  67,   68,   69,   102,  103,  107,
/*  BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  */
	0,    1,    8,    9,    16,   17,   24,   25,   32,   33,   40,   41,   48,   49,   56,   57,   64,
/*  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  */
	2,    3,    10,   11,   18,   19,   26,   27,   34,   35,   42,   43,   50,   51,   58,   59,   66,
/*  CPLK  A     S     D     F     G     H     J     K     L     ;     "     #     ENTR                    */
	4,    5,    12,   13,   20,   21,   28,   29,   36,   37,   44,   45,   89,   52,
/*  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /     RSFT              ARWU        */
	6,    100,  7,    14,   15,   22,   23,   30,   31,   38,   39,   46,   47,               61,
/*  LCTL  LWIN  LALT              SPC               RALT  RFNC  RMNU  RCTL              ARWL  ARWD  ARWR  */
	91,   90,   92,               93,               94,   60,   95,   54,               63,   62,   70,
};

const std::vector<unsigned int> mk_pro_l_keymap =
{
/*  ESC   F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK        P1    P2    P3    P4   */
	11,   22,   30,   25,   27,   7,    51,   57,   62,   86,   87,   83,   85,   79,   72,   0,
/*	BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  NMLK  NMDV  NMTM  NMMI */
	14,   15,   23,   31,   39,   38,   46,   47,   55,   63,   71,   70,   54,   81,   3,    1,    2,    100,  108,  116,  118,
/*  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  NM7   NM8   NM9   NMPL */
	9,    8,    16,   24,   32,   33,   41,   40,   48,   56,   64,   65,   49,   82,   94,   92,   88,   96,   104,  112,  110,
/*  CPLK    A   S     D     F     G     H     J     K     L     ;     '     \     ENTR                    NM4   NM5   NM6        */
	17,   10,   18,   26,   34,   35,   43,   42,   50,   58,   66,   67,   68,   84,                     97,   105,  113,
/*  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /     RSFT              ARWU        NM1   NM2   NM3   NMER */
	73,   19,   12,   20,   28,   36,   37,   45,   44,   52,   60,   69,   74,               80,         98,   106,  114,  111,
/*  LCTL  LWIN  LALT              SPACE             RALT  RWFNC RMNU  RCTRL             ARWL  ARDN  ARWR  NM0         NMPD       */
	6,    90,   75,               91,               77,   78,   61,   4,                95,   93,   5,    107,        115,
};

const std::vector<unsigned int> mk850_keymap =
{
/*        ESC   F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK        AIM   AIMU   AIMD */
	      6,    27,   34,   41,   48,   62,   69,   76,   83,   90,   97,   104,  111,  118,  125,  132,
/*	M1    BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  NMLK  NMDV  NMTM  NMMI */
	      7,    21,   28,   35,   42,   49,   56,   63,   70,   77,   84,   91,   98,   112,  119,  126,  133,  254,  147,  154,  161,
/*  M2    TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  NM7   NM8   NM9   NMPL */
	      8,    22,   29,   36,   43,   50,   57,   64,   71,   78,   85,   92,   99,   113,  120,  127,  134,  141,  148,  155,  162,
/*  M3    CPLK  A     S     D     F     G     H     J     K     L     ;     '     #     ENTR                    NM4   NM5   NM6        */
	      9,    23,   30,   37,   44,   51,   58,   65,   72,   79,   86,   93,   0,    114,                    142,  149,  156,
/*  M4    LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /     RSFT              ARWU        NM1   NM2   NM3   NMER */
	      10,   0,    24,   31,   38,   45,   52,   59,   66,   73,   80,   87,   115,              129,        143,  150,  157,  164,
/*  M5    LCTL  LWIN  LALT              SPACE             RALT  RWFNC RMNU  RCTRL             ARWL  ARDN  ARWR  NM0         NMPD       */
	      11,   18,   25,               53,               81,   88,   95,   116,              123,  130,  137,  144,        158,
};

/*-------------------------------------------------------------*\
| CoolerMaster SK (60%)                                         |
\*-------------------------------------------------------------*/
const std::vector<unsigned int> sk620_keymap =
{
/*	T1    T2    T3    T4    T5    T6    T7    T8    T9    T10   T11   T12   T13   T14   T15        */
/*	L1    ESC   1     2     3     4     5     6     7     8     9     0     -     =     BPSC        R1  */
	   	  8,    15,   22,   29,   36,   43,   50,   57,   64,   71,   78,   85,   92,   106,
/*  L2    TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]                 R2  */
	  	  9,    23,   30,   37,   44,   51,   58,   65,   72,   79,   86,   93,   100,
/*  L3    CPLK  A     S     D     F     G     H     J     K     L     ;     "     \     ENTR        R3  */
	  	  10,   24,   31,   38,   45,   52,   59,   66,   73,   80,   87,   94,   101,  108,
/*  L4    LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     #     RSFT  ARWU   DEL  R$  */
	      11,   18,   25,   32,   39,   46,   53,   60,   67,   74,   81,   88,   95,   102,   109,
/*  L5    LCTL  LWIN  LALT              SPACE                         RALT  RWFNC ARWL  ARDN  ARWR  R5  */
	  	  12,   19,   26,               54,                           82,   89,   96,   103,   110,
/*  B1    B2    B3    B4    B5    B6    B7    B8    B9    B10   B11   B12   B13   B14   B15    B15      */
};

/*-------------------------------------------------------------*\
| CoolerMaster SK (60%)                                         |
\*-------------------------------------------------------------*/
const std::vector<unsigned int> sk622_keymap =
{
/*	T1    T2    T3    T4    T5    T6    T7    T8    T9    T10   T11   T12   T13   T14   T15        */
/*	L1    ESC   1     2     3     4     5     6     7     8     9     0     -     =     BPSC        R1  */
	   	  8,    15,   22,   29,   36,   43,   50,   57,   64,   71,   78,   85,   92,   106,
/*  L2    TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]                 R2  */
	  	  9,    23,   30,   37,   44,   51,   58,   65,   72,   79,   86,   93,   100,
/*  L3    CPLK  A     S     D     F     G     H     J     K     L     ;     "     \     ENTR        R3  */
	  	  10,   24,   31,   38,   45,   52,   59,   66,   73,   80,   87,   94,   101,  108,
/*  L4    LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     #     RSFT  ARWU   DEL  R$  */
	      11,   18,   25,   32,   39,   46,   53,   60,   67,   74,   81,   88,   95,   102,   109,
/*  L5    LCTL  LWIN  LALT              SPACE                         RALT  RWFNC ARWL  ARDN  ARWR  R5  */
	  	  12,   19,   26,               54,                           82,   89,   96,   103,   110,
/*  B1    B2    B3    B4    B5    B6    B7    B8    B9    B10   B11   B12   B13   B14   B15    B15      */
};

const std::vector<unsigned int> sk630_keymap =
{
/*  ESC   F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK        */
	9,    33,   41,   49,   57,    73,   81,   89,   97,  105,  113,  121,  129,  137,  145,  153,
/*  BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  */
	10,   26,   34,   42,   50,   58,   66,   74,   82,   90,   98,  106,   114,  130,  138,  146,  154,
/*  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  */
	11,   27,   35,   43,   51,   59,   67,   75,   83,   91,   99,  107,   115,  131,  139,  147,  155,
/*  CPLK  A     S     D     F     G     H     J     K     L     ;     "     #     ENTR                    */
	12,   28,   36,   44,   52,   60,   68,   76,   84,   92,  100,  108,   116,  132,
/*  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /     RSFT              ARWU        */
	13,   21,   29,   37,   45,   53,   61,   69,   77,   85,   93,  101,   133,              149,
/*  LCTL  LWIN  LALT              SPC               RALT  RFNC  RMNU  RCTL              ARWL  ARWD  ARWR  */
	14,   22,   30,               62,               94,  102,  110,   134,              142,  150,  158,
};

const std::vector<unsigned int> sk650_keymap =
{
/*  ESC   F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK                               */
	0,    24,   32,   40,   48,   64,   72,   80,   88,   96,   104,  112,  120,  128,  136,  144,
/*	BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  NMLK  NMDV  NMTM  NMMI */
	1,    17,   25,   33,   41,   49,   57,   65,   73,   81,   89,   97,   105,  121,  129,  137,  145,  153,  161,  169,  177,
/*  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  NM7   NM8   NM9   NMPL */
	2,    18,   26,   34,   42,   50,   58,   66,   74,   82,   90,   98,   106,  122,  130,  138,  146,  154,  162,  170,  178,
/*  CPLK  A     S     D     F     G     H     J     K     L     ;     '     #     ENTR                    NM4   NM5   NM6        */
	3,    19,   27,   35,   43,   51,   59,   67,   75,   83,   91,   99,   107,  123,                    155,  163,  171,
/*  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /           RSFT        ARWU        NM1   NM2   NM3   NMER */
	4,    20,   28,   36,   44,   52,   60,   68,   76,   84,   92,   100,        124,        140,        156,  164,  172,  180,
/*  LCTL  LWIN  LALT              SPACE             RALT  RWFNC RMNU  RCTRL             ARWL  ARDN  ARWR        NM0   NMPD       */
	5,    13,   21,               53,                     85,   93,   101,        125,  133,  141,  149,        165,  173,
};

const std::vector<unsigned int> sk652_keymap =
{

/*  ESC         F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK                         */
    9,          33,   41,   49,   57,   73,   81,   89,   97,   105,  113,  121,  129,  137,  145,  153,
/*  BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  NMLK  NMDV  NMTM  NMMI */
    10,   26,   34,   42,   50,   58,   66,   74,   82,   90,   98,   106,  114,  130,  138,  146,  154,  162,  170,  178,  186,
/*  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  NM7   NM8   NM9   NMPL */
    11,   27,   35,   43,   51,   59,   67,   75,   83,   91,   99,   107,  115,  131,  139,  147,  155,  163,  171,  179,  187,
/*  CPLK  A     S     D     F     G     H     J     K     L     ;     '     #     ENTR                    NM4   NM5   NM6        */
    12,   28,   36,   44,   52,   60,   68,   76,   84,   92,   100,  108,  116,  132,                    164,  172,  180,
/*  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /           RSFT        ARWU        NM1   NM2   NM3   NMER */
    13,   21,   29,   37,   45,   53,   61,   69,   77,   85,   93,   101,        133,        149,        165,  173,  181,  189,
/*  LCTL  LWIN  LALT              SPACE                         RALT  RWFNC RMNU  RCTRL ARWL  ARDN  ARWR        NM0   NMPD       */
    14,   22,   30,               62,                           94,   102,  110,  134,  142,  150,  158,        174,  182,
};

const std::vector<unsigned int> sk653_keymap =
{
/*  ESC   F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK                               */
	0,    24,   32,   40,   48,   64,   72,   80,   88,   96,   104,  112,  120,  128,  136,  144,
/*	BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  NMLK  NMDV  NMTM  NMMI */
	1,    17,   25,   33,   41,   49,   57,   65,   73,   81,   89,   97,   105,  121,  129,  137,  145,  153,  161,  169,  177,
/*  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  NM7   NM8   NM9   NMPL */
	2,    18,   26,   34,   42,   50,   58,   66,   74,   82,   90,   98,   106,  122,  130,  138,  146,  154,  162,  170,  178,
/*  CPLK  A     S     D     F     G     H     J     K     L     ;     '     #     ENTR                    NM4   NM5   NM6        */
	3,    19,   27,   35,   43,   51,   59,   67,   75,   83,   91,   99,   107,  123,                    155,  163,  171,
/*  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /           RSFT        ARWU        NM1   NM2   NM3   NMER */
	4,    20,   28,   36,   44,   52,   60,   68,   76,   84,   92,   100,        124,        140,        156,  164,  172,  180,
/*  LCTL  LWIN  LALT              SPACE             RALT  RWFNC RMNU  RCTRL             ARWL  ARDN  ARWR        NM0   NMPD       */
	5,    13,   21,               53,                     85,   93,   101,        125,  133,  141,  149,        165,  173,
};

const std::vector<unsigned int> mk730_keymap =
{
/*      ESC   F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK            */
        7,    28,   35,   42,   49,   63,   70,   77,   84,   91,   98,   105,  112,   119,  126,  133,
/*	L1  BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  R1  */
	    8,    22,   29,   36,   43,   50,   57,   64,   71,   78,   85,   92,   99,    113,  120,  127,  134,
/*  L2  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  R2  */
	    9,    23,   30,   37,   44,   51,   58,   65,   72,   79,   86,   93,   100,   114,  121,  128,  135,
/*  L3  CPLK  A     S     D     F     G     H     J     K     L     ;     '     #     ENTR                    R3  */
	    10,   24,   31,   38,   45,   52,   59,   66,   73,   80,   87,   94,   108,   115,
/*  L4  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /           RSFT        ARWU        R4  */
	    11,   18,   25,   32,   39,   46,   53,   60,   67,   74,   81,   88,          116,       130,
/*      LCTL  LWIN  LALT              SPACE             RALT  RWFNC RMNU  RCTRL             ARWL  ARDN  ARWR      */
	    12,   19,   26,               54,               82,   89,   96,   117,               124, 131,  138,
/*      B1    B2    B3    B4    B5    B6    B7    B8    B9    B10   B11   B12   B12                          */
};

const std::vector<unsigned int> mk750_keymap =
{
/*      ESC   F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK  MUT   PLA   REW   FFWD            */
        7,    28,   35,   42,   49,   63,   70,   77,   84,   91,   98,   105,  112,  119,  136,  133,
/*	L1  BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  NMLK  NMDV  NMTM  NMMI  R1  */
	    8,    22,   29,   36,   43,   50,   57,   64,   71,   78,   85,   92,   99,   113,  120,  127,  134,    0,  148,  155,  162,
/*  L2  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  NM7   NM8   NM9   NMPL  R2  */
	    9,    23,   30,   37,   44,   51,   58,   65,   72,   79,   86,   93,   100,  114,  121,  128,  135,  142,  149,  156,  163,
/*  L3  CPLK  A     S     D     F     G     H     J     K     L     ;     '     #     ENTR                    NM4   NM5   NM6         R3  */
	    10,   24,   31,   38,   45,   52,   59,   66,   73,   80,   87,   94,   108,  115,                    143,  150,  157,
/*  L4  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /           RSFT        ARWU        NM1   NM2   NM3   NMER  R4  */
	    11,   18,   25,   32,   39,   46,   53,   60,   67,   74,   81,   88,         116,        130,        144,  151,  158,  165,
/*      LCTL  LWIN  LALT              SPACE             RALT  RWFNC RMNU  RCTRL             ARWL  ARDN  ARWR        NM0   NMPD            */
	    12,   19,   26,               54,               82,   89,   96,   117,              124,  131,  138,        152,  159,
/*      B1    B2    B3    B4    B5    B6    B7    B8    B9    B10   B11   B12   B12   B13   B14   B15   B16  B17  B18                     */
};


const std::vector<unsigned int> ck530_keymap =
{
/*  ESC   F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK        */
    6,    21,   27,   34,   41,   55,   62,   69,   76,   83,   90,   97,   104,  111,  118,  125,
/*  BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  */
    7,    15,   22,   28,   35,   42,   49,   56,   63,   70,   77,   84,   91,   105,  112,  119,  126,
/*  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  */
    8,    16,   23,   29,   36,   43,   50,   57,   64,   71,   78,   85,   92,   106,  113,  120,  127,
/*  CPLK  A     S     D     F     G     H     J     K     L     ;     "     #     ENTR                    */
    9,    17,   24,   30,   37,   44,   51,   58,   65,   72,   79,   86,   93,   107,
/*  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /           RSFT        ARWU        */
    10,   18,   25,   31,   38,   45,   52,   59,   66,   73,   80,   87,         108,        122,
/*  LCTL  LWIN  LALT              SPC                     RALT  RFNC  RMNU  RCTL        ARWL  ARWD  ARWR  */
    11,   12,   19,               46,                    74,   81,    88,  109,         116,  123,  130,
};

const std::vector<unsigned int> ck530_v2_keymap =
{
/*  ESC   F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK        */
    7,	  28,   35,   42,   49,   63,   70,   77,   84,   91,   98,  105,  112,   119,  126,  133,
/*  BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  */
    8,    22,   29,   36,   43,   50,   57,   64,   71,   78,   85,   92,   99,   113,  120,  127,  134,
/*  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  */
    9,    23,   30,   37,   44,   51,   58,   65,   72,   79,   86,   93,  100,   114,  121,  128,  135,
/*  CPLK  A     S     D     F     G     H     J     K     L     ;     "     #     ENTR                    */
    10,   24,   31,   38,   45,   52,   59,   66,   73,   80,   87,   94,  108,   115,
/*  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /           RSFT        ARWU        */
    11,   18,   25,   32,   39,   46,   53,   60,   67,   74,   88,   81,         116,        130,
/*  LCTL  LWIN  LALT              SPC                     RALT  RFNC  RMNU  RCTL        ARWL  ARWD  ARWR  */
    12,   19,   26,               54,                     82,   89,   96,   117,        124,  131,  138,
};

const std::vector<unsigned int> ck550_v2_keymap =
{
/*  ESC         F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK                         */
	0,          18,   24,   30,   36,   48,   54,   60,   66,   72,   78,   84,   90,   96,   102,  108,
/*	BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  NMLK  NMDV  NMTM  NMMI */
	1,    13,   19,   25,   31,   37,   43,   49,   55,   61,   67,   73,   79,   91,   97,   103,  109,  115,  121,  127,  133,
/*  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  NM7   NM8   NM9   NMPL */
    2,    14,   20,   26,   32,   38,   44,   50,   56,   62,   68,   74,   80,   92,   98,   104,  110,  116,  122,  128,  134,
/*  CPLK  A     S     D     F     G     H     J     K     L     ;     '     #     ENTR                    NM4   NM5   NM6        */
    3,    15,   21,   27,   33,   39,   45,   51,   57,   63,   69,   75,   87,   93,                     117,  123,  129,
/*  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /           RSFT        ARWU        NM1   NM2   NM3   NMER */
    4,    10,   16,   22,   28,   34,   40,   46,   52,   58,   64,   70,/*76,*/  94,         106,        118,  124,  130,  136,
/*  LCTL  LWIN  LALT              SPACE                   RALT  RWFNC RMNU        RCTRL  ARWL ARDN  ARWR        NM0   NMPD       */
    5,    11,   17,               41,                     65,   71,   77,         95,    101, 107,  113,        125,  131,
};

const std::vector<unsigned int> ck552_keymap =
{
/*  ESC         F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK                         */
	0,          18,   24,   30,   36,   48,   54,   60,   66,   72,   78,   84,   90,   96,   102,  108,
/*	BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BPSC  INS   HOME  PGUP  NMLK  NMDV  NMTM  NMMI */
	1,    13,   19,   25,   31,   37,   43,   49,   55,   61,   67,   73,   79,   91,   97,   103,  109,  115,  121,  127,  133,
/*  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     \     DEL   END   PGDN  NM7   NM8   NM9   NMPL */
    2,    14,   20,   26,   32,   38,   44,   50,   56,   62,   68,   74,   80,   92,   98,   104,  110,  116,  122,  128,  134,
/*  CPLK  A     S     D     F     G     H     J     K     L     ;     '     #     ENTR                    NM4   NM5   NM6        */
    3,    15,   21,   27,   33,   39,   45,   51,   57,   63,   69,   75,   81,   93,                     117,  123,  129,
/*  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /           RSFT        ARWU        NM1   NM2   NM3   NMER */
    4,    10,   16,   22,   28,   34,   40,   46,   52,   58,   64,   70,/*76,*/  94,         106,        118,  124,  130,  136,
/*  LCTL  LWIN  LALT              SPACE                   RALT  RWFNC RMNU        RCTRL  ARWL ARDN  ARWR        NM0   NMPD       */
    5,    11,   17,               41,                     65,   71,   77,         95,    101, 107,  113,        125,  131,
};

/*-------------------------------------------------------------------------*\
|  KEYMAPS                                                                  |
\*-------------------------------------------------------------------------*/
keyboard_keymap_overlay_values mk_pro_s_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        mk_pro_s_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Key,        Alternate Name,     OpCode,                         |
        \*---------------------------------------------------------------------------------------------------------*/
    },
};

keyboard_keymap_overlay_values mk_pro_l_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        mk_pro_l_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Name,       Alternate Name,     OpCode,                         |
        \*---------------------------------------------------------------------------------------------------------*/
        {   0,      0,      17,         101,        "Key: P1",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      18,         109,        "Key: P2",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      19,         117,        "Key: P3",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      20,         119,        "Key: P4",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    },
};

/*-------------------------------------------------------------*\
| CoolerMaster MK85O Keyboard                                   |
| Unknown Keys: ISO\, ISO# set to 0                             |
\*-------------------------------------------------------------*/
keyboard_keymap_overlay_values mk850_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        mk850_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Name,           Alternate Name,     OpCode,                     |
        \*---------------------------------------------------------------------------------------------------------*/
        {   0,      0,      17,       146,          "Key: Aim <|>", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, }, // aimpad <|>
        {   0,      0,      18,       153,          "Key: Aim -",   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, }, // aimpad +
        {   0,      0,      19,       160,          "Key: Aim +",   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, }, // aimpad -
        {   0,      1,       0,         0,          "Key: M5",      KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,       0,         1,          "Key: M4",      KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,       0,         2,          "Key: M3",      KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,       0,         3,          "Key: M2",      KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,       0,         4,          "Key: M1",      KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    },
};

keyboard_keymap_overlay_values sk620_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_SIXTY,
    {
        sk620_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-----------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                     |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,     OpCode,                             |
        \*-----------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          7,         "Light: Top 1",              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_ROW,         },
        {   0,      0,      1,         14,         "Light: Top 2",              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      2,         21,         "Light: Top 3",              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      3,         28,         "Light: Top 4",              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      4,         35,         "Light: Top 5",              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      5,         42,         "Light: Top 6",              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      6,         49,         "Light: Top 7",              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      7,         56,         "Light: Top 8",              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      8,         63,         "Light: Top 9",              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      9,         70,         "Light: Top 10",             KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      10,        77,         "Light: Top 11",             KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      11,        84,         "Light: Top 12",             KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      12,        91,         "Light: Top 13",             KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      13,        98,         "Light: Top 14",             KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      14,       105,         "Light: Top 14",             KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      0,          0,         "Light: Left 1",             KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      0,          1,         "Light: Left 2",             KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      0,          2,         "Light: Left 3",             KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      0,          3,         "Light: Left 4",             KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      0,          4,         "Light: Left 5",             KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      16,       112,         "Light: Right 1",            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      16,       113,         "Light: Right 2",            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      16,       114,         "Light: Right 3",            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      16,       115,         "Light: Right 4",            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      16,       116,         "Light: Right 5",            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       0,         6,         "Light: Bottom 1",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_ROW,         },
        {   0,      6,       1,        20,         "Light: Bottom 2",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       2,        27,         "Light: Bottom 3",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       3,        34,         "Light: Bottom 4",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       4,        41,         "Light: Bottom 5",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       5,        48,         "Light: Bottom 6",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       6,        55,         "Light: Bottom 7",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       7,        62,         "Light: Bottom 8",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       8,        69,         "Light: Bottom 9",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       9,        76,         "Light: Bottom 10",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       10,       83,         "Light: Bottom 11",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       11,       90,         "Light: Bottom 12",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       12,       97,         "Light: Bottom 13",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       13,      104,         "Light: Bottom 14",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       14,      111,         "Light: Bottom 15",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       14,      118,         "Light: Bottom 16",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    },
};

keyboard_keymap_overlay_values sk622_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_SIXTY,
    {
        sk622_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Name,               Alternate Name,     OpCode,                 |
        \*---------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          7,          "Light: Top 1",     KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_ROW, },
        {   0,      0,      1,         14,          "Light: Top 2",     KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      2,         21,          "Light: Top 3",     KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      3,         28,          "Light: Top 4",     KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      4,         35,          "Light: Top 5",     KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      5,         42,          "Light: Top 6",     KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      6,         49,          "Light: Top 7",     KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      7,         56,          "Light: Top 8",     KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      8,         63,          "Light: Top 9",     KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      9,         70,          "Light: Top 10",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      10,        77,          "Light: Top 11",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      11,        84,          "Light: Top 12",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      12,        91,          "Light: Top 13",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      13,        98,          "Light: Top 14",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      14,       105,          "Light: Top 14",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      0,          0,          "Light: Left 1",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      0,          1,          "Light: Left 2",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      0,          2,          "Light: Left 3",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      0,          3,          "Light: Left 4",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      0,          4,          "Light: Left 5",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      16,       112,          "Light: Right 1",   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      16,       113,          "Light: Right 2",   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      16,       114,          "Light: Right 3",   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      16,       115,          "Light: Right 4",   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      16,       116,          "Light: Right 5",   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       0,         6,          "Light: Bottom 1",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_ROW, },
        {   0,      6,       1,        20,          "Light: Bottom 2",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       2,        27,          "Light: Bottom 3",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       3,        34,          "Light: Bottom 4",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       4,        41,          "Light: Bottom 5",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       5,        48,          "Light: Bottom 6",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       6,        55,          "Light: Bottom 7",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       7,        62,          "Light: Bottom 8",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       8,        69,          "Light: Bottom 9",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       9,        76,          "Light: Bottom 10", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       10,       83,          "Light: Bottom 11", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       11,       90,          "Light: Bottom 12", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       12,       97,          "Light: Bottom 13", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       13,      104,          "Light: Bottom 14", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       14,      111,          "Light: Bottom 15", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       14,      118,          "Light: Bottom 16", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    },
};

keyboard_keymap_overlay_values sk630_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        sk630_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
        \*---------------------------------------------------------------------------------------------------------*/
    }
};

keyboard_keymap_overlay_values sk650_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        sk650_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
        \*---------------------------------------------------------------------------------------------------------*/
    }
};

keyboard_keymap_overlay_values sk652_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        sk652_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
        \*---------------------------------------------------------------------------------------------------------*/
    }
};

keyboard_keymap_overlay_values sk653_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        sk653_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
        \*---------------------------------------------------------------------------------------------------------*/
    }
};

/*-------------------------------------------------------------*\
| CoolerMaster MK730 Keyboard                                   |
\*-------------------------------------------------------------*/
keyboard_keymap_overlay_values mk730_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        mk730_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Name,               Alternate Name,     OpCode,                 |
        \*---------------------------------------------------------------------------------------------------------*/
        {   0,      1,      0,          1,          "Light: Left 1",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      0,          2,          "Light: Left 2",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      0,          3,          "Light: Left 3",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      0,          4,          "Light: Left 4",    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      18,       141,          "Light: Right 1",   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      18,       142,          "Light: Right 2",   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      18,       143,          "Light: Right 3",   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      18,       144,          "Light: Right 4",   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       1,        13,          "Light: Bottom 1",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_ROW, },
        {   0,      6,       2,        20,          "Light: Bottom 2",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       3,        27,          "Light: Bottom 3",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       4,        34,          "Light: Bottom 4",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       5,        41,          "Light: Bottom 5",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       6,        55,          "Light: Bottom 6",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       7,        62,          "Light: Bottom 7",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       8,        69,          "Light: Logo",      KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      10,        76,          "Light: Bottom 8",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      11,        90,          "Light: Bottom 9",  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      12,       104,          "Light: Bottom 10", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      13,       111,          "Light: Bottom 11", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      14,       118,          "Light: Bottom 12", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      15,       125,          "Light: Bottom 13", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    }
};

/*-------------------------------------------------------------*\
| CoolerMaster MK750 Keyboard                                   |
| based on keymap defined in Signal.                            |
| The keymap needs the following adjustments                    |
| NMLK - Unknown set to 0                                       |
| SCLK - Unknown set to 0                                       |
| CAPS - Unknown set to 0                                       |
| Guesses on ISO\ and #                                         |
\*-------------------------------------------------------------*/
keyboard_keymap_overlay_values mk750_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        mk750_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,     OpCode,         |
        \*---------------------------------------------------------------------------------------------------------*/
        {   0,      0,      17,       140,          KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      18,       147,          KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      19,       154,          KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      20,       161,          KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      0,          1,          "Light: Left 1",            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      0,          2,          "Light: Left 2",            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      0,          3,          "Light: Left 3",            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      0,          4,          "Light: Left 4",            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      22,       170,          "Light: Right 1",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      22,       171,          "Light: Right 2",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      22,       172,          "Light: Right 3",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      22,       173,          "Light: Right 4",           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       1,        20,          "Light: Bottom 1",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_ROW, },
        {   0,      6,       2,        27,          "Light: Bottom 2",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       3,        34,          "Light: Bottom 3",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       4,        41,          "Light: Bottom 4",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       5,        55,          "Light: Bottom 5",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       6,        62,          "Light: Bottom 6",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       7,        69,          "Light: Bottom 7",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       8,        76,          "Light: Bottom 8",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,       9,        83,          "Light: Bottom 9",          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      10,        90,          "Light: Bottom 10",         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      11,       104,          "Light: Bottom 11",         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      12,       111,          "Light: Bottom 12",         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      13,       118,          "Light: Bottom 13",         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      14,       125,          "Light: Bottom 14",         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      15,       132,          "Light: Bottom 15",         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      16,       146,          "Light: Bottom 16",         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      17,       153,          "Light: Bottom 17",         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      18,       160,          "Light: Bottom 18",         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    }
};

keyboard_keymap_overlay_values ck530_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        ck530_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
        \*---------------------------------------------------------------------------------------------------------*/
    }
};

keyboard_keymap_overlay_values ck530_v2_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        ck530_v2_keymap,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
        \*---------------------------------------------------------------------------------------------------------*/
    }
};


keyboard_keymap_overlay_values ck550v2_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        ck550_v2_keymap,
        {
            {
                /* Add more regional layout fixes here */
                /*---------------------------------------------------------------------------------------------------------*\
                | Edit Keys                                                                                                 |
                |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
                \*---------------------------------------------------------------------------------------------------------*/
            },
        },
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Name,           Alternate Name,     OpCode,                             |
        \*---------------------------------------------------------------------------------------------------------*/
        {   0,      0,      17,        120,         "Indicator: N", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      18,        126,         "Indicator: C", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      19,        132,         "Indicator: S", KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    },
};

keyboard_keymap_overlay_values ck552_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        ck552_keymap,
        {
            {
                /* Add more regional layout fixes here */
                /*---------------------------------------------------------------------------------------------------------*\
                | Edit Keys                                                                                                 |
                |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
                \*---------------------------------------------------------------------------------------------------------*/
            },
        },
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
        \*---------------------------------------------------------------------------------------------------------*/
    },
};

static const cm_kb_zone cm_generic_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
};

cm_kb_device mk_pro_s_device
{
    COOLERMASTER_KEYBOARD_PRO_S_PID,
    {
        &cm_generic_zone,
    },
    &mk_pro_s_layout,
};

cm_kb_device mk_pro_l_device
{
    COOLERMASTER_KEYBOARD_PRO_L_PID,
    {
        &cm_generic_zone,
    },
    &mk_pro_l_layout,
};

cm_kb_device mk850_device
{
    COOLERMASTER_KEYBOARD_MK850_PID,
    {
        &cm_generic_zone,
    },
    &mk850_layout,
};

cm_kb_device sk620w_device
{
    COOLERMASTER_KEYBOARD_SK620W_PID,
    {
        &cm_generic_zone,
    },
    &sk620_layout,
};

cm_kb_device sk620b_device
{
    COOLERMASTER_KEYBOARD_SK620B_PID,
    {
        &cm_generic_zone,
    },
    &sk620_layout,
};

cm_kb_device sk622w_device
{
    COOLERMASTER_KEYBOARD_SK622W_PID,
    {
        &cm_generic_zone,
    },
    &sk622_layout,
};

cm_kb_device sk622b_device
{
    COOLERMASTER_KEYBOARD_SK622B_PID,
    {
        &cm_generic_zone,
    },
    &sk622_layout,
};

cm_kb_device sk630_device
{
    COOLERMASTER_KEYBOARD_SK630_PID,
    {
        &cm_generic_zone,
    },
    &sk630_layout,
};

cm_kb_device sk650_device
{
    COOLERMASTER_KEYBOARD_SK650_PID,
    {
        &cm_generic_zone,
    },
    &sk650_layout,
};

cm_kb_device sk652_device
{
    COOLERMASTER_KEYBOARD_SK652_PID,
    {
        &cm_generic_zone,
    },
    &sk652_layout,
};

cm_kb_device sk653_device
{
    COOLERMASTER_KEYBOARD_SK653_PID,
    {
        &cm_generic_zone,
    },
    &sk652_layout,
};

/*---------------------------------------------------------*\
| TODO: Keymap is incomplete. Extra keys mode enabled to    |
| aid in key discovery.                                     |
\*---------------------------------------------------------*/
cm_kb_device mk730_device
{
    COOLERMASTER_KEYBOARD_MK730_PID,
    {
        &cm_generic_zone,
    },
    &mk730_layout,
};

cm_kb_device mk750_device
{
    COOLERMASTER_KEYBOARD_MK750_PID,
    {
        &cm_generic_zone,
    },
    &mk750_layout,
};

cm_kb_device ck530_device
{
    COOLERMASTER_KEYBOARD_CK530_PID,
    {
        &cm_generic_zone,
    },
    &ck530_layout,
};

cm_kb_device ck530_v2_device
{
    COOLERMASTER_KEYBOARD_CK530_V2_PID,
    {
        &cm_generic_zone,
    },
    &ck530_v2_layout,
};

cm_kb_device ck550_v2_device
{
    COOLERMASTER_KEYBOARD_CK550_V2_PID,
    {
        &cm_generic_zone,
    },
    &ck550v2_layout,
};

cm_kb_device ck552_v2_device
{
    COOLERMASTER_KEYBOARD_CK552_V2_PID,
    {
        &cm_generic_zone,
    },
    &ck552_layout,
};

cm_kb_device mk_pro_l_white_device
{
    COOLERMASTER_KEYBOARD_PRO_L_WHITE_PID,
    {
        &cm_generic_zone,
    },
    &mk_pro_s_layout,
};

/*-----------------------------------------------------------------*\
|  KEYBOARDS                                                        |
\*-----------------------------------------------------------------*/
const cm_kb_device* cm_kb_devices[] =
{
    &mk_pro_s_device,
    &mk_pro_l_device,
    &mk850_device,
    &sk620w_device,
    &sk620b_device,
    &sk622w_device,
    &sk622b_device,
    &sk630_device,
    &sk650_device,
    &sk652_device,
    &sk653_device,
    &mk730_device,
    &mk750_device,
    &ck530_device,
    &ck530_v2_device,
    &ck550_v2_device,
    &ck552_v2_device,
    &mk_pro_l_white_device,
};

const unsigned int COOLERMASTER_KEYBOARD_DEVICE_COUNT = (sizeof(cm_kb_devices) / sizeof(cm_kb_devices[ 0 ]));
const cm_kb_device** cm_kb_device_list = cm_kb_devices;
