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

struct libwebsocket *ws = NULL;
Emulator *emu = NULL;

int callback_emu (struct libwebsocket_context *this,
			 struct libwebsocket *wsi,
			 enum libwebsocket_callback_reasons reason,
			 void *user, void *in, size_t len)
{
  Cpu *cpu = emu->cpu;
  Debugger *deb = emu->debugger;

  static FILE *fp = NULL;
  static bool upload = false;

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
      static bool red_led = false;
      static bool green_led = false;
      static bool serial = false;

      // P1.0 ON (launchpad red LED)
      if (P1DIR_0 && P1OUT_0) {
	if (red_led == false) {
	  web_send("P1.0 1", CONTROL);
	  red_led = true;
	}
      }
      // P1.0 OFF (launchpad red LED)
      else if (P1DIR_0 && !P1OUT_0) {
	if (red_led == true) {
	  web_send("P1.0 0", CONTROL);
	  red_led = false;
	}
      }

      // P1.1 ON
      if (P1DIR_1 && P1OUT_1) {
	web_send("P1.1 1", CONTROL);
      }
      // P1.1 OFF
      else if (P1DIR_1 && !P1OUT_1) {
	web_send("P1.1 0", CONTROL);
      }

      // P1.2 ON
      if (P1DIR_2 && P1OUT_2) {
	web_send("P1.2 1", CONTROL);
      }
      // P1.2 OFF
      else if (P1DIR_2 && !P1OUT_2) {
	web_send("P1.2 0", CONTROL);
      }

      // 1.6 ON (launchpad green LED)
      if (P1DIR_6 && P1OUT_6) {
	if (green_led == false) {
	  web_send("P1.6 1", CONTROL);
	  green_led = true;
	}
      }
    
      // 1.6 OFF (launchpad green LED) 
      else if (P1DIR_6 && !P1OUT_6) {
	if (green_led == true) {
	  web_send("P1.6 0", CONTROL);
	  green_led = false;
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
      static bool serial = false;
      char *buf = (char *)in;      
      
      if (serial) {
	uint8_t *data = (uint8_t *) buf;
	int i, j = 0;

 	//printf("len is %d\nstr is %s\n", len, data);

	for (i = 0;i < len;i++) {
	  uint8_t thing = *(data + i);
	  usleep(333);
	  while (*cpu->usci->IFG2 & RXIFG) {j++;}
	  
	  if (thing == '\n') {                  
	    thing = '\r';
	  }                          

	  //printf("Got 0x%02X (%c)\n", thing, thing);
	  /*
	    if (*data == '\\') { 
	    //read(sp, data, 1);
	    data += 1;

	    if (*data == 'h') {
	    //read(sp, data, 2);                                    

	    //data[2] = 0;                         
	    //*cpu->usci->UCA0RXBUF = (uint8_t) strtoul(data, NULL, 16);  
	    }                                                     
	    } 
	  */
	  //else {                                                                                  
	  *cpu->usci->UCA0RXBUF = thing;
	  //}                 
	    
	  *cpu->usci->IFG2 |= RXIFG;
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
