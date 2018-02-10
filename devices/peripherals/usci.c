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

#define TXIFG 0x02
#define RXIFG 0x01

#include "usci.h"

/*
int master;
FILE *slave;
int sp;
char c;

void *thrd (void *ctxt)
{
  Usci *usci = (Usci *)ctxt;
  char buf[64] = {0};

  while (true) {
    usleep(333);
    if ( read(sp, buf, 1) > 0 ) {
      while (*usci->IFG2 & RXIFG);

      if (*buf == '\n') {
	*buf = '\r';
      }
      if (*buf == '\\') {
	// Ah, escape sequence, what will I parse it as?
	read(sp, buf, 1);
	if (*buf == 'h') {
	  read(sp, buf, 2);
	  buf[2] = 0;
	  *usci->UCA0RXBUF = (uint8_t) strtoul(buf, NULL, 16);
	}
      }
      else {    
	*usci->UCA0RXBUF = *(uint8_t *) buf;
      }

      *usci->IFG2 |= RXIFG;
    }
  }  

  return NULL;
}

void open_pty (Emulator *emu) 
{
  Cpu *cpu = emu->cpu;

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

    setpgid(0, 0);
    execvp(args[0], args);
    exit(1);
  }
  // Parent                                                            

  sp = open(slavename, O_RDWR, O_NONBLOCK);  
  read(sp, buf, 100);

  tcgetattr(sp, &termios_p);
  termios_p.c_lflag |= ECHO;
  tcsetattr(sp, 0, &termios_p);
  
  pthread_t t;
  if( pthread_create(&t, NULL, thrd, (void *)cpu->usci ) ) {
    fprintf(stderr, "Error creating thread\n");
  }
}
*/
void handle_usci (Emulator *emu) 
{
  Cpu *cpu = emu->cpu;
  Debugger *deb = emu->debugger;
  Usci *usci = cpu->usci;
  Port_1 *p1 = cpu->p1;
  
  static bool uart_active = false;
  
  // Handle sending from TX pin (P1.2) 
  if (p1->SEL_2 && p1->SEL2_2) {
    if (uart_active == false) {
      puts("UART TX pin activated on P1.2");
      uart_active = true;
    }

    // UCAxTXIFG
    if (*usci->IFG2 & TXIFG) {
      uint8_t c = *usci->UCA0TXBUF;
      unsigned char str[2];
      str[0] = c;
      str[1] = 0;

      *usci->IFG2 &= ~TXIFG;

      if (c & 0xFF) {
	if (deb->web_interface) {
	  print_serial(emu, str);
	  //write(sp, usci->UCA0TXBUF, 1);
	}
	else if (deb->console_interface) {
	  //write(sp, usci->UCA0TXBUF, 1);
	}

	*usci->UCA0TXBUF = 0;
      }

      //*usci->IFG2 &= TXIFG;
      *usci->IFG2 |= TXIFG;
    }
  }

  return;
}

void setup_usci (Emulator *emu) 
{
  Cpu *cpu = emu->cpu;
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
  *usci->IFG2 &= ~RXIFG;
}
