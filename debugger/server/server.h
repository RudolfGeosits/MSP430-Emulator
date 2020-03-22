/*
  MSP430 Emulator
  Copyright (C) 2020 Rudolf Geosits (rgeosits@live.esu.edu)

  "MSP430 Emulator" is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  "MSP430 Emulator" is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _SERVER_H_
#define _SERVER_H_

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <libwebsockets.h>

extern int callback_http (
                   struct lws *wsi,
                   enum lws_callback_reasons reason,
                   void *user, void *in, size_t len);

extern int callback_emu (
		  struct lws *wsi,
                  enum lws_callback_reasons reason,
                  void *user, void *in, size_t len);

static struct lws_protocols protocols [] = 
{
    {
        "http-only",
        callback_http,
        0
    },

    {
        "emu-protocol",
        callback_emu,
        0,
        1024 * 4,
        0,
    },

    {
        NULL, NULL, 0, 0
    }
};

#endif
