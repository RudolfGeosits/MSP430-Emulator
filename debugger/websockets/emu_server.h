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

#ifndef _EMU_SERVER_H_
#define _EMU_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include "../../libs/libwebsockets-2.0.2/libwebsockets.h"

#include "../../main.h"
#include "packet_queue.h"

enum {
  CONTROL_PACKET_OPCODE  = 0x00,
  CONSOLE_PACKET_OPCODE  = 0x01,
  SERIAL_PACKET_OPCODE   = 0x02,
};

enum {
  P1_0_ON_PACKET  = 0x00,
  P1_0_OFF_PACKET = 0x01,

  P1_1_ON_PACKET  = 0x02,
  P1_1_OFF_PACKET = 0x03,

  P1_2_ON_PACKET  = 0x04,
  P1_2_OFF_PACKET = 0x05,

  P1_3_ON_PACKET  = 0x06,
  P1_3_OFF_PACKET = 0x07,

  P1_4_ON_PACKET  = 0x08,
  P1_4_OFF_PACKET = 0x09,

  P1_5_ON_PACKET  = 0x0A,
  P1_5_OFF_PACKET = 0x0B,

  P1_6_ON_PACKET  = 0x0C,
  P1_6_OFF_PACKET = 0x0D,

  P1_7_ON_PACKET  = 0x0E,
  P1_7_OFF_PACKET = 0x0F,

  UPDATE_REG_R0_PACKET = 0x10,
  UPDATE_REG_R1_PACKET = 0x11,
  UPDATE_REG_R2_PACKET = 0x12,
  UPDATE_REG_R3_PACKET = 0x13,
  UPDATE_REG_R4_PACKET = 0x14,
  UPDATE_REG_R5_PACKET = 0x15,
  UPDATE_REG_R6_PACKET = 0x16,
  UPDATE_REG_R7_PACKET = 0x17,
  UPDATE_REG_R8_PACKET = 0x18,
  UPDATE_REG_R9_PACKET = 0x19,
  UPDATE_REG_R10_PACKET = 0x1A,
  UPDATE_REG_R11_PACKET = 0x1B,
  UPDATE_REG_R12_PACKET = 0x1C,
  UPDATE_REG_R13_PACKET = 0x1D,
  UPDATE_REG_R14_PACKET = 0x1E,
  UPDATE_REG_R15_PACKET = 0x1F,
  UPDATE_ALL_REGS_PACKET = 0x20,

  SERVO_MOTOR = 0x21,
};

int callback_http (
                   struct lws *wsi,
                   enum lws_callback_reasons reason,
                   void *user, void *in, size_t len);

int callback_emu (
		  struct lws *wsi,
		  //enum lws_callback_reasons reason,
		  enum lws_callback_reasons reason,
		  void *user, void *in, size_t len);

//static struct libwebsocket_protocols protocols[] = {
static struct lws_protocols protocols [] = {
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

struct Server {
  // Pending Packets Queue  
  Packet *pending_packets_head;
  Packet *pending_packets_tail;
  uint32_t packets_queued;

  // Other  
  bool spin_lock;
};

void print_console (Emulator *emu, char *buf);
void print_serial (Emulator *emu, char *buf);
void send_control (Emulator *emu, uint8_t opcode, 
		   void *data, size_t size);
void *web_server (void *ctxt);

#endif
