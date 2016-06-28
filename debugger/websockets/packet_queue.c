#include "packet_queue.h"

void init_packet_queue (Emulator *emu) 
{
  Server *s = emu->debugger->server;

  s->pending_packets_head = NULL;
  s->pending_packets_tail = NULL;
  s->packets_queued = 0;
  s->spin_lock = false;
}

void destroy_packet_queue (Emulator *emu)
{

}

bool packet_queue_full (Emulator *emu) 
{
  Server *s = emu->debugger->server;

  return false;
}

bool packet_queue_empty (Emulator *emu) 
{
  Server *s = emu->debugger->server;

  if (s->packets_queued > 0 && s->pending_packets_head != NULL) {
    return false;
  }
  else {
    return true;
  }
}

void packet_enqueue (Emulator *emu, void *item, size_t size, 
		     uint8_t opcode)
{  
  Server *s = emu->debugger->server;
  Packet *head, *tail, *cur;
  void *heap_item;
  
  while (s->spin_lock);
  s->spin_lock = true;

  if (s->pending_packets_head == NULL) {
    head = tail = s->pending_packets_head = s->pending_packets_tail = 
      (Packet *) calloc(1, sizeof(Packet));        

    // copy item data onto the heap so it won't go out of scope
    heap_item = calloc(1, size);
    memcpy(heap_item, (const void *)item, size);
    
    head->message = heap_item;
    head->length = size;
    head->opcode = opcode;
    head->next = NULL;
  }
  else {
    //printf("IN ELSE OF PACKET_ENQUEUE\n");
    cur = s->pending_packets_head;
    while (cur->next != NULL) {
      cur = cur->next;
    }
  
    cur->next = tail = (Packet *) calloc(1, sizeof(Packet));

    // copy item data onto the heap so it won't go out of scope
    heap_item = calloc(1, size);
    memcpy(heap_item, (const void *)item, size);

    cur->next->message = heap_item;
    cur->next->length = size;
    cur->next->opcode = opcode;
    cur->next->next = NULL;
  }

  s->packets_queued++;
  s->spin_lock = false;
  return;
}

void print_packet_queue (Emulator *emu) 
{
  Server *s = emu->debugger->server;
  Packet *cur = s->pending_packets_head;
  uint32_t packet = 0;

  printf("packets: %u\n", s->packets_queued);

  while (cur != NULL) {
    printf("I have %s, len %u, PACKET #%u\n", 
	   (char *)cur->message, (unsigned int)cur->length, packet++);
    
    cur = cur->next;
  }
}

Packet packet_dequeue (Emulator *emu) 
{
  Server *s = emu->debugger->server;
  Packet *head, *tail, *saved, ret;
  
  while (s->spin_lock);
  s->spin_lock = true;

  //printf("Count:%d\n", s->packets_queued);
  //printf("in packet_dequeue(), head = s->pending_packets_head = %p\n",
  //s->pending_packets_head);
  head = s->pending_packets_head;
  //printf("head = %p\n", head);

  if ( packet_queue_empty(emu) ) {
    printf("EMPTY QUEUE...\n");
    return ret;
  }
  
  s->pending_packets_head = head->next;
  
  //if (s->packets_queued == 2) {
    //sleep(2);
  //}

  ret.message = head->message;
  ret.length  = head->length;
  ret.opcode  = head->opcode;
  ret.next    = NULL;
  
  //free(head->message);
  free(head);
  s->packets_queued--;

  s->spin_lock = false;
  return ret;
}
