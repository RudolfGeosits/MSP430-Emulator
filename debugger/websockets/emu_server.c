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

#include "emu_server.h"

#define TXIFG 0x02
#define RXIFG 0x01
#define HIGH  1
#define LOW   0
#define OUT   1
#define IN    0

Emulator *emu = NULL;

uint8_t *data;
int lent;

void *thrd (void *ctxt) 
{
  Usci *usci = (Usci *)ctxt;
  int i, j = 0;
  int len = lent;
  uint8_t *bytes = data;

  //printf("len is %d\nstr is %s\n", len, bytes);

  while (true) {
    usleep(333);
    while (*usci->IFG2 & RXIFG);
    uint8_t thing = *(bytes);

    if (thing == '\n') {                  
      thing = '\r';
    }                          
    
    //printf("Got 0x%02X '%c'\n", thing, thing);
    
    if (*bytes == '\\') {
      ++bytes;
      if (*bytes == 'h' || *bytes == 'H') {
	++bytes;
	
	char buf[3] = {*bytes, *(bytes+1), 0};

	//printf("%s - %s\n", buf, bytes);
	thing = (uint8_t) strtoul(buf, NULL, 16);

	++bytes;
      }  
    }	  

    *usci->UCA0RXBUF = thing;    
    *usci->IFG2 |= RXIFG;
	    
    //printf("\n0x%04X in UCA0RXBUF\n", (uint8_t)*usci->UCA0RXBUF);

    //puts("waiting..");
    while (*usci->IFG2 & RXIFG);
    //puts("done");
    //*usci->IFG2 |= RXIFG;
    
    if (*usci->UCA0RXBUF == '\r') break;
    ++bytes;
  }

  return NULL;
}

