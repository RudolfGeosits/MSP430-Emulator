#include "packet_queue.h"

void init_packet_queue (Emulator *emu) 
{
  Server *serv = emu->debugger->server;
  
  // init elements (100 of them)
  serv->pending_packets = (Packet *) malloc(sizeof(Packet) * 100);

  // Empty Queue
  serv->head = serv->tail = serv->pending_packets;
}

bool packet_queue_empty (Emulator *emu) 
{
  Server *serv = emu->debugger->server;

  // Empty Queue when head = tail
  if (serv->tail == serv->pending_packets) {
    return true;
  }

  return false;
}

void enqueue (Emulator *emu, void *item, size_t size)
{
  Server *serv = emu->debugger->server;
  Packet *p = serv->tail;

  p->message = item;
  p->length = size;

  ++serv->tail;
}

Packet *dequeue (Emulator *emu) 
{
  Server *serv = emu->debugger->server;
  Packet *p = serv->tail;
  
  --serv->tail;
  return p;
}
