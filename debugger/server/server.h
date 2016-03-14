/*
  MSP430 Emulator
  Copyright (C) 2016 Rudolf Geosits (rgeosits@live.esu.edu)  
                                                                      
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
                                                                   
  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.          
                                                       
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses
*/

#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include <libwebsockets.h>

#include "../../devices/memory/memspace.h"
#include "../../devices/cpu/registers.h"
#include "../../devices/utilities.h"
#include "../../devices/peripherals/port1.h"
#include "../../devices/peripherals/usci.h"

enum {CONTROL, STDOUT, SERIAL};

int callback_http (struct libwebsocket_context *this,
                          struct libwebsocket *wsi,
                          enum libwebsocket_callback_reasons reason,
                          void *user, void *in, size_t len);

int callback_emu (struct libwebsocket_context *this,
                         struct libwebsocket *wsi,
                         enum libwebsocket_callback_reasons reason,
                         void *user, void *in, size_t len);

static struct libwebsocket_protocols protocols[] = {
  /* first protocol must always be HTTP handler */
  {
    "http-only",   // name
    callback_http, // callback
    0              // per_session_data_size
  },

  {
    "emu-protocol",
    callback_emu,
    0,
    1024 * 4, // rx buffer size
    0,        // id
    0         // user context data
  },

  {
    NULL, NULL, 0   /* End of list */
  }
};

void web_send (char *buf, uint8_t type);
void *web_server (void *ctxt);

#endif
