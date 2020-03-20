/*
* RGB <--> HSV conversion in integer arithmetics, to be used on Windows.
* Copyright (c) 2013 Martin Mitas
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#include "hsv.h"


#define MIN(a,b)      ((a) < (b) ? (a) : (b))
#define MAX(a,b)      ((a) > (b) ? (a) : (b))

#define MIN3(a,b,c)   MIN((a), MIN((b), (c)))
#define MAX3(a,b,c)   MAX((a), MAX((b), (c)))

#define RGBGetRValue(rgb)   (rgb & 0x000000FF)
#define RGBGetGValue(rgb)   ((rgb >> 8) & 0x000000FF)
#define RGBGetBValue(rgb)   ((rgb >> 16) & 0x000000FF)

#define ToRGBColor(r, g, b) ((b << 16) | (g << 8) | (r))

void rgb2hsv(unsigned int rgb, hsv_t* hsv)
{
    int r = RGBGetRValue(rgb);
    int g = RGBGetGValue(rgb);
    int b = RGBGetBValue(rgb);
	int m = MIN3(r, g, b);
	int M = MAX3(r, g, b);
	int delta = M - m;

	if (delta == 0) {
		/* Achromatic case (i.e. grayscale) */
		hsv->hue = -1;          /* undefined */
		hsv->saturation = 0;
	}
	else {
		int h;

		if (r == M)
			h = ((g - b) * 60 * HUE_DEGREE) / delta;
		else if (g == M)
			h = ((b - r) * 60 * HUE_DEGREE) / delta + 120 * HUE_DEGREE;
		else /*if(b == M)*/
			h = ((r - g) * 60 * HUE_DEGREE) / delta + 240 * HUE_DEGREE;

		if (h < 0)
			h += 360 * HUE_DEGREE;

		hsv->hue = h;

		/* The constatnt 8 is tuned to statistically cause as little
		* tolerated mismatches as possible in RGB -> HSV -> RGB conversion.
		* (See the unit test at the bottom of this file.)
		*/
		hsv->saturation = (256 * delta - 8) / M;
	}
	hsv->value = M;
}

unsigned int hsv2rgb(hsv_t* hsv)
{
    unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;

	if (hsv->saturation == 0) {
		r = g = b = hsv->value;
	}
	else {
		int h = (hsv->hue)%360;
		int s = hsv->saturation;
		int v = hsv->value;
		int i = h / (60 * HUE_DEGREE);
		int p = (256 * v - s*v) / 256;

		if (i & 1) {
			int q = (256 * 60 * HUE_DEGREE*v - h*s*v + 60 * HUE_DEGREE*s*v*i) / (256 * 60 * HUE_DEGREE);
			switch (i) {
			case 1:   r = q; g = v; b = p; break;
			case 3:   r = p; g = q; b = v; break;
			case 5:   r = v; g = p; b = q; break;
			}
		}
		else {
			int t = (256 * 60 * HUE_DEGREE*v + h*s*v - 60 * HUE_DEGREE*s*v*(i + 1)) / (256 * 60 * HUE_DEGREE);
			switch (i) {
			case 0:   r = v; g = t; b = p; break;
			case 2:   r = p; g = v; b = t; break;
			case 4:   r = t; g = p; b = v; break;
			}
		}
	}

    return ToRGBColor(r, g, b);
}


#ifdef TEST

#include <stdio.h>

#define DIFF(a,b)     ((a) >= (b) ? (a) - (b) : (b) - (a))

int
main(int argc, char** argv)
{
	int r0, g0, b0;
	hsv_t hsv;
	int r1, g1, b1;
	int ok = 0, fuzzy = 0, ko = 0, total;

	for (r0 = 0; r0 < 256; r0++) {
		for (g0 = 0; g0 < 256; g0++) {
			for (b0 = 0; b0 < 256; b0++) {
				COLORREF tmp;

				rgb2hsv(RGB(r0, g0, b0), &hsv);
				tmp = hsv2rgb(&hsv);
				r1 = GetRValue(tmp);
				g1 = GetGValue(tmp);
				b1 = GetBValue(tmp);

				if (r0 == r1 && b0 == b1 && g0 == g1)
					ok++;
				else if (DIFF(r0, r1) <= 1 && DIFF(b0, b1) <= 1 && DIFF(g0, g1) <= 1)
					fuzzy++;
				else
					ko++;
			}
		}
	}

	total = ok + fuzzy + ko;

	printf("Test results:\n");
	printf("  * exact matches: %8d (%5.2f%%)\n", ok, (100.0 * ok) / total);
	printf("  * fuzzy matches: %8d (%5.2f%%)\n", fuzzy, (100.0 * fuzzy) / total);
	printf("  * mismatches:    %8d (%5.2f%%)\n", ko, (100.0 * ko) / total);

	return (ko == 0 ? 0 : 1);
}
#endif  /* TEST */
