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

#include "server.h"

#define TXIFG 0x02
#define RXIFG 0x01
#define HIGH  1
#define LOW   0
#define OUT   1
#define IN    0

struct libwebsocket *ws = NULL;
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

int callback_emu (struct libwebsocket_context *this,
			 struct libwebsocket *wsi,
			 enum libwebsocket_callback_reasons reason,
			 void *user, void *in, size_t len)
{
  Cpu *cpu = emu->cpu;
  Port_1 *p1 = emu->cpu->p1;
  Debugger *deb = emu->debugger;

  static FILE *fp = NULL;
  static bool upload = false;
  static bool serial = false;

  switch (reason) {
    case LWS_CALLBACK_ESTABLISHED: {
      puts("connection established");
      
      // Flip ready flag for the emulator to begin
      deb->web_server_ready = true;

      // get the ball rolling
      libwebsocket_callback_on_writable(this, wsi);

      break;
    }

    case LWS_CALLBACK_SERVER_WRITEABLE: {
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
	    web_send("P1.0 1", CONTROL);
	    p1_0_on = true;
	  }
	}
	else if (p1->OUT_0 == LOW) {
	  if (p1_0_on == true) {
	    web_send("P1.0 0", CONTROL);
	    p1_0_on = false;
	  }
	}
      }

      // P1.1 ON/OFF
      if (p1->DIR_1 == OUT) {
	if (p1->OUT_1 == HIGH) {	  
	  if (p1_1_on == false) {
	    web_send("P1.1 1", CONTROL);
	    p1_1_on = true;
	  }
	}
	else if (p1->OUT_1 == LOW) {
	  if (p1_1_on == true) {
	    web_send("P1.1 0", CONTROL);
	    p1_1_on = false;
	  }
	}
      }

      // P1.2 ON/OFF
      if (p1->DIR_2 == OUT) {
	if (p1->OUT_2 == HIGH) {	  
	  if (p1_2_on == false) {
	    web_send("P1.2 1", CONTROL);
	    p1_2_on = true;
	  }
	}
	else if (p1->OUT_2 == LOW) {
	  if (p1_2_on == true) {
	    web_send("P1.2 0", CONTROL);
	    p1_2_on = false;
	  }
	}
      }

      // P1.3 ON/OFF
      if (p1->DIR_3 == OUT) {
	if (p1->OUT_3 == HIGH) {	  
	  if (p1_3_on == false) {
	    web_send("P1.3 1", CONTROL);
	    p1_3_on = true;
	  }
	}
	else if (p1->OUT_3 == LOW) {
	  if (p1_3_on == true) {
	    web_send("P1.3 0", CONTROL);
	    p1_3_on = false;
	  }
	}
      }

      // P1.4 ON/OFF
      if (p1->DIR_4 == OUT) {
	if (p1->OUT_4 == HIGH) {	  
	  if (p1_4_on == false) {
	    web_send("P1.4 1", CONTROL);
	    p1_4_on = true;
	  }
	}
	else if (p1->OUT_4 == LOW) {
	  if (p1_4_on == true) {
	    web_send("P1.4 0", CONTROL);
	    p1_4_on = false;
	  }
	}
      }

      // P1.5 ON/OFF
      if (p1->DIR_5 == OUT) {
	if (p1->OUT_5 == HIGH) {	  
	  if (p1_5_on == false) {
	    web_send("P1.5 1", CONTROL);
	    p1_5_on = true;
	  }
	}
	else if (p1->OUT_5 == LOW) {
	  if (p1_5_on == true) {
	    web_send("P1.5 0", CONTROL);
	    p1_5_on = false;
	  }
	}
      }
      
      // P1.6 ON/OFF
      if (p1->DIR_6 == OUT) {
	if (p1->OUT_6 == HIGH) {	  
	  if (p1_6_on == false) {
	    web_send("P1.6 1", CONTROL);
	    p1_6_on = true;
	  }
	}
	else if (p1->OUT_6 == LOW) {
	  if (p1_6_on == true) {
	    web_send("P1.6 0", CONTROL);
	    p1_6_on = false;
	  }
	}
      }

      // P1.7 ON/OFF
      if (p1->DIR_7 == OUT) {
	if (p1->OUT_7 == HIGH) {	  
	  if (p1_7_on == false) {
	    web_send("P1.7 1", CONTROL);
	    p1_7_on = true;
	  }
	}
	else if (p1->OUT_7 == LOW) {
	  if (p1_7_on == true) {
	    web_send("P1.7 0", CONTROL);
	    p1_7_on = false;
	  }
	}
      }      
      
      libwebsocket_callback_on_writable(this, wsi);
      break;
    }

    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR: {
      puts("Connection Error");
      break;
    }

    case LWS_CALLBACK_CLOSED: {
      puts("Connection Closed");
      break;
    }

    case LWS_CALLBACK_CLIENT_WRITEABLE: {
      puts("cli writable");
      break;
    }

    case LWS_CALLBACK_RECEIVE: {
      char *buf = (char *)in;      
      
      if (serial) {
	lent = len;
	data = (uint8_t *) in;

	pthread_t t;                      	

	if( pthread_create(&t, NULL, thrd, (void *)cpu->usci ) ) {
	  fprintf(stderr, "Error creating thread\n");                    
	}

	serial = false;
      }
      else if ( !strncmp((const char *)buf, (const char *)"PAUSE", sizeof("PAUSE")) ) {
	if (deb->run) {
	  deb->run = false;
	  deb->debug_mode = true;

	  // display first round of registers
	  display_registers(emu);
	  disassemble(emu, cpu->pc, 1);
	}
      }
      else if ( !strncmp((const char *)buf, (const char *)"PLAY", sizeof("PLAY")) ) {
	deb->run = true;
	deb->debug_mode = false;
      }
      else if ( !strncmp((const char *)buf, (const char *)"UPLOAD", sizeof("UPLOAD")) ) {
	upload = true;
	fp = fopen("tmp.elf", "wb");
	puts("upload set");
      }      
      else if ( !strncmp((const char *)buf, (const char *)"NPLOAD", sizeof("NPLOAD")) ) {
	upload = false;
	fclose(fp);
	puts("npload reset");
	system("msp430-objcopy -O binary tmp.elf tmp.bin");
	puts("objcopy done");

	deb->web_firmware_uploaded = true;
      }
      else if (upload) {
	unsigned char *str = (unsigned char *)in;	
	fwrite(str, 1, len, fp);
      }

      else if ( !strncmp((const char *)buf, (const char *)"_SERIAL_", sizeof("_SERIAL_")) ) {
   	//puts("serial");
	serial = true;
      }

      else {
	printf("%s\n", buf);

	if (!deb->run && deb->debug_mode) {
	  exec_cmd(emu, buf, len);
	}
      }

      break;
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
  Debugger *debugger = emu->debugger;

  int port = 9000;
  struct libwebsocket_context *context;

  struct lws_context_creation_info context_info = {
    .port = port, 
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

  // create libwebsocket context representing this server
  context = libwebsocket_create_context(&context_info);

  if (context == NULL) {
    fprintf(stderr, "libwebsocket init failed\n");
    exit(1);
  }
    
  printf("starting server...\n");
    
  while (true) {
    libwebsocket_service(context, 10); // ms
    usleep(1000);
  }

  libwebsocket_context_destroy(context);
  return NULL;
}

void web_send (char *buf, uint8_t type)
{
  int len;
  char *msg, *begin;
  char *intro;// = "_STDOUT_";

  if (type != CONTROL) {
    if (type == STDOUT) {
      intro = "_STDOUT_";      
    }
    else if (type == SERIAL) {
      intro = "_SERIAL_";      
    }

    // Send intro
    len = strlen(intro);
    msg = (char *) malloc(len + LWS_SEND_BUFFER_PRE_PADDING 
			  + LWS_SEND_BUFFER_POST_PADDING);

    begin = msg + LWS_SEND_BUFFER_PRE_PADDING;

    strncpy(begin, intro, len);
    libwebsocket_write(ws, begin, len, LWS_WRITE_TEXT);    
  }

  if (ws == NULL) {
    printf("fail.\n");
    return;
  }

  // send message
  len = strlen(buf);
  msg = (char *) malloc(len + LWS_SEND_BUFFER_PRE_PADDING 
			+ LWS_SEND_BUFFER_POST_PADDING);

  begin = msg + LWS_SEND_BUFFER_PRE_PADDING;

  strncpy(begin, buf, len);
  libwebsocket_write(ws, begin, len, LWS_WRITE_TEXT);

  free(msg);
}

int callback_http (struct libwebsocket_context *this, 
			  struct libwebsocket *wsi,
			  enum libwebsocket_callback_reasons reason, 
			  void *user, void *in, size_t len)
{
  ws = wsi;

  return 0;
}