int callback_emu (
		  struct lws *wsi,
		  //enum lws_callback_reasons reason,
		  enum lws_callback_reasons reason,
		  void *user, void *in, size_t len)
{
  Cpu *cpu = emu->cpu;
  Port_1 *p1 = emu->cpu->p1;
  Debugger *deb = emu->debugger;

  switch (reason) {
    case LWS_CALLBACK_ESTABLISHED: {
      puts("connection established");
      
      // Flip ready flag for the emulator to begin
      deb->web_server_ready = true;

      // get the ball rolling
      //libwebsocket_callback_on_writable(this, wsi);
      lws_callback_on_writable(wsi);
      
      break;
    }
      
    case LWS_CALLBACK_SERVER_WRITEABLE: {
      if ( !packet_queue_empty(emu) ) {
	  Packet p = packet_dequeue(emu);
	  
	  void *msg = p.message;
	  size_t msg_len = p.length;
	  uint8_t op = p.opcode;
	
	  //printf("[%s] of len %u, opcode: %u\n\n",
	  //(char *)msg, (unsigned int)msg_len, op);
	
	  // Leave room for websock header/trailer & opcode
	  size_t pack_len = msg_len + sizeof(op)
	    + LWS_SEND_BUFFER_PRE_PADDING 
	    + LWS_SEND_BUFFER_POST_PADDING;
	
	  void *packet = malloc(pack_len);
	
	  // Zero out our packet
	  memset( (packet + LWS_SEND_BUFFER_PRE_PADDING), 0, 
		  msg_len + sizeof(op) );

	  // Place opcode into packet
	  *((uint8_t *)(packet + LWS_SEND_BUFFER_PRE_PADDING)) = op;
	
	  // Place message into packet
	  memcpy( (packet + LWS_SEND_BUFFER_PRE_PADDING + sizeof(op)),
		  (const void *)p.message, msg_len);

	  /*
	  int i;
	  for (i = 0;i < pack_len;i++){
	    printf( "%02X (%c) | ", *((uint8_t *)(packet+i)),
		    *((char *)(packet+i)) );
	  }
	  puts("\n");
	  */

	  lws_write(wsi, packet+LWS_SEND_BUFFER_PRE_PADDING, 
			     msg_len + sizeof(op), 
			     LWS_WRITE_BINARY);
	  
	  free(p.message);
	  free(packet);
      }

      static bool p1_0_on = false;
      static bool p1_1_on = false;
      static bool p1_2_on = false;
      static bool p1_3_on = false;
      static bool p1_4_on = false;
      static bool p1_5_on = false;
      static bool p1_6_on = false;
      static bool p1_7_on = false;
      
      // P1.0 ON/OFF
      if (p1->DIR_0 == OUT) {
	if (p1->OUT_0 == HIGH) {
	  if (p1_0_on == false) {
	    send_control(emu, P1_0_ON_PACKET, NULL, 0);
	    p1_0_on = true;
	  }
	}
	else if (p1->OUT_0 == LOW) {
	  if (p1_0_on == true) {
	    send_control(emu, P1_0_OFF_PACKET, NULL, 0);
	    p1_0_on = false;
	  }
	}
      }

      // P1.1 ON/OFF
      if (p1->DIR_1 == OUT) {
	if (p1->OUT_1 == HIGH) {	  
	  if (p1_1_on == false) {
	    send_control(emu, P1_1_ON_PACKET, NULL, 0);
	    p1_1_on = true;
	  }
	}
	else if (p1->OUT_1 == LOW) {
	  if (p1_1_on == true) {
	    send_control(emu, P1_1_OFF_PACKET, NULL, 0);
	    p1_1_on = false;
	  }
	}
      }

      // P1.2 ON/OFF
      if (p1->DIR_2 == OUT) {
	if (p1->OUT_2 == HIGH) {	  
	  if (p1_2_on == false) {
	    send_control(emu, P1_2_ON_PACKET, NULL, 0);
	    p1_2_on = true;
	  }
	}
	else if (p1->OUT_2 == LOW) {
	  if (p1_2_on == true) {
	    send_control(emu, P1_2_OFF_PACKET, NULL, 0);
	    p1_2_on = false;
	  }
	}
      }

      // P1.3 ON/OFF
      if (p1->DIR_3 == OUT) {
	if (p1->OUT_3 == HIGH) {	  
	  if (p1_3_on == false) {
	    send_control(emu, P1_3_ON_PACKET, NULL, 0);
	    p1_3_on = true;
	  }
	}
	else if (p1->OUT_3 == LOW) {
	  if (p1_3_on == true) {
	    send_control(emu, P1_3_OFF_PACKET, NULL, 0);
	    p1_3_on = false;
	  }
	}
      }

      // P1.4 ON/OFF
      if (p1->DIR_4 == OUT) {
	if (p1->OUT_4 == HIGH) {	  
	  if (p1_4_on == false) {
	    send_control(emu, P1_4_ON_PACKET, NULL, 0);
	    p1_4_on = true;
	  }
	}
	else if (p1->OUT_4 == LOW) {
	  if (p1_4_on == true) {
	    send_control(emu, P1_4_OFF_PACKET, NULL, 0);
	    p1_4_on = false;
	  }
	}
      }

      // P1.5 ON/OFF
      if (p1->DIR_5 == OUT) {
	if (p1->OUT_5 == HIGH) {	  
	  if (p1_5_on == false) {
	    send_control(emu, P1_5_ON_PACKET, NULL, 0);
	    p1_5_on = true;
	  }
	}
	else if (p1->OUT_5 == LOW) {
	  if (p1_5_on == true) {
	    send_control(emu, P1_5_OFF_PACKET, NULL, 0);
	    p1_5_on = false;
	  }
	}
      }
      
      // P1.6 ON/OFF
      if (p1->DIR_6 == OUT) {
	if (p1->OUT_6 == HIGH) {	  
	  if (p1_6_on == false) {
	    send_control(emu, P1_6_ON_PACKET, NULL, 0);
	    p1_6_on = true;
	  }
	}
	else if (p1->OUT_6 == LOW) {
	  if (p1_6_on == true) {
	    send_control(emu, P1_6_OFF_PACKET, NULL, 0);
	    p1_6_on = false;
	  }
	}
      }

      // P1.7 ON/OFF
      if (p1->DIR_7 == OUT) {
	if (p1->OUT_7 == HIGH) {	  
	  if (p1_7_on == false) {
	    send_control(emu, P1_7_ON_PACKET, NULL, 0);
	    p1_7_on = true;
	  }
	}
	else if (p1->OUT_7 == LOW) {
	  if (p1_7_on == true) {
	    send_control(emu, P1_7_OFF_PACKET, NULL, 0);
	    p1_7_on = false;
	  }
	}
      }      

      lws_callback_on_writable( wsi);
      break;
    }

    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR: {
      puts("Connection Error");
      break;
    }

    case LWS_CALLBACK_CLOSED: {
      puts("Connection Closed");
      exit(0);
      break;
    }

    case LWS_CALLBACK_CLIENT_WRITEABLE: {
      puts("cli writable");
      break;
    }

    case LWS_CALLBACK_RECEIVE: {
      static FILE *fp = NULL;
      static bool upload_in_progress = false;
      static uint32_t uploaded_bytes, file_size;

      char *buf = (char *)in;      
      
      if (upload_in_progress) { // Continue transaction of upload
	int i;
	for (i = 0;i < len;i++){	     
	  fwrite(&buf[i], 1, 1, fp);
	  uploaded_bytes++;

	  if (uploaded_bytes >= file_size) {
	    puts("met bytes");
	    fclose(fp);
	    system("msp430-objcopy -O binary tmp.elf tmp.bin");

	    deb->web_firmware_uploaded = true;
	    upload_in_progress = false;
	    return 0;
	  }
	}
	
	return 0;
      }
      
      unsigned char opcode = buf[0];
      printf("opcode: %02X\n", *(uint8_t*)in);
      
      switch (opcode) {
        case 0x00: { // Upload File
	   uint8_t byte1 = *((uint8_t *)(in+1));
	   uint8_t byte2 = *((uint8_t *)(in+2));
	   uint8_t byte3 = *((uint8_t *)(in+3));
	   uint8_t byte4 = *((uint8_t *)(in+4));

	   file_size = 0;
	   file_size = byte1; file_size <<= 3*8;
	   file_size |= ((0x00000000 | byte2) << 2*8);
	   file_size |= ((0x00000000 | byte3) << 1*8);
	   file_size |= ((0x00000000 | byte4));

	   printf("got in with file_size %u, but got len %d\n", 
		  (unsigned int)file_size, (unsigned int)len);	   

	   if (file_size >= 40000) {
	     exit(1);
	     deb->quit = true;
	   }

	   upload_in_progress = true;
	   uploaded_bytes = 0;
	   fp = fopen("tmp.elf", "wb");

	   // Get Any Bytes that are in with this packet
	   int i;
	   for (i = 5;i < len;i++){	     
	     fwrite(&buf[i], 1, 1, fp);
	     uploaded_bytes++;

	     if (uploaded_bytes >= file_size) {
	       puts("met bytes");
	       fclose(fp);
	       system("msp430-objcopy -O binary tmp.elf tmp.bin");

	       deb->web_firmware_uploaded = true;
	       upload_in_progress = false;
	       return 0;
	     }
	   }

	   break;
	 }

         case 0x01: { // PLAY
	   printf("Got play\n");
	   cpu->running = true;
	   deb->debug_mode = false;
	   update_register_display(emu);

	   return 0;
	 }
      
         case 0x02: { // PAUSE
	   printf("Got pause\n");

	   if (cpu->running) {
	     cpu->running = false;
	     deb->debug_mode = true;

	     // display first round of registers
	     display_registers(emu);
	     disassemble(emu, cpu->pc, 1);
	     update_register_display(emu);
	   }
	   
	   return 0;
	 }

         case 0x03: { // SERIAL DATA
	   if (len > 1000) exit(1);

	   lent = len - 1;
	   data = (uint8_t *) (in + 1);
	   
	   pthread_t t;
	   if( pthread_create(&t, NULL, thrd, (void *)cpu->usci ) ) {
	     fprintf(stderr, "Error creating thread\n");                    
	   }
	   
	   //printf("Got serial data %s ... %d bytes long\n", 
	   //(char *)(in + 1), (unsigned int)len - 1);
	   
	   return 0;
	 }

         case 0x04: { // Console Input Data
	   if (len > 1000) exit(1);

	   buf = buf + 1;	   
	   printf("%s\n", buf);

	   if (!cpu->running && deb->debug_mode) {
	     exec_cmd(emu, buf, len);	  
	     update_register_display(emu);
	   }

	   return 0;
         }
      
         default: break;
      }
    }
  
  default: {
    break;
  }
}
    
  return 0;
}

