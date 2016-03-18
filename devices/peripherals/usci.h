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

#ifndef _USCI_H_
#define _USCI_H_

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <stdint.h>
#include <fcntl.h>

#include "../cpu/registers.h"
#include "../utilities.h"

// USCI_Ax Control Register 0
typedef struct Ctl0 
{
  bool UCPEN;   // Parity Enable: Disabled (0) Enabled (1)
  bool UCPAR;   // Parity Select: Odd (0) Even (1)
  bool UCMSB;   // MSB first: LSB first (0) MSB first (1)
  bool UC7BIT;  // Char Len: 8-bit (0) 7-bit (1)
  bool UCSPB;   // Stop bit select: One STP bits (0) Two STP bits (1)
 
  // USCI Mode - When UCSYNC = 0, its asynchronous mode
  // UART Mode (00)  
  // Idle-line multiprocessor mode (01)
  // Address-bit multiprocessor mode (10)
  // UART mode with automatic baud rate detection (11)
  uint8_t UCMODE : 2;
  
  bool UCSYNC;       // Synchronous mode enable: Async (0) Sync (1)
} Ctl0;

// USCI_Ax Control Register 1
typedef struct Ctl1 {
  // USCI clock source select, these bits select the Baud rate source clock.
  // UCLK: 00 | ACLK: 01 | SMCLK: 10 | SMCLK: 11
  uint8_t UCSSEL : 2;  

  bool UCRXEIE : 1; // Recv erroneous-character interrupt enable
  bool UCBRKIE : 1; // receive break character interrupt enable

  // dormant. Puts SCI into sleep mode: 
  // 0 - Not dormant. All received chars will set UCAxRXIFG
  // 1 - Dormant. Only chars that are preceded by an idle line or addr bit
  bool UCDORM : 1;

  // Transmit address, Next frame to be transmitted will be marked as address
  // Depedning on the selected multiproc mode. Data (0) Address (1)
  bool UCTXADDR : 1;

  // Transmit break, transmits a break with the next write to the TX buffer
  // Not a break (0) Transmit a break (1)
  bool UCTXBRK : 1;
  
  // Software reset enable: 
  // Disabled. USCI reset released for operation (0)
  // Enabled. USCI logic held in reset start. (1)
  bool UCSWRST : 1;  
} Ctl1;

typedef struct Usci 
{
  // USCI_A0 Module Register Pointers
  uint8_t *UCA0CTL0;
  uint8_t *UCA0CTL1;
  uint8_t *UCA0BR0;
  uint8_t *UCA0BR1;
  uint8_t *UCA0MCTL;
  uint8_t *UCA0STAT;
  uint8_t *UCA0RXBUF;
  uint8_t *UCA0TXBUF;
  uint8_t *UCA0ABCTL;
  uint8_t *UCA0IRTCTL;
  uint8_t *UCA0IRRCTL;

  uint8_t *IFG2;
  
  Ctl0 ctl0;
  Ctl1 ctl1;
} Usci;

void setup_usci(Emulator *emu);
void handle_usci(Emulator *emu);

#endif
