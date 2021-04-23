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
#include <libcmmk/libcmmk.h>

#ifndef WIN32
#include <unistd.h> /* getuid() */
#endif
#include <string.h> /* memset() */
#include <assert.h>

#include <hidapi/hidapi.h>

#ifdef CMMK_TRACE
#include <stdio.h>
#include <ctype.h>
#endif

/* Initialize keyboard layouts */
typedef int16_t keyboard_layout[CMMK_ROWS_MAX][CMMK_COLS_MAX];

#include "mappings/iso/pro_s.h"
#include "mappings/iso/pro_l.h"
#include "mappings/iso/mk750.h"
#include "mappings/iso/sk630.h"
#include "mappings/iso/sk650.h"

#include "mappings/ansi/pro_s.h"
#include "mappings/ansi/pro_l.h"
#include "mappings/ansi/mk750.h"
#include "mappings/ansi/sk630.h"
#include "mappings/ansi/sk650.h"

static keyboard_layout const *keyboard_layouts[] = {
	[CMMK_LAYOUT_US_S] = &layout_ansi_pro_s,
	[CMMK_LAYOUT_US_L] = &layout_ansi_pro_l,
	[CMMK_LAYOUT_US_MK750] = &layout_ansi_mk750,
	[CMMK_LAYOUT_US_SK630] = &layout_ansi_sk630,
	[CMMK_LAYOUT_US_SK650] = &layout_ansi_sk650,
	[CMMK_LAYOUT_EU_S] = &layout_iso_pro_s,
	[CMMK_LAYOUT_EU_L] = &layout_iso_pro_l,
	[CMMK_LAYOUT_EU_MK750] = &layout_iso_mk750,
	[CMMK_LAYOUT_EU_SK630] = &layout_iso_sk630,
	[CMMK_LAYOUT_EU_SK650] = &layout_iso_sk650,
};

/* Some global definitions */
enum {
	CMMK_USB_VENDOR = 0x2516,

	CMMK_USB_INTERFACE = 1,

	CMMK_USB_EP_IN = 0x04,
	CMMK_USB_EP_OUT = 0x83
};

/* linear -> matrix */
static int transpose(struct cmmk *dev, struct rgb const *linear, struct cmmk_color_matrix *matrix)
{
	int i;

	for (i = 0; i < CMMK_KEYLIST_SIZE; ++i) {
		if (dev->rowmap[i] < 0 || dev->colmap[i] < 0) {
			continue;
		}

		matrix->data[dev->rowmap[i]][dev->colmap[i]] = linear[i];
	}

	return CMMK_OK;
}

/* Too bad C doesn't have templates */
static int transpose_effects(struct cmmk *dev, uint8_t const *linear, struct cmmk_effect_matrix *matrix)
{
	int i;

	for (i = 0; i < CMMK_KEYLIST_SIZE; ++i) {
		if (dev->rowmap[i] < 0 || dev->colmap[i] < 0) {
			continue;
		}

		matrix->data[dev->rowmap[i]][dev->colmap[i]] = linear[i];
	}

	return CMMK_OK;
}

/* matrix -> linear */
int transpose_reverse(struct cmmk *dev, struct cmmk_color_matrix const *matrix, struct rgb *linear)
{
	keyboard_layout const *layout = keyboard_layouts[dev->layout];

	int i;
	int j;

	for (i = 0; i < CMMK_ROWS_MAX; ++i) {
		for (j = 0; j < CMMK_COLS_MAX; ++j) {
			int pos = 0;

			if ((pos = (*layout)[i][j]) < 0 || pos > CMMK_KEYLIST_SIZE) {
				continue;
			}

			linear[pos] = matrix->data[i][j];
		}
	}

	return CMMK_OK;
}

int transpose_effects_reverse(struct cmmk *dev, struct cmmk_effect_matrix const *matrix, uint8_t *linear)
{
	keyboard_layout const *layout = keyboard_layouts[dev->layout];

	int i;
	int j;

	for (i = 0; i < CMMK_ROWS_MAX; ++i) {
		for (j = 0; j < CMMK_COLS_MAX; ++j) {
			int pos = 0;

			if ((pos = (*layout)[i][j]) < 0 || pos > CMMK_KEYLIST_SIZE) {
				continue;
			}

			linear[pos] = matrix->data[i][j];
		}
	}

	return CMMK_OK;
}

