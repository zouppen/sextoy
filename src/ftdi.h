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

#include <ftdi.h>

#define BANG_OK 0

/**
 * Allocates storage for FTDI context. This is just a combined malloc
 * and memset.
 */
struct ftdi_context* ftdi_context_new(void);

/**
 * Initializes FTDI device and sets device ready for button control.
 *
 * Preallocated FTDI context must be passed to this function. Serial
 * number may be obtained with `usb-devices` command. An example of
 * serial number: FTGDL1AX
 */
int ftdi_bitbang_init(struct ftdi_context *c, const char *serial);

/**
 * Pulls down given pins for constant time and then release all pins
 * to floating state.
 */
int ftdi_pulldown(struct ftdi_context *c, const uint8_t pins);

/*
 * Use ftdi_deinit() or ftdi_free() to uninitialize given device.
 */
