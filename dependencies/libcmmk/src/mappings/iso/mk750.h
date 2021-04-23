/*
 * This file is part of libcmmk.
 *
 * libcmmk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * libcmmk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libcmmk.  If not, see <http://www.gnu.org/licenses/>.
 */
static keyboard_layout layout_iso_mk750 = {
	/*
	ESC   F1    F2    F3    F4    XXX   F5    F6    F7    F8    XXX   F9    F10   F11   F12   PRN   SCL   PAU   MUT   PLA   REW   FWD   */
	{96,  97,   98,   99,   104,  -1,   105,  106,  112,   113, -1,   114,  67,   68,   69,   102,  103,  107,  111,  110,  108,  109},

	/*
	^     1     2     3     4     5     6     7     8     9     0     ß     ´     XXX   BCK   INS   HOM   PUP   #LCK  #/    #*    #-    */
	{0,   1,    8,    9,    16,   17,   24,   25,   32,   33,   40,   41,   48,   -1,   49,   56,   57,   64,   72,   73,   80,   81},

	/*
	TAB   Q     W     E     R     T     Z/Y   U     I     O     P     Ü/[   +/]   XXX   ENT   DEL   END   PDN   #7    #8    #9    #+    */
	{2,   3,    10,   11,   18,   19,   26,   27,   34,   35,   42,   43,   50,   -1,   52,   58,   59,   66,   74,   75,   82,   83},

	/*
	CAP   A     S     D     F     G     H     J     K     L     Ö/;   Ä/'   #/C42 XXX   XXX   XXX   XXX   XXX   #4    #5     #6   XXX   */
	{4,   5,    12,   13,   20,   21,   28,   29,   36,   37,   44,   45,   119,  -1,   -1,   -1,   -1,   -1,   76,   77,   84,   -1},

	/*
	LSHFT </C45 Y/Z   X     C     V     B     N     M     ,     .     -//   XXX   XXX   RSHFT XXX   UP    XXX   #1    #2    #3    #ENTER */
	{6,   118,  7,    14,   15,   22,   23,   30,   31,   38,   39,   46,   -1,   -1,   47,   -1,   61,   -1,  78,   79,   86,    85},

	/*
	LCTRL LWIN  LALT  XXX   XXX   XXX   SPACE XXX   XXX   XXX   RALT  RWIN  FN    XXX   RCTRL LEFT  DOWN  RIGHT #0    XXX   #,    XXX    */
	{91,  90,   92,   -1,   -1,   -1,   93,   -1,   -1,   -1,   53,   95,   60,   -1,   54,   63,   62,   70,   71,   -1,   87,   -1},

	/*
	XXX   LED1  LED2  LED3  LED4  XXX   LED6  LED7  LED8  LED9  LED10 LED11 LED12 LED13 LED14 LED15 XXX   LED17 LED18 LED19 LED20 XXX   */
	{-1,  128,  140,  126,  141,  -1,   142,  125,  143,  151,  124,  123,  122,  150,  121,  120,  -1,   137,  135,  136,  138,  -1}
};