#ifdef CMMK_TRACE
static void hexdump(void const *ptr, size_t buflen)
{
	unsigned char *buf = (unsigned char*)ptr;
	size_t i;
	size_t j;

	printf("        0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
	for (i = 0; i < buflen; i += 16) {
		printf("%06lx: ", i);

		for (j = 0; j < 16; j++) {
			if (i+j < buflen) {
				printf("%02x ", buf[i+j]);
			} else {
				printf("   ");
			}
		}

		printf(" ");

		for (j = 0; j < 16; j++) {
			if (i+j < buflen) {
				printf("%c", isprint(buf[i+j]) ? buf[i+j] : '.');
			}
		}

		printf("\n");
	}
}
#endif

static int send_command(hid_device *dev, unsigned char *data, size_t datasiz)
{
#ifdef CMMK_TRACE
	printf(">>\n");
	hexdump(data, datasiz);
#endif

	if (hid_write(dev, data, datasiz) < 0) {
		return CMMK_USB_COMM;
	}

	if (hid_read(dev, data, datasiz) < 0) {
		return CMMK_USB_COMM;
	}

#ifdef CMMK_TRACE
	printf("<<\n");
	hexdump(data, datasiz);
#endif

	return CMMK_OK;
}

int cmmk_find_device(int *product)
{
	static int supported_devices[] = {
		CMMK_USB_MASTERKEYS_PRO_L,
		CMMK_USB_MASTERKEYS_PRO_S,
		CMMK_USB_MASTERKEYS_MK750,
		CMMK_USB_MASTERKEYS_PRO_L_WHITE,
		CMMK_USB_MASTERKEYS_SK630,
		CMMK_USB_MASTERKEYS_SK650,
	};

	struct hid_device_info *list = NULL;

	int res = 1;

	list = hid_enumerate(CMMK_USB_VENDOR, 0);

	for (struct hid_device_info *dev = list; dev != NULL; dev = dev->next) {
		for (size_t j = 0; j < (sizeof(supported_devices) / sizeof(supported_devices[0])); ++j) {
			if (dev->product_id == supported_devices[j] && dev->interface_number == CMMK_USB_INTERFACE) {
				*product = dev->product_id;

				res = 0;

				break;
			}
		}
	}

	hid_free_enumeration(list);

	return res;
}


static int cmmk_try_determine_layout(struct cmmk *dev, int product)
{
	char fw[16];

	enum cmmk_layout_type general_layout = CMMK_LAYOUT_TYPE_ANSI;
	enum cmmk_product_type device_model;

	if (cmmk_get_firmware_version(dev, fw, sizeof(fw)) == 0) {
		if (fw[0] == '1') {
			/* ANSI firmware */
			general_layout = CMMK_LAYOUT_TYPE_ANSI;
		} else {
			general_layout = CMMK_LAYOUT_TYPE_ISO;
		}
	}

	switch ((enum cmmk_product) product) {
		case CMMK_USB_MASTERKEYS_PRO_L:
		case CMMK_USB_MASTERKEYS_PRO_L_WHITE: device_model = CMMK_PRODUCT_MASTERKEYS_PRO_L; break;
		case CMMK_USB_MASTERKEYS_PRO_S: device_model = CMMK_PRODUCT_MASTERKEYS_PRO_S; break;
		case CMMK_USB_MASTERKEYS_MK750: device_model = CMMK_PRODUCT_MASTERKEYS_MK750; break;
		case CMMK_USB_MASTERKEYS_SK630: device_model = CMMK_PRODUCT_MASTERKEYS_SK630; break;
		case CMMK_USB_MASTERKEYS_SK650: device_model = CMMK_PRODUCT_MASTERKEYS_SK650; break;
	}

	if (general_layout == CMMK_LAYOUT_TYPE_ANSI) {
		switch (device_model) {
			case CMMK_PRODUCT_MASTERKEYS_PRO_L: return CMMK_LAYOUT_US_L;
			case CMMK_PRODUCT_MASTERKEYS_PRO_S: return CMMK_LAYOUT_US_S;
			case CMMK_PRODUCT_MASTERKEYS_MK750: return CMMK_LAYOUT_US_MK750;
			case CMMK_PRODUCT_MASTERKEYS_SK630: return CMMK_LAYOUT_US_SK630;
			case CMMK_PRODUCT_MASTERKEYS_SK650: return CMMK_LAYOUT_US_SK630;
		}
	} else {
		switch (device_model) {
			case CMMK_PRODUCT_MASTERKEYS_PRO_L: return CMMK_LAYOUT_EU_L;
			case CMMK_PRODUCT_MASTERKEYS_PRO_S: return CMMK_LAYOUT_EU_S;
			case CMMK_PRODUCT_MASTERKEYS_MK750: return CMMK_LAYOUT_EU_MK750;
			case CMMK_PRODUCT_MASTERKEYS_SK630: return CMMK_LAYOUT_EU_SK630;
			case CMMK_PRODUCT_MASTERKEYS_SK650: return CMMK_LAYOUT_EU_SK650;
		}
	}

	return -1;
}

/*
 * Attach to and detach from USB device
 */
int cmmk_attach(struct cmmk *dev, int product, int layout)
{
	struct hid_device_info *list;

	list = hid_enumerate(CMMK_USB_VENDOR, product);

	dev->product = product;

	for (struct hid_device_info *dev_info = list; dev_info != NULL; dev_info = dev_info->next) {
		if (dev_info->interface_number != CMMK_USB_INTERFACE) {
			continue;
		}

		dev->dev = hid_open_path(dev_info->path);

		if (layout < 0) {
			if ((layout = cmmk_try_determine_layout(dev, product)) < 0) {
				hid_free_enumeration(list);
				cmmk_detach(dev);

				return CMMK_LAYOUT_DETECTION_FAILED;
			}
		}

		break;
	}
	
	hid_free_enumeration(list);

	if (dev->dev != NULL) {
		/*
		* Generate lookup map
		*/
		cmmk_force_layout(dev, layout);

		dev->multilayer_mode = 0;

		return CMMK_OK;
	} else {
		return 1;
	}
}

int cmmk_attach_path(struct cmmk *dev, char const *path, int product, int layout)
{
	dev->dev = hid_open_path(path);

	if (dev->dev != NULL) {
		if (layout < 0) {
			layout = cmmk_try_determine_layout(dev, product);
		}

		/*
		* Generate lookup map
		*/
		cmmk_force_layout(dev, layout);

		dev->multilayer_mode = 0;

		return CMMK_OK;
	} else {
		return 1;
	}
}

int cmmk_detach(struct cmmk *dev)
{
	hid_close(dev->dev);

	return CMMK_OK;
}

int cmmk_force_layout(struct cmmk *dev, int layout)
{
	int i;
	int j;

	keyboard_layout const *keyboard_layout;

	dev->layout = layout;

	memset(dev->rowmap, -1, sizeof(dev->rowmap));
	memset(dev->colmap, -1, sizeof(dev->colmap));

	keyboard_layout = keyboard_layouts[dev->layout];

	for (i = 0; i < CMMK_ROWS_MAX; ++i) {
		for (j = 0; j < CMMK_COLS_MAX; ++j) {
			int p = (*keyboard_layout)[i][j];

			if (p < 0) {
				continue;
			}

			dev->rowmap[p] = i;
			dev->colmap[p] = j;
		}
	}

	return CMMK_OK;
}

int cmmk_get_firmware_version(struct cmmk *dev, char *fw, size_t fwsiz)
{
	unsigned char data[65] = { 0x01, 0x02 };
	int r;

	if ((r = send_command(dev->dev, data, sizeof(data))) != 0) {
		return r;
	}

	/* Don't want to read past the response buffer */
	if (fwsiz > 60) {
		fwsiz = 60;
	}

	strncpy(fw, (char *)data + 4, fwsiz);

	return CMMK_OK;
}

enum cmmk_product_type cmmk_get_device_model(struct cmmk *dev)
{
	switch (dev->layout) {
	case CMMK_LAYOUT_US_S:
	case CMMK_LAYOUT_EU_S:
		return CMMK_PRODUCT_MASTERKEYS_PRO_S;

	case CMMK_LAYOUT_US_L:
	case CMMK_LAYOUT_EU_L:
		return CMMK_PRODUCT_MASTERKEYS_PRO_L;

	case CMMK_LAYOUT_US_MK750:
	case CMMK_LAYOUT_EU_MK750:
		return CMMK_PRODUCT_MASTERKEYS_MK750;
	}

	assert(0 && "unreachable");
}

enum cmmk_layout_type cmmk_get_device_layout(struct cmmk *dev)
{
	switch (dev->layout) {
	case CMMK_LAYOUT_US_S:
	case CMMK_LAYOUT_US_L:
	case CMMK_LAYOUT_US_MK750:
		return CMMK_LAYOUT_TYPE_ANSI;

	case CMMK_LAYOUT_EU_S:
	case CMMK_LAYOUT_EU_L:
	case CMMK_LAYOUT_EU_MK750:
		return CMMK_LAYOUT_TYPE_ISO;
	}

	assert(0 && "unreachable");
}

const char * cmmk_product_to_str(int product)
{
	switch ((enum cmmk_product) product) {
		case CMMK_USB_MASTERKEYS_PRO_S: return "Cooler Master Masterkeys Pro S";
		case CMMK_USB_MASTERKEYS_PRO_L: return "Cooler Master Masterkeys Pro L";
		case CMMK_USB_MASTERKEYS_PRO_L_WHITE: return "Cooler Master Masterkeys Pro L White";
		case CMMK_USB_MASTERKEYS_MK750: return "Cooler Master Masterkeys MK750";
		case CMMK_USB_MASTERKEYS_SK630: return "Cooler Master Masterkeys SK630";
		case CMMK_USB_MASTERKEYS_SK650: return "Cooler Master Masterkeys SK650";
	}

	return "unknown";
}

const char * cmmk_layout_to_str(int layout)
{
	switch ((enum cmmk_layout) layout) {
		case CMMK_LAYOUT_US_S:
		case CMMK_LAYOUT_US_L:
		case CMMK_LAYOUT_US_MK750:
		case CMMK_LAYOUT_US_SK630:
		case CMMK_LAYOUT_US_SK650:
			return "US";
		case CMMK_LAYOUT_EU_S:
		case CMMK_LAYOUT_EU_L:
		case CMMK_LAYOUT_EU_MK750:
		case CMMK_LAYOUT_EU_SK630:
		case CMMK_LAYOUT_EU_SK650:
			return "EU";

		case CMMK_LAYOUT_INVAL:
			return "invalid";
	}

	return "unknown";
}

/*
 * Enter and leave direct control mode. Any control commands outside of control
 * mode are ignored.
 */
int cmmk_set_control_mode(struct cmmk *dev, int mode)
{
	unsigned char data[64] = {0x41, mode};

	return send_command(dev->dev, data, sizeof(data));
}

int cmmk_set_active_profile(struct cmmk *dev, int prof)
{
	unsigned char setprof[64] = {0x51, 0x00, 0x00, 0x00, prof};

	return send_command(dev->dev, setprof, sizeof(setprof));
}

int cmmk_get_active_profile(struct cmmk *dev, int *prof)
{
	int r;

	unsigned char getprof[64] = {0x52, 0x00};

	if ((r = send_command(dev->dev, getprof, sizeof(getprof))) != 0)
		return r;

	*prof = getprof[4];

	return CMMK_OK;
}

int cmmk_save_active_profile(struct cmmk *dev)
{
	unsigned char saveprof[64] = {0x50, 0x55};

	return send_command(dev->dev, saveprof, sizeof(saveprof));
}


static int set_effect1(struct cmmk *dev, int eff)
{
	unsigned char data[64] = {0x51, 0x28, 0x00, 0x00, eff};

	return send_command(dev->dev, data, sizeof(data));
}


static int set_effect(
	struct cmmk *dev,
	int eff,
	int p1, int p2, int p3,
	struct rgb const *col1,
	struct rgb const *col2)
{
	unsigned char data[64] = {
		0x51, 0x2c, dev->multilayer_mode, 0x00, eff,  p1,   p2,   p3,
		0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	if (col1 != NULL) {
		data[10] = col1->R;
		data[11] = col1->G;
		data[12] = col1->B;
	}

	if (col2 != NULL) {
		data[13] = col2->R;
		data[14] = col2->G;
		data[15] = col2->B;
	}

	memset(data + 16, 0xff, 48);

	return send_command(dev->dev, data, sizeof(data));
}

static int get_effect(
	struct cmmk *dev,
	int eff,
	int *p1, int *p2, int *p3,
	struct rgb *col1,
	struct rgb *col2)
{
	int r;

	unsigned char data[64] = {
		0x52, 0x2c, dev->multilayer_mode, 0x00, eff
	};

	memset(data + 5, 0xff, 59);

	if ((r = send_command(dev->dev, data, sizeof(data))) != 0) {
		return r;
	}

	if (p1) {
		*p1 = data[5];
	}

	if (p2) {
		*p2 = data[6];
	}

	if (p3) {
		*p3 = data[7];
	}

	if (col1) {
		col1->R = data[10];
		col1->G = data[11];
		col1->B = data[12];
	}

	if (col2) {
		col2->R = data[13];
		col2->G = data[14];
		col2->B = data[15];
	}

	return CMMK_OK;
}

int cmmk_set_active_effect(struct cmmk *dev, enum cmmk_effect_id eff)
{
	if (eff < 0 || (eff > CMMK_EFFECT_CUSTOMIZED
			&& eff != CMMK_EFFECT_OFF
			&& eff != CMMK_EFFECT_MULTILAYER)) {
		return CMMK_INVAL;
	}

	return set_effect1(dev, eff);
}

int cmmk_get_active_effect(struct cmmk *dev, enum cmmk_effect_id *eff)
{
	unsigned char data[64] = {0x52, 0x28};
	int r;

	if ((r = send_command(dev->dev, data, sizeof(data))) != 0) {
		return r;
	}

	*eff = data[4];

	return CMMK_OK;
}

int cmmk_get_effect(struct cmmk *dev, enum cmmk_effect_id id, struct cmmk_generic_effect *eff)
{
	return get_effect(dev, id, &eff->p1, &eff->p2, &eff->p3, &eff->color1, &eff->color2);
}
int cmmk_set_effect(struct cmmk *dev, enum cmmk_effect_id id, struct cmmk_generic_effect const *eff)
{
	return set_effect(dev, id, eff->p1, eff->p2, eff->p3, &eff->color1, &eff->color2);
}


int cmmk_get_enabled_effects(
	struct cmmk *dev,
	enum cmmk_effect_id *effs,
	size_t siz,
	size_t *n)
{
	unsigned char data[64] = {0x52, 0x29};

	size_t i;
	size_t j = 0;

	int r;

	if ((r = send_command(dev->dev, data, sizeof(data))) != 0) {
		return r;
	}

	for (i = 4; data[i] != 0xff && j < siz; ++i) {
		effs[j++] = data[i];
	}

	*n = j;
	return CMMK_OK;
}

int cmmk_set_enabled_effects(
	struct cmmk *dev,
	enum cmmk_effect_id const *effs,
	size_t n)
{
	unsigned char data[64] = {0x51, 0x29};

	size_t i;

	for (i = 0; i < n; ++i) {
		data[4 + i] = effs[i];
	}

	while (i < 18) {
		data[4 + i] = 0xff;

		++i;
	}

	return send_command(dev->dev, data, sizeof(data));;
}

int cmmk_get_effect_fully_lit(struct cmmk *dev, struct cmmk_effect_fully_lit *eff)
{
	return get_effect(dev, CMMK_EFFECT_FULLY_LIT, NULL, NULL, NULL, &eff->color, NULL);
}

int cmmk_set_effect_fully_lit(struct cmmk *dev, struct cmmk_effect_fully_lit const *eff)
{
	return set_effect(dev, CMMK_EFFECT_FULLY_LIT, 0x00, 0x00, 0xff, &eff->color, NULL);
}


int cmmk_get_effect_breathe(struct cmmk *dev, struct cmmk_effect_breathe *eff)
{
	return get_effect(dev, CMMK_EFFECT_BREATHE, &eff->speed, NULL, NULL, &eff->color, NULL);
}

int cmmk_set_effect_breathe(struct cmmk *dev, struct cmmk_effect_breathe const *eff)
{
	return set_effect(dev, CMMK_EFFECT_BREATHE, eff->speed, 0x00, 0xff, &eff->color, NULL);
}


int cmmk_get_effect_cycle(struct cmmk *dev, struct cmmk_effect_cycle *eff)
{
	return get_effect(dev, CMMK_EFFECT_CYCLE, &eff->speed, NULL, NULL, NULL, NULL);
}

int cmmk_set_effect_cycle(struct cmmk *dev, struct cmmk_effect_cycle const *eff)
{
	return set_effect(dev, CMMK_EFFECT_CYCLE, eff->speed, 0x00, 0xff, NULL, NULL);
}


int cmmk_get_effect_single(struct cmmk *dev, struct cmmk_effect_single *eff)
{
	return get_effect(dev, CMMK_EFFECT_SINGLE, &eff->speed, NULL, NULL, &eff->active, &eff->rest);
}

int cmmk_set_effect_single(struct cmmk *dev, struct cmmk_effect_single const *eff)
{
	return set_effect(dev, CMMK_EFFECT_SINGLE, eff->speed, 0x00, 0xff, &eff->active, &eff->rest);
}


int cmmk_get_effect_wave(struct cmmk *dev, struct cmmk_effect_wave *eff)
{
	int r;
	int p2;

	if ((r = get_effect(dev, CMMK_EFFECT_WAVE, &eff->speed, &p2, NULL, &eff->start, NULL)) != 0) {
		return r;
	}

	eff->direction = p2;
	return CMMK_OK;
}

int cmmk_set_effect_wave(struct cmmk *dev, struct cmmk_effect_wave const *eff)
{
	return set_effect(dev, CMMK_EFFECT_WAVE, eff->speed, eff->direction, 0xff, &eff->start, NULL);
}


int cmmk_get_effect_ripple(struct cmmk *dev, struct cmmk_effect_ripple *eff)
{
	int r;
	int p2;

	if ((r = get_effect(dev, CMMK_EFFECT_RIPPLE, &eff->speed, &p2, NULL, &eff->active, &eff->rest)) != 0) {
		return r;
	}

	eff->ripple_type = (p2 == 0x80) ? CMMK_RIPPLE_RANDOM_COLOR : CMMK_RIPPLE_GIVEN_COLOR;

	return CMMK_OK;
}

int cmmk_set_effect_ripple(struct cmmk *dev, struct cmmk_effect_ripple const *eff)
{
	return set_effect(dev, CMMK_EFFECT_RIPPLE, eff->speed, eff->ripple_type ? 0x80 : 0x00, 0xff, &eff->active, &eff->rest);
}


int cmmk_get_effect_cross(struct cmmk *dev, struct cmmk_effect_cross *eff)
{
	return get_effect(dev, CMMK_EFFECT_CROSS, &eff->speed, NULL, NULL, &eff->active, &eff->rest);
}

int cmmk_set_effect_cross(struct cmmk *dev, struct cmmk_effect_cross const *eff)
{
	return set_effect(dev, CMMK_EFFECT_CROSS, eff->speed, 0x00, 0xff, &eff->active, &eff->rest);
}


int cmmk_get_effect_raindrops(struct cmmk *dev, struct cmmk_effect_raindrops *eff)
{
	return get_effect(dev, CMMK_EFFECT_RAINDROPS, &eff->speed, NULL, &eff->interval, &eff->active, &eff->rest);
}

int cmmk_set_effect_raindrops(struct cmmk *dev, struct cmmk_effect_raindrops const *eff)
{
	return set_effect(dev, CMMK_EFFECT_RAINDROPS, eff->speed, 0x00, eff->interval, &eff->active, &eff->rest);
}


int cmmk_get_effect_stars(struct cmmk *dev, struct cmmk_effect_stars *eff)
{
	return get_effect(dev, CMMK_EFFECT_STARS, &eff->speed, NULL, &eff->interval, &eff->active, &eff->rest);
}

int cmmk_set_effect_stars(struct cmmk *dev, struct cmmk_effect_stars const *eff)
{
	return set_effect(dev, CMMK_EFFECT_STARS, eff->speed, 0x00, eff->interval, &eff->active, &eff->rest);
}

int cmmk_get_effect_snake(struct cmmk *dev, struct cmmk_effect_snake *eff)
{
	return get_effect(dev, CMMK_EFFECT_SNAKE, &eff->speed, NULL, NULL, NULL, NULL);
}

int cmmk_set_effect_snake(struct cmmk *dev, struct cmmk_effect_snake const *eff)
{
	return set_effect(dev, CMMK_EFFECT_SNAKE, eff->speed, 0x00, 0xff, NULL, NULL);
}

int cmmk_set_customized_leds(struct cmmk *dev, struct cmmk_color_matrix const *colmap)
{
	unsigned char data[64] = {0x51, 0xa8};

	int i;
	int j;

#ifdef _MSC_VER
	struct rgb linear[CMMK_KEYLIST_SIZE] = { 0 };
#else
	struct rgb linear[CMMK_KEYLIST_SIZE] = {};
#endif
	struct rgb *nextcol = linear;

	transpose_reverse(dev, colmap, linear);

	for (i = 0; i < 8; ++i) {
		data[2] = i*2;

		for (j = 0; j < 16; ++j) {
			int const offset = 4 + (j * 3);

			data[offset] = nextcol->R;
			data[offset + 1] = nextcol->G;
			data[offset + 2] = nextcol->B;

			++nextcol;
		}

		send_command(dev->dev, data, sizeof(data));
	}

	return CMMK_OK;
}

int cmmk_get_customized_leds(struct cmmk *dev, struct cmmk_color_matrix *colmap)
{
#ifdef _MSC_VER
	struct rgb linear[CMMK_KEYLIST_SIZE] = { 0 };
#else
	struct rgb linear[CMMK_KEYLIST_SIZE] = {};
#endif

	unsigned char data[64] = {0x52, 0xa8};

	int i;
	int j;

	struct rgb *ptr = linear;

	for (i = 0; i < 8; ++i) {
		data[2] = i * 2;

		send_command(dev->dev, data, sizeof(data));

		for (j = 0; j < 16; ++j) {
			int const offset = 4 + (j * 3);

			ptr->R = data[offset];
			ptr->G = data[offset + 1];
			ptr->B = data[offset + 2];

			++ptr;
		}
	}

	transpose(dev, linear, colmap);

	return CMMK_OK;
}

int cmmk_switch_multilayer(struct cmmk *dev, int active)
{
	dev->multilayer_mode = active > 0;

	return CMMK_OK;
}

int cmmk_get_multilayer_map(struct cmmk *dev, struct cmmk_effect_matrix *effmap)
{
	int r;

	unsigned char data[64] = {0x52, 0xa0, 0x01, 0x00};
	uint8_t linear[CMMK_KEYLIST_SIZE];

	/* Call 1 */
	data[4] = 0x00;
	data[5] = 0x07;

	if ((r = send_command(dev->dev, data, sizeof(data))) != 0) {
		return r;
	}

	memcpy(linear, data + 8, 56);

	/* Call 2 */
	data[4] = 0x07;
	data[5] = 0x07;

	if ((r = send_command(dev->dev, data, sizeof(data))) != 0) {
		return r;
	}

	memcpy(linear + 56, data + 8, 56);

	/* Call 3 */
	data[4] = 0x0e;
	data[5] = 0x01;

	if ((r = send_command(dev->dev, data, sizeof(data))) != 0) {
		return r;
	}

	memcpy(linear + 112, data + 8, 8);

	transpose_effects(dev, linear, effmap);

	return CMMK_OK;
}

int cmmk_set_multilayer_map(struct cmmk *dev, struct cmmk_effect_matrix const *effmap)
{
	int r;

	unsigned char data[64] = {0x51, 0xa0, 0x01, 0x00};
	uint8_t linear[CMMK_KEYLIST_SIZE] = {0};

	transpose_effects_reverse(dev, effmap, linear);

	/* Call 1 */
	data[4] = 0x00;
	data[5] = 0x07;

	memcpy(data + 8, linear, 56);

	if ((r = send_command(dev->dev, data, sizeof(data))) != 0) {
		return r;
	}

	/* Call 2 */
	data[4] = 0x07;
	data[5] = 0x07;

	memcpy(data + 8, linear + 56, 56);

	if ((r = send_command(dev->dev, data, sizeof(data))) != 0) {
		return r;
	}

	/* Call 3 */
	data[4] = 0x0e;
	data[5] = 0x01;

	memcpy(data + 8, linear + 112, 8);

	if ((r = send_command(dev->dev, data, sizeof(data))) != 0) {
		return r;
	}

	return CMMK_OK;
}

int cmmk_lookup_key_id(struct cmmk *dev, int row, int col)
{
	keyboard_layout const *layout = keyboard_layouts[dev->layout];

	return (*layout)[row][col];
}

/*
 * Set the single key `key' to the given color.
 */
int cmmk_set_single_key_by_id(struct cmmk *dev, int key, struct rgb const *color)
{
	unsigned char data[64] = {0xc0, 0x01, 0x01, 0x00, key, color->R, color->G, color->B};

	return send_command(dev->dev, data, sizeof(data));
}

/*
 * Set the single key in row `row` and column `col` to the given color.
 */
int cmmk_set_single_key(struct cmmk *dev, int row, int col, struct rgb const *color)
{
	int key = cmmk_lookup_key_id(dev, row, col);

	return cmmk_set_single_key_by_id(dev, key, color);
}


/*
 * Set the entire keyboard to the given color.
 */
int cmmk_set_all_single(struct cmmk *dev, struct rgb const *col)
{
	unsigned char data[64] = {0xc0, 0x00, 0x00, 0x00, col->R, col->G, col->B};

	return send_command(dev->dev, data, sizeof(data));
}


/*
 * Set the entire keyboard in one step from the given map.
 *
 * colmap *must* be at least CMMK_KEYLIST_SIZE entries long.
 * Otherwise, segmentation faults ensue.
 *
 * Keys in the map are indized by their individual mappings, so
 * colmap[K_ESC] will address the ESC key, much like
 * set_single_key(..., K_ESC, ...) will.
 */
int cmmk_set_leds(struct cmmk *dev, struct cmmk_color_matrix const *colmap)
{
	unsigned char data[64];

	int i;
	int j;

#ifdef _MSC_VER
	struct rgb linear[CMMK_KEYLIST_SIZE] = { 0 };
#else
	struct rgb linear[CMMK_KEYLIST_SIZE] = {};
#endif
	struct rgb *nextcol = linear;

	transpose_reverse(dev, colmap, linear);

	for (i = 0; i < 8; ++i) {
		data[0] = 0xc0;
		data[1] = 0x02;
		data[2] = i*2;
		data[3] = 0x00;

		for (j = 0; j < 16; ++j) {
			int const offset = 4 + (j * 3);

			data[offset] = nextcol->R;
			data[offset + 1] = nextcol->G;
			data[offset + 2] = nextcol->B;

			++nextcol;
		}

		send_command(dev->dev, data, sizeof(data));
	}

	return CMMK_OK;
}


/*
 * Unpublished functions (debug, survey, ...)
 */
int cmmk_send_anything(struct cmmk *dev, unsigned char *data, size_t data_siz)
{
	return send_command(dev->dev, data, data_siz);
}
