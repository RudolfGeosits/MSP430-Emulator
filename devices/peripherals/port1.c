/*
  MSP430 Emulator
  Copyright (C) 2014, 2015 Rudolf Geosits (rgeosits@live.esu.edu)  
                                                                      
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

#include "port1.h"

/* Cheat Sheet:
 *  IN:  0 = digital 0, 1 = digital 1
 *  OUT: 0 = INPUT, 1 = OUTPUT
 *  DIR: 0 = INPUT, 1 = OUTPUT
 *  REN: 0 = Pull Down, 1 = Pull Up
 *  IFG: 0 = No int pend, 1 = Interrupt Pending
 *   IE: 0 = int not enabled, 1 = int enabled
 */

void handle_port_1 (Emulator *emu)
{
  Cpu *cpu = emu->cpu;
  Port *p = cpu->p1;

  /* Handler P1.0 */
  if (*p->DIR & 0x01) {      /* Check Direction */
    P1DIR_0 = true;          /* Set P1DIR.0 flag (for gui) ON */
    if (*p->OUT & 0x01) {    /* Check OUTPUT */
      P1OUT_0 = true;        /* Set P1OUT.0 flag (for gui) ON */
    }
    else {
      P1OUT_0 = false;
    }
  }
  else {                     /* Check INPUT */
    P1DIR_0 = false;
  }

  if (*p->IE & 0x01) {       /* Check if Interrupt Enabled for pin */
    P1IE_0 = true;

    if (*p->IFG & 0x01) {    /* Check For Interrupt Pending */            
      P1IFG_0 = true;        /* Set P1IFG.0 flag indicating INT */
    }
    else {
      P1IFG_0 = false;
    }
  }
  else {
    P1IE_0 = false;
  }

  /* Handler P1.1 */
  if (*p->DIR & 0x02) {
    P1DIR_1 = true;
    if (*p->OUT & 0x02) {
      P1OUT_1 = true;
    }
    else {
      P1OUT_1 = false;
    }
  }
  else {
    P1DIR_1 = false;
  }

  if (*p->IE & 0x02) {
    P1IE_1 = true;

    if (*p->IFG & 0x02) {
      P1IFG_1 = true;
    }
    else {
      P1IFG_1 = false;
    }
  }
  else {
    P1IE_1 = false;
  }
  
  /* Handler P1.2 */
  if (*p->DIR & 0x04) {
    P1DIR_2 = true;
    if (*p->OUT & 0x04) {
      P1OUT_2 = true;
    }
    else {
      P1OUT_2 = false;
    }
  }
  else {
    P1DIR_2 = false;
  }

  if (*p->IE & 0x04) {
    P1IE_2 = true;

    if (*p->IFG & 0x04) {
      P1IFG_2 = true;
    }
    else {
      P1IFG_2 = false;
    }
  }
  else {
    P1IE_2 = false;
  }

  /* Handler P1.3 */
  if (*p->DIR & 0x08) {
    P1DIR_3 = true;
    if (*p->OUT & 0x08) {
      P1OUT_3 = true;
    }
    else {
      P1OUT_3 = false;
    }
  }
  else {
    P1DIR_3 = false;
  }

  if (*p->IE & 0x08) {
    P1IE_3 = true;

    if (*p->IFG & 0x08) {
      P1IFG_3 = true;
    }
    else {
      P1IFG_3 = false;
    }
  }
  else {
    P1IE_3 = false;
  }

  /* Handler P1.4 */
  if (*p->DIR & 0x10) {
    P1DIR_4 = true;
    if (*p->OUT & 0x10) {
      P1OUT_4 = true;
    }
    else {
      P1OUT_4 = false;
    }
  }
  else {
    P1DIR_4 = false;
  }

  if (*p->IE & 0x10) {
    P1IE_4 = true;

    if (*p->IFG & 0x10) {
      P1IFG_4 = true;
    }
    else {
      P1IFG_4 = false;
    }
  }
  else {
    P1IE_4 = false;
  }


  /* Handler P1.5 */
  if (*p->DIR & 0x20) {
    P1DIR_5 = true;
    if (*p->OUT & 0x20) {
      P1OUT_5 = true;
    }
    else {
      P1OUT_5 = false;
    }
  }
  else {
    P1DIR_5 = false;
  }

  if (*p->IE & 0x20) {
    P1IE_5 = true;

    if (*p->IFG & 0x20) {
      P1IFG_5 = true;
    }
    else {
      P1IFG_5 = false;
    }
  }
  else {
    P1IE_5 = false;
  }

  /* Handler P1.6 */
  if (*p->DIR & 0x40) {
    P1DIR_6 = true;
    if (*p->OUT & 0x40) {
      P1OUT_6 = true;
    }
    else {
      P1OUT_6 = false;
    }
  }
  else {
    P1DIR_6 = false;
  }

  if (*p->IE & 0x40) {
    P1IE_6 = true;

    if (*p->IFG & 0x40) {
      P1IFG_6 = true;
    }
    else {
      P1IFG_6 = false;
    }
  }
  else {
    P1IE_6 = false;
  }

  /* Handler P1.7 */
  if (*p->DIR & 0x80) {
    P1DIR_7 = true;
    if (*p->OUT & 0x80) {
      P1OUT_7 = true;
    }
    else {
      P1OUT_7 = false;
    }
  }
  else {
    P1DIR_7 = false;
  }

  if (*p->IE & 0x80) {
    P1IE_7 = true;

    if (*p->IFG & 0x80) {
      P1IFG_7 = true;
    }
    else {
      P1IFG_7 = false;
    }
  }
  else {
    P1IE_7 = false;
  }

}

