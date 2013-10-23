/* -*- mode: c; c-file-style: "linux" -*-
 *
 *  Bit-banging interface for a cheap Chinese sex toy.
 *
 *  Copyright 2013 Koodilehto Osk
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ftdi.h"

#define DURATION 50000 /* Command duration in microseconds */

/**
 * Validates that given expression or function returns zero.
 * Otherwise, it returns using that non-zero value.
 *
 * The return type of the function you are using this macro must be
 * int.
 */
#define CHECK(expression)		\
	{				\
		int x = (expression);	\
		if (x != 0) return x;	\
	}

/**
 * Pulls given pins LOW. Other pins are floating (pull-up takes them
 * HIGH).
 *
 * In your context there must be ftdi_context in variable `c` and the
 * return value of the function must be bool (false meaning failure).
 */
#define FTDI_PULLDOWN(pins) CHECK(ftdi_set_bitmode(c, pins, BITMODE_BITBANG))

struct ftdi_context* ftdi_context_new(void)
{
	struct ftdi_context *p = malloc(sizeof(struct ftdi_context));
	memset(p,0,sizeof(struct ftdi_context));
	return p;
}

int ftdi_bitbang_init(struct ftdi_context *c, const char * const serial)
{
	// Open and initialize
	CHECK(ftdi_init(c));
	CHECK(ftdi_usb_open_desc(c, 0x0403, 0x6001, NULL, serial));

	// Enable bitbang mode. Put all pins in input mode.
	FTDI_PULLDOWN(0);

	return BANG_OK;
}

int ftdi_pulldown(struct ftdi_context *c, const uint8_t pins)
{
	// Pull down given pins, wait and release and wait again
	FTDI_PULLDOWN(pins);
	CHECK(usleep(DURATION));
	FTDI_PULLDOWN(0);
	CHECK(usleep(DURATION));
	return BANG_OK;
}
