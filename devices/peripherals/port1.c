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

#include "port1.h"

/* Cheat Sheet:
 *  PxIN :  0 = LOW input, 1 = HIGH input
 *  PxOUT:  0 = LOW output, 1 = HIGH output
 *  PxDIR:  0 = INPUT, 1 = OUTPUT
 *  PxREN:  0 = Pull Up/Down DISABLED, 1 = Pull Up/Down ENABLED
 *
 *  PxSEL2 | PxSEL | Explaination
 *       0 |     0 | I/O function selected 
 *       0 |     1 | Primary Peripheral module function selected
 *       1 |     0 | Reserved ?
 *       1 |     1 | Secondary Peripheral module function selected
 *  [NOTE: P1 and P2 port pin INTs are disabled when PxSEL = 1]
 *
 *  PxIFG:  0 = No interrupt pending, 1 = Interrupt Pending
 *  PxIES:  PxIFG set with a [0 = LOW-HIGH, 1 = HIGH-LOW] transition 
 *  PxIE :  0 = interrupt disabled, 1 = interrupt enabled
 */

void handle_port_1 (Emulator *emu)
{
  Cpu *cpu = emu->cpu;
  Port_1 *p = cpu->p1;

  //////////////////// P1.0 ////////////////////////
 
  if (*p->DIR & 0x01) {    // Check Direction
    p->DIR_0 = true;       // Set P1DIR.0 flag
    if (*p->OUT & 0x01) {  // Check OUTPUT 
      p->OUT_0 = true;     // Set P1OUT.0 flag
    }
    else {
      p->OUT_0 = false;
    }
  }
  else {                   // Check INPUT 
    p->DIR_0 = false;
  }
  
  /// Check if Interrupt Enabled for pin 
  if (*p->IE & 0x01) {     
    p->IE_0 = true;

    // Check For Interrupt Pending 
    if (*p->IFG & 0x01) {  
      // Set p->IFG.0 flag indicating INT 
      p->IFG_0 = true;
    }
    else {
      p->IFG_0 = false;
    }
  }
  else {
    p->IE_0 = false;
  }  
  
  // Check primary select
  if (*p->SEL & 0x01) {
    if (p->SEL_0 == false) {
      puts("P1_SEL_0 = 1");
    }
    
    p->SEL_0 = true;
  }
  else {
    if (p->SEL_0 == true) {
      puts("P1_SEL_0 = 0");
    }

    p->SEL_0 = false;
  }

  // Check secondary select
  if (*p->SEL2 & 0x01) {
    if (p->SEL2_0 == false) {
      puts("P1_SEL2_0 = 1");
    }
    
    p->SEL2_0 = true;
  }
  else {
    if (p->SEL2_0 == true) {
      puts("P1_SEL2_0 = 0");
    }

    p->SEL2_0 = false;    
  }

  //////////////////// P1.1 ////////////////////////

  // Check Direction and IN/OUT
  if (*p->DIR & 0x02) {
    p->DIR_1 = true;
    if (*p->OUT & 0x02) {
      p->OUT_1 = true;
    }
    else {
      p->OUT_1 = false;
    }
  }
  else {
    p->DIR_1 = false;
  }

  // Check Interrupts
  if (*p->IE & 0x02) {
    p->IE_1 = true;

    if (*p->IFG & 0x02) {
      p->IFG_1 = true;
    }
    else {
      p->IFG_1 = false;
    }
  }
  else {
    p->IE_1 = false;
  }

  // Check primary select
  if (*p->SEL & 0x02) {
    if (p->SEL_1 == false) {
      puts("P1_SEL_1 = 1");
    }

    p->SEL_1 = true;
  }
  else {
    if (p->SEL_1 == true) {
      puts("P1_SEL_1 = 0");
    }

    p->SEL_1 = false;
  }

  // Check secondary select
  if (*p->SEL2 & 0x02) {
    if (p->SEL2_1 == false) {
      p->SEL2_1 = true;
      puts("P1_SEL2_1 = 1");
    }
  }
  else {
    if (p->SEL2_1 == true) {
      p->SEL2_1 = false;    
      puts("P1_SEL2_1 = 0");
    }
  }

  //////////////////// P1.2 ////////////////////////

  if (*p->DIR & 0x04) {
    p->DIR_2 = true;
    if (*p->OUT & 0x04) {
      p->OUT_2 = true;
    }
    else {
      p->OUT_2 = false;
    }
  }
  else {
    p->DIR_2 = false;
  }

  if (*p->IE & 0x04) {
    p->IE_2 = true;

    if (*p->IFG & 0x04) {
      p->IFG_2 = true;
    }
    else {
      p->IFG_2 = false;
    }
  }
  else {
    p->IE_2 = false;
  }

  // Check primary select
  if (*p->SEL & 0x04) {
    if (p->SEL_2 == false) {
      puts("P1_SEL_2 = 1");
    }
    
    p->SEL_2 = true;
  }
  else {
    if (p->SEL_2 == true) {
      puts("P1_SEL_2 = 0");
    }

    p->SEL_2 = false;
  }

  // Check secondary select
  if (*p->SEL2 & 0x04) {
    if (p->SEL2_2 == false) {
      puts("P1_SEL2_2 = 1");
    }
    
    p->SEL2_2 = true;
  }
  else {
    if (p->SEL2_2 == true) {
      puts("P1_SEL2_2 = 0");
    }

    p->SEL2_2 = false;    
  }

  ////////////////////////////////////////////////

  // Handler P1.3 
  if (*p->DIR & 0x08) {
    p->DIR_3 = true;
    if (*p->OUT & 0x08) {
      p->OUT_3 = true;
    }
    else {
      p->OUT_3 = false;
    }
  }
  else {
    p->DIR_3 = false;
  }

  if (*p->IE & 0x08) {
    p->IE_3 = true;

    if (*p->IFG & 0x08) {
      p->IFG_3 = true;
    }
    else {
      p->IFG_3 = false;
    }
  }
  else {
    p->IE_3 = false;
  }

  ///////////////////////////////////////////////////////////////

  // Handler P1.4 
  if (*p->DIR & 0x10) {
    p->DIR_4 = true;
    if (*p->OUT & 0x10) {
      p->OUT_4 = true;
    }
    else {
      p->OUT_4 = false;
    }
  }
  else {
    p->DIR_4 = false;
  }

  if (*p->IE & 0x10) {
    p->IE_4 = true;

    if (*p->IFG & 0x10) {
      p->IFG_4 = true;
    }
    else {
      p->IFG_4 = false;
    }
  }
  else {
    p->IE_4 = false;
  }

  /////////////////////////////////////////////////

  // Handler P1.5 
  if (*p->DIR & 0x20) {
    p->DIR_5 = true;
    if (*p->OUT & 0x20) {
      p->OUT_5 = true;
    }
    else {
      p->OUT_5 = false;
    }
  }
  else {
    p->DIR_5 = false;
  }

  if (*p->IE & 0x20) {
    p->IE_5 = true;

    if (*p->IFG & 0x20) {
      p->IFG_5 = true;
    }
    else {
      p->IFG_5 = false;
    }
  }
  else {
    p->IE_5 = false;
  }

  ////////////////////////////////////////////////////

  // Handler P1.6 
  if (*p->DIR & 0x40) {
    p->DIR_6 = true;
    if (*p->OUT & 0x40) {
      p->OUT_6 = true;
    }
    else {
      p->OUT_6 = false;
    }
  }
  else {
    p->DIR_6 = false;
  }

  if (*p->IE & 0x40) {
    p->IE_6 = true;

    if (*p->IFG & 0x40) {
      p->IFG_6 = true;
    }
    else {
      p->IFG_6 = false;
    }
  }
  else {
    p->IE_6 = false;
  }

  ////////////////////////////////////////////////////

  // Handler P1.7 
  if (*p->DIR & 0x80) {
    p->DIR_7 = true;
    if (*p->OUT & 0x80) {
      p->OUT_7 = true;
    }
    else {
      p->OUT_7 = false;
    }
  }
  else {
    p->DIR_7 = false;
  }

  if (*p->IE & 0x80) {
    p->IE_7 = true;

    if (*p->IFG & 0x80) {
      p->IFG_7 = true;
    }
    else {
      p->IFG_7 = false;
    }
  }
  else {
    p->IE_7 = false;
  }

}

