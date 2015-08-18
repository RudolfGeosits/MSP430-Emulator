#define TXIFG 0x02
#define RXIFG 0x01
#include <termios.h>

int master;
FILE *slave;
int sp;
char c;

void *thrd (void *ctxt)
{
  Usci *usci = (Usci *)ctxt;
  char buf[64] = {0};

  while (true) {
    usleep(1000);
    if ( read(sp, buf, 1) > 0 ) {
      while (*usci->IFG2 & RXIFG);
      if (*buf == '\n') *buf = '\r';

      *usci->UCA0RXBUF = *(uint8_t *) buf;
      *usci->IFG2 |= RXIFG;
    }
  }  

  return NULL;
}

void open_pty (Cpu *cpu) 
{
  char slavename[64], buf[64];
  struct termios termios_p;
  
  master = posix_openpt(O_RDWR);

  grantpt(master);
  unlockpt(master);
  ptsname_r(master, slavename, sizeof slavename);
  snprintf(buf, sizeof buf, "-S%s/%d", strrchr(slavename,'/')+1, master);
  
  // Child (pty)
  if( !fork() ) {   
    char * const args[] = {
      "xterm", buf, 
      NULL
    };

    //execlp("xterm", "xterm", buf, (char *)0);
    execvp(args[0], args);
    exit(1);
  }
  // Parent                                                            
  // TURN OFF ECHO
  sp = open(slavename, O_RDWR, O_NONBLOCK);
  
  tcgetattr(sp, &termios_p);
  termios_p.c_iflag &= ~IGNBRK & ~BRKINT;
  termios_p.c_lflag &= ~ECHO;
  //termios_p.c_cc[VINTR] = 0x4;
  tcsetattr(sp, 0, &termios_p);
  
  pthread_t t;
  if( pthread_create(&t, NULL, thrd, (void *)cpu->usci ) ) {
    fprintf(stderr, "Error creating thread\n");
  }
}

void handle_usci (Cpu *cpu) 
{
  Usci *usci = cpu->usci;

  if (*usci->UCA0TXBUF & 0xFF) {
    write(sp, usci->UCA0TXBUF, 1);
    *usci->UCA0TXBUF = 0;
  }
}

void setup_usci (Cpu *cpu) 
{
  Usci *usci = cpu->usci;

  static const uint16_t UCA0CTL0_VLOC = 0x60; // Control Register 0
  static const uint16_t UCA0CTL1_VLOC = 0x61; // Control Register 1
  static const uint16_t UCA0BR0_VLOC  = 0x62; // Baud Rate ctl Register 0
  static const uint16_t UCA0BR1_VLOC  = 0x63; // Baud Rate ctl Register 1
  static const uint16_t UCA0MCTL_VLOC = 0x64; // Modulation ctl Register
  static const uint16_t UCA0STAT_VLOC = 0x65; // Status Register
  static const uint16_t UCA0RXBUF_VLOC = 0x66; // RECV buffer register
  static const uint16_t UCA0TXBUF_VLOC = 0x67; // Transmit buffer register
  static const uint16_t UCA0ABCTL_VLOC = 0x5D; // Auto-Baud control register
  static const uint16_t UCA0IRTCTL_VLOC = 0x5E; // IrDA transmit control reg
  static const uint16_t UCA0IRRCTL_VLOC = 0x5F; // IrDA Receive control reg
  static const uint16_t IFG2_VLOC       = 0x03; // Interrupt flag register 2
  
  // Set initial values
  *(usci->UCA0CTL0   = (uint8_t *) get_addr_ptr(UCA0CTL0_VLOC))  = 0;
  *(usci->UCA0CTL1  = (uint8_t *) get_addr_ptr(UCA0CTL1_VLOC))   = 0x01;
  *(usci->UCA0BR0  = (uint8_t *) get_addr_ptr(UCA0BR0_VLOC))     = 0;
  *(usci->UCA0BR1  = (uint8_t *) get_addr_ptr(UCA0BR1_VLOC))     = 0;
  *(usci->UCA0MCTL  = (uint8_t *) get_addr_ptr(UCA0MCTL_VLOC))   = 0;
  *(usci->UCA0STAT  = (uint8_t *) get_addr_ptr(UCA0STAT_VLOC))   = 0;
  *(usci->UCA0RXBUF  = (uint8_t *) get_addr_ptr(UCA0RXBUF_VLOC)) = 0;
  *(usci->UCA0TXBUF  = (uint8_t *) get_addr_ptr(UCA0TXBUF_VLOC)) = 0;
  *(usci->UCA0ABCTL  = (uint8_t *) get_addr_ptr(UCA0ABCTL_VLOC))   = 0;
  *(usci->UCA0IRTCTL  = (uint8_t *) get_addr_ptr(UCA0IRTCTL_VLOC)) = 0;
  *(usci->UCA0IRRCTL  = (uint8_t *) get_addr_ptr(UCA0IRRCTL_VLOC)) = 0;  

  usci->IFG2  = (uint8_t *) get_addr_ptr(IFG2_VLOC);
  *usci->IFG2 |= TXIFG;
  //*usci->IFG2 |= RXIFG;
}
