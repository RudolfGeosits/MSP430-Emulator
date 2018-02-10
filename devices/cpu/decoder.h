/*
  This file is part of MSP430 Emulator
        
  MSP430 Emulator is free software: you can redistribute it and/or 
  modify it under the terms of the GNU General Public License as 
  published by the Free Software Foundation, either version 3 of the 
  License, or (at your option) any later version.            
                                                                               
  MSP430 Emulator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of               
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                
  GNU General Public License for more details.                                 
                                                                               
  You should have received a copy of the GNU General Public License            
  along with MSP430 Emulator.  
  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _DECODER_H_
#define _DECODER_H_

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "registers.h"
#include "../utilities.h"
#include "formatI.h"
#include "formatII.h"
#include "formatIII.h"

int16_t run_constant_generator(uint8_t source, uint8_t as_flag);

void decode(Emulator *emu, uint16_t instruction, bool disassemble);

uint16_t fetch(Emulator *emu);

enum { 
  WORD, 
  BYTE 
};

enum {
  EXECUTE = 0,
  DISASSEMBLE,
};

#endif
