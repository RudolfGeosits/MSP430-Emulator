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
#include "../../libs/libwebsockets-2.0.2/libwebsockets.h"

extern int callback_http (
                   struct lws *wsi,
                   enum lws_callback_reasons reason,
                   void *user, void *in, size_t len);

extern int callback_emu (
		  struct lws *wsi,
                  enum lws_callback_reasons reason,
                  void *user, void *in, size_t len);

static struct lws_protocols protocols [] = {
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
