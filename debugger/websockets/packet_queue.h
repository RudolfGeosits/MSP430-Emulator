#ifndef _PACKET_QUEUE_H
#define _PACKET_QUEUE_H

#include "../../main.h"

struct Packet {
  void *message;
  size_t length;
  uint8_t opcode;

  Packet *next;
};

void init_packet_queue (Emulator *emu);
bool packet_queue_empty (Emulator *emu);

void packet_enqueue (Emulator *emu, void *info, size_t len, uint8_t opcode);
Packet packet_dequeue (Emulator *emu);
void print_packet_queue (Emulator *emu);

#endif
