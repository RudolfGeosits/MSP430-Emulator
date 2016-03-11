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

    case LWS_CALLBACK_SERVER_WRITEABLE: {
      puts("serv writable");
      break;
    }
      
    case LWS_CALLBACK_CLIENT_WRITEABLE: {
      puts("cli writable");
      break;
    }

    case LWS_CALLBACK_RECEIVE: {
      char *buf = (char *)in;      
      
      if ( !strncmp((const char *)buf, (const char *)"PAUSE", sizeof("PAUSE")) ) {
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

	/*
	int i;
	for (i = 0;i < len;i++) {
	  printf("%02X", str[i]);
	}
	*/
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
  static bool red_led = false;
  static bool green_led = false;
    
  while (true) {
    libwebsocket_service(context, 10); // ms
    if (!ws) continue;

    // 1.0 ON (launchpad red LED)
    if (P1DIR_0 && P1OUT_0) {
      if (red_led == false) {
	web_send("RED_ON");
	red_led = true;
      }
    }

    // 1.0 OFF (launchpad red LED)
    else if (P1DIR_0 && !P1OUT_0) {
      if (red_led == true) {
	web_send("RED_OFF");
	red_led = false;
      }
    }

    // 1.6 ON (launchpad green LED)
    if (P1DIR_6 && P1OUT_6) {
      if (green_led == false) {
	web_send("GREEN_ON");
	green_led = true;
      }
    }
    
    // 1.6 OFF (launchpad green LED) 
    else if (P1DIR_6 && !P1OUT_6) {
      if (green_led == true) {
	web_send("GREEN_OFF");
	green_led = false;
      }
    }

  }
    
  libwebsocket_context_destroy(context);
  return NULL;
}

void web_send (char *buf)
{
  if (ws == NULL) {
    printf("fail.\n");
    return;
  }

  int len = strlen(buf);
  char *sendbuf = (char *) malloc(len + LWS_SEND_BUFFER_PRE_PADDING 
				  + LWS_SEND_BUFFER_POST_PADDING);

  char *begin = sendbuf + LWS_SEND_BUFFER_PRE_PADDING;

  strncpy(begin, buf, len);
  libwebsocket_write(ws, begin, len, LWS_WRITE_TEXT);

  free(sendbuf);
}

int callback_http (struct libwebsocket_context *this, 
			  struct libwebsocket *wsi,
			  enum libwebsocket_callback_reasons reason, 
			  void *user, void *in, size_t len)
{
  ws = wsi;

  return 0;
}