void setup_port_1 (Emulator *emu) {
  Cpu *cpu = emu->cpu;
  Port *p = cpu->p1;
  
  static const uint16_t IN_VLOC   = 0x20;   /* Input */
  static const uint16_t OUT_VLOC  = 0x21;   /* Output */
  static const uint16_t DIR_VLOC  = 0x22;   /* Direction */
  static const uint16_t IFG_VLOC  = 0x23;   /* Interrupt flag */
  static const uint16_t IES_VLOC  = 0x24;   /* Interrupt Edge Select */
  static const uint16_t IE_VLOC   = 0x25;   /* Interrupt Enable */
  static const uint16_t SEL_VLOC  = 0x26;   /* Select */
  static const uint16_t SEL2_VLOC = 0x41;   /* Select 2 */
  static const uint16_t REN_VLOC  = 0x27;   /* Resistor Enable */

  *(p->IN   = (uint8_t *) get_addr_ptr(IN_VLOC))   = 0;
  *(p->OUT  = (uint8_t *) get_addr_ptr(OUT_VLOC))  = 0;
  *(p->DIR  = (uint8_t *) get_addr_ptr(DIR_VLOC))  = 0;
  *(p->IFG  = (uint8_t *) get_addr_ptr(IFG_VLOC))  = 0;
  *(p->IES  = (uint8_t *) get_addr_ptr(IES_VLOC))  = 0;
  *(p->IE   = (uint8_t *) get_addr_ptr(IE_VLOC))   = 0;
  *(p->SEL  = (uint8_t *) get_addr_ptr(SEL_VLOC))  = 0;
  *(p->SEL2 = (uint8_t *) get_addr_ptr(SEL2_VLOC)) = 0;
  *(p->REN  = (uint8_t *) get_addr_ptr(REN_VLOC))  = 0;
}

/* POWER UP CLEAR (PUC)                                                        
 *                                                                             
 * A PUC is always generated when a POR is generated, but a POR is not         
 * generated by a PUC. The following events trigger a PUC:                     
 *                                                                             
 * • A POR signal                                                              
 * • Watchdog timer expiration when in watchdog mode only                      
 * • Watchdog timer security key violation                                     
 * • A Flash memory security key violation                                     
 * • A CPU instruction fetch from the peripheral address range 0h to 01FFh     
void power_up_clear () {
  *P1OUT = *P1DIR = *P1IFG = *P1IE = *P1SEL = *P1SEL2 = *P1REN = 0;
}
 */
