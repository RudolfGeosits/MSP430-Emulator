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

int callback_http (struct libwebsocket_context *this,
                   struct libwebsocket *wsi,
                   enum libwebsocket_callback_reasons reason,
                   void *user, void *in, size_t len);

int callback_emu (struct libwebsocket_context *this,
                  struct libwebsocket *wsi,
                  enum libwebsocket_callback_reasons reason,
                  void *user, void *in, size_t len);

static struct libwebsocket_protocols protocols[]= {
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
    0
  },

  {
    NULL, NULL, 0
  }
};

#endif
