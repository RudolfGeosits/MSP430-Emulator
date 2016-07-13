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

#ifndef _PORT_1_
#define _PORT_1_

#include "../cpu/registers.h"
#include "../utilities.h"

struct Port_1 {
  // Peripheral register pointers
  
  // Port 1        = r/w =   = reset? =
  uint8_t *IN;   /* r          -      */
  uint8_t *OUT;  /* r/w     unchanged */
  uint8_t *DIR;  /* r/w     PUC reset */
  uint8_t *IFG;  /* r/w     PUC reset */
  uint8_t *IES;  /* r/w     unchanged */
  uint8_t *IE;   /* r/w     PUC reset */
  uint8_t *SEL;  /* r/w     PUC reset */
  uint8_t *SEL2; /* r/w     PUC reset */
  uint8_t *REN;  /* r/w     PUC reset */

  // Peripherals activation flags (for emulator)
  bool DIR_0, OUT_0, IFG_0, IE_0, SEL_0, SEL2_0;
  bool DIR_1, OUT_1, IFG_1, IE_1, SEL_1, SEL2_1;
  bool DIR_2, OUT_2, IFG_2, IE_2, SEL_2, SEL2_2;
  bool DIR_3, OUT_3, IFG_3, IE_3, SEL_3, SEL2_3;
  bool DIR_4, OUT_4, IFG_4, IE_4, SEL_4, SEL2_4;
  bool DIR_5, OUT_5, IFG_5, IE_5, SEL_5, SEL2_5;
  bool DIR_6, OUT_6, IFG_6, IE_6, SEL_6, SEL2_6;
  bool DIR_7, OUT_7, IFG_7, IE_7, SEL_7, SEL2_7;

};

void setup_port_1(Emulator *emu);
void handle_port_1(Emulator *emu);

#endif