void *web_server (void *ctxt)
{
  emu = (Emulator *) ctxt;
  Debugger *deb = emu->debugger;

  int port = 9001;
  //struct libwebsocket_context *context;
  struct lws_context *context;

  struct lws_context_creation_info context_info = {
    .port = deb->ws_port, //port, 
    .iface = NULL, 
    .protocols = protocols, 
    .extensions = NULL,
    .ssl_cert_filepath = NULL, 
    .ssl_private_key_filepath = NULL, 
    .ssl_ca_filepath = NULL,
    .gid = -1, 
    .uid = -1, 
    .options = 0, 
    NULL, 
    .ka_time = false, 
    .ka_probes = false, 
    .ka_interval = false
  };

  // Initialize packet queuing system
  init_packet_queue(emu);
  
  // create libwebsocket context representing this server
  context = lws_create_context(&context_info);

  if (context == NULL) {
    fprintf(stderr, "libwebsocket init failed\n");
    deb->quit = true;
    exit(1);
  }
    
  printf("starting server...\n");
    
  while (true) {
    lws_service(context, 10); // ms
    usleep(1000);
  }

  lws_context_destroy(context);
  return NULL;
}

void send_control(Emulator *emu, uint8_t opcode, 
		  void *data, size_t data_size)
{
  const static uint8_t NUM_OPCODE_BYTES   = 1;
  const static uint8_t NUM_DATA_LEN_BYTES = 1;

  if (data == NULL) { // Simple case
    packet_enqueue(emu, (void *)&opcode, 1, CONTROL_PACKET_OPCODE);
  }
  else { // data case ( OP DL DATADATADATA )
    uint8_t full_packet[NUM_OPCODE_BYTES + NUM_DATA_LEN_BYTES + data_size];

    // First part of packet is the opcode for specific control type
    memcpy(full_packet, (void *)&opcode, NUM_OPCODE_BYTES);

    // Second part is the data length field for following data
    memcpy(full_packet + NUM_OPCODE_BYTES, &data_size, 
	   NUM_DATA_LEN_BYTES);

    // Third part is the data itself.
    memcpy(full_packet + NUM_OPCODE_BYTES + NUM_DATA_LEN_BYTES, 
	   data, data_size);
    
    /*
    int i;
    for (i = 0;i < 1+size;i++) {
      printf("%02X ", *(uint8_t *)(full_packet + i));
    }
    puts(" .");
    */

    packet_enqueue(emu, (void *)&full_packet, 
		   NUM_OPCODE_BYTES + NUM_DATA_LEN_BYTES + data_size, 
		   CONTROL_PACKET_OPCODE);
    
  }
}

void print_serial (Emulator *emu, char *buf) 
{
  packet_enqueue(emu, buf, strlen(buf) + 1, SERIAL_PACKET_OPCODE);
}

void print_console (Emulator *emu, char *buf)
{
  packet_enqueue(emu, buf, strlen(buf) + 1, CONSOLE_PACKET_OPCODE);
}

int callback_http (
		   struct lws *wsi,
		   //enum libwebsocket_callback_reasons reason, 
		   enum lws_callback_reasons reason, 
		   void *user, void *in, size_t len)
{
  return 0;
}