void setup_port_1 (Emulator *emu)
{
  Cpu *cpu = emu->cpu;
  Port_1 *p = cpu->p1;
  
  static const uint16_t IN_VLOC   = 0x20;   // Input
  static const uint16_t OUT_VLOC  = 0x21;   // Output
  static const uint16_t DIR_VLOC  = 0x22;   // Direction
  static const uint16_t IFG_VLOC  = 0x23;   // Interrupt flag
  static const uint16_t IES_VLOC  = 0x24;   // Interrupt Edge Select
  static const uint16_t IE_VLOC   = 0x25;   // Interrupt Enable
  static const uint16_t SEL_VLOC  = 0x26;   // Select
  static const uint16_t SEL2_VLOC = 0x41;   // Select 2
  static const uint16_t REN_VLOC  = 0x27;   // Resistor Enable
  
  *(p->IN   = (uint8_t *) get_addr_ptr(IN_VLOC))   = 0;
  *(p->OUT  = (uint8_t *) get_addr_ptr(OUT_VLOC))  = 0;
  *(p->DIR  = (uint8_t *) get_addr_ptr(DIR_VLOC))  = 0;
  *(p->IFG  = (uint8_t *) get_addr_ptr(IFG_VLOC))  = 0;
  *(p->IES  = (uint8_t *) get_addr_ptr(IES_VLOC))  = 0;
  *(p->IE   = (uint8_t *) get_addr_ptr(IE_VLOC))   = 0;
  *(p->SEL  = (uint8_t *) get_addr_ptr(SEL_VLOC))  = 0;
  *(p->SEL2 = (uint8_t *) get_addr_ptr(SEL2_VLOC)) = 0;
  *(p->REN  = (uint8_t *) get_addr_ptr(REN_VLOC))  = 0;

  
  p->DIR_0 = false; p->OUT_0 = false; p->IFG_0 = false; 
  p->IE_0 = false; p->SEL_0 = false; p->SEL2_0 = false;
  
  p->DIR_1 = false; p->OUT_1 = false; p->IFG_1 = false; 
  p->IE_1 = false; p->SEL_1 = false; p->SEL2_1 = false;
  
  p->DIR_2 = false; p->OUT_2 = false; p->IFG_2 = false; 
  p->IE_2 = false; p->SEL_2 = false; p->SEL2_2 = false;
  
  p->DIR_3 = false; p->OUT_3 = false; p->IFG_3 = false; 
  p->IE_3 = false; p->SEL_3 = false; p->SEL2_3 = false;
  
  p->DIR_4 = false; p->OUT_4 = false; p->IFG_4 = false; 
  p->IE_4 = false; p->SEL_4 = false; p->SEL2_4 = false;
  
  p->DIR_5 = false; p->OUT_5 = false; p->IFG_5 = false; 
  p->IE_5 = false; p->SEL_5 = false; p->SEL2_5 = false;
  
  p->DIR_6 = false; p->OUT_6 = false; p->IFG_6 = false; 
  p->IE_6 = false; p->SEL_6 = false; p->SEL2_6 = false;
  
  p->DIR_7 = false; p->OUT_7 = false; p->IFG_7 = false; 
  p->IE_7 = false; p->SEL_7 = false; p->SEL2_7 = false;
}

/* POWER UP CLEAR (PUC)      
 *
 * A PUC is always generated when a POR is generated, but a POR is not
 * generated by a PUC. The following events trigger a PUC:  
 *                                                
 * A POR signal                             
 * Watchdog timer expiration when in watchdog mode only
 * Watchdog timer security key violation          
 * A Flash memory security key violation        
 * A CPU instruct fetch from the peripheral address range 0h to 01FFh

void power_up_clear () {
  *P1OUT = *P1DIR = *P1IFG = *P1IE = *P1SEL = *P1SEL2 = *P1REN = 0;
}
 */
