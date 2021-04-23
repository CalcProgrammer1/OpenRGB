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
static keyboard_layout layout_iso_sk630 = {
	/*
	ESC    F1    F2    F3    F4    XXX   F5    F6    F7    F8    XXX   F9    F10   F11   F12   PRN   SCL   PAU   MUT   PLA   REW   FWD    */
	{ 9,   33,   41,   49,   57,   -1,   73,   81,   89,   97,   -1,  105,  113,  121,  129,  137,  145,  153,   -1,   -1,   -1,   -1},

	/*
	^      1     2     3     4     5     6     7     8     9     0     ß     ´     XXX   BCK   INS   HOM   PUP   #LCK  #/    #*    #-     */
	{10,   26,   34,   42,   50,   58,   66,   74,   82,   90,   98,  106,  114,   -1,  130,  138,  146,  154,   -1,   -1,   -1,   -1},

	/*
	TAB    Q     W     E     R     T     Z/Y   U     I     O     P     Ü/[   +/]   XXX   ENT   DEL   END   PDN   #7    #8    #9    #+     */
	{11,   27,   35,   43,   51,   59,   67,   75,   83,   91,   99,  107,  115,   -1,  132,  139,  147,  155,   -1,   -1,   -1,   -1},

	/*
	CAP    A     S     D     F     G     H     J     K     L     Ö/;   Ä/'  #/C42  XXX   XXX   XXX   XXX   XXX   #4    #5     #6   XXX    */
	{12,   28,   36,   44,   52,   60,   68,   76,   84,   92,  100,  108,  124,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1},

	/*
	LSHFT </C45  Y/Z   X     C     V     B     N     M     ,     .     -//   XXX   XXX  RSHFT  XXX   UP    XXX   #1    #2    #3    #ENTER */
	{13,   21,   29,   37,   45,   53,   61,   69,   77,   85,   93,  101,   -1,   -1,  133,   -1,  149,   -1,   -1,   -1,   -1,   -1},

	/*
	LCTRL LWIN  LALT  XXX   XXX   XXX   SPACE XXX   XXX   XXX   RALT  RWIN   FN    XXX  RCTRL LEFT  DOWN  RIGHT #0    XXX   #,    XXX    */
	{14,   22,   30,   -1,   -1,   -1,   62,   -1,   -1,   -1,   94,  102,  110,   -1,  134,  142,  150,  158,   -1,   -1,   -1,   -1},

	/* Bottom row does not exist */
	{-1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1},
};