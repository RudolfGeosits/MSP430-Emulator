#include "server.h"

struct lws *ws = NULL;
struct lws_context *context;
pid_t emulator_pids[1000] = {0};

void handle_sigchld(int sig) {
  int saved_errno = errno;

  while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}
  errno = saved_errno;
}

void signal_callback_handler(int signum)
{
  printf("Caught signal %d\n", signum);
  // Cleanup and close up stuff here
  int saved_errno = errno;

  while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}
  errno = saved_errno;

  // Terminate program
  exit(signum);
}

void web_send (char *buf)
{
  int len;
  char *msg, *begin;

  // send message 
  len = strlen(buf);

  msg = (char *) malloc(len + LWS_SEND_BUFFER_PRE_PADDING
			+ LWS_SEND_BUFFER_POST_PADDING);

  begin = msg + LWS_SEND_BUFFER_PRE_PADDING;

  strncpy(begin, buf, len);
  int n = lws_write(ws, begin, len, LWS_WRITE_TEXT);
  printf("sent %d bytes of %d len\n", n, len);
  
  free(msg);
}


int callback_http (
		   struct lws *wsi,
		   enum lws_callback_reasons reason,
		   void *user, void *in, size_t len)
{
  ws = wsi;
  return 0;
}

int callback_emu ( struct lws *wsi,
		   enum lws_callback_reasons reason,
		   void *user, void *in, size_t len )
{
  static unsigned new_ws_port = 9001;
  char port_str[100] = {0};
  
  switch (reason) {
    case LWS_CALLBACK_PROTOCOL_INIT: {
      ws = wsi;
      break;
    };

    case LWS_CALLBACK_SERVER_WRITEABLE: {
      pid_t pid;
      sprintf(port_str, "%u", new_ws_port);
  
      // Child (pty)     
      if( !(pid = fork()) ) {     
	printf("Child: Got pid #%u\n", pid);

	char * const args[] = {
	  "nice", "-20", "./MSP430", port_str,
	  //"./MSP430", port_str,
	  //"gdb", "-ex", "run", "--args", "./MSP430", port_str,
	  NULL
	};

	setpgid(0, 0);     
	execvp(args[0], args);
	exit(1);                
      }

      // Parent 
      printf("Parent: Got pid #%u\n", pid);
      usleep(1000);
    
      web_send(port_str);
      ++new_ws_port;

      lws_close_reason(wsi, 0, NULL, 0);

      break;
    };

    case LWS_CALLBACK_ESTABLISHED: {
      puts("connection established");

      lws_callback_on_writable(wsi);
      break;
    }

    default: {
      printf("Some other thing: %d\n", reason); 
      break;
      
      ws = wsi;
    }
  }
  
  return 0;
}

int main (int argc, char **argv)
{
  int port = 9000;

  struct lws_context_creation_info context_info = {
    .port = port,
    .iface = NULL,
    .protocols = protocols,
    .ssl_cert_filepath = NULL,
    .ssl_private_key_filepath = NULL,
    .ssl_ca_filepath = NULL,

    .gid = -1,
    .uid = -1,

    .max_http_header_pool = 1,
    .options = 0,
  };
  
  // create lws context representing this server
  context = lws_create_context(&context_info);
  
  if (context == NULL) {
    puts("lws init failed\n");
    return -1;
  }

  signal(SIGINT, signal_callback_handler);

  // Setup child reaping handler
  struct sigaction sa;
  sa.sa_handler = &handle_sigchld;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
  if (sigaction(SIGCHLD, &sa, 0) == -1) {
    perror(0);
    exit(1);
  }

  printf("starting server...\n");

  while (true) {
    lws_service(context, 10); // ms
    //lws_callback_on_writable_all_protocol(context, &protocols[1]);
    usleep(1000);
  }

  lws_context_destroy(context);  
  return 0;
}
