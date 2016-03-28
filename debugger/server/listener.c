#include "listener.h"

struct libwebsocket *ws = NULL;

void web_send (char *buf)
{
  int len;
  char *msg, *begin;
  char *intro;// = "_STDOUT_";

  /*
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
  */
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


int callback_emu (struct libwebsocket_context *this,
		  struct libwebsocket *wsi,
		  enum libwebsocket_callback_reasons reason,
		  void *user, void *in, size_t len)
{
  static unsigned new_ws_port = 9001;
  char port_str[100] = {0};
  
  switch (reason) {
    case LWS_CALLBACK_ESTABLISHED: {
      sprintf(port_str, "%u", new_ws_port);

      puts("connection established");
      
      // Child (pty)                                                        
      if( !fork() ) {                                                       
	char * const args[] = {                                             
	  "nice", "-20", "./MSP430", port_str,
	  NULL                                                              
	};                                                                  
                                                                        
	setpgid(0, 0);                                                      
	execvp(args[0], args);                                              
	exit(1);                                                            
      }                                                                     
      // Parent 

      usleep(1000);
      web_send(port_str);
      ++new_ws_port;
    }
  }
  
  return 0;
}

int main (int argc, char **argv)
{
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
  return 0;
}
