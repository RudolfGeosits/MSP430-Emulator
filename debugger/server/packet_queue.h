#ifndef _PACKET_QUEUE_H
#define _PACKET_QUEUE_H

//#include "stdbool.h"
#include "../../main.h"

typedef struct Packet {
  void *message;
  size_t length;
} Packet;

void init_packet_queue (Emulator *emu);
bool packet_queue_empty (Emulator *emu);

#endif